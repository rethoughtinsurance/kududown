// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// WriteBatch holds a collection of updates to apply atomically to a DB.
//
// The updates are applied in the order in which they are added
// to the WriteBatch.  For example, the value of "key" will be "v3"
// after the following batch is written:
//
//    batch.Put("key", "v1");
//    batch.Delete("key");
//    batch.Put("key", "v2");
//    batch.Put("key", "v3");
//
// Multiple threads can invoke const methods on a WriteBatch without
// external synchronization, but if any of the threads may call a
// non-const method, all threads accessing the same WriteBatch must use
// external synchronization.

#ifndef STORAGE_LEVELDB_INCLUDE_WRITE_BATCH_H_
#define STORAGE_LEVELDB_INCLUDE_WRITE_BATCH_H_

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
      for (size_t x = 0; x < values.size(); ++x) {
        delete values[x];
      }
      values.clear();
    }
    /**
     * Adds the new slice to the list of values.
     */
    void addValue(kudu::Slice* slice) {
      values.push_back(slice);
    }

    /**
     * Returns a value from the list of values.
     */
    kudu::Slice* get(size_t x) {
      if (x >= values.size()) {
        return 0;
      }
      return values[x];
    }

    size_t size() {
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
    std::vector<kudu::Slice*> values;
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
      return ops[index];
    }

    // Support for iterating over the contents of a batch.
    class Handler {
    public:
      virtual ~Handler();
      virtual void Put(const kudu::Slice& key, const kudu::Slice& value) = 0;
      virtual void Delete(const kudu::Slice& key) = 0;
    };

    kudu::Status Iterate(Handler* handler) const;

    bool hasData;

  private:
    std::vector<BatchOp*> ops;

    // Intentionally copyable
  };

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_INCLUDE_WRITE_BATCH_H_
