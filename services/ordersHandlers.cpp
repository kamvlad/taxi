#pragma once

#include "ordersExceptions.h"
#include "common/requestHandler.h"
#include "db/ordersRepository.h"

bool getStringField(const Document &content, const std::string &name, std::string &rslt) {
  auto it = content.find(name);

  if (it != content.end()) {
    rslt = it->second;
    return true;
  } else {
    return false;
  }
}

class CreateOrderHandler : public RequestHandler {
public:
  CreateOrderHandler(const std::shared_ptr<OrdersRepository>& orders) : orders_(orders) { ; }

  virtual bool process(RequestMethod method, const std::string &param, const Document &content,
                       Document &result) {
    if (method == RequestMethod::POST && param == "") {
      std::string userId;
      std::string promoId;
      bool hasUserIdField = getStringField(content, "userId", userId);
      bool hasPromoIdField = getStringField(content, "promoId", promoId);

      if (hasUserIdField && hasPromoIdField) {
        auto order = orders_->createOrder(userId, promoId);
        result["orderId"] = order.getId();
      } else if (!(hasPromoIdField && hasPromoIdField)) {
        auto order = orders_->createOrder();
        orders_->createOrder();
        result["orderId"] = order.getId();
      } else {
        if (!hasUserIdField) {
          throw UserIdNotFound();
        } else {
          throw PromoIdNotFound();
        }
      }
      return true;
    }
    return false;
  }
private:
  std::shared_ptr<OrdersRepository> orders_;
};

class GetOrderHandler : public RequestHandler {
public:
  GetOrderHandler(const std::shared_ptr<OrdersRepository>& orders) : orders_(orders) { ; }

  virtual bool process(RequestMethod method, const std::string &param, const Document &content,
                       Document &result) {
    if (method == RequestMethod::GET && param != "") {
      auto order = orders_->getOrder(param);
      result["userId"] = order.getUserId();
      result["promoId"] = order.getPromoId();
      return true;
    }
    return false;
  }
private:
  std::shared_ptr<OrdersRepository> orders_;
};


class SetOrderStatusHandler : public RequestHandler {
public:
  SetOrderStatusHandler(const std::shared_ptr<OrdersRepository>& orders) : orders_(orders) { ; }

  virtual bool process(RequestMethod method, const std::string &param, const Document &content,
                       Document &result) {
    if (method == RequestMethod::POST && param != "") {
      std::string newStatus;

      if (!getStringField(content, "newStatus", newStatus)) {
        throw NewStatusNotFound();
      }

      if (newStatus == "cancel") {
        orders_->cancelOrder(param);
      } else if (newStatus == "success") {
        orders_->successOrder(param);
      } else {
        throw IncorrectStatus();
      }

      result["status"] = "OK";
      return true;
    }
    return false;
  }
private:
  std::shared_ptr<OrdersRepository> orders_;
};

