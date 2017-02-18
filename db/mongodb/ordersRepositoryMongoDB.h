#pragma once

#include <mongocxx/uri.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/database.hpp>
#include "db/ordersRepository.h"
#include "ordersSimpleRequests.h"

class OrdersRepositoryMongoDB : public OrdersRepository {
public:
  OrdersRepositoryMongoDB(const std::string &dbName);

  OrdersRepositoryMongoDB(const std::string &dbURI, const std::string &dbName);

  virtual Order createOrder();

  virtual Order createOrder(const std::string &userId, const std::string &promoId);

  virtual Order getOrder(const std::string &orderId);

  virtual void cancelOrder(const std::string &orderId);

  virtual void successOrder(const std::string &orderId);

  virtual ~OrdersRepositoryMongoDB() { ; }

private:
  mongocxx::pool clientsPool_;
  std::string dbName_;
  const OrdersSimpleRequests request_;
};
