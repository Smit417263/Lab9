/*	Author: Smit Patel
 *  Partner(s) Name: 
 *	Lab Section:023
 *	Assignment: Lab #9  Exercise #1
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


unsigned char tmpB;
unsigned char tmpC;
unsigned char tmpD;
enum led3 {B_start, B_l1, B_l2, B_l3} B_state;

void B_tick(){
	switch(B_state){
		case B_start:
			B_state = B_l1;
			break;
		case B_l1:
			B_state = B_l2;
			break;
		case B_l2:
			B_state = B_l3;
			break;
		case B_l3:
			B_state = B_l1;
			break;
		default:
			break;
	}
	
	switch(B_state){
		case B_start:
			break;
		case B_l1:
			tmpB = 0x01;
			break;
		case B_l2:
			tmpB = 0x02;
			break;
		case B_l3:
			tmpB = 0x04;
			break;
		default:
			break;
	}
	
}	

enum led1 {T_start, T_off, T_on} T_state;
	
void T_tick(){
	switch(T_state) {
		case T_start:
			T_state = T_off;
			break;
		case T_off:
			T_state = T_on;
			break;
		case T_on:
			T_state = T_off;
			break;
		default:
			break;
	}
	switch(T_state){
		case T_start:
			break;
		case T_off:
			tmpC = 0x00;
			break;
		case T_on:
			tmpC = 0x08;
			break;
		default:
			break;
	}
}

enum C_state {start, on, off} C_state;
	
void C_tick(){
		switch(C_state){
			case start:
			C_state = off;
			break;
			case off:
			C_state = on;
			break;
			case on:
			C_state = off;
			break;
			default:
			break;
		}
		switch(C_state)
		{
			case start:
			break;
			case off:
			tmpD = tmpB + tmpC;
			break;
			case on:
			tmpD = tmpB + tmpC;
			break;
			default:
			break;
		}

		PORTB = tmpD;
}



int main(void){
	
	DDRB = 0xFF; PORTB = 0x00;
	
	
	B_state = B_start;
	T_state = T_start;
	C_state = start;
	tmpB = 0x00; 

	TimerSet(1000);
	TimerOn();
    
    while (1) {
		B_tick();
		T_tick();
		C_tick();
		while (!TimerFlag){}  
		TimerFlag = 0;  
    }

	return 1;
}


