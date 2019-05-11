#define _POSIX_C_SOURCE 199309L /*!< Macro de compilación */

/**
 * @brief Librería que procesa cada conexión
 *
 * En este fichero, se definen las funciones necesarias
 * para, dada una conexión, satifacer todas las peticiones
 *
 * @file procesa_conexion.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */

#include "../includes/procesa_conexion.h"
#include "../includes/procesa_peticion.h"

#include <syslog.h>



/**
 * @brief Función que analiza si el cliente quiere conexión persistente
 *
 * Esta función analiza una petición HTTP y,
 * dependiendo de la versión del protocolo y
 * del campo Connection de la cabecera, analiza
 * si el cliente quiere mantener abierta la conexión
 *
 * @ingroup Procesa_conexion
 * @param campos_parseados Estructura con los campos parseados de la petición
 * @return resultado booleano
 */
int _cliente_quiere_conexion_persistente(Parsear *campos_parseados);

/**
 * @brief Función que analiza si el cliente quiere cerrar la conexión
 *
 * Esta función analiza una petición HTTP y,
 * dependiendo de la versión del protocolo y
 * del campo Connection de la cabecera, analiza
 * si el cliente quiere mantener abierta la conexión
 *
 * @ingroup Procesa_conexion
 * @param campos_parseados Estructura con los campos parseados de la petición
 * @return resultado booleano
 */
int _cliente_quiere_cerrar_la_conexion(Parsear *campos_parseados);

/********************************************/
/********************************************/
/********************************************/
/*       IMPLEMENTACIÓN DE FUNCIONES        */
/********************************************/
/********************************************/
/********************************************/

int procesa_conexion(int connfd, char *resources_path) {

  Parsear campos_parseados;
  int retorno;

  retorno = parsear_peticion(connfd, &campos_parseados);
  if (retorno != OK) {
    if (retorno == CLOSE_CONNECTION_REQUEST) {
      return OK;
    } else {
      syslog(LOG_ERR, "Error al llamar a parsear petición");
      return ERROR_PARSEAR_PETICION;
    }
  }

  retorno = procesa_peticion(connfd, resources_path, campos_parseados);
  if (retorno != OK) {
      syslog(LOG_ERR, "Error al llamar a procesa_peticion");
      return ERROR_PROCESA_PETICION;
  }

  /* Si el cliente solicita conexion persistente en HTML 1.1 sigue ejecutando */
  if (_cliente_quiere_conexion_persistente(&campos_parseados) == TRUE) {

    /* Mientras el cliente mantenga la conexión abierta hacemos un bucle */
    while (_cliente_quiere_cerrar_la_conexion(&campos_parseados) == FALSE) {
      /* El cliente no quiere cerrar la conexión, así que se vuelve a recibir otra petición */
      retorno = parsear_peticion(connfd, &campos_parseados);
      if (retorno != OK) {
            syslog(LOG_ERR, "Error al llamar a parsear_peticion (%d)", retorno);
            return ERROR_PARSEAR_PETICION;
      }

      if (retorno == CLOSE_CONNECTION_REQUEST) {
        break;
      }

      retorno = procesa_peticion(connfd, resources_path, campos_parseados);
      if (retorno != OK) {
        syslog(LOG_ERR, "Error al llamar a procesa_peticion (%d)", retorno);
        return ERROR_PROCESA_PETICION;
      }
    }
    return OK;
  } else {
    return OK;
  }
}


int _cliente_quiere_conexion_persistente(Parsear *campos_parseados) {

  int   i;
  int   flag;
  char *header_name  = NULL;
  char *header_value = NULL;

  /*
   * Comprueba si es una petición con HTTP 1.1,
   * porque si no no puede hacer conexión persistente
   */
  if ((int)campos_parseados->minor_version == 0) {
    return FALSE;
  }

  /* Comprueba si el cliente ha especificado en los
   * headers que quiere mantener activa la conexión
   */

  for (i = 0; i < (int)campos_parseados->num_headers; i++) {

    header_name = (char *)malloc(((int)campos_parseados->headers[i].name_len + 1) * sizeof(char));
    if (header_name == NULL) {
      syslog(LOG_ERR, "Error en malloc() en _cliente_quiere_conexion_persistente");
      return ERROR;
    }
    header_value = (char *)malloc(((int)campos_parseados->headers[i].value_len + 1) * sizeof(char));
    if (header_value == NULL) {
      syslog(LOG_ERR, "Error en malloc() en _cliente_quiere_conexion_persistente");
      if (header_name) free(header_name);
      return ERROR;
    }

    /* Guarda los valores de header_name y header_value */
    sprintf(header_name, "%.*s", (int)campos_parseados->headers[i].name_len, campos_parseados->headers[i].name);
    if (header_name == NULL) {
      syslog(LOG_ERR, "Error en sprintf() en _cliente_quiere_conexion_persistente");
      if (header_name) free(header_name);
      if (header_value) free(header_value);
      return ERROR;
    }
    sprintf(header_value, "%.*s", (int)campos_parseados->headers[i].value_len, campos_parseados->headers[i].value);
    if (header_value == NULL) {
      syslog(LOG_ERR, "Error en sprintf() en _cliente_quiere_conexion_persistente");
      if (header_name) free(header_name);
      if (header_value) free(header_value);
      return ERROR;
    }

    if (strcmp(header_name, "Connection") == 0) {
      if (strcmp(header_value, "keep-alive") == 0) {
        flag = TRUE;
      } else {
        flag = FALSE;
      }
    } else {
      flag = FALSE;
    }


    free(header_name);
    free(header_value);

    if (flag == TRUE) {
      return TRUE;
    }
  }

  return FALSE;
}


int _cliente_quiere_cerrar_la_conexion(Parsear *campos_parseados) {

  char *metodo = NULL;

  if (campos_parseados->rret == 0) {
    return TRUE;
  }

  metodo = (char *)malloc(((int)campos_parseados->method_len + 1) * sizeof(char));
  if (metodo == NULL) {
    syslog(LOG_ERR, "Error en malloc() en _cliente_quiere_cerrar_la_conexion");
    return ERROR;
  }

  sprintf(metodo, "%.*s", (int)campos_parseados->method_len, campos_parseados->method);
  if (metodo == NULL) {
    syslog(LOG_ERR, "Error en malloc() en _cliente_quiere_cerrar_la_conexion");
    if (metodo) free(metodo);
    return ERROR;
  }

  if (strcmp(metodo, "CLOSE") == 0) {
    free(metodo);
    return TRUE;
  } else {
    free(metodo);
    return FALSE;
  }
}
