#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "confuse.h"
#include "../includes/configParser.h"

int main() {
  char abspath[MAX_STR];
  configOptions opts;

  if (parseConfig("server.conf", &opts) == ERROR) {
    return ERROR;
  }

  printf("server_root: %s\n", opts.server_root);
  printf("max_clients: %d\n", opts.max_clients);
  printf("listen_port: %d\n", opts.listen_port);
  printf("server_signature: %s\n", opts.server_signature);
  printf("thread_count: %d\n", opts.thread_count);
  printf("queue_size: %d\n", opts.queue_size);

  if (getcwd(abspath, MAX_STR) == NULL) {
    return ERROR;
  }
  strcat(abspath, opts.server_root);

  printf("%s\n", abspath);
}
