"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var ng2_logger_1 = require("ng2-logger");
var avro_typescript_1 = require("avro-typescript");
var kududown_schema_types_1 = require("./kududown-schema-types");
var fs = require("fs");
var Generator;
(function (Generator) {
    var logger = ng2_logger_1.Log.create('kududown-generate-schema', ng2_logger_1.Level.DATA, ng2_logger_1.Level.INFO, ng2_logger_1.Level.ERROR, ng2_logger_1.Level.WARN);
    if (!process.env.KUDUDOWN_HOME) {
        throw new Error("The KUDUDOWN_HOME environment variable must be set");
    }
    var avscFolder = process.env.KUDUDOWN_HOME + "/resources/avsc/";
    // Build an array of avsc filenames to generate based on KUDUDOWN_SCHEMA_TYPES.
    var avscSourceFileNames = [];
    for (var t in kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES) {
        if (parseInt(t) >= 0) {
            avscSourceFileNames.push(kududown_schema_types_1.KUDUDOWN_SCHEMA_TYPES[t] + ".avsc");
        }
    }
    function generate() {
        //logger.info("Generating Typescript interfaces in kududown-schema.ts from avsc definintions.");
        //logger.data(`avsc schema folder: ${avscFolder}`);
        var typeNames = [];
        var kududownSchemaFile = "./ts/kududown-schema.ts";
        // Init the file.
        if (fs.existsSync(kududownSchemaFile)) {
            fs.unlinkSync(kududownSchemaFile);
        }
        // Create the module header comment.
        fs.appendFileSync(kududownSchemaFile, "//KuduDown TypeScript Interfaces");
        // Output the interface for each top-level type.
        for (var _i = 0, avscSourceFileNames_1 = avscSourceFileNames; _i < avscSourceFileNames_1.length; _i++) {
            var avscFileName = avscSourceFileNames_1[_i];
            //logger.info(`Generating: ${avscFileName}`);
            var avscString = fs.readFileSync("" + avscFolder + avscFileName, "UTF8");
            var avscSchema = JSON.parse(avscString);
            var avroToTypeScriptResult = avro_typescript_1.avroToTypeScript(avscSchema, typeNames);
            fs.appendFileSync(kududownSchemaFile, "\n\n");
            fs.appendFileSync(kududownSchemaFile, avroToTypeScriptResult.tsInterface);
        }
        //logger.info("Typescript interface generation complete.");
    }
    Generator.generate = generate;
})(Generator = exports.Generator || (exports.Generator = {}));
Generator.generate();
