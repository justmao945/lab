
all: fdbuf_test.exe

fdbuf_test.exe: fdbuf_test.cc fdbuf.h 
	clang++ -g -stdlib=libc++ fdbuf_test.cc -o fdbuf_test.exe -fprofile-arcs -ftest-coverage

clean:
	${RM} *.exe *.gc*
