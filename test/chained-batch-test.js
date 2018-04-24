const test       = require('tape')
    , leveldown  = require('../ts/lib/kududown')
    , abstract   = require('abstract-leveldown/abstract/chained-batch-test')

abstract.all(leveldown, test)
