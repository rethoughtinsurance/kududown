/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#include <node.h>
#include <node_buffer.h>

#include "database.h"
#include "iterator.h"
#include "iterator_async.h"
#include "common.h"
#include "tracer.h"

using namespace node_addon_tracer;

namespace kududown {

  static Nan::Persistent<v8::FunctionTemplate> iterator_constructor;

  Iterator::Iterator(Database* database, uint32_t id, bool keys, bool values,
                     int limit, std::string* lt, std::string* lte,
                     std::string* gt, std::string* gte, bool fillCache,
                     bool keyAsBuffer, bool valueAsBuffer,
                     unsigned int highWaterMark)
      : database(database), id(id),
        keys(keys), values(values), limit(limit),
        lt(lt), lte(lte), gt(gt), gte(gte),
        keyAsBuffer(keyAsBuffer), valueAsBuffer(valueAsBuffer),
        highWaterMark(highWaterMark) {

    Nan::HandleScope scope;

    ended = false;
    nexting = false;
    seeking = false;
    inBatch = false;

    batchRowCount = 0;
    currentRowCount = 0;
    totalRowCount = 0;

    options = new ReadOptions();

    endWorker = NULL;

    // create the Scanner
    batch = new kudu::client::KuduScanBatch();
    scanner = 0;
    scanner = new kudu::client::KuduScanner(database->tablePtr.get());
    scanner->SetBatchSizeBytes(0);
    scanner->KeepAlive();
    iteratorStatus = scanner->Open();

    tracer::Log("Iterator", LogLevel::DEBUG, "Iterator CTOR");

    if (!iteratorStatus.ok()) {
      scanner->Close();
      delete scanner;
      scanner = 0;
      tracer::Log("Iterator", LogLevel::DEBUG, iteratorStatus.ToString());
    }
    scanner->SetBatchSizeBytes(highWaterMark);
    schema = scanner->GetProjectionSchema();

    tracer::Log("Iterator", LogLevel::DEBUG, "Iterator constructed");
  }

  Iterator::~Iterator() {
    delete options;
    if (scanner != 0) {
      scanner->Close();
    }
    delete scanner;
    delete batch;

    if (lt != NULL)
      delete lt;
    if (gt != NULL)
      delete gt;
    if (lte != NULL)
      delete lte;
    if (gte != NULL)
      delete gte;
  }


  bool
  Iterator::IteratorNext(std::vector<std::pair<std::string, std::string> >& result) {

    size_t size = 0;

    if (!iteratorStatus.ok()) {
      tracer::Log("IteratorNext", LogLevel::WARN, iteratorStatus.ToString());
      return false;
    }

    if (limit > 0 && totalRowCount >= (size_t)limit) {
      char tmp[128];
      sprintf(tmp, "totalRowCount of %lu exceeds limit of %d", totalRowCount, limit);
      tracer::Log("IteratorNext", LogLevel::WARN, tmp);
      return false;
    }

    if (currentRowCount >= batchRowCount) {
      char tmp[128];
      sprintf(tmp, "Getting more rows! totalRowCount: %lu, currentRowCount: %lu, batchRowCount: %lu",
              totalRowCount, currentRowCount, batchRowCount);
      tracer::Log("IteratorNext", LogLevel::WARN, tmp);

      batchRowCount = 0;
      currentRowCount = 0;

      while (scanner->HasMoreRows() && batchRowCount == 0) {
        tracer::Log("IteratorNext", LogLevel::DEBUG, "scanner has more rows");
        iteratorStatus = scanner->NextBatch(batch);

        if (!iteratorStatus.ok()) {
          tracer::Log("IteratorNext", LogLevel::DEBUG, iteratorStatus.ToString());
          scanner->Close();
          return false;
        }

        batchRowCount = batch->NumRows();
        totalRowCount += batchRowCount;
        char tmp[128];
        sprintf(tmp, "Reading %lu rows from iterator", batchRowCount);
        tracer::Log("Iterator", LogLevel::DEBUG, tmp);
      }
      if (batchRowCount == 0) {
        // nothing left to read
        return false;
      }
    }

    // we have a batch that we are processing

    // while we haven't hit the highWaterMark or run out of rows
    while (size < highWaterMark && currentRowCount < batchRowCount) {
      char tmp[128];
      sprintf(tmp, "size is %lu, currentRowCount: %lu, batchRowCount: %lu",
              size, currentRowCount, batchRowCount);
      tracer::Log("IteratorNext", LogLevel::DEBUG, tmp);
      kudu::client::KuduScanBatch::RowPtr row(batch->Row(currentRowCount++));
      std::string key, value;

      iteratorStatus = Database::getSliceAsString(row, schema.Column(0).type(), 0, key);
      if (!iteratorStatus.ok()) {
        tracer::Log("Iterator", LogLevel::ERROR, iteratorStatus.ToString());
        return false;
      }
      iteratorStatus = Database::getSliceAsString(row, schema.Column(1).type(), 1, value);
      if (!iteratorStatus.ok()) {
        tracer::Log("Iterator", LogLevel::ERROR, iteratorStatus.ToString());
        return false;
      }
      tracer::Log("Iterator", LogLevel::TRACE, "Read key: " + key);
      tracer::Log("Iterator", LogLevel::TRACE, "Read value: " + value);
      size += key.size() + value.size();
      result.push_back(std::make_pair(key, value));
    }
    char tmp[128];
    sprintf(tmp, "Total rows read so far: %lu", totalRowCount);
    tracer::Log("Iterator", LogLevel::DEBUG, tmp);

    return true;
  }

