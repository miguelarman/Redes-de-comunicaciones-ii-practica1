#ifndef PROCESA_CONEXION_H
/**
 * @file procesa_conexion.h
 * @defgroup Procesa_conexion Procesa conexión
 *
 * Esta librería se encarga de procesar cada
 * conexión que se recibe en el servidor. De
 * analizar si se pide una conexión persistente, etc.
 */

#define PROCESA_CONEXION_H /*!< Macro de compilación */

/**
 * @brief Función que procesa una conexión
 *
 * Esta función analiza cada conexión entrante
 * al servidor, y para cada petición la procesa,
 * y cierra la conexión cuando se pide terminar
 * una conexión o si no es persistente
 *
 * @ingroup Procesa_conexion
 * @param connfd Descriptor del socket por el mandar la respuesta
 * @param resources_path Ruta de la carpeta resources/
 * @return Código de ejecución
 */
int procesa_conexion(int connfd, char *resources_path);

#endif
