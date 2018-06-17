typedef struct Snake
{
  int x, y;
  int dir = 0; // Direção (1 - Cima, 2 - Direita, 3 - Baixo, 4 - Esquerda)
  Snake* prox = NULL; // Ponteiro para a próxima parte do corpo;
}  snake;

typedef struct Food
{
  int x, y;
} food;

void adicionaEmCima(Snake* s)
{
  s->prox = (Snake*) malloc(sizeof(Snake));
  s->prox->x = s->x;
  s->prox->y = s->y - 1;
  s->prox->prox = NULL;
  return;
}

void adicionaNaDireita(Snake* s)
{
  s->prox = (Snake*)malloc(sizeof(Snake));
  s->prox->x = s->x + 1;
  s->prox->y = s->y;
  s->prox->prox = NULL;
  return;
}

void adicionaEmBaixo(Snake* s)
{
  s->prox = (Snake*)malloc(sizeof(Snake));
  s->prox->x = s->x;
  s->prox->y = s->y + 1;
  s->prox->prox = NULL;
  return;
}

void adicionaNaEsquerda(Snake* s)
{
  s->prox = (Snake*)malloc(sizeof(Snake));
  s->prox->x = s->x - 1;
  s->prox->y = s->y;
  s->prox->prox = NULL;
  return;
}

void inicializaJogo(Snake* s, Food* f)
{
  s->x = random(4, 13); // Gera uma posição de Start para a Cobra no centro do mapa (da posição 4 à 12)
  s->y = random(4, 13);
  s->dir = random(1, 5); // Gera uma direção aleatória para a cobra pro jogo começar

  if (s->dir == 1) {       // Adiciona as outras 2 partes do corpo da cobra dependendo da direção que ela está indo
    adicionaEmBaixo(s);
    adicionaEmBaixo(s->prox);
  }
  if (s->dir == 2) {
    adicionaNaEsquerda(s);
    adicionaNaEsquerda(s->prox);
  }
  if (s->dir == 3) {
    adicionaEmCima(s);
    adicionaEmCima(s->prox);
  }
  if (s->dir == 4) {
    adicionaNaDireita(s);
    adicionaNaDireita(s->prox);
  }
}

void criaComida(Food* f, Snake* s)
{
  bool flag = true; // True se devo achar novo lugar
  Snake* atual = s;
  while (flag)
  {
    flag = false;
    // Gera um novo local aleatório para a comida
    f->x = random(16);
    f->y = random(16);

    // Confere se o alimento não foi gerado em algum local em que a cobra ocupa, percorrendo a cobra e conferindo as posições
    do {
      if (f->x == atual->x && f->y == atual->y)
        flag = true;
      atual = atual->prox;
    } while (atual != NULL && flag == true);
  }
  return;
}

void movimentaCobra(Snake* s, int* ultimo_x, int* ultimo_y)
{
  Snake* atual = s;

  int anterior_x = atual->x;  // Salva a atual posição da cabeça
  int anterior_y = atual->y;
  int novo_x; // Para auxiliar na troca de posições
  int novo_y;

  if (atual->dir == 1) {    // Atualiza a posição da cabeça de acordo com a direção garantindo que se for sair do tabuleiro vai aparecer do outro lado
    atual->y = (atual->y + 16 - 1) % 16;
  }
  else if (atual->dir == 2) {
    atual->x = (atual->x + 16 + 1) % 16;
  }
  else if (atual->dir == 3) {
    atual->y = (atual->y + 16 + 1) % 16;
  }
  else if (atual->dir == 4) {
    atual->x = (atual->x + 16 - 1) % 16;
  }

  atual = atual->prox;  // Avança para a próxima parte do corpo
  while (atual != NULL)
  {
    novo_x = atual->x;  // Salva a posição atual desse corpo
    novo_y = atual->y;
    
    atual->x = anterior_x;  // Atualiza a posição desse corpo para a posição do corpo anterior
    atual->y = anterior_y;

    anterior_x = novo_x;  // Auxilia na troca de posições novas e antigas
    anterior_y = novo_y;

    atual = atual->prox;  // Avança para o próximo corpo
  }

  *ultimo_x = anterior_x;
  *ultimo_y = anterior_y;

  return;
}

void imprimeTabuleiro(Snake* s, Food* f)
{
  Snake* atual = s;
  char tabuleiro[16][16];
  int i, j;

  for (i = 0; i < 16; i++)  // Cria o tabuleiro com pontos . . .
  {
    for (j = 0; j < 16; j++)
    {
      tabuleiro[i][j] = '.';
    }
  }

  while (atual != NULL) // Onde estiver a cobra será X X X X
  {
    tabuleiro[atual->x][atual->y] = 'X';
    atual = atual->prox;
  }

  tabuleiro[f->x][f->y] = 'o';  // Onde está a comida será o

  Serial.print(" ");  // Imprime o tabuleiro com o numero das linhas e colunas
  Serial.println("");
  for (i = 0; i < 16; i++)
  {
    for (j = 0; j < 16; j++)
    {
      Serial.print(tabuleiro[i][j]);
    }
    Serial.println("");
  }
}

void acendeLeds(Snake* s, Food* f, int* ledsX, int n, int* ledsY, int m)
{
  Snake* atual = s;
  int t = 10; // Tempo de espera por led aceso t (ms)
  byte aux = 0;

  // Acender os Leds para a Cobra
  for(atual = s; atual != NULL; atual = atual->prox)
  {
      // Pega a posição atual do corpo em X e Y e converte para mandar o sinal
      // Ex: X = 10 -> 10 = 1010 -> Manda HIGH para o 1º e o 3º Fio para acenter o 11º led
      aux = (byte) atual->x;
      for(int i = 0; i < 4; i++)
      {
        digitalWrite(ledsX[i], bool(aux & B0001));
        aux = aux >> 1;
      }
      
      aux = (byte) atual->y;
      for(int i = 0; i < 4; i++)
      {
        digitalWrite(ledsY[i], bool(aux & B0001));
        aux = aux >> 1;
      }
      
      // Espera um tempo aceso e então desliga o led
      delay(t);
      for(int i = 0; i < 4; i++)
      {
        digitalWrite(ledsX[i], LOW);
        digitalWrite(ledsY[i], LOW);
      }
  }

  // Acende os Leds da Comida
  // Pega a posição atual do corpo em X e Y e converte para mandar o sinal
  // Ex: X = 15 -> 15 = 1111 -> Manda HIGH para todos os Fios para acenter o 16º led
  aux = (byte) f->x;
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(ledsX[i], bool(aux & B0001));
    aux = aux >> 1;
  }
  
  aux = (byte) f->y;
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(ledsY[i], bool(aux & B0001));
    aux = aux >> 1;
  }
  
  // Espera um tempo aceso e então desliga o led
  delay(t);
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(ledsX[i], LOW);
    digitalWrite(ledsY[i], LOW);
  }
}


