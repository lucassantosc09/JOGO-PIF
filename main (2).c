#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "keyboard.h"
#include "screen.h"

#define largura 60
#define altura 30
#define arquivoScore "score.txt"
#define velocidadeInicial 200000 
#define velocidadePosmaca 250000 

#define corverde "\x1b[32m"
#define corvermelha "\x1b[31m"
#define resetarRestante "\x1b[0m"

typedef struct Ponto {
 int x, y;
} Ponto;

typedef struct CaudaCobra {
 Ponto posicao;
 struct CaudaCobra *ptr;
} CaudaCobra;

typedef struct {
 CaudaCobra *cabeca;
 CaudaCobra *cauda;
 int comprimento;
 Ponto direcao;
} Cobra;

typedef struct {
 Ponto posicao;
} Comida;


void salvarPontuacao(int pontuacao);
void carregarPontuacoes();
void alocarMemoriaCobra(Cobra *cobra);

void start(Cobra *cobra, Comida *comida, int *velocidade) {
CaudaCobra *cabecaCauda =(CaudaCobra*)malloc(sizeof(CaudaCobra));
    
 cabecaCauda -> posicao.x = largura / 2;
 cabecaCauda -> posicao.y = altura / 2;
 cabecaCauda-> ptr = NULL;

 cobra -> cabeca = cabecaCauda;
 cobra -> cauda = cabecaCauda;
 cobra-> comprimento = 1;
 cobra-> direcao.x = 1; 
 cobra -> direcao.y = 0;

 comida-> posicao.x = rand() % largura;
 comida-> posicao.y = rand() % altura;

 *velocidade = velocidadeInicial;
}

void borda(Cobra *cobra, Comida *comida) {
    screenClear();

 printf(" ");
 for (int i = 0; i < largura; i++) {
   printf("#");
 }
   printf("\n");

for (int i = 0; i < altura; i++) {
        printf("#");
    
 for (int j = 0; j < largura; j++) {
  int posicapCobra = 0;
  CaudaCobra *atual = cobra -> cabeca;
     
while (atual != NULL) {
    
 if (atual ->posicao.x == j && atual -> posicao.y == i) {
  printf(corverde "x" resetarRestante);
     
    posicapCobra = 1;
    break;
 }
atual = atual -> ptr;
}
if (!posicapCobra) {
    if (comida -> posicao.x == j && comida -> posicao.y == i) {
    printf(corvermelha "O" resetarRestante);
     } else {
     printf(" ");
     }
    }
   }
    printf("#\n");
}


    printf(" ");
    for (int i = 0; i < largura; i++) {
    printf("#");
    }
    printf("\n");
}

void atualizarCobra(Cobra *cobra) {
CaudaCobra * novaCabeca = (CaudaCobra *) malloc (sizeof(CaudaCobra));
    novaCabeca-> posicao.x = cobra -> cabeca -> posicao.x + cobra->direcao.x;
    novaCabeca->posicao.y = cobra -> cabeca-> posicao.y + cobra-> direcao.y;
    novaCabeca-> ptr = cobra->cabeca;
    cobra -> cabeca = novaCabeca;

if (novaCabeca ->  posicao.x < 0 || novaCabeca -> posicao.x >= largura || novaCabeca-> posicao.y < 0 || novaCabeca->  posicao.y >= altura) {
printf("Game over ! pontuação: %d\n", cobra->comprimento - 1);
 salvarPontuacao(cobra-> comprimento - 1); 
  printf("\nPontuações anteriores:\n");
 carregarPontuacoes(); 
 alocarMemoriaCobra(cobra); 
 exit(0); 
    }
}

void paraFruta(Cobra *cobra, Comida *comida, int *velocidade) {
if (cobra -> cabeca->posicao.x == comida->posicao.x && cobra ->cabeca-> posicao.y == comida ->posicao.y) {
 cobra -> comprimento++;
 comida -> posicao.x = rand() % largura;
 comida -> posicao.y = rand() % altura;
  if (*velocidade > velocidadePosmaca) {
    *velocidade = *velocidade - velocidadePosmaca;
  } else {
  *velocidade = velocidadePosmaca;
}
} else {
   CaudaCobra *corpo = cobra->cabeca;
    
    while (corpo -> ptr ->ptr != NULL) {
    corpo = corpo-> ptr;
}
        
    free(corpo -> ptr);
    corpo-> ptr= NULL;
    cobra ->cauda = corpo;
    }
}

void salvarPontuacao(int pontuacao) {
 FILE *arquivo = fopen(arquivoScore, "a");
 
  if (arquivo == NULL) {
  fprintf(stderr, "erro ao abrir o arquivo: %s\n", arquivoScore);
  return;
}
  fprintf(arquivo, "%d\n", pontuacao);
  fclose(arquivo);
}

void carregarPontuacoes() {
 FILE *arquivo = fopen(arquivoScore, "r");
 if (arquivo == NULL) {
  fprintf(stderr, "erro ao abrir o arquivo: %s\n", arquivoScore);
  return;
}
  int pontuacao;
  while (fscanf(arquivo, "%d", &pontuacao) != EOF) {
   printf("%d ", pontuacao);
}
    fclose(arquivo);
}

void tela() {
    screenClear();
    screenUpdate();
}

void menuPrincipal() {
    screenClear();
    printf("Bem vindo ao jogo da cobrinha !\n\n");
    printf("pressione qualquer botao para começar :)\n");
}

void alocarMemoriaCobra(Cobra *cobra) {
    CaudaCobra *corpo = cobra ->cabeca;
    while (corpo  != NULL) {
        CaudaCobra *ptr = corpo -> ptr;
        free(corpo ); 
        corpo =ptr;
    }
    cobra -> cabeca = NULL;
    cobra -> cauda = NULL;
}

int main() {
    Cobra cobra;
    Comida comida;
    char teclapre;
    int pontuacao = 0;
    int velocidade;

    srand(time(NULL));
    keyboardInit();
    tela();

while (1) {
 menuPrincipal();
 getchar(); 
 start(&cobra, &comida, &velocidade);

 while (1) {
  if (keyhit()) {
    teclapre = readch();
    switch (teclapre) {
    case 'w':
    if (cobra.direcao.y == 0) {
     cobra.direcao.x = 0;
      cobra.direcao.y = -1;
 }
  break;
     case 's':
      if (cobra.direcao.y == 0) {
      cobra.direcao.x = 0;
      cobra.direcao.y = 1;
}
  break;
     case 'a':
      if (cobra.direcao.x == 0) {
      cobra.direcao.x = -1;
      cobra.direcao.y = 0;
}
 break;
     case 'd':
      if (cobra.direcao.x == 0) {
      cobra.direcao.x = 1;
     cobra.direcao.y = 0;
}
 break;
     case 'q':
     salvarPontuacao(pontuacao);
     alocarMemoriaCobra(&cobra);
    return 0;
 }
}
     atualizarCobra(&cobra);
     paraFruta(&cobra, &comida, &velocidade);
     borda(&cobra, &comida);
     usleep(velocidade); 
     pontuacao = cobra.comprimento - 1;
        }
}

    return 0;
}
