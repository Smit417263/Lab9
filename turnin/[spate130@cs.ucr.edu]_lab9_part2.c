/*	Author: Smit Patel
 *  Partner(s) Name: 
 *	Lab Section:023
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link:
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


 volatile unsigned char TimerFlag = 0;
  unsigned long _avr_timer_M = 1;
  unsigned long _avr_timer_cntcurr = 0;
  
  void TimerOn() {
	  TCCR1B = 0x0B;

	  OCR1A = 125;
	  TIMSK1 = 0x02;

	  TCNT1=0;

	  _avr_timer_cntcurr = _avr_timer_M;

	  SREG |= 0x80; // 0x80: 1000000
  }

  void TimerOff() {
	  TCCR1B = 0x00;
  }

  void TimerISR() {
	  TimerFlag = 1;
  }

  ISR(TIMER1_COMPA_vect) {
	  _avr_timer_cntcurr--;
	  if (_avr_timer_cntcurr == 0) {
		  TimerISR();
		  _avr_timer_cntcurr = _avr_timer_M;
	  }
 }

  void TimerSet(unsigned long M) {
	  _avr_timer_M = M;
	  _avr_timer_cntcurr = _avr_timer_M;
  }


unsigned char led1;
unsigned char led3;

enum B_State{B_Start, B_On, B_Off} B_State;

void BlinkingLEDSM() {
    switch (B_State){
    case B_Start:
        B_State = B_On;
        break;
    case B_On:
        B_State = B_Off;
        break;
    case B_Off:
        B_State = B_Start;
        break;
    
    default:
        B_State = B_Start;
        break;
    }

    switch(B_State)
    {
    case B_Start:
        led1 = 0;
        break;
    case B_On:
        led1 = 0x08;
        break;
    case B_Off:
        led1 = 0;
        break;
    default:
        break;
    }

}


enum T_State {T_Start, l0, l1, l2} T_State;

void ThreeLEDsSM(){
    switch(T_State){
    case T_Start:
        T_State = l0;
        break;
    case l0:
        T_State = l1;
        break;
    case l1:
        T_State = l2;
        break;
    case l2:
        T_State = l0;
        break;
    
    default:
        T_State = T_Start;
        break;
    }

    switch(T_State){
   case T_Start:
        led3 = 0x00;
        break;
    case l0:
        led3 = 0x01;
        break;
    case l1:
        led3 = 0x02;
        break;
    case l2:
        led3 = 0x04;
        break;
    
    default:
        break;
    }

}

enum C_State{C_Start,on} C_State;

void CombinedLEDsSM() {
    switch (C_State){
    case C_Start:
        C_State = on;
        break;
    case on:
        C_State = on;
        break;
    default:
        C_State = C_Start;
        break;
    }

    switch(C_State){
    case C_Start:
        break;
    case on:
        PORTB = led1 | led3;
        break;
    default:
        break;
    }

}

int main(void){

    DDRB = 0xFF; PORTB = 0x00;

    B_State = B_Start;
    T_State = T_Start;
    C_State = C_Start;

    unsigned long B_elapsedTime = 1000;
    unsigned long T_elapsedTime = 300;
    unsigned long timerPeriod = 100;

    TimerSet(timerPeriod);
    TimerOn();

    while(1){
        if(B_elapsedTime >= 1000){
            BlinkingLEDSM();
            B_elapsedTime = 0;
        }
        if(T_elapsedTime >= 300){
            ThreeLEDsSM();
            T_elapsedTime = 0;
        }
        CombinedLEDsSM();
        while(!TimerFlag){};
        TimerFlag = 0;

        B_elapsedTime = B_elapsedTime + timerPeriod;
        T_elapsedTime = T_elapsedTime + timerPeriod;


    }
    return 1;
}
