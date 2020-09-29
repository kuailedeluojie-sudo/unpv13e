include ../Make.defines

PROGS =	tcpservselect01 tcpservselect02 

all:	${PROGS}


tcpservselect01:	tcpservselect01.o
		${CC} ${CFLAGS} -o $@ tcpservselect01.o ${LIBS}

tcpservselect02:	tcpservselect02.o
		${CC} ${CFLAGS} -o $@ tcpservselect02.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
