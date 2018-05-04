/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#include <napi.h>


#include "database.h"
#include "async.h"
#include "database_async.h"
#include "batch.h"
#include "iterator.h"
#include "common.h"
#include "kududown.h"
#include "kudu/client/stubs.h"
//#include "tracer.h"

//using namespace node_addon_tracer;
using namespace kudu;

namespace kududown {

  Napi::FunctionReference Database::constructor;

  Database::Database(const Napi::CallbackInfo& info)
      : location(new Nan::Utf8String(from)),
        currentIteratorId(0), pendingCloseWorker(NULL),
        blockCache(NULL), filterPolicy(NULL) {
  }

  Database::~Database() {
    //session->Close();
    delete location;
  }

  Napi::Object
    Database::Init(Napi::Env env, Napi::Object exports) {

      v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(
          Database::New);
      database_constructor.Reset(tpl);
      tpl->SetClassName(Nan::New("Database").ToLocalChecked());
      tpl->InstanceTemplate()->SetInternalFieldCount(1);
      Nan::SetPrototypeMethod(tpl, "open", Database::Open);
      Nan::SetPrototypeMethod(tpl, "close", Database::Close);
      Nan::SetPrototypeMethod(tpl, "put", Database::Put);
      Nan::SetPrototypeMethod(tpl, "get", Database::Get);
      Nan::SetPrototypeMethod(tpl, "del", Database::Delete);
      Nan::SetPrototypeMethod(tpl, "batch", Database::Batch);
      Nan::SetPrototypeMethod(tpl, "approximateSize", Database::ApproximateSize);
      Nan::SetPrototypeMethod(tpl, "compactRange", Database::CompactRange);
      Nan::SetPrototypeMethod(tpl, "getProperty", Database::GetProperty);
      Nan::SetPrototypeMethod(tpl, "iterator", Database::Iterator);
    }


  NAN_METHOD(Database::New){
    Database* obj = new Database(info[0]);
    obj->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
  }

  v8::Local<v8::Value>
  Database::NewInstance(v8::Local<v8::String> &location) {
    Nan::EscapableHandleScope scope;

    Nan::MaybeLocal<v8::Object> maybeInstance;
    v8::Local<v8::Object> instance;

    v8::Local<v8::FunctionTemplate> constructorHandle = Nan::New<
        v8::FunctionTemplate>(database_constructor);

    v8::Local<v8::Value> argv[] = { location };
    maybeInstance = Nan::NewInstance(constructorHandle->GetFunction(), 1, argv);

    if (maybeInstance.IsEmpty())
      Nan::ThrowError("Could not create new Database instance");
    else
      instance = maybeInstance.ToLocalChecked();
    return scope.Escape(instance);
  }

  NAN_METHOD(Database::Open){
  LD_METHOD_SETUP_COMMON(open, 0, 1)

  bool createIfMissing = BooleanOptionValue(optionsObj, "createIfMissing", true);
  bool errorIfExists = BooleanOptionValue(optionsObj, "errorIfExists");
  bool compression = BooleanOptionValue(optionsObj, "compression", true);

//uint32_t cacheSize = UInt32OptionValue(optionsObj, "cacheSize", 8 << 20);
  uint32_t writeBufferSize = UInt32OptionValue(
      optionsObj
      , "writeBufferSize"
      , 4 << 20
  );
  uint32_t blockSize = UInt32OptionValue(optionsObj, "blockSize", 4096);
  uint32_t maxOpenFiles = UInt32OptionValue(optionsObj, "maxOpenFiles", 1000);
  uint32_t blockRestartInterval = UInt32OptionValue(
      optionsObj
      , "blockRestartInterval"
      , 16
  );
  uint32_t maxFileSize = UInt32OptionValue(optionsObj, "maxFileSize", 2 << 20);

//database->blockCache = leveldb::NewLRUCache(cacheSize);
//database->filterPolicy = leveldb::NewBloomFilterPolicy(10);

  OpenWorker* worker = new OpenWorker(
      database
      , new Nan::Callback(callback)
      , database->blockCache
      , database->filterPolicy
      , createIfMissing
      , errorIfExists
      , compression
      , writeBufferSize
      , blockSize
      , maxOpenFiles
      , blockRestartInterval
      , maxFileSize
  );
// persist to prevent accidental GC
  v8::Local<v8::Object> _this = info.This();
  worker->SaveToPersistent("database", _this);
  Nan::AsyncQueueWorker(worker);
}

// for an empty callback to iterator.end()
  NAN_METHOD(EmptyMethod) {
}


