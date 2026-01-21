#include "metronome.h"
#include <stdio.h>

void UART0_SendStr(char *str) {
	while(*str != 0) {
		while(!(UART0->S1 & UART0_S1_TDRE_MASK));
		UART0->D = *str++;
	}
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = CR;
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = LF;
}

void UART0_SendNum(int num) {
	char str[10];
	sprintf(str, "%d", num);
	UART0_SendStr(str);
}

void PWM_BuzzerInit(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	PORTB->PCR[0] = PORT_PCR_MUX(3);
	TPM1->SC |= TPM_SC_PS(7);
	TPM1->MOD = 82;
	
	TPM1->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM1->CONTROLS[0].CnV = 0;
	
	TPM1->SC |= TPM_SC_CMOD(1);
}

void BuzzerOn(void) {
	TPM1->CONTROLS[0].CnV = (TPM1->MOD / 2);
}

void BuzzerOff(void) {
	TPM1->CONTROLS[0].CnV = 0;
}

void Metronome_TimerInit(void) {
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM0->SC = 0;
	
	TPM0->SC = TPM_SC_PS(4);
	TPM0->MOD = 1310;
	TPM0->SC |= TPM_SC_TOIE_MASK;
	
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);
	
	TPM0->SC |= TPM_SC_CMOD(1);
}

void CalcInterval(void) {
	beat_interval_ms = 60000 / current_bpm;
}

void Update_LCD_Display(void) {
	char buff[17];
	LCD1602_ClearAll();
	LCD1602_SetCursor(0, 0);
	
	if(is_running) {
		LCD1602_Print("METRONOME: ON");
	}
	else {
		LCD1602_Print("METRONOME: OFF");
	}
	
	LCD1602_SetCursor(0, 1);
	sprintf(buff, "TEMPO: %d BPM", current_bpm);
	LCD1602_Print(buff);
}