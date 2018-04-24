const test       = require('tape')
    , leveldown  = require('../ts/lib/kududown')
    , abstract   = require('abstract-leveldown/abstract/iterator-range-test')

abstract.all(leveldown, test)
