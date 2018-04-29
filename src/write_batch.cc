// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// WriteBatch::rep_ :=
//    sequence: fixed64
//    count: fixed32
//    data: record[count]
// record :=
//    kTypeValue varstring varstring         |
//    kTypeDeletion varstring
// varstring :=
//    len: varint32
//    data: uint8[len]


#include "write_batch.h"

namespace kududown {

  WriteBatch::WriteBatch() : hasData(false) {
    Clear();
  }

  WriteBatch::~WriteBatch() {
    Clear();
  }

  WriteBatch::Handler::~Handler() {
  }

  void
  WriteBatch::Clear() {
    while (!ops.empty()) {
      delete ops.front();
      ops.pop_front();
    }
  }

  kudu::Status
  WriteBatch::Iterate(Handler* handler) const {
//  Slice input(rep_);
//  if (input.size() < kHeader) {
//    return Status::Corruption("malformed WriteBatch (too small)");
//  }
//
//  input.remove_prefix(kHeader);
//  Slice key, value;
//  int found = 0;
//  while (!input.empty()) {
//    found++;
//    char tag = input[0];
//    input.remove_prefix(1);
//    switch (tag) {
//      case kTypeValue:
//        if (GetLengthPrefixedSlice(&input, &key) &&
//            GetLengthPrefixedSlice(&input, &value)) {
//          handler->Put(key, value);
//        } else {
//          return Status::Corruption("bad WriteBatch Put");
//        }
//        break;
//      case kTypeDeletion:
//        if (GetLengthPrefixedSlice(&input, &key)) {
//          handler->Delete(key);
//        } else {
//          return Status::Corruption("bad WriteBatch Delete");
//        }
//        break;
//      default:
//        return Status::Corruption("unknown WriteBatch tag");
//    }
//  }
//  if (found != WriteBatchInternal::Count(this)) {
//    return Status::Corruption("WriteBatch has wrong count");
//  } else {
//    return Status::OK();
//  }
    return kudu::Status::OK();
  }


  void
  WriteBatch::Put(const kudu::Slice& key, const kudu::Slice& value) {
    BatchOp* newOp = new BatchOp('p');
    newOp->addValue(new kudu::Slice(value));
  }

  void
  WriteBatch::Delete(const kudu::Slice& key) {
    BatchOp* newOp = new BatchOp('d');
    newOp->addValue(new kudu::Slice(key));
  }

}// namespace kududown
