/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "bsp.h"

/* LED�ڶ�Ӧ��RCCʱ�� */
#define LED_GPIO_CLK_ALLENABLE() {	\
		__HAL_RCC_GPIOA_CLK_ENABLE();	\
	};

#define GPIO_PIN_LED1 GPIO_PIN_7
#define GPIO_PORT_LED1 GPIOA

#define GPIO_PIN_LED2 GPIO_PIN_8
#define GPIO_PORT_LED2 GPIOA


/* ���ⲿ���õĺ������� */
void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);
uint8_t bsp_IsLedOn(uint8_t _no);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
