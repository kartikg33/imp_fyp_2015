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
#define FS 8000 //read sample rate
#define readsec 0.1 //no. of seconds of input values

/*
// new variables
struct buff
{
  int val;
  Node *next;
  Node *prev;
};
typedef struct buff BUFF;
*/



//variables
const long BUFFLEN = FS * readsec; //no of samples in buffer
//int buff[BUFFLEN];
long ptr_in = 0;
long ptr_out = 0;

struct buffer
{
  int val;
  boolean seen;
};
//typedef struct buffer buff;
struct buffer buff[BUFFLEN];


void setup(){

  pinMode(mic, INPUT); //microphone
  
cli();//stop interrupts

//set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


sei();//allow interrupts

  Serial.begin(115200);

}//end setup
  
ISR(TIMER2_COMPA_vect){//timer1 interrupt 8kHz - read analogpin

  buff[ptr_in].val = analogRead(mic); 
  buff[ptr_in].seen = false;
  //Serial.println(buff[ptr_in]); //debugging input
  if (ptr_in >= BUFFLEN-1){
    ptr_in = -1;
  }
  ptr_in += 1;
  
}

void loop(){
  //Serial.println(500); //debugging output
  //Serial.println("Stuff");
  //delay(1);
  
  if(buff[ptr_out].seen == false){
    Serial.println(buff[ptr_out].val);
    buff[ptr_out].seen = true;
  }
  if (ptr_out >= BUFFLEN-1){
    ptr_out = -1;
  }
  ptr_out += 1;
  delay(0.1);
  
}