  kudu::Status
  Iterator::IteratorStatus() {
    return this->iteratorStatus;
  }

  void
  Iterator::IteratorEnd() {
  }

  void
  Iterator::Release() {
    database->ReleaseIterator(id);
  }

  void
  checkEndCallback(Iterator* iterator) {

    if (iterator->endWorker != NULL) {
      Nan::AsyncQueueWorker(iterator->endWorker);
      iterator->endWorker = NULL;
    }
  }

  NAN_METHOD(Iterator::Seek) {
    return Nan::ThrowError(Nan::New("Iterator::Seek - is not supported.").ToLocalChecked());
  }
//  Iterator* iterator = Nan::ObjectWrap::Unwrap<Iterator>(info.This());
//
//  iterator->ReleaseTarget();
//
//  v8::Local<v8::Value> targetBuffer = info[0].As<v8::Value>();
//  LD_STRING_OR_BUFFER_TO_COPY(_target, targetBuffer, target);
//  iterator->target = new kudu::Slice(_targetCh_, _targetSz_);
//
//  iterator->GetIterator();
//  Iterator* dbIterator = iterator->dbIterator;
//
//  //dbIterator->Seek(*iterator->target);
//  iterator->seeking = true;
//  iterator->landed = false;
//
//  if (iterator->OutOfRange(iterator->target)) {
//    if (iterator->reverse) {
//      dbIterator->SeekToFirst();
//      dbIterator->Prev();
//    }
//    else {
//      dbIterator->SeekToLast();
//      dbIterator->Next();
//    }
//  }
//  else {
//    if (dbIterator->Valid()) {
//      int cmp = dbIterator->key().compare(*iterator->target);
//      if (cmp > 0 && iterator->reverse) {
//        dbIterator->Prev();
//      }
//      else if (cmp < 0 && !iterator->reverse) {
//        dbIterator->Next();
//      }
//    }
//    else {
//      if (iterator->reverse) {
//        dbIterator->SeekToLast();
//      }
//      else {
//        dbIterator->SeekToFirst();
//      }
//      if (dbIterator->Valid()) {
//        int cmp = dbIterator->key().compare(*iterator->target);
//        if (cmp > 0 && iterator->reverse) {
//          dbIterator->SeekToFirst();
//          dbIterator->Prev();
//        }
//        else if (cmp < 0 && !iterator->reverse) {
//          dbIterator->SeekToLast();
//          dbIterator->Next();
//        }
//      }
//    }
//  }
//
//  info.GetReturnValue().Set(info.Holder());
//}

