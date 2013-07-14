/*
Code to make a speaker sound and LEDs light. This will be used as an indicator. 

Written by Halie Murray-Davis for Real World Measurements. 3/29/2013.
*/

#include "pitches.h"
#include "SoftwareSerial.h"
#include "math.h"

int leds []= {0, 1, 2, 3, 4}; //set pins for LEDS
int melody []= {NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A5, NOTE_F4}; //melody for the temperature outside range case
int melody2 []= {NOTE_A4, NOTE_C4, NOTE_F3};
int rhythm[] = {2, 1, 1, 4, 8};
int rhythm2 [] = {3,3,6};
int rhythm4[]={4,4,4,6};
int thermocouple = A0;
int thermistor = A1;
int hydrometer = A2;
int currentstate = 0;
int max_temp = 35;
int min_temp = 27;
int temp_error = 0;
int max_humidity = 85;
int min_humidity = 65;
int roomtemp; //will be used to calibrate thermocuple, from thermistor
int bTis; //the b value for the linear, roomtemp eq from thermistor
int mTis; //the slope from the roomtemp eq from thermistor
int percent_humidity;

void setup() {
    int currentstate =0;
    pinMode(thermocouple, INPUT);
    pinMode (thermistor, INPUT);
    pinMode (hydrometer, INPUT);
    for (int x=0; x<5; x++) {
      pinMode(leds [x], OUTPUT);
      leds[x]=LOW;
    }    
    Serial.begin(4800); 
}

void loop() {
  
  // read the sensor output off the appropriate pin and return it in a variable for later use. Also labels this and prints it to the serial port for easy de-bugging.
  int thermistor_val = analogRead(thermistor);    // read the input pin
 // Serial.println("Thermistor");
  //Serial.println(thermistor_val);   // debug value
  
  int thermocouple_val = analogRead(thermocouple);
  //Serial.println("Thermocouple");
  //Serial.println(thermocouple_val);
  
  //get roomtemperature from thermistor:
  mTis = (30-20)/(1022-750);
  bTis = 25-(mTis * 875);
  roomtemp = (mTis * thermistor_val) + bTis; 
  
 Serial.println("room remperature:" );
 Serial.println(roomtemp);
  Serial.println(" " );
 
 // Emily's mystical, magical function which makes the analog read off the thermocouple into something useful
 int temp_inside_box = ( (5 * pow(10, -3)) * (pow(thermocouple_val,1)) ) + roomtemp; //add in roomtemb b/c that's cool and good, according the the mystical, magical one.
 Serial.println("temp_inside_box:" );
 Serial.println(temp_inside_box);

  //begin code to switch between cases:
    //case 1= do nothing
    //case 2= temperature problem
    //case 3= humidity problem
    //case 4 = both temperature and humidity problem 

  switch (currentstate){
    
  case 1: //the do nothing case
      //makes it be silent if all is good:
    if (temp_inside_box <= max_temp && temp_inside_box >= min_temp) {
      //if (hydrometer_val >= min_humidity && hydrometer_val <= max_humidity) {
        noTone(8);
      //}
    }
    
      //breaks to case 2 if the thermo couple escapes the acceptable range:
    if (temp_inside_box >= max_temp + temp_error || temp_inside_box <= min_temp - temp_error){
      Serial.println ("Temp too high.");
      break;
    }
        
  case 2: //thermocouple is outside range
      Serial.println("Inside case two--thermocouple outside of range!!!");
      
      //breaks if the temperature is OK:
      if (temp_inside_box >=min_temp && temp_inside_box <= max_temp) {
        Serial.println ("Exited temperature too high case because temperature returned to good range.");
         break;
      }

      Serial.println("Should play music. :(");
        //plays music if the temperature is an issue:
        for (int thisNote=0; thisNote <5; thisNote++) {
          int rhythms=1000/rhythm[thisNote];
            if (temp_inside_box <=max_temp && temp_inside_box >= min_temp) { //checks temp before playing each note and will break if the temp is OK.
              break;
            }
            
              digitalWrite (leds [0, 1], HIGH);
              tone(8, melody[thisNote], rhythms);
              int pause=rhythms * 1.3;
              delay(pause);
              noTone(8);
            //}   //end for if music 
        } //end for larger music loop which progresses through the notes      
    
    } // end switch statements
    currentstate++;
    if (currentstate >2) {
      currentstate=0;
    }
}

