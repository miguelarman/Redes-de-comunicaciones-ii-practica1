#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#define DEFAULT_SERVER_ROOT "\\"
#define DEFAULT_MAX_CLIENTS 20
#define DEFAULT_LISTEN_PORT 8080
#define DEFAULT_SERVER_SIGNATURE "Redes2"
#define DEFAULT_THREAD_COUNT 20
#define DEFAULT_QUEUE_SIZE 20

#define ERROR -1
#define SUCCESS 0

typedef struct
{
  char *server_root;
  int max_clients;
  int listen_port;
  char *server_signature;
  int thread_count;
  int queue_size;
} configOptions;

int parseConfig(char *cf, configOptions *out_confopt);

#endif
