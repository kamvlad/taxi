#pragma once

#include "common/exceptions.h"

class UserNotFound : public BusinessLogicException {
public:
  UserNotFound() : BusinessLogicException("_USER_NOT_FOUND", 10) { ; }
};

class OrderNotFound : public BusinessLogicException {
public:
  OrderNotFound() : BusinessLogicException("_ORDER_NOT_FOUND", 11) { ; }
};

class PromoNotFound : public BusinessLogicException {
public:
  PromoNotFound() : BusinessLogicException("_PROMO_NOT_FOUND", 12) { ; }
};

class PromoExpired : public BusinessLogicException {
public:
  PromoExpired() : BusinessLogicException("_PROMO_EXPIRED", 13) { ; }
};

class IncorrectUserId : public BusinessLogicException {
public:
  IncorrectUserId() : BusinessLogicException("_INCORRECT_USER_ID", 14) { ; }
};

class IncorrectOrderId : public BusinessLogicException {
public:
  IncorrectOrderId() : BusinessLogicException("_INCORRECT_ORDER_ID", 15) { ; }
};

class IncorrectPromoId : public BusinessLogicException {
public:
  IncorrectPromoId() : BusinessLogicException("_INCORRECT_PROMO_ID", 16) { ; }
};

class OrderExists : public BusinessLogicException {
public:
  OrderExists() : BusinessLogicException("_ORDER_EXISTS_", 17) { ; }
};