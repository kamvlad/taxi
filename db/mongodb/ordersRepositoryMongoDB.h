#pragma once

#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/database.hpp>
#include "db/ordersRepository.h"
#include "ordersSimpleRequests.h"

class OrdersRepositoryMongoDB : public OrdersRepository {
public:
  OrdersRepositoryMongoDB(const std::shared_ptr<mongocxx::pool>& clientsPool, const std::string& database);

  virtual Order createOrder() override;

  virtual Order createOrder(const std::string &userId, const std::string &promoId) override;

  virtual Order getOrder(const std::string &orderId) override;

  virtual void cancelOrder(const std::string &orderId) override;

  virtual void successOrder(const std::string &orderId) override;

  virtual ~OrdersRepositoryMongoDB() { ; }
private:
  bsoncxx::oid getUserOId(const std::string &userId) const;
  bsoncxx::oid getOrderOId(const std::string &orderId) const;
  bsoncxx::oid getPromoOId(const std::string &promoId) const;
private:
  std::shared_ptr<mongocxx::pool> clientsPool_;
  const std::string dbName_;
  const OrdersSimpleRequests request_;
};
