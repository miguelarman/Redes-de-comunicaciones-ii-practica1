/**
 * @brief Descripción del fichero/librería
 *
 * Descripción más elaborada
 *
 * @file configParser.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../includes/confuse.h"
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

  strcpy(out_confopt->server_root, cfg_getstr(cfg, "server_root"));
  out_confopt->max_clients = cfg_getint(cfg, "max_clients");
  out_confopt->listen_port = cfg_getint(cfg, "listen_port");
  strcpy(out_confopt->server_signature, cfg_getstr(cfg, "server_signature"));
  out_confopt->thread_count = cfg_getint(cfg, "thread_count");
  out_confopt->queue_size = cfg_getint(cfg, "queue_size");

  cfg_free(cfg);
  return SUCCESS;
}
