CC=clang
CXX=clang++
SRCDEP=../pstack.c ../pstack.h
SRC=../pstack.c
CFLAGS=-g -O0 -I..
CXXFLAGS=-g -O0 -I..
LDFLAGS=-rdynamic -ldl

all: segfault.c.exe test.c.exe 	 test.cc.exe \
		hello.c.exe dlopen.c.exe funcptr.c.exe fork.c.exe \
		hello1.c.exe pthread.c.exe

segfault.c.exe: segfault.c ${SRCDEP}
	${CC} ${CFLAGS} segfault.c ${SRC} -o segfault.c.exe ${LDFLAGS}

test.c.exe: test.c ${SRCDEP}
	${CC} ${CFLAGS} test.c ${SRC} -o test.c.exe ${LDFLAGS}

test.cc.exe: test.cc ${SRCDEP}
	${CXX} ${CXXFLAGS} test.cc ${SRC} -o test.cc.exe ${LDFLAGS}

hello.c.exe: hello.c libhello.so
	${CC} ${CFLAGS} -L. hello.c -lhello -o hello.c.exe ${LDFLAGS}

dlopen.c.exe: dlopen.c
	${CC} ${CFLAGS} dlopen.c -o dlopen.c.exe -ldl ${LDFLAGS}

libhello.so: libhello.c ${SRCDEP}
	${CC} ${CFLAGS} -fPIC -shared ${SRC} libhello.c -o libhello.so ${LDFLAGS}

hello1.c.exe: hello1.c libhello1.so ${SRCDEP}
	${CC} ${CFLAGS} -L. ${SRC} hello1.c -lhello1 -o hello1.c.exe ${LDFLAGS}

# no -fPIC
libhello1.so: libhello1.c ${SRCDEP} 
	${CC} ${CFLAGS} -shared libhello1.c -o libhello1.so ${LDFLAGS}

funcptr.c.exe: funcptr.c ${SRCDEP}
	${CC} ${CFLAGS} funcptr.c ${SRC} -o funcptr.c.exe ${LDFLAGS}

fork.c.exe: fork.c ${SRCDEP}
	${CC} ${CFLAGS} fork.c ${SRC} -o fork.c.exe ${LDFLAGS}

pthread.c.exe: pthread.c ${SRCDEP}
	${CC} ${CFLAGS} pthread.c ${SRC} -o pthread.c.exe ${LDFLAGS} -pthread

clean:
	${RM} *.exe *.so
