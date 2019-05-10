#ifndef CONNECTION_H
/**
 * @file connection.h
 * @defgroup Connection
 *
 * Esta librería se encarga de adiminstrar una conexión
 */


#define CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>

#define ERROR_BIND -2

/**
 * @brief Función que prepara un socket TCP
 *
 * Esta función crea un sockete TCP, hace bind en la ip
 * y puerto dadas como argumentos y hace listen
 *
 * @ingroup Connection
 * @param ip Dirección ip donde se quiere el socket
 * @param port Puerto donde se desea el socket
 * @param backlog Tamaño máximo de la cola de listen
 * @return Desciptor del socket creado
 */
int tcp_listen(char *ip, int port, int backlog);

/**
 * @brief Función que acepta una conexión
 *
 * Esta función, dado un socket TCP en el que se ha hecho listen, hace accept y devuelve una conexión
 *
 * @ingroup Connection
 * @param sockfd Descriptor del socket
 * @param addr Dirección de la conexión aceptada
 * @param addrlen Tamaño de la dirección de la conexión aceptada
 * @return Retorno del accept
 */
int accept_connection(int sockfd, struct sockaddr* addr, socklen_t *addrlen);

/**
 * @brief Función que cierra una conexión
 *
 * Esta función cierra un socket TCP
 *
 * @ingroup Connection
 * @param fd Descriptor del socket
 * @return Retorno del close
 */
int close_connection(int fd);

#endif
