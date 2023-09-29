#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
int turno_actual = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *comer_hamburguesa(void *tid) {
    long mi_turno = (long)tid;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (cantidad_restante_hamburguesas <= 0) {
            printf("Se terminaron las hamburguesas :( Hilo %ld se retira.\n", mi_turno);
            pthread_mutex_unlock(&mutex);
            break; // Se terminaron las hamburguesas, salir del bucle
        }

        if (mi_turno == turno_actual) {
            printf("Hola! soy el hilo(comensal) %ld, me voy a comer una hamburguesa! Todavía quedan %d\n", mi_turno, cantidad_restante_hamburguesas);
            cantidad_restante_hamburguesas--; // Me como una hamburguesa
            turno_actual = (turno_actual + 1) % NUMBER_OF_THREADS; // Cambiar el turno
        }

        pthread_cond_broadcast(&cond); // Despertar a todos los hilos
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola!, soy el hilo principal. Estoy creando el hilo %d\n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)(long)i);
        if (status != 0) {
            printf("Algo salió mal al crear el hilo. Recibí el código de error %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(threads[i], NULL); // Esperar a que todos los hilos terminen
    }

    printf("Todos los hilos han terminado. Se terminaron las hamburguesas :(\n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}
