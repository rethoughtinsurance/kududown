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


#ifndef AVRO_KUDUSCHEMATYPE_H_321375268__H_
#define AVRO_KUDUSCHEMATYPE_H_321375268__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace rtip {
enum KuduSchemaType {
    KuduScan,
    KuduInsert,
    KuduScanResult,
};

}
namespace avro {
template<> struct codec_traits<rtip::KuduSchemaType> {
    static void encode(Encoder& e, rtip::KuduSchemaType v) {
		if (v < rtip::KuduScan || v > rtip::KuduScanResult)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for rtip::KuduSchemaType and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, rtip::KuduSchemaType& v) {
		size_t index = d.decodeEnum();
		if (index < rtip::KuduScan || index > rtip::KuduScanResult)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for rtip::KuduSchemaType and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<rtip::KuduSchemaType>(index);
    }
};

}
#endif
