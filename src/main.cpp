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
char enteredCode[7];

bool intruderDetected = false; 
bool systemArmed = false; 
bool codeRequired = false;

void setup() {

    Serial.begin(9600);

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);

}

void loop() {

    while (systemArmed == false) {

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Activate");
        lcd.setCursor(0, 1);
        lcd.print("Security System: ");

        char key = keypad.getKey();

        if (key == '#'){
            systemArmed = true;
        }
    }

    while (systemArmed == true && intruderDetected == false) {

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);

        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        long distance = duration * 0.034 / 2;

        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("System Armed");
        lcd.setCursor(0, 2);
        lcd.print("Scanning...");

        if (distance < 10) {
            intruderDetected = true;
            codeRequired = true;
        }
    }

    while (intruderDetected == true && codeRequired == true) {

        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("INTRUDER DETECTED!");
        
        delay(2000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter code: ");

        codeRequired = false;

    }

    while (systemArmed == true && intruderDetected == true && codeRequired == false) {

        char key = keypad.getKey();

        if (key) {

            lcd.setCursor(cursorPosition, 2);
            lcd.print(key);

            enteredCode[cursorPosition] = key;
            cursorPosition++;

            if (cursorPosition >= 6) {

                enteredCode[6] = '\0'; 

                if (strcmp(enteredCode, "67**67") == 0) {

                    delay(500);
                    lcd.clear();
                    lcd.setCursor(0, 1);
                    lcd.print("Alarm Disarmed!");

                    delay(3000);

                    systemArmed = false;
                    intruderDetected = false;

                }else{
                    delay(500);
                    lcd.clear();
                    lcd.setCursor(0, 1);
                    lcd.print("Pincode Incorrect!");

                    delay(1500);

                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Enter pincode: ");

                    cursorPosition = 0;
                    enteredCode[0] = '\0';
                }
            }
        }
    }
}