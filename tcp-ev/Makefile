.PHONY: clean

CXX = clang++
CXX_FLAGS = -Wall
LIB = -lev

all: srv cli

cli: client.cc common.cc common.h
	${CXX} ${CXX_FLAGS} -o $@ client.cc common.cc ${LIB}

srv: server.cc common.cc common.h
	${CXX} ${CXX_FLAGS} -o $@ server.cc common.cc ${LIB}

clean:
	${RM} cli srv

