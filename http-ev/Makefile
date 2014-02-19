.PHONY: clean

CC = clang
CC_FLAGS = -Wall -O2
LIB = -lev -lssl -lcrypto  

all: srv srv_en

srv: server.c parser.c parser.h
	${CC} ${CC_FLAGS} -o $@ server.c parser.c ${LIB}

srv_en: server.c crypt.c crypt.h parser.c parser.h
	${CC} ${CC_FLAGS} -DENCRYPT -o $@ server.c crypt.c parser.c ${LIB}

tst_crypt: tst_crypt.c crypt.c  crypt.h
	${CC} ${CC_FLAGS} -o $@ tst_crypt.c crypt.c ${LIB}

tst_parser: tst_parser.c parser.c parser.h
	${CC} ${CC_FLAGS} -o $@ tst_parser.c parser.c ${LIB}

clean:
	${RM} srv srv_en tst_crypt tst_parser

