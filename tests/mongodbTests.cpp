#include <gtest/gtest.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <db/OrdersRepositoryMongoDB.h>

using namespace mongocxx;
using namespace bsoncxx;
using namespace bsoncxx::types;

instance inst{};

class MongoDBTest : public ::testing::Test {
protected:
    MongoDBTest() {
        dbClient = client(uri{});
        db = dbClient["taxiTestDB"];
    }

    virtual void SetUp() {
        db.drop();
        users = db["users"];
        orders = db["orders"];
        promos = db["promos"];
        usersIds.clear();
        promosIds.clear();
    }

    oid addPromo(int32_t count, int32_t perUserCount) {
        auto builder = bsoncxx::builder::stream::document{};
        document::value documentValue =
                builder << "count" << count
                        << "perUserCount" << perUserCount
                        << "lockedCount" << 0
                        << "orders" << builder::stream::open_array << builder::stream::close_array
                        << "users" << builder::stream::open_array << builder::stream::close_array
                        << builder::stream::finalize;
        result::insert_one r = promos.insert_one(documentValue.view()).value();
        return r.inserted_id().get_oid().value;
    }

    oid addUser() {
        auto builder = bsoncxx::builder::stream::document{};
        document::value documentValue = builder << builder::stream::finalize;
        result::insert_one r = users.insert_one(documentValue.view()).value();
        usersIds.push_back(r.inserted_id().get_oid());
        return r.inserted_id().get_oid().value;
    }
protected:
    collection users;
	collection orders;
	collection promos;
    client dbClient;
    database db;
    std::vector<b_oid> usersIds;
    std::vector<b_oid> promosIds;
};

TEST_F(MongoDBTest, Basic) {
	try {
		/*result::insert_one r = users.insert_one(documentValue.view()).value();
		result::insert_one r2 = orders.insert_one(documentValue.view()).value();
		types::b_oid oid = r.inserted_id().get_oid();*/
        auto u = addUser();
        auto p = addPromo(10, 2);
        OrdersRepositoryMongoDB db("taxiTestDB");

        db.initializeConnection();
        db.createOrder(u.to_string(), p.to_string());
        //std::cout << p.to_string();
	} catch (const std::logic_error& e) {
		//TODO Here
        std::cout << e.what() << std::endl;
        throw;
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

