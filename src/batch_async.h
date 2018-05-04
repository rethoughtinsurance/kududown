/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef KD_BATCH_ASYNC_H
#define KD_BATCH_ASYNC_H

#include <napi.h>

#include "async.h"
#include "batch.h"
#include "database.h"

namespace kududown {

  class BatchWriteWorker : public AsyncWorker {
  public:
    BatchWriteWorker(Batch* batch);

    virtual ~BatchWriteWorker();
    virtual void Execute();

  private:
    Batch* batch;
  };

} // namespace kududown

#endif
