#include <cassert>
#include <mongocxx/client.hpp>
#include "OrdersRepositoryMongoDB.h"

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
    DBConnection = mongocxx::client(dbURI_);
    Storage = DBConnection[dbName_];
}

Order OrdersRepositoryMongoDB::createOrder(const std::string& userId, const std::string& promoId) {
    mongocxx::collection promos = Storage["test"];

//    promos.find_one()
}

Order OrdersRepositoryMongoDB::getOrder(const OrderId& orderId) {

}

void OrdersRepositoryMongoDB::cancelOrder(const OrderId& orderId) {

}

void OrdersRepositoryMongoDB::successOrder(const OrderId& orderId) {

}