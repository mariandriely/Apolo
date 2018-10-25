#include <Arduino.h>
#include <robo_hardware2.h>
#include <main.h>

float delayOne = 0;
float delayTwo = 500;

int pinMaisEsq = 0;
int pinEsq = 1;
int pinDir = 2;
int pinMaisDir = 3;

calibracao_dados val;
bool desafio = false;

void setup() {
    Serial.begin(9600);
    robo.configurar(false);    
}

void loop() { 
    btnSubirRampa = robo.botao1Pressionado();
    btnCalibrarBranco = robo.botao2Pressionado();
    btnSeguirLinha = robo.botao3Pressionado();

    if (btnSubirRampa == true) {
        robo.lerCalibracao(val);
        valorCorte[0] = val.refletanciaMaisEsq;
        valorCorte[1] = val.refletanciaEsq;
        valorCorte[2] = val.refletanciaDir;
        valorCorte[3] = val.refletanciaMaisDir;    
        for (int i = 0; i < 4; i++) {
            Serial.print("VALOR DE CORTE ");
            Serial.print(i);
            Serial.print(" : ");
            Serial.println(valorCorte[i]);
        }
        if (rampa) {
            rampa = false;      
            robo.desligarLed(1);
        } else {
            rampa = true;
            robo.ligarLed(1);
        }    
    } else if (btnCalibrarBranco == true) {    
        calibrarBranco();
        delay(5000);
        calibrarPreto();
        for (int i=0; i<4; i++) {
            valorCorte[i] = (valorBranco[i] + valorPreto[i])/2;      
        }
        val.refletanciaMaisEsq = valorCorte[0];
        val.refletanciaEsq = valorCorte[1];
        val.refletanciaDir = valorCorte[2];
        val.refletanciaMaisDir = valorCorte[3];
        robo.salvarCalibracao(val);
    } else if (btnSeguirLinha == true) {        
        robo.lerCalibracao(val);
        valorCorte[0] = val.refletanciaMaisEsq;
        valorCorte[1] = val.refletanciaEsq;
        valorCorte[2] = val.refletanciaDir;
        valorCorte[3] = val.refletanciaMaisDir;    
        for (int i = 0; i < 4; i++) {
            Serial.print("VALOR DE CORTE ");
            Serial.print(i);
            Serial.print(" : ");
            Serial.println(valorCorte[i]);
        }
        if (seguirLinha) {
            seguirLinha = false;
            robo.desligarLed(3);
        } else {
            seguirLinha = true;
            robo.ligarLed(3);
        }
    }
    
    if (seguirLinha) {            
        if ((robo.lerSensorSonarEsq() < 10) && (robo.lerSensorSonarDir() < 10)) {
            initRampa();
        } else if (robo.lerSensorSonarFrontal() < 10) {
//            desviarObstaculo();
        } else {
            initSeguirLinha();
        }
    } else if (rampa) {
        initRampa();
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
        bool led = false;
        for (int j=0; j<10; j++) {
            valorSensores[0] = (analogRead(pinMaisEsq)/1023.0)*100.0;
            valorSensores[1] = (analogRead(pinEsq)/1023.0)*100.0;
            valorSensores[2] = (analogRead(pinDir)/1023.0)*100.0;
            valorSensores[3] = (analogRead(pinMaisDir)/1023.0)*100.0;  
            valorPreto[i] += valorSensores[i];
            Serial.print("|");
            if (led) {
                led = false;
                robo.desligarLed(2);
            } else {
                led = true;
                robo.ligarLed(2);
            }
            delay(150);
        }
    }

    for (int i=0; i<4; i++) {
        valorPreto[i] /= 10;
        Serial.print("PRETO ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(valorPreto[i]);
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
        bool led = false;
        for (int j=0; j<10; j++) {
            valorSensores[0] = (analogRead(pinMaisEsq)/1023.0)*100.0;
            valorSensores[1] = (analogRead(pinEsq)/1023.0)*100.0;
            valorSensores[2] = (analogRead(pinDir)/1023.0)*100.0;
            valorSensores[3] = (analogRead(pinMaisDir)/1023.0)*100.0;  
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
        Serial.print("BRANCO ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(valorBranco[i]);
    }
  
    Serial.println();
    Serial.println("Branco calibrado");
}

void initSeguirLinha() {      
    int delay90 = 0;
    int delayCurva = 0;
  
    float sensores[4] = {0, 0, 0, 0};
    sensores[0] = (analogRead(pinMaisEsq)/1023.0)*100.0;
    sensores[1] = (analogRead(pinEsq)/1023.0)*100.0;
    sensores[2] = (analogRead(pinDir)/1023.0)*100.0;
    sensores[3] = (analogRead(pinMaisDir)/1023.0)*100.0;
        
    if (sensores[0] > valorCorte[0] && sensores[1] > valorCorte[1] && sensores[2] < valorCorte[2] && sensores[3] < valorCorte[3]) { // PPBB        
        Serial.println("====CURVA À ESQUERDA====");
        delay(delayOne);
        robo.acionarMotores(-40,40);
        delay(delayTwo);
    } else if (sensores[0] < valorCorte[0] && sensores[1] < valorCorte[1] && sensores[2] > valorCorte[2] && sensores[3] > valorCorte[3]) { // BBPP        
        Serial.println("====CURVA À DIREITA====");
        delay(delayOne);
        robo.acionarMotores(40,-40);    
        delay(delayTwo);
    } else if (sensores[2] < valorCorte[2] && sensores[1] > valorCorte[1]){ // DIR = branco e ESQ = preto        
        Serial.println("====CURVA À ESQUERDA====");
        robo.acionarMotores(40,-40); // Curva à esquerda
        delay(delayCurva);
    } else if (sensores[2] > valorCorte[2] && sensores[1] < valorCorte[1]){ // DIR = preto e ESQ = branco            
        Serial.println("====CURVA À DIREITA====");
        robo.acionarMotores(-40,40); // Curva à direita
    } else if (sensores[3] > valorCorte[3] && sensores[0] < valorCorte[0]) { // +DIR = preto ou +ESQ = branco        
        Serial.println("====CURVA À ESQUERDA====");
        robo.acionarMotores(-40,40);        
    } else if (sensores[3] < valorCorte[3] && sensores[0] > valorCorte[0]) { // +DIR = branco e +ESQ = preto                
        Serial.println("====CURVA À DIREITA====");
        robo.acionarMotores(40,-40);        
        delay(delayCurva);
    } else if(sensores[0] > valorCorte[0] && sensores[3] > valorCorte[3]) {        
        robo.acionarMotores(30,40);
        delay(delayCurva);
    } else if (sensores[1] < valorCorte[0] && sensores[2] < valorCorte[2]) {
        robo.acionarMotores(30,40);
    }

    Serial.print("Sensores refletancia: +esq; esq; dir; +dir ");
	Serial.print(sensores[0]); //Imprime o valor do sensor pela porta serial
	Serial.print("; ");
	Serial.print(sensores[1]); //Imprime o valor do sensor pela porta serial
	Serial.print("; ");
	Serial.print(sensores[2]); //Imprime o valor do sensor pela porta serial
	Serial.print("; ");
	Serial.println(sensores[3]); //Imprime o valor do sensor pela porta serial
}

void initRampa() {        
    int delay90 = 0;
    int delayCurva = 0;
  
    float sensores[4] = {0, 0, 0, 0};
    sensores[0] = (analogRead(0)/1023.0)*100.0;
    sensores[1] = (analogRead(1)/1023.0)*100.0;
    sensores[2] = (analogRead(2)/1023.0)*100.0;
    sensores[3] = (analogRead(3)/1023.0)*100.0;
    
    if (sensores[2] < valorCorte[2] && sensores[1] > valorCorte[1]){ // DIR = branco e ESQ = preto
        robo.acionarMotores(35,0); // Curva à esquerda
        delay(delayCurva);
    } else if (sensores[2] > valorCorte[2] && sensores[1] < valorCorte[1]){ // DIR = preto e ESQ = branco    
        robo.acionarMotores(0,50); // Curva à direita
        delay(delayCurva);
    } else if (sensores[3] > valorCorte[3] && sensores[0] < valorCorte[0]) { // +DIR = preto ou +ESQ = branco
        robo.acionarMotores(0,50);
        delay(delay90);      
    } else if (sensores[3] < valorCorte[3] && sensores[0] > valorCorte[0]) { // +DIR = branco e +ESQ = preto
        robo.acionarMotores(35,0);
        delay(delay90);    
    } else {
        robo.acionarMotores(50,75);
    }
}

void desviarObstaculo() {
    robo.acionarMotores(-10,-10);
    delay(1000); 
    robo.acionarMotores(-30,40); // Gira para a direita
    delay(350);
    robo.acionarMotores(0,0);
    delay(1000);
    robo.acionarMotores(30,40); // Vai para frente
    delay(600); 
    robo.acionarMotores(0,0);
    delay(1000);
    robo.acionarMotores(30,-40); // Gira a esquerda
    delay(300);    
    robo.acionarMotores(0,0);
    delay(1000);
    robo.acionarMotores(30,40); // Vai pra frente
    delay(900);
    robo.acionarMotores(0,0);
    delay(1000);
    robo.acionarMotores(30,-40); // Gira para esquerda
    delay(400);
    robo.acionarMotores(0,0);    
}

 void desafioStart() {
    robo.ligarLed(1); // Forte - 1
    delay(3000);
    for (int i = 0; i < 5; i++) {
        robo.desligarLed(1); // Desliga
        delay(150);
        robo.ligarLed(1); // Fraco - 0
        delay(150);
    }        
    robo.ligarLed(1); // Forte - 1
    delay(3000);
    for (int i = 0; i < 5; i++) {
        robo.desligarLed(1); // Desliga
        delay(150);
        robo.ligarLed(1); // Fraco - 0
        delay(150);
    }        
    
  robo.desligarLed(1);
}
