CC = gcc
LD = ld
INCLUDE_FLAGS = -I.

GSOAP_CFLAGS = $(shell pkg-config --cflags gsoap)
GSOAP_LIBS = $(shell pkg-config --libs gsoap)
GSOAP_CLIENT_OBJS = ws/soapClient.o ws/soapC.o

CFLAGS = $(INCLUDE_FLAGS) $(GSOAP_CFLAGS) -D_REENTRANT -O2 -g

# SLAPI
SLAPI_LIBS = -lslapi 

LDFLAGS = 
LIBS = $(GSOAP_LIBS) -shared $(SLAPI_LIBS)

OBJS = slapi-plugin-publisher.o
TARGET = /usr/lib/ldap
WSDL = http://localhost:8191/JMSService?wsdl


all: plugin

ws:
	-mkdir ws
	wsdl2h -c -o ws/jms2webservice.h $(WSDL)
	soapcpp2 -C -c -x -w -d ws ws/jms2webservice.h

plugin: ws $(GSOAP_CLIENT_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o slapi-plugin-publisher.so $(OBJS) $(GSOAP_CLIENT_OBJS) $(LIBS)

	

install: plugin
	cp slapi-plugin-publisher.so $(TARGET)/plugin-publisher.so

%.c.%.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *.o
	-rm -f ws/*.o
	-rm -f *.so
