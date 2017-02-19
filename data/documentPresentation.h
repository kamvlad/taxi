#pragma once

#include "document.h"

class DocumentPresentation {
public:
  virtual std::string toString(const Document&) const = 0;

  virtual Document fromString(const std::string&) const = 0;

  virtual ~DocumentPresentation() { ; }
};