#include <gtest/gtest.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <iostream>

TEST(MongoDBTest, Basic) {
    auto builder = bsoncxx::builder::stream::document{};
    builder << "a" << "b";

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;
    std::cout << bsoncxx::to_json(doc_value) << std::endl;
}

