/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */


#include "batch.h"
#include "batch_async.h"

namespace kududown {

/** NEXT WORKER **/

BatchWriteWorker::BatchWriteWorker (
    Batch* batch
  , Nan::Callback *callback
) : AsyncWorker(NULL, callback)
  , batch(batch)
{};

BatchWriteWorker::~BatchWriteWorker () {}

void BatchWriteWorker::Execute () {
  SetStatus(batch->Write());
}

} // namespace kududown
