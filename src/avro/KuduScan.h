/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AVRO_KUDUSCAN_H_321375268__H_
#define AVRO_KUDUSCAN_H_321375268__H_

#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace rtip {
enum KuduOperationType {
  SCAN, INSERT, DELETE
};

struct KuduOperation {
  std::string id;
  KuduOperationType operationType;
  KuduOperation() :
      id(std::string()), operationType(KuduOperationType()) {
  }
};

enum class KuduPredicateType {
  COMPARISON, IN_LIST, IS_NULL, IS_NOT_NULL
};

enum KuduDataType {
  BOOLEAN,
  INT8,
  INT16,
  INT32,
  INT64,
  UNIXTIME64,
  FLOAT32,
  FLOAT64,
  DECIMAL,
  STRING,
  BINARY
};

enum KuduColumnEncoding {
  AUTO_ENCODING,
  PLAIN_ENCODING,
  RLE,
  DICT_ENCODING,
  BIT_SHUFFLE,
  PREFIX_ENCODING
};

struct _KuduScan_avsc_Union__0__ {
private:
  size_t idx_;
  boost::any value_;
public:
  size_t idx() const {
    return idx_;
  }
  bool is_null() const {
    return (idx_ == 0);
  }
  void set_null() {
    idx_ = 0;
    value_ = boost::any();
  }
  KuduColumnEncoding get_KuduColumnEncoding() const;
  void set_KuduColumnEncoding(const KuduColumnEncoding& v);
  _KuduScan_avsc_Union__0__();
};

struct KuduColumnDef {
  typedef _KuduScan_avsc_Union__0__ encoding_t;
  std::string columnName;
  KuduDataType dataType;
  bool allowNull;
  encoding_t encoding;
  bool isPrimaryKey;
  KuduColumnDef() :
      columnName(std::string()), dataType(KuduDataType()), allowNull(bool()), encoding(
          encoding_t()), isPrimaryKey(bool()) {
  }
};

enum class KuduComparisonOperator {
  EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, IS_NULL, IS_NOT_NULL
};

struct KuduComparisonPredicate {
  KuduComparisonOperator op;
  std::string dataValue;
  KuduComparisonPredicate() :
      op(KuduComparisonOperator()), dataValue(std::string()) {
  }
};

struct KuduInListPredicate {
  std::vector<std::string> valueList;
  KuduInListPredicate() :
      valueList(std::vector<std::string>()) {
  }
};

struct _KuduScan_avsc_Union__1__ {
private:
  size_t idx_;
  boost::any value_;
public:
  size_t idx() const {
    return idx_;
  }
  bool is_null() const {
    return (idx_ == 0);
  }
  void set_null() {
    idx_ = 0;
    value_ = boost::any();
  }
  KuduComparisonPredicate get_KuduComparisonPredicate() const;
  void set_KuduComparisonPredicate(const KuduComparisonPredicate& v);
  KuduInListPredicate get_KuduInListPredicate() const;
  void set_KuduInListPredicate(const KuduInListPredicate& v);
  _KuduScan_avsc_Union__1__();
};

struct KuduPredicate {
  typedef _KuduScan_avsc_Union__1__ predicate_t;
  KuduPredicateType type;
  KuduColumnDef column;
  predicate_t predicate;
  KuduPredicate() :
      type(KuduPredicateType()), column(KuduColumnDef()), predicate(
          predicate_t()) {
  }
};

struct KuduScan {
  KuduOperation op;
  std::string tableName;
  std::vector<KuduPredicate> predicateList;
  KuduScan() :
      op(KuduOperation()), tableName(std::string()), predicateList(
          std::vector<KuduPredicate>()) {
  }
};

inline KuduColumnEncoding _KuduScan_avsc_Union__0__::get_KuduColumnEncoding() const {
  if (idx_ != 1) {
    throw avro::Exception("Invalid type for union");
  }
  return boost::any_cast<KuduColumnEncoding>(value_);
}

inline
void _KuduScan_avsc_Union__0__::set_KuduColumnEncoding(
    const KuduColumnEncoding& v) {
  idx_ = 1;
  value_ = v;
}

