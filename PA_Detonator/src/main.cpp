#include <Arduino.h> //Zalaczenie bibliotek
#include <Keypad.h>
#include <LiquidCrystal.h>
#define LED1red 4 //Deklaracja zmiennych, przypisanie pinow
#define LED2red 8
#define LED1green 3
#define LED2green 9
#define LED1blue 2
#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define BUZZER 30
#define LED_BLUE 10
#define LED_RED 52
#define LED_GREEN 50
/*
#define PIN_A  33
#define PIN_B  35
#define PIN_C  37
#define PIN_D  36
#define PIN_E  34
#define PIN_F  36
#define PIN_G  38
#define D1  40
#define D2  44
#define D3  46
#define D4  48
 */
//4-digit segment display

LiquidCrystal lcd(53, 51, 49, 47, 45, 43); //Przypisanie pinow do wyswietlacza

const byte ROWS = 4; //deklaracja siatki klawiatury
const byte COLS = 4;

byte rowPins[ROWS] = {28, 26, 24, 22}; //przypisanie pinow do klawiatury
byte colPins[COLS] = {29, 27, 25, 23};

char keypressed; // nacisniety klawisz

char keys[ROWS][COLS] = //mapowanie klawiatury
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keyboard = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

int stanLED1red = LOW; //Deklaracja stanow 
int stanLED2red = LOW;
int stanLED1green = LOW;
int stanLED2green = LOW;
int stanLED1blue = LOW;

unsigned long aktualny_czas = 0; //wartosci czasowe do milisa
unsigned long zapTimeLED1red = 0;
unsigned long zapTimeLED2red = 0;
unsigned long zapTimeLED1green = 0;
unsigned long zapTimeLED2green = 0;
unsigned long zapTimeLED1blue = 0;
unsigned long zapTimeKeyBoard = 0;
unsigned long zapTimeHCSR04 = 0;

String PIN = ""; // Zmienne przechowujace wartosci wpisane
String unPIN = "";
int correctcheck;
boolean planted = false; //Zmienne logiczne
boolean storedPIN = false;
boolean checkPIN = false;
int i = 1;

int adcvalue = 0; //Zmienna ADC do wejsc analogowych
int pot = 0;



int measureDistance() // Deklaracja czujnika ultradzwiekowego
{
  long time, distance;

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  time = pulseIn(ECHO_PIN, HIGH);
  distance = time / 58;

  return distance;
}

void range(int a, int b) //Funkcja do pomiaru odleglosci
{
  int howFar = measureDistance();
  if ((howFar > a) && (howFar < b)){
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }
}

void setPIN () // Funkcja do zapisu PINu
{
  PIN = "";
  i = 1;
  lcd.setCursor(0,0);
  lcd.print("Set PIN");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("with numbers");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("to accept");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("press #");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PIN: ");
  lcd.setCursor(0,1);
  lcd.print(">>>");
  lcd.setCursor(4,1);
  while(storedPIN==false)
  {
    keypressed = keyboard.getKey();
    if(keypressed != NO_KEY) 
    {
      if (keypressed=='0'|| keypressed=='1'|| keypressed=='2'|| keypressed=='3'|| keypressed=='4'|| keypressed=='5' || keypressed=='6' || keypressed=='7' || keypressed == '8' || keypressed == '9') 
      {
        PIN += keypressed;
        lcd.print("*");
      }
      else if (keypressed=='#')
      {
        storedPIN = true;
        break;
      }
      else if (keypressed == 'D')
      {
        PIN = "";
        storedPIN = false;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("PIN:");
        lcd.setCursor(0,1);
        lcd.print(">>>");
        lcd.setCursor(4,1);
      }
    }
  }
  planted = true;
}

