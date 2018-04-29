/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#include <node.h>
#include <node_buffer.h>

#include "database.h"
#include "async.h"
#include "database_async.h"
#include "batch.h"
#include "iterator.h"
#include "common.h"
#include "kududown.h"
#include "kudu/client/stubs.h"

namespace kududown {

#define CHECK_OK_OR_RETURN(status, message) \
  if (!((status).ok())) { \
    KUDU_LOG(ERROR) << (status).ToString(); \
    return status; \
  } \

  static Nan::Persistent<v8::FunctionTemplate> database_constructor;

  Database::Database(const v8::Local<v8::Value>& from)
      : location(new Nan::Utf8String(from)), currentIteratorId(0), pendingCloseWorker(
      NULL), blockCache(NULL), filterPolicy(NULL) {
  }

  Database::~Database() {

    delete location;
  }

  /* Calls from worker threads, NO V8 HERE *****************************/

  kudu::Status
  Database::OpenDatabase(Options* opts) {
    if (!opts) {
      return kudu::Status::InvalidArgument("Options cannot be empty");
    }
    this->options = *opts;

    kudu::Status kuduStatus = connect();

    if (kuduStatus.ok()) {
      if (opts->tableName.size() > 0) {
        return openTable(opts->tableName);
      }
      else {
        //throw kudu::Status::InvalidArgument("A table name must be supplied");
        return openTable("impala::rtip.rtip_test");
      }
    }
    return kuduStatus;
  }

  kudu::Status
  Database::openTable(std::string tableName) {
    KUDU_LOG(INFO)<< "Opening table " << tableName;
    kudu::Status kuduStatus = kuduClientPtr->OpenTable(tableName,
        &tablePtr);
    if (kuduStatus.ok()) {
      KUDU_LOG(INFO) << "Table OK";
    }
    else {
      KUDU_LOG(ERROR) << kuduStatus.ToString() << "Table NOT OK";
    }
    this->tableStatus = kuduStatus;
    return kuduStatus;
  }

  kudu::Status
  Database::connect() {
    kudu::client::SetVerboseLogLevel(2);

    kudu::Status kuduStatus =
        kudu::client::KuduClientBuilder().add_master_server_addr("192.168.1.3").Build(
            &kuduClientPtr);

    if (kuduStatus.ok()) {
      KUDU_LOG(INFO)<< kuduStatus.ToString();
    }
    else {
      KUDU_LOG(ERROR) << kuduStatus.ToString();
    }
    return kuduStatus;
  }

  kudu::Status
  Database::PutToDatabase(WriteOptions* options, kudu::Slice key,
                          kudu::Slice value) {

    if (kuduClientPtr == 0) {
      return kudu::Status::RuntimeError(
          "Not connected. Unable to perform write operation.");
    }
    if (tablePtr == 0) {
      KUDU_LOG(ERROR)<< tableStatus.ToString();
      return this->tableStatus;
    }

    // open a session
    kudu::client::sp::shared_ptr<kudu::client::KuduSession> session =
        kuduClientPtr->NewSession();
    kudu::client::KuduUpsert* upsert = tablePtr->NewUpsert();
    kudu::KuduPartialRow* row = upsert->mutable_row();

    kudu::Status st = row->SetString(0, key);
    if (!st.ok()) {
      KUDU_LOG(ERROR)<< st.message();
      session->Close();
      return st;
    }
    st = row->SetString(1, value);
    if (!st.ok()) {
      KUDU_LOG(ERROR)<< st.message();
      session->Close();
      return st;
    }

    st = session->Apply(upsert);
    if (!st.ok()) {
      KUDU_LOG(ERROR)<< st.message();
      session->Close();
      return st;
    }

    session->Close();
    return kudu::Status::OK();
  }

  kudu::Status
  Database::GetFromDatabase(ReadOptions* options, kudu::Slice key,
                            std::string& value) {
    if (kuduClientPtr == 0) {
      return kudu::Status::RuntimeError(
          "Not connected. Unable to perform read operation.");
    }
    if (tablePtr == 0) {
      KUDU_LOG(ERROR)<< tableStatus.ToString();
      return this->tableStatus;
    }

    kudu::client::KuduScanner scanner(tablePtr.get());

    // make a predicate for <tableName>.key column and get a value
    // equal to the passed in key
    kudu::client::KuduPredicate* p = tablePtr->NewComparisonPredicate(
        "key", kudu::client::KuduPredicate::ComparisonOp::EQUAL,
        kudu::client::KuduValue::CopyString(key));

    scanner.AddConjunctPredicate(p);

    //scanner.KeepAlive();
    kudu::Status st = scanner.Open();

    std::string msg("Unable to get table scanner: ");
    msg.append(st.ToString());

    if (!st.ok()) {
      KUDU_LOG(ERROR) << st.ToString();
      scanner.Close();
      return st;
    }

    kudu::client::KuduScanBatch batch;

    int num_rows = 0;

    while (scanner.HasMoreRows()) {
      scanner.NextBatch(&batch);
      num_rows += batch.NumRows();

      // TODO num_rows > 1 ??
      kudu::client::KuduSchema schema = scanner.GetProjectionSchema();

      for (kudu::client::KuduScanBatch::const_iterator it = batch.begin();
          it != batch.end(); ++it) {

        kudu::client::KuduScanBatch::RowPtr row(*it);

        // TODO fix this when we add projection fields to the schema
        // and are dealing with tables that have more than 2 columns (i.e. rtip_test)
        for (size_t x = 0; x < schema.num_columns(); ++x) {
          std::string str;
          kudu::Status st =
              getSliceAsString(row, schema.Column(1).type(), x, str);
          value = str;
        }
      }
    }
    if (num_rows == 0) {
      std::string msg("NotFound: " + key.ToString() + " was not found");
      KUDU_LOG(WARNING) << msg;
      //value.clear();
      return kudu::Status::NotFound(msg);
    }
    return kudu::Status::OK();
  }


