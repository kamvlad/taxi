#include <gtest/gtest.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

using namespace mongocxx;
using namespace bsoncxx;

instance inst{};

class MongoDBTest : public ::testing::Test {
protected:
  virtual void SetUp() {
		dbClient = client(uri{});
    db = dbClient["taxiTestDB"];
		db.drop();

		users = db["users"];
		orders = db["orders"];
		promos = db["promos"];
  }

	types::b_oid addPromo(unsigned count, unsigned perUserCount) {
		auto builder = bsoncxx::builder::stream::document{};
		document::value documentValue = builder << builder::stream::finalize;
	}
protected:
	collection users;
	collection orders;
	collection promos;
  client dbClient;
  database db;
};



TEST_F(MongoDBTest, Basic) {
	auto builder = bsoncxx::builder::stream::document{};
	document::value documentValue = builder << builder::stream::finalize;


	try {
		result::insert_one r = users.insert_one(documentValue.view()).value();
		result::insert_one r2 = orders.insert_one(documentValue.view()).value();
		types::b_oid oid = r.inserted_id().get_oid();
	} catch (const std::logic_error& e) {
		//TODO Here
	}


    //auto builder = bsoncxx::builder::stream::document{};
//    builder << "a" << "b";
//
//    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;
//    std::cout << bsoncxx::to_json(doc_value) << std::endl;
	//mongocxx::uri uri("mongodb://localhost:27017");
	//mongocxx::client dbClient(uri);
//	dbClient["taxiTestDB"];
}

