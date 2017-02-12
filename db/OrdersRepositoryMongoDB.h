#pragma once

#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include "ordersRepository.h"

class OrdersRepositoryMongoDB : public OrdersRepository {
public:
    OrdersRepositoryMongoDB(const std::string& dbName);
    OrdersRepositoryMongoDB(const std::string& dbURI, const std::string& dbName);

    virtual void initializeConnection();
    virtual Order createOrder();
    virtual Order createOrder(const std::string& userId, const std::string& promoId);
    virtual Order getOrder(const OrderId& orderId);
    virtual void cancelOrder(const OrderId& orderId);
    virtual void successOrder(const OrderId& orderId);

    virtual ~OrdersRepositoryMongoDB() { ; }
private:
    mongocxx::uri dbURI_;
    std::string dbName_;
};
