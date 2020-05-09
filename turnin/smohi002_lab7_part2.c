/*	Author: Sana
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//Demo: https://drive.google.com/open?id=12vndNTH2i6mCSsuB7Lwc-ujjxYUAtOcf
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "../header/timer.h"
#include "../header/io.h"
#endif

enum States {start, pb0, waitfall, wait, pb1, pb2, pb11, pb0h, pb1h, pb2h, pb11h} state;

unsigned char A0 = 0x00;
unsigned char C;
unsigned char i = 0x00;

void Tick() {
	switch(state) {
		case start:
			state = pb0;
			break;
		case pb0:
			if (!A0) state = pb1;
			else {
				state = waitfall;
				if (C > 0) C--;
			}
			break;
		case pb1:
			if (!A0) state = pb2;
			else {
				state = waitfall;
				if (C < 9) C++;
			}
			break;
		case pb2:
			if (!A0) state = pb11;
			else {
				state = waitfall;
				if (C > 0) C--;
			}
                        break;
		case pb11:
			if (!A0) state = pb0; 
			else {
				state = waitfall;
				if (C < 9) C++;
			}
			break;
		case waitfall:
                        if (!A0) state = wait;
                        else state = waitfall;
                        break;
                case wait:
                        if (!A0) state = wait;
                        else {
				state = pb0h; 
				PORTB = 0x01;
/*	                if (prev == 0x02 && C < 9) {
                                C++;
                        }
                        else if ((prev == 0x01 || prev == 0x03) && C > 0) {
                                C--;
                        }
*/			}
                        break;
		case pb0h:
			if (!A0) state = pb1;
			else state = pb1h;
			break;
		case pb1h:
			if (!A0) state = pb2;
			else state = pb2h;
			break;
		case pb2h:
			if (!A0) state = pb11;
			else state = pb11h;
			break;
		case pb11h:
			if (!A0) state = pb0;
			else state = pb0h;
			break;
		default:
			state = start;
			break;
	};
	switch(state) {
		case pb0:
			PORTB = 0x01;
			break;
		case pb1:
			PORTB = 0x02;
			break;
		case pb2:
			PORTB = 0x04;
			break;
		case pb11:
			PORTB = 0x02;
			break;
		case waitfall:  break;
                case wait:	break;
		case pb0h:
			PORTB = 0x01;
			break;
		case pb1h:
			PORTB = 0x02;
			break;
		case pb2h:
			PORTB = 0x04;
			break;
		case pb11h:
			PORTB = 0x02;
			break;
		default: 	break;
	};
}
	
int main(void) {
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTC = 0x00;
	TimerSet(300);
	TimerOn();

	C = 0x05;
    while (1) {
	A0 = (~PINA & 0x01);
	Tick();
	while(!TimerFlag); // Wait 1 sec
	TimerFlag = 0;
	if (C < 9) {
		LCD_ClearScreen();
		LCD_WriteData(C + '0');
	}
	else {
		LCD_ClearScreen();
		LCD_DisplayString(1, "WINNER!");
		i++;
		if (i >= 3) C = 0x05;
    	}
    }
    return 1;
}