inline KuduComparisonPredicate _KuduScan_avsc_Union__1__::get_KuduComparisonPredicate() const {
  if (idx_ != 1) {
    throw avro::Exception("Invalid type for union");
  }
  return boost::any_cast<KuduComparisonPredicate>(value_);
}

inline
void _KuduScan_avsc_Union__1__::set_KuduComparisonPredicate(
    const KuduComparisonPredicate& v) {
  idx_ = 1;
  value_ = v;
}

inline KuduInListPredicate _KuduScan_avsc_Union__1__::get_KuduInListPredicate() const {
  if (idx_ != 2) {
    throw avro::Exception("Invalid type for union");
  }
  return boost::any_cast<KuduInListPredicate>(value_);
}

inline
void _KuduScan_avsc_Union__1__::set_KuduInListPredicate(
    const KuduInListPredicate& v) {
  idx_ = 2;
  value_ = v;
}

inline _KuduScan_avsc_Union__0__::_KuduScan_avsc_Union__0__() :
    idx_(0) {
}
inline _KuduScan_avsc_Union__1__::_KuduScan_avsc_Union__1__() :
    idx_(0) {
}
}
namespace avro {
template<> struct codec_traits<rtip::KuduOperationType> {
  static void encode(Encoder& e, rtip::KuduOperationType v) {
    if (v > rtip::DELETE) {
      std::ostringstream error;
      error << "enum value " << v
          << " is out of bound for rtip::KuduOperationType and cannot be encoded";
      throw avro::Exception(error.str());
    }
    e.encodeEnum(v);
  }
  static void decode(Decoder& d, rtip::KuduOperationType& v) {
    size_t index = d.decodeEnum();
    if (index > rtip::DELETE) {
      std::ostringstream error;
      error << "enum value " << index
          << " is out of bound for rtip::KuduOperationType and cannot be decoded";
      throw avro::Exception(error.str());
    }
    v = static_cast<rtip::KuduOperationType>(index);
  }
};

template<> struct codec_traits<rtip::KuduOperation> {
  static void encode(Encoder& e, const rtip::KuduOperation& v) {
    avro::encode(e, v.id);
    avro::encode(e, v.operationType);
  }
  static void decode(Decoder& d, rtip::KuduOperation& v) {
    if (avro::ResolvingDecoder *rd = dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin(); it != fo.end();
          ++it) {
        switch (*it) {
        case 0:
          avro::decode(d, v.id);
          break;
        case 1:
          avro::decode(d, v.operationType);
          break;
        default:
          break;
        }
      }
    } else {
      avro::decode(d, v.id);
      avro::decode(d, v.operationType);
    }
  }
};

template<> struct codec_traits<rtip::KuduPredicateType> {
  static void encode(Encoder& e, rtip::KuduPredicateType v) {
    if (v < rtip::KuduPredicateType::COMPARISON || v > rtip::KuduPredicateType::IS_NOT_NULL) {
      std::ostringstream error;
      error << "enum value " << (unsigned int)v
          << " is out of bound for rtip::KuduPredicateType and cannot be encoded";
      throw avro::Exception(error.str());
    }
    e.encodeEnum((unsigned long int)v);
  }
  static void decode(Decoder& d, rtip::KuduPredicateType& v) {
    size_t index = d.decodeEnum();
    if (index > (size_t)rtip::KuduPredicateType::IS_NOT_NULL) {
      std::ostringstream error;
      error << "enum value " << index
          << " is out of bound for rtip::KuduPredicateType and cannot be decoded";
      throw avro::Exception(error.str());
    }
    v = static_cast<rtip::KuduPredicateType>(index);
  }
};

template<> struct codec_traits<rtip::KuduDataType> {
  static void encode(Encoder& e, rtip::KuduDataType v) {
    if (v > rtip::BINARY) {
      std::ostringstream error;
      error << "enum value " << v
          << " is out of bound for rtip::KuduDataType and cannot be encoded";
      throw avro::Exception(error.str());
    }
    e.encodeEnum(v);
  }
  static void decode(Decoder& d, rtip::KuduDataType& v) {
    size_t index = d.decodeEnum();
    if (index > (size_t)rtip::BINARY) {
      std::ostringstream error;
      error << "enum value " << index
          << " is out of bound for rtip::KuduDataType and cannot be decoded";
      throw avro::Exception(error.str());
    }
    v = static_cast<rtip::KuduDataType>(index);
  }
};

