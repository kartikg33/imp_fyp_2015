//timer interrupts
//by Amanda Ghassaei
//June 2012
//http://www.instructables.com/id/Arduino-Timer-Interrupts/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

//timer setup for timer2.
//For arduino uno or any board with ATMEL 328/168.. diecimila, duemilanove, lilypad, nano, mini...

//this code will enable an arduino timer interrupt.
//timer2 will interrupt at 8kHz

//constants
#define mic A5
//#define FS 8000 //read sample rate
#define FS 44100 //read sample rate
#define readsec 0.1 //no. of seconds of input values

//variables
int temp;
float tau = 1/FS;


void setup(){

  pinMode(mic, INPUT); //microphone
  Serial.begin(115200); //baud 115200

}//end setup

void loop(){
  //temp=analogRead(mic);
  Serial.println(analogRead(mic));
  delay(tau);
}
