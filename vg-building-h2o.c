/*
* Objetivo: O objetivo desse programa, é criar um visualizador global para uma
* aplicacao multi-threds.
* Descrição: Essa é uma aplicação multi threds que une moleculas de h2o, tendo threds
* com a molecula de oxigenio e hidrogenio.
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Numero de threads a serem executadas
#define N_OXI 6
#define N_HIDRO 12

// Semaforos definidos como globais, mais duas variáveis globais como volateis
sem_t mutex, fila_mutex_oxi, fila_mutex_hidro;
sem_t barreira, fila_oxi, fila_hidro;
sem_t draw;
volatile int oxigenio = 0, hidrogenio = 0;

// Desenha a fila de Oxigenio
void draw_oxygen(int altura, int qtde, int reposicionar) {
  int tamanho = altura-2, espacamento = 3;
  char figure = '*';

  // Cada execução representa uma linha
  for (int i = 0; i < altura; i++) {
    if (reposicionar == 1) printf("     ");
    // Desenha o número 'k' de objetos um dos lado do outro
    for (int k = 0; k < qtde; k++) {
      // Desenha os espaços antes
      for (int n = 0; n < abs(i-altura/2); n++) printf(" ");
      // Desenha a parte de cima
      for (int j = 0; j <= i + 1*i && i <= altura/2; j++) printf("%c", figure);
      // Desenha a parte de baixo
      for (int j = tamanho; i > altura/2 && j > 0; j--) printf("%c", figure);
      // Desenha os espaços depois
      for (int n = 0; n < abs(i-altura/2); n++) printf(" ");
      // Espaçamento entre os elementos
      for (int w = 0; w < espacamento; w++) printf(" ");
    }
    // Variavel que controla o desenho de baixo
    if (i > altura/2) tamanho = tamanho-2;
    printf("\n");
  }
}

// Desenha a fila de Oxigenio
void draw_hidro(int altura, int qtde) {
  int espacamento = 3;
  char figure = '*';
  // Cada execução representa uma linha
  for (int i = 0; i < altura; i++) {
    // Desenha o número 'k' de objetos um dos lado do outro
    for (int k = 0; k < qtde; k++) {
      // Desenha o lado esquerdo
      printf("%c", figure);
      // Desenha o centro
      for (int j = 0; j < altura-2; j++) {
        if (i == altura/2) printf("%c", figure);
        else printf(" ");
      }
      // Desenha o lado direito
      printf("%c", figure);
      // Espaçamento entre os elementos
      for (int w = 0; w < espacamento; w++) printf(" ");
    }
    printf("\n");
  }
}

// Desenha uma molecula da agua
void draw_h2o() {
  int tamanho = 7;
  draw_oxygen(tamanho, 1, 1);
  draw_hidro(tamanho, 2);
}

// Controlador da interface do visualizador
void atualiza_vg(int status) {
  int tamanho = 5;
  system("clear");
  printf("Fila de Oxigenio\n");
  draw_oxygen(tamanho, oxigenio, 0);
  printf("\n\n");
  printf("Fila de Hidrogenio\n");
  draw_hidro(tamanho, hidrogenio);
  if (status == 1) {
    printf("\n\n");
    printf("****************\n");
    printf("** Formou H2O **\n");
    printf("****************\n");
    draw_h2o();
  }
  sem_post(&draw);
}

void* f_oxigenio(void *v) {
  int volue;
  sleep(random()%3);
  sem_wait(&fila_mutex_oxi);
  sem_wait(&mutex);
  // Incrementando numero de oxigenios
  oxigenio = oxigenio + 1;
  sem_wait(&draw);
  atualiza_vg(0);
  sleep(1);
  if (hidrogenio >= 2) {
    sem_post(&fila_hidro);
    sem_post(&fila_hidro);
    // Ao moleculas são retiradas do contador de cada fila
    hidrogenio = hidrogenio - 2;
    sem_post(&fila_oxi);
    oxigenio = oxigenio - 1;
    sem_wait(&draw);
    atualiza_vg(0);
  } else sem_post(&mutex);

  // Aguardando fila de oxigenio
  sem_wait(&fila_oxi);

  // Desenha molecula da agua (vinculo das moleculas)
  sem_wait(&draw);
  atualiza_vg(1);
  sleep(1);

  // Oxi - Esperando passar pela barreira
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
  // Oxi - Formou molecula

  return NULL;
}

void* f_hidrogenio(void* v) {
  int volue;
  sleep(random()%3);
  sem_wait(&fila_mutex_hidro);
  sem_wait(&mutex);
  // Incrementando numero de hidrogenio
  hidrogenio = hidrogenio + 1;
  sem_wait(&draw);
  atualiza_vg(0);
  sleep(1);
  if (hidrogenio >= 2 && oxigenio >= 1) {
    sem_post(&fila_hidro);
    sem_post(&fila_hidro);
    // Ao moleculas são retiradas do contador de cada fila
    hidrogenio = hidrogenio - 2;
    sem_post(&fila_oxi);
    oxigenio = oxigenio - 1;
    sem_wait(&draw);
    atualiza_vg(0);
  } else sem_post(&mutex);

  // Aguardando fila do hidrogenio
  sem_wait(&fila_hidro);
  // Cria vinculo entre as moleculas
  // Esperando passar pela barreira
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
  // Hidro - Formou molecula

  return NULL;
}

int main() {
  pthread_t thr_oxigenio[N_OXI], thr_hidrogenio[N_HIDRO];
  int i;
  system("clear");

  // Iniciando Semaforos
  sem_init(&mutex, 0, 1);
  sem_init(&barreira, 0, 3);
  sem_init(&fila_oxi, 0, 0);
  sem_init(&fila_hidro, 0, 0);
  sem_init(&fila_mutex_oxi, 0, 1);
  sem_init(&fila_mutex_hidro, 0, 2);
  sem_init(&draw, 0, 1);

  // Criando threads
  for (i = 0; i < N_OXI; i++) pthread_create(&thr_oxigenio[i], NULL, f_oxigenio, NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_create(&thr_hidrogenio[i], NULL, f_hidrogenio, NULL);

  // Executando threads
  for (i = 0; i < N_OXI; i++) pthread_join(thr_oxigenio[i], NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_join(thr_hidrogenio[i], NULL);

  /* Uma gota da água é formada */

  return 0;
}
