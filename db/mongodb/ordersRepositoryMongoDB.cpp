#include <bsoncxx/oid.hpp>
#include <mongocxx/client.hpp>
#include "db/ordersExceptions.h"
#include "ordersRepositoryMongoDB.h"

using oid = bsoncxx::oid;

const std::string OrdersCollectionName("orders");
const std::string PromosCollectionName("promos");

OrdersRepositoryMongoDB::OrdersRepositoryMongoDB(const std::shared_ptr<mongocxx::pool>& clientsPool,
                                                 const std::string& database) :
        clientsPool_{clientsPool},
        dbName_(database),
        request_() {
  ;
}

Order OrdersRepositoryMongoDB::createOrder() {
  auto client = clientsPool_->acquire();
  auto db = (*client)[dbName_];
  auto orders = db[OrdersCollectionName];
  oid orderOId = request_.addOrder(orders);

  return Order(orderOId.to_string());
}

Order OrdersRepositoryMongoDB::createOrder(const std::string &userId, const std::string &promoId) {
  auto client = clientsPool_->acquire();
  auto db = (*client)[dbName_];
  auto promos = db[PromosCollectionName];
  oid orderOId;
  oid promoOId = getPromoOId(promoId);
  oid userOId = getUserOId(userId);

  PromoInfo promoInfo = request_.getPromoInfo(promos, promoOId);

  if (promoInfo.count == 0) {
    throw PromoExpired();
  }

  if (!request_.addUserToPromo(promos, promoOId, userOId, promoInfo.perUserCount, orderOId)) {
    if (!request_.usePromo(promos, promoOId, userOId, orderOId)) {
      throw PromoExpired();
    }
  }

  return Order(orderOId.to_string(), promoId, userId);
}

Order OrdersRepositoryMongoDB::getOrder(const std::string &orderId) {
  auto client = clientsPool_->acquire();
  auto db = (*client)[dbName_];
  auto promos = db[PromosCollectionName];
  auto orders = db[OrdersCollectionName];
  oid orderOId = getOrderOId(orderId);

  if (request_.hasOrderWithoutPromo(orders, orderOId)) {
    return Order(orderId);
  }

  OrderInfo info(request_.getOrderInfo(promos, orderOId));
  return Order(orderId, info.userId.to_string(), info.promoId.to_string());
}

void OrdersRepositoryMongoDB::cancelOrder(const std::string &orderId) {
  auto client = clientsPool_->acquire();
  auto db = (*client)[dbName_];
  auto promos = db[PromosCollectionName];
  auto orders = db[OrdersCollectionName];
  oid orderOId = getOrderOId(orderId);

  if (!request_.removeOrder(orders, orderOId)) {
    OrderInfo info(request_.getOrderInfo(promos, orderOId));
    request_.cancelOrder(promos, orderOId, info.userId);
  }
}

void OrdersRepositoryMongoDB::successOrder(const std::string &orderId) {
  auto client = clientsPool_->acquire();
  auto db = (*client)[dbName_];
  auto promos = db[PromosCollectionName];
  auto orders = db[OrdersCollectionName];
  oid orderOId = getOrderOId(orderId);

  if (!request_.removeOrder(orders, orderOId)) {
    OrderInfo info(request_.getOrderInfo(promos, orderOId));
    request_.successOrder(promos, orderOId, info.userId);
  }
}

oid OrdersRepositoryMongoDB::getUserOId(const std::string &userId) const {
  try {
    return oid(userId);
  } catch (const std::exception& e) {
    throw IncorrectUserId();
  }
}

oid OrdersRepositoryMongoDB::getOrderOId(const std::string &orderId) const {
  try {
    return oid(orderId);
  } catch (const std::exception& e) {
    throw IncorrectOrderId();
  }
}

oid OrdersRepositoryMongoDB::getPromoOId(const std::string &promoId) const {
  try {
    return oid(promoId);
  } catch (const std::exception& e) {
    throw IncorrectPromoId();
  }
}