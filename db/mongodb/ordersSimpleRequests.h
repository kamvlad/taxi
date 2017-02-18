#pragma once

#include <bsoncxx/oid.hpp>
#include <mongocxx/collection.hpp>
#include "promo.h"

struct PromoInfo {
  const int32_t count;
  const int32_t perUserCount;

  PromoInfo(int32_t count_, int32_t perUserCount_) : count(count_), perUserCount(perUserCount_) { ; }
};

class OrdersSimpleRequests {
  using Collection = mongocxx::collection;
  using oid =  bsoncxx::oid;
public:
  oid addPromo(Collection &promos, int32_t count, int32_t perUserCount) const;

  oid addUser(Collection &users) const;

  PromoInfo getPromoCountPerUser(Collection &promos, const oid &promoId) const;

  bool addUserToPromo(Collection &promos, const oid &promoId, const oid &userId, int32_t perUserCount,
                      const oid& orderId) const;

  bool usePromo(Collection &promos, const oid &promoId, const oid &userId, const oid& orderId) const;

  bool successOrder(Collection &promos, const oid &orderId, const oid &promoId, const oid &userId) const;

  bool cancelOrder(Collection &promos, const oid &orderId, const oid &promoId, const oid &userId) const;

  bool removePromo(Collection &promos, const oid &promoId, const oid &orderId, const oid &userId) const;

  oid addOrder(Collection &orders) const;

  bool removeOrder(Collection &orders, const oid &orderId) const;

  void checkOrder(Collection &orders, const oid &orderId) const;

  oid getOrderUser(Collection &promos, const oid &promoId, const oid &orderId) const;

  Promo getPromo(Collection &promos, const oid &promoId) const;
};