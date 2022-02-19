
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include "main.h"
int contenitori[NUMERO_CONTENITORI];
pthread_mutex_t mutexes[NUMERO_CONTENITORI]; // mutex dei contenitori
int main()
{
    contenitori[0]=QTA_MAX_CONTENITORE;
    inizializza_mutex();
    pthread_t consumatori[NUMERO_CONSUMATORI];

    for (int i = 0; i < NUMERO_CONSUMATORI; i++)
    {
        if (pthread_create(&consumatori[i], NULL, (void *)consumatore_thread, NULL) != 0)
        {
            handle_error("pthread_create")
        }
    }
    /* join dei thread. */
    for (long int i = 0; i < NUMERO_CONSUMATORI; i++)
    {
        if (pthread_join(consumatori[i], NULL) != 0)
        {
            handle_error("pthread_join");
        }
    }
    printf("%d", contenitori[0]);
    return 0;
}

int qta_alcol_per_cocktail = 5;
int qta_bevanda_per_cocktail = 5;

void consumatore_thread()
{
    for (int i = 0; i < 1; i++)
    {
        // contenitore alcol tra 0 e 1
        int alcol = 0; // rand() % 2;
        // contenitore bevanda tra 2 e 3
        int bevanda = 2 + (rand() % 2);

        if (pthread_mutex_lock(&mutexes[alcol]) != 0)
        {
            printf("error alcol");
        }

        if (contenitori[alcol] < qta_alcol_per_cocktail)
        {
            riempi_contenitore(alcol);
            printf("riempio alcol\n");
        }
        contenitori[alcol] -= qta_alcol_per_cocktail;
        pthread_mutex_unlock(&mutexes[alcol]);

        if (pthread_mutex_lock(&mutexes[bevanda]) != 0)
        {
            printf("error bevanda");
        }

        if (contenitori[bevanda] < qta_bevanda_per_cocktail)

            riempi_contenitore(bevanda);

        contenitori[bevanda] -= qta_bevanda_per_cocktail;
        pthread_mutex_unlock(&mutexes[bevanda]);
    }
}

void riempi_contenitore(int n_contenitore)
{
    contenitori[n_contenitore] = QTA_MAX_CONTENITORE;
}

void inizializza_mutex()
{
    for (int i = 0; i < NUMERO_CONTENITORI; i++)
    {
        if (pthread_mutex_init(&mutexes[i], NULL) == -1)
            handle_error("pthread_mutex_init");
    }
}