#include <stdio.h>

#include "confuse.h"
#include "../includes/configParser.h"

int parseConfig(char *conffile, configOptions *out_confopt) {
  if (conffile == NULL || out_confopt == NULL) {
    return ERROR;
  }

  cfg_opt_t opts[] =
  {
    CFG_STR("server_root", DEFAULT_SERVER_ROOT, CFGF_NONE),
    CFG_INT("max_clients", DEFAULT_MAX_CLIENTS, CFGF_NONE),
    CFG_INT("listen_port", DEFAULT_LISTEN_PORT, CFGF_NONE),
    CFG_STR("server_signature", DEFAULT_SERVER_SIGNATURE, CFGF_NONE),
    CFG_INT("thread_count", DEFAULT_THREAD_COUNT, CFGF_NONE),
    CFG_INT("queue_size", DEFAULT_QUEUE_SIZE, CFGF_NONE),
    CFG_END()
  };
  cfg_t *cfg;

  cfg = cfg_init(opts, CFGF_NONE);
  if (cfg_parse(cfg, conffile) == CFG_PARSE_ERROR) {
    return ERROR;
  }

  out_confopt->server_root = cfg_getstr(cfg, "server_root");
  out_confopt->max_clients = cfg_getint(cfg, "max_clients");
  out_confopt->listen_port = cfg_getint(cfg, "listen_port");
  out_confopt->server_signature = cfg_getstr(cfg, "server_signature");
  out_confopt->thread_count = cfg_getint(cfg, "thread_count");
  out_confopt->queue_size = cfg_getint(cfg, "queue_size");

  cfg_free(cfg);
  return SUCCESS;
}

int main() {
  char *cf = "server.conf";
  configOptions opt;

  if (parseConfig(cf, &opt) == ERROR) {
    return ERROR;
  }

  printf("server_root: %s\n", opt.server_root);
  printf("max_clients: %d\n", opt.max_clients);
  printf("listen_port: %d\n", opt.listen_port);
  printf("server_signature: %s\n", opt.server_signature);
  printf("thread_count: %d\n", opt.thread_count);
  printf("queue_size: %d\n", opt.queue_size);
}
