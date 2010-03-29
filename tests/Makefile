CC = gcc
LD = ld
INCLUDE_FLAGS = -I.

GSOAP_CFLAGS = $(shell pkg-config --cflags gsoap)
GSOAP_LIBS = $(shell pkg-config --libs gsoap)
GSOAP_CLIENT_OBJS = ws/soapClient.o ws/soapC.o

CFLAGS = $(INCLUDE_FLAGS) $(GSOAP_CFLAGS) -D_REENTRANT -O2 -g

LDFLAGS = 
LIBS = $(GSOAP_LIBS)


all: $(GSOAP_CLIENT_OBJS) soap_initialization.o post_message_queue.o
	$(CC) $(CFLAGS) -o soap_initialization soap_initialization.o $(GSOAP_CLIENT_OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o post_message_queue post_message_queue.o $(GSOAP_CLIENT_OBJS) $(LIBS)

%.c.%.o:
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

clean:
	-rm -f *.o
	-rm -f soap_initialization
	-rm -f post_message_queue
