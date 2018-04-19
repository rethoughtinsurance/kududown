import { Log, Level } from 'ng2-logger';
import { KUDUDOWN_SCHEMA_TYPES } from "./kududown-schema-types";
import * as fs from "fs";
import { TSMap } from "typescript-map";

/**
 * This module converts avsc files in JSON to Javascript objects syntax in a typescript
 * code file.
 */

const logger = Log.create('kududown-avsc-converter', Level.DATA, Level.INFO, Level.ERROR, Level.WARN);
logger.i("Converting AVSC schema files to TypeScript objects");

export module AvscConverter {

if(!process.env.KUDUDOWN_HOME) {
  throw new Error(`The KUDUDOWN_HOME environment variable must be set`);
}

// Build constant for the avroTypes folder.
const avscFolder: string = `${process.env.KUDUDOWN_HOME}/resources/avsc`;

// Create the output file.
export const KUDUDOWN_AVSC_OBJ_FILE: string = `./ts/kududown-avsc-obj.ts`;

export function convert() {

  let fd;

  try {

    // Get the avsc file names.
    let avscSourceFileNames: string[] = [];

    for (let t in KUDUDOWN_SCHEMA_TYPES) {
      if(parseInt(t) >= 0) {
        avscSourceFileNames.push(KUDUDOWN_SCHEMA_TYPES[t] + ".avsc");
      }
    }

    // Init the file.
    fd = fs.openSync(KUDUDOWN_AVSC_OBJ_FILE, "w+", 0o666);

    logger.i(`avscFolder: ${avscFolder}`)
    logger.i(`Initializing ${KUDUDOWN_AVSC_OBJ_FILE} file`);

    // Create the module imports
    fs.appendFileSync(fd, `import { TSMap } from "typescript-map";\n\n`);
    //fs.appendFileSync(RT_AVSC_OBJ_FILE, `\n\n`);

    // Create the module header comment.
    fs.appendFileSync(fd, `//KuduDown Avsc Object Map`);
    fs.appendFileSync(fd, `\n\n`);

    // Declare the map
    fs.appendFileSync(fd, `export const KuduDownSchemaAvscMap = new TSMap<string,any>();`)
    fs.appendFileSync(fd, `\n\n`);

    // For each type, load the JSON from the file, convert to object syntax
    // and add to the ts file.
    for(let t in KUDUDOWN_SCHEMA_TYPES) {
      if(parseInt(t) >= 0) {
        const typeName = KUDUDOWN_SCHEMA_TYPES[t];
        const schemaTextJSON = fs.readFileSync(`${avscFolder}/${typeName}.avsc`, "UTF8");
        logger.i(`Converting typeName: ${typeName}`);

        // Convert to Javascript syntax.
        // The regex finds this: "identifier" :
        // And replaces it with: identifier :
        const schemaTextTS = schemaTextJSON.replace(/"([^"]+(?=" :))" :/g, '$1 :');

        // Add the TS object syntax to the map.
        fs.appendFileSync(fd, `KuduDownSchemaAvscMap.set("${typeName}", ${schemaTextTS});`);
        fs.appendFileSync(fd, `\n\n`);

      }
    }

  } catch(e) {
    logger.error(e.stack);
  } finally {
    if (fd !== undefined)
      fs.closeSync(fd);
      logger.i("AVSC Conversion to TypesScript objects complete");
  }


}

}

AvscConverter.convert();
