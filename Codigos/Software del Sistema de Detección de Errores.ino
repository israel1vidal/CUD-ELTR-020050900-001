////////VARIABLES DE FUSIBLE Y ALIMENTACION////////
float Vi;
float fus;

char estado = 'A';
float temp;
int LecB = 0;
int Error = 0;
float Inv;
float MAX1;
float MAX2;

////////PINE DE LEDS////////
#define ErrFA   3  //Led de error en Alimentacios
#define ErrFB   4  //Led de error en Fusible
#define ErrP    5  //Led de error en Potencia
#define ErrI    6  //Led de error en Inversor
#define ErrCI   7  //Led de error en Circuito integrado
#define ErrGrl  8  //Led de error en General
#define BB_Stop  10 //Boton para detener el sondo de alarma (buzzer)

////////LECTURA DE ERRORES////////

#define PinVi A0
#define PinFus A1
#define PinPo1 A2
#define PinPo2 A3
#define PinInv A4
#define PinCi1 12
#define PinCi2 13

////////VARIABLES DE POTENCIA////////
long temp2;
float P1;
float P2;

////////VARIABLES DE POTENCIA////////
int pwm_valor1;
int pwm_valor2;


void setup() {
  Serial.begin(9600);
  pinMode(PinCi1, INPUT);
  pinMode(PinCi2, INPUT);
  pinMode(ErrFA, OUTPUT); //LED marca error
  pinMode(ErrFB, OUTPUT); //LED marca error
  pinMode(ErrP, OUTPUT); //LED marca error 
  pinMode(ErrI, OUTPUT); //LED marca error
  pinMode(ErrCI, OUTPUT); //LED marca error
  pinMode(ErrGrl, OUTPUT); //LED marca error
  pinMode(BB_Stop, INPUT); //Boton de Stop Buzzer
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
      digitalWrite(ErrI, LOW);
      digitalWrite(ErrCI, LOW);
      digitalWrite(ErrGrl, LOW);
      temp = millis();
      lectura();
      LecB = 0;
      if ((Vi < 984) || (Vi > 1023)) {
        estado = 'B';
      }
      if ((fus < 984) || (fus > 1023)) {
        estado = 'C';
      }
      if (pwm_valor1 < 3 && pwm_valor2 < 3 || pwm_valor1 > 3 && pwm_valor2 > 3) {
        
        estado = 'D';
      }
      if (Error == 1) {
        estado = 'E';
      }
      if (Inv < 1000 || Inv > 1023) {
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
        if (LecB == 0) {
          tone(9, 1800);
        } else {
          noTone(9);
        }
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
        if (LecB == 0) {
          tone(9, 1800);
        } else {
          noTone(9);
        }
      }
      if ((fus > 984) && (fus < 1023)) {
        estado = 'A';
      }
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'D':
      lectura();
      noTone(9);
      digitalWrite(ErrCI, HIGH);
      digitalWrite(ErrI, HIGH);
      if (millis() > temp + 500) {
        temp = millis();
        if (LecB == 0) {
          tone(9, 1800);
        } else {
          noTone(9);
        }
      }
      if (pwm_valor1 < 3 && pwm_valor2 < 3 || pwm_valor1 > 3 && pwm_valor2 > 3) {
        estado = 'D';
      }else{
        estado = 'A';
      }

      break;
    /////////////////////////////////////////////////////////////////////////
    case 'E':
      lectura();
      noTone(9);
      digitalWrite(ErrGrl, HIGH);
      digitalWrite(ErrP, HIGH);
      if (millis() > temp + 500) {
        temp = millis();
        if (LecB == 0) {
          tone(9, 1800);
        } else {
          noTone(9);
        }
      }
      if (Error == 0) {
        estado = 'A';
      }
      break;
    /////////////////////////////////////////////////////////////////////////
    case 'F':
      lectura();
      noTone(9);
      digitalWrite(ErrGrl, HIGH);
      digitalWrite(ErrI, HIGH);
      if (millis() > temp + 500) {
        temp = millis();
        if (LecB == 0) {
          tone(9, 1800);
        } else {
          noTone(9);
        }
      }
      if (Inv > 1000 && Inv < 1024) {
        estado = 'A';
      }
      break;
      /////////////////////////////////////////////////////////////////////////
  }
}


void lectura() {
  float P1_Ant = P1;
  float P2_Ant = P2;
  if (digitalRead(BB_Stop == HIGH))LecB = 1;
  pwm_valor1 = pulseIn(PinCi1, HIGH);
  pwm_valor2 = pulseIn(PinCi2, HIGH);
  Vi = analogRead(PinVi);
  fus = analogRead(PinFus);
  P1 = analogRead(PinPo1);
  P2 = analogRead(PinPo2);
  Inv = analogRead(PinInv);
  Serial.print(P1);
  Serial.print(" ");
  Serial.println(P2);

  if (P1 >= 750 && P1 <= 1023 ) {
    if (P2 >= 750 && P2 <= 1023) {
      Error = 1;
    } else {
      Error = 0;
    }
  }
  if (P1 >= 0 && P1 <= 200) {
    if (P2 >= 0 && P2 <= 200) {
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
P1_Ant = P1;
P2_Ant = P2;
}
