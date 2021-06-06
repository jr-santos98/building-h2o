# Building H2O

Construção de uma solução e um visualizador global para o problema da contrução da molécula de H2O, presente no livro: [The Little Book of Semaphores](https://greenteapress.com/wp/semaphores/), by Allen Downey.

O pdf [*LittleBookOfSemaphores-páginas-155-160.pdf*](LittleBookOfSemaphores-páginas-155-160.pdf),
contém a parte exata do livro que contém o problema descrito. O problema apresentado pelo autor,
utiliza semaforo e barreira, e foi criado para ser realizado com apenas 3 threads.
O código transcrito exatamente como está no livro, está presente em:
[*building-h2o-origen.c*](building-h2o-origen.c).

## Modelo implementado

O modelo implementado em [*building-h2o.c*](building-h2o.c), teve algumas modificações da versão apresentada
no livro base. A versão do autor contava que seriam criadas apenas 3 threads, dessa forma,
ele não libera o semaforo que serve como barreira, que efita que as moleculás
sigam o caminho sem se juntarem formando a moleculá da água.

Ele utiliza a barreira de uma forma muito simples, pois não considera que mais 3 elementos poderão
esperar ou querer entrar logo em seguida, dessa forma, ela foi editada a realizar tal controle.
Por conta disso, foi realizado uma modificação, que restaura esse semaforo, permitindo
que outras moléculas da água possam ser formadas.

Além disso, surgiu outro problema durante a execução do algoritmo. Por conta de muitas threads
estarem esperando pela liberação do semaforo, acaba que várias moleculas começam
a serem formadas no mesmo momento, impedindo as filas de bloquear corretamente
o trafego delas, e permitindo que algumas moleculas sejam criadas erradas como: HO2, H3.
Dessa forma, foi colocado um sleep nas threads do hidrogenio.
Permitindo assim, a criação da molecula correta.

De fato a solução implementada a cima foi colocada, mas isso não é correta, destaca apenas que o algoritmo está errado.
A implementação do Sleep, foi utilizada inicialmente para tentar evitar um erro presente
na implementação da barreira. Esse erro surgiu ao utilizar n threads no lugar de apenas 3.
Por conta disso, a implementação da barreira foi alterada para impedir que esse tipo de problema aconteça.
E com essa correção, não se faz mais necessário a utilização de sleep, para impedir que moléculas erradas
sejam criadas, e threads "espertinhas" consigam pessar pela barreira sem ser a vez delas.
A implementação utilizada para corrigir esse erro, foi criar uma fila para o mutex,
de forma que cada molecula possa entrar no mutex apenas na sua quantidade limite.
A fila do mutex só é liberada, quando o ultimo componente da molecula h2o, passa pela barreira.

### Justificava para uma terceira Implementação

A necessidade de uma terceira implementação surgiu, com a ideia de criar o mesmo algoritmo
utilziando outra estretégia para realizar o controle das informações sensíveis entre as threads.
A diferença dessa implementação será com a substituição dos Semaforos por mutex e lock.
Houve uma diferença significativa da velocidade de processamento entre os dois modelos,
e o tamanho do código. Sendo a terceira implementação, a que possui o código
menor e mais rápido. Além disso, o modelo de visualização, também pode ser
implementado de uma forma melhor.

> Ao utilizar o comando `make`, será realizado a geração de todos os executáveis disponiveis.
Para executar apenas os de uma das implementações utilizadas, pode se utilizar as informações
contidas a baixo.

## Visualizador Global da 2ª implementação

O visualizador global foi implementado em: [*vg-building-h2o.c*](vg-building-h2o.c).
Ele utiliza Semaforos para fazer o controle das variaveis públicas entre as threads.

Para utilizar o visualizador global, basta executar os seguintes passos:

`make code-2`

`./vg-building-h2o`

O makefile também é configurado para que seja possível executar o código original,
com o comando `make origen`, e também compila o código inicial de desenho com o
comando `make draw`. Além disso, para remover todos os executáveis, basta
utilizar o comando: `make clean`.

## Visualizador Global da 3ª implementação

O visualizador global foi implementado em: [*2vg-lock-building-h2o.c*](2vg-lock-building-h2o.c).
Ele utiliza Lock e váriaveis de condição para fazer o controle das variaveis públicas entre as threads.

Para utilizar o visualizador global, basta executar os seguintes passos:

`make code-3`

`./vg-building-h2o`

O makefile também é configurado para que seja possível executar o código original,
com o comando `make origen`, e também compila o código inicial de desenho com o
comando `make draw`. Além disso, para remover todos os executáveis, basta
utilizar o comando: `make clean`.

### Criação do Visualizador Global

Para criar o visualizador, foi desenvolvido o código [*draw-h2o.c*](draw-h2o.c).
Nele está contido o modelo inicial e mais básico do visualizador, e não
o contém o código do construtor, possui apenas a parte visual do desenho.
Nele foi desenvolvido as funções que foram utilizadas e aprimoradas no visualizador.
