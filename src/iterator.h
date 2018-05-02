/* Copyright (c) 2012-2017 LevelDOWN contributors
 * See list at <https://github.com/level/leveldown#contributing>
 * MIT License <https://github.com/level/leveldown/blob/master/LICENSE.md>
 */

#ifndef _ITERATOR_H__
#define _ITERATOR_H__

#include <node.h>
#include <vector>
#include <nan.h>

#include "async.h"
#include "kududown.h"
#include "kuduoptions.h"

namespace kududown {

class Database;

class Iterator : public Nan::ObjectWrap {

public:
  static void Init();

  static v8::Local<v8::Object>
  NewInstance(v8::Local<v8::Object> database, v8::Local<v8::Number> id,
      v8::Local<v8::Object> optionsObj);

  Iterator(Database* database, uint32_t id, kudu::Slice* start,
      std::string* end, bool keys, bool values, int limit,
      std::string* lt, std::string* lte, std::string* gt, std::string* gte,
      bool fillCache, bool keyAsBuffer, bool valueAsBuffer, size_t highWaterMark);

  ~Iterator();

  bool IteratorNext(std::vector<std::pair<std::string, std::string> >& result);

  kudu::Status IteratorStatus();

  void IteratorEnd();

  void Release();


private:
  Database* database;
  uint32_t id;
  //Iterator* dbIterator;
  ReadOptions* options;
  kudu::Slice* start;

  std::string* end;
  bool seeking;
  bool keys;
  bool values;
  int limit;
  std::string* lt;
  std::string* lte;
  std::string* gt;
  std::string* gte;
  int count;


public:
  bool keyAsBuffer;
  bool valueAsBuffer;

  size_t highWaterMark;
  bool nexting;
  bool ended;
  AsyncWorker* endWorker;

private:

  kudu::Status iteratorStatus;
  kudu::client::KuduScanner* scanner;

  static NAN_METHOD(New);
  static NAN_METHOD(Seek);
  static NAN_METHOD(Next);
  static NAN_METHOD(End);
};

} // namespace kududown

#endif

/*
 if (kuduClientPtr == 0) {
 return kudu::Status::RuntimeError(
 "Not connected. Unable to perform write operation.");
 }
 if (tablePtr == 0) {
 KUDU_LOG(ERROR)<< tableStatus.ToString();
 return this->tableStatus;
 }

 kudu::client::KuduScanner scanner(tablePtr.get());

 //addPredicates(scanner, predicates);

 scanner.KeepAlive();
 kudu::Status st = scanner.Open();

 std::string msg("Unable to get table scanner: ");
 msg.append(st.ToString());

 CHECK_OK_OR_RETURN(st, msg);

 kudu::client::KuduScanBatch batch;

 int num_rows = 0;

 while (scanner.HasMoreRows()) {
 scanner.NextBatch(&batch);
 num_rows += batch.NumRows();
 kudu::client::KuduSchema schema = scanner.GetProjectionSchema();

 for (kudu::client::KuduScanBatch::const_iterator it = batch.begin();
 it != batch.end(); ++it) {

 kudu::client::KuduScanBatch::RowPtr row(*it);
 std::string newRow;

 for (size_t x = 0; x < schema.num_columns(); ++x) {
 std::string str;
 kudu::Status st = getSliceAsString(row, schema.Column(x).type(), x,
 str);
 newRow.append(str);
 if (x + 1 < schema.num_columns())
 newRow.append(",");
 }

 //resultSet.resultRows.push_back(newRow);
 }
 }
 */
