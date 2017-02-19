#include <memory>
#include <sstream>
#include <fastcgi2/logger.h>
#include <fastcgi2/config.h>
#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include "db/DatabaseComponent.h"
#include "ordersRepositoryMongoDB.h"
#include "mongodbComponentConfig.h"


class MongoDBComponent : virtual public fastcgi::Component, public DatabaseComponent {
public:
  MongoDBComponent(fastcgi::ComponentContext *context) :
          fastcgi::Component(context), logger_(nullptr), ordersRepository_(), clientsPool_() {
  }

  virtual ~MongoDBComponent() {
  }

public:
  virtual void onLoad() override {
    MongoDBComponentConfig config(context()->getComponentXPath(), context()->getConfig());
    initLogger(config);

    try {
      createClientsPool(config);
    } catch (const std::exception& e) {
      logger_->error("Create mongodb connection pool error : %s", e.what());
      throw;
    }

    logger_->info("MongoDB component has started...");
  }

  virtual std::shared_ptr<OrdersRepository> getOrdersRepository() override {
    return ordersRepository_;
  }

  virtual void onUnload() override {
    ;
  }

private:
  void initLogger(const MongoDBComponentConfig& config) {
    logger_ = context()->findComponent<fastcgi::Logger>(config.getLogger());
    if (!logger_) {
      throw std::runtime_error("cannot get component " + config.getLogger());
    }
  }

  void createClientsPool(const MongoDBComponentConfig& config) {
    clientsPool_ = std::make_shared<mongocxx::pool>(config.getMongoDBURI(logger_));
    ordersRepository_.reset(new OrdersRepositoryMongoDB(clientsPool_, config.getDatabaseName()));
  }

private:
  fastcgi::Logger *logger_;
  std::shared_ptr<OrdersRepository> ordersRepository_;
  std::shared_ptr<mongocxx::pool> clientsPool_;
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("MongoDBComponent_factory", MongoDBComponent)
FCGIDAEMON_REGISTER_FACTORIES_END()