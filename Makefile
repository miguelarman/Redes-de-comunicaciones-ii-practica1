# #
# # 'make depend' uses makedepend to automatically generate dependencies
# #               (dependencies are added to end of Makefile)
# # 'make'        build executable file 'mycc'
# # 'make clean'  removes all .o and executable files
# #
#
# # define the C compiler to use
# CC = gcc
#
# # define any compile-time flags
# CFLAGS = -Wall -g
#
# # define any directories containing header files other than /usr/include
# #
# INCLUDES = -I$(CURDIR)/includes
#
# # define library paths in addition to /usr/lib
# #   if I wanted to include libraries not in /usr/lib I'd specify
# #   their path using -Lpath, something like:
# LFLAGS = -L$(CURDIR)/srclib
#
# # define any libraries to link into executable:
# #   if I want to link in libraries (libx.so or libx.a) I use the -llibname
# #   option, something like (this will link in libmylib.so and libm.so:
# LIBS = -lmylib -lm
#
# # define the C source files
# SRCS = emitter.c error.c init.c lexer.c main.c symbol.c parser.c
#
# # define the C object files
# #
# # This uses Suffix Replacement within a macro:
# #   $(name:string1=string2)
# #         For each word in 'name' replace 'string1' with 'string2'
# # Below we are replacing the suffix .c of all words in the macro SRCS
# # with the .o suffix
# #
# OBJS = $(SRCS:.c=.o)
#
# # define the executable file
# MAIN = mycc
#
# #
# # The following part of the makefile is generic; it can be used to
# # build any executable just by changing the definitions above and by
# # deleting dependencies appended to the file from 'make depend'
# #
#
# .PHONY: depend clean
#
# all:    $(MAIN)
#         @echo  Simple compiler named mycc has been compiled
#
# $(MAIN): $(OBJS)
#         $(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
#
# # this is a suffix replacement rule for building .o's from .c's
# # it uses automatic variables $<: the name of the prerequisite of
# # the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# # (see the gnu make manual section about automatic variables)
# .c.o:
#         $(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@
#
# clean:
#         $(RM) *.o *~ $(MAIN)
#
# depend: $(SRCS)
#         makedepend $(INCLUDES) $^
#
# DO NOT DELETE THIS LINE -- make depend needs it

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
########################################################
OBJECTSTESTER_DAEMON = srclib/daemon.o testers/tester_daemon.o
OBJECTSTEST_SERVER = srclib/connection.o srclib/procesa_peticion.o srclib/picohttpparser.o testers/test_server.o
########################################################
OBJECTS = srclib/daemon.o testers/tester_daemon.o srclib/connection.o srclib/picohttpparser.o testers/test_server.o srclib/procesa_peticion.o
########################################################

all: $(FILES) clear

tester_daemon: $(OBJECTSTESTER_DAEMON)
	$(CC) $(CFLAGS) -o tester_daemon $(OBJECTSTESTER_DAEMON)

tester_daemon.o: includes/daemon.h testers/tester_daemon.c
	$(CC) $(CFLAGS) -c testers/tester_daemon.c

test_server: $(OBJECTSTEST_SERVER)
	$(CC) $(CFLAGS) -o test_server $(OBJECTSTEST_SERVER)

test_server.o: includes/connection.h testers/test_server.c
	$(CC) $(CFLAGS) -c testers/test_server.c



daemon.o: srclib/daemon.c includes/daemon.h
	$(CC) $(CFLAGS) -c srclib/daemon.c

connection.o: srclib/connection.c includes/connection.h
	$(CC) $(CFLAGS) -c srclib/connection.c

procesa_peticion.o: srclib/procesa_peticion.c includes/procesa_peticion.h includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/procesa_peticion.c

picohttpparser.o: srclib/picohttpparser.c includes/picohttpparser.h
	$(CC) $(CFLAGS) -c srclib/picohttpparser.c

########################################################

clear:
	rm -rf *.o $(OBJECTS)

clean:
	rm -rf *.o $(FILES)
