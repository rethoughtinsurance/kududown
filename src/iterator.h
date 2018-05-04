/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef KD_ITERATOR_H__
#define KD_ITERATOR_H__

#include <napi.h>
#include <vector>

#include "async.h"
#include "kududown.h"
#include "kuduoptions.h"

using namespace kudu;

namespace kududown {

class Database;

class Iterator : public Nan::ObjectWrap {

public:
  static void Init(Napi::Env env, Napi::Object exports);

  static v8::Local<v8::Object>
  NewInstance(v8::Local<v8::Object> database, v8::Local<v8::Number> id,
      v8::Local<v8::Object> optionsObj);

  Iterator(Database* database, uint32_t id, bool keys, bool values, int limit,
      std::string* lt, std::string* lte, std::string* gt, std::string* gte,
      bool fillCache, bool keyAsBuffer, bool valueAsBuffer,
      unsigned int highWaterMark);

  ~Iterator();

  bool IteratorNext(std::vector<std::pair<std::string, std::string> >& result);

  Status IteratorStatus();

  void IteratorEnd();

  void Release();


private:
  Database* database;
  uint32_t id;
  //Iterator* dbIterator;
  ReadOptions* options;

  bool seeking;
  bool keys;
  bool values;
  bool inBatch;
  int limit; // -1 is unlimited, otherwise this is a row limit

  std::string* lt;
  std::string* lte;
  std::string* gt;
  std::string* gte;
  size_t batchRowCount;
  size_t currentRowCount;
  size_t totalRowCount;

public:
  bool keyAsBuffer;
  bool valueAsBuffer;

  unsigned int highWaterMark;
  bool nexting;
  bool ended;
  AsyncWorker* endWorker;

private:
  bool CreateScanner();

  Status iteratorStatus;
  client::KuduScanner* scanner;
  client::KuduScanBatch* batch;
  client::KuduSchema schema;

  static NAN_METHOD(New);
  static NAN_METHOD(Seek);
  static NAN_METHOD(Next);
  static NAN_METHOD(End);
};

} // namespace kududown

#endif

/*
 if (kuduClientPtr == 0) {
 return Status::RuntimeError(
 "Not connected. Unable to perform write operation.");
 }
 if (tablePtr == 0) {
 KUDU_LOG(ERROR)<< tableStatus.ToString();
 return this->tableStatus;
 }

 client::KuduScanner scanner(tablePtr.get());

 //addPredicates(scanner, predicates);

 scanner.KeepAlive();
 Status st = scanner.Open();

 std::string msg("Unable to get table scanner: ");
 msg.append(st.ToString());

 CHECK_OK_OR_RETURN(st, msg);

 client::KuduScanBatch batch;

 int num_rows = 0;

 while (scanner.HasMoreRows()) {
 scanner.NextBatch(&batch);
 num_rows += batch.NumRows();
 client::KuduSchema schema = scanner.GetProjectionSchema();

 for (client::KuduScanBatch::const_iterator it = batch.begin();
 it != batch.end(); ++it) {

 client::KuduScanBatch::RowPtr row(*it);
 std::string newRow;

 for (size_t x = 0; x < schema.num_columns(); ++x) {
 std::string str;
 Status st = getSliceAsString(row, schema.Column(x).type(), x,
 str);
 newRow.append(str);
 if (x + 1 < schema.num_columns())
 newRow.append(",");
 }

 //resultSet.resultRows.push_back(newRow);
 }
 }
 */
