#include "Messages.h"

Messages::Dictionary enENDictionary() {
  Messages::Dictionary r;

  r["_USER_NOT_FOUND"] = "User not found";
  r["_ORDER_NOT_FOUND"] = "Order not found";
  r["_PROMO_NOT_FOUND"] = "Promo code not found";
  r["_PROMO_EXPIRED"] = "Promo code expired";
  r["_INCORRECT_USER_ID"] = "Incorrect user id";
  r["_INCORRECT_ORDER_ID"] = "Incorret order id";
  r["_INCORRECT_PROMO_ID"] = "Incorrect promo id";
  r["_ORDER_EXISTS_"] = "Order already exists";
  r["_USERID_NOT_FOUND"] = "Field 'userid' not found";
  r["_PROMOID_NOT_FOUND"] = "Field 'promoid' not found";
  r["_NEWSTATUS_NOT_FOUND"] ="Field 'newstatus' not found";
  r["_INCORRECT_STATUS"] = "Incorrect status";
  r["_CONTENT_TYPE_NOT_SUPPORTED"] = "Content type not supported";

  return r;
};

Messages::Messages() {
  addDictionary("en_EN", enENDictionary());
}

void Messages::addDictionary(std::string&& name, Dictionary&& dictionary) noexcept {
  langToDictionary_.insert(std::make_pair<std::string, Dictionary>(std::move(name), std::move(dictionary)));
}

const std::string& Messages::get(const std::string& locale, const std::string& id) const noexcept {
  auto itDict = langToDictionary_.find(locale);

  if (itDict == langToDictionary_.end()) {
    itDict = langToDictionary_.find("en_EN");
  }

  auto& dict = itDict->second;
  auto word = dict.find(id);

  if (word == dict.end()) {
    return id;
  } else {
    return word->second;
  }
}

const std::string& getMessage(const std::string& locale, const std::string& id) noexcept {
  static Messages messages;
  return messages.get(locale, id);
}