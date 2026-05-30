#include <Servo.h> //librerias utilizadas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int servoPin = A3;
Servo myServo;

int ledR = A2; //declarar que pines se usaran
int ledG = 11;
int ledB = A1;
int buz = 3;
int boton1 = 13;
int boton2 = 12;
int pot = A0;

int disp[7] = {10,9,8,7,6,5,4}; //pines pantalla LCD
byte num[3][7] = {
  {1,1,1,1,1,1,0}, //0
  {0,1,1,0,0,0,0}, //1
  {1,1,0,1,1,0,1}  //2
};

int prevEstado1 = LOW; //estados para antirrebote
int prevEstado2 = LOW;

int contador = 0;
int alarma = 0;


//funciones para cada color del LED RGB
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
  myServo.attach(servoPin); //relacionar myServo con el pin servoPin
  
  pinMode(ledR, OUTPUT); //configuración de cada pin 
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(pot, INPUT);

  for(int i = 0; i < 7; i++){ 
    pinMode(disp[i], OUTPUT);
  }

  lcd.init(); //configuración pantalla LCD
  lcd.backlight();

  Serial.begin(9600); //iniciar comunicación serial
}

void loop() {

  //antirebote boton1
  int estado1 = digitalRead(boton1);
  delay(15);

  if (prevEstado1 == HIGH && estado1 == LOW) {
    contador++;
    if(contador > 2){ //cuando contador es mayor a dos
      contador = 0;   //vuelve a cero
    }
    alarma = 0; 
    digitalWrite(buz, LOW); //buzzer inicia apagado
    lcd.clear();
    Serial.println(contador);
  }
  prevEstado1 = estado1;

  for(int i = 0; i < 7; i++){
    digitalWrite(disp[i], num[contador][i]); //mostrar en display el numero definido por contador
  }

  int lecPot;
  int potFrec;
  int potServoFrec;
  
  float lecPotF; 
  float potTemp; //lecturas de potenciometro en temperatura deben tener decimales

  switch(contador){ //dependiendo el caso, definido por contador

    case 0: //cuando contador es cero
      myServo.write(0);
      digitalWrite(buz, LOW);
      RGBoff(ledR, ledG, ledB);
      break;

    case 1: //cuando contador es uno
      lecPot = analogRead(pot); //leer valores del pot de 0 a 1023
      potFrec = map(lecPot, 0, 1023, 0, 200); //cambiar el rango de lectura de 0 a 200
      Serial.println(potFrec); 
      potServoFrec = map(potFrec, 0, 200, 0, 180); //cambiar el rango de lectura de 0 a 180
      myServo.write(potServoFrec); //servo tomara el angulo segun la lectura del pot, de 0 a 180

      lcd.setCursor(0, 0);
      lcd.print("Frec: ");
      lcd.print(potFrec);
      lcd.print(" lpm     ");

      if (potFrec < 60){ //bradicardia
        RGByellow(ledR, ledG, ledB);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
      }
      else if (potFrec >= 60 && potFrec <= 100){ //normal
        RGBgreen(ledR, ledG, ledB);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :)     ");
      }
      else if (potFrec > 100 && potFrec < 150){ //taquicardia
        RGBred(ledR, ledG, ledB);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
      }
      else { //taquicardia severa
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        RGBred(ledR, ledG, ledB);
        delay(500);
        RGBoff(ledR, ledG, ledB);
        delay(500);
      }
      break;

    case 2: //cuando contador es 2

      int estado2 = digitalRead(boton2); //antirebote en boton2
      delay(15);

      if (prevEstado2 == HIGH && estado2 == LOW) {
        alarma = 2;
        digitalWrite(buz, LOW);
      }
      prevEstado2 = estado2;

      lecPotF = analogRead(pot); //tomar medida del pot como flotante (con decimales)
      potTemp = (lecPotF / 1023.0) * (45.0 - 20.0) + 20.0;
      Serial.println(potTemp, 1); //se mostrara un decimal

      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(potTemp, 1);
      lcd.print(" C      ");

      if (potTemp < 35.0){ //Hipotermia
        RGBpurple(ledR, ledG, ledB);
        myServo.write(0);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        if (alarma != 2){
          alarma = 1;
        }
      }
      else if (potTemp >= 35.0 && potTemp <= 37.0){ //Normal
        RGBcyan(ledR, ledG, ledB);
        myServo.write(45);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :)     ");
        alarma = 0;
      }
      else if (potTemp > 37.0 && potTemp <= 38.0){ //Febrícula
        RGByellow(ledR, ledG, ledB);
        myServo.write(90);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        alarma = 0;
      }
      else if (potTemp > 38.0 && potTemp <= 39.0){ //Fiebre
        RGBorange(ledR, ledG, ledB);
        myServo.write(135);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        alarma = 0;
      }
      else { //Fiebre alta
        RGBwhite(ledR, ledG, ledB);
        myServo.write(180);
        lcd.setCursor(0, 1);
        lcd.print("Estado:  :(     ");
        if (alarma != 2){
          alarma = 1;
        }
      }

      if (alarma == 1){ //buzzer solo sonará cuando alarma == 1 
        digitalWrite(buz, HIGH);
      }
      else {
        digitalWrite(buz, LOW);
      }

      break;
  }
}
