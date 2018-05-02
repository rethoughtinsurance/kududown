const test       = require('tape')
    , leveldown  = require('../ts/lib/kududown')
    , abstract   = require('abstract-leveldown/abstract/iterator-test')
    , testCommon    = require('abstract-leveldown/testCommon')

    
var db
  , sourceData = (function () {
      var d = []
        , i = 0
        , k
      for (; i <  5000; i++) {
        k = (i < 10 ? '0' : '') + i
        d.push({
            type  : 'put'
          , key   : k
          , value : Math.random()
        })
      }
      return d
    }())


test('setUp db', function (t) {
  db = leveldown(testCommon.location())
  db.open(function (err) {
    t.error(err)
    db.batch(sourceData, t.end.bind(t))
  })
})

var collectEntries = function (iterator, callback) {
  var data = []
  var next = function () {
    iterator.next(function (err, key, value) {
      if (err) return callback(err)
      if (!arguments.length) {
        return iterator.end(function (err) {
          callback(err, data)
        })
      }
      data.push({ key: key, value: value })
      setTimeout(next, 0)
    })
  }
  next()
}


test("test iterator select all", function (t) {
	var opts = {}
	opts.keyAsBuffer = false
	opts.valueAsBuffer = false
    collectEntries(db.iterator(opts), function (err, result) {
      t.error(err)
      t.is(result.length, expected.length, 'correct number of entries')
      t.same(result, expected)
      t.end()
    })
})
