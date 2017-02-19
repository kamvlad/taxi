#include "db/ordersExceptions.h"
#include "ordersSimpleRequests.h"

using Collection = mongocxx::collection;
using oid =  bsoncxx::oid;
using Document = bsoncxx::document::value;
using Builder = bsoncxx::builder::stream::document;
using namespace bsoncxx::builder::stream;
namespace Options = mongocxx::options;

oid OrdersSimpleRequests::addPromo(Collection &promos, int32_t count, int32_t perUserCount) const {
  Document request =
          Builder{} << "count" << count
                    << "perUserCount" << perUserCount
                    << "lockedCount" << 0
                    << "orders" << open_array << close_array
                    << "users" << open_array << close_array
                    << finalize;
  auto r = promos.insert_one(request.view()).value();
  return r.inserted_id().get_oid().value;
}

oid OrdersSimpleRequests::addUser(Collection &users) const {
  Document request = Builder{} << finalize;
  auto r = users.insert_one(request.view()).value();
  return r.inserted_id().get_oid().value;
}

PromoInfo OrdersSimpleRequests::getPromoInfo(Collection &promos, const oid &promoId) const {
  Document request =
          Builder{} << "_id" << promoId << finalize;
  Document projectionOption = Builder{} << "perUserCount" << 1 << "count" << 1 << finalize;
  auto options = Options::find();
  options.projection(projectionOption.view());

  auto r = promos.find_one(request.view(), options);
  if (r) {
    auto rv =  r->view();
    return PromoInfo(rv["count"].get_int32(), rv["perUserCount"].get_int32());
  } else {
    throw PromoNotFound();
  }
}

bool OrdersSimpleRequests::addUserToPromo(Collection &promos, const oid &promoId, const oid &userId,
                                          int32_t perUserCount, const oid& orderId) const {
  Document request =
          Builder{} << "_id" << promoId
                    << "count" << open_document
                               << "$gt" << 0
                               << close_document
                    << "users" << open_document << "$not" << open_document
                                                          << "$elemMatch" << open_document
                                                                          << "_id" << userId << close_document
                                                          << close_document << close_document << finalize;
  Document update =
          Builder{} << "$inc" << open_document
                    << "count" << -1 << "lockedCount" << 1 << close_document
                    << "$push" << open_document
                                  << "users" << open_document
                                             << "_id" << userId << "count" << (perUserCount - 1) << close_document
                                  << "orders" << open_document
                                              << "_id" << orderId << "userId" << userId << close_document
                               << close_document << finalize;
  auto r = promos.update_one(request.view(), update.view()).value();
  return (r.matched_count() == 1) && (r.modified_count() == 1);
}

bool OrdersSimpleRequests::usePromo(Collection &promos, const oid &promoId, const oid &userId,
                                    const oid& orderId) const {
  Document request =
          Builder{} << "_id" << promoId
                    << "count" << open_document
                                  << "$gt" << 0 << close_document
                                  << "users" << open_document
                                             << "$elemMatch" << open_document
                                                             << "_id" << userId
                                                             << "count" << open_document << "$gt" << 0 << close_document
                                             << close_document
                              << close_document << finalize;
  Document update =
          Builder{} << "$inc" << open_document
                    << "count" << -1 << "lockedCount" << 1 << "users.$.count" << -1 << close_document
                    << "$push" << open_document << "orders" << open_document
                                                            << "_id" << orderId
                                                            << "userId" << userId << close_document
                    << close_document << finalize;
  auto r = promos.update_one(request.view(), update.view()).value();
  return (r.matched_count() == 1) && (r.modified_count() == 1);
}

