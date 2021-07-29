#!/bin/bash

INPUTFILENAME=${1}
OUTPUTFILENAME=${2}
OPTIONS="${3}"

INCLUDES_AWK=$(dirname ${0})/includes.awk
if [ ! -f ${INCLUDES_AWK} ]; then
    echo "${0}: Error, file includes.awk not found from '${INCLUDES.AWK}', pwd='$(pwd)'"
    exit 1
fi
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
echo "" >>${TMPFILENAME}
echo "// Generated with: ${0} ${*}" >>${TMPFILENAME}
echo "" >>${TMPFILENAME}
for option in ${OPTIONS} ; do
    echo "#define ${option}" >>${TMPFILENAME} || exit 1
done
grep -vE '^#version ' ${INPUTFILENAME} >>${TMPFILENAME} || exit 1

awk -f ${INCLUDES_AWK} <${TMPFILENAME} >${OUTPUTFILENAME} || exit 1

rm ${TMPFILENAME}
exit 0
