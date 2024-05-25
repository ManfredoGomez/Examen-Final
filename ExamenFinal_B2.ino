/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico:EB5AV 
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: Parte 1
   Dev: Manfredo Guillermo Vasquez Gomez
   Fecha: 25 de Mayo
   Enlade de simulacion:https://www.tinkercad.com/things/cogqCZ2myZp-proyecto-final-de-b2
*/
#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definición del teclado matricial
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // Conectar las filas del teclado a estos pines
byte colPins[COLS] = {5, 4, 3, 2}; // Conectar las columnas del teclado a estos pines

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo servoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C de la pantalla LCD, 16x2

int counter = 0;
bool counting = false;
bool direction = true; // true para contar de 0 a 99, false para contar de 99 a 0

// Pines conectados al 7448
#define PIN_A A0
#define PIN_B A1
#define PIN_C A2
#define PIN_D A3

// Definir los pines de los interruptores
int switches[3] = {6, 7, 8};

void setup() {
  Serial.begin(9600);
  servoMotor.attach(10); // Pin del servo
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Configurar pines para LEDs del auto fantástico
  for (int i = 11; i <= 14; i++) {
    pinMode(i, OUTPUT);
  }

  // Configurar pines para el 7448
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);

  // Configurar pines para los interruptores
  for (int i = 0; i < 3; i++) {
    pinMode(switches[i], INPUT_PULLUP);
  }

  // Mostrar mensaje fijo en la pantalla LCD
  lcd.setCursor(2, 0);
  lcd.print("Manfredo"); 
  lcd.setCursor(0, 1);
  lcd.print("Proyecto Final");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    switch(key) {
      case '1':
        counter = 0;
        counting = true; 
        direction = true; 
        break;
      case '2':
        counter = 99;
        counting = true; 
        direction = false; 
        break;
      case '3':
        // Animación del auto fantástico con 4 LEDs
        animateCar();
        break;
      case '4':
        // Animación personalizada en el display de 7 segmentos
        animateCustom();
        break;
      case '5':
        // Movimiento del servomotor de 0 a 180 grados
        moveServo();
        break;
      default:
        break;
    }
  }

  // Contador automático
  if (counting) {
    if (direction) {
      if (counter < 100) {
        Serial.println(counter);
        delay(1000); // Espera 1 segundo
        counter++;
      }
    } else {
      if (counter >= 0) {
        Serial.println(counter);
        delay(1000); // Espera 1 segundo
        counter--;
      }
    }
    // Detener el contador cuando llega a los extremos
    if ((direction && counter == 100) || (!direction && counter == -1)) {
      counting = false;
    }
  }
}

void animateCar() {
  // Implementa la animación del auto fantástico con 4 LEDs aquí
  for (int i = 0; i < 4; i++) {
    digitalWrite(i + 11, HIGH); 
    delay(200); 
    digitalWrite(i + 11, LOW); 
  }
  for (int i = 2; i >= 0; i--) {
    digitalWrite(i + 11, HIGH); 
    delay(200); // Esperar
    digitalWrite(i + 11, LOW);
  }
}

void animateCustom() {
  int delayTime = 500;   // Valor por defecto
  for (int i = 0; i < 3; i++) {
    if (digitalRead(switches[i]) == LOW) {
      delayTime = 1000 * (i + 1);  // 1, 2 o 3 segundos
      break;
    }
  }

  // Contar de dos en dos
  for (int i = 0; i <= 9; i += 2) {
    displayNumber(i);
    delay(delayTime); 
  }

  delay(2000); // Espera 2 segundos antes de empezar la siguiente cuenta

  // Contar de tres en tres
  for (int i = 0; i <= 9; i += 3) {
    displayNumber(i);
    delay(delayTime); 
  }
}

void displayNumber(int number) {
  // Convierte el número a BCD y lo muestra en el 7448
  digitalWrite(PIN_A, bitRead(number, 0));
  digitalWrite(PIN_B, bitRead(number, 1));
  digitalWrite(PIN_C, bitRead(number, 2));
  digitalWrite(PIN_D, bitRead(number, 3));
}

void moveServo() {
  // Mueve el servomotor de 0 a 180 grados
  for (int angle = 0; angle <= 180; angle += 1) {
    servoMotor.write(angle);
    delay(15);
  }
  // Regresar el servomotor a 0 grados
  for (int angle = 180; angle >= 0; angle -= 1) {
    servoMotor.write(angle);
    delay(15);
  }
}

