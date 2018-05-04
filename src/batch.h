#ifndef KD_BATCH_H
#define KD_BATCH_H

#include <vector>
#include <napi.h>

#include "database.h"
#include "write_batch.h"

using namespace kudu;

namespace kududown {

  class Batch : public Nan::ObjectWrap {
  public:
    void Init(Napi::Env env, Napi::Object exports);

    static v8::Local<v8::Value>
    NewInstance(v8::Local<v8::Object> database, v8::Local<v8::Object> optionsObj);

    Batch(kududown::Database* database, bool sync);
    ~Batch();

    Status WriteIt();

  private:
    kududown::Database* database;
    WriteOptions* options;
    WriteBatch* batch;
    bool hasData; // keep track of whether we're writing data or not

    static NAN_METHOD(New);
  static NAN_METHOD(Put);
  static NAN_METHOD(Del);
  static NAN_METHOD(Clear);
  static NAN_METHOD(Write);
};

}
 // namespace kududown

#endif
