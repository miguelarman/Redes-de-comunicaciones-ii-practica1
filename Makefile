########################################################
# File: Makefile
#
# Authors: Mario García, Miguel Arconada
#
# Description: Makefile of the whole project. With options to compile and execute the programs
########################################################
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
FILES = tester_daemon test_server #server
TESTERS = tester_daemon test_server
########################################################
OBJECTSTESTER_DAEMON = srclib/daemon.o testers/tester_daemon.o
OBJECTSTEST_SERVER = srclib/connection.o srclib/daemon.o srclib/procesa_peticion.o srclib/procesa_conexion.o srclib/picohttpparser.o testers/test_server.o
OBJECTSSERVER = srclib/blockingQueue.o srclib/connection.o srclib/daemon.o srclib/procesa_conexion.o srclib/procesa_peticion.o srclib/configParser.o srclib/picohttpparser.o src/server.o
########################################################
OBJECTS = srclib/daemon.o testers/tester_daemon.o srclib/blockingQueue.o srclib/connection.o srclib/picohttpparser.o srclib/procesa_peticion.o srclib/procesa_conexion.o #src/server.o
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

test_server.o: includes/connection.h includes/procesa_peticion.h includes/procesa_conexion.h includes/daemon.h testers/test_server.c
	$(CC) $(CFLAGS) -c testers/test_server.c

server: $(OBJECTSSERVER)
	$(CC) $(CFLAGS) -o server $(OBJECTSSERVER)

server.o: includes/connection.h includes/procesa_peticion.h includes/procesa_conexion.h includes/daemon.h includes/picohttpparser.h src/server.c
	$(CC) $(CFLAGS) -c src/server.c



prueba_script: prueba_script.o
	$(CC) -g -o prueba_script prueba_script.o

prueba_script.o: prueba_script.c
	$(CC) -g -c prueba_script.c

prueba: prueba.o
	$(CC) -g -o prueba prueba.o

prueba.o: prueba.c
	$(CC) -g -c prueba.c

daemon.o: srclib/daemon.c includes/daemon.h
	$(CC) $(CFLAGS) -c srclib/daemon.c

# confuse.o: srclib/confuse.c includes/confuse.h
# 	$(CC) $(CFLAGS) -c srclib/confuse.c

connection.o: srclib/connection.c includes/connection.h
	$(CC) $(CFLAGS) -c srclib/connection.c

procesa_peticion.o: srclib/procesa_peticion.c includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_peticion.c

procesa_conexion.o: srclib/procesa_conexion.c includes/procesa_conexion.h includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_conexion.c

picohttpparser.o: srclib/picohttpparser.c includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/picohttpparser.c

blockingQueue.o: srclib/blockingQueue.c includes/blockingQueue.h
	$(CC) $(CFLAGS) -c srclib/blockingQueue.c

########################################################
doxy-file:
	@echo "Al hacer este paso se sobreescribe el Doxyfile. Es necesario poner RECURSIVE = YES y GENERATE_XML = YES para usar moxygen"
	# doxygen -g

doxygen:
	doxygen Doxyfile

moxygen:
	moxygen --anchors --groups --output api/api-%s.md xml/
	#"Da problemas con los enlaces. Usar la regex \{#(.*?)\} ----> <a name="dolar1"></a>"
########################################################

clear:
	rm -rf *.o $(OBJECTS)

clean:
	rm -rf *.o $(FILES)
