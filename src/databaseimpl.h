/*
 * databaseimpl.h
 *
 *  Created on: May 4, 2018
 *      Author: janet
 */

#ifndef KD_DATABASEIMPL_H_
#define KD_DATABASEIMPL_H_

#include <client/client.h>

using namespace kudu;

namespace kududown {
  class DatabaseImpl {

    Status
    OpenDatabase(Options* options);
    Status
    PutToDatabase(WriteOptions* options, Slice key, Slice value);
    Status
    GetFromDatabase(ReadOptions* options, Slice key, std::string& value);
    Status
    DeleteFromDatabase(WriteOptions* options, Slice key);
    Status
    WriteBatchToDatabase(WriteOptions* options, WriteBatch* batch);
    uint64_t
    ApproximateSizeFromDatabase(const void* range);

    void
    CompactRangeFromDatabase(const Slice* start, const Slice* end);
    void
    GetPropertyFromDatabase(const Slice& property, std::string* value);

    void
    CloseDatabase();
    void
    ReleaseIterator(uint32_t id);

    client::sp::shared_ptr<client::KuduSession>
    openSession();

    static Status
    getSliceAsString(client::KuduScanBatch::RowPtr row,
                     client::KuduColumnSchema::DataType type, int index,
                     std::string&);
  protected:

    client::sp::shared_ptr<client::KuduClient> kuduClientPtr;
    client::sp::shared_ptr<client::KuduTable> tablePtr;

  private:
    Options options;

    Status tableStatus;

    uint32_t currentIteratorId;
    void (*pendingCloseWorker);
    void* blockCache;
    void* filterPolicy;

    std::map<uint32_t, kududown::Iterator*> iterators;

    Status
    connect();
    Status
    openTable(std::string);
  };
}
#endif /* KD_DATABASEIMPL_H_ */
