/*
Code to make a speaker sound. This will be used as an indicator. 

Written by Halie Murray-Davis for Real World Measurements. 3/29/2013.
*/

#include "pitches.h"
#include "SoftwareSerial.h"

int leds []= {0, 1, 2, 3, 4}; //set pins for LEDS
int melody []= {NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A5, NOTE_F4};
int rhythm[] = {2, 1, 1, 4, 8};
int fakesensor = A0;

void setup() {
    pinMode(fakesensor, INPUT);
    for (int x=0; x<5; x++) {
      pinMode(leds [x], OUTPUT);
      leds[x]=LOW;
    }    
    Serial.begin(4800); 
}

void loop() {
  //Serial.print("Hello world.");totestserialport
  int val = analogRead(fakesensor);    // read the input pin
  Serial.println(val);             // debug value
    if (val>=680){
        for (int thisNote=0; thisNote <5; thisNote++) {
          int rhythms=1000/rhythm[thisNote];
          tone(8, melody[thisNote], rhythms);
          int pause=rhythms * 1.3;
          delay(pause);
          noTone(8);
        }
         for (int y=0; y<4; y++) {
           digitalWrite (leds [y], HIGH);
         }
        
    }
    
   
}


