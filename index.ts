import * as kududownSchema from './ts/kududown-schema';
import {KUDUDOWN_SCHEMA_TYPES} from './ts/kududown-schema-types';
import {kuduDownSchemaAvscMap} from './ts/kududown-avsc-obj';
import * as levelupTypes from './ts/levelup-types';

console.log(`Top of index before kududown load`);
// Get the levelDOWN function for export, which should be exported as leveldown
// The tests require the name `leveldown` but since consumers will use
// both the real `leveldown` and this module this will not work. It means
// changing all the tests. Therefore, this is renamed to `kududown` so
// the export is a different name.

// Must be commented until the kududown.node loads.
// And we should probably change all of the names to kududown.
const KuduDOWN = require('./kududown').KuduDOWN;

export {KuduDOWN};

export {kududownSchema, KUDUDOWN_SCHEMA_TYPES,
  kuduDownSchemaAvscMap, levelupTypes};
