########################################################
# File: Makefile
#
# Authors: Mario García, Miguel Arconada
#
# Description: Makefile of the whole project. With options to compile and execute the programs
########################################################
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
FILES = tester_daemon test_server
TESTERS = tester_daemon test_server
########################################################
OBJECTSTESTER_DAEMON = srclib/daemon.o testers/tester_daemon.o
OBJECTSTEST_SERVER = srclib/connection.o srclib/procesa_peticion.o srclib/procesa_conexion.o srclib/picohttpparser.o testers/test_server.o
########################################################
OBJECTS = srclib/daemon.o testers/tester_daemon.o srclib/connection.o srclib/picohttpparser.o testers/test_server.o srclib/procesa_peticion.o srclib/procesa_conexion.o
########################################################

all: $(FILES) clear

testers: $(TESTERS) clear

test_server_exe: test_server
	valgrind --leak-check=full ./test_server

tester_daemon: $(OBJECTSTESTER_DAEMON)
	$(CC) $(CFLAGS) -o tester_daemon $(OBJECTSTESTER_DAEMON)

tester_daemon.o: includes/daemon.h testers/tester_daemon.c
	$(CC) $(CFLAGS) -c testers/tester_daemon.c

test_server: $(OBJECTSTEST_SERVER)
	$(CC) $(CFLAGS) -o test_server $(OBJECTSTEST_SERVER)

test_server.o: includes/connection.h includes/procesa_peticion.h includes/procesa_conexion.h testers/test_server.c
	$(CC) $(CFLAGS) -c testers/test_server.c



daemon.o: srclib/daemon.c includes/daemon.h
	$(CC) $(CFLAGS) -c srclib/daemon.c

connection.o: srclib/connection.c includes/connection.h
	$(CC) $(CFLAGS) -c srclib/connection.c

procesa_peticion.o: srclib/procesa_peticion.c includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_peticion.c

procesa_conexion.o: srclib/procesa_conexion.c includes/procesa_conexion.h includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_conexion.c

picohttpparser.o: srclib/picohttpparser.c includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/picohttpparser.c

########################################################

clear:
	rm -rf *.o $(OBJECTS)

clean:
	rm -rf *.o $(FILES)
