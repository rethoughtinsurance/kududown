/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef _DATABASE_H__
#define _DATABASE_H__

#include <map>
#include <vector>
#include <node.h>
#include <nan.h>

#include <client/client.h>

#include "kududown.h"
#include "kuduoptions.h"
#include "write_batch.h"
#include "iterator.h"

namespace kududown {

  NAN_METHOD(KuduDOWN);

  struct Reference
  {
    Nan::Persistent<v8::Object> handle;
    kudu::Slice slice;

    Reference(v8::Local<v8::Value> obj, kudu::Slice slice)
        : slice(slice) {
      v8::Local<v8::Object> _obj = Nan::New<v8::Object>();
      _obj->Set(Nan::New("obj").ToLocalChecked(), obj);
      handle.Reset(_obj);
    }

  };

  static inline void
  ClearReferences(std::vector<Reference *> *references) {
    for (std::vector<Reference *>::iterator it = references->begin();
        it != references->end();) {
      DisposeStringOrBufferFromSlice((*it)->handle, (*it)->slice);
      it = references->erase(it);
    }
    delete references;
  }

  class Database : public Nan::ObjectWrap
  {
    friend class Iterator;

  public:
    static void Init();

    static v8::Local<v8::Value>  NewInstance(v8::Local<v8::String> &location);

    Database(const v8::Local<v8::Value>& from);
    ~Database();

    kudu::Status OpenDatabase(Options* options);
    kudu::Status PutToDatabase(WriteOptions* options, kudu::Slice key, kudu::Slice value);
    kudu::Status GetFromDatabase(ReadOptions* options, kudu::Slice key, std::string& value);
    kudu::Status DeleteFromDatabase(WriteOptions* options, kudu::Slice key);
    kudu::Status WriteBatchToDatabase(WriteOptions* options, WriteBatch* batch);
    uint64_t     ApproximateSizeFromDatabase(const void* range);

    void    CompactRangeFromDatabase(const kudu::Slice* start, const kudu::Slice* end);
    void    GetPropertyFromDatabase(const kudu::Slice& property, std::string* value);

//    kududown::Iterator* NewIterator(Database* database, uint32_t id, kudu::Slice* start,
//        std::string* end, bool reverse, bool keys, bool values,
//        int limit, std::string* lt, std::string* lte,
//        std::string* gt, std::string* gte, bool fillCache,
//        bool keyAsBuffer, bool valueAsBuffer, size_t highWaterMark);

    void CloseDatabase();
    void ReleaseIterator(uint32_t id);

    kudu::client::sp::shared_ptr<kudu::client::KuduSession> openSession();

  protected:
    Nan::Utf8String* location;
    kudu::client::sp::shared_ptr<kudu::client::KuduClient> kuduClientPtr;
    kudu::client::sp::shared_ptr<kudu::client::KuduTable> tablePtr;
    //kudu::client::sp::shared_ptr<kudu::client::KuduSession> session;
  private:
    Options options;

    kudu::Status tableStatus;

    uint32_t currentIteratorId;
    void (*pendingCloseWorker);
    void* blockCache;
    void* filterPolicy;

    std::map<uint32_t, kududown::Iterator*> iterators;

    kudu::Status connect();
    kudu::Status openTable(std::string);
    kudu::Status getSliceAsString(kudu::client::KuduScanBatch::RowPtr row,
                                 kudu::client::KuduColumnSchema::DataType type,
                                 int index, std::string&);

    static void WriteDoing(uv_work_t *req);
    static void WriteAfter(uv_work_t *req);

    static NAN_METHOD(New);
  static NAN_METHOD(Open);
  static NAN_METHOD(Close);
  static NAN_METHOD(Put);
  static NAN_METHOD(Delete);
  static NAN_METHOD(Get);
  static NAN_METHOD(Batch);
  static NAN_METHOD(Write);
  static NAN_METHOD(Iterator);
  static NAN_METHOD(ApproximateSize);
  static NAN_METHOD(CompactRange);
  static NAN_METHOD(GetProperty);
};

} // namespace kududown

#endif
