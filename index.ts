import * as kududownSchema from './ts/kududown-schema';
import {KUDUDOWN_SCHEMA_TYPES} from './ts/kududown-schema-types';
import {KuduDownSchemaAvscMap} from './ts/kududown-avsc-obj';
// import * as leveldown from "./leveldown";
// console.log(`leveldown typeof: ${typeof leveldown}`);
const leveldown = require('./leveldown');

export {kududownSchema, KUDUDOWN_SCHEMA_TYPES,
  KuduDownSchemaAvscMap, leveldown};

module.exports = leveldown;
