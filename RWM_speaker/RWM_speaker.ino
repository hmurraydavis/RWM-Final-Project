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
int melody4[]={NOTE_A5, NOTE_A4, NOTE_E5, NOTE_E4};
int rhythm[] = {2, 1, 1, 4, 8};
int rhythm2 [] = {3,3,6};
int rhythm4[]={4,4,4,6};
int thermocouple = A0;
int thermistor = A1;
int hydrometer = A2;
int currentstate = 0;
int max_temp = 50;
int min_temp = 20;
int temp_error = 5;
int hydrometer_upper_bound = 20;
int hydrometer_lower_bound = 15;
int hydrometer_error = 5;
int roomtemp; //will be used to calibrate thermocuple, from thermistor
int bTis; //the b value for the linear, roomtemp eq from thermistor
int mTis; //the slope from the roomtemp eq from thermistor

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
  Serial.println("Thermistor");
  Serial.println(thermistor_val);   // debug value
  
  int thermocouple_val = analogRead(thermocouple);
  Serial.println("Thermocouple");
  Serial.println(thermocouple_val);
  
  int hydrometer_val = analogRead(hydrometer);
  Serial.println("Hydrometer");
  Serial.println(hydrometer_val);
  
  //get roomtemperature from thermistor:
  mTis = (30-20)/(1022-750);
  bTis = 25-(mTis * 875);
  roomtemp = (mTis * thermistor_val) + bTis; 
 
 // Emily's mystical, magical function which makes the analog read off the thermocouple into something useful
 int temp_inside_box = (-35.6 * log (thermocouple_val) ) + 244.89 + roomtemp; //add in roomtemb b/c that's cool and good, according the the mystical, magical one.

  //begin code to switch between cases:
    //case 1= do nothing
    //case 2= temperature problem
    //case 3= humidity problem
    //case 4 = both temperature and humidity problem 

  switch (currentstate){
    
  case 1: //the do nothing case
      //makes it be silent if all is good:
    if (temp_inside_box <= max_temp && temp_inside_box >= min_temp) {
      if (hydrometer_val >= hydrometer_lower_bound && hydrometer_val <= hydrometer_upper_bound) {
        noTone(8);
      }
    }
    
      //breaks to case 2 if the thermo couple escapes the acceptable range:
    if (temp_inside_box >= max_temp + temp_error || temp_inside_box <= min_temp - temp_error){
      break;
    }
    
      //breaks to case 3 (the oh-no-hydrometer case) if the hydrometer escapes the acceptable range:
    if (hydrometer_val <= hydrometer_lower_bound - hydrometer_error || hydrometer_val >= hydrometer_upper_bound + hydrometer_error) {
      currentstate = 3; //if there is a humidity problem, bypasses case 2
      break;
    }
        
  case 2: //thermocouple is outside range
      Serial.println("Inside case two--thermocouple outside of range!!!");
      
      //breaks if the temperature is OK:
      if (temp_inside_box >=min_temp - temp_error && temp_inside_box <= max_temp + temp_error) {
         break;
      }

      //breaks if the humidity is bad:
      if (hydrometer_val + hydrometer_error >= hydrometer_upper_bound || hydrometer_val - hydrometer_error <= hydrometer_lower_bound) {
          break; //don't need to re-assign currentstate b/c case 3 is the next one
      }
        //plays music if the temperature is an issue:
        for (int thisNote=0; thisNote <5; thisNote++) {
          int rhythms=1000/rhythm[thisNote];
            if (temp_inside_box <=max_temp && temp_inside_box >= min_temp) { //checks temp before playing each note and will break if the temp is OK.
              break;
            }
          
            if (temp_inside_box >= max_temp || temp_inside_box <= min_temp) { //plays music if temperature is bad.
              digitalWrite (leds [0, 1], HIGH);
              tone(8, melody[thisNote], rhythms);
              int pause=rhythms * 1.3;
              delay(pause);
              noTone(8);
            }   //end for if music 
        } //end for larger music loop which progresses through the notes
        
    case 3: //hydrometer is outside range
      //breaks out of case if hydrometer is within the correct bounds:
      if (hydrometer_val <= hydrometer_upper_bound + hydrometer_error || hydrometer_val >= hydrometer_lower_bound - hydrometer_error){
        break;
      }

      //breaks out of case if temperature is wrong: (OK, b/c will go to case 4 (both bad) and we know the humidity isn't good, if it's made it this far)
      if (temp_inside_box + temp_error >= min_temp || temp_inside_box - temp_error <= max_temp) {
        break;
      }
      
      //sets off alarm if hydrometer is outside the correct bounds:
      if (hydrometer_val >=hydrometer_upper_bound || hydrometer_val <= hydrometer_lower_bound) {
             for (int thisNote2=0; thisNote2 <3; thisNote2++) {
                int rhythms=1000/rhythm2[thisNote2];
                if (hydrometer_val >=hydrometer_lower_bound && hydrometer_val <= hydrometer_upper_bound) {
                      break;
                }    
          
                if (hydrometer_val >= hydrometer_upper_bound || hydrometer_val <= hydrometer_lower_bound) {
                  digitalWrite (leds [2, 3], HIGH);
                  tone(8, melody2[thisNote2], rhythms);
                  int pause=rhythms * 1.3;
                  delay(pause);
                  noTone(8);
                } //end music playing hydrometer error conditional
             } //ends note progression for loop  
       } //ends the if loop which activates music playing if there are issues with hydrometer range.
       
    case 4:
      if (temp_inside_box >= max_temp || temp_inside_box <= min_temp)   {
        if (hydrometer_val >=hydrometer_upper_bound || hydrometer_val <= hydrometer_lower_bound){
            for (int thisNote4=0; thisNote4 < 3; thisNote4++) {
                int rhythms=1000/rhythm4[thisNote4];
                if (hydrometer_val >=hydrometer_lower_bound && hydrometer_val <= hydrometer_upper_bound) {
                    break;
                }    
                
                if (temp_inside_box >= max_temp || temp_inside_box <= min_temp) {
                    break;
                }
                
                  digitalWrite (leds [1, 3], HIGH);
                  tone(8, melody4[thisNote4], rhythms);
                  int pause=rhythms * 1.3;
                  delay(pause);
                  noTone(8);
             } //ends note progression for loop  
        }
      }
      
    
    } // end switch statements
    currentstate++;
    if (currentstate >1) {
      currentstate=0;
    }
}

