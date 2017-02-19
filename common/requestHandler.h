#pragma once

#include <memory>
#include <bsoncxx/json.hpp>

enum class RequestMethod {
  POST, GET
};

class RequestHandler {
public:
  RequestHandler() { ; }

  RequestHandler(const RequestHandler&) = delete;

  RequestHandler& operator= (const RequestHandler&) = delete;

  Document handleRequest(RequestMethod method, const std::string &param,
                               const Document &content) {
    Document result;
    handleRequest(method, param, content, result);
    return result;
  }

  RequestHandler *setNext(RequestHandler *next) {
    next_.reset(next);
    return next;
  };

  virtual ~RequestHandler() { ; }

protected:
  virtual bool process(RequestMethod method, const std::string &param, const Document &content,
                       Document &result) = 0;

private:
  void handleRequest(RequestMethod method, const std::string &param,
                     const Document &content, Document &result) {
    if (!process(method, param, content, result)) {
      if (next_) {
        next_->handleRequest(method, param, content, result);
      } else {
        throw std::invalid_argument("invalid request");
      }
    }
  }

private:
  std::unique_ptr<RequestHandler> next_;
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