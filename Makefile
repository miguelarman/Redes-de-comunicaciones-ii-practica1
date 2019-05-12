########################################################
# File: Makefile
#
# Authors: Mario García, Miguel Arconada
#
# Description: Makefile of the whole project. With options to compile and execute the programs
########################################################
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic -D_GNU_SOURCE
LOCALLIBDIR = /usr/local/lib
LDFLAGS = -L$(LOCALLIBDIR)
LDLIBS  = -lconfuse
FILES = tester_daemon test_server server
TESTERS = tester_daemon test_server
########################################################
OBJECTSTESTER_DAEMON = srclib/daemon.o testers/tester_daemon.o
OBJECTSTEST_SERVER = srclib/connection.o srclib/daemon.o srclib/procesa_peticion.o srclib/procesa_conexion.o srclib/picohttpparser.o testers/test_server.o
OBJECTSSERVER = srclib/blockingQueue.o srclib/configParser.o srclib/connection.o srclib/daemon.o srclib/procesa_conexion.o srclib/procesa_peticion.o srclib/picohttpparser.o src/server.o
########################################################
OBJECTS = srclib/daemon.o testers/tester_daemon.o srclib/configParser.o srclib/blockingQueue.o srclib/connection.o srclib/picohttpparser.o srclib/procesa_peticion.o srclib/procesa_conexion.o src/server.o
########################################################

all: $(FILES) clear

testers: $(TESTERS) clear



tester_daemon: $(OBJECTSTESTER_DAEMON)
	$(CC) $(CFLAGS) -o tester_daemon $(OBJECTSTESTER_DAEMON) $(LDFLAGS) $(LDLIBS)

tester_daemon.o: includes/daemon.h testers/tester_daemon.c
	$(CC) $(CFLAGS) -c testers/tester_daemon.c $(LDFLAGS) $(LDLIBS)

test_server: $(OBJECTSTEST_SERVER)
	$(CC) $(CFLAGS) -o test_server $(OBJECTSTEST_SERVER) $(LDFLAGS) $(LDLIBS)

test_server.o: includes/connection.h includes/procesa_peticion.h includes/procesa_conexion.h includes/daemon.h testers/test_server.c
	$(CC) $(CFLAGS) -c testers/test_server.c $(LDFLAGS) $(LDLIBS)

server: $(OBJECTSSERVER)
	$(CC) $(CFLAGS) -pthread -o server $(OBJECTSSERVER) $(LDFLAGS) $(LDLIBS)

server.o: includes/connection.h includes/procesa_peticion.h includes/procesa_conexion.h includes/daemon.h includes/picohttpparser.h src/server.c
	$(CC) $(CFLAGS) -pthread -c src/server.c $(LDFLAGS) $(LDLIBS)




daemon.o: srclib/daemon.c includes/daemon.h
	$(CC) $(CFLAGS) -c srclib/daemon.c $(LDFLAGS) $(LDLIBS)

connection.o: srclib/connection.c includes/connection.h
	$(CC) $(CFLAGS) -c srclib/connection.c $(LDFLAGS) $(LDLIBS)

procesa_peticion.o: srclib/procesa_peticion.c includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_peticion.c $(LDFLAGS) $(LDLIBS)

procesa_conexion.o: srclib/procesa_conexion.c includes/procesa_conexion.h includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_conexion.c $(LDFLAGS) $(LDLIBS)

picohttpparser.o: srclib/picohttpparser.c includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/picohttpparser.c $(LDFLAGS) $(LDLIBS)

blockingQueue.o: srclib/blockingQueue.c includes/blockingQueue.h
	$(CC) $(CFLAGS) -pthread -c srclib/blockingQueue.c $(LDFLAGS) $(LDLIBS)

configParser.o: srclib/configParser.c includes/configParser.h
	$(CC) $(CFLAGS) -pthread -c srclib/configParser.c $(LDFLAGS) $(LDLIBS)

########################################################
doxy-file:
	@echo "Al hacer este paso se sobreescribe el Doxyfile. Es necesario poner RECURSIVE = YES y GENERATE_XML = YES para usar moxygen"
	doxygen -g

doxygen:
	doxygen Doxyfile_personalizado

moxygen:
	moxygen --anchors --groups --output api/api-%s.md xml/
	#"Da problemas con los enlaces. Usar la regex \{#(.*?)\} ----> <a name="dolar1"></a>"
########################################################

clear:
	rm -rf *.o $(OBJECTS)

clean:
	rm -rf *.o $(FILES)
