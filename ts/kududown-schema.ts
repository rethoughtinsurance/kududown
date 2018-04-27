//KuduDown TypeScript Interfaces

export enum KuduOperationType {
  SCAN = 'SCAN',
  INSERT = 'INSERT',
  UPSERT = 'UPSERT',
  DELETE = 'DELETE'
};

export interface KuduOperation {
	id: string;
	operationType: KuduOperationType;
}

export enum KuduDataType {
  BOOLEAN = 'BOOLEAN',
  INT8 = 'INT8',
  INT16 = 'INT16',
  INT32 = 'INT32',
  INT64 = 'INT64',
  UNIXTIME64 = 'UNIXTIME64',
  FLOAT32 = 'FLOAT32',
  FLOAT64 = 'FLOAT64',
  DECIMAL = 'DECIMAL',
  STRING = 'STRING',
  BINARY = 'BINARY'
};

export enum KuduColumnEncoding {
  AUTO_ENCODING = 'AUTO_ENCODING',
  PLAIN_ENCODING = 'PLAIN_ENCODING',
  RLE = 'RLE',
  DICT_ENCODING = 'DICT_ENCODING',
  BIT_SHUFFLE = 'BIT_SHUFFLE',
  PREFIX_ENCODING = 'PREFIX_ENCODING'
};

export interface KuduColumnDef {
	columnName: string;
	dataType: KuduDataType;
	allowNull: boolean;
	encoding?: null | undefined | KuduColumnEncoding;
	isPrimaryKey: boolean;
}

export enum KuduPredicateType {
  COMPARISON = 'COMPARISON',
  IN_LIST = 'IN_LIST',
  IS_NULL = 'IS_NULL',
  IS_NOT_NULL = 'IS_NOT_NULL'
};

export enum KuduComparisonOperator {
  EQUAL = 'EQUAL',
  GREATER = 'GREATER',
  GREATER_EQUAL = 'GREATER_EQUAL',
  LESS = 'LESS',
  LESS_EQUAL = 'LESS_EQUAL',
  IS_NULL = 'IS_NULL',
  IS_NOT_NULL = 'IS_NOT_NULL'
};

export interface KuduComparisonPredicate {
	column: KuduColumnDef;
	operator: KuduComparisonOperator;
	dataValue: string;
}

export interface KuduInListPredicate {
	column: KuduColumnDef;
	valueList: string[];
}

export interface KuduPredicate {
	type: KuduPredicateType;
	column: KuduColumnDef;
	predicate?: null | undefined | KuduComparisonPredicate | KuduInListPredicate;
}

export interface KuduScan {
	op: KuduOperation;
	tableName: string;
	projectedColumnList: KuduColumnDef[];
	predicateList: KuduPredicate[];
}


export interface KuduDataValue {
	dataType: KuduDataType;
	value?: null | undefined | string | Buffer;
}

export interface KuduRow {
	values: KuduDataValue[];
}


export interface KuduScanResult {
	scanId: string;
	rows: KuduRow[];
}


export interface KuduInsert {
	op: KuduOperation;
	tableName: string;
	columns: KuduColumnDef[];
	row: KuduRow;
}


export interface KuduUpsert {
	op: KuduOperation;
	tableName: string;
	columns: KuduColumnDef[];
	row: KuduRow;
}


export interface KuduKey {
	op: KuduOperation;
	tableName: string;
	columns: KuduColumnDef[];
	row: KuduRow;
}


export interface KuduTableDef {
	tableName: string;
	columns: { [index:string]:KuduColumnDef };
}
