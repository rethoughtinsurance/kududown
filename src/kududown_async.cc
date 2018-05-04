///* Copyright (c) 2012-2017 LevelDOWN contributors
// * See list at <https://github.com/level/leveldown#contributing>
// * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
// */
//
//#include <leveldb/db.h>
//
#include "kududown.h"
#include "kududown_async.h"

using namespace kudu;

namespace kududown {

/** DESTROY WORKER **/

DestroyWorker::DestroyWorker (
    Nan::Utf8String* location
  , Nan::Callback *callback
) : AsyncWorker(NULL, callback)
  , location(location)
{};

DestroyWorker::~DestroyWorker () {
  delete location;
}

void DestroyWorker::Execute () {
  kududown::Options options;
  SetStatus(Status::OK());
}

/** REPAIR WORKER **/

RepairWorker::RepairWorker (
    Nan::Utf8String* location
  , Nan::Callback *callback
) : AsyncWorker(NULL, callback)
  , location(location)
{};

RepairWorker::~RepairWorker () {
  delete location;
}

void RepairWorker::Execute () {
  kududown::Options options;
  SetStatus(Status::OK());
}

} // namespace kududown
