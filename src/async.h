/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef KD_ASYNC_H
#define KD_ASYNC_H

#include <napi.h>

using namespace kudu;

namespace kududown {

class Database;

/* abstract */
class AsyncWorker {

public:
  AsyncWorker(Database* database, napi_async_execute_callback callback)
  {
  }

protected:
  void SetStatus(Status status) {
    //this->status = status;
    //if (!status.ok())
      //SetErrorMessage(status.ToString().c_str());
  }

  //Database* database;

private:
  //Status status;
};

} // namespace kududown

#endif
