#include <bsoncxx/oid.hpp>
#include <mongocxx/client.hpp>
#include "db/ordersExceptions.h"
#include "ordersRepositoryMongoDB.h"

using oid = bsoncxx::oid;

OrdersRepositoryMongoDB::OrdersRepositoryMongoDB(const std::string &dbName) :
        request_{},
        clientsPool_{},
        dbName_(dbName) {

}

//TODO Fix it

OrdersRepositoryMongoDB::OrdersRepositoryMongoDB(const std::string &dbURI, const std::string &dbName) :
        request_{},
        clientsPool_{mongocxx::uri{dbURI}},
        dbName_(dbName) {

}

Order OrdersRepositoryMongoDB::createOrder() {
  auto client = clientsPool_.acquire();
  auto db = (*client)[dbName_];
  auto orders = db["orders"];
  oid orderOId = request_.addOrder(orders);

  return Order(orderOId.to_string());
}

Order OrdersRepositoryMongoDB::createOrder(const std::string &userId, const std::string &promoId) {
  auto client = clientsPool_.acquire();
  auto db = (*client)[dbName_];
  auto promos = db["promos"];
  oid orderOId;
  oid promoOId(promoId);
  oid userOId(userId);

  PromoInfo promoInfo = request_.getPromoCountPerUser(promos, promoOId);

  if (promoInfo.count == 0) {
    throw PromoExpired();
  }

  if (!request_.addUserToPromo(promos, promoOId, userOId, promoInfo.perUserCount, orderOId)) {
    if (!request_.usePromo(promos, promoOId, userOId, orderOId)) {
      throw PromoExpired();
    }
  }

  //TODO Create proper orderId
  return Order(orderOId.to_string(), promoId, userId);
}

Order OrdersRepositoryMongoDB::getOrder(const std::string &orderId) {

}

void OrdersRepositoryMongoDB::cancelOrder(const std::string &orderId) {

}

void OrdersRepositoryMongoDB::successOrder(const std::string &orderId) {

}