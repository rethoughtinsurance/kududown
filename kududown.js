const util              = require('util')
    , AbstractKuduDOWN = require('abstract-leveldown').AbstractLevelDOWN

    , binding           = require('bindings')('kududown').kududown

    , ChainedBatch      = require('./chained-batch')
    , Iterator          = require('./iterator')


function KuduDOWN (location) {
  if (!(this instanceof KuduDOWN))
    return new KuduDOWN(location)

  AbstractKuduDOWN.call(this, location)
  this.binding = binding(location)
}
// console.log('typeof KuduDOWN: ' + (typeof KuduDOWN))
util.inherits(KuduDOWN, AbstractKuduDOWN)


KuduDOWN.prototype._open = function (options, callback) {
  this.binding.open(options, callback)
}


KuduDOWN.prototype._close = function (callback) {
  this.binding.close(callback)
}


KuduDOWN.prototype._put = function (key, value, options, callback) {
  this.binding.put(key, value, options, callback)
}


KuduDOWN.prototype._get = function (key, options, callback) {
  this.binding.get(key, options, callback)
}


KuduDOWN.prototype._del = function (key, options, callback) {
  this.binding.del(key, options, callback)
}


KuduDOWN.prototype._chainedBatch = function () {
  return new ChainedBatch(this)
}


KuduDOWN.prototype._batch = function (operations, options, callback) {
  return this.binding.batch(operations, options, callback)
}


KuduDOWN.prototype.approximateSize = function (start, end, callback) {
  if (start == null ||
      end == null ||
      typeof start === 'function' ||
      typeof end === 'function') {
    throw new Error('approximateSize() requires valid `start`, `end` and `callback` arguments')
  }

  if (typeof callback !== 'function') {
    throw new Error('approximateSize() requires a callback argument')
  }

  start = this._serializeKey(start)
  end = this._serializeKey(end)

  this.binding.approximateSize(start, end, callback)
}


KuduDOWN.prototype.compactRange = function (start, end, callback) {
  this.binding.compactRange(start, end, callback)
}


KuduDOWN.prototype.getProperty = function (property) {
  if (typeof property != 'string')
    throw new Error('getProperty() requires a valid `property` argument')

  return this.binding.getProperty(property)
}


KuduDOWN.prototype._iterator = function (options) {
  return new Iterator(this, options)
}


KuduDOWN.destroy = function (location, callback) {
  if (arguments.length < 2)
    throw new Error('destroy() requires `location` and `callback` arguments')

  if (typeof location != 'string')
    throw new Error('destroy() requires a location string argument')

  if (typeof callback != 'function')
    throw new Error('destroy() requires a callback function argument')

  binding.destroy(location, callback)
}


KuduDOWN.repair = function (location, callback) {
  if (arguments.length < 2)
    throw new Error('repair() requires `location` and `callback` arguments')

  if (typeof location != 'string')
    throw new Error('repair() requires a location string argument')

  if (typeof callback != 'function')
    throw new Error('repair() requires a callback function argument')

  binding.repair(location, callback)
}

// Do not export here, do it through index.ts
module.exports = KuduDOWN.default = KuduDOWN
