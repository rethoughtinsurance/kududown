/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef KD_DATABASE_H__
#define KD_DATABASE_H__

#include <map>
#include <vector>
#include <napi.h>

#include <client/client.h>

#include "kududown.h"
#include "kuduoptions.h"
#include "write_batch.h"
#include "iterator.h"

using namespace kudu;

namespace kududown {

  NAN_METHOD(KuduDOWN);

  struct Reference
  {
    Nan::Persistent<v8::Object> handle;
    Slice slice;

    Reference(v8::Local<v8::Value> obj, Slice slice)
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

    Status OpenDatabase(Options* options);
    Status PutToDatabase(WriteOptions* options, Slice key, Slice value);
    Status GetFromDatabase(ReadOptions* options, Slice key, std::string& value);
    Status DeleteFromDatabase(WriteOptions* options, Slice key);
    Status WriteBatchToDatabase(WriteOptions* options, WriteBatch* batch);
    uint64_t     ApproximateSizeFromDatabase(const void* range);

    void    CompactRangeFromDatabase(const Slice* start, const Slice* end);
    void    GetPropertyFromDatabase(const Slice& property, std::string* value);

    void CloseDatabase();
    void ReleaseIterator(uint32_t id);

    client::sp::shared_ptr<client::KuduSession> openSession();

    static Status getSliceAsString(client::KuduScanBatch::RowPtr row,
                                         client::KuduColumnSchema::DataType type,
                                         int index, std::string&);
  protected:
    Nan::Utf8String* location;
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

    Status connect();
    Status openTable(std::string);


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
