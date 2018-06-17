#include <LiquidCrystal.h>
#include "funcoes.h"

/* Declaração dos Pinos:
 *                                         Y
 *  Joystick:  Eixo X = A5    ---|GND      /\
 *             Eixo Y = A4    ---|5V       0
 *             Button = A3    ---|VRx   0 -|- 1023  -> X
 *                            ---|VRy     1023
 *
 *  LCD:
 *
 *
 *
 *  Buzzer:
 *
 *
 *
 *  MatrizLed: Leds X = {52, 50, 48, 46}
 *             Leds Y = {53, 51, 49, 47}
 *
 */

  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
  int ledsX[4] = {53, 51, 49, 47};  // X = {53, 51, 49, 47}
  int ledsY[4] = {52, 50, 48, 46};  // Y = {52, 50, 48, 46}
  int joy_x = A5;
  int joy_y = A4;
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

  Serial.begin(9600);
  lcd.begin(16,2);
  
  pinMode(joy_x, INPUT);
  pinMode(joy_y, INPUT);
  
  for(int i = 0; i < 4; i++)
  {
    pinMode(ledsY[i], OUTPUT);
    pinMode(ledsX[i], OUTPUT);
  }

  randomSeed(analogRead(0));

  inicializaJogo(&s, &f);
  criaComida(&f, &s);
}

void loop() {

  
  while (!colidiu) {

    // Delay "velocidade" de Atualização do Jogo
    delay(350);

    // Muda a Direção da Cobra
    if     (analogRead(joy_y) <= 341 && s.dir != 3)  // Cima ??esquerda
      s.dir = 1;
    else if(analogRead(joy_x) >= 682 && s.dir != 4)  // Direita ??baixo
      s.dir = 2;
    else if(analogRead(joy_y) >= 682 && s.dir != 1)  // Baixo ??direita
      s.dir = 3;
    else if(analogRead(joy_x) <= 341 && s.dir != 2)  // Esquerda ??cima
      s.dir = 4;
      
    // Atualiza Posição da Cobra
    movimentaCobra(&s, &ultimo_x, &ultimo_y);

    // Se comeu na iteração passada adiciona um novo bloco na cauda da cobra exatamente no lugar da corpo na ultima iteração
    if (comeu)
    {
      atual = &s;
      while (atual->prox != NULL)   // Vamos até o ultimo bloco
        atual = atual->prox;
      atual->prox = (Snake*)malloc(sizeof(Snake));  // O proximo é um novo bloco
      atual->prox->x = ultimo_x;    // O lugar dele é na posição do ultimo bloco que saiu de lá
      atual->prox->y = ultimo_y;
      atual->prox->prox = NULL;
      pontuacao += 1;   // Atualiza pontuação
      comeu = 0;      // Vai ter que comer de novo
      criaComida(&f, &s); // Cria nova comida
    }

    // Verifica se a cobra comeu
    if (s.x == f.x && s.y == f.y)
      comeu = 1;

    lcd.setCursor(0, 0);
    lcd.print("Pontuacao: ");
    lcd.print(pontuacao);
    imprimeTabuleiro(&s, &f); // Ta invertido o X com Y essa porra!!!
    
    acendeLeds(&s, &f, ledsX, 4, ledsY, 4);

    // Verifica se a cobra bateu no prórpio corpo
    atual = s.prox;
    while (atual != NULL)
    {
      if (atual->x == s.x && atual->y == s.y) { // Se bateu não vai mais repetir o loop do jogo
        colidiu = 1;
      }
      atual = atual->prox;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fim de Jogo");
  delay(5000);
  
}

