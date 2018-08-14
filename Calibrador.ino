float valorSensorMaisEsq;
float valorSensorEsq;
float valorSensorDir;
float valorSensorMaisDir;

void setup() {
  Serial.begin(9600);
}

void loop() {

  float c1;
  float c2;
  float c3;
  float c4;

  for (int i=0; i<10; i++) {
    valorSensorMaisEsq = (analogRead(1)/1023.0)*100.0;
    valorSensorEsq = (analogRead(2)/1023.0)*100.0;
    valorSensorDir = (analogRead(3)/1023.0)*100.0;
    valorSensorMaisDir = (analogRead(0)/1023.0)*100.0;
    c1 += valorSensorMaisEsq;
    c2 += valorSensorEsq;
    c3 += valorSensorDir;
    c4 += valorSensorMaisDir;
    delay(100);
  }

  c1 /= 10;
  c2 /= 10;
  c3 /= 10;
  c4 /= 10;

  Serial.print("Sensores refletancia: +esq; esq; dir; +dir ");
  Serial.print(c1); //Imprime o valor do sensor pela porta serial
  Serial.print("; ");
  Serial.print(c2); //Imprime o valor do sensor pela porta serial
  Serial.print("; ");
  Serial.print(c3); //Imprime o valor do sensor pela porta serial
  Serial.print("; ");
  Serial.println(c4); //Imprime o valor do sensor pela porta serial
  
}
