#include <memory>
#include <thread>
#include <sstream>
#include <fastcgi2/logger.h>
#include <fastcgi2/config.h>
#include <fastcgi2/stream.h>
#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <bsoncxx/json.hpp>
#include "i18n/Messages.h"
#include "data/documentPresentation.h"
#include "db/DatabaseComponent.h"
#include "data/jsonPresentation.h"
#include "common/httpStatuses.h"
#include "common/requestHandler.h"
#include "common/exceptions.h"
#include "ordersHandlers.cpp"

class OrdersService : virtual public fastcgi::Component, virtual public fastcgi::Handler {
public:
  OrdersService(fastcgi::ComponentContext *context) :
          fastcgi::Component(context), logger_(nullptr), handlers_(nullptr) {
  }

  virtual ~OrdersService() {
  }

public:
  virtual void onLoad() {
    auto config = context()->getConfig();
    auto xpath = context()->getComponentXPath();

    const std::string loggerComponentName = config->asString(xpath + "/logger");
    logger_ = context()->findComponent<fastcgi::Logger>(loggerComponentName);
    if (!logger_) {
      throw std::runtime_error("cannot get component " + loggerComponentName);
    }

    const std::string databaseComponentName = config->asString(xpath + "/database");
    auto database = context()->findComponent<DatabaseComponent>(databaseComponentName);

    if (!database) {
      logger_->error("ERROR orders service cannot find database component");
      throw std::runtime_error("cannot get component " + databaseComponentName);
    }

    auto ordersRepository = database->getOrdersRepository();

    if (!ordersRepository) {
      logger_->error("ERROR orders repository is null");
      throw std::runtime_error("orders repository is null");
    }

    registerDocumentsPresentations();
    createRequestsHandlers(ordersRepository);
  }

  virtual void onUnload() {
    ;
  }

  virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
    const std::string lang("en_EN");
    fastcgi::RequestStream output(request);
    auto method = parseRequestType(request->getRequestMethod());
    auto presentation = getDocumentPresentation(request->getContentType());

    if (method != RequestMethod::GET && presentation == nullptr) {
      request->setStatus(HttpStatus::UnsupportedMediaType);
      output << getMessage(lang, "_CONTENT_TYPE_NOT_SUPPORTED") << "\r\n";
      return;
    }

    std::string requestBody;
    request->requestBody().toString(requestBody);

    Document requestContent;
    try {
      if (method != RequestMethod::GET) {
        requestContent = std::move(presentation->fromString(requestBody));
      }
    } catch (const std::exception &e) {
      request->setStatus(HttpStatus::BadRequest);
      output << e.what() << "\r\n";
      return;
    }

    try {
      std::string orderId = getOrderID(request->getScriptName());
      Document result = handlers_->handleRequest(method, orderId, requestContent);

      output << presentation->toString(result);
      request->setStatus(HttpStatus::OK);
    } catch (const BusinessLogicException &e) {
      Document error;
      error["error"] = getMessage(lang, e.what());
      error["errorCode"] = std::to_string(e.getCode());
      output << presentation->toString(error);
      request->setStatus(HttpStatus::OK);
    } catch (const std::invalid_argument &e) {
      request->setStatus(HttpStatus::BadRequest);
      output << e.what() << "\r\n";
      return;
    } catch (const std::exception &e) {
      logger_->error("ERROR %s", e.what());
      request->setStatus(HttpStatus::InternalServerError);
    } catch (...) {
      logger_->error("ERROR Unexpected exception");
      request->setStatus(HttpStatus::InternalServerError);
    }
  }
private:
  void registerDocumentsPresentations() {
    presentations_["application/json"] = std::unique_ptr<DocumentPresentation>(new jsonPresentation());
  }

  void createRequestsHandlers(const std::shared_ptr<OrdersRepository>& ordersRepository) {
    handlers_.reset(new CreateOrderHandler(ordersRepository));
    handlers_->setNext(new GetOrderHandler(ordersRepository))
            ->setNext(new SetOrderStatusHandler(ordersRepository));

  }

  std::string getOrderID(const std::string &str) {
    if (str.size() > 8) {
      return str.substr(8);
    } else {
      return "";
    }
  }

  const DocumentPresentation* getDocumentPresentation(const std::string& contentType) const {
    auto it = presentations_.find(contentType);
    if (it != presentations_.end()) {
      return it->second.get();
    } else {
      return nullptr;
    };
  };
private:
  fastcgi::Logger *logger_;
  std::unique_ptr<RequestHandler> handlers_;
  std::map<std::string, std::unique_ptr<const DocumentPresentation>> presentations_;
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("OrdersService_factory", OrdersService)
FCGIDAEMON_REGISTER_FACTORIES_END()