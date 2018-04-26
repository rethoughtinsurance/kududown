import { TSMap } from "typescript-map";

//KuduDown Avsc Object Map

export const kuduDownSchemaAvscMap = new TSMap<string,any>();

kuduDownSchemaAvscMap.set("KuduColumnDef", {
  type : "record",
  name : "KuduColumnDef",
  namespace : "org.kududown.avro",
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
);

kuduDownSchemaAvscMap.set("KuduColumnEncoding", {
  type : "enum",
  name : "KuduColumnEncoding",
  namespace : "org.kududown.avro",
  symbols : [ "AUTO_ENCODING", "PLAIN_ENCODING", "RLE", "DICT_ENCODING", "BIT_SHUFFLE", "PREFIX_ENCODING" ]
}
);

kuduDownSchemaAvscMap.set("KuduComparisonOperator", {
  type : "enum",
  name : "KuduComparisonOperator",
  namespace : "org.kududown.avro",
  symbols : [ "EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "IS_NULL", "IS_NOT_NULL" ]
}
);

kuduDownSchemaAvscMap.set("KuduComparisonPredicate", {
  type : "record",
  name : "KuduComparisonPredicate",
  namespace : "org.kududown.avro",
  fields : [ {
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
}
);

kuduDownSchemaAvscMap.set("KuduDataType", {
  type : "enum",
  name : "KuduDataType",
  namespace : "org.kududown.avro",
  symbols : [ "BOOLEAN", "INT8", "INT16", "INT32", "INT64", "UNIXTIME64", "FLOAT32", "FLOAT64", "DECIMAL", "STRING", "BINARY" ]
}
);

kuduDownSchemaAvscMap.set("KuduDataValue", {
  type : "record",
  name : "KuduDataValue",
  namespace : "org.kududown.avro",
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
);

kuduDownSchemaAvscMap.set("KuduInListPredicate", {
  type : "record",
  name : "KuduInListPredicate",
  namespace : "org.kududown.avro",
  fields : [ {
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
    name : "valueList",
    type : {
      type : "array",
      items : "string"
    }
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduInsert", {
  type : "record",
  name : "KuduInsert",
  namespace : "org.kududown.avro",
  doc : "* A KuduInsert KuduRow contains all requested columns, primary key or otherwise.",
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
          symbols : [ "SCAN", "INSERT", "UPSERT", "DELETE" ]
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
      doc : "* KuduRow holds any row of arbitrary columns for writing or reading.",
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

kuduDownSchemaAvscMap.set("KuduKey", {
  type : "record",
  name : "KuduKey",
  namespace : "org.kududown.avro",
  doc : "* A KuduKey is similar to KuduInsert but contains only primary key columns,\n   * it can be passed to/from put/get/delete, and optionally returned from a\n   * kududown iterator if desired (although not much practical use for\n   * that, normally only values will be requested).\n   * Note: There is no KuduDelete operation, it is just a KuduKey passed\n   * to a leveldown.delete.",
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
          symbols : [ "SCAN", "INSERT", "UPSERT", "DELETE" ]
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
      doc : "* KuduRow holds any row of arbitrary columns for writing or reading.",
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

kuduDownSchemaAvscMap.set("KuduMessage", {
  type : "record",
  name : "KuduMessage",
  namespace : "org.kududown.avro",
  fields : [ {
    name : "schemaType",
    type : {
      type : "enum",
      name : "KuduSchemaType",
      symbols : [ "KuduScan", "KuduRow", "KuduScanResult", "KuduInsert", "KuduUpsert", "KuduKey" ]
    }
  }, {
    name : "obj",
    type : "bytes"
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduOperation", {
  type : "record",
  name : "KuduOperation",
  namespace : "org.kududown.avro",
  fields : [ {
    name : "id",
    type : "string"
  }, {
    name : "operationType",
    type : {
      type : "enum",
      name : "KuduOperationType",
      symbols : [ "SCAN", "INSERT", "UPSERT", "DELETE" ]
    }
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduOperationType", {
  type : "enum",
  name : "KuduOperationType",
  namespace : "org.kududown.avro",
  symbols : [ "SCAN", "INSERT", "UPSERT", "DELETE" ]
}
);

kuduDownSchemaAvscMap.set("KuduPredicate", {
  type : "record",
  name : "KuduPredicate",
  namespace : "org.kududown.avro",
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
        name : "column",
        type : "KuduColumnDef"
      }, {
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
        name : "column",
        type : "KuduColumnDef"
      }, {
        name : "valueList",
        type : {
          type : "array",
          items : "string"
        }
      } ]
    } ]
  } ]
}
);

kuduDownSchemaAvscMap.set("KuduPredicateType", {
  type : "enum",
  name : "KuduPredicateType",
  namespace : "org.kududown.avro",
  symbols : [ "COMPARISON", "IN_LIST", "IS_NULL", "IS_NOT_NULL" ]
}
);

kuduDownSchemaAvscMap.set("KuduRow", {
  type : "record",
  name : "KuduRow",
  namespace : "org.kududown.avro",
  doc : "* KuduRow holds any row of arbitrary columns for writing or reading.",
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
          symbols : [ "SCAN", "INSERT", "UPSERT", "DELETE" ]
        }
      } ]
    }
  }, {
    name : "tableName",
    type : "string"
  }, {
    name : "projectedColumnList",
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
          type : "KuduColumnDef"
        }, {
          name : "predicate",
          type : [ "null", {
            type : "record",
            name : "KuduComparisonPredicate",
            fields : [ {
              name : "column",
              type : "KuduColumnDef"
            }, {
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
              name : "column",
              type : "KuduColumnDef"
            }, {
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

kuduDownSchemaAvscMap.set("KuduScanResult", {
  type : "record",
  name : "KuduScanResult",
  namespace : "org.kududown.avro",
  doc : "* KuduScanResult is only used for test data stored on disk for a query.\n   * The kududown API only returns KuduRow values.",
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
        doc : "* KuduRow holds any row of arbitrary columns for writing or reading.",
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

kuduDownSchemaAvscMap.set("KuduSchemaType", {
  type : "enum",
  name : "KuduSchemaType",
  namespace : "org.kududown.avro",
  symbols : [ "KuduScan", "KuduRow", "KuduScanResult", "KuduInsert", "KuduUpsert", "KuduKey" ]
}
);

kuduDownSchemaAvscMap.set("KuduUpsert", {
  type : "record",
  name : "KuduUpsert",
  namespace : "org.kududown.avro",
  doc : "* A KuduUpsert KuduRow contains all requested columns, primary key or otherwise.",
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
          symbols : [ "SCAN", "INSERT", "UPSERT", "DELETE" ]
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
      doc : "* KuduRow holds any row of arbitrary columns for writing or reading.",
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

