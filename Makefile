CC=gcc
CFLAGS = -g
LIBS = -lpthread
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: peer

peer: peer.o ip-conv.o peer_parser.o stun_client.o bin_str_conv.o
	$(CC) -o peer peer.o ip-conv.o peer_parser.o stun_client.o bin_str_conv.o $(LIBS)

peer.o: peer.c ip-conv.c ip-conv.h peer_parser.c peer_parser.h stun_client.c stun_client.h bin_str_conv.c

clean:
	rm -f peer *.o
