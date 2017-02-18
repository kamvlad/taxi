#pragma once

#include <memory>
#include <json/json.hpp>

enum class RequestMethod {
  POST, GET
};

class JSONRequestHandler {
public:
  nlohmann::json handleRequest(RequestMethod method, const std::string &param,
                               const nlohmann::json &content) {
    nlohmann::json result;
    handleRequest(method, param, content, result);
    return result;
  }

  JSONRequestHandler *setNext(JSONRequestHandler *next) {
    next_.reset(next);
    return next;
  };

  virtual ~JSONRequestHandler() { ; }

protected:
  virtual bool process(RequestMethod method, const std::string &param, const nlohmann::json &content,
                       nlohmann::json &result) = 0;

private:
  void handleRequest(RequestMethod method, const std::string &param,
                     const nlohmann::json &content, nlohmann::json &result) {
    if (!process(method, param, content, result)) {
      if (next_) {
        next_->handleRequest(method, param, content, result);
      } else {
        throw std::invalid_argument("invalid request");
      }
    }
  }

private:
  std::unique_ptr<JSONRequestHandler> next_;
};

inline RequestMethod parseRequestType(const std::string &method) {
  if (method == "POST") {
    return RequestMethod::POST;
  } else if (method == "GET") {
    return RequestMethod::GET;
  } else {
    throw std::invalid_argument("Unknown request type");
  }
}