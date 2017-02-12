#pragma once

#include <exception>

class BusinessLogicException : public std::runtime_error {
public:
    BusinessLogicException(const std::string& what_arg, unsigned code) : std::runtime_error(what_arg), code_(code) { ; }
    BusinessLogicException(const char* what_arg, unsigned code) : std::runtime_error(what_arg), code_(code) { ; }

    unsigned getCode() const noexcept { return code_; }
private:
    unsigned code_;
};