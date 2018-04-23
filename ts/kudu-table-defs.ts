import * as kududownSchema from './kududown-schema';


// Kudu table definitions.
//


// CREATE TABLE IF NOT EXISTS rtip_object_index
// (
//   type_name STRING ENCODING DICT_ENCODING,
//   key STRING ENCODING DICT_ENCODING,
//   value STRING,
//   id BIGINT,
//   version_id BIGINT,
//   case_file_id BIGINT,
//   case_file_version_id BIGINT,
//   PRIMARY KEY (type_name, key, value, id, version_id)
// )

export interface IrTipObjectIndex {
  typeName: kududownSchema.KuduColumnDef,
  key: kududownSchema.KuduColumnDef,
  value: kududownSchema.KuduColumnDef,
  id: kududownSchema.KuduColumnDef,
  versionId: kududownSchema.KuduColumnDef,
  caseFileId: kududownSchema.KuduColumnDef,
  caseFileVersionId: kududownSchema.KuduColumnDef,
}

export const rTipObjectIndex : IrTipObjectIndex  = {
  typeName: {
    columnName: 'type_name',
    dataType: kududownSchema.KuduDataType.STRING,
    allowNull: false,
    isPrimaryKey: true
  },
  key: {
    columnName: 'key',
    dataType: kududownSchema.KuduDataType.STRING,
    allowNull: false,
    isPrimaryKey: true
  },
  value: {
    columnName: 'value',
    dataType: kududownSchema.KuduDataType.STRING,
    allowNull: false,
    isPrimaryKey: true
  },
  id: {
    columnName: 'id',
    dataType: kududownSchema.KuduDataType.INT64,
    allowNull: false,
    isPrimaryKey: true
  },
  versionId: {
    columnName: 'version_id',
    dataType: kududownSchema.KuduDataType.INT64,
    allowNull: false,
    isPrimaryKey: true
  },
  caseFileId: {
    columnName: 'case_file_id',
    dataType: kududownSchema.KuduDataType.INT64,
    allowNull: false,
    isPrimaryKey: false
  },
  caseFileVersionId: {
    columnName: 'case_file_version_id',
    dataType: kududownSchema.KuduDataType.INT64,
    allowNull: false,
    isPrimaryKey: false
  }

}

// CREATE TABLE IF NOT EXISTS rtip_object
// (
//   id BIGINT,
//   version_id BIGINT,
//   object_data STRING, // Change to BINARY, cannot run through Impala or FlywayDB
//   PRIMARY KEY(id, version_id)
// )

export interface IrTipObject {
  id: kududownSchema.KuduColumnDef,
  versionId: kududownSchema.KuduColumnDef,
  objectData: kududownSchema.KuduColumnDef,
}

export const rTipObject: IrTipObject = {
  id: {
    columnName: 'id',
    dataType: kududownSchema.KuduDataType.INT64,
    allowNull: false,
    isPrimaryKey: true
  },
  versionId: {
    columnName: 'version_id',
    dataType: kududownSchema.KuduDataType.INT64,
    allowNull: false,
    isPrimaryKey: true
  },
  objectData: {
    columnName: 'object_data',
    dataType: kududownSchema.KuduDataType.BINARY,
    allowNull: false,
    isPrimaryKey: false
  }

}
