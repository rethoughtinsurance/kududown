/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#include <napi.h>

#include "database.h"
#include "iterator.h"
#include "batch.h"
#include "kududown.h"
#include "kududown_async.h"

//#include "tracer.h"

namespace kududown {

NAN_METHOD(DestroyDB) {
  Nan::HandleScope scope;

  Nan::Utf8String* location = new Nan::Utf8String(info[0]);

  Nan::Callback* callback = new Nan::Callback(
      v8::Local<v8::Function>::Cast(info[1]));

  DestroyWorker* worker = new DestroyWorker(
      location
    , callback
  );

  Nan::AsyncQueueWorker(worker);

  info.GetReturnValue().SetUndefined();
}

NAN_METHOD(RepairDB) {
  Nan::HandleScope scope;

  Nan::Utf8String* location = new Nan::Utf8String(info[0]);

  Nan::Callback* callback = new Nan::Callback(
      v8::Local<v8::Function>::Cast(info[1]));

  RepairWorker* worker = new RepairWorker(
      location
    , callback
  );

  Nan::AsyncQueueWorker(worker);

  info.GetReturnValue().SetUndefined();
}

void Init(v8::Local<v8::Object> target) {

  Database::Init();
  kududown::Iterator::Init();
  kududown::Batch::Init();

  v8::Local<v8::Function> kududown =
      Nan::New<v8::FunctionTemplate>(KuduDOWN)->GetFunction();

  kududown->Set(
      Nan::New("destroy").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(DestroyDB)->GetFunction()
  );

  kududown->Set(
      Nan::New("repair").ToLocalChecked()
    , Nan::New<v8::FunctionTemplate>(RepairDB)->GetFunction()
  );

  target->Set(Nan::New("kududown").ToLocalChecked(), kududown);

  //node_addon_tracer::tracer::Init(target);
}

NODE_MODULE(kududown, Init)

} // namespace kududown