  kudu::Status
  Database::getSliceAsString(kudu::client::KuduScanBatch::RowPtr row,
                             kudu::client::KuduColumnSchema::DataType type,
                             int index, std::string &output) {

    kudu::Slice slice;

    switch (type) {
      case kudu::client::KuduColumnSchema::DataType::BINARY: {
        kudu::Status st = row.GetBinary(index, &slice);
        if (!st.ok()) {
          return st;
        }
        output = slice.ToString();
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::BOOL: {
        bool b;
        kudu::Status st = row.GetBool(index, &b);
        if (!st.ok()) {
          return st;
        }
        output = slice.ToString();
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::DOUBLE: {
        double d;
        kudu::Status st = row.GetDouble(index, &d);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%g", d);
        output = tmp;
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::FLOAT: {
        float flt;
        kudu::Status st = row.GetFloat(index, &flt);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%g", flt);
        output = tmp;
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::INT16: {
        short int i;
        kudu::Status st = row.GetInt16(index, &i);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%hd", i);
        output = tmp;
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::INT32: {
        int32_t i32;
        kudu::Status st = row.GetInt32(index, &i32);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%d", i32);
        output = tmp;
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::INT64: {
        int64_t bigint;
        kudu::Status st = row.GetInt64(index, &bigint);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%ld", bigint);
        output = tmp;
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::INT8: {
        signed char i;
        kudu::Status st = row.GetInt8(index, &i);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%.1s", &i);
        output = tmp;
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::STRING: {
        kudu::Status st = row.GetString(index, &slice);
        if (!st.ok()) {
          return st;
        }
        output = slice.ToString();
        return st;
      }
      case kudu::client::KuduColumnSchema::DataType::UNIXTIME_MICROS: {
        int64_t theTime;

        kudu::Status st = row.GetUnixTimeMicros(index, &theTime);
        if (!st.ok()) {
          return st;
        }
        char tmp[512];
        sprintf(tmp, "%ld", theTime);
        output = tmp;
        return st;
      }
      default:
        return kudu::Status::NotFound("No conversion for datatype "
            + kudu::client::KuduColumnSchema::DataTypeToString(type));
    }
  }

  kudu::Status
  Database::DeleteFromDatabase(WriteOptions* options, kudu::Slice key) {

    if (kuduClientPtr == 0) {
      return kudu::Status::RuntimeError(
          "Not connected. Unable to perform write operation.");
    }
    if (tablePtr == 0) {
      KUDU_LOG(ERROR)<< tableStatus.ToString();
      return this->tableStatus;
    }

    kudu::client::sp::shared_ptr<kudu::client::KuduSession> session =
            kuduClientPtr->NewSession();

    kudu::client::KuduDelete* del = tablePtr->NewDelete();
    kudu::KuduPartialRow* row = del->mutable_row();
    kudu::Status st = row->SetString(0, key);
    if (!st.ok()) {
      return st;
    }
    st = session->Apply(del);
    if (!st.ok()) {
      KUDU_LOG(ERROR)<< st.message();
      session->Flush();
      if (st.message().ToString().find_first_of("key not found") != std::string::npos) {
        return kudu::Status::OK();
      }

      return st;
    }

    session->Flush();
    return kudu::Status::OK();
  }

  kudu::Status
  Database::WriteBatchToDatabase(WriteOptions* options, WriteBatch* batch) {

    if (kuduClientPtr == 0) {
      batch->Clear();
      return kudu::Status::RuntimeError(
          "Not connected. Unable to perform write batch operation.");
    }
    if (tablePtr == 0) {
      batch->Clear();
      KUDU_LOG(ERROR)<< tableStatus.ToString();
      return this->tableStatus;
    }

    // open a session
    kudu::client::sp::shared_ptr<kudu::client::KuduSession> session =
        kuduClientPtr->NewSession();

    while (!batch->empty()) {
      BatchOp* op = batch->removeFront();

      kudu::client::KuduWriteOperation* kuduWrite;
      switch (op->getOp()) {
        case 'p':
          kuduWrite = tablePtr->NewUpsert();
          break;
        case 'd':
          kuduWrite = tablePtr->NewDelete();
          break;
        default:
          session->Close();
          batch->Clear();
          std::string msg("Unknown batch operation '");
          msg + op->getOp();
          msg.append("'. Batch operations must be either 'p' or 'd'.");
          return kudu::Status::InvalidArgument(msg);
      }

      kudu::KuduPartialRow* row = kuduWrite->mutable_row();
      size_t valueCount = 0;
      while (!op->empty()) {
        kudu::Slice* sl = op->removeFront();

        kudu::Status st = row->SetString(valueCount++, *sl);
        delete sl;

        if (!st.ok()) {
          KUDU_LOG(ERROR)<< st.message();
          batch->Clear();
          session->Close();
          return st;
        }
      }

      kudu::Status st = session->Apply(kuduWrite);
      if (!st.ok()) {
        KUDU_LOG(ERROR)<< st.message();
        batch->Clear();
        session->Close();
        return st;
      }
    }

    session->Close();
    return kudu::Status::OK();
  }

  uint64_t
  Database::ApproximateSizeFromDatabase(const void* range) {
    return 0;
  }

  void
  Database::CompactRangeFromDatabase(const kudu::Slice* start, const kudu::Slice* end) {
    //db->CompactRange(start, end);
  }

  void
  Database::GetPropertyFromDatabase(const kudu::Slice& property,
   std::string* value) {
    *value = kudu::Status::NotSupported("GetPropertyFromDatabase is not supported").ToString();
  }

  Iterator*
  Database::NewIterator(Database* database, uint32_t id, kudu::Slice* start,
      std::string* end, bool reverse, bool keys, bool values,
      int limit, std::string* lt, std::string* lte,
      std::string* gt, std::string* gte, bool fillCache,
      bool keyAsBuffer, bool valueAsBuffer, size_t highWaterMark) {

    return new kududown::Iterator(database, id, start, end, keys, values, limit,
        lt, lte, gt, gte, fillCache, keyAsBuffer, valueAsBuffer);
  }

  void
  Database::ReleaseIterator(uint32_t id) {
// called each time an Iterator is End()ed, in the main thread
// we have to remove our reference to it and if it's the last iterator
// we have to invoke a pending CloseWorker if there is one
// if there is a pending CloseWorker it means that we're waiting for
// iterators to end before we can close them

    iterators.erase(id);
    if (iterators.empty() && pendingCloseWorker != NULL) {
      Nan::AsyncQueueWorker((AsyncWorker*) pendingCloseWorker);
      pendingCloseWorker = NULL;
    }
  }

  void
  Database::CloseDatabase() {
  }

  /* V8 exposed functions *****************************/

  NAN_METHOD(KuduDOWN){
  v8::Local<v8::String> location = info[0].As<v8::String>();
  info.GetReturnValue().Set(Database::NewInstance(location));
}

  void
  Database::Init() {
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

//// for an empty callback to iterator.end()
//NAN_METHOD(EmptyMethod) {
//}
//
  NAN_METHOD(Database::Close){
  LD_METHOD_SETUP_COMMON_ONEARG(close)

  CloseWorker* worker = new CloseWorker(
      database
      , new Nan::Callback(callback)
  );
// persist to prevent accidental GC
  v8::Local<v8::Object> _this = info.This();
  worker->SaveToPersistent("database", _this);

//  if (!database->iterators.empty()) {
//    // yikes, we still have iterators open! naughty naughty.
//    // we have to queue up a CloseWorker and manually close each of them.
//    // the CloseWorker will be invoked once they are all cleaned up
//    database->pendingCloseWorker = worker;
//
//    for (
//        std::map< uint32_t, kududown::Iterator * >::iterator it
//            = database->iterators.begin()
//      ; it != database->iterators.end()
//      ; ++it) {
//
//        // for each iterator still open, first check if it's already in
//        // the process of ending (ended==true means an async End() is
//        // in progress), if not, then we call End() with an empty callback
//        // function and wait for it to hit ReleaseIterator() where our
//        // CloseWorker will be invoked
//
//        kududown::Iterator *iterator = it->second;
//
//        if (!iterator->ended) {
//          v8::Local<v8::Function> end =
//              v8::Local<v8::Function>::Cast(iterator->handle()->Get(
//                  Nan::New<v8::String>("end").ToLocalChecked()));
//          v8::Local<v8::Value> argv[] = {
//              Nan::New<v8::FunctionTemplate>(EmptyMethod)->GetFunction() // empty callback
//          };
//          Nan::MakeCallback(
//              iterator->handle()
//            , end
//            , 1
//            , argv
//          );
//        }
//    }
//  } else {
//    Nan::AsyncQueueWorker(worker);
//  }

  Nan::AsyncQueueWorker(worker);
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
  } else {

    KUDU_LOG(INFO) << "AT BATCH";

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
