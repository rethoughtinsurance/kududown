"use strict";
//KuduDown TypeScript Interfaces
Object.defineProperty(exports, "__esModule", { value: true });
var KuduOperationType;
(function (KuduOperationType) {
    KuduOperationType["SCAN"] = "SCAN";
    KuduOperationType["INSERT"] = "INSERT";
    KuduOperationType["DELETE"] = "DELETE";
})(KuduOperationType = exports.KuduOperationType || (exports.KuduOperationType = {}));
;
var KuduDataType;
(function (KuduDataType) {
    KuduDataType["BOOLEAN"] = "BOOLEAN";
    KuduDataType["INT8"] = "INT8";
    KuduDataType["INT16"] = "INT16";
    KuduDataType["INT32"] = "INT32";
    KuduDataType["INT64"] = "INT64";
    KuduDataType["UNIXTIME64"] = "UNIXTIME64";
    KuduDataType["FLOAT32"] = "FLOAT32";
    KuduDataType["FLOAT64"] = "FLOAT64";
    KuduDataType["DECIMAL"] = "DECIMAL";
    KuduDataType["STRING"] = "STRING";
    KuduDataType["BINARY"] = "BINARY";
})(KuduDataType = exports.KuduDataType || (exports.KuduDataType = {}));
;
var KuduColumnEncoding;
(function (KuduColumnEncoding) {
    KuduColumnEncoding["AUTO_ENCODING"] = "AUTO_ENCODING";
    KuduColumnEncoding["PLAIN_ENCODING"] = "PLAIN_ENCODING";
    KuduColumnEncoding["RLE"] = "RLE";
    KuduColumnEncoding["DICT_ENCODING"] = "DICT_ENCODING";
    KuduColumnEncoding["BIT_SHUFFLE"] = "BIT_SHUFFLE";
    KuduColumnEncoding["PREFIX_ENCODING"] = "PREFIX_ENCODING";
})(KuduColumnEncoding = exports.KuduColumnEncoding || (exports.KuduColumnEncoding = {}));
;
var KuduComparisonOperator;
(function (KuduComparisonOperator) {
    KuduComparisonOperator["EQUAL"] = "EQUAL";
    KuduComparisonOperator["GREATER"] = "GREATER";
    KuduComparisonOperator["GREATER_EQUAL"] = "GREATER_EQUAL";
    KuduComparisonOperator["LESS"] = "LESS";
    KuduComparisonOperator["LESS_EQUAL"] = "LESS_EQUAL";
    KuduComparisonOperator["IS_NULL"] = "IS_NULL";
    KuduComparisonOperator["IS_NOT_NULL"] = "IS_NOT_NULL";
})(KuduComparisonOperator = exports.KuduComparisonOperator || (exports.KuduComparisonOperator = {}));
;
