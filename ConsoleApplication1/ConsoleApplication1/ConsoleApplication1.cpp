// ConsoleApplication1.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "stdafx.h"
#include <time.h>	// nada ainda
#include <Windows.h>// Sleep() 
#include <stdlib.h>	// rand()
#include <conio.h>	// getch() e kbhit()

#pragma warning(disable : 4996)

int Alt = 16;
int Lar = 16;

struct Snake
{
	int x, y;
	int dir = 0; // Direção (1 - Cima, 2 - Direita, 3 - Baixo, 4 - Esquerda)
	Snake* prox = NULL; // Ponteiro para a próxima parte do corpo;
}  s;

struct Food
{
	int x, y;
} f;

void adicionaEmCima(Snake* s)
{
	s->prox = (Snake*)malloc(sizeof(Snake));
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

void criaComida(Food* f, Snake* s)
{
	bool flag = true;	// True se devo achar novo lugar
	Snake* atual = s;
	while (flag)
	{
		flag = false;
		atual = s;
		
		// Gera um novo local aleatório para a comida
		f->x = rand() % 16;
		f->y = rand() % 16;

		// Confere se o alimento não foi gerado em algum local em que a cobra ocupa, percorrendo a cobra e conferindo as posições
		do {
			if (f->x == atual->x && f->y == atual->y)
				flag = true;
			atual = atual->prox;
		} while (atual != NULL);
	}
	return;
}

void movimentaCobra(Snake* s, int* ultimo_x, int* ultimo_y)
{
	Snake* atual = s;

	int anterior_x = atual->x;	// Salva a atual posição da cabeça
	int anterior_y = atual->y;
	int novo_x;	// Para auxiliar na troca de posições
	int novo_y;

	if (atual->dir == 1) {		// Atualiza a posição da cabeça de acordo com a direção garantindo que se for sair do tabuleiro vai aparecer do outro lado
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

	atual = atual->prox;	// Avança para a próxima parte do corpo
	while (atual != NULL)
	{
		novo_x = atual->x;	// Salva a posição atual desse corpo
		novo_y = atual->y;

		atual->x = anterior_x;	// Atualiza a posição desse corpo para a posição do corpo anterior
		atual->y = anterior_y;

		anterior_x = novo_x;	// Auxilia na troca de posições novas e antigas
		anterior_y = novo_y;

		atual = atual->prox;	// Avança para o próximo corpo
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

	for (i = 0; i < 16; i++)	// Cria o tabuleiro com pontos . . .
	{
		for (j = 0; j < 16; j++)
		{
			tabuleiro[i][j] = '.';
		}
	}

	tabuleiro[f->y][f->x] = 'o';	// Onde está a comida será o

	tabuleiro[atual->y][atual->x] = 'O';	// A cabeça da cobra será O
	atual = atual->prox;
	while (atual != NULL)	// Onde for o corpo da cobra será S
	{
		tabuleiro[atual->y][atual->x] = 'X';
		atual = atual->prox;
	}

	printf("  \t");	// Imprime o tabuleiro com o numero das linhas e colunas
	for (i = 0; i < 16; i++)
		printf("%i ", i);
	printf("\n");
	for (i = 0; i < 16; i++)
	{
		printf("%i \t", i);
		for (j = 0; j < 16; j++)
		{
			printf(" %c", tabuleiro[i][j]);
		}
		printf("\n");
	}
}


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
	Snake s;
	Food f;
	Snake* atual = NULL;
	Snake* anterior = NULL;
	int pontuacao = 0;
	int ultimo_x = 0;
	int ultimo_y = 0;
	bool colidiu = 0;
	bool comeu = 0;

	srand(time(NULL));

	s.x = rand() % 8 + 4; // Gera uma posição de Start para a Cobra no centro do mapa (da posição 4 à 12)
	s.y = rand() % 8 + 4;
	s.dir = rand() % 4 + 1; // Gera uma direção aleatória para a cobra pro jogo começar

	if (s.dir == 1) {				// Adiciona as outras 2 partes do corpo da cobra dependendo da direção que ela está indo
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

	while (!colidiu) {

		Sleep(350);
		fflush(stdin);
		if (kbhit())	// Se tem alguma tecla sendo apertada, Pego a tecla e mudo a direção da cobra de acordo com a tecla
		{
			fflush(stdin);
			switch (getch())
			{
			case 'w':
				if (s.dir != 3)	// Não vai ir pra cima caso esteja indo pra baixo
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

		if (comeu)	// Se comeu na iteração passada adiciona um novo bloco na cauda da cobra exatamente no lugar da corpo na ultima iteração
		{
			atual = &s;
			while (atual->prox != NULL)		// Vamos até o ultimo bloco
				atual = atual->prox;
			atual->prox = (Snake*)malloc(sizeof(Snake));	// O proximo é um novo bloco
			atual->prox->x = ultimo_x;		// O lugar dele é na posição do ultimo bloco que saiu de lá
			atual->prox->y = ultimo_y;
			atual->prox->prox = NULL;
			pontuacao += 1;		// Atualiza pontuação
			comeu = 0;			// Vai ter que comer de novo
			criaComida(&f, &s);	// Cria nova comida
		}

		if (s.x == f.x && s.y == f.y)	// Verifica se a cobra comeu
		{
			comeu = 1;
		}

		system("@cls||clear");
		printf("Pontuação: %d \n\n", pontuacao);
		imprimeTabuleiro(&s, &f);

		atual = s.prox;			// Verifica se a cobra bateu no prórpio corpo
		while (atual != NULL)
		{
			if (atual->x == s.x && atual->y == s.y) {	// Se bateu não vai mais repetir o loop do jogo
				colidiu = 1;
			}
			atual = atual->prox;
		}

	}

	printf("Fim de Jogo!!!");
	fflush(stdin);
	getchar();

	return 0;
}