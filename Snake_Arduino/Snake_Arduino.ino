#include "funcoes.h"

  Snake s;
  Food f;
  Snake* atual = NULL;
  Snake* anterior = NULL;
  int pontuacao = 0;
  int ultimo_x = 0;
  int ultimo_y = 0;
  bool colidiu = false;
  bool comeu = false;

void setup() {
  // put your setup code here, to run once:

  srand(time(NULL));

  s.x = rand() % 8 + 4; // Gera uma posição de Start para a Cobra no centro do mapa (da posição 4 à 12)
  s.y = rand() % 8 + 4;
  s.dir = rand() % 4 + 1; // Gera uma direção aleatória para a cobra pro jogo começar

  if (s.dir == 1) {       // Adiciona as outras 2 partes do corpo da cobra dependendo da direção que ela está indo
    adicionaEmBaixo(&s);
    adicionaEmBaixo(s.prox);
  }
  if (s.dir == 2) {
    adicionaNaEsquerda(&s);
    adicionaNaEsquerda(s.prox);
  }
  if (s.dir == 3) {
    adicionaEmCima(&s);
    adicionaEmCima(s.prox);
  }
  if (s.dir == 4) {
    adicionaNaDireita(&s);
    adicionaNaDireita(s.prox);
  }
  criaComida(&f, &s);

}

void loop() {
  // put your main code here, to run repeatedly:
  while(!colidiu)
  {
    fflush(stdin);
    if (kbhit())  // Se tem alguma tecla sendo apertada, Pego a tecla e mudo a direção da cobra de acordo com a tecla
    {
      fflush(stdin);
      switch (getch())
      {
        case 'w':
          if (s.dir != 3) // Não vai ir pra cima caso esteja indo pra baixo
            s.dir = 1;
          break;
        case 'd':
          if (s.dir != 4)
            s.dir = 2;
          break;
        case's':
          if (s.dir != 1)
            s.dir = 3;
          break;
        case'a':
          if (s.dir != 2)
            s.dir = 4;
          break;
        default:
          break;
      }
    }

    movimentaCobra(&s, &ultimo_x, &ultimo_y);

    if (comeu)  // Se comeu na iteração passada adiciona um novo bloco na cauda da cobra exatamente no lugar da corpo na ultima iteração
    {
      atual = &s;
      while (atual->prox != NULL)   // Vamos até o ultimo bloco
        atual = atual->prox;
      atual->prox = (Snake*)malloc(sizeof(Snake));  // O proximo é um novo bloco
      atual->prox->x = ultimo_x;    // O lugar dele é na posição do ultimo bloco que saiu de lá
      atual->prox->y = ultimo_y;
      pontuacao += 1;   // Atualiza pontuação
      comeu = 0;      // Vai ter que comer de novo
      criaComida(&f, &s); // Cria nova comida
    }

    if (s.x == f.x && s.y == f.y) // Verifica se a cobra comeu
    {
      comeu = 1;
    }

    delay(700);
    system("@cls||clear");
    printf("Pontuação: %d \n\n", pontuacao);
    imprimeTabuleiro(&s, &f);

    atual = s.prox;     // Verifica se a cobra bateu no prórpio corpo
    while (atual != NULL)
    {
      if (atual->x == s.x && atual->y == s.y){  // Se bateu não vai mais repetir o loop do jogo
        colidiu = true;
      }
      atual = atual->prox;
    }
  }
}

// ConsoleApplication1.cpp : define o ponto de entrada para o aplicativo do console.
//




/// INICIALIZAR A COBRA
/// INICIALIZAR A COMIDA
/// MUDAR A DIREÇÃO DO MOVIMENTO DA COBRA DE ACORDO COM O USUÁRIO
/// FAZER A COBRA APARECER DO OUTRO LADO DO MAPA (PASSOU DE 0 VAI PRA 16 E VICE VERSA)
/// MOVIMENTA A COBRA
/// COLOCAR NOVO "BLOCO DE CORPO" CASO TENHA COMIDO NA ITERAÇÃO PASSADA E ATUALIZAR PONTUAÇAO
/// CONFERIR SE BATEU E ACABAR COM O JOGO
/// SE COMEU COLOCAR UM NOVO "PEDAÇO DE CORPO" NA PROXIMA VEZ QUE FOR ATUALIZAR O CORPO
/// IMPRIMIR NA TELA O JOGO E A PONTUAÇÃO
/// IMPLEMENTAR O DELAY PARA CADA LOOP "VELOCIDADE DA COBRA"
/// REPETIR ^ ^
// ARRUMAR TABULEIRO PRA DEIXAR MAIS BONITO
// ARRUMAR INPUT DA COBRA (APERTAR PRA CIMA QND ELA TA INDO PRA BAIXO)
// FAZER FIM DE JOGO



int main()
{
  while (!colidiu) {

  }

  printf("Fim de Jogo!!!");
  fflush(stdin);
  getchar();

  return 0;
}
