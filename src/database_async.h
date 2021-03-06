/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef LD_DATABASE_ASYNC_H
#define LD_DATABASE_ASYNC_H

#include <vector>
#include <node.h>

#include "kuduoptions.h"
#include "async.h"

namespace kududown {

class OpenWorker: public AsyncWorker {
public:
  OpenWorker(Database *database, Nan::Callback *callback, void* blockCache,
      const void* filterPolicy, bool createIfMissing, bool errorIfExists,
      bool compression, uint32_t writeBufferSize, uint32_t blockSize,
      uint32_t maxOpenFiles, uint32_t blockRestartInterval,
      uint32_t maxFileSize);

  virtual ~OpenWorker();
  virtual void Execute();

private:
  Options* options;
};

class CloseWorker: public AsyncWorker {
public:
  CloseWorker(Database *database, Nan::Callback *callback);

  virtual ~CloseWorker();
  virtual void Execute();
  virtual void WorkComplete();
};

class IOWorker: public AsyncWorker {
public:
  IOWorker(Database *database, Nan::Callback *callback, kudu::Slice key,
      v8::Local<v8::Object> &keyHandle);

  virtual ~IOWorker();
  virtual void WorkComplete();

protected:
  kudu::Slice key;
};

class ReadWorker: public IOWorker {
public:
  ReadWorker(Database *database, Nan::Callback *callback, kudu::Slice key,
      bool asBuffer, bool fillCache, v8::Local<v8::Object> &keyHandle);

  virtual ~ReadWorker();
  virtual void Execute();
  virtual void HandleOKCallback();

private:
  bool asBuffer;
  ReadOptions* options;
  std::string value;
};

class DeleteWorker: public IOWorker {
public:
  DeleteWorker(Database *database, Nan::Callback *callback, kudu::Slice key,
      bool sync, v8::Local<v8::Object> &keyHandle);

  virtual ~DeleteWorker();
  virtual void Execute();

protected:
  WriteOptions* options;
};

class WriteWorker: public DeleteWorker {
public:
  WriteWorker(Database *database, Nan::Callback *callback, kudu::Slice key,
      kudu::Slice value, bool sync, v8::Local<v8::Object> &keyHandle,
      v8::Local<v8::Object> &valueHandle);

  virtual ~WriteWorker();
  virtual void Execute();
  virtual void WorkComplete();

private:
  kudu::Slice value;
};

class BatchWorker: public AsyncWorker {
public:
  BatchWorker(Database *database, Nan::Callback *callback, WriteBatch* batch,
      bool sync);

  virtual
  ~BatchWorker();
  virtual void
  Execute();

private:
  WriteOptions* options;
  WriteBatch* batch;
};

class ApproximateSizeWorker: public AsyncWorker {
public:
  ApproximateSizeWorker(Database *database, Nan::Callback *callback,
      kudu::Slice start, kudu::Slice end, v8::Local<v8::Object> &startHandle,
      v8::Local<v8::Object> &endHandle);

  virtual
  ~ApproximateSizeWorker();
  virtual void
  Execute();
  virtual void
  HandleOKCallback();
  virtual void
  WorkComplete();

private:
  //kudu::Range range;
  uint64_t size;
};

class CompactRangeWorker: public AsyncWorker {
public:
  CompactRangeWorker(Database *database, Nan::Callback *callback,
      kudu::Slice start, kudu::Slice end, v8::Local<v8::Object> &startHandle,
      v8::Local<v8::Object> &endHandle);

  virtual
  ~CompactRangeWorker();
  virtual void
  Execute();
  virtual void
  HandleOKCallback();
  virtual void
  WorkComplete();

private:
  kudu::Slice rangeStart;
  kudu::Slice rangeEnd;
};

} // namespace kududown

#endif
