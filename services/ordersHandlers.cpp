#pragma once

#include "common/exceptions.h"
#include "common/requestHandler.h"

//TODO Remove it
class EchoHandler : public JSONRequestHandler {

public:
  virtual bool process(RequestMethod method, const std::string &param, const nlohmann::json &content,
                       nlohmann::json &result) {
    result = content;
    return true;
  }
};

bool getStringField(const nlohmann::json &content, const std::string &name, std::string &rslt) {
  auto it = content.find(name);

  if (it != content.end() && it->is_string()) {
    rslt = *it;
    return true;
  } else {
    return false;
  }
}

class CreateOrderHandler : public JSONRequestHandler {

public:
  //TODO Make it non copybale
  virtual bool process(RequestMethod method, const std::string &param, const nlohmann::json &content,
                       nlohmann::json &result) {
    if (method == RequestMethod::POST && param == "") {
      std::string userId;
      std::string promoId;
      bool hasUserIdField = getStringField(content, "userId", userId);
      bool hasPromoIdField = getStringField(content, "promoId", promoId);

      if (hasUserIdField && hasPromoIdField) {

      } else if (!(hasPromoIdField && hasPromoIdField)) {

      } else {
        if (!hasUserIdField) {
          //TODO enum codes
          throw BusinessLogicException("userId not found", 100);
        } else {
          //TODO enum codes
          throw BusinessLogicException("promoId not found", 100);
        }
      }
      return true;
    }
    return false;
  }
};

class GetOrderHandler : public JSONRequestHandler {
public:
  //TODO Make it non copybale
  virtual bool process(RequestMethod method, const std::string &param, const nlohmann::json &content,
                       nlohmann::json &result) {
    //TODO Parse param?
    std::cout << "HERE" << std::endl;
    if (method == RequestMethod::GET && param != "") {
      result["userId"] = param;
      result["promoId"] = "DEAD";
      return true;
    }
    return false;
  }
};


class SetOrderStatusHandler : public JSONRequestHandler {
public:
  //TODO Make it non copybale
  virtual bool process(RequestMethod method, const std::string &param, const nlohmann::json &content,
                       nlohmann::json &result) {
    //TODO Parse param?
    if (method == RequestMethod::POST && param != "") {
      std::string newStatus;

      if (!getStringField(content, "newStatus", newStatus)) {
        throw BusinessLogicException("newStatus not found", 100);
      }

      if (newStatus == "cancel") {

      } else if (newStatus == "success") {

      } else {
        throw BusinessLogicException("unknown status", 100);
      }
      return true;
    }
    return false;
  }
};

