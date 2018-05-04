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


//  struct Reference
//  {
//    Nan::Persistent<v8::Object> handle;
//    Slice slice;
//
//    Reference(v8::Local<v8::Value> obj, Slice slice)
//        : slice(slice) {
//      v8::Local<v8::Object> _obj = Nan::New<v8::Object>();
//      _obj->Set(Nan::New("obj").ToLocalChecked(), obj);
//      handle.Reset(_obj);
//    }
//
//  };
//
//  static inline void
//  ClearReferences(std::vector<Reference *> *references) {
//    for (std::vector<Reference *>::iterator it = references->begin();
//        it != references->end();) {
//      DisposeStringOrBufferFromSlice((*it)->handle, (*it)->slice);
//      it = references->erase(it);
//    }
//    delete references;
//  }

  class Database : public Napi::ObjectWrap<Database>
  {
    friend class Iterator;

  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Database(const Napi::CallbackInfo& info);
    ~Database();

    Napi::Value Open(const Napi::CallbackInfo& info);
    Napi::Value Close(const Napi::CallbackInfo& info);
    Napi::Value Put(const Napi::CallbackInfo& info);
    Napi::Value Delete(const Napi::CallbackInfo& info);
    Napi::Value Get(const Napi::CallbackInfo& info);
    Napi::Value Batch(const Napi::CallbackInfo& info);
    Napi::Value Iterator(const Napi::CallbackInfo& info);
    Napi::Value GetProperty(const Napi::CallbackInfo& info);

//    static NAN_METHOD(New);
//  static NAN_METHOD(Open);
//  static NAN_METHOD(Close);
//  static NAN_METHOD(Put);
//  static NAN_METHOD(Delete);
//  static NAN_METHOD(Get);
//  static NAN_METHOD(Batch);
//  static NAN_METHOD(Write);
//  static NAN_METHOD(Iterator);
//  static NAN_METHOD(ApproximateSize);
//  static NAN_METHOD(CompactRange);
//  static NAN_METHOD(GetProperty);

  private:
    Nan::Utf8String* location;

    static Napi::FunctionReference constructor;
};

} // namespace kududown

#endif
