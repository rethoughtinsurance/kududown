const test       = require('tape')
    , leveldown  = require('../ts/lib/kududown')
    , abstract   = require('abstract-leveldown/abstract/get-test')

abstract.all(leveldown, test)
