# Building H2O

Construção de uma solução e um visualizador global para o problema da contrução da molécula de H2O, presente no livro: [The Little Book of Semaphores](https://greenteapress.com/wp/semaphores/), by Allen Downey.

O pdf [*LittleBookOfSemaphores-páginas-155-160.pdf*](LittleBookOfSemaphores-páginas-155-160.pdf),
contém a parte exata do livro que contém o problema descrito.
O código transcrito exatamente como está no livro, está presente em:
[*building-h2o-origen.c*](building-h2o-origen.c).

## Modelo implementado

O modelo implementado em [*building-h2o.c*](building-h2o.c), teve algumas modificações da versão apresentada
no livro base. A versão do autor contava que seriam criadas apenas 3 threads, dessa forma,
ele não libera o semaforo que serve como barreira, que efita que as moleculás
sigam o caminho sem se juntarem formando a moleculá da água.
Por conta disso, foi realizado uma modificação, que restaura esse semaforo, permitindo
que outras moléculas da água possam ser formadas.

Além disso, surgiu outro problema durante a execução do algoritmo. Por conta de muitas threads
estarem esperando pela liberação do semaforo, acaba que várias moleculas começam
a serem formadas no mesmo momento, impedindo as filas de bloquear corretamente
o trafego delas, e permitindo que algumas moleculas sejam criadas erradas como: HO2, H3.
Dessa forma, foi colocado um sleep nas threads do hidrogenio.
Permitindo assim, a criação da molecula correta.

## Visualizador Global

O visualizador global foi implementado em: [*vg-building-h2o.c*](vg-building-h2o.c).

Para utilizar o visualizador global, basta executar os seguintes passos:

`make`
`./vg-building-h2o`

### Criação do Visualizador Global

Para criar o visualizador, foi desenvolvido o código [*draw-h2o.c*](draw-h2o.c).
Nele está contido o modelo inicial e mais básico do visualizador, e não
o contém o código do construtor, possui apenas a parte visual do desenho.
Nele foi desenvolvido as funções que foram utilizadas e aprimoradas no visualizador.