template<> struct codec_traits<rtip::KuduColumnEncoding> {
  static void encode(Encoder& e, rtip::KuduColumnEncoding v) {
    if (v < rtip::AUTO_ENCODING || v > rtip::PREFIX_ENCODING) {
      std::ostringstream error;
      error << "enum value " << v
          << " is out of bound for rtip::KuduColumnEncoding and cannot be encoded";
      throw avro::Exception(error.str());
    }
    e.encodeEnum(v);
  }
  static void decode(Decoder& d, rtip::KuduColumnEncoding& v) {
    size_t index = d.decodeEnum();
    if (index > (size_t)rtip::PREFIX_ENCODING) {
      std::ostringstream error;
      error << "enum value " << index
          << " is out of bound for rtip::KuduColumnEncoding and cannot be decoded";
      throw avro::Exception(error.str());
    }
    v = static_cast<rtip::KuduColumnEncoding>(index);
  }
};

template<> struct codec_traits<rtip::_KuduScan_avsc_Union__0__> {
  static void encode(Encoder& e, rtip::_KuduScan_avsc_Union__0__ v) {
    e.encodeUnionIndex(v.idx());
    switch (v.idx()) {
    case 0:
      e.encodeNull();
      break;
    case 1:
      avro::encode(e, v.get_KuduColumnEncoding());
      break;
    }
  }
  static void decode(Decoder& d, rtip::_KuduScan_avsc_Union__0__& v) {
    size_t n = d.decodeUnionIndex();
    if (n >= 2) {
      throw avro::Exception("Union index too big");
    }
    switch (n) {
    case 0:
      d.decodeNull();
      v.set_null();
      break;
    case 1: {
      rtip::KuduColumnEncoding vv;
      avro::decode(d, vv);
      v.set_KuduColumnEncoding(vv);
    }
      break;
    }
  }
};

template<> struct codec_traits<rtip::KuduColumnDef> {
  static void encode(Encoder& e, const rtip::KuduColumnDef& v) {
    avro::encode(e, v.columnName);
    avro::encode(e, v.dataType);
    avro::encode(e, v.allowNull);
    avro::encode(e, v.encoding);
    avro::encode(e, v.isPrimaryKey);
  }
  static void decode(Decoder& d, rtip::KuduColumnDef& v) {
    if (avro::ResolvingDecoder *rd = dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin(); it != fo.end();
          ++it) {
        switch (*it) {
        case 0:
          avro::decode(d, v.columnName);
          break;
        case 1:
          avro::decode(d, v.dataType);
          break;
        case 2:
          avro::decode(d, v.allowNull);
          break;
        case 3:
          avro::decode(d, v.encoding);
          break;
        case 4:
          avro::decode(d, v.isPrimaryKey);
          break;
        default:
          break;
        }
      }
    } else {
      avro::decode(d, v.columnName);
      avro::decode(d, v.dataType);
      avro::decode(d, v.allowNull);
      avro::decode(d, v.encoding);
      avro::decode(d, v.isPrimaryKey);
    }
  }
};

template<> struct codec_traits<rtip::KuduComparisonOperator> {
  static void encode(Encoder& e, rtip::KuduComparisonOperator v) {
    if (v > rtip::KuduComparisonOperator::IS_NOT_NULL) {
      std::ostringstream error;
      error << "enum value " << (unsigned int)v
          << " is out of bound for rtip::KuduComparisonOperator and cannot be encoded";
      throw avro::Exception(error.str());
    }
    e.encodeEnum((unsigned long int)v);
  }
  static void decode(Decoder& d, rtip::KuduComparisonOperator& v) {
    size_t index = d.decodeEnum();
    if (index > (size_t)rtip::KuduComparisonOperator::IS_NOT_NULL) {
      std::ostringstream error;
      error << "enum value " << index
          << " is out of bound for rtip::KuduComparisonOperator and cannot be decoded";
      throw avro::Exception(error.str());
    }
    v = static_cast<rtip::KuduComparisonOperator>(index);
  }
};

