#include <gtest/gtest.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <db/ordersExceptions.h>
#include <db/mongodb/ordersSimpleRequests.h>

using namespace mongocxx;
using namespace bsoncxx;

instance inst{};

class OrdersSimpleRequestsTests : public ::testing::Test {
protected:
  OrdersSimpleRequestsTests() {
    dbClient = client(uri{});
    db = dbClient["taxiTestDB"];
  }

  virtual void SetUp() {
    db.drop();
    users = db["users"];
    orders = db["orders"];
    promos = db["promos"];
  }

protected:
  collection users;
  collection orders;
  collection promos;
  client dbClient;
  database db;
  OrdersSimpleRequests requests;
};

TEST_F(OrdersSimpleRequestsTests, promoCounterPerUser) {
  oid promoId = requests.addPromo(promos, 20, 10);
  PromoInfo promoInfo = requests.getPromoCountPerUser(promos, promoId);
  ASSERT_EQ(10, promoInfo.perUserCount);
  ASSERT_EQ(20, promoInfo.count);

  try {
    requests.getPromoCountPerUser(promos, oid());
    FAIL();
  } catch (const PromoNotFound &) {
    ;
  }
}

TEST_F(OrdersSimpleRequestsTests, addUserToPromo) {
  oid promoId = requests.addPromo(promos, 3, 2);
  oid userId = requests.addUser(users);

  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 2, oid()));
  ASSERT_FALSE(requests.addUserToPromo(promos, promoId, userId, 2, oid()));

  Promo promo(requests.getPromo(promos, promoId));
  ASSERT_EQ(1, promo.getLockedCount());
  ASSERT_EQ(2, promo.getCount());
  ASSERT_EQ(1, promo.getUserPromoCount(userId));
}

TEST_F(OrdersSimpleRequestsTests, usePromo) {
  oid promoId = requests.addPromo(promos, 3, 2);
  oid userId = requests.addUser(users);

  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 2, oid()));
  ASSERT_TRUE(requests.usePromo(promos, promoId, userId, oid()));
  ASSERT_FALSE(requests.usePromo(promos, promoId, userId, oid()));

  Promo promo(requests.getPromo(promos, promoId));
  ASSERT_EQ(2, promo.getLockedCount());
  ASSERT_EQ(1, promo.getCount());
  ASSERT_EQ(0, promo.getUserPromoCount(userId));
}

TEST_F(OrdersSimpleRequestsTests, removePromo) {
  oid promoId = requests.addPromo(promos, 1, 1);
  oid promoId2 = requests.addPromo(promos, 2, 1);
  oid userId = requests.addUser(users);
  oid orderId1;
  oid orderId2;

  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 1, orderId1));
  ASSERT_TRUE(requests.addUserToPromo(promos, promoId2, userId, 1, orderId2));
  ASSERT_TRUE(requests.removePromo(promos, promoId, orderId1, userId));
  ASSERT_FALSE(requests.removePromo(promos, promoId2, orderId2, userId));
}

TEST_F(OrdersSimpleRequestsTests, successOrder) {
  oid promoId = requests.addPromo(promos, 1, 1);
  oid userId = requests.addUser(users);

  oid orderId;
  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 1, orderId));
  ASSERT_TRUE(requests.successOrder(promos, orderId, promoId, userId));
  Promo promo = requests.getPromo(promos, promoId);

  ASSERT_EQ(0, promo.getLockedCount());
  ASSERT_EQ(0, promo.getUserPromoCount(userId));
  ASSERT_FALSE(promo.hasOrder(orderId));
}

TEST_F(OrdersSimpleRequestsTests, successOrderWithIncorrectData) {
  oid promoId = requests.addPromo(promos, 1, 1);
  oid userId = requests.addUser(users);

  oid orderId;
  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 1, orderId));
  ASSERT_FALSE(requests.successOrder(promos, orderId, oid(), userId));
  ASSERT_FALSE(requests.successOrder(promos, oid(), promoId, userId));
  ASSERT_FALSE(requests.successOrder(promos, orderId, promoId, oid()));

  Promo promo = requests.getPromo(promos, promoId);
  ASSERT_EQ(1, promo.getLockedCount());
  ASSERT_TRUE(promo.hasOrder(orderId));
}

TEST_F(OrdersSimpleRequestsTests, cancelOrder) {
  oid promoId = requests.addPromo(promos, 1, 1);
  oid userId = requests.addUser(users);

  oid orderId;
  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 1, orderId));
  ASSERT_TRUE(requests.cancelOrder(promos, orderId, promoId, userId));

  Promo promo = requests.getPromo(promos, promoId);
  ASSERT_EQ(0, promo.getLockedCount());
  ASSERT_EQ(1, promo.getCount());
  ASSERT_EQ(1, promo.getUserPromoCount(userId));
}

TEST_F(OrdersSimpleRequestsTests, cancelOrderWithIncorretData) {
  oid promoId = requests.addPromo(promos, 1, 1);
  oid userId = requests.addUser(users);

  oid orderId;
  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 1, orderId));
  ASSERT_FALSE(requests.cancelOrder(promos, oid(), promoId, userId));
  ASSERT_FALSE(requests.cancelOrder(promos, orderId, oid(), userId));
  ASSERT_FALSE(requests.cancelOrder(promos, orderId, promoId, oid()));

  Promo promo = requests.getPromo(promos, promoId);
  ASSERT_EQ(1, promo.getLockedCount());
  ASSERT_EQ(0, promo.getCount());
  ASSERT_EQ(0, promo.getUserPromoCount(userId));
}

TEST_F(OrdersSimpleRequestsTests, getOrderUser) {
  oid promoId = requests.addPromo(promos, 3, 2);
  oid userId1 = requests.addUser(users);
  oid userId2 = requests.addUser(users);

  oid orderId1;
  oid orderId2;
  oid orderId3;

  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId1, 2, orderId1));
  ASSERT_TRUE(requests.usePromo(promos, promoId, userId1, orderId2));
  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId2, 2, orderId3));

  ASSERT_EQ(userId1, requests.getOrderUser(promos, promoId, orderId1));
  ASSERT_EQ(userId1, requests.getOrderUser(promos, promoId, orderId2));
  ASSERT_EQ(userId2, requests.getOrderUser(promos, promoId, orderId3));
}

TEST_F(OrdersSimpleRequestsTests, getUserWithWrongData) {
  oid promoId = requests.addPromo(promos, 3, 2);
  oid userId = requests.addUser(users);
  oid orderId;

  ASSERT_TRUE(requests.addUserToPromo(promos, promoId, userId, 2, orderId));

  try {
    ASSERT_EQ(userId, requests.getOrderUser(promos, oid(), orderId));
    FAIL();
  } catch (const OrderNotFound&) {
    ;
  }

  try {
    ASSERT_EQ(userId, requests.getOrderUser(promos, promoId, oid()));
    FAIL();
  } catch (const OrderNotFound&) {

  }
}

TEST_F(OrdersSimpleRequestsTests, addAndRemoveOrder) {
  oid orderId = requests.addOrder(orders);

  ASSERT_TRUE(requests.removeOrder(orders, orderId));

  try {
    requests.checkOrder(orders, orderId);
    FAIL();
  } catch (const OrderNotFound&) {

  }
}