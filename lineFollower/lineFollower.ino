/**********************************************************
* UNIVERSIDADE FEDERAL DE PERNAMBUCO - UFPE
* CENTRO DE INFORMARICA - CIN
* INTRODUCAO A COMPUTACAO - IF668 EC
* CODIGO PARA LINEFOLLOWER
* TRABALHO DE CONCLUSÃO DA DISCIPLINA
* GRUPO 8
* INTEGRANTES:  MATHEUS BATISTA DE ARAÚJO (mba6); CIBELLE SOUSA RODRIGUES (csr2); EROS SANTOS VASCONCELOS (esv); PEDRO JORGE LIMA DA SILVA(pjls2); 
* LEONARDO GABRIEL MOREIRA DE OLIVEIRA (lgmo)
*
************************************************* *******/

/* ------------------- SENSOR ULTRASSÔNICO ------------------- */
#include "Ultrasonic.h"         
#define ECHO_PIN 10             
#define TRIG_PIN 9              
int distancia=0;                  
Ultrasonic ultrasonic(9, 10);   
/* ------------------------------------------------------------ */


/* --------------------- PONTE H MOTORES ---------------------- */
#define MOTOR_E1 8
#define MOTOR_E2 7
#define MOTOR_EV 6
#define MOTOR_DV 5
#define MOTOR_D2 4
#define MOTOR_D1 3
#define VELOCIDADE_MIN 40         
#define VELOCIDADE_MAX 70         
#define VELOCIDADE_MIN_OBST 40    
#define VELOCIDADE_MAX_OBST 70    
/* ------------------------------------------------------------ */


/* ----------------------- SENSOR OPTICO ---------------------- */
#define IN_PIN 2                // Pino de entrada
#define COR_LINHA HIGH          // Define para cor da linha (BRANCO -> HIGH; PRETO -> LOW)
/* ------------------------------------------------------------ */


/* ---------------------------- LEDS -------------------------- */
#define LED_DIREITA 11    
#define LED_ESQUERDA 12   
#define LED_OBSTACULO 13  
/* ------------------------------------------------------------ */

//DECLARAÇÕES DAS VARIÁVEIS

int flagObstaculo = 1;    // Variavel que indica se o obstaculo ja desviado

//DECLARAÇÕES DAS FUNÇÕES
int lerDistancia();       // Lê distancia entre carrinho e o obstaculo
void checkObstaculo();    // Usada para checar se existe obstaculo na frente

void para();              // Para os motores
void andaEsquerda();      // Move o carrinho para esquerda ate que encontre a linha 
void andaDireita();       // Move o carrinho para direita ate que encontre a linha 

void desviaFrente(int esquerdaVel, int direitaVel);
void desviaEsquerda(int esquerdaVel, int direitaVel);
void desviaDireita(int esquerdaVel, int direitaVel);
void desvia();            // Move o carrinho para desviar o obstaculo
/* ------------------------------------------------------------ */


void setup() {
  // Define os pinos como saida do motor
  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_EV, OUTPUT);
  pinMode(MOTOR_DV, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);

  // Sensor óptico
  pinMode(IN_PIN, INPUT);

  // Sensor Ultrassonico
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);

  // Leds
  pinMode(LED_DIREITA, OUTPUT);
  pinMode(LED_ESQUERDA, OUTPUT);
  pinMode(LED_OBSTACULO, OUTPUT);
}

void loop() {
  
  andaEsquerda();                             // Move carrinho para esquerda ate encontrar linha
  checkObstaculo();                           // Checa se tem obstaculo
  andaDireita();                              // Move o carrinho para o lado direito até que encontre a linha preta
  checkObstaculo();
}


/* ------------------------------- FUNÇÕES ----------------------------- */

// Função que ler a distancia do carrinho ao obstaculo
int lerDistancia(){
  digitalWrite(TRIG_PIN, LOW);    
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);             // Emite um pulso sonoro (indica o incio da transmissao de dados)
  delayMicroseconds(10);                
  digitalWrite(TRIG_PIN, LOW);              // Para a emissão
  distancia = (ultrasonic.Ranging(CM));     // A variavel distancia recebe o valor lido pelo sensor (distancia = (tempo echo em nivel alto*velocidade do som)/2)
  return distancia;                         // Retorna a distancia lida em centímetros
}

void checkObstaculo(){
  if (flagObstaculo) {                        // Verifica se o obstaculo ja foi desviado    
    distancia = lerDistancia();               // Chama a função 'lerDistancia' que verifica a distancia entre o carrinho e o obstaculo
  }
  if (distancia <= 19 && flagObstaculo) {     // Verifica se a distancia lida é menor ou igual a 19cm e se o obstaculo ja foi desviado
    desvia();                                 // Chama a função para desviar o obstaculo
  }
}

// Função que trava a movimentação dos motores
void para() {
  analogWrite(MOTOR_EV, 0);
  analogWrite(MOTOR_DV, 0);
  digitalWrite(MOTOR_E1, HIGH);
  digitalWrite(MOTOR_E2, HIGH);
  digitalWrite(MOTOR_D1, HIGH);
  digitalWrite(MOTOR_D2, HIGH);
}

//Função que move o carrinho para o lado esquerdo
void andaEsquerda() {
  // Troca os leds para indicar o movimento, pode ser usado para debug
  digitalWrite(LED_DIREITA, LOW);     
  digitalWrite(LED_ESQUERDA, HIGH);   
  
  // Loop enquanto o sensor optico estiver lendo diferente de COR_LINHA
  
  while (digitalRead(IN_PIN) != COR_LINHA){
    // Movimentação dos motores, motor direito no sentido horario na velocidade maxima e o motor esquerdo parado
    // analogWrite(MOTOR_EV, VELOCIDADE_MIN);
    analogWrite(MOTOR_EV, 0);
    analogWrite(MOTOR_DV, VELOCIDADE_MAX);
    digitalWrite(MOTOR_E1, HIGH);
    digitalWrite(MOTOR_E2, LOW);
    digitalWrite(MOTOR_D1, LOW);
    digitalWrite(MOTOR_D2, HIGH);
  };

  para();
  

}

