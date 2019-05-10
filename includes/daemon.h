#ifndef DAEMON_H
/**
 * @file daemon.h
 * @defgroup Daemon
 *
 * Esta librería se encarga de demonizar
 */

#define DAEMON_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ES_PADRE 5

/**
 * @brief Función que demoniza
 *
 * Esta función crea un proceso demonio y
 * notifica al padre para que termine su ejecución
 *
 * @ingroup Daemon
 * @return Código de si es el proceso padre o el demonio
 */
int demonizar(void);


#endif
