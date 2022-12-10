#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>

char Linija[100];

sem_t s1, s2, sm;

void *prvaFunkcija(void *fajl)
{
    FILE *fp = fopen("Parni.txt", "w");

    while (1)
    {
        sem_wait(&s1);
        if (strcmp(Linija, "") == 0)
        {
            sem_post(&sm);
            break;
        }
        fprintf(fp, "%s\n", Linija);
        sem_post(&sm);
    }

    fclose(fp);
    // sleep(1);
}
void *drugaFunkcija(void *fajl)
{
    FILE *fn = fopen("Neparni.txt", "w");

    while (1)
    {
        sem_wait(&s2);
        if (strcmp(Linija, "") == 0)
        {
            sem_post(&sm);
            break;
        }
        fprintf(fn, "%s", Linija);
        sem_post(&sm);
    }

    fclose(fn);
}
int main(int argc, char *argv[])
{
    pthread_t n1, n2;
    FILE *f;

    char *Datoteka;

    // Datoteka = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char));
    Datoteka = (char *)malloc((strlen("zadatak.txt") + 1) * sizeof(char));

    f = fopen("zadatak.txt", "r");

    sem_init(&s1, 0, 0);
    sem_init(&s2, 0, 0);
    sem_init(&sm, 0, 1);

    pthread_create(&n1, NULL, prvaFunkcija, NULL);
    pthread_create(&n2, NULL, drugaFunkcija, NULL);

    int i = 0;
    while (!feof(f))
    {
        sem_wait(&sm);
        fgets(Linija, 100, f);
        if (i % 2 == 0)
            sem_post(&s1);
        else
            sem_post(&s2);
        i++;
    }
    strcpy(Linija,"");
    sem_post(&s1);
    sem_post(&s2);
    pthread_join(n1, NULL);
    pthread_join(n2, NULL);

    sem_destroy(&sm);
    sem_destroy(&s1);
    sem_destroy(&s2);

    fclose(f);

    free(Datoteka);

    return 0;
}
