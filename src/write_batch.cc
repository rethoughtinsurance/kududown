
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
    if (!key.empty()) {
      newOp->addKey(key.ToString());
    }
    if (!value.empty()) {
      newOp->addValue(value.ToString());
    }

    this->ops.push_back(newOp);
  }

  void
  WriteBatch::Delete(const kudu::Slice& key) {
    BatchOp* newOp = new BatchOp('d');
    newOp->addValue(key.ToString());
    this->ops.push_back(newOp);
  }

}// namespace kududown
