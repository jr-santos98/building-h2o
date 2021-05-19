#include <stdio.h>
#include <stdlib.h>

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

int main(){
  printf("H2O 7\n");
  draw_h2o();

  return 0;
}
