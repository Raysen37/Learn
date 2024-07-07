#include "stm32f10x.h"
/******
计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1)
					       = CK_PSC / (PSC + 1) / (ARR + 1)
						   CK_PSC =72M

******/

/******* enter IRQ every 10ms *******/
void Timer_Init(void)
{
	/* 开时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* 时基单元配置 */
	TIM_InternalClockConfig(TIM2);  //使用内部时钟(默认)72MHz
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 -1;	//ARR自动重装器的值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;	//PSC预分频率的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;	//重复计数器，用于高级定时器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	//清除中断标志位(不清除会在初始化完成后立即进入中断)
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	//定时器中断使能，不用中断无需配置
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2, ENABLE);	//使能定时器
}
