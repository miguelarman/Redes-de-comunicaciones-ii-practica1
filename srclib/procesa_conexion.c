#define _POSIX_C_SOURCE 199309L

/**
 * @brief Librería que procesa cada conexión
 *
 * En este fichero, se definen las funciones necesarias
 * para, dada una conexión, satifacer todas las peticiones
 *
 * @file procesa_conexiones.c
 * @author Miguel Arconada Manteca y Mario García Pascual
 * @date 9-5-2019
 */

#include "../includes/procesa_peticion.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

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
    /* TODO */
    if (retorno == CLOSE_CONNECTION_REQUEST) {
      return OK;
    }
  }

  retorno = procesa_peticion(connfd, resources_path, campos_parseados);
  if (retorno != OK) {
    /* TODO */
  }

  /* Si el cliente solicita conexion persistente en HTML 1.1 sigue ejecutando */
  if (_cliente_quiere_conexion_persistente(&campos_parseados) == TRUE) {

    /* TODO */
    while (_cliente_quiere_cerrar_la_conexion(&campos_parseados) == FALSE) {
      /* El cliente no quiere cerrar la conexión, así que se vuelve a recibir otra petición */
      retorno = parsear_peticion(connfd, &campos_parseados);
      if (retorno != OK) {
        /* TODO */
      }

      if (retorno == CLOSE_CONNECTION_REQUEST) {
        break;
      }

      retorno = procesa_peticion(connfd, resources_path, campos_parseados);
      if (retorno != OK) {
        /* TODO */
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

  /* Comprueba si es una petición con HTTP 1.1,
   * porque si no no puede hacer conexión persistente
   */
  if ((int)campos_parseados->minor_version == 0) {
    return FALSE;
  }

  /* TODO Comprueba si el cliente ha especificado en los
   * headers que quiere mantener activa la conexión
   */

  for (i = 0; i < (int)campos_parseados->num_headers; i++) {

    header_name = (char *)malloc(((int)campos_parseados->headers[i].name_len + 1) * sizeof(char));
    if (header_name == NULL) {
      /* TODO */
    }
    header_value = (char *)malloc(((int)campos_parseados->headers[i].value_len + 1) * sizeof(char));
    if (header_value == NULL) {
      /* TODO */
    }

    /* Guarda los valores de header_name y header_value */
    sprintf(header_name, "%.*s", (int)campos_parseados->headers[i].name_len, campos_parseados->headers[i].name);
    if (header_name == NULL) {
      /* TODO */
    }
    sprintf(header_value, "%.*s", (int)campos_parseados->headers[i].value_len, campos_parseados->headers[i].value);
    if (header_value == NULL) {
      /* TODO */
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

  /* return TRUE; */

  char *metodo = NULL;

  if (campos_parseados->rret == 0) {
    return TRUE;
  }

  metodo = (char *)malloc(((int)campos_parseados->method_len + 1) * sizeof(char));
  if (metodo == NULL) {
    /* TODO */
  }

  sprintf(metodo, "%.*s", (int)campos_parseados->method_len, campos_parseados->method);
  if (metodo == NULL) {
    /* TODO */
  }

  if (strcmp(metodo, "CLOSE") == 0) {
    free(metodo);
    return TRUE;
  } else {
    free(metodo);
    return FALSE;
  }
}
