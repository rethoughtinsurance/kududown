#!/bin/bash

# Run TypeScript generators for kududownSchema interfaces and kududownSchemaAvscMap
# of TypeScript object syntax source for avsc schema files.
echo $PWD
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR
export KUDUDOWN_HOME=$DIR

# Build script to copy resource and other files to lib/ output.
rm -rf ./ts/lib

# Compile rtSchemaAvscMap converter with tsc, to be run before other compilation.
tsc --outDir ./ts/lib/ts ./ts/kududown-avsc-converter 2>&1

# Run node module to generate TypeScript avsc objects.
node ./ts/lib/ts/kududown-avsc-converter 2>&1

# Compile kududownSchema interface generator with tsc, to be run before other compilation.
tsc --outDir ./ts/lib/ts ./ts/kududown-generate-schema 2>&1

# Run node module to generate TypeScript interfaces from avsc files.
node ./ts/lib/ts/kududown-generate-schema 2>&1

# Compile all files
tsc 2>&1
