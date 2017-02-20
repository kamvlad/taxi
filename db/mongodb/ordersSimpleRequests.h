#pragma once

#include <bsoncxx/oid.hpp>
#include <mongocxx/collection.hpp>
#include "promo.h"

struct PromoResponse {
  const int32_t count;
  const int32_t perUserCount;
	const bool hasUser;

  PromoResponse(int32_t count_, int32_t perUserCount_, bool hasUser_) : count(count_), perUserCount(perUserCount_),
																																		hasUser(hasUser_) { ; }
};

struct OrderInfo {
  using oid =  bsoncxx::oid;

  const oid userId;
  const oid promoId;

  OrderInfo(const oid& userId_, const oid& promoId_) : userId(userId_), promoId(promoId_) { ; }
  bool operator==(const OrderInfo& o) const { return (userId == o.userId) && (promoId == o.promoId); }
};

class OrdersSimpleRequests {
  using Collection = mongocxx::collection;
  using oid =  bsoncxx::oid;
public:
  oid addPromo(Collection &promos, int32_t count, int32_t perUserCount) const;

  oid addUser(Collection &users) const;

	bool hasUser(Collection &users, const oid &userId) const;

  PromoResponse getPromoInfo(Collection &promos, const oid &promoId, const oid& userId) const;

  bool addUserToPromo(Collection &promos, const oid &promoId, const oid &userId, int32_t perUserCount,
                      const oid& orderId) const;

  bool usePromo(Collection &promos, const oid &promoId, const oid &userId, const oid& orderId) const;

  bool successOrder(Collection &promos, const oid &orderId, const oid& userId) const;

  bool cancelOrder(Collection &promos, const oid &orderId, const oid& userId) const;

  bool removePromo(Collection &promos, const oid &orderId, const oid &userId) const;

  oid addOrder(Collection &orders) const;

  bool removeOrder(Collection &orders, const oid &orderId) const;

  bool hasOrderWithoutPromo(Collection &orders, const oid &orderId) const;

  OrderInfo getOrderInfo(Collection &promos, const oid &orderId) const;

  Promo getPromo(Collection &promos, const oid &promoId) const;
};