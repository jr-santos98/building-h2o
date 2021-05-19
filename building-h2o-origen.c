/*
* Descrição: Transcrição do problema de contrução de uma molecula da agua.
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N_OXI 1
#define N_HIDRO 2

sem_t mutex;
sem_t barreira;
sem_t fila_oxi;
sem_t fila_hidro;
volatile int oxigenio = 0, hidrogenio = 0;

void* f_oxigenio(void *v) {
  sem_wait(&mutex);
  oxigenio = oxigenio + 1;
  if (hidrogenio >= 2) {
    sem_post(&fila_hidro);
    sem_post(&fila_hidro);
    hidrogenio = hidrogenio - 2;
    sem_post(&fila_oxi);
    oxigenio = oxigenio - 1;
  } else sem_post(&mutex);

  sem_wait(&fila_oxi);
  // Cria vinculo
  sem_wait(&barreira);
  sem_post(&mutex);

  return NULL;
}

void* f_hidrogenio(void* v) {
  sem_wait(&mutex);
  hidrogenio = hidrogenio + 1;
  if (hidrogenio >= 2 && oxigenio >= 1) {
    sem_post(&fila_hidro);
    sem_post(&fila_hidro);
    hidrogenio = hidrogenio - 2;
    sem_post(&fila_oxi);
    oxigenio = oxigenio - 1;
  } else sem_post(&mutex);

  sem_wait(&fila_hidro);
  // Cria vinculo
  sem_wait(&barreira);

  return NULL;
}

int main() {
  pthread_t thr_oxigenio[N_OXI], thr_hidrogenio[N_HIDRO];
  int i;

  sem_init(&mutex, 0, 1);
  sem_init(&barreira, 0, 3);
  sem_init(&fila_oxi, 0, 0);
  sem_init(&fila_hidro, 0, 0);

  for (i = 0; i < N_OXI; i++) pthread_create(&thr_oxigenio[i], NULL, f_oxigenio, NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_create(&thr_hidrogenio[i], NULL, f_hidrogenio, NULL);

  for (i = 0; i < N_OXI; i++) pthread_join(thr_oxigenio[i], NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_join(thr_hidrogenio[i], NULL);

  return 0;
}
