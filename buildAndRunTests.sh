#!/usr/bin/env bash

echo -e "building..."

make clean
make

echo -e "running..."

./programArgumentsParserTest