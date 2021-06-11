#!/bin/bash

INPUTFILENAME=${1}
OUTPUTFILENAME=${2}
HASHDEFINE="${3}"

if [ ! -f ${INPUTFILENAME} ]; then
    echo "${0}: File '${INPUTFILENAME}' not found error."
    exit 1
fi

VERSION=$(grep -E '^#version ' ${INPUTFILENAME})
if [ -z "${VERSION}" ]; then
    echo "${0}: Missing '#version ..' from the input file '${INPUTFILENAME}'."
    exit 1
fi

echo "${VERSION}" >${OUTPUTFILENAME} || exit 1
echo "${HASHDEFINE}" >>${OUTPUTFILENAME} || exit 1
grep -vE '^#version ' ${INPUTFILENAME} >>${OUTPUTFILENAME} || exit 1
