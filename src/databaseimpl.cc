/*
 * databaseimpl.cc
 *
 *  Created on: May 4, 2018
 *      Author: janet
 */

#include "database.h"

client::sp::shared_ptr<client::KuduSession>
DatabaseImpl::openSession() {
  return kuduClientPtr->NewSession();
}

Status
DatabaseImpl::OpenDatabase(Options* opts) {
  if (!opts) {
    return Status::InvalidArgument("Options cannot be empty");
  }
  this->options = *opts;

  Status kuduStatus = connect();

  if (kuduStatus.ok()) {
    if (opts->tableName.size() > 0) {
      kuduStatus = openTable(opts->tableName);
    }
    else {
      //throw Status::InvalidArgument("A table name must be supplied");
      kuduStatus = openTable("impala::rtip.rtip_test");
    }
  }
//    if (kuduStatus.ok()) {
//      session = kuduClientPtr->NewSession();
//    }

  return kuduStatus;
}

Status
DatabaseImpl::openTable(std::string tableName) {
  //tracer::Log("Database" , LogLevel::DEBUG, "Opening table " + tableName);
  Status kuduStatus = kuduClientPtr->OpenTable(tableName, &tablePtr);
  if (kuduStatus.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, "Table OK");
  }
  else {
    //tracer::Log("Database" , LogLevel::DEBUG, "Table NOT OK");
  }
  this->tableStatus = kuduStatus;
  return kuduStatus;
}

Status
DatabaseImpl::connect() {
  //client::SetVerboseLogLevel(2);

  Status kuduStatus = client::KuduClientBuilder().add_master_server_addr(
      "192.168.1.3").Build(&kuduClientPtr);

  if (kuduStatus.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, kuduStatus.ToString());
  }
  else {
    //tracer::Log("Database" , LogLevel::DEBUG, kuduStatus.ToString());
  }
  return kuduStatus;
}

Status
DatabaseImpl::PutToDatabase(WriteOptions* options, Slice key, Slice value) {

  if (kuduClientPtr == 0) {
    return Status::RuntimeError(
        "Not connected. Unable to perform write operation.");
  }
  if (tablePtr == 0) {
    //tracer::Log("Database" , LogLevel::DEBUG, this->tableStatus.ToString());
    return this->tableStatus;
  }

  // open a session
  client::sp::shared_ptr < client::KuduSession > session =
      kuduClientPtr->NewSession();
  client::KuduUpsert* upsert = tablePtr->NewUpsert();
  KuduPartialRow* row = upsert->mutable_row();

  Status st = row->SetString(0, key);
  if (!st.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
    //session->Close();
    return st;
  }
  st = row->SetString(1, value);
  if (!st.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
    //session->Close();
    return st;
  }

  st = session->Apply(upsert);
  if (!st.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
    //session->Close();
    return st;
  }

  session->Close();
  return Status::OK();
}

Status
DatabaseImpl::GetFromDatabase(ReadOptions* options, Slice key,
                              std::string& value) {

  if (kuduClientPtr == 0) {
    return Status::RuntimeError(
        "Not connected. Unable to perform read operation.");
  }
  if (tablePtr == 0) {
    //tracer::Log("Database" , LogLevel::DEBUG, tableStatus.ToString());
    return this->tableStatus;
  }

  client::KuduScanner scanner(tablePtr.get());

  // make a predicate for <tableName>.key column and get a value
  // equal to the passed in key

  // TODO: make key name(s) based on schema
  // Schema schema = tablePtr->schema();
  client::KuduPredicate* p = tablePtr->NewComparisonPredicate(
      "key", client::KuduPredicate::ComparisonOp::EQUAL,
      client::KuduValue::CopyString(key));

  Status st = scanner.AddConjunctPredicate(p);
  if (!st.ok()) {
//      tracer::Log("Database" , LogLevel::DEBUG,
//                      std::string("FAILED TO ADD PREDICATE FOR KEY: ") + key.ToDebugString());
//      tracer::Log("Database", LogLevel::DEBUG, std::string(st.ToString()));
  }
  else {
//      tracer::Log("Database" , LogLevel::DEBUG,
//                std::string("ADDED PREDICATE FOR KEY: ") + key.ToDebugString());
  }
  //scanner.KeepAlive();
  st = scanner.Open();

  if (!st.ok()) {
    std::string msg("Unable to get table scanner: ");
    msg.append(st.ToString());
    scanner.Close();
    //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
    return st;
  }

  client::KuduScanBatch batch;

  int num_rows = 0;
  //tracer::Log("DatabaseImpl::GetFromDatabase", LogLevel::DEBUG, std::string("Starting SCAN."));
  while (scanner.HasMoreRows()) {
    scanner.NextBatch(&batch);
    num_rows += batch.NumRows();

    char tmp[128];
    sprintf(tmp, "%d", batch.NumRows());
    //tracer::Log("Database", LogLevel::TRACE,
    std::string("Got next batch with ") + tmp + " rows."
    );
    // TODO num_rows > 1 ??
    client::KuduSchema schema = scanner.GetProjectionSchema();

    for (client::KuduScanBatch::const_iterator it = batch.begin();
        it != batch.end(); ++it) {

      client::KuduScanBatch::RowPtr row(*it);

      //tracer::Log("Database", LogLevel::TRACE, "Read row: " + row.ToString());
      // TODO fix this when we add projection fields to the schema
      // and are dealing with tables that have more than 2 columns (i.e. rtip_test)
      //for (size_t x = 0; x < schema.num_columns(); ++x) {
      std::string str;
      Status st = getSliceAsString(row, schema.Column(1).type(), 1, str);
      value = str;
      //}
    }
  }
  if (num_rows == 0) {
    std::string msg("NotFound: " + key.ToDebugString() + " was not found");
    Status status = Status::NotFound(msg);
    //tracer::Log("Database" , LogLevel::DEBUG, status.ToString());
    //value.clear();
    return status;
  }
  return Status::OK();
}

