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
  OrdersRepositoryTests() : dbName("taxiTestDB") {
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

//TODO Finish it
TEST_F(OrdersRepositoryTests, createOrderWithPromo) {
  ;
}
