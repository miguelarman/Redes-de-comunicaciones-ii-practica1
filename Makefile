########################################################
# File: Makefile
#
# Authors: Mario García, Miguel Arconada
#
# Description: Makefile of the whole project. With options to compile and execute the programs
########################################################
CC = gcc
CFLAGS = -g -Wall
FILES = tester_daemon
########################################################
OBJECTSTESTER_DAEMON = srclib/daemon.o testers/tester_daemon.o
########################################################
OBJECTS = srclib/daemon.o testers/tester_daemon.o
########################################################

all: $(FILES) clear

tester_daemon: $(OBJECTSTESTER_DAEMON)
	$(CC) $(CFLAGS) -o tester_daemon $(OBJECTSTESTER_DAEMON)

tester_daemon.o: srclib/daemon.h testers/tester_daemon.c
	$(CC) $(CFLAGS) -c testers/tester_daemon.c




daemon.o: srclib/daemon.c srclib/daemon.h
	$(CC) $(CFLAGS) -c srclib/daemon.c

########################################################

clear:
	rm -rf *.o $(OBJECTS)

clean:
	rm -rf *.o $(FILES)
