#include "JSONPresentation.h"

std::string JSONPresentation::toString(const Document &document) const {
  if (document.empty()) {
    return "{}";
  }

  std::string r = "{";
  auto it = document.begin();
  auto end = document.end();

  for (; it != end; ++it) {
    r += "\"" + it->first + "\":\"" + it->second + "\",";
  }

  r.back() = '}';
  return r;
}

Document JSONPresentation::fromString(const std::string &str) const {
  auto doc = bsoncxx::from_json(str);
  auto docView = doc.view();
  Document rslt;

  for (const auto &v : docView) {
    if (v.type() == bsoncxx::types::b_utf8::type_id)
      rslt[v.key().to_string()] = v.get_utf8().value.to_string();
  }

  return rslt;
}

