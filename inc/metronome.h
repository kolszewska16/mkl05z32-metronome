#ifndef METRONOME_H
#define METRONOME_H

#include "frdm_bsp.h"
#include "i2c.h"
#include "lcd1602.h"
#include "defines.h"

extern volatile uint16_t current_bpm;
extern volatile uint8_t is_running;
extern volatile uint32_t beat_interval_ms;

/**
 * @brief Sends a character string via UART0 interface.
 *
 * @param str Pointer to the null-terminated string to be sent.
 */
void UART0_SendStr(char *str);

/**
 * @brief Converts an integer to a string and sends it via UART0.
 *
 * @param num The integer value to be sent.
 */
void UART0_SendNum(int num);

/**
 * @brief Initializes the TPM1 timer and the output pin for buzzer PWM control.
 */
void PWM_BuzzerInit(void);

/**
 * @brief Activates the sound generation.
 */
void BuzzerOn(void);

/**
 * @brief Deactivates the sound generation.
 */
void BuzzerOff(void);

/**
 * @brief Initializes the TPM0 timer as the metronome's time base.
 */
void Metronome_TimerInit(void);

/**
 * @brief Calculate the time interval based on the current tempo (BPM).
 */
void CalcInterval(void);

/**
 * @brief Updates the information shown on the LCD display.
 */
void Update_LCD_Display(void);

#endif  /* METRONOME_H */