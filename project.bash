# Project development settings for bash.

export MAKEFLAGS="-s -j1"
export HISTFILE=$(pwd)/.bash_history

export CXXFLAGS="-W -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align=strict -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wuseless-cast -fdiagnostics-color=always -g"
export LDFLAGS="-g -fdiagnostics-color=always -Og"
history -r
