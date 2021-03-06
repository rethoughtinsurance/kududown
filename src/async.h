/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef LD_ASYNC_H
#define LD_ASYNC_H

#include <node.h>
#include <nan.h>


namespace kududown {

class Database;

/* abstract */
class AsyncWorker: public Nan::AsyncWorker {

public:
  AsyncWorker(Database* database, Nan::Callback *callback) :
      Nan::AsyncWorker(callback), database(database) {
  }

protected:
  void SetStatus(kudu::Status status) {
    this->status = status;
    if (!status.ok())
      SetErrorMessage(status.ToString().c_str());
  }

  Database* database;

private:
  kudu::Status status;
};

} // namespace kududown

#endif
