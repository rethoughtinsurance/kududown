kududown
=========

## Current implementation

In the repository https://github.com/rethoughtinsurance/kududown the develop branch has
the current implementation which is a port of the leveldown module https://github.com/Level/leveldown.

This is based on the NAN API at https://github.com/nodejs/nan. It uses header files to wrap
the Node V8 C++ calls.

## New Implementation

We are moving to a new implementation based on N-API.

I've looked at two options available:

##### Use N-API directly (https://nodejs.org/dist/latest-v10.x/docs/api/n-api.html)


##### Use node-addon-api, a package of header files based on N-API (https://github.com/nodejs/node-addon-api).

To install the `node-addon-api` package use the command:
> yarn add node-addon-api --save

#### Working example
See the test case at n-api-kududown/src/testnapi.cc and n-api-kududown/test/testnapi.js.

# Building

## Install Dev Tools
### NPM
Install npm as appropriate for your OS

Once npm is installed, it can be used to install the other required tools.

### Typescript
npm i typescript --global

### Node-gyp 
npm i node-gyp --global

### Yarn
npm i yarn --global

## Install required npm packages using YARN
Install the required packages:
> cd <kududown_repo>
> yarn install

To rebuild or build the C++ add on at anytime:

> node-gyp rebuild

Or to just build the changed source in the add on:
> node-gyp build

To see the compile and link lines from the above commands use the '-v' flag.
> node-gyp build -v

# Running the testsuite
Running the tests is easiest using yarn and the `scripts` in the `package.json` file.
> yarn run test

I've also added a few script commands to run individual tests. For example:
> yarn run open-test

will run the tests in `test/open-test.js`.

To run them individually from the command line ensure that the [tape](https://www.npmjs.com/package/tape) executable's directory is in your `PATH`. Then invoke it like this:

> tape test/open-test.js

