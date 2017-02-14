#pragma once

class Order {
public:
    Order(const std::string& id, const std::string& userId, const std::string& promoId) :
            id_(id),
            userId_(userId),
            promoId_(promoId),
            promoUsed_(true) { ; }
    Order(const std::string& id) :
            id_(id),
            userId_(),
            promoId_(),
            promoUsed_(false) { ; }

    std::string getId() const noexcept { return id_; }
    std::string getUserId(std::string& userId) const noexcept { return id_; }
    std::string getPromoId(std::string& promoId) const noexcept { return promoId_; }
    bool isPromoUsed() const noexcept { return promoUsed_; }
private:
    std::string id_;
    std::string userId_;
    std::string promoId_;
    bool promoUsed_;
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
    virtual Order createOrder(const std::string& userId, const std::string& promoId) = 0;
    virtual Order getOrder(const std::string& orderId) = 0;
    virtual void cancelOrder(const std::string& orderId) = 0;
    virtual void successOrder(const std::string& orderId) = 0;

    virtual ~OrdersRepository() { ; }
};