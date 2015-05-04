
#include "wiring_private.h"
#include "wiring_analog.c"
#include "pins_arduino.h"

//constants
//#define mic A0
#define gainpin 3

//variables
/*
0 = off
64 = 1.25V
128 = 2.5V
keep it between 0.7 and 2.2V
therefore 
*/
int gain = 80; //between 64 and 128, 0 turns off
int read;
uint8_t low, high;
uint8_t pin = 0;

void setup(){
  pinMode(gainpin, OUTPUT);
  pinMode(A0, INPUT); //microphone
  Serial.begin(230400);//115200
  analogWrite(gainpin,gain);
}//end setup

void loop(){

        // set the analog reference (high two bits of ADMUX) and select the
        // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
        // to 0 (the default).
        ADMUX = (analog_reference << 6) | (pin & 0x07);
        // start the conversion
        sbi(ADCSRA, ADSC);
        // ADSC is cleared when the conversion finishes
        while (bit_is_set(ADCSRA, ADSC));
        // we have to read ADCL first; doing so locks both ADCL
        // and ADCH until ADCH is read.  reading ADCL second would
        // cause the results of each conversion to be discarded,
        // as ADCL and ADCH would be locked when it completed.
        low  = ADCL;
        high = ADCH;
        // combine the two bytes
        read = (high << 8) | low;
  
  Serial.println(read);
}
