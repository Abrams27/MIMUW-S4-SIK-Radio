#!/usr/bin/env bash

function validateOutput() {
  if diff out $1 >/dev/null
  then
    echo -e -n "out passed"
  else
    echo -e "invalid out\n"
    exit 1
  fi


  if diff err $2 >/dev/null
  then
    echo -e ", err passed"
  else
    echo -e ", invalid err\n"
    exit 1
  fi
}

echo -e "-- responseResolverBodyParsingTest tests --"

echo -e -n "should parse body without metadata: "
./responseResolverNoMetadataTest 1> out 2> err
validateOutput src/test/cc/response-resolver-tests/output/responseResolverNoMetadataTest.out src/test/cc/response-resolver-tests/output/responseResolverNoMetadataTest.err

echo -e -n "should parse body with metadata: "
./responseResolverMetadataTest 1> out 2> err
validateOutput src/test/cc/response-resolver-tests/output/responseResolverMetadataTest.out src/test/cc/response-resolver-tests/output/responseResolverMetadataTest.err


echo -e "-- all tests passed --\n"