// JMB TODO
/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */


#ifndef __kududown_async_h__
#define __kududown_async_h__

#include <node.h>

#include "async.h"

namespace kududown {

  class DestroyWorker : public AsyncWorker
  {
  public:
    DestroyWorker(Nan::Utf8String* location, Nan::Callback *callback);

    virtual
    ~DestroyWorker();
    virtual void
    Execute();

  private:
    Nan::Utf8String* location;
  };

  class RepairWorker : public AsyncWorker
  {
  public:
    RepairWorker(Nan::Utf8String* location, Nan::Callback *callback);

    virtual
    ~RepairWorker();
    virtual void
    Execute();

  private:
    Nan::Utf8String* location;
  };

} // namespace kududown

#endif
