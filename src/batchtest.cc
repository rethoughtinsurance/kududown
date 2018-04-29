#include "write_batch.h"

using namespace kududown;

int main(char** args, int argc) {

  WriteBatch wb;

  wb.Put(kudu::Slice("hello"), kudu::Slice("world!"));
  wb.Put(kudu::Slice("hello2"), kudu::Slice("world!"));
  wb.Put(kudu::Slice("hello3"), kudu::Slice("world!"));
  wb.Put(kudu::Slice("hello4"), kudu::Slice("world!"));
  wb.Delete(kudu::Slice("hello"));

  wb.Clear();

}
