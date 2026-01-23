#include "MKL05Z4.h"
#include "frdm_bsp.h"

#include "i2c.h"
#include "lcd1602.h"
#include "led.h"
#include "uart0.h"

#include "defines.h"
#include "metronome.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char rx_buf[16];
char ON[] = "ON";
char OFF[] = "OFF";
char UP[] = "UP";
char DOWN[] = "DOWN";
char Error[] = "Wrong command";
char Too_Long[] = "Too long string";

uint8_t rx_buf_pos = 0;
char temp, buf;
uint8_t rx_FULL = 0;
uint8_t too_long = 0;

volatile uint16_t current_bpm = BPM_DEFAULT;
volatile uint8_t is_running = 0;
volatile uint32_t ms_counter = 0;
volatile uint32_t beat_interval_ms = 0;
volatile uint8_t pulse_active = 0;
volatile uint8_t pulse_timer = 0;


void UART0_IRQHandler() {
	if(UART0->S1 & UART0_S1_RDRF_MASK) {
		temp=UART0->D;
		if(!rx_FULL) {
			if(temp != CR) {
				if(!too_long) {
					rx_buf[rx_buf_pos] = temp;
					rx_buf_pos++;
					if(rx_buf_pos == 16)
						too_long = 1;
				}
			}
			else {
				if(!too_long) {
					rx_buf[rx_buf_pos] = 0;
				}
				rx_FULL = 1;
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}

void TPM0_IRQHandler(void) {
	if(TPM0->STATUS & TPM_STATUS_TOF_MASK) {
		TPM0->STATUS |= TPM_STATUS_TOF_MASK;
		
		if(is_running) {
			ms_counter++;
			
			if(ms_counter >= beat_interval_ms) {
				ms_counter = 0;				
				PTB->PCOR = (1 << 8);				
				pulse_active = 1;
				pulse_timer = 50;
			}
		}
		
		if(pulse_active) {
			pulse_timer--;
			if(pulse_timer == 0) {
				PTB->PSOR = (1 << 8);
				pulse_active = 0;
			}
		}
	}
}

int main(void) {	
	uint8_t i;
	
	LED_Init();
	UART0_Init();
	LCD1602_Init();
	
	Metronome_TimerInit();
	
	CalcInterval();
	Update_LCD_Display();
	UART0_SendStr("Metronome ready. Type: ON, OFF, UP, DOWN or BPM value");
	
	while(1) {
		if(rx_FULL) {
			if(too_long) {
				for(i = 0; Too_Long[i] != 0; i++) {
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));
						UART0->D = Too_Long[i];
				}
				while(!(UART0->S1 & UART0_S1_TDRE_MASK));
				UART0->D = LF;
				while(!(UART0->S1 & UART0_S1_TDRE_MASK));
				UART0->D = CR;
				too_long = 0;
			}
			else {
				if(strcmp(rx_buf, ON) == 0)	{
					is_running = 1;
					ms_counter = 0;
					Update_LCD_Display();
					UART0_SendStr("Metronome started");
				}
				else if(strcmp(rx_buf, OFF) == 0) {
					is_running = 0;
					PTB->PSOR = (1 << 8);
					Update_LCD_Display();
					UART0_SendStr("Metronome stopped");
				}
				else if(strcmp(rx_buf, UP) == 0) {
					if(current_bpm < BPM_MAX) {
						current_bpm += BPM_STEP;
						CalcInterval();
						Update_LCD_Display();
						UART0_SendStr("Tempo UP");
						UART0_SendNum(current_bpm);
					}
					else {
						UART0_SendStr("Max BPM reached");
					}
				}
				else if(strcmp(rx_buf, DOWN) == 0) {
					if(current_bpm > BPM_MIN) {
						current_bpm -= BPM_STEP;
						CalcInterval();
						Update_LCD_Display();
						UART0_SendStr("Tempo DOWN");
						UART0_SendNum(current_bpm);
					}
					else {
						UART0_SendStr("Min BPM reached");
					}
				}
				else if(rx_buf[0] >= '0' && rx_buf[0] <= '9') {
					uint8_t is_numeric = 1;
					for(i = 0; rx_buf[i] != 0; i++) {
						if(rx_buf[i] < '0' || rx_buf[i] > '9') {
							is_numeric = 0;
							break;
						}
					}
					if(is_numeric) {
						int val = atoi(rx_buf);
						if(val >= BPM_MIN && val <= BPM_MAX) {
							current_bpm = (uint16_t)val;
							CalcInterval();
							Update_LCD_Display();
							UART0_SendStr("New tempo set");
							UART0_SendNum(current_bpm);
						}
						else {
							UART0_SendStr("Out of range");
						}
					}
					else {
						UART0_SendStr("Digits only");
					}
				}
				else {
					for(i = 0; Error[i] != 0; i++) {
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));
						UART0->D = Error[i];
					}
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));
						UART0->D = LF;
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));
						UART0->D = CR;
				}
			rx_buf_pos=0;
			rx_FULL=0;
			}
		}
	}	
}