Status
DatabaseImpl::getSliceAsString(client::KuduScanBatch::RowPtr row,
                               client::KuduColumnSchema::DataType type,
                               int index, std::string &output) {

  Slice slice;

  switch (type) {
    case client::KuduColumnSchema::DataType::BINARY: {
      Status st = row.GetBinary(index, &slice);
      if (!st.ok()) {
        return st;
      }
      output = slice.ToString();
      return st;
    }
    case client::KuduColumnSchema::DataType::BOOL: {
      bool b;
      Status st = row.GetBool(index, &b);
      if (!st.ok()) {
        return st;
      }
      output = b == true ? "true" : "false";
      return st;
    }
    case client::KuduColumnSchema::DataType::DOUBLE: {
      double d;
      Status st = row.GetDouble(index, &d);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%g", d);
      output = tmp;
      return st;
    }
    case client::KuduColumnSchema::DataType::FLOAT: {
      float flt;
      Status st = row.GetFloat(index, &flt);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%g", flt);
      output = tmp;
      return st;
    }
    case client::KuduColumnSchema::DataType::INT16: {
      short int i;
      Status st = row.GetInt16(index, &i);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%hd", i);
      output = tmp;
      return st;
    }
    case client::KuduColumnSchema::DataType::INT32: {
      int32_t i32;
      Status st = row.GetInt32(index, &i32);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%d", i32);
      output = tmp;
      return st;
    }
    case client::KuduColumnSchema::DataType::INT64: {
      int64_t bigint;
      Status st = row.GetInt64(index, &bigint);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%ld", bigint);
      output = tmp;
      return st;
    }
    case client::KuduColumnSchema::DataType::INT8: {
      signed char i;
      Status st = row.GetInt8(index, &i);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%.1s", &i);
      output = tmp;
      return st;
    }
    case client::KuduColumnSchema::DataType::STRING: {
      Status st = row.GetString(index, &slice);
      if (!st.ok()) {
        return st;
      }
      output = slice.ToString();
      return st;
    }
    case client::KuduColumnSchema::DataType::UNIXTIME_MICROS: {
      int64_t theTime;

      Status st = row.GetUnixTimeMicros(index, &theTime);
      if (!st.ok()) {
        return st;
      }
      char tmp[512];
      sprintf(tmp, "%ld", theTime);
      output = tmp;
      return st;
    }
    default:
      return Status::NotFound(
          "No conversion for datatype "
              + client::KuduColumnSchema::DataTypeToString(type));
  }
}

Status
DatabaseImpl::DeleteFromDatabase(WriteOptions* options, Slice key) {

  if (kuduClientPtr == 0) {
    return Status::RuntimeError(
        "Not connected. Unable to perform delete operation.");
  }
  if (tablePtr == 0) {
    //tracer::Log("Database" , LogLevel::DEBUG, tableStatus.ToString());
    return this->tableStatus;
  }

  client::sp::shared_ptr < client::KuduSession > session =
      kuduClientPtr->NewSession();

  client::KuduDelete* del = tablePtr->NewDelete();
  KuduPartialRow* row = del->mutable_row();
  Status st = row->SetString(0, key);
  if (!st.ok()) {
    return st;
  }
  st = session->Apply(del);
  if (!st.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
    session->Flush();
    if (st.ToString().find_first_of("key not found") != std::string::npos) {
      return Status::OK();
    }

    return st;
  }

  session->Flush();
  return Status::OK();
}

