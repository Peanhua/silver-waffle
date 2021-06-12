#!/bin/bash

INPUTFILENAME=${1}
OUTPUTFILENAME=${2}
OPTIONS="${3}"

if [ ! -f ${INPUTFILENAME} ]; then
    echo "${0}: File '${INPUTFILENAME}' not found error."
    exit 1
fi

VERSION=$(grep -E '^#version ' ${INPUTFILENAME})
if [ -z "${VERSION}" ]; then
    echo "${0}: Missing '#version ..' from the input file '${INPUTFILENAME}'."
    exit 1
fi

TMPFILENAME="${OUTPUTFILENAME}.tmp"
echo "${VERSION}" >${TMPFILENAME} || exit 1
for option in ${OPTIONS} ; do
    echo "#define ${option}" >>${TMPFILENAME} || exit 1
done
grep -vE '^#version ' ${INPUTFILENAME} >>${TMPFILENAME} || exit 1

awk -f includes.awk < ${TMPFILENAME} > ${OUTPUTFILENAME} || exit 1

rm ${TMPFILENAME}
exit 0
