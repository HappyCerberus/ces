#!/bin/bash

CXX=g++
WARNINGS="-pedantic -Wall -Wextra -Werror"
WDISABLE=""
DEBUGFLAGS="-g -O0"
RELEASEFLAGS="-O3"
CXXFLAGS="-std=c++20 -fcoroutines -I. ${DEBUGFLAGS} ${WARNINGS} ${WDISABLE}"
LDFLAGS=""

${CXX} ${CXXFLAGS} ${LDFALGS} -o example-http-client example-http-client.cc lib/socket.cc lib/epoll.cc lib/async-ops.cc
