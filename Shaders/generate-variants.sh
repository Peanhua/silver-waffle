#!/bin/bash

INPUTFILENAME=${1}
OUTPUTFILENAME=${2}
OPTIONS="${3}"

if [ ! -f ${INPUTFILENAME} ]; then
    echo "${0}: File '${INPUTFILENAME}' not found error."
    exit 1
fi

TMPFILENAME="${OUTPUTFILENAME}.tmp"
echo "// Generated with: ${0} ${*}" >${TMPFILENAME} || exit 1
echo "" >>${TMPFILENAME}                            || exit 1
for option in ${OPTIONS} ; do
    echo "#define ${option}" >>${TMPFILENAME}       || exit 1
done
cat ${INPUTFILENAME} >>${TMPFILENAME}               || exit 1

mv ${TMPFILENAME} ${OUTPUTFILENAME}
