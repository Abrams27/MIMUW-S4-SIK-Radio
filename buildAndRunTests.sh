#!/usr/bin/env bash

echo -e "building tests ..."

make clean
make

echo -e "running tests ..."

./programArgumentsParserTest
./defaultRadioProxyArgumentsResolverTest