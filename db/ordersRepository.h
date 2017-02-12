#pragma once

using OrderId = std::string;
using UserId = std::string;
using PromoId = std::string;

//TODO mock class
class Order {

};

class OrdersRepository {
public:
    OrdersRepository() { ; }
    OrdersRepository(const OrdersRepository&) = delete;
    OrdersRepository& operator=(const OrdersRepository&) = delete;
    OrdersRepository(OrdersRepository&&) = delete;
    OrdersRepository& operator=(OrdersRepository&&) = delete;

    virtual void initializeConnection() = 0;
    virtual Order createOrder() = 0;
    virtual Order createOrder(const UserId& userId, const PromoId& promoId) = 0;
    virtual Order getOrder(const OrderId& orderId) = 0;
    virtual void cancelOrder(const OrderId& orderId) = 0;
    virtual void successOrder(const OrderId& orderId) = 0;

    virtual ~OrdersRepository() { ; }
};