#ifndef LD_BATCH_H
#define LD_BATCH_H

#include <vector>
#include <node.h>

#include "database.h"
#include "write_batch.h"

namespace kududown {

  class Batch : public Nan::ObjectWrap {
  public:
    static void Init();

    static v8::Local<v8::Value>
    NewInstance(v8::Local<v8::Object> database,
                v8::Local<v8::Object> optionsObj);

    Batch(kududown::Database* database, bool sync);
    ~Batch();

    kudu::Status WriteIt();

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
