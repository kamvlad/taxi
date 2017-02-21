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

  const std::string& get(const std::string& locale, const std::string& id) const noexcept;

public:
  class Dictionary : public std::unordered_map<std::string, std::string> {
  public:
    Dictionary() { ; }

    Dictionary(const Dictionary&) = delete;

    Dictionary& operator=(const Dictionary&) = delete;

    Dictionary(Dictionary&&) = default;

    Dictionary& operator=(Dictionary&&) = default;

    virtual ~Dictionary() {
      ;
    }
  };
private:
  void addDictionary(std::string&& name, Dictionary&& dictionary) noexcept;

private:
  std::unordered_map<std::string, Dictionary> langToDictionary_;
};

const std::string& getMessage(const std::string& locale, const std::string& id) noexcept;