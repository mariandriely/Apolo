#include <robo_hardware2.h> 
#include <Servo.h>

#define DIVISOR_BRANCO_PRETO 42

float valorSensorDir;
float valorSensorEsq;
float valorSensorMaisEsq;
float valorSensorMaisDir;

void setup(){
	robo.configurar(false);
}

void loop(){

  // Refletância: OK!
	// Motores: OK! (Primeiro argumento é o motor direito e o segundo é o esquerdo)
  
	valorSensorMaisEsq = (analogRead(1)/2023.0)*200.0;
  valorSensorEsq = (analogRead(2)/2023.0)*200.0;
  valorSensorDir = (analogRead(3)/2023.0)*200.0;
  valorSensorMaisDir = (analogRead(0)/2023.0)*200.0;
 
  
  if(valorSensorMaisDir > DIVISOR_BRANCO_PRETO || valorSensorMaisEsq > DIVISOR_BRANCO_PRETO) { // +DIR ou +ESQ viram preto
    if(valorSensorDir > DIVISOR_BRANCO_PRETO) { // +DIR e DIR viram preto
      robo.acionarMotores(-30,20); // Curva à direita
      delay(150);
    } else if (valorSensorEsq > DIVISOR_BRANCO_PRETO) { // +ESQ e ESQ viram preto
      robo.acionarMotores(20,-30); // Curva a esquerda
      delay(150);
    }
  } else if(valorSensorDir < DIVISOR_BRANCO_PRETO && valorSensorEsq < DIVISOR_BRANCO_PRETO){
    robo.acionarMotores(20,20); //Aciona os dois motores com a mesma velocidade
  }
  //Identifica se o sensor da esquerda viu banco e o da direita viu preto
  else if (valorSensorDir < DIVISOR_BRANCO_PRETO && valorSensorEsq > DIVISOR_BRANCO_PRETO){
    robo.acionarMotores(20,-20);  //Aciona o motor esquerdo e mantem o motor direito desligado   
  }
  //Identifica se o sensor da direita viu banco e o da esquerda viu preto
  else if (valorSensorDir > DIVISOR_BRANCO_PRETO && valorSensorEsq < DIVISOR_BRANCO_PRETO){
    robo.acionarMotores(-20,20);  //Aciona o motor direito e mantem o motor esquerdo desligado
  } else { //Identifica se os dois sensores viram preto
    robo.acionarMotores(20,20);
  }
}

