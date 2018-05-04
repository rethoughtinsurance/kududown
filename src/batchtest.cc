#include "write_batch.h"

using namespace kudu;
using namespace kududown;

int main(int argc, char** args) {

  WriteBatch wb;

  wb.Put(Slice("hello"), Slice("world!"));
  wb.Put(Slice("hello2"), Slice("world!"));
  wb.Put(Slice("hello3"), Slice("world!"));
  wb.Put(Slice("hello4"), Slice("world!"));
  wb.Delete(Slice("hello"));

  std::cout << wb.size() << std::endl;

  for (size_t x =0; x < wb.size(); ++x) {
    BatchOp* op = wb.get(x);
    for (size_t y = 0; y < op->size(); ++y) {
      std::cout << op->get(y)->ToString() << std::endl;
    }
  }

}
