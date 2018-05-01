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

using namespace std;

namespace kududown {

  WriteBatch::WriteBatch() : hasData(false) {
    Clear();
  }

  WriteBatch::~WriteBatch() {
    Clear();
  }

  void
  WriteBatch::Clear() {
    for (size_t i = 0; i < ops.size(); ++i) {
      delete ops[i];
    }
    ops.clear();
  }

  void
  WriteBatch::Put(const kudu::Slice& key, const kudu::Slice& value) {
    BatchOp* newOp = new BatchOp('p');
    newOp->addValue(key.ToString());
    newOp->addValue(value.ToString());
    this->ops.push_back(newOp);
  }

  void
  WriteBatch::Delete(const kudu::Slice& key) {
    BatchOp* newOp = new BatchOp('d');
    newOp->addValue(key.ToString());
    this->ops.push_back(newOp);
  }

}// namespace kududown
