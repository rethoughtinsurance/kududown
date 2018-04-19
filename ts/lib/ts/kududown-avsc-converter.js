"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var ng2_logger_1 = require("ng2-logger");
var kududown_schema_types_1 = require("./kududown-schema-types");
var fs = require("fs");
/**
 * This module converts avsc files in JSON to Javascript objects syntax in a typescript
 * code file.
 */
var logger = ng2_logger_1.Log.create('kududown-avsc-converter', ng2_logger_1.Level.DATA, ng2_logger_1.Level.INFO, ng2_logger_1.Level.ERROR, ng2_logger_1.Level.WARN);
logger.i("Converting AVSC schema files to TypeScript objects");
var AvscConverter;
(function (AvscConverter) {
    if (!process.env.KUDUDOWN_HOME) {
        throw new Error("The KUDUDOWN_HOME environment variable must be set");
    }
    // Build constant for the avroTypes folder.
    var avscFolder = process.env.KUDUDOWN_HOME + "/resources/avsc";
    // Create the output file.
    AvscConverter.KUDUDOWN_AVSC_OBJ_FILE = "./ts/kududown-avsc-obj.ts";
    function convert() {
        var fd;
        try {
            // Get the avsc file names.
            var avscSourceFileNames = [];
            for (var t in kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES) {
                if (parseInt(t) >= 0) {
                    avscSourceFileNames.push(kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES[t] + ".avsc");
                }
            }
            // Init the file.
            fd = fs.openSync(AvscConverter.KUDUDOWN_AVSC_OBJ_FILE, "w+", 438);
            logger.i("avscFolder: " + avscFolder);
            logger.i("Initializing " + AvscConverter.KUDUDOWN_AVSC_OBJ_FILE + " file");
            // Create the module imports
            fs.appendFileSync(fd, "import { TSMap } from \"typescript-map\";\n\n");
            //fs.appendFileSync(RT_AVSC_OBJ_FILE, `\n\n`);
            // Create the module header comment.
            fs.appendFileSync(fd, "//KuduDown Avsc Object Map");
            fs.appendFileSync(fd, "\n\n");
            // Declare the map
            fs.appendFileSync(fd, "export const KuduDownSchemaAvscMap = new TSMap<string,any>();");
            fs.appendFileSync(fd, "\n\n");
            // For each type, load the JSON from the file, convert to object syntax
            // and add to the ts file.
            for (var t in kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES) {
                if (parseInt(t) >= 0) {
                    var typeName = kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES[t];
                    var schemaTextJSON = fs.readFileSync(avscFolder + "/" + typeName + ".avsc", "UTF8");
                    logger.i("Converting typeName: " + typeName);
                    // Convert to Javascript syntax.
                    // The regex finds this: "identifier" :
                    // And replaces it with: identifier :
                    var schemaTextTS = schemaTextJSON.replace(/"([^"]+(?=" :))" :/g, '$1 :');
                    // Add the TS object syntax to the map.
                    fs.appendFileSync(fd, "KuduDownSchemaAvscMap.set(\"" + typeName + "\", " + schemaTextTS + ");");
                    fs.appendFileSync(fd, "\n\n");
                }
            }
        }
        catch (e) {
            logger.error(e.stack);
        }
        finally {
            if (fd !== undefined)
                fs.closeSync(fd);
            logger.i("AVSC Conversion to TypesScript objects complete");
        }
    }
    AvscConverter.convert = convert;
})(AvscConverter = exports.AvscConverter || (exports.AvscConverter = {}));
AvscConverter.convert();
