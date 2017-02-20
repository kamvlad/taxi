#pragma once

#include <bsoncxx/json.hpp>
#include <bsoncxx/types/value.hpp>
#include "document.h"
#include "documentPresentation.h"

class jsonPresentation : public DocumentPresentation {
public:
  virtual std::string toString(const Document& document) const override;

  virtual Document fromString(const std::string& str) const override;

  virtual ~jsonPresentation() { ; }
};