/*	Author: Sana
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//Demo: https://drive.google.com/open?id=12rwV7OEQ4EEE93OZ1PWeIorYhlopU8Jw
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "../header/timer.h"
#endif



enum States {start, Init, wait, dec, waitDec, inc, waitInc, reset, waitReset} state;

      unsigned char tmpA;
      unsigned char C;
      unsigned char i = 0x00;

void Tick() {
	switch(state) {
		case start:
			state = Init;
			break;
		case Init:
			C = 0x00;
			state = wait;
			break;
		case wait:
			if (tmpA == 0x00) {	 state = wait;}
			else if (tmpA == 0x01) { state = inc;}
			else if (tmpA == 0x02) { state = dec;}
			else 		       {  state = reset;}
			break;
		case dec:
			if (tmpA == 0x02) {	 state = waitDec;}
			else if (tmpA == 0x03) { state = reset;}
			else {			 state = wait;}
			break;
		case waitDec:
			if (tmpA == 0x02 && i < 10) {	   state = waitDec;}
			else if (tmpA == 0x03 && i < 10) { state = reset;}
			else if (i >= 10) {	 	   state = dec; i = 0;}
			else {			 	   state = wait;}
			break;
		case inc:
			if (tmpA == 0x01) {      state = waitInc;}
                        else if (tmpA == 0x03) { state = reset;}
                        else {                   state = wait;}
			break;
		case waitInc:
			if (tmpA == 0x01 && i < 10) {	   state = waitInc;}
                        else if (tmpA == 0x03 && i < 10) { state = reset;}
			else if (i >= 10) { 	 	   state = inc; i = 0;}
			else { 			 	   state = wait;}
                        break;
		case reset:
                        if (tmpA == 0x03) { state = waitReset;}
                        else {              state = wait;}
			break;
		case waitReset:
			if (tmpA == 0x03) { state = waitReset;}
			else {		    state = wait;}
                        break;
		default:
			PORTC = 0x00;
			state = start;
			break;
	}
	switch(state) {
		case Init:				break;
		case wait:				break;
		case dec:	if (C != 0x00) {C--;}	break;
		case waitDec:	i++;			break;
		case inc:	if (C != 0x09) {C++;}	break;
		case waitInc:	i++;			break;
		case reset:	C = 0x00; 		break;
		case waitReset:				break;
		default:				break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; //PORTA = input
	DDRC = 0xFF; PORTC = 0x00; //PORTC = output
	DDRD = 0xFF; PORTD = 0x00; // PORTD = output
	TimerSet(100);
	TimerOn();

	i = 0x00;
	state = start;

    while (1) {
	tmpA = ~PINA & 0x03;
	Tick();	
	while(!TimerFlag);
	TimerFlag = 0;
	LCD_ClearScreen();
	LCD_WriteData(C + '0');
    }
    return 1;
}
