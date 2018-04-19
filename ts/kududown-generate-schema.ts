import { Log, Level } from 'ng2-logger';
import { avroToTypeScript, RecordType, avroToTypeScriptResult } from "avro-typescript";
import { KUDUDOWN_SCHEMA_TYPES } from "./kududown-schema-types";
import * as fs from "fs";

export module Generator {

const logger = Log.create('kududown-generate-schema', Level.DATA, Level.INFO, Level.ERROR, Level.WARN);

if(!process.env.KUDUDOWN_HOME) {
  throw new Error(`The KUDUDOWN_HOME environment variable must be set`);
}

const avscFolder: string = `${process.env.KUDUDOWN_HOME}/resources/avsc/`;

// Build an array of avsc filenames to generate based on KUDUDOWN_SCHEMA_TYPES.

let avscSourceFileNames: string[] = [];

for (let t in KUDUDOWN_SCHEMA_TYPES) {
  if(parseInt(t) >= 0) {
    avscSourceFileNames.push(KUDUDOWN_SCHEMA_TYPES[t] + ".avsc");
  }
}

export function generate(): void {

  //logger.info("Generating Typescript interfaces in kududown-schema.ts from avsc definintions.");
  //logger.data(`avsc schema folder: ${avscFolder}`);

  let typeNames: string[] = [];

  const kududownSchemaFile: string = "./ts/kududown-schema.ts";


    // Init the file.
    if (fs.existsSync(kududownSchemaFile)) {
      fs.unlinkSync(kududownSchemaFile);
    }

    // Create the module header comment.
    fs.appendFileSync(kududownSchemaFile, "//KuduDown TypeScript Interfaces");

    // Output the interface for each top-level type.
    for(let avscFileName of avscSourceFileNames) {
      //logger.info(`Generating: ${avscFileName}`);
      let avscString = fs.readFileSync(`${avscFolder}${avscFileName}`, "UTF8");
      let avscSchema = JSON.parse(avscString) as RecordType;
      let avroToTypeScriptResult: avroToTypeScriptResult = avroToTypeScript(avscSchema as RecordType,
        typeNames);
      fs.appendFileSync(kududownSchemaFile, "\n\n");
      fs.appendFileSync(kududownSchemaFile, avroToTypeScriptResult.tsInterface);
    }

    //logger.info("Typescript interface generation complete.");

}

}

Generator.generate();
