// A quick test to see if things load properly from index
import { Log, Level } from 'ng2-logger';
import {kududownSchema, KUDUDOWN_SCHEMA_TYPES,
  KuduDownSchemaAvscMap} from '../index';
// import {kududown} from '../index';

const logger = Log.create('kududown-quicktest', Level.DATA, Level.INFO, Level.ERROR, Level.WARN);
const type: kududownSchema.KuduDataType = kududownSchema.KuduDataType.BOOLEAN;

logger.d(`type: ${type}`);

// This won't run until the library loads.
// function openLeveldownDb() {
//   logger.d(`Attempt to open database with invalid location`);
//   const db = kududown('xxxx');
// }
//
// openLeveldownDb();
