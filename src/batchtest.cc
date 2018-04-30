#include "write_batch.h"

using namespace kududown;

int main(int argc, char** args) {

  WriteBatch wb;

  wb.Put(kudu::Slice("hello"), kudu::Slice("world!"));
  wb.Put(kudu::Slice("hello2"), kudu::Slice("world!"));
  wb.Put(kudu::Slice("hello3"), kudu::Slice("world!"));
  wb.Put(kudu::Slice("hello4"), kudu::Slice("world!"));
  wb.Delete(kudu::Slice("hello"));

  std::cout << wb.size() << std::endl;

  for (size_t x =0; x < wb.size(); ++x) {
    BatchOp* op = wb.get(x);
    for (size_t y = 0; y < op->size(); ++y) {
      std::cout << op->get(y)->ToString() << std::endl;
    }
  }

}
