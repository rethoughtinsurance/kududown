import { TSMap } from "typescript-map";

//KuduDown Avsc Object Map

export const kuduDownSchemaAvscMap = new TSMap<string,any>();

kuduDownSchemaAvscMap.set("KuduScan", {
  type : "record",
  name : "KuduScan",
  namespace : "org.kududown.avro",
  fields : [ {
    name : "op",
    type : {
      type : "record",
      name : "KuduOperation",
      fields : [ {
        name : "id",
        type : "string"
      }, {
        name : "operationType",
        type : {
          type : "enum",
          name : "KuduOperationType",
          symbols : [ "SCAN", "INSERT", "DELETE" ]
        }
      } ]
    }
  }, {
    name : "tableName",
    type : "string"
  }, {
    name : "predicateList",
    type : {
      type : "array",
      items : {
        type : "record",
        name : "KuduPredicate",
        fields : [ {
          name : "type",
          type : {
            type : "enum",
            name : "KuduPredicateType",
            symbols : [ "COMPARISON", "IN_LIST", "IS_NULL", "IS_NOT_NULL" ]
          }
        }, {
          name : "column",
          type : {
            type : "record",
            name : "KuduColumnDef",
            fields : [ {
              name : "columnName",
              type : "string"
            }, {
              name : "dataType",
              type : {
                type : "enum",
                name : "KuduDataType",
                symbols : [ "BOOLEAN", "INT8", "INT16", "INT32", "INT64", "UNIXTIME64", "FLOAT32", "FLOAT64", "DECIMAL", "STRING", "BINARY" ]
              }
            }, {
              name : "allowNull",
              type : "boolean"
            }, {
              name : "encoding",
              type : [ "null", {
                type : "enum",
                name : "KuduColumnEncoding",
                symbols : [ "AUTO_ENCODING", "PLAIN_ENCODING", "RLE", "DICT_ENCODING", "BIT_SHUFFLE", "PREFIX_ENCODING" ]
              } ]
            }, {
              name : "isPrimaryKey",
              type : "boolean"
            } ]
          }
        }, {
          name : "predicate",
          type : [ "null", {
            type : "record",
            name : "KuduComparisonPredicate",
            fields : [ {
              name : "operator",
              type : {
                type : "enum",
                name : "KuduComparisonOperator",
                symbols : [ "EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "IS_NULL", "IS_NOT_NULL" ]
              }
            }, {
              name : "dataValue",
              type : "string"
            } ]
          }, {
            type : "record",
            name : "KuduInListPredicate",
            fields : [ {
              name : "valueList",
              type : {
                type : "array",
                items : "string"
              }
            } ]
          } ]
        } ]
      }
    }
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduInsert", {
  type : "record",
  name : "KuduInsert",
  namespace : "org.kududown.avro",
  fields : [ {
    name : "op",
    type : {
      type : "record",
      name : "KuduOperation",
      fields : [ {
        name : "id",
        type : "string"
      }, {
        name : "operationType",
        type : {
          type : "enum",
          name : "KuduOperationType",
          symbols : [ "SCAN", "INSERT", "DELETE" ]
        }
      } ]
    }
  }, {
    name : "tableName",
    type : "string"
  }, {
    name : "columns",
    type : {
      type : "array",
      items : {
        type : "record",
        name : "KuduColumnDef",
        fields : [ {
          name : "columnName",
          type : "string"
        }, {
          name : "dataType",
          type : {
            type : "enum",
            name : "KuduDataType",
            symbols : [ "BOOLEAN", "INT8", "INT16", "INT32", "INT64", "UNIXTIME64", "FLOAT32", "FLOAT64", "DECIMAL", "STRING", "BINARY" ]
          }
        }, {
          name : "allowNull",
          type : "boolean"
        }, {
          name : "encoding",
          type : [ "null", {
            type : "enum",
            name : "KuduColumnEncoding",
            symbols : [ "AUTO_ENCODING", "PLAIN_ENCODING", "RLE", "DICT_ENCODING", "BIT_SHUFFLE", "PREFIX_ENCODING" ]
          } ]
        }, {
          name : "isPrimaryKey",
          type : "boolean"
        } ]
      }
    }
  }, {
    name : "row",
    type : {
      type : "record",
      name : "KuduRow",
      fields : [ {
        name : "values",
        type : {
          type : "array",
          items : {
            type : "record",
            name : "KuduDataValue",
            fields : [ {
              name : "dataType",
              type : "KuduDataType"
            }, {
              name : "value",
              type : [ "null", "string", "bytes" ]
            } ]
          }
        }
      } ]
    }
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduScanResult", {
  type : "record",
  name : "KuduScanResult",
  namespace : "org.kududown.avro",
  fields : [ {
    name : "scanId",
    type : "string"
  }, {
    name : "rows",
    type : {
      type : "array",
      items : {
        type : "record",
        name : "KuduRow",
        fields : [ {
          name : "values",
          type : {
            type : "array",
            items : {
              type : "record",
              name : "KuduDataValue",
              fields : [ {
                name : "dataType",
                type : {
                  type : "enum",
                  name : "KuduDataType",
                  symbols : [ "BOOLEAN", "INT8", "INT16", "INT32", "INT64", "UNIXTIME64", "FLOAT32", "FLOAT64", "DECIMAL", "STRING", "BINARY" ]
                }
              }, {
                name : "value",
                type : [ "null", "string", "bytes" ]
              } ]
            }
          }
        } ]
      }
    }
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduRow", {
  type : "record",
  name : "KuduRow",
  namespace : "org.kududown.avro",
  fields : [ {
    name : "values",
    type : {
      type : "array",
      items : {
        type : "record",
        name : "KuduDataValue",
        fields : [ {
          name : "dataType",
          type : {
            type : "enum",
            name : "KuduDataType",
            symbols : [ "BOOLEAN", "INT8", "INT16", "INT32", "INT64", "UNIXTIME64", "FLOAT32", "FLOAT64", "DECIMAL", "STRING", "BINARY" ]
          }
        }, {
          name : "value",
          type : [ "null", "string", "bytes" ]
        } ]
      }
    }
  } ]
}
);