template<> struct codec_traits<rtip::KuduComparisonPredicate> {
  static void encode(Encoder& e, const rtip::KuduComparisonPredicate& v) {
    avro::encode(e, v.op);
    avro::encode(e, v.dataValue);
  }
  static void decode(Decoder& d, rtip::KuduComparisonPredicate& v) {
    if (avro::ResolvingDecoder *rd = dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin(); it != fo.end();
          ++it) {
        switch (*it) {
        case 0:
          avro::decode(d, v.op);
          break;
        case 1:
          avro::decode(d, v.dataValue);
          break;
        default:
          break;
        }
      }
    } else {
      avro::decode(d, v.op);
      avro::decode(d, v.dataValue);
    }
  }
};

template<> struct codec_traits<rtip::KuduInListPredicate> {
  static void encode(Encoder& e, const rtip::KuduInListPredicate& v) {
    avro::encode(e, v.valueList);
  }
  static void decode(Decoder& d, rtip::KuduInListPredicate& v) {
    if (avro::ResolvingDecoder *rd = dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin(); it != fo.end();
          ++it) {
        switch (*it) {
        case 0:
          avro::decode(d, v.valueList);
          break;
        default:
          break;
        }
      }
    } else {
      avro::decode(d, v.valueList);
    }
  }
};

template<> struct codec_traits<rtip::_KuduScan_avsc_Union__1__> {
  static void encode(Encoder& e, rtip::_KuduScan_avsc_Union__1__ v) {
    e.encodeUnionIndex(v.idx());
    switch (v.idx()) {
    case 0:
      e.encodeNull();
      break;
    case 1:
      avro::encode(e, v.get_KuduComparisonPredicate());
      break;
    case 2:
      avro::encode(e, v.get_KuduInListPredicate());
      break;
    }
  }
  static void decode(Decoder& d, rtip::_KuduScan_avsc_Union__1__& v) {
    size_t n = d.decodeUnionIndex();
    if (n >= 3) {
      throw avro::Exception("Union index too big");
    }
    switch (n) {
    case 0:
      d.decodeNull();
      v.set_null();
      break;
    case 1: {
      rtip::KuduComparisonPredicate vv;
      avro::decode(d, vv);
      v.set_KuduComparisonPredicate(vv);
    }
      break;
    case 2: {
      rtip::KuduInListPredicate vv;
      avro::decode(d, vv);
      v.set_KuduInListPredicate(vv);
    }
      break;
    }
  }
};

template<> struct codec_traits<rtip::KuduPredicate> {
  static void encode(Encoder& e, const rtip::KuduPredicate& v) {
    avro::encode(e, v.type);
    avro::encode(e, v.column);
    avro::encode(e, v.predicate);
  }
  static void decode(Decoder& d, rtip::KuduPredicate& v) {
    if (avro::ResolvingDecoder *rd = dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin(); it != fo.end();
          ++it) {
        switch (*it) {
        case 0:
          avro::decode(d, v.type);
          break;
        case 1:
          avro::decode(d, v.column);
          break;
        case 2:
          avro::decode(d, v.predicate);
          break;
        default:
          break;
        }
      }
    } else {
      avro::decode(d, v.type);
      avro::decode(d, v.column);
      avro::decode(d, v.predicate);
    }
  }
};

template<> struct codec_traits<rtip::KuduScan> {
  static void encode(Encoder& e, const rtip::KuduScan& v) {
    avro::encode(e, v.op);
    avro::encode(e, v.tableName);
    avro::encode(e, v.predicateList);
  }
  static void decode(Decoder& d, rtip::KuduScan& v) {
    if (avro::ResolvingDecoder *rd = dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin(); it != fo.end();
          ++it) {
        switch (*it) {
        case 0:
          avro::decode(d, v.op);
          break;
        case 1:
          avro::decode(d, v.tableName);
          break;
        case 2:
          avro::decode(d, v.predicateList);
          break;
        default:
          break;
        }
      }
    } else {
      avro::decode(d, v.op);
      avro::decode(d, v.tableName);
      avro::decode(d, v.predicateList);
    }
  }
};

}
#endif
