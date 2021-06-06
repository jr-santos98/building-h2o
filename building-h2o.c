/*
* Descrição: Essa é uma aplicação multi threds que une moleculas de h2o, tendo threds
* com a molecula de oxigenio e hidrogenio.
* Metodo: É utilizado Semaforos para realizar as operações
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Numero de threads a serem executadas
#define N_OXI 5
#define N_HIDRO 10

// Semaforos definidos como globais, mais duas variáveis globais como volateis
sem_t mutex, fila_mutex_oxi, fila_mutex_hidro;
sem_t barreira, fila_oxi, fila_hidro;
volatile int oxigenio = 0, hidrogenio = 0;

void* f_oxigenio(void *v) {
  int volue;
  sleep(random()%3);
  sem_wait(&fila_mutex_oxi);
  printf("Oxigenio esperando a vez\n");
  sem_wait(&mutex);
  // Incrementando numero de oxigenios
  oxigenio = oxigenio + 1;
  if (hidrogenio >= 2) {
    printf("Liberando hidrogenios e oxigenios\n");
    sem_post(&fila_hidro);
    sem_post(&fila_hidro);
    // Ao moleculas são retiradas do contador de cada fila
    hidrogenio = hidrogenio - 2;
    sem_post(&fila_oxi);
    oxigenio = oxigenio - 1;
  } else sem_post(&mutex);

  printf("Aguardando fila de oxigenio\n");
  sem_wait(&fila_oxi);
  // Cria vinculo
  printf("Oxi - Esperando passar pela barreira\n");
  sem_wait(&barreira);
  // Prepara barreira para formar uma nova molecula
  sem_getvalue(&barreira, &volue);
  if (volue == 0) {
    sem_post(&barreira);
    sem_post(&barreira);
    sem_post(&barreira);
    sem_post(&fila_mutex_oxi);
    sem_post(&fila_mutex_hidro);
    sem_post(&fila_mutex_hidro);
  }
  sem_post(&mutex);
  printf("Oxi - Formou molecula\n");

  return NULL;
}

void* f_hidrogenio(void* v) {
  int volue;
  sleep(random()%3);
  sem_wait(&fila_mutex_hidro);
  printf("Hidrogenio esperando a vez\n");
  sem_wait(&mutex);
  // Incrementando numero de hidrogenio
  hidrogenio = hidrogenio + 1;
  sleep(1);
  if (hidrogenio >= 2 && oxigenio >= 1) {
    printf("Liberando hidrogenios e oxigenios\n");
    sem_post(&fila_hidro);
    sem_post(&fila_hidro);
    // Ao moleculas são retiradas do contador de cada fila
    hidrogenio = hidrogenio - 2;
    sem_post(&fila_oxi);
    oxigenio = oxigenio - 1;
  } else sem_post(&mutex);

  printf("Aguardando fila do hidrogenio\n");
  sem_wait(&fila_hidro);
  // Cria vinculo
  printf("Hidro - Esperando passar pela barreira\n");
  sem_wait(&barreira);
  // Prepara barreira para formar uma nova molecula
  sem_getvalue(&barreira, &volue);
  if ( volue == 0) {
    sem_post(&barreira);
    sem_post(&barreira);
    sem_post(&barreira);
    sem_post(&fila_mutex_oxi);
    sem_post(&fila_mutex_hidro);
    sem_post(&fila_mutex_hidro);
  }
  printf("Hidro - Formou molecula\n");

  return NULL;
}

int main() {
  pthread_t thr_oxigenio[N_OXI], thr_hidrogenio[N_HIDRO];
  int i;

  // Iniciando Semaforos
  sem_init(&mutex, 0, 1);
  sem_init(&barreira, 0, 3);
  sem_init(&fila_oxi, 0, 0);
  sem_init(&fila_hidro, 0, 0);
  sem_init(&fila_mutex_oxi, 0, 1);
  sem_init(&fila_mutex_hidro, 0, 2);
  // Criando threads
  for (i = 0; i < N_OXI; i++) pthread_create(&thr_oxigenio[i], NULL, f_oxigenio, NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_create(&thr_hidrogenio[i], NULL, f_hidrogenio, NULL);

  // Executando threads
  for (i = 0; i < N_OXI; i++) pthread_join(thr_oxigenio[i], NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_join(thr_hidrogenio[i], NULL);

  /* Uma gota da água é formada */

  return 0;
}
