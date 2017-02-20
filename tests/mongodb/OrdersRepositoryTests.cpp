#include <gtest/gtest.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <db/ordersExceptions.h>
#include <db/mongodb/ordersSimpleRequests.h>
#include <db/mongodb/ordersRepositoryMongoDB.h>

using namespace mongocxx;
using namespace bsoncxx;

class OrdersRepositoryTests : public ::testing::Test {
protected:
  OrdersRepositoryTests() : dbName("taxiTestDB") {
    dbClient = client(uri{"mongodb://localhost"});
    db = dbClient[dbName];
  }

  virtual void SetUp() override {
    db.drop();
    users = db["users"];
    promos = db["promos"];

    promo1 = requests.addPromo(promos, 3, 2).to_string();
    promo2 = requests.addPromo(promos, 3, 2).to_string();
    user1 = requests.addUser(users).to_string();
    user2 = requests.addUser(users).to_string();

    orders.reset(new OrdersRepositoryMongoDB(std::make_shared<mongocxx::pool>(), dbName));
  }

  virtual void TearDown() override {
    orders = nullptr;
  }

protected:
  const std::string dbName;
  std::unique_ptr<OrdersRepositoryMongoDB> orders;
  collection users;
  collection promos;
  client dbClient;
  database db;
  OrdersSimpleRequests requests;
  std::string promo1;
  std::string promo2;
  std::string user1;
  std::string user2;
};

TEST_F(OrdersRepositoryTests, createOrderWithPromo) {
  auto order = orders->createOrder(user1, promo1);
  orders->successOrder(order.getId());
  order = orders->createOrder(user1, promo1);
  orders->successOrder(order.getId());

  try {
    orders->createOrder(user1, promo1);
    FAIL();
  } catch (const PromoExpired& ) {
    ;
  }

  order = orders->createOrder(user2, promo1);
  orders->successOrder(order.getId());

  try {
    orders->createOrder(user2, promo1);
    FAIL();
  } catch (const PromoExpired& ) {
    ;
  }
}

TEST_F(OrdersRepositoryTests, createOrderWithoutPromo) {
  Order order = orders->createOrder();
  Order inDBOrder = orders->getOrder(order.getId());

  ASSERT_FALSE(inDBOrder.isPromoUsed());
  ASSERT_EQ(order.getId(), inDBOrder.getId());
}

TEST_F(OrdersRepositoryTests, getOrderWithPromo) {
  Order order = orders->createOrder(user1, promo1);
  Order inDBOrder = orders->getOrder(order.getId());

  ASSERT_TRUE(inDBOrder.isPromoUsed());
  ASSERT_EQ(order.getId(), inDBOrder.getId());
  ASSERT_EQ(promo1, inDBOrder.getPromoId());
  ASSERT_EQ(user1, inDBOrder.getUserId());
}

TEST_F(OrdersRepositoryTests, getOrderWithIncorrectData) {
  try {
    orders->getOrder(oid().to_string());
    FAIL();
  } catch (const OrderNotFound&) {
    ;
  }
}

TEST_F(OrdersRepositoryTests, expiredPromo) {
  auto order = orders->createOrder(user1, promo1);
  orders->successOrder(order.getId());
  order = orders->createOrder(user1, promo1);
  orders->successOrder(order.getId());
  order = orders->createOrder(user2, promo1);
  orders->successOrder(order.getId());

  try {
    orders->createOrder(user2, promo1);
    FAIL();
  } catch (const PromoExpired&) {

  }
}

TEST_F(OrdersRepositoryTests, expiredPromoForUser) {
  auto order = orders->createOrder(user1, promo1);
  orders->successOrder(order.getId());
  order = orders->createOrder(user1, promo1);
  orders->successOrder(order.getId());

  try {
    orders->createOrder(user1, promo1);
    FAIL();
  } catch (const PromoExpired&) {

  }
}

TEST_F(OrdersRepositoryTests, incorrectPromo) {
  try {
    orders->createOrder(user1, oid().to_string());
    FAIL();
  } catch (const PromoNotFound&) {
    ;
  }
}

TEST_F(OrdersRepositoryTests, cancelOrderWithPromo1) {
  Order order1 = orders->createOrder(user1, promo1);
  orders->successOrder(order1.getId());
  Order order2 = orders->createOrder(user1, promo1);
  orders->cancelOrder(order2.getId());
  Order order3 = orders->createOrder(user1, promo1);

  try {
    orders->getOrder(order1.getId());
    FAIL();
  } catch (const OrderNotFound&) {

  }
}

TEST_F(OrdersRepositoryTests, cancelOrderWithPromo2) {
  Order order1 = orders->createOrder(user1, promo1);
  orders->successOrder(order1.getId());
  Order order2 = orders->createOrder(user1, promo1);
  Order order3 = orders->createOrder(user2, promo1);
  orders->cancelOrder(order2.getId());
  orders->successOrder(order3.getId());
  Order order4 = orders->createOrder(user2, promo1);

  try {
    orders->getOrder(order2.getId());
    FAIL();
  } catch (const OrderNotFound&) {

  }
}

TEST_F(OrdersRepositoryTests, cancelOrderWithoutPromo) {
  Order order1 = orders->createOrder();
  orders->cancelOrder(order1.getId());

  try {
    orders->getOrder(order1.getId());
    FAIL();
  } catch (const OrderNotFound&) {

  }
}

TEST_F(OrdersRepositoryTests, successOrderWithPromo1) {
  Order order1 = orders->createOrder(user1, promo1);
  orders->successOrder(order1.getId());
  Order order2 = orders->createOrder(user1, promo1);
  orders->successOrder(order2.getId());

  try {
    orders->getOrder(order1.getId());
    FAIL();
  } catch (const OrderNotFound&) {

  }

  try {
    orders->getOrder(order2.getId());
    FAIL();
  } catch (const OrderNotFound&) {

  }

  try {
    orders->createOrder(user1, promo1);
  } catch (const PromoExpired&) {

  }
}

TEST_F(OrdersRepositoryTests, successOrderWithPromo2) {
  Order order1 = orders->createOrder(user1, promo1);
  orders->successOrder(order1.getId());
  Order order2 = orders->createOrder(user1, promo1);
  orders->successOrder(order2.getId());
  Order order3 = orders->createOrder(user2, promo1);
  orders->successOrder(order3.getId());

  try {
    orders->createOrder(user2, promo1);
    FAIL();
  } catch (const PromoExpired&) {

  }
}

TEST_F(OrdersRepositoryTests, successOrderWithoutPromo) {
  Order order1 = orders->createOrder();
  orders->successOrder(order1.getId());

  try {
    orders->getOrder(order1.getId());
    FAIL();
  } catch (const OrderNotFound&) {

  }
}

TEST_F(OrdersRepositoryTests, alreadyExistsOrder) {
  orders->createOrder(user1, promo1);

  try {
    orders->createOrder(user1, promo1);
    FAIL();
  } catch (const OrderExists&) {

  }
}

TEST_F(OrdersRepositoryTests, userNotFound) {
  try {
    orders->createOrder(oid().to_string(), promo1);
    FAIL();
  } catch (const UserNotFound&) {

  }
}