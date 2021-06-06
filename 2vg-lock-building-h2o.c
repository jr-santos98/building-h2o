/*
* Objetivo: O objetivo desse programa, é criar um visualizador global para uma
* aplicacao multi-threds.
* Descrição: Essa é uma aplicação multi threds que une moleculas de h2o, tendo threds
* com a molecula de oxigenio e hidrogenio.
* Metodo: É utilizado Lock e Váriaveis de condição para realizar as operações
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Numero de threads a serem executadas
#define N_OXI 6
#define N_HIDRO 12

//sem_t fila_oxi, fila_hidro;
volatile int oxigenio = 0, hidrogenio = 0;
volatile int fila_oxigenio = 0, fila_hidrogenio = 0;

// Utilizando Locks e variáveis de condição
// sem verificação de erros
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t draw = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fila_oxi = PTHREAD_COND_INITIALIZER;
pthread_cond_t fila_hidro = PTHREAD_COND_INITIALIZER;
pthread_cond_t barreira_oxi = PTHREAD_COND_INITIALIZER;
pthread_cond_t barreira_hidro = PTHREAD_COND_INITIALIZER;

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
  printf("Fila espera de Oxigenio\n");
  draw_oxygen(tamanho-2, fila_oxigenio, 0);
  printf("\n");
  printf("Fila espera de Hidrogenio\n");
  draw_hidro(tamanho-2, fila_hidrogenio);
  printf("\n");
  printf("Fila de Oxigenio\n");
  draw_oxygen(tamanho, oxigenio, 0);
  printf("\n\n");
  printf("Fila de Hidrogenio\n");
  draw_hidro(tamanho, hidrogenio);
  if (status == 1) {
    //printf("\n\n");
    printf("****************\n");
    printf("** Formou H2O **\n");
    printf("****************\n");
    draw_h2o();
  }
  pthread_mutex_unlock(&draw);
}

void* f_oxigenio(void *v) {
  sleep(random()%3);
  pthread_mutex_lock(&lock);
  fila_oxigenio++;
  while (oxigenio != 0) {
    pthread_mutex_lock(&draw);
    atualiza_vg(0);
    sleep(1);
    printf(".\n");
    pthread_cond_wait(&fila_oxi, &lock);
  }
  fila_oxigenio--;
  oxigenio++;
  pthread_mutex_lock(&draw);
  atualiza_vg(0);
  sleep(1);
  while (hidrogenio <= 1) {
    pthread_cond_wait(&barreira_oxi, &lock);
  }
  hidrogenio = 0;
  oxigenio = 0;
  // Formar molecula
  // Formou H2O
  pthread_mutex_lock(&draw);
  atualiza_vg(1);
  sleep(1);
  pthread_cond_signal(&fila_oxi);
  pthread_cond_broadcast(&fila_hidro);
  pthread_mutex_unlock(&lock);

  return NULL;
}

void* f_hidrogenio(void* v) {
  sleep(random()%3);
  pthread_mutex_lock(&lock);
  fila_hidrogenio++;
  while (hidrogenio > 1) {
    pthread_mutex_lock(&draw);
    atualiza_vg(0);
    sleep(1);
    printf("...\n");
    pthread_cond_wait(&fila_hidro, &lock);
  }
  fila_hidrogenio--;
  hidrogenio++;
  pthread_mutex_lock(&draw);
  atualiza_vg(0);
  sleep(1);
  while (hidrogenio == 1) {
    pthread_cond_wait(&barreira_hidro, &lock);
    pthread_cond_signal(&barreira_oxi);
  }
  // Formar molecula
  pthread_cond_signal(&barreira_hidro);
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main() {
  pthread_t thr_oxigenio[N_OXI], thr_hidrogenio[N_HIDRO];
  int i;
  system("clear");

  // Criando threads
  for (i = 0; i < N_OXI; i++) pthread_create(&thr_oxigenio[i], NULL, f_oxigenio, NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_create(&thr_hidrogenio[i], NULL, f_hidrogenio, NULL);

  // Executando threads
  for (i = 0; i < N_OXI; i++) pthread_join(thr_oxigenio[i], NULL);
  for (i = 0; i < N_HIDRO; i++) pthread_join(thr_hidrogenio[i], NULL);

  /* Uma gota da água é formada */

  return 0;
}
