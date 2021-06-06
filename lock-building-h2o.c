/*
* Descrição: Essa é uma aplicação multi threds que une moleculas de h2o, tendo threds
* com a molecula de oxigenio e hidrogenio.
* Metodo: É utilizado Lock e Váriaveis de condição para realizar as operações
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Numero de threads a serem executadas
#define N_OXI 5
#define N_HIDRO 10

//sem_t fila_oxi, fila_hidro;
volatile int oxigenio = 0, hidrogenio = 0;

// Utilizando Locks e variáveis de condição
// sem verificação de erros
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fila_oxi = PTHREAD_COND_INITIALIZER;
pthread_cond_t fila_hidro = PTHREAD_COND_INITIALIZER;
pthread_cond_t barreira_oxi = PTHREAD_COND_INITIALIZER;
pthread_cond_t barreira_hidro = PTHREAD_COND_INITIALIZER;

void* f_oxigenio(void *v) {
  sleep(random()%3);
  printf("Oxigenio esperando a vez\n");
  pthread_mutex_lock(&lock);
  while (oxigenio != 0) {
    printf("Oxigenio esperando na fila\n");
    pthread_cond_wait(&fila_oxi, &lock);
  }
  printf("Oxigenio verificando hidrogenio\n");
  oxigenio++;
  while (hidrogenio <= 1) {
    printf("Oxigenio esperando por hidrogenio\n");
    pthread_cond_wait(&barreira_oxi, &lock);
  }
  printf("Oxigenio monta a molecula\n");
  hidrogenio = 0;
  oxigenio = 0;
  // Formar molecula
  printf("Formou H2O\n");
  pthread_cond_signal(&fila_oxi);
  pthread_cond_broadcast(&fila_hidro);
  pthread_mutex_unlock(&lock);

  return NULL;
}

void* f_hidrogenio(void* v) {
  sleep(random()%3);
  printf("Hidrogenio esperando a vez\n");
  pthread_mutex_lock(&lock);
  while (hidrogenio > 1) {
    printf("Hidrogenio esperando na fila\n");
    pthread_cond_wait(&fila_hidro, &lock);
  }
  printf("Hidrogenio verificando hidrogenio\n");
  hidrogenio++;
  while (hidrogenio == 1) {
    printf("Hidrogenio esperando por hidrogenio\n");
    pthread_cond_wait(&barreira_hidro, &lock);
    pthread_cond_signal(&barreira_oxi);
  }
  printf("Hidrogenio formando molecula\n");
  // Formar molecula
  printf("Hidrogenio libera barreira\n");
  pthread_cond_signal(&barreira_hidro);
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main() {
  pthread_t thr_oxigenio[N_OXI], thr_hidrogenio[N_HIDRO];
  int i;

  // Criando threads
  for (i = 0; i < N_OXI; i++) pthread_create(&thr_oxigenio[i], NULL, f_oxigenio, NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_create(&thr_hidrogenio[i], NULL, f_hidrogenio, NULL);

  // Executando threads
  for (i = 0; i < N_OXI; i++) pthread_join(thr_oxigenio[i], NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_join(thr_hidrogenio[i], NULL);

  /* Uma gota da água é formada */

  return 0;
}
