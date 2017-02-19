#include <thread>
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

class OrdersRepositoryConcurrentTests : public ::testing::Test {
protected:
  OrdersRepositoryConcurrentTests() : dbName("taxiTestDB") {
    dbClient = client(uri{});
    db = dbClient[dbName];
  }

  virtual void SetUp() override {
    db.drop();
    users = db["users"];
    promos = db["promos"];

    promo1 = requests.addPromo(promos, 3, 2).to_string();
    promo2 = requests.addPromo(promos, 3, 2).to_string();
    user1 = oid().to_string();
    user2 = oid().to_string();

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

void func(OrdersRepositoryMongoDB* orders, const std::string& userId, const std::string& promoId) {
  for (int i = 0; i < 1000; ++i) {
    auto order = orders->createOrder(userId, promoId);
    orders->getOrder(order.getId());
    orders->cancelOrder(order.getId());
  }
}

//TODO Finish it
TEST_F(OrdersRepositoryConcurrentTests, createOrderWithPromo) {
  std::thread a(func, orders.get(), user1, promo1);
  std::thread b(func, orders.get(), user2, promo1);

  a.join();
  b.join();
}
