import * as kududownSchema from './ts/kududown-schema';
import {KUDUDOWN_SCHEMA_TYPES} from './ts/kududown-schema-types';
import {KuduDownSchemaAvscMap} from './ts/kududown-avsc-obj';

console.log(`Top of index before kududown load`);
// Get the levelDOWN function for export, which should be exported as leveldown
// The tests require the name `leveldown` but since consumers will use
// both the real `leveldown` and this module this will not work. It means
// changing all the tests. Therefore, this is renamed to `kududown` so
// the export is a different name.

// Must be commented until the leveldown.node loads.
// And we should probably change all of the names to kududown.
//const kududown = require('./leveldown').levelDOWN;

export {kududownSchema, KUDUDOWN_SCHEMA_TYPES,
  KuduDownSchemaAvscMap};

// This was a hack, it overrides module.exports which means all of the
// typescript generated exports are gone.
//module.exports = leveldown;