  NAN_METHOD(Database::Close){
    LD_METHOD_SETUP_COMMON_ONEARG(close)

    CloseWorker* worker = new CloseWorker(database, new Nan::Callback(callback));
    // persist to prevent accidental GC
    v8::Local<v8::Object> _this = info.This();
    worker->SaveToPersistent("database", _this);

    if (!database->iterators.empty()) {
      //tracer::Log("Database::Close", LogLevel::INFO, "Database still has iterators");
      // yikes, we still have iterators open! naughty naughty.
      // we have to queue up a CloseWorker and manually close each of them.
      // the CloseWorker will be invoked once they are all cleaned up
      database->pendingCloseWorker = worker;

      for (std::map<uint32_t, kududown::Iterator*>::iterator it
              = database->iterators.begin()
        ; it != database->iterators.end()
        ; ++it) {

          // for each iterator still open, first check if it's already in
          // the process of ending (ended==true means an async End() is
          // in progress), if not, then we call End() with an empty callback
          // function and wait for it to hit ReleaseIterator() where our
          // CloseWorker will be invoked

        kududown::Iterator *iterator = it->second;

        if (!iterator->ended) {
          v8::Local<v8::Function> end =
              v8::Local<v8::Function>::Cast(iterator->handle()->Get(
                  Nan::New<v8::String>("end").ToLocalChecked()));
          v8::Local<v8::Value> argv[] = {
              Nan::New<v8::FunctionTemplate>(EmptyMethod)->GetFunction() // empty callback
          };
          Nan::MakeCallback(iterator->handle(), end, 1, argv);
        }
      }
    }
    else {
      Nan::AsyncQueueWorker(worker);
    }
  }

  NAN_METHOD(Database::Put){
  LD_METHOD_SETUP_COMMON(put, 2, 3)

  v8::Local<v8::Object> keyHandle = info[0].As<v8::Object>();
  v8::Local<v8::Object> valueHandle = info[1].As<v8::Object>();
  LD_STRING_OR_BUFFER_TO_SLICE(key, keyHandle, key);
  LD_STRING_OR_BUFFER_TO_SLICE(value, valueHandle, value);

  bool sync = BooleanOptionValue(optionsObj, "sync");

  WriteWorker* worker = new WriteWorker(
      database
      , new Nan::Callback(callback)
      , key
      , value
      , sync
      , keyHandle
      , valueHandle
  );

// persist to prevent accidental GC
  v8::Local<v8::Object> _this = info.This();
  worker->SaveToPersistent("database", _this);
  Nan::AsyncQueueWorker(worker);
}

  NAN_METHOD(Database::Get){
  LD_METHOD_SETUP_COMMON(get, 1, 2)

  v8::Local<v8::Object> keyHandle = info[0].As<v8::Object>();
  LD_STRING_OR_BUFFER_TO_SLICE(key, keyHandle, key);

  bool asBuffer = BooleanOptionValue(optionsObj, "asBuffer", true);
  //bool fillCache = BooleanOptionValue(optionsObj, "fillCache", true);

  ReadWorker* worker = new ReadWorker(
      database
      , new Nan::Callback(callback)
      , key
      , asBuffer
      , false
      , keyHandle
  );
// persist to prevent accidental GC
  v8::Local<v8::Object> _this = info.This();
  worker->SaveToPersistent("database", _this);
  Nan::AsyncQueueWorker(worker);
}

