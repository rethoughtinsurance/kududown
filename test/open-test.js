const test       = require('tape')
    , leveldown  = require('../kududown')
    , abstract   = require('abstract-leveldown/abstract/open-test')

// Can probably change the name of leveldown here to kududown and it should work,
// passes the function reference to the abstract.all which runs the test.
// Ideally we can expose this in the index.ts module instead.
abstract.all(leveldown, test)