  NAN_METHOD(Iterator::Next) {
    Iterator* iterator = Nan::ObjectWrap::Unwrap<Iterator>(info.This());

    if (!info[0]->IsFunction()) {
      return Nan::ThrowError("next() requires a callback argument");
    }

    v8::Local<v8::Function> callback = info[0].As<v8::Function>();

    if (iterator->ended) {
      LD_RETURN_CALLBACK_OR_ERROR(callback, "iterator has ended");
    }

    NextWorker* worker =
        new NextWorker(iterator, new Nan::Callback(callback), checkEndCallback);

    // persist to prevent accidental GC
    v8::Local<v8::Object> _this = info.This();
    worker->SaveToPersistent("iterator", _this);
    iterator->nexting = true;
    Nan::AsyncQueueWorker(worker);

    info.GetReturnValue().Set(info.Holder());
  }

  NAN_METHOD(Iterator::End) {
    Iterator* iterator = Nan::ObjectWrap::Unwrap<Iterator>(info.This());

    if (!info[0]->IsFunction()) {
      return Nan::ThrowError("end() requires a callback argument");
    }

    if (!iterator->ended) {
      v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(info[0]);

      EndWorker* worker = new EndWorker(iterator, new Nan::Callback(callback));
      // persist to prevent accidental GC
      v8::Local<v8::Object> _this = info.This();
      worker->SaveToPersistent("iterator", _this);
      iterator->ended = true;

      if (iterator->nexting) {
        // waiting for a next() to return, queue the end
        iterator->endWorker = worker;
      }
      else {
        Nan::AsyncQueueWorker(worker);
      }
    }

    info.GetReturnValue().Set(info.Holder());
  }

