/*
Code to make a speaker sound. This will be used as an indicator. 

Written by Halie Murray-Davis for Real World Measurements. 3/29/2013.
*/

#include "pitches.h"
#include "SoftwareSerial.h"

int leds []= {0, 1, 2, 3, 4}; //set pins for LEDS
int melody []= {NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A5, NOTE_F4};
int rhythm[] = {2, 1, 1, 4, 8};
int thermocouple = A0;
int thermistor = A1;

void setup() {
    int currentstate =0;
    pinMode(thermocouple, INPUT);
    for (int x=0; x<5; x++) {
      pinMode(leds [x], OUTPUT);
      leds[x]=LOW;
    }    
    Serial.begin(4800); 
}

void loop() {
  //Serial.print("Hello world.");totestserialport
  int val = analogRead(thermistor);    // read the input pin
  Serial.println("Thermistor");
  Serial.println(val);   // debug value
  Serial.println(" "); //makes space between outputs in serial port
  
  int thermocouple_val = analogRead(thermocouple);
  Serial.println("Thermocouple");
  Serial.println(thermocouple_val);
  
  Serial.println(currentstate);
   
  switch (currentstate){
    
  case 1: //the do nothing case
    if (thermocouple_val <= 500 && thermocouple_val >= 200) {
      noTone(8);
    }
    
    if (thermocouple_val >= 505 || thermocouple_val <= 195){
      break;
    }
        
  case 2:
      Serial.println("Inside case one!!!");
      
      if (thermocouple_val >=200 && thermocouple_val <= 500) {
              break;
            }
      
        for (int thisNote=0; thisNote <5; thisNote++) {
          int rhythms=1000/rhythm[thisNote];
            if (thermocouple_val >=200 && thermocouple_val <= 500) {
              break;
            }
          
            if (thermocouple_val >= 500 || thermocouple_val <= 200) {
              digitalWrite (leds [1], HIGH);
              tone(8, melody[thisNote], rhythms);
              int pause=rhythms * 1.3;
              delay(pause);
              noTone(8);
            }   //end for if music 
        } //end for larger music loop which progresses through the notes
    }
    currentstate++;
    if (currentstate >1) {
      currentstate=0;
    }
}