void setunPIN() //Funkcja do zapisu PINu rozbrajania
{
  unPIN = " ";
  lcd.setCursor(0,0);
  lcd.print("SET unPIN");
  lcd.setCursor(0,1);
  lcd.print("and tap #");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("unPIN:");
  lcd.setCursor(0,1);
  lcd.print(">>>");
  lcd.setCursor(4,1);
  while(storedPIN==true)
  {
    keypressed = keyboard.getKey();
    if(keypressed != NO_KEY) 
    {
      if (keypressed == '0'|| keypressed == '1'|| keypressed == '2'|| keypressed == '3'|| keypressed == '4'|| keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9') 
      {
        unPIN += keypressed;
        lcd.print(keypressed);
      }
      else if (keypressed == '#')
      {
        break;
      }
      else if (keypressed == 'D')
      {
        unPIN = "";
        storedPIN = true;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("unPIN:");
        lcd.setCursor(0,1);
        lcd.print(">>>");
        lcd.setCursor(4,1);
      }
    }
  }
}

void setup() 
{
  Serial.begin(9600); //Deklaracja predkosci przesylu
  lcd.begin(16,2);
  pinMode(LED1blue, OUTPUT);
  pinMode(LED1green, OUTPUT);
  pinMode(LED2green, OUTPUT);
  pinMode(LED1red, OUTPUT);
  pinMode(LED2red, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  pinMode(LED_RED, OUTPUT); //Deklaracja wyjsc
  /*
  pinMode(BUZZER, OUTPUT);
  pinMode(PIN_A, OUTPUT);     
  pinMode(PIN_B, OUTPUT);     
  pinMode(PIN_C, OUTPUT);     
  pinMode(PIN_D, OUTPUT);     
  pinMode(PIN_E, OUTPUT);     
  pinMode(PIN_F, OUTPUT);     
  pinMode(PIN_G, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT); //Deklaracja wyjsc wyswietlacza
  */
  setPIN();
  lcd.clear();
}



void loop() // Petla obslugi wejscia
{
  if(storedPIN==true)
  {
    digitalWrite(LED1green, HIGH);
    digitalWrite(LED2green, HIGH);
    digitalWrite(LED_GREEN, HIGH); 
  
      setunPIN();
      lcd.clear();
    if(PIN==unPIN) 
    {
      lcd.clear();
      lcd.println("CORRECT");
      delay(1000);
    } else  {
      digitalWrite(LED1green, LOW);
      digitalWrite(LED1red, HIGH);
      lcd.setCursor(0,0);
      lcd.print("WRONG!");
      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
      delay(1000);
      lcd.clear();
    }
  }

  /* 
  adcvalue = analogRead(A0);
  pot = analogRead(A1);

  if(adcvalue < 250) 
  {
    digitalWrite(LED_BLUE, HIGH);
  } else {
    digitalWrite(LED_BLUE, LOW);
  }
  delay (50); 
  */


 
 /*aktualny_czas = millis();*/

  /*if(aktualny_czas - zapTimeLED1blue >= 100UL) {
    zapTimeLED1blue = aktualny_czas;
    stanLED1blue = !stanLED1blue;
    digitalWrite(LED1blue, stanLED1blue);
  }*/
/*
  if(aktualny_czas - zapTimeLED1green >= 125UL) {
    zapTimeLED1green = aktualny_czas;
    stanLED1green = !stanLED1green;
    digitalWrite(LED1green, stanLED1green);
  }

  if(aktualny_czas - zapTimeLED1red >= 350UL) {
    zapTimeLED1red = aktualny_czas;
    stanLED1red = !stanLED1red;
    digitalWrite(LED1red, stanLED1red);
  }

  if(aktualny_czas - zapTimeLED2red >= 300UL) {
    zapTimeLED2red = aktualny_czas;
    stanLED2red = !stanLED2red;
    digitalWrite(LED2red, stanLED2red);
  }

   if(aktualny_czas - zapTimeLED2green >= 225UL) {
    zapTimeLED2green = aktualny_czas;
    stanLED2green = !stanLED2green;
    digitalWrite(LED2green, stanLED2green);
  }

  if(aktualny_czas - zapTimeHCSR04 >= 200UL) {
    zapTimeHCSR04 = aktualny_czas;

    {
      range(5, 15);
      delay(100);
    }
  }

  if(aktualny_czas - zapTimeKeyBoard >= 100UL) {
    zapTimeKeyBoard = aktualny_czas;
    if(storedPIN==true)
    {
      setunPIN();
      lcd.clear();
    if(PIN==unPIN) 
    {
      lcd.clear();
      lcd.println("CORRECT");
      delay(1000);
    }
    } 


  }
*/ 


}
