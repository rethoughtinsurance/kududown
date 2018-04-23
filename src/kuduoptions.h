#ifndef __kudu_options_h__
#define __kudu_options_h__

#include <string>

namespace kududown {
  struct Options {
    Options()
        : tableName(""), createIfMissing(false), errorIfExists(true) {
    }
    Options(std::string tName)
        : tableName(tName), createIfMissing(false), errorIfExists(true) {
    }

    Options(const Options& other) {
      this->tableName = other.tableName;
      this->createIfMissing = other.createIfMissing;
      this->errorIfExists = other.errorIfExists;
    }

    Options& operator=(const Options& other) {
      if (this != &other) {
        this->tableName = other.tableName;
        this->createIfMissing = other.createIfMissing;
        this->errorIfExists = other.errorIfExists;
      }
      return *this;
    }

    std::string tableName;

    // always false as this refers to a directory missing
    // which is not applicable to Kudu
    bool createIfMissing;
    bool errorIfExists;
  };

  struct WriteOptions {
    // none available
  };

  struct ReadOptions {
    // none available
  };

}

#endif
