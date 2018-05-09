const test       = require('tape')
    , testCommon    = require('abstract-leveldown/testCommon')
    , leveldown  = require('../ts/lib/kududown')
    , abstract   = require('abstract-leveldown/abstract/batch-test')

    var isTypedArray = require('abstract-leveldown/abstract/util').isTypedArray
    
var db;

test('setUp db', function (t) {
  db = leveldown(testCommon.location())
  db.open(function (err) {
    t.error(err)
    t.end()
  })
})


  test('test batch() with empty array', function (t) {
    db.batch([], function (err) {
      t.error(err)
      t.end()
    })
  })

  test('test simple batch()', function (t) {
    db.batch([{ type: 'put', key: 'foo', value: 'bar' }], function (err) {
      t.error(err)

      db.get('foo', function (err, value) {
        t.error(err)
        var result
        if (isTypedArray(value)) {
        	console.log("###########it is a type array");
        	console.log("##VALUE is " + value)
          result = String.fromCharCode.apply(null, new Uint16Array(value))
        } else {
          t.ok(typeof Buffer !== 'undefined' && value instanceof Buffer)
          result = value.toString()
        }
        t.equal(result, 'bar')
        t.end()
      })
    })
  })

  test('test multiple batch()', function (t) {
    db.batch([
      { type: 'put', key: 'foobatch1', value: 'bar1' },
      { type: 'put', key: 'foobatch2', value: 'bar2' },
      { type: 'put', key: 'foobatch3', value: 'bar3' },
      { type: 'del', key: 'foobatch2' }
    ], function (err) {
      t.error(err)

      var r = 0
      var done = function () {
        if (++r === 3) { t.end() }
      }

      db.get('foobatch1', function (err, value) {
    	  console.log("In callback for foobatch1")
        t.error(err)
        var result
        if (isTypedArray(value)) {
        	console.log("###########it is a type array");
        	console.log("##VALUE is " + value)
          result = String.fromCharCode.apply(null, new Uint16Array(value))
        } else {
          t.ok(typeof Buffer !== 'undefined' && value instanceof Buffer)
          result = value.toString()
        }
        t.equal(result, 'bar1')
        done()
      })

      db.get('foobatch2', function (err, value) {
    	  console.log("In callback for foobatch2")
        t.ok(err, 'entry not found')
        t.ok(typeof value === 'undefined', 'value is undefined')
        t.ok(verifyNotFoundError(err), 'NotFound error')
        done()
      })

      db.get('foobatch3', function (err, value) {
    	  console.log("In callback for foobatch3")
        t.error(err)
        var result2
        if (isTypedArray(value)) {
          result2 = String.fromCharCode.apply(null, new Uint16Array(value))
        } else {
          t.ok(typeof Buffer !== 'undefined' && value instanceof Buffer)
          result2 = value.toString()
        }
        t.equal(result2, 'bar3')
        done()
      })
    })
  })
  
