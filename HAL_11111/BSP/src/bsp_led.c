/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2019-04-24 armfly  ��ʽ����
*
*	Copyright (C), 2019-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp_led.h"
#include "main.h"

/*
	������STM32-V5 ������LED���߷��䣺
		LD1     : PI10/TP_NCS          (�͵�ƽ�������ߵ�ƽϨ��)
		LD2     : PF7/NRF24L01_CSN     (�͵�ƽ�������ߵ�ƽϨ��)
		LD3     : PF8/SF_CS            (�͵�ƽ�������ߵ�ƽϨ��)
		LD4     : PC2/NRF905_CSN/VS1053_XCS  (�͵�ƽ�������ߵ�ƽϨ��)
*/



/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//void bsp_InitLed(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStruct;

//	/* ��GPIOʱ�� */
//	LED_GPIO_CLK_ALLENABLE();
//	
//	/*
//		�������е�LEDָʾ��GPIOΪ�������ģʽ
//		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
//		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
//	*/
//	bsp_LedOff(1);
//	bsp_LedOff(2);


//	/* ����LED */
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   		/* ����������� */
//	GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* ���������費ʹ�� */
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		/* GPIO�ٶȵȼ� */

//	GPIO_InitStruct.Pin = GPIO_PIN_LED1;
//	HAL_GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStruct);

//	GPIO_InitStruct.Pin = GPIO_PIN_LED2;
//	HAL_GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStruct);
//	
//	// GPIO_InitStruct.Pin = GPIO_PIN_LED3;
//	// HAL_GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStruct);
//	
//	// GPIO_InitStruct.Pin = GPIO_PIN_LED4;
//	// HAL_GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStruct);
//}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		GPIO_PORT_LED1->BSRR = (uint32_t)GPIO_PIN_LED1 << 16U;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BSRR = (uint32_t)GPIO_PIN_LED2 << 16U;
	}
	// else if (_no == 2)
	// {
	// 	GPIO_PORT_LED3->BSRR = GPIO_PIN_LED3;
	// }
	// else if (_no == 3)
	// {
	// 	GPIO_PORT_LED4->BSRR = GPIO_PIN_LED4;
	// }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		GPIO_PORT_LED1->BSRR = (uint32_t)GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BSRR = (uint32_t)GPIO_PIN_LED2;
	}
	// else if (_no == 2)
	// {
	// 	GPIO_PORT_LED3->BSRR = (uint32_t)GPIO_PIN_LED3 << 16U;
	// }
	// else if (_no == 3)
	// {
	// 	GPIO_PORT_LED3->BSRR = (uint32_t)GPIO_PIN_LED4 << 16U;
	// }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
	}
	// else if (_no == 3)
	// {
	// 	GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;
	// }
	// else if (_no == 4)
	// {
	// 	GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;
	// }
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) == 1)
		{
			return 0;
		}
		return 1;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) == 1)
		{
			return 0;
		}
		return 1;
	}
	// else if (_no == 3)
	// {
	// 	if ((GPIO_PORT_LED3->ODR & GPIO_PIN_LED3) == 1)
	// 	{
	// 		return 1;
	// 	}
	// 	return 0;
	// }
	// else if (_no == 4)
	// {
	// 	if ((GPIO_PORT_LED4->ODR & GPIO_PIN_LED4) == 1)
	// 	{
	// 		return 1;
	// 	}
	// 	return 0;
	// }

	return 0;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/