Status
DatabaseImpl::WriteBatchToDatabase(WriteOptions* options, WriteBatch* batch) {

  if (kuduClientPtr == 0) {
    //batch->Clear();
    return Status::RuntimeError(
        "Not connected. Unable to perform write batch operation.");
  }
  if (tablePtr == 0) {
    //batch->Clear();
    //tracer::Log("Database" , LogLevel::DEBUG, tableStatus.ToString());
    return this->tableStatus;
  }

  // open a session
  client::sp::shared_ptr < client::KuduSession > session =
      kuduClientPtr->NewSession();
  session->SetFlushMode(client::KuduSession::MANUAL_FLUSH);

  Status st;

  //tracer::Log("Database" , LogLevel::DEBUG, "Executing WriteBatchToDatabase");

  for (size_t x = 0; x < batch->size(); ++x) {
    BatchOp* op = batch->get(x);

    client::KuduWriteOperation* kuduWrite;
    switch (op->getOp()) {
      case 'p':
        //KUDU_LOG(WARNING) << "Executing WriteBatchToDatabase NewUpsert";
        kuduWrite = tablePtr->NewUpsert();
        break;
      case 'd':
        //KUDU_LOG(WARNING) << "Executing WriteBatchToDatabase NewDelete";
        kuduWrite = tablePtr->NewDelete();
        break;
      default:
        st = session->Flush();
        if (!st.ok()) {
          //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
          session->Close();
          return st;
        }
        session->Close();
        std::string msg("Unknown batch operation '");
        msg + op->getOp();
        msg.append("'. Batch operations must be either 'p' or 'd'.");
        //tracer::Log("Database" , LogLevel::DEBUG, std::string(msg));
        return Status::InvalidArgument(msg);
    }

    KuduPartialRow* row = kuduWrite->mutable_row();

    for (size_t y = 0; y < op->keySize(); ++y) {
      std::string& colValue = op->getKey(y);
      Status st = row->SetString(y, colValue);

      if (!st.ok()) {
        //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
        //batch->Clear();
        st = session->Flush();
        if (!st.ok()) {
          //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
          session->Close();
          return st;
        }
        session->Close();
        return st;
      }
    }
    for (size_t y = 0; y < op->valueSize(); ++y) {
      std::string& colValue = op->getValue(y);
      Status st = row->SetString(y, colValue);

      if (!st.ok()) {
        //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
        //batch->Clear();
        st = session->Flush();
        if (!st.ok()) {
          //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
          session->Close();
          return st;
        }
        session->Close();
        return st;
      }
    }

    //tracer::Log("Database", LogLevel::TRACE, "Applying WRITE operation");
    st = session->Apply(kuduWrite);
    if (!st.ok()) {
      //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
      session->Close();
      return st;
    }
  }
  st = session->Flush();
  if (!st.ok()) {
    //tracer::Log("Database" , LogLevel::DEBUG, st.ToString());
    session->Close();
    return st;
  }
  //tracer::Log("Database" , LogLevel::DEBUG, std::string("WRITE COMPLETE"));
  session->Close();
  return Status::OK();
}

uint64_t
DatabaseImpl::ApproximateSizeFromDatabase(const void* range) {
  return 0;
}

void
DatabaseImpl::CompactRangeFromDatabase(const Slice* start, const Slice* end) {
  //db->CompactRange(start, end);
}

void
DatabaseImpl::GetPropertyFromDatabase(const Slice& property,
                                      std::string* value) {
  *value =
      Status::NotSupported("GetPropertyFromDatabase is not supported").ToString();
}

void
DatabaseImpl::ReleaseIterator(uint32_t id) {
// called each time an Iterator is End()ed, in the main thread
// we have to remove our reference to it and if it's the last iterator
// we have to invoke a pending CloseWorker if there is one
// if there is a pending CloseWorker it means that we're waiting for
// iterators to end before we can close them

  iterators.erase(id);
  if (iterators.empty() && pendingCloseWorker != NULL) {
    Nan::AsyncQueueWorker((AsyncWorker*) pendingCloseWorker);
    pendingCloseWorker = NULL;
  }
}

void
DatabaseImpl::CloseDatabase() {
  // check to see if there are any iterators still pending
  // or if any batch operations are in effect?
}

/* V8 exposed functions *****************************/

NAN_METHOD(KuduDOWN) {
  v8::Local<v8::String> location = info[0].As<v8::String>();
  info.GetReturnValue().Set(DatabaseImpl::NewInstance(location));
}

