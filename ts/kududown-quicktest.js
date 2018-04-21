"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
// A quick test to see if things load properly from index
var ng2_logger_1 = require("ng2-logger");
var index_1 = require("../index");
// import {kududown} from '../index';
var logger = ng2_logger_1.Log.create('kududown-quicktest', ng2_logger_1.Level.DATA, ng2_logger_1.Level.INFO, ng2_logger_1.Level.ERROR, ng2_logger_1.Level.WARN);
var type = index_1.kududownSchema.KuduDataType.BOOLEAN;
logger.d("type: " + type);
// This won't run until the library loads.
// function openLeveldownDb() {
//   logger.d(`Attempt to open database with invalid location`);
//   const db = kududown('xxxx');
// }
//
// openLeveldownDb();
