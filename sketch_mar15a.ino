#include "FastLED.h"
#define NUM_LEDS 45
#define DATA_PIN A0
#define SCORE_LEDS 4
#define BRIGHTNESS 30
#define WIN 98
#define LOSE 99

CRGB leds[NUM_LEDS];
CRGB sleds[NUM_LEDS];

bool reachedEnd = false;
byte gameState = 0;
int period = 1000;
unsigned long time_now = 0;
byte Position = 0;
byte level = 0;

const byte ledSpeed[4] = {500, 100, 50, 10}; 
//const byte ledSpeed[4] = {500, 500, 500, 500}; // FOR DEBUGGING AND TESTING ONLY

//Debounce
bool findRandom = false;
byte spot = 0;

void setup() {
  // Setup code
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // Score/Level indicator
  pinMode(12, OUTPUT); // Level 1
  pinMode(11, OUTPUT); // Level 2
  pinMode(10, OUTPUT); // Level 3
  pinMode(9, OUTPUT);  // Level 4

  pinMode(A3, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Reset");
}

void loop() {  
  // Main game loop
  FastLED.setBrightness(BRIGHTNESS);
//
// Game state 0, also known as Idle state
//
  if (gameState == 0) {    
    fill_rainbow(leds, NUM_LEDS, 0, 20); //2 = longer gradient strip
    
    if (digitalRead(A3) == LOW) {
      Position = 0;
      findRandom = true;
      delay(500);
      for (byte i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 0, 0);
        delay(40);
        FastLED.show();
       int thisPitch = map (i, 60, 0, 100, 1500);
       tone(8, thisPitch,120);
      }

      digitalWrite(12, HIGH);
      digitalWrite(11,HIGH);
      digitalWrite(10,HIGH);
      digitalWrite(9,HIGH);
      delay(1000);
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);
      delay(500);
      gameState = 1;
    }
    FastLED.show();
  }

//
// Level 1
//
  if (gameState == 1) {
    period = ledSpeed[0];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(NUM_LEDS-4) + 3; // Spot spawns pseudo-randomly within the led strip
                               // The formula for getting the spot:
                               // random(n) + 3
                               // n = total number of led lights - 4
        findRandom = false;
      }
      leds[spot - 1].setRGB(255, 140, 0);
      leds[spot].setRGB(0, 255, 0);
      leds[spot + 1].setRGB(255, 110, 0);
      PlayGame(spot - 1, spot + 1);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      findRandom = false;
      if (Position > spot - 1 && Position < spot + 3) {
        level = gameState;
        gameState = WIN;

        digitalWrite(12, HIGH);
        digitalWrite(11,LOW);
        digitalWrite(10,LOW);
        digitalWrite(9,LOW);
      } 
      else {
        gameState = LOSE;
      }
    }
  }

//
// Level 2
//
 if (gameState == 2) {
    period = ledSpeed[2];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(NUM_LEDS-4) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);
      PlayGame(spot, spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = WIN; 
      digitalWrite(12, HIGH);
      digitalWrite(11,HIGH);
      digitalWrite(10,LOW);
      digitalWrite(9,LOW);            
      } 
      else {
        gameState = LOSE;
      }
    }
  }

//
// Level 3
//
  if (gameState == 3) {
    period = ledSpeed[2];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(NUM_LEDS-4) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);  
      PlayGame(spot, spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = WIN;

        digitalWrite(12, HIGH);
        digitalWrite(11,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(9,LOW); 
      } 
      else {
        gameState = LOSE;
      }
    }
  }

//
// Level 4
//
  if (gameState == 4) {
    period = ledSpeed[3];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(NUM_LEDS-4) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);
      PlayGame(spot, spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = WIN;

        digitalWrite(12, HIGH);
        digitalWrite(11,HIGH);
        digitalWrite(10,HIGH);
        digitalWrite(9,HIGH);
      } 
      else {
        gameState = LOSE;
      }
    }
  }
  
  if (gameState == WIN) {
    winner();
  }
  if (gameState == LOSE) {
    loser();
  } 
}

void PlayGame(byte bound1, byte bound2) {
  leds[Position].setRGB(255, 0, 0);
  if (Position < bound1 + 1 || Position > bound2 + 1) {
    leds[Position - 1].setRGB(0, 0, 0);
  }
  FastLED.show();
  Position++;
  if (Position >= NUM_LEDS) {
    leds[Position - 1].setRGB(0, 0, 0);
    Position = 0;
  }
}

void winner() {
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < NUM_LEDS; j++) {
      leds[j].setRGB(0, 255, 0);
        tone(8, 1000, 250);
    }
    FastLED.show();
    delay(500);
    clearLEDS();
    FastLED.show();
    delay(500);
  
  }
  findRandom = true;
  Position = 0;

  gameState = level + 1;
  if (gameState > 4) {
    gameState = 0;
  }
}

void loser() {
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < NUM_LEDS; j++) {
      leds[j].setRGB(255, 0, 0);
      tone(8, 200, 250);
    }
    FastLED.show();
    delay(500);
    clearLEDS();
    FastLED.show();
    delay(500);
    digitalWrite(12,LOW);
    digitalWrite(11,LOW);
    digitalWrite(10,LOW);
    digitalWrite(9,LOW);
  }
  gameState = 0;
}

void clearLEDS() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}