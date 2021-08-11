# Project settings for bash.


MYHISTFILE=$(pwd)/.bash_history
if [ "${HISTFILE}" != ${MYHISTFILE} ]; then
    export HISTFILE=${MYHISTFILE}
    history -r
    alias blender='MESA_LOADER_DRIVER_OVERRIDE=i965 /usr/bin/blender'
fi
    

PROFILE=dev

if [ -n "${1}" ]; then
    PROFILE=${1}
fi

if [ -f Makefile ]; then
    make distclean
fi

echo "Using profile: ${PROFILE}"
export MAKEFLAGS="-s -j7"
case ${PROFILE} in
    dev)
        export CXXFLAGS="-Wfatal-errors -W -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align=strict -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wuseless-cast -fdiagnostics-color=always -g"
        export LDFLAGS="-g -fdiagnostics-color=always -Og -rdynamic"
        ;;
    optimized)
        export CXXFLAGS="-Wfatal-errors -O3 -DNDEBUG -flto -W -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align=strict -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wuseless-cast -fdiagnostics-color=always"
        export LDFLAGS="-O3 -DNDEBUG -flto"
        ;;
    valgrind)
        export CXXFLAGS="-Wfatal-errors -O3 -W -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align=strict -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wuseless-cast -fdiagnostics-color=always -g -DNDEBUG -Wno-unused"
        export LDFLAGS="-g -fdiagnostics-color=always -O3 -rdynamic -DNDEBUG"
        ;;
    *)
        echo "Error, unknown profile '${PROFILE}'."
        ;;
esac

USE_SANITIZERS=0
if [ ${USE_SANITIZERS} -ne 0 ]; then
    CXXFLAGS+=" -fsanitize-address"
    LDFLAGS+=" -fsanitize-address"
fi


autoreconf --include=m4 --install
./configure --prefix=$(pwd) --with-valgrind
#--with-gpu-thread