  void
  Iterator::Init() {
    v8::Local<v8::FunctionTemplate> tpl =
        Nan::New<v8::FunctionTemplate>(Iterator::New);
    iterator_constructor.Reset(tpl);
    tpl->SetClassName(Nan::New("Iterator").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Nan::SetPrototypeMethod(tpl, "seek", Iterator::Seek);
    Nan::SetPrototypeMethod(tpl, "next", Iterator::Next);
    Nan::SetPrototypeMethod(tpl, "end", Iterator::End);
  }

  v8::Local<v8::Object>
  Iterator::NewInstance(v8::Local<v8::Object> database,
                        v8::Local<v8::Number> id,
                        v8::Local<v8::Object> optionsObj) {

    Nan::EscapableHandleScope scope;

    Nan::MaybeLocal<v8::Object> maybeInstance;
    v8::Local<v8::Object> instance;
    v8::Local<v8::FunctionTemplate> constructorHandle = Nan::New<
        v8::FunctionTemplate>(iterator_constructor);

    if (optionsObj.IsEmpty()) {
      v8::Local<v8::Value> argv[2] = { database, id };
      maybeInstance = Nan::NewInstance(constructorHandle->GetFunction(), 2,
                                       argv);
    }
    else {
      v8::Local<v8::Value> argv[3] = { database, id, optionsObj };
      maybeInstance = Nan::NewInstance(constructorHandle->GetFunction(), 3,
                                       argv);
    }

    if (maybeInstance.IsEmpty())
      Nan::ThrowError("Could not create new Iterator instance");
    else
      instance = maybeInstance.ToLocalChecked();
    return scope.Escape(instance);
  }

  NAN_METHOD(Iterator::New) {

    Database* database = Nan::ObjectWrap::Unwrap<Database>(info[0]->ToObject());

    int limit = -1;
    // default highWaterMark from Readble-streams
    size_t highWaterMark = 16 * 1024;
    v8::Local<v8::Value> id = info[1];
    v8::Local<v8::Object> optionsObj;
    v8::Local<v8::Object> ltHandle;
    v8::Local<v8::Object> lteHandle;
    v8::Local<v8::Object> gtHandle;
    v8::Local<v8::Object> gteHandle;

    std::string* lt = NULL;
    std::string* lte = NULL;
    std::string* gt = NULL;
    std::string* gte = NULL;

    if (info.Length() > 1 && info[2]->IsObject()) {
      optionsObj = v8::Local<v8::Object>::Cast(info[2]);

      if (!optionsObj.IsEmpty() && optionsObj->Has(Nan::New("limit").ToLocalChecked())) {
        limit = v8::Local<v8::Integer>::Cast(optionsObj->Get(
                  Nan::New("limit").ToLocalChecked()))->Value();
      }

      if (optionsObj->Has(Nan::New("highWaterMark").ToLocalChecked())) {
        highWaterMark =
            v8::Local<v8::Integer>::Cast(optionsObj->Get(Nan::New("highWaterMark").ToLocalChecked()))->Value();
      }

      if (optionsObj->Has(Nan::New("lt").ToLocalChecked())
          && (node::Buffer::HasInstance(optionsObj->Get(Nan::New("lt").ToLocalChecked()))
          || optionsObj->Get(Nan::New("lt").ToLocalChecked())->IsString())) {

        v8::Local<v8::Value> ltBuffer = optionsObj->Get(Nan::New("lt").ToLocalChecked());

        if (StringOrBufferLength(ltBuffer) > 0) {
          LD_STRING_OR_BUFFER_TO_COPY(_lt, ltBuffer, lt)
          lt = new std::string(_ltCh_, _ltSz_);
          delete[] _ltCh_;
        }
      }

      if (optionsObj->Has(Nan::New("lte").ToLocalChecked())
          && (node::Buffer::HasInstance(optionsObj->Get(Nan::New("lte").ToLocalChecked()))
          || optionsObj->Get(Nan::New("lte").ToLocalChecked())->IsString())) {

        v8::Local<v8::Value> lteBuffer = optionsObj->Get(Nan::New("lte").ToLocalChecked());

        // ignore end if it has size 0 since a Slice can't have length 0
        if (StringOrBufferLength(lteBuffer) > 0) {
          LD_STRING_OR_BUFFER_TO_COPY(_lte, lteBuffer, lte)
          lte = new std::string(_lteCh_, _lteSz_);
          delete[] _lteCh_;
        }
      }

      if (optionsObj->Has(Nan::New("gt").ToLocalChecked())
          && (node::Buffer::HasInstance(optionsObj->Get(Nan::New("gt").ToLocalChecked()))
          || optionsObj->Get(Nan::New("gt").ToLocalChecked())->IsString())) {

        v8::Local<v8::Value> gtBuffer = optionsObj->Get(Nan::New("gt").ToLocalChecked());

        // ignore end if it has size 0 since a Slice can't have length 0
        if (StringOrBufferLength(gtBuffer) > 0) {
          LD_STRING_OR_BUFFER_TO_COPY(_gt, gtBuffer, gt)
          gt = new std::string(_gtCh_, _gtSz_);
          delete[] _gtCh_;
        }
      }

      if (optionsObj->Has(Nan::New("gte").ToLocalChecked())
          && (node::Buffer::HasInstance(optionsObj->Get(Nan::New("gte").ToLocalChecked()))
          || optionsObj->Get(Nan::New("gte").ToLocalChecked())->IsString())) {

        v8::Local<v8::Value> gteBuffer = optionsObj->Get(Nan::New("gte").ToLocalChecked());

        // ignore end if it has size 0 since a Slice can't have length 0
        if (StringOrBufferLength(gteBuffer) > 0) {
          LD_STRING_OR_BUFFER_TO_COPY(_gte, gteBuffer, gte)
          gte = new std::string(_gteCh_, _gteSz_);
          delete[] _gteCh_;
        }
      }
    }

    bool keys = BooleanOptionValue(optionsObj, "keys", true);
    bool values = BooleanOptionValue(optionsObj, "values", true);
    bool keyAsBuffer = BooleanOptionValue(optionsObj, "keyAsBuffer", true);
    bool valueAsBuffer = BooleanOptionValue(optionsObj, "valueAsBuffer", true);
    bool fillCache = BooleanOptionValue(optionsObj, "fillCache");

    Iterator* iterator = new Iterator(database, (uint32_t)id->Int32Value(),
                                      keys, values,
                                      limit, lt, lte, gt, gte, fillCache,
                                      keyAsBuffer, valueAsBuffer, highWaterMark);
    iterator->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
  } // NAN_METHOD(Iterator::New)

}  // namespace kududown
