/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#include <napi.h>

#include "database.h"
#include "iterator.h"
#include "batch.h"
#include "kududown.h"
#include "kududown_async.h"

//#include "tracer.h"

namespace kududown {

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {

  kududown::Iterator::Init(Napi::Env env, Napi::Object exports);
  kududown::Batch::Init(Napi::Env env, Napi::Object exports);

  //node_addon_tracer::tracer::Init(target);
  return Database::Init(Napi::Env env, Napi::Object exports);
}

NODE_API_MODULE(kududown, InitAll)

} // namespace kududown
