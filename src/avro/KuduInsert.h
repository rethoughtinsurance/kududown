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


#ifndef AVRO_KUDUINSERT_H_321375268__H_
#define AVRO_KUDUINSERT_H_321375268__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace rtip {
enum KuduOperationType {
    SCAN,
    INSERT,
    DELETE,
};

struct KuduOperation {
    std::string id;
    KuduOperationType operationType;
    KuduOperation() :
        id(std::string()),
        operationType(KuduOperationType())
        { }
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
    BINARY,
};

enum KuduColumnEncoding {
    AUTO_ENCODING,
    PLAIN_ENCODING,
    RLE,
    DICT_ENCODING,
    BIT_SHUFFLE,
    PREFIX_ENCODING,
};

struct _KuduInsert_avsc_Union__0__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    KuduColumnEncoding get_KuduColumnEncoding() const;
    void set_KuduColumnEncoding(const KuduColumnEncoding& v);
    _KuduInsert_avsc_Union__0__();
};

struct KuduColumnDef {
    typedef _KuduInsert_avsc_Union__0__ encoding_t;
    std::string columnName;
    KuduDataType dataType;
    bool allowNull;
    encoding_t encoding;
    bool isPrimaryKey;
    KuduColumnDef() :
        columnName(std::string()),
        dataType(KuduDataType()),
        allowNull(bool()),
        encoding(encoding_t()),
        isPrimaryKey(bool())
        { }
};

struct _KuduInsert_avsc_Union__1__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::string get_string() const;
    void set_string(const std::string& v);
    std::vector<uint8_t> get_bytes() const;
    void set_bytes(const std::vector<uint8_t>& v);
    _KuduInsert_avsc_Union__1__();
};

struct KuduDataValue {
    typedef _KuduInsert_avsc_Union__1__ value_t;
    KuduDataType dataType;
    value_t value;
    KuduDataValue() :
        dataType(KuduDataType()),
        value(value_t())
        { }
};

struct KuduRow {
    std::vector<KuduDataValue > values;
    KuduRow() :
        values(std::vector<KuduDataValue >())
        { }
};

struct KuduInsert {
    KuduOperation op;
    std::string tableName;
    std::vector<KuduColumnDef > columns;
    KuduRow row;
    KuduInsert() :
        op(KuduOperation()),
        tableName(std::string()),
        columns(std::vector<KuduColumnDef >()),
        row(KuduRow())
        { }
};

inline
KuduColumnEncoding _KuduInsert_avsc_Union__0__::get_KuduColumnEncoding() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<KuduColumnEncoding >(value_);
}

inline
void _KuduInsert_avsc_Union__0__::set_KuduColumnEncoding(const KuduColumnEncoding& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::string _KuduInsert_avsc_Union__1__::get_string() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _KuduInsert_avsc_Union__1__::set_string(const std::string& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<uint8_t> _KuduInsert_avsc_Union__1__::get_bytes() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<uint8_t> >(value_);
}

inline
void _KuduInsert_avsc_Union__1__::set_bytes(const std::vector<uint8_t>& v) {
    idx_ = 2;
    value_ = v;
}

inline _KuduInsert_avsc_Union__0__::_KuduInsert_avsc_Union__0__() : idx_(0) { }
inline _KuduInsert_avsc_Union__1__::_KuduInsert_avsc_Union__1__() : idx_(0) { }
}
namespace avro {
template<> struct codec_traits<rtip::KuduOperationType> {
    static void encode(Encoder& e, rtip::KuduOperationType v) {
		if (v < rtip::SCAN || v > rtip::DELETE)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for rtip::KuduOperationType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, rtip::KuduOperationType& v) {
		size_t index = d.decodeEnum();
		if (index < rtip::SCAN || index > rtip::DELETE)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for rtip::KuduOperationType and cannot be decoded";
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
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
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

template<> struct codec_traits<rtip::KuduDataType> {
    static void encode(Encoder& e, rtip::KuduDataType v) {
		if (v < rtip::BOOLEAN || v > rtip::BINARY)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for rtip::KuduDataType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, rtip::KuduDataType& v) {
		size_t index = d.decodeEnum();
		if (index < rtip::BOOLEAN || index > rtip::BINARY)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for rtip::KuduDataType and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<rtip::KuduDataType>(index);
    }
};

template<> struct codec_traits<rtip::KuduColumnEncoding> {
    static void encode(Encoder& e, rtip::KuduColumnEncoding v) {
		if (v < rtip::AUTO_ENCODING || v > rtip::PREFIX_ENCODING)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for rtip::KuduColumnEncoding and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, rtip::KuduColumnEncoding& v) {
		size_t index = d.decodeEnum();
		if (index < rtip::AUTO_ENCODING || index > rtip::PREFIX_ENCODING)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for rtip::KuduColumnEncoding and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<rtip::KuduColumnEncoding>(index);
    }
};

template<> struct codec_traits<rtip::_KuduInsert_avsc_Union__0__> {
    static void encode(Encoder& e, rtip::_KuduInsert_avsc_Union__0__ v) {
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
    static void decode(Decoder& d, rtip::_KuduInsert_avsc_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
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
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
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

template<> struct codec_traits<rtip::_KuduInsert_avsc_Union__1__> {
    static void encode(Encoder& e, rtip::_KuduInsert_avsc_Union__1__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_string());
            break;
        case 2:
            avro::encode(e, v.get_bytes());
            break;
        }
    }
    static void decode(Decoder& d, rtip::_KuduInsert_avsc_Union__1__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 3) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::string vv;
                avro::decode(d, vv);
                v.set_string(vv);
            }
            break;
        case 2:
            {
                std::vector<uint8_t> vv;
                avro::decode(d, vv);
                v.set_bytes(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<rtip::KuduDataValue> {
    static void encode(Encoder& e, const rtip::KuduDataValue& v) {
        avro::encode(e, v.dataType);
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, rtip::KuduDataValue& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.dataType);
                    break;
                case 1:
                    avro::decode(d, v.value);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.dataType);
            avro::decode(d, v.value);
        }
    }
};

template<> struct codec_traits<rtip::KuduRow> {
    static void encode(Encoder& e, const rtip::KuduRow& v) {
        avro::encode(e, v.values);
    }
    static void decode(Decoder& d, rtip::KuduRow& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.values);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.values);
        }
    }
};

template<> struct codec_traits<rtip::KuduInsert> {
    static void encode(Encoder& e, const rtip::KuduInsert& v) {
        avro::encode(e, v.op);
        avro::encode(e, v.tableName);
        avro::encode(e, v.columns);
        avro::encode(e, v.row);
    }
    static void decode(Decoder& d, rtip::KuduInsert& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.op);
                    break;
                case 1:
                    avro::decode(d, v.tableName);
                    break;
                case 2:
                    avro::decode(d, v.columns);
                    break;
                case 3:
                    avro::decode(d, v.row);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.op);
            avro::decode(d, v.tableName);
            avro::decode(d, v.columns);
            avro::decode(d, v.row);
        }
    }
};

}
#endif
