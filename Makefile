CC = gcc
CC_OPTIONS = -Wall -O2 -c
RM = /bin/rm -f

SRCS := clock.c fcyc.c init.c mem-chase.c
OBJS := $(SRCS:%.c=%.o)

all: mem-chase

%.o: %.c
	${CC} ${CC_OPTIONS} $<

mem-chase: ${OBJS}
	gcc -o $@ ${OBJS}

clean:
	${RM} *.o mem-chase