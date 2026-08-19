#include <Arduino.h> 
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 10;
const int echoPin = 11;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', '%'},
  {'4', '5', '6', '!'},
  {'7', '8', '9', '@'},
  {'*', '0', '#', '?'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int cursorPosition = 0;
bool intruderDetected = false;
char enteredCode[7];

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

}

void loop() {

  while (intruderDetected == false) {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");

    delay(250);

    if (distance < 10) {

      intruderDetected = true;

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Intruder Detected!");

      delay(250);

      lcd.setCursor(0, 1);
      lcd.print("Enter Pincode:");

      cursorPosition = 0;
    }
  }

  char key = keypad.getKey();

  if (key) {

    lcd.setCursor(cursorPosition, 2);
    lcd.print(key);

    cursorPosition++;
  }
}