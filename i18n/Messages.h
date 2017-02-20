#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Messages {
public:
  Messages();

  Messages(const Messages&) = delete;

  Messages& operator=(const Messages&) = delete;

  Messages(Messages&&) = delete;

  Messages& operator=(Messages&&) = delete;

  const std::string get(const std::string& locale, const std::string& id) const noexcept;
public:
  using Dictionary = std::unordered_map<std::string, std::string>;
  using DictionaryPtr = std::unique_ptr<Dictionary>;
private:
  std::unordered_map<std::string, DictionaryPtr> langToDictionary_;
};

std::string getMessage(const std::string& locale, const std::string& id) noexcept;