bool OrdersSimpleRequests::successOrder(Collection &promos, const oid &orderId, const oid &userId) const {
  Document request =
          Builder{} << "orders" << open_document << "$elemMatch" << open_document
                                                                 << "_id" << orderId
                                                                 << "userId" << userId
                                                                 << close_document
                                << close_document << finalize;
  Document update =
          Builder{} << "$inc" << open_document << "lockedCount" << -1 << close_document
                    << "$pull" << open_document
                               << "orders" << open_document
                                           << "_id" << orderId
                                           << "userId" << userId
                                           << close_document
                               << close_document << finalize;
  auto r = promos.update_one(request.view(), update.view()).value();
  return (r.matched_count() == 1) && (r.modified_count() == 1);
}

bool OrdersSimpleRequests::cancelOrder(Collection &promos, const oid &orderId, const oid &userId) const {
  Document request =
          Builder{} << "orders" << open_document << "$elemMatch" << open_document
                                                                 << "_id" << orderId
                                                                 << "userId" << userId
                                                                 << close_document
                                << close_document
                    << "users" << open_document << "$elemMatch" << open_document
                                                                << "_id" << userId
                                                                << close_document
                               << close_document << finalize;
  Document update =
          Builder{} << "$inc" << open_document << "lockedCount" << -1
                              << "count" << 1
                              << "users.$.count" << 1
                              << close_document
                    << "$pull" << open_document
                               << "orders" << open_document
                                           << "_id" << orderId
                                           << "userId" << userId
                                           << close_document
                               << close_document << finalize;
  auto r = promos.update_one(request.view(), update.view()).value();
  return (r.matched_count() == 1) && (r.modified_count() == 1);
}

bool OrdersSimpleRequests::removePromo(Collection &promos, const oid &orderId, const oid &userId) const {
  Document request =
          Builder{} << "count" << 0
                    << "lockedCount" << 1
                    << "orders" << open_document << "$elemMatch" << open_document
                                                                 << "_id" << orderId
                                                                 << "userId" << userId
                                                                 << close_document
                                << close_document << finalize;
  auto r = promos.delete_one(request.view()).value();
  return (r.deleted_count() == 1);
}

oid OrdersSimpleRequests::addOrder(Collection &orders) const {
  Document request = Builder{} << finalize;
  auto r = orders.insert_one(request.view()).value();
  return r.inserted_id().get_oid().value;
}

bool OrdersSimpleRequests::removeOrder(Collection &orders, const oid &orderId) const {
  Document request =
          Builder{} << "_id" << orderId << finalize;
  auto r = orders.delete_one(request.view()).value();
  return (r.deleted_count() == 1);
}

bool OrdersSimpleRequests::hasOrderWithoutPromo(Collection &orders, const oid &orderId) const {
  Document request =
          Builder{}  << "_id" << orderId << finalize;
  Document projectionOption = Builder{} << "userId" << 1 << finalize;
  auto options = Options::find();
  options.projection(projectionOption.view());

  auto r = orders.find_one(request.view(), options);
  return bool(r);
}

OrderInfo OrdersSimpleRequests::getOrderInfo(Collection &promos, const oid &orderId) const {
  Document request =
          Builder{} << "orders" << open_document << "$elemMatch" << open_document
                                                                 << "_id" << orderId
                                                                 << close_document
                                << close_document << finalize;
  Document projectionOption = Builder{} << "orders.$.userId" << 1 << finalize;
  auto options = Options::find();
  options.projection(projectionOption.view());

  auto r = promos.find_one(request.view(), options);
  if (r) {
    auto doc = r->view();
    auto orders = doc["orders"].get_array();
    auto order = orders.value[0].get_document().view();
    auto promoId = doc["_id"].get_oid().value;
    auto userId = order["userId"].get_oid().value;

    return OrderInfo(userId, promoId);
  } else {
    throw OrderNotFound();
  }
}

Promo OrdersSimpleRequests::getPromo(Collection &promos, const oid &promoId) const {
  Document request =
          Builder{} << "_id" << promoId << finalize;

  auto r = promos.find_one(request.view());
  if (r) {
    return Promo(*r);
  } else {
    throw PromoNotFound();
  }
}