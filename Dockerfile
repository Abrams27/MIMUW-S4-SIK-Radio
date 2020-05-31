FROM mimuw.sik.abrams/ubuntu-libs:1_0

COPY src/ radio/src
COPY buildAndRunTests.sh radio/buildAndRunTests.sh
COPY Makefile radio/Makefile

ENTRYPOINT bin/bash