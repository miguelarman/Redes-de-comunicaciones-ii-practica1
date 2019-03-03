#include "../includes/daemon.h"
#include <stdio.h>

/* gcc -c daemon.c
e361902@12-6-66-175:~/practica1/srclib$ ls
daemon.c  daemon.h  daemon.o
e361902@12-6-66-175:~/practica1/srclib$ cd ../testers/
e361902@12-6-66-175:~/practica1/testers$ gcc -c tester_daemon.c
e361902@12-6-66-175:~/practica1/testers$ ls
daemon.o  tester_daemon.c  tester_daemon.o
e361902@12-6-66-175:~/practica1/testers$ gcc -o tester_daemon ../srclib/daemon.o tester_daemon.o
e361902@12-6-66-175:~/practica1/testers$ ls
daemon.o  tester_daemon  tester_daemon.c  tester_daemon.o
e361902@12-6-66-175:~/practica1/testers$ ./tester_daemon */

int main () {
  int r;

  printf("Testing demonizar(). Check syslog to see correct functioning\n");

  r = demonizar();

  printf("Returned: %d", r);
  if (r == 0) {
    printf("\tCorrect execution (check syslog)\n");
  } else {
    printf("\tIncorrect execution (check syslog)\n");
  }

  return 0;
}
