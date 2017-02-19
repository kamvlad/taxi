#pragma once

class MongoDBComponentConfig {
public:
  MongoDBComponentConfig(const std::string& xpath, const fastcgi::Config *config) : xpath_(xpath), config_(config) {
    ;
  }

  std::string getLogger() const {
    return config_->asString(xpath_ + "/logger");
  }

  mongocxx::uri getMongoDBURI(fastcgi::Logger *logger) const {
    std::stringstream uri;

    if (!getConfig(getHostPath(1), uri)) {
      throw std::logic_error("host configuration for monogodb component not found");
    }

    int n = 2;
    while (getConfig(getHostPath(n), uri, ",")) {
      ++n;
    }

    if (!getConfig("database", uri, "/")) {
      throw std::logic_error("database for monogodb component isn't set");
    }

    getOptionsString(uri);
    std::string sUri(uri.str());

    if (logger != nullptr)
      logger->info("Connecting to database uri : mongodb://user:password@%s", sUri.c_str());

    uri.str("");
    uri << "mongodb://";

    if (getConfig("user", uri)) {
      getConfig("password", uri, ":");
      uri << "@" << sUri;
    } else {
      uri << sUri;
    }
    return mongocxx::uri(uri.str());
  }

  std::string getDatabaseName() const {
    return getConfig("database");
  }
private:
  void getOptionsString(std::stringstream& destination) const {
    const std::string optionsList[] = { "replicaSet", "ssl", "connectTimeoutMS", "socketTimeoutMS",
                                        "maxPoolSize", "minPoolSize", "maxIdleTimeMS", "waitQueueMultiple",
                                        "waitQueueTimeoutMS"};
    std::stringstream options;
    bool hasOptions = false;

    for (const auto& option : optionsList) {
      hasOptions = getOption(option, options, hasOptions);
    }

    if (hasOptions) {
      destination << "?" << options.str();
    }
  }

  std::string getHostPath(unsigned n) const {
    std::stringstream path;
    path << "hosts/host[" << n << "]";
    return path.str();
  }

  std::string getConfig(const std::string element) const {
    return config_->asString(xpath_ + "/" + element);
  }

  bool getConfig(const std::string element, std::stringstream& destination, const std::string& prefix = "") const {
    try {
      destination << prefix << config_->asString(xpath_ + "/" + element);
    } catch (const std::exception &e) {
      return false;
    }
    return true;
  }

  bool getOption(const std::string element, std::stringstream& destination, bool hasOption) const {
    try {
      if (hasOption) {
        destination << "&";
      }
      destination << element << "=" << config_->asString(xpath_ + "/options/" + element);
    } catch (const std::exception &e) {
      return hasOption;
    }
    return true;
  }

private:
  const std::string& xpath_;
  const fastcgi::Config *config_;
};