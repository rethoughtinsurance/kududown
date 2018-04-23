const test       = require('tape')
    , leveldown  = require('../ts/lib/kududown')
    , abstract   = require('abstract-leveldown/abstract/put-test')

abstract.all(leveldown, test)
