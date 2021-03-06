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


#ifndef AVRO_KUDUCOMPARISONPREDICATE_H_321375268__H_
#define AVRO_KUDUCOMPARISONPREDICATE_H_321375268__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace rtip {
enum KuduComparisonOperator {
    EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    IS_NULL,
    IS_NOT_NULL,
};

struct KuduComparisonPredicate {
    KuduComparisonOperator operator;
    std::string dataValue;
    KuduComparisonPredicate() :
        operator(KuduComparisonOperator()),
        dataValue(std::string())
        { }
};

}
namespace avro {
template<> struct codec_traits<rtip::KuduComparisonOperator> {
    static void encode(Encoder& e, rtip::KuduComparisonOperator v) {
		if (v < rtip::EQUAL || v > rtip::IS_NOT_NULL)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for rtip::KuduComparisonOperator and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, rtip::KuduComparisonOperator& v) {
		size_t index = d.decodeEnum();
		if (index < rtip::EQUAL || index > rtip::IS_NOT_NULL)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for rtip::KuduComparisonOperator and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<rtip::KuduComparisonOperator>(index);
    }
};

template<> struct codec_traits<rtip::KuduComparisonPredicate> {
    static void encode(Encoder& e, const rtip::KuduComparisonPredicate& v) {
        avro::encode(e, v.operator);
        avro::encode(e, v.dataValue);
    }
    static void decode(Decoder& d, rtip::KuduComparisonPredicate& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.operator);
                    break;
                case 1:
                    avro::decode(d, v.dataValue);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.operator);
            avro::decode(d, v.dataValue);
        }
    }
};

}
#endif
