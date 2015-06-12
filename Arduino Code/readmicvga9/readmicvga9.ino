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
#define board1 A0
#define board2 A1
#define board3 A2
#define gain1 3
#define gain2 5
#define gain3 6
#define FS 32000 //read sample rate
#define readsec 0.1 //no. of seconds of input values

//variables
int temp;
/*
0 = off
64 = 1.25V
128 = 2.5V
keep it between 0.7 and 2.2V
therefore 
*/
int gain = 75; //between 64 and 128, 0 turns off
float tau = 1/FS;
int read;

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void setup(){
  setPwmFrequency(3, 1024);
  setPwmFrequency(5, 1024);
  setPwmFrequency(6, 1024);
  pinMode(gain1, OUTPUT);
  pinMode(gain2, OUTPUT);
  pinMode(gain3, OUTPUT);
  pinMode(board1, INPUT);
  pinMode(board2, INPUT);
  pinMode(board3, INPUT);
  analogWrite(gain1,gain);
  analogWrite(gain2,gain);
  analogWrite(gain3,gain);
  
  Serial.begin(230400);

}//end setup

void loop(){
 
  Serial.write('\n');
  read = analogRead(board1)*0.25;
  Serial.write(byte(read));
  read = analogRead(board2)*0.25;
  Serial.write(byte(read));
  read = analogRead(board3)*0.25;
  Serial.write(byte(read));

    
  //Serial.write(byte(read>>8));

}
