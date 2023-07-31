#define ENCODER_PIN 25

int rpm;
volatile byte pulsos;
unsigned long timeold;

unsigned int pulsos_por_volta = 16;

void contador() {
  pulsos++;
}

void setup() {
  Serial.begin(115200);
  //Pino do sensor como entrada
  pinMode(ENCODER_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), contador, FALLING);
  pulsos = 0;
  rpm = 0;
  timeold = 0;
}

void loop() {
  //Atualiza contador a cada intervalo de tempo
  if (millis() - timeold >= 500) {
    //Desabilita interrupcao durante o calculo
    detachInterrupt(0);
    rpm = (60 * 1000 / pulsos_por_volta ) / (millis() - timeold) * pulsos;
    timeold = millis();
    pulsos = 0;

    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    
    //Habilita interrupcao
    attachInterrupt(0, contador, FALLING);
  }
}
