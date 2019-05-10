#ifndef DAEMON_H
/**
 * @file daemon.h
 * @defgroup Daemon Daemonizar procesos
 *
 * Esta librería se encarga de demonizar
 */

#define DAEMON_H /*!< Macro de compilación */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ES_PADRE 5 /*!< Retorno a la función llamante cuando el proceso es el padre */

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
