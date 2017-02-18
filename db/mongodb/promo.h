#pragma once

class Promo {
  using oid = bsoncxx::oid;
public:
  Promo(const bsoncxx::document::value& documentValue) : documentValue_(documentValue) {
    document_ = documentValue_.view();
  }

  oid getId() const {
    return document_["_id"].get_oid().value;
  }

  int32_t getCount() const {
    return document_["count"].get_int32();
  }

  int32_t getLockedCount() const {
    return document_["lockedCount"].get_int32();
  }

  oid getUserByOrder(const oid& orderId) const {
    for (const auto& order : document_["orders"].get_array().value) {
      auto doc = order.get_document().view();
      if (doc["_id"].get_oid().value == orderId) {
        return doc["userId"].get_oid().value;
      }
    }
    throw std::logic_error("Order not found in promo");
  }

  int32_t getUserPromoCount(const oid& userId) const {
    for (const auto& user : document_["users"].get_array().value) {
      auto doc = user.get_document().view();
      if (doc["_id"].get_oid().value == userId) {
        return doc["count"].get_int32();
      }
    }
    throw std::logic_error("User not found in promo");
  }

  bool hasOrder(const oid& orderId) const {
    for (const auto& order : document_["orders"].get_array().value) {
      auto doc = order.get_document().view();
      if (doc["_id"].get_oid().value == orderId) {
        return true;
      }
    }
    return false;
  }
private:
  bsoncxx::document::value documentValue_;
  bsoncxx::document::view document_;
};
