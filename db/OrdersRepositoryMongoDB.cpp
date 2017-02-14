#include <mongocxx/client.hpp>
#include "OrdersRepositoryMongoDB.h"

using namespace mongocxx;
using namespace bsoncxx;
using namespace bsoncxx::builder::stream;
using Document = bsoncxx::builder::stream::document;

thread_local mongocxx::client DBConnection;
thread_local mongocxx::database Storage;

OrdersRepositoryMongoDB::OrdersRepositoryMongoDB(const std::string& dbName) :
        dbURI_{},
        dbName_(dbName) {

}

OrdersRepositoryMongoDB::OrdersRepositoryMongoDB(const std::string& dbURI, const std::string& dbName) :
        dbURI_{dbURI},
        dbName_(dbName) {

}

void OrdersRepositoryMongoDB::initializeConnection() {
    DBConnection = client(dbURI_);
    Storage = DBConnection[dbName_];
}

//TODO Remove it!
#include <iostream>
#include <bsoncxx/json.hpp>

Order OrdersRepositoryMongoDB::createOrder() {
    ;
}

Order OrdersRepositoryMongoDB::createOrder(const std::string& userId, const std::string& promoId) {
    collection promos = Storage["promos"];
    Document builder{};
    builder << "_id" << oid(promoId) << "count" << open_document << "$gt" << 0 << close_document;
    document::value search = builder << finalize;

    auto options = options::find();
    document::value  v = builder << "perUserCount" << 1 << finalize;
    options.projection(v.view());

    auto r = promos.find_one(search.view(), options);
    std::cout << "JSON: " << to_json(r.value()) << std::endl;
}

Order OrdersRepositoryMongoDB::getOrder(const std::string& orderId) {

}

void OrdersRepositoryMongoDB::cancelOrder(const std::string& orderId) {

}

void OrdersRepositoryMongoDB::successOrder(const std::string& orderId) {

}