//Função que move o carrinho para o lado direito
void andaDireita() {

  // Troca os leds para indicar o movimento, pode ser usado para debug
  digitalWrite(LED_ESQUERDA, LOW);    
  digitalWrite(LED_DIREITA, HIGH);    
  
  // Loop enquanto o sensor optico estiver lendo diferente de COR_LINHA
  while (digitalRead(IN_PIN) != COR_LINHA){
    // Movimentação dos motores Motor direito parado e o motor esquerdo no sentido horario na velocidade maxima
    analogWrite(MOTOR_EV, VELOCIDADE_MAX);
    analogWrite(MOTOR_DV, 0);
    // analogWrite(MOTOR_DV, VELOCIDADE_MIN);
    digitalWrite(MOTOR_E1, LOW);
    digitalWrite(MOTOR_E2, HIGH);
    digitalWrite(MOTOR_D1, HIGH);
    digitalWrite(MOTOR_D2, LOW);
  } ;
  para();

}

// Funçao executada para o robô andar pra frente durante a rotina de desvio, parametrizada para facilitar o uso
void desviaFrente(int esquerdaVel, int direitaVel){
    analogWrite(MOTOR_EV, esquerdaVel);
    analogWrite(MOTOR_DV, direitaVel);
    digitalWrite(MOTOR_E1, LOW);
    digitalWrite(MOTOR_E2, HIGH);
    digitalWrite(MOTOR_D1, LOW);
    digitalWrite(MOTOR_D2, HIGH);
}


// Funçao executada para o robô girar para a esquerda durante a rotina de desvio, parametrizada para facilitar o uso
void desviaEsquerda(int esquerdaVel, int direitaVel){
    analogWrite(MOTOR_EV, esquerdaVel);
    analogWrite(MOTOR_DV, direitaVel);
    digitalWrite(MOTOR_E1, LOW);
    digitalWrite(MOTOR_E2, HIGH);
    digitalWrite(MOTOR_D1, HIGH);
    digitalWrite(MOTOR_D2, LOW);
}


// Funçao executada para o robô girar para a direita durante a rotina de desvio, parametrizada para facilitar o uso
void desviaDireita(int esquerdaVel, int direitaVel){
    analogWrite(MOTOR_EV, esquerdaVel);
    analogWrite(MOTOR_DV, direitaVel);
    digitalWrite(MOTOR_E1, HIGH);
    digitalWrite(MOTOR_E2, LOW);
    digitalWrite(MOTOR_D1, LOW);
    digitalWrite(MOTOR_D2, HIGH);
}



void desvia() {
  unsigned long tempo = millis();     // Variavel que recebe o tempo de execução da função em millisegundos

  // Giro para Direita enquanto o tempo for menor que 200ms
  while (millis() - tempo < 200){
    digitalWrite(LED_OBSTACULO, HIGH);
    desviaDireita(VELOCIDADE_MAX_OBST - 20, VELOCIDADE_MAX_OBST - 20);
  } ;


  // Para o carrinho durante 300ms
  tempo = millis();
  while (millis() - tempo < 300){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Movimento para frente durente 200ms
  tempo = millis();
  while (millis() - tempo < 200){
    digitalWrite(LED_OBSTACULO, HIGH);
    desviaFrente(VELOCIDADE_MAX_OBST, VELOCIDADE_MAX_OBST + 5);
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 150){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Giro para Esquerda durante 200ms
  tempo = millis();
  while (millis() - tempo < 200){
    digitalWrite(LED_OBSTACULO, HIGH);
    desviaEsquerda(VELOCIDADE_MAX_OBST, VELOCIDADE_MAX_OBST);
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 150){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Movimento para frente durante 700ms
  tempo = millis();
  while (millis() - tempo < 700){
    desviaFrente(VELOCIDADE_MAX_OBST, VELOCIDADE_MAX_OBST);
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 150){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Giro para Esquerda durante 150ms
  tempo = millis();
  while (millis() - tempo < 150){
    desviaEsquerda(VELOCIDADE_MAX_OBST, VELOCIDADE_MAX_OBST);
  };

  // Para o carrinho durante 150ms
  tempo = millis();
  while (millis() - tempo < 150){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  } ;

  // Movimenta para frente até o sensor optico encontre o linha preta
  while (digitalRead(IN_PIN) == HIGH){
    desviaFrente(60, 65);
  };

  // Para o carrinho durante 150ms
  tempo = millis();   //Reinicia o tempo
  while (millis() - tempo < 150){
    para();
    digitalWrite(LED_OBSTACULO, LOW);
  };

  // Giro para Direita ate que o sensor optico encontre a linha preta
  while (digitalRead(IN_PIN) == HIGH){
    desviaDireita(VELOCIDADE_MIN_OBST, VELOCIDADE_MAX_OBST);
  } ;
   
  para();                             // Para o carrinho 
  digitalWrite(LED_OBSTACULO, LOW);   //Desliga o led(indicando o fim do desvio)
  delay(50);                          // Espera 50ms
  flagObstaculo = 0;                  // Zera a variavel 'flagObstaculo', indicando que o carrinho já realizou o desvio de obstaculo e não permitindo que o faça novamente, evitando desvios acidentais durante o resto do percuso
}
/* ----------------------END OF CODE---------------------------- */
