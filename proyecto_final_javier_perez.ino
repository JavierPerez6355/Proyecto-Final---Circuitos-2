#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int servoPin = A3;
Servo myServo;

int ledR = A2;
int ledG = 11;
int ledB = A1;
int buz = 3;
int boton1 = 13;
int boton2 = 12;
int pot = A0;

int disp[7] = {10,9,8,7,6,5,4};
byte num[3][7] = {
  {1,1,1,1,1,1,0},
  {0,1,1,0,0,0,0},
  {1,1,0,1,1,0,1}
};

int prevEstado1 = LOW;
int prevEstado2 = LOW;
int contador = 0;
int alarma = 0;

void RGBoff(int ledR, int ledG, int ledB){
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
}
void RGBred(int ledR, int ledG, int ledB){
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, LOW);
}
void RGBgreen(int ledR, int ledG, int ledB){
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, LOW);
}
void RGBblue(int ledR, int ledG, int ledB){
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
}
void RGByellow(int ledR, int ledG, int ledB){
    digitalWrite(ledR, HIGH);
    analogWrite(ledG, 60);
    digitalWrite(ledB, LOW);
}
void RGBpurple(int ledR, int ledG, int ledB){
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
}
void RGBcyan(int ledR, int ledG, int ledB){
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
}
void RGBorange(int ledR, int ledG, int ledB){
    digitalWrite(ledR, HIGH);
    analogWrite(ledG, 10);
    digitalWrite(ledB, LOW);
}
void RGBwhite(int ledR, int ledG, int ledB){
    digitalWrite(ledR, HIGH);
    analogWrite(ledG, 60);
    digitalWrite(ledB, HIGH);
}

void setup() {
  myServo.attach(servoPin);
  
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(pot, INPUT);

  for(int i = 0; i < 7; i++){
    pinMode(disp[i], OUTPUT);
  }

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
}

void loop() {
  int estado1 = digitalRead(boton1);
  delay(15);

  if (prevEstado1 == HIGH && estado1 == LOW) {
    contador++;
    if(contador > 2){
      contador = 0;
    }
    alarma = 0;
    digitalWrite(buz, LOW);
    lcd.clear();
    Serial.println(contador);
  }
  prevEstado1 = estado1;

  for(int i = 0; i < 7; i++){
    digitalWrite(disp[i], num[contador][i]);
  }

  int lecPot;
  int potFrec;
  int potServoFrec;
  float lecPotF;
  float potTemp;

  switch(contador){

    case 0:
      myServo.write(0);
      digitalWrite(buz, LOW);
      RGBoff(ledR, ledG, ledB);
      break;

    case 1:
      lecPot = analogRead(pot);
      potFrec = map(lecPot, 0, 1023, 0, 200);
      Serial.println(potFrec);
      potServoFrec = map(potFrec, 0, 200, 0, 180);
      myServo.write(potServoFrec);

      lcd.setCursor(0, 0);
      lcd.print("Frec: ");
      lcd.print(potFrec);
      lcd.print(" lpm     ");

      if (potFrec < 60){
        RGByellow(ledR, ledG, ledB);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
      }
      else if (potFrec >= 60 && potFrec <= 100){
        RGBgreen(ledR, ledG, ledB);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :)     ");
      }
      else if (potFrec > 100 && potFrec < 150){
        RGBred(ledR, ledG, ledB);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        RGBred(ledR, ledG, ledB);
        delay(500);
        RGBoff(ledR, ledG, ledB);
        delay(500);
      }
      break;

    case 2:

      int estado2 = digitalRead(boton2);
      delay(15);

      if (prevEstado2 == HIGH && estado2 == LOW) {
        alarma = 2;
        digitalWrite(buz, LOW);
      }
      prevEstado2 = estado2;

      lecPotF = analogRead(pot);
      potTemp = (lecPotF / 1023.0) * (45.0 - 20.0) + 20.0;
      Serial.println(potTemp, 1);

      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(potTemp, 1);
      lcd.print(" C      ");

      if (potTemp < 35.0){
        RGBpurple(ledR, ledG, ledB);
        myServo.write(0);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        if (alarma != 2){
          alarma = 1;
        }
      }
      else if (potTemp >= 35.0 && potTemp <= 37.0){
        RGBcyan(ledR, ledG, ledB);
        myServo.write(45);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :)     ");
        alarma = 0;
      }
      else if (potTemp > 37.0 && potTemp <= 38.0){
        RGByellow(ledR, ledG, ledB);
        myServo.write(90);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        alarma = 0;
      }
      else if (potTemp > 38.0 && potTemp <= 39.0){
        RGBorange(ledR, ledG, ledB);
        myServo.write(135);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        alarma = 0;
      }
      else {
        RGBwhite(ledR, ledG, ledB);
        myServo.write(180);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        if (alarma != 2){
          alarma = 1;
        }
      }

      if (alarma == 1){
        digitalWrite(buz, HIGH);
      }
      else {
        digitalWrite(buz, LOW);
      }

      break;
  }
}
