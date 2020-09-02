#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

sem_t AJobDone, BJobDone;
char *quote_A, *quote_B;


void* modifyA_printB(void *para)
{
	int i = 0;
	while (quote_A[i] != '\0') {
		usleep(rand() & 15);
		--quote_A[i++];
	}

	sem_post(&AJobDone);
	sem_wait(&BJobDone);

	printf("quote_B:\t%s\n", quote_B);
}

void* modifyB_printA(void *para)
{
	int i = 0;
	while (quote_B[i] != '\0') {
		usleep(rand() & 100);
		--quote_B[i++];
	}

	sem_post(&BJobDone);
	sem_wait(&AJobDone);

	printf("quote_A:\t%s\n", quote_A);
}

int main()
{
	sem_init(&AJobDone, 0, 0);
	sem_init(&BJobDone, 0, 0);

	quote_A = strdup(
		"J!epo(u!lopx!ipx!nboz!pg!zpv!ibwf!fwfs!nfu!Ejkltusb-!cvu!"
		"zpv!qspcbcmz!lopx!uibu!bssphbodf!jo!dpnqvufs!tdjfodf!jt!"
		"nfbtvsfe!jo!obop.Ejkltusbt/!.!Bmbo!Lbz"
	);
	quote_B = strdup(
		"Uif!rvftujpo!pg!xifuifs!dpnqvufst!dbo!uijol!jt!mjlf!uif!rvftujpo!"
		"pg!xifuifs!tvcnbsjoft!dbo!txjn/ぞ!.!Fethfs!X/!Ejkltusb"
	);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, modifyA_printB, NULL);
	pthread_create(&tid2, NULL, modifyB_printA, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	free(quote_B);
	free(quote_A);

	return 0;
}
