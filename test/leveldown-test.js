const test      = require('tape')
    , leveldown = require('../ts/lib/kududown')
    , abstract  = require('abstract-leveldown/abstract/leveldown-test')

abstract.args(leveldown, test)
