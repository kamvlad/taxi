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
  OrdersRepositoryTests() {
    dbClient = client(uri{});
    db = dbClient["taxiTestDB"];
  }

  virtual void SetUp() {
    db.drop();
    users = db["users"];
    orders = db["orders"];
    promos = db["promos"];

    promo1 = requests.addPromo(promos, 3, 2).to_string();
    promo2 = requests.addPromo(promos, 3, 2).to_string();
    user1 = oid().to_string();
    user2 = oid().to_string();
  }

protected:
  collection users;
  collection orders;
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
  OrdersRepositoryMongoDB orders("taxiTestDB");
  //TODO Test is OrderId proper
  orders.createOrder(user1, promo1);
  orders.createOrder(user1, promo1);

  try {
    orders.createOrder(user1, promo1);
    FAIL();
  } catch (const PromoExpired& ) {
    ;
  }

  orders.createOrder(user2, promo1);

  try {
    orders.createOrder(user2, promo1);
    FAIL();
  } catch (const PromoExpired& ) {
    ;
  }
}

TEST_F(OrdersRepositoryTests, createOrderWithoutPromo) {
  OrdersRepositoryMongoDB orders("taxiTestDB");

  Order order = orders.createOrder();
}