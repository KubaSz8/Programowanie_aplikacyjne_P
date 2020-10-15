#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#define LED1red 4
#define LED1green 3
#define LED1blue 2

LiquidCrystal lcd(53, 51, 49, 47, 45, 43);
const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {28, 26, 24, 22};
byte colPins[COLS] = {29, 27, 25, 23};

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int stanLED1red = LOW;
int stanLED1green = LOW;
int stanLED1blue = LOW;

unsigned long aktualny_czas = 0;
unsigned long zapTimeLED1red = 0;
unsigned long zapTimeLED1green = 0;
unsigned long zapTimeLED1blue = 0;
unsigned long zapTimeKeyBoard = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED1blue, OUTPUT);
  pinMode(LED1green, OUTPUT);
  pinMode(LED1red, OUTPUT);
  
}

void loop() {
  
  aktualny_czas = millis();

  if(aktualny_czas - zapTimeKeyBoard >= 100UL) {
    zapTimeKeyBoard = aktualny_czas;

    char klawisz = klawiatura.getKey();

    if(klawisz) {
      lcd.print(klawisz);
    }
  }

  if(aktualny_czas - zapTimeLED1blue >= 100UL) {
    zapTimeLED1blue = aktualny_czas;
    stanLED1blue = !stanLED1blue;
    digitalWrite(LED1blue, stanLED1blue);
  }

  if(aktualny_czas - zapTimeLED1green >= 125UL) {
    zapTimeLED1green = aktualny_czas;
    stanLED1green = !stanLED1green;
    digitalWrite(LED1green, stanLED1green);
  }

  if(aktualny_czas - zapTimeLED1red >= 150UL) {
    zapTimeLED1red = aktualny_czas;
    stanLED1red = !stanLED1red;
    digitalWrite(LED1red, stanLED1red);
  }

}