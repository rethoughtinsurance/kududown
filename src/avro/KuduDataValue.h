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


#ifndef AVRO_KUDUDATAVALUE_H_321375268__H_
#define AVRO_KUDUDATAVALUE_H_321375268__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace rtip {
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

struct _KuduDataValue_avsc_Union__0__ {
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
    _KuduDataValue_avsc_Union__0__();
};

struct KuduDataValue {
    typedef _KuduDataValue_avsc_Union__0__ value_t;
    KuduDataType dataType;
    value_t value;
    KuduDataValue() :
        dataType(KuduDataType()),
        value(value_t())
        { }
};

inline
std::string _KuduDataValue_avsc_Union__0__::get_string() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::string >(value_);
}

inline
void _KuduDataValue_avsc_Union__0__::set_string(const std::string& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<uint8_t> _KuduDataValue_avsc_Union__0__::get_bytes() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<uint8_t> >(value_);
}

inline
void _KuduDataValue_avsc_Union__0__::set_bytes(const std::vector<uint8_t>& v) {
    idx_ = 2;
    value_ = v;
}

inline _KuduDataValue_avsc_Union__0__::_KuduDataValue_avsc_Union__0__() : idx_(0) { }
}
namespace avro {
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

template<> struct codec_traits<rtip::_KuduDataValue_avsc_Union__0__> {
    static void encode(Encoder& e, rtip::_KuduDataValue_avsc_Union__0__ v) {
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
    static void decode(Decoder& d, rtip::_KuduDataValue_avsc_Union__0__& v) {
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

}
#endif
