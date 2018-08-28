#include <robo_hardware2.h>

float valorCorte[4] = {0, 0, 0, 0};
float valorPreto[4] = {0, 0, 0, 0};
float valorBranco[4] = {0, 0, 0, 0};

boolean btnCalibrarPreto;
boolean btnCalibrarBranco;
boolean btnSeguirLinha;
boolean seguirLinha = false;

void setup() {
  Serial.begin(9600);
  robo.configurar(false);
}

void loop() { 
  btnCalibrarPreto = robo.botao1Pressionado();
  btnCalibrarBranco = robo.botao2Pressionado();
  btnSeguirLinha = robo.botao3Pressionado();
  
  if (btnCalibrarPreto == true) {
    calibrarPreto();     
  } else if (btnCalibrarBranco == true) {
    calibrarBranco();
  } else if (btnSeguirLinha == true) {    
    if (seguirLinha) {
      seguirLinha = false;
      robo.desligarLed(3);
    } else {
      seguirLinha = true;
      robo.ligarLed(3);
    }
  }

  if (seguirLinha) {
    initSeguirLinha();
  } else {
    robo.acionarMotores(0,0);
  }
}

void calibrarPreto() {
  float valorSensores[4] = {0, 0, 0, 0};
  
  Serial.println("Calibrando preto");
  Serial.print("[");
  for (int i=0; i<40; i++){
    Serial.print(" ");
  }
  Serial.println("]");
  Serial.print(" ");
  
  for (int i=0; i<4; i++) {
    boolean led = false;
    for (int j=0; j<10; j++) {
      valorSensores[0] = (analogRead(1)/1023.0)*100.0;
      valorSensores[1] = (analogRead(2)/1023.0)*100.0;
      valorSensores[2] = (analogRead(3)/1023.0)*100.0;
      valorSensores[3] = (analogRead(0)/1023.0)*100.0;  
      valorPreto[i] += valorSensores[i];
      Serial.print("|");
      if (led) {
        led = false;
        robo.desligarLed(1);
      } else {
        led = true;
        robo.ligarLed(1);
      }
      delay(150);
    }
  }

  for (int i=0; i<4; i++) {
    valorPreto[i] /= 10;
  }
  
  Serial.println();
  Serial.println("Preto calibrado");
}

void calibrarBranco() {
  float valorSensores[4] = {0, 0, 0, 0};
  
  Serial.println("Calibrando branco");
  Serial.print("[");
  for (int i=0; i<40; i++){
    Serial.print(" ");
  }
  Serial.println("]");
  Serial.print(" ");
  
  for (int i=0; i<4; i++) {
    boolean led = false;
    for (int j=0; j<10; j++) {
      valorSensores[0] = (analogRead(1)/1023.0)*100.0;
      valorSensores[1] = (analogRead(2)/1023.0)*100.0;
      valorSensores[2] = (analogRead(3)/1023.0)*100.0;
      valorSensores[3] = (analogRead(0)/1023.0)*100.0;  
      valorBranco[i] += valorSensores[i];
      Serial.print("|");
      delay(150);
      if (led) {
        led = false;
        robo.desligarLed(2);
      } else {
        led = true;
        robo.ligarLed(2);
      }
    }
  }

  for (int i=0; i<4; i++) {
    valorBranco[i] /= 10;
  }
  
  Serial.println();
  Serial.println("Branco calibrado");
}

void initSeguirLinha() {
  for (int i=0; i<4; i++) {
    valorCorte[i] = (valorBranco[i] + valorPreto[i])/2;
    Serial.print("VALOR DE CORTE DO SENSOR ");
    Serial.print(i);
    Serial.print(":");
    Serial.println(valorCorte[i]);
  }
  
  float sensores[4] = {0, 0, 0, 0};
  sensores[0] = (analogRead(1)/1023.0)*100.0;
  sensores[1] = (analogRead(2)/1023.0)*100.0;
  sensores[2] = (analogRead(3)/1023.0)*100.0;
  sensores[3] = (analogRead(0)/1023.0)*100.0;

  if (sensores[3] > valorCorte[3] || sensores[0] > valorCorte[0]) { // +DIR = preto ou +ESQ = preto
    if (sensores[2] > valorCorte[2] && sensores[1] > valorCorte[1]) { // ESQ = preto e DIR = preto
      robo.acionarMotores(15,15);
    } else if (sensores[2] > valorCorte[2]) { // +DIR = preto e DIR = preto      
      robo.acionarMotores(-25,15); // Curva à direita
      delay(200);
    } else if (sensores[1] > valorCorte[1]) { // +ESQ = preto e DIR = preto      
      robo.acionarMotores(15,-25); // Curva à esquerda      
      delay(200);
    }
  } else if (sensores[2] < valorCorte[2] && sensores[1] > valorCorte[1]){ // DIR = branco e ESQ = preto
      robo.acionarMotores(15,-25); // Curva à esquerda
  } else if (sensores[2] > valorCorte[2] && sensores[1] < valorCorte[1]){ // DIR = preto e ESQ = branco
    robo.acionarMotores(-25,15); // Curva à direita
  } else { // TUDO BRANCO
    robo.acionarMotores(15,15); // GO!
  }
}

