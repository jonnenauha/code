#
#	Makefile
#

CLINAME 	= sock_cli
SRVNAME 	= sock_srv

CC		= CC

NETOBJS 	= network.o
NETSRC		= network.cc 
NETHEAD 	= network.hh

UTLOBJS 	= utility.o
UTLSRC		= utility.cc
UTLHEAD 	= utility.hh

CLIOBJS 	= client.o $(UTLOBJS) $(NETOBJS)
CLISRC		= client.cc

SRVOBJS 	= server.o $(UTLOBJS) $(NETOBJS)
SRVSRC		= server.cc

CFLAGS		= 
LFLAGS		= -lsocket -lnsl -lm


all:	server client

client: $(CLIOBJS)
	$(CC)	$(LFLAGS) $(CLIOBJS) -o $(CLINAME)

server: $(SRVOBJS)
	$(CC)	$(LFLAGS) $(SRVOBJS) -o $(SRVNAME)

client.o: $(CLISRC)
	$(CC)	$(CFLAGS) -c $(CLISRC)

server.o: $(SRVSRC)
	$(CC)	$(CFLAGS) -c $(SRVSRC)

network.o: $(NETSRC) $(NETHEAD)
	$(CC)	$(CFLAGS) -c $(NETSRC)

utility.o: $(UTLSRC) $(UTLHEAD)
	$(CC)	$(CFLAGS) -c $(UTLSRC)

clean:	
	rm -f core ./*.o

clear:
	clear

clobber: clean clear
	rm -f $(CLINAME) $(SRVNAME) ./*.bck
