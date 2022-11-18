////////VARIABLES DE FUSIBLE Y ALIMENTACION////////
float Vi;
float fus;

char estado = 'A';
float temp;
////////PINE DE LEDS////////
#define ErrFA   3  //Led de error en Alimentacios
#define ErrFB   4  //Led de error en Fusible
#define ErrP    5  //Led de error en Potencia
#define ErrI    6  //Led de error en Inversor
#define ErrCI   7  //Led de error en Circuito integrado
#define ErrGrl  8  //Led de error en General
////////VARIABLES DE POTENCIA////////
long temp2;
float P1;
float P2;

void setup() {
  Serial.begin(9600);
  pinMode(ErrFA, OUTPUT); //LED marca error
  pinMode(ErrFB, OUTPUT); //LED marca error
  pinMode(ErrP, OUTPUT); //LED marca error
  pinMode(ErrT, OUTPUT); //LED marca error
  pinMode(ErrCI, OUTPUT); //LED marca error
  pinMode(ErrGrl, OUTPUT); //LED marca error
  pinMode(9, OUTPUT); // buzzer
}

void loop() {
  switch (estado) {
    /////////////////////////////////////////////////////////////////////////
    case 'A':
      noTone(9);
      digitalWrite(ErrFA, LOW);
      digitalWrite(ErrFB, LOW);
      digitalWrite(ErrP, LOW);
      digitalWrite(ErrT, LOW);
      digitalWrite(ErrCI, LOW);
      digitalWrite(ErrGrl, LOW);
      temp = millis();
      lectura();
      if ((Vi < 984) || (Vi > 1023)) {
        estado = 'B';
      }
      if ((fus < 984) || (fus > 1023)) {
        estado = 'C';
      }
      if(){
        estado ='D';
      }
      if (Error == 1) {
        estado = 'E';
      }
      if(Inv < 1000 || Inv > 1023){
        estado = 'F';
      }
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'B':
      lectura();
      noTone(9);
      digitalWrite(ErrFA, HIGH);
      digitalWrite(ErrGrl, HIGH);
      if (millis() > temp + 500) {
        temp = millis();
        tone(9, 1800);
      }
      if ((Vi > 984) && (Vi < 1023)) {
        estado = 'A';
      }
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'C':
      lectura();
      noTone(9);
      digitalWrite(ErrFB, HIGH);
      digitalWrite(ErrGrl, HIGH);
      if (millis() > temp + 500) {
        temp = millis();
        tone(9, 1800);
      }
      if ((fus > 984) && (fus < 1023)) {
        estado = 'A';
      }
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'D':
            //No llegamos a integrar un Sistema PWM por falta de tiempo,no disculpamos
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'E':
            lectura();
            noTone(9);
            digitalWrite(ErrGrl,HIGH);
            digitalWrite(ErrP,HIGH);
            if (millis() > temp + 500) {
              temp = millis();
              tone(9, 1800);
            }
            if(Error == 0){
              estado = 'A';
            }
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'F':
            lectura();
            noTone(9);
            digitalWrite(ErrGrl,HIGH);
            digitalWrite(ErrI,HIGH);
            if (millis() > temp + 500) {
              temp = millis();
              tone(9, 1800);
            }
            if(Error == 0){
              estado = 'A';
            }
      break;
      /////////////////////////////////////////////////////////////////////////
  }
}


void lectura() {
  Vi = analogRead(A0);
  fus = analogRead(A1);
  P1 = analogRead(A2);
  P2 = analogRead(A3);
  Inv = analogRead(A4);
  Serial.print(P1);
  Serial.print(" ");
  Serial.println(P2);

  if (P1 >= 750 && P1 = < 1023 ) {
    if (P2 >= 750 && P2 = < 1023) {
      Error = 1;
    } else {
      Error = 0;
    }
  }
  if (P1 >= 0 && P1 = < 200) {
    if (P2 >= 0 && P2 = < 200) {
      Error = 1;
    } else {
      Error = 0;
    }
  }

  if (P1 > P1_Ant || P2 > P2_Ant) {
    MAX1 = P1;
    MAX2 = P2;
  } else {
    if (MAX1 < 900 || MAX2 < 900) {
      Error = 1;
    } else {
      Error = 0;
    }
  }
  float P1_Ant = P1;
  float P2_Ant = P2;
}
