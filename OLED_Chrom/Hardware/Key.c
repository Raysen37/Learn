#include "stm32f10x.h"                  // Device header
uint8_t Key_KeyNumber;
						//**定时器扫描按键**//
						
void GPIO_KeyInit(void)	//初始化GPIO								
{
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA), ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Timer_Init(void)//初始化定时器
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_Period=300-1;
	TimeBaseInitStructure.TIM_Prescaler=7200-1;
	TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

uint8_t Key(void)//获取按键值
{
	uint8_t	Temp;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;
	return Temp;
}

uint8_t Get_Key(void)//获取电平变化
{	
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
	{
		KeyNum=1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		KeyNum=2;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
	{
		KeyNum=4;
	}
	return KeyNum;
}

void Loop_Key(void)//扫描按键
{	
	static uint8_t LastKey=0,NowKey=0;
	LastKey=NowKey;
	NowKey=Get_Key();
	if(LastKey==1 && NowKey==0)
	{
		Key_KeyNumber=1;
	}
	if(LastKey==2 && NowKey==0)
	{
		Key_KeyNumber=2;
	}

	if(LastKey==4 && NowKey==0)
	{
		Key_KeyNumber=4;
	}
}

void Key_Init(void)//按键初始化
{	
	GPIO_KeyInit();
	Timer_Init();
}

void TIM2_IRQHandler(void)//定时器扫描（30ms）
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==1)
	{
		Loop_Key();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
