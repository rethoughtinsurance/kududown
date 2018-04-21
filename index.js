"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var kududownSchema = require("./ts/kududown-schema");
exports.kududownSchema = kududownSchema;
var kududown_schema_types_1 = require("./ts/kududown-schema-types");
exports.KUDUDOWN_SCHEMA_TYPES = kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES;
var kududown_avsc_obj_1 = require("./ts/kududown-avsc-obj");
exports.KuduDownSchemaAvscMap = kududown_avsc_obj_1.KuduDownSchemaAvscMap;
console.log("Top of index before kududown load");
// This was a hack, it overrides module.exports which means all of the
// typescript generated exports are gone.
//module.exports = leveldown;
