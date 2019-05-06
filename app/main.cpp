/* Control Numero 2
   Alejandro Yañez*/

using namespace std;
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    int namlen = 0; /* Largo del nombre de la maquina */
    char mensaje[100]; /* mensaje  */
    char nombre_maquina[50]; /*Nombre de la máquina */ 
    time_t t = time(NULL); /* Fecha y Hora */
    struct tm tm = *localtime(&t); /* Struct para la fecha */

    MPI_Status estado; /* devuelve estado al recibir*/

    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    MPI_Get_processor_name(nombre_maquina, &namlen); /* Obtener el nombre de la máquina */

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    /* Averiguamos el número de procesos que estan 
     * ejecutando nuestro porgrama 
     */

    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (mi_rango != 0) {
        /* Crea mensaje */
        sprintf(mensaje,"%s %d-%d-%d %d:%d:%d", nombre_maquina, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        dest = 0;
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
    } else { /* mi_rango == 0 */
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            printf("%s\n", mensaje);
        }
    }
    /* Termina con MPI. Recordemos que después de 
     * esta llamada no podemos llamar a funciones 
     * MPI, ni siquiera de nuevo a MPI_Init 
     */
    MPI_Finalize();
    return EXIT_SUCCESS;
}
