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
    const std::string loggerComponentName = context()->getConfig()->asString(
            context()->getComponentXPath() + "/logger");
    logger_ = context()->findComponent<fastcgi::Logger>(loggerComponentName);
    if (!logger_) {
      throw std::runtime_error("cannot get component " + loggerComponentName);
    }

    handlers_.reset(new CreateOrderHandler());
    //TODO maybe change interface
    handlers_->setNext(new GetOrderHandler())->setNext(new SetOrderStatusHandler());
    logger_->debug("onLoad");
    //TODO no hardcode in db connection!!!!
  }

  virtual void onThreadStart() {
    ;
  }

  virtual void onUnload() {
    ;
  }

  virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
    RequestMethod method = parseRequestType(request->getRequestMethod());
    fastcgi::RequestStream output(request);
    if (method != RequestMethod::GET && request->getContentType() != "application/json") {
      request->setStatus(HttpStatus::UnsupportedMediaType);
      output << "Only json content type is allowed\r\n";
      return;
    }

    using json = nlohmann::json;
    std::string requestBody;
    request->requestBody().toString(requestBody);

    json jsonRequest;
    try {
      if (method != RequestMethod::GET) {
        jsonRequest = json::parse(requestBody);
      }
    } catch (const std::invalid_argument &e) {
      request->setStatus(HttpStatus::BadRequest);
      output << "JSON: " << e.what() << "\r\n";
      return;
    }

    try {
      std::string orderId = getOrderID(request->getScriptName());

      std::string result(handlers_->handleRequest(method, orderId, jsonRequest).dump());
      output << result;
      request->setStatus(HttpStatus::OK);
    } catch (const BusinessLogicException &e) {
      output << R"({"error":")" << e.what()
             << R"(","errorCode":)" << e.getCode() << "}";
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
  std::string getOrderID(const std::string &str) {
    if (str.size() > 8) {
      return str.substr(8);
    } else {
      return "";
    }
  }

private:
  fastcgi::Logger *logger_;
  std::unique_ptr<JSONRequestHandler> handlers_;
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
  FCGIDAEMON_ADD_DEFAULT_FACTORY("OrdersService_factory", OrdersService)
FCGIDAEMON_REGISTER_FACTORIES_END()