  NAN_METHOD(Database::Delete){
LD_METHOD_SETUP_COMMON(del, 1, 2)

v8::Local<v8::Object> keyHandle = info[0].As<v8::Object>();
LD_STRING_OR_BUFFER_TO_SLICE(key, keyHandle, key);

bool sync = BooleanOptionValue(optionsObj, "sync");

DeleteWorker* worker = new DeleteWorker(
    database
    , new Nan::Callback(callback)
    , key
    , sync
    , keyHandle
);
// persist to prevent accidental GC
v8::Local<v8::Object> _this = info.This();
worker->SaveToPersistent("database", _this);
Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(Database::Batch) {

  if ((info.Length() == 0 || info.Length() == 1) && !info[0]->IsArray()) {
    v8::Local<v8::Object> optionsObj;
    if (info.Length() > 0 && info[0]->IsObject()) {
      optionsObj = info[0].As<v8::Object>();
    }
    info.GetReturnValue().Set(Batch::NewInstance(info.This(), optionsObj));
    return;
  }

  LD_METHOD_SETUP_COMMON(batch, 1, 2);

  bool sync = BooleanOptionValue(optionsObj, "sync");

  v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(info[0]);

  WriteBatch* batch = new WriteBatch();
  bool hasData = false;

  for (unsigned int i = 0; i < array->Length(); i++) {
    if (!array->Get(i)->IsObject())
      continue;

    v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(array->Get(i));
    v8::Local<v8::Value> keyBuffer = obj->Get(Nan::New("key").ToLocalChecked());
    v8::Local<v8::Value> type = obj->Get(Nan::New("type").ToLocalChecked());

    if (type->StrictEquals(Nan::New("del").ToLocalChecked())) {
      LD_STRING_OR_BUFFER_TO_SLICE(key, keyBuffer, key)

      batch->Delete(key);
      if (!hasData)
        hasData = true;

      DisposeStringOrBufferFromSlice(keyBuffer, key);
    } else if (type->StrictEquals(Nan::New("put").ToLocalChecked())) {
      v8::Local<v8::Value> valueBuffer = obj->Get(Nan::New("value").ToLocalChecked());

      LD_STRING_OR_BUFFER_TO_SLICE(key, keyBuffer, key)
      LD_STRING_OR_BUFFER_TO_SLICE(value, valueBuffer, value)
      batch->Put(key, value);
      if (!hasData)
        hasData = true;

      DisposeStringOrBufferFromSlice(keyBuffer, key);
      DisposeStringOrBufferFromSlice(valueBuffer, value);
    }
  }

  // don't allow an empty batch through
  if (hasData) {
    BatchWorker* worker = new BatchWorker(
        database
      , new Nan::Callback(callback)
      , batch
      , sync
    );
    // persist to prevent accidental GC
    v8::Local<v8::Object> _this = info.This();
    worker->SaveToPersistent("database", _this);
    Nan::AsyncQueueWorker(worker);
  }
  else {
    LD_RUN_CALLBACK(callback, 0, NULL);
  }
}

NAN_METHOD(Database::ApproximateSize) {
  v8::Local<v8::Object> startHandle = info[0].As<v8::Object>();
  v8::Local<v8::Object> endHandle = info[1].As<v8::Object>();

  LD_METHOD_SETUP_COMMON(approximateSize, -1, 2)

  LD_STRING_OR_BUFFER_TO_SLICE(start, startHandle, start)
  LD_STRING_OR_BUFFER_TO_SLICE(end, endHandle, end)

  ApproximateSizeWorker* worker  = new ApproximateSizeWorker(
      database
    , new Nan::Callback(callback)
    , start
    , end
    , startHandle
    , endHandle
  );
  // persist to prevent accidental GC
  v8::Local<v8::Object> _this = info.This();
  worker->SaveToPersistent("database", _this);
  Nan::AsyncQueueWorker(worker);
}

//Nan::NAN_METHOD_RETURN_TYPE Database::CompactRange(Nan::NAN_METHOD_ARGS_TYPE info)
NAN_METHOD(Database::CompactRange) {
  v8::Local<v8::Object> startHandle = info[0].As<v8::Object>();
  v8::Local<v8::Object> endHandle = info[1].As<v8::Object>();

  LD_METHOD_SETUP_COMMON(compactRange, -1, 2)
  LD_STRING_OR_BUFFER_TO_SLICE(start, startHandle, start)
  LD_STRING_OR_BUFFER_TO_SLICE(end, endHandle, end)

  CompactRangeWorker* worker  = new CompactRangeWorker(
      database
    , new Nan::Callback(callback)
    , start
    , end
    , startHandle
    , endHandle
  );
  // persist to prevent accidental GC
  v8::Local<v8::Object> _this = info.This();
  worker->SaveToPersistent("database", _this);
  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(Database::GetProperty) {
  v8::Local<v8::Value> propertyHandle = info[0].As<v8::Object>();
  v8::Local<v8::Function> callback; // for LD_STRING_OR_BUFFER_TO_SLICE

  LD_STRING_OR_BUFFER_TO_SLICE(property, propertyHandle, property)

  kududown::Database* database =
      Nan::ObjectWrap::Unwrap<kududown::Database>(info.This());

  std::string* value = new std::string();
  database->GetPropertyFromDatabase(property, value);
  v8::Local<v8::String> returnValue
      = Nan::New<v8::String>(value->c_str(), value->length()).ToLocalChecked();
  delete value;
  delete[] property.data();

  info.GetReturnValue().Set(returnValue);
}

NAN_METHOD(Database::Iterator) {
  Database* database = Nan::ObjectWrap::Unwrap<Database>(info.This());

  v8::Local<v8::Object> optionsObj;
  if (info.Length() > 0 && info[0]->IsObject()) {
    optionsObj = v8::Local<v8::Object>::Cast(info[0]);
  }

  // each iterator gets a unique id for this Database, so we can
  // easily store & lookup on our `iterators` map
  uint32_t id = database->currentIteratorId++;
  Nan::TryCatch try_catch;

  v8::Local<v8::Object> iteratorHandle =
      kududown::Iterator::NewInstance(info.This(), Nan::New<v8::Number>(id), optionsObj);

  if (try_catch.HasCaught()) {
    // NB: node::FatalException can segfault here if there is no room on stack.
    return Nan::ThrowError("Fatal Error in Database::Iterator!");
  }

  kududown::Iterator *iterator = Nan::ObjectWrap::Unwrap<kududown::Iterator>(iteratorHandle);

  database->iterators[id] = iterator;

  // register our iterator
  /*
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  obj->Set(Nan::New("iterator"), iteratorHandle);
  Nan::Persistent<v8::Object> persistent;
  persistent.Reset(nan_isolate, obj);
  database->iterators.insert(std::pair<uint32_t, Nan::Persistent<v8::Object> &>(id, persistent));
  */
  info.GetReturnValue().Set(iteratorHandle);
}

}  // namespace kududown
