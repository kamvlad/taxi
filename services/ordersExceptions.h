#pragma once

#include "common/exceptions.h"

class IncorrectRequest : public BusinessLogicException {
public:
  IncorrectRequest(const std::string &what_arg, unsigned code) : BusinessLogicException(what_arg, code) { ; }

  IncorrectRequest(const char *what_arg, unsigned code) : BusinessLogicException(what_arg, code) { ; }
};

class UserIdNotFound : public IncorrectRequest {
public:
  UserIdNotFound() : IncorrectRequest("_USERID_NOT_FOUND", 71) { ; }
};

class PromoIdNotFound : public IncorrectRequest {
public:
  PromoIdNotFound() : IncorrectRequest("_USERID_NOT_FOUND", 72) { ; }
};

class NewStatusNotFound : public IncorrectRequest {
public:
  NewStatusNotFound() : IncorrectRequest("_NEWSTATUS_NOT_FOUND", 73) { ; }
};

class IncorrectStatus : public IncorrectRequest {
public:
  IncorrectStatus() : IncorrectRequest("_USERID_NOT_FOUND", 74) { ; }
};