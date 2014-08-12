CC=gcc
CFLAGS = -g
LDLIBS = -lpthread

EXEC= peer

SOURCE= peer.c ip-conv.c peer_parser.c stun_client.c bin_str_conv.c file_packet_parser.c

OBJS= ${SOURCE:.c=.o}

.PREFIXES= .c .o

peer: ${SOURCE}
	${CC} -g -o $@ ${CFLAGS} ${SOURCE} ${LDLIBS}

all: peer

clean:
	rm -f peer *.o
