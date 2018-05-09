#ifndef _WRITE_BATCH_H_
#define _WRITE_BATCH_H_

#include <string>
#include <vector>
#include <client/client.h>

namespace kududown {

  class BatchOp {
  public:

    /**
     * Construct a new BatchOp with 'd' for a 'delete' operation
     * or 'p' for a 'put' operation.
     *
     * This class takes the ownership of any slices
     * in its list of values.
     *
     * It will release ownership of any values removed via a call
     * to removeFront().
     *
     * NOTE: Any values remaining in this class's list of values
     * will be deleted in the destructor!
     */
    BatchOp(char o) : op(o) {}

    ~BatchOp() {
//      for (size_t x = 0; x < values.size(); ++x) {
//        delete values[x];
//      }
      values.clear();
    }
    /**
     * Adds the new key to the list of values.
     */
    void addKey(std::string v) {
      keys.push_back(v);
    }
    /**
     * Adds the new value to the list of values.
     */
    void addValue(std::string v) {
      keys.push_back(v);
    }
    /**
     * Returns a key from the list of keys.
     */
    std::string& getKey(size_t x) {
      return keys.at(x);
    }
    /**
     * Returns a value from the list of values.
     */
    std::string& getValue(size_t x) {
      return values.at(x);
    }

    size_t keySize() {
      return keys.size();
    }

    size_t valueSize() {
      return values.size();
    }

    char getOp() {
      return op;
    }

  private:
    // not allowed
    BatchOp();
    BatchOp& operator=(const BatchOp&);

    char op;
    std::vector<std::string> keys;
    std::vector<std::string> values;
  };

  class WriteBatch {
  public:
    WriteBatch();
    ~WriteBatch();

    // Store the mapping "key->value" in the database.
    // in our case, the key won't be used and the value
    // will be an Avro binary object - KuduMessage?
    void Put(const kudu::Slice& key, const kudu::Slice& value);

    // If the database contains a mapping for "key", erase it.  Else do nothing.
    void Delete(const kudu::Slice& key);

    // Clear all updates buffered in this batch.
    void Clear();

    size_t size() {
      return ops.size();
    }

    BatchOp* get(size_t index) {
      return ops.at(index);
    }

    bool hasData;

  private:
    std::vector<BatchOp*> ops;

    // Intentionally copyable
  };

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_INCLUDE_WRITE_BATCH_H_
