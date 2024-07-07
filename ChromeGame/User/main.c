/***************************************************************************************
  *此项目由@moyiji个人创建并免费开源，你可以查看，使用和修改，并应用到自己的项目当中
  *感谢UP主@江协科技&JL单片机
  *请注意OLED的驱动程序来源于@江协科技
  *
  *程序名称：ChromeDinoGame
  *创建时间：2024/04/13
  *作者：莫忆己（moyiji）   B站UID: 345699644
  *当前版本：V1.0
  *编码格式：UTF-8   如遇乱码请修改
  *开发环境：Keil5(库函数开发)
  *
  *项目说明：GPIOB_Pin_1 -> Down   GPIOB_Pin_11-> Jump   GPIOB_Pin_0 -> Power  ReSet -> ReStart
  *
  * 1234555
  *请注意由于个人能力和精力问题，当前版本程序十分冗杂，存在部分代码无效的情况，且存在部分Bug
  ***************************************************************************************
  */


#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"
#include "Key.h"
#include "stdlib.h"

/* 分数；速度；位置 */
uint32_t num = 0;
uint32_t score = 0;
uint8_t speed = 5;
int8_t cactus_speed = 3;
int8_t cloud_flag = 2;
int8_t pos = 0;
int8_t cactus_pos = 128;
int8_t cloud_pos = 128;
int8_t dino_jump_pos = 42;
int8_t crow_pos = 0;
int8_t high_crow_speed = 5;
int8_t high_crow_pos = 128;

/* 跳跃变化 */
int8_t jump_speed[11] = {10,16,20,23,25,26,20,13,4,0};

/* flag用于判断各种临界条件和状态 */
int y_pos = 0;
int y_pos1;
int y_pos2;
int dino_jump_flag = 1;
int dino_jump_flag_flag = 0;
int dino_jump_finish_flag = 0;
int run_flag = 1;
int key_flag = 0;
uint8_t over_flag = 0;
int barrier_flag = 0;
uint8_t barrier_flag_num = 0;
int crow_fly_flag = 1;
int oled_reverse_flag = 1;
int power_flag = 0;
int invincibl_flag = 0;
int power_timer_flag = 32;
int barrier2_end_flag = 0;

/* 按键情况 */
int KeyNum1 = 0;
int KeyNum2 = 0;
int KeyNum3 = 0;

/* 函数声明 */
uint16_t ShowDinoJump(void);
void ShowScore(void);
void ShowGameCore(void);
void ShowGroud(void);
void ShowCloud(void);
void ShowDinoRun(void);
void ShowBigDinoRun(void);
void ShowDinoDownRun(void);
void ShowGameAnimate(void);
uint8_t KeyStatus(void);
void ShowCrowFly(void);

/* 主函数循环 */
int main(void)
{
	OLED_Init();
	Key_Init();
	Timer_Init();
	while (1)
	{
		ShowGameAnimate();
		
		Delay_ms(100);     
		OLED_Update();
		OLED_Clear();
	}
}
/* 显示各种动画 */
void ShowGameAnimate(void)
{

	if(num > 100)score++; 
	ShowGroud();
	ShowCloud();
	ShowGameCore();//程序核心函数
	KeyStatus();
	ShowScore();
}

void ShowScore(void)
{
	OLED_ShowNum(1, 2, score, 5, OLED_6X8);			//显示分数，随时间变化（中断内增加）
	OLED_DrawRectangle(0, 0, 32, 10,OLED_UNFILLED);	//显示矩形方框
	/* black -> white */
	if(oled_reverse_flag == -1)OLED_Reverse();		//颜色反转	
	if(score % 100 == 0 && score>50)oled_reverse_flag = -oled_reverse_flag;
}

void ShowGroud(void)
{
	OLED_ShowNum(1, 2, score, 5, OLED_6X8);			//重复显示分数以保证gameover仍然显示
	OLED_DrawRectangle(0, 0, 32, 10,OLED_UNFILLED);
	OLED_ShowGround(0, 56, 128, 8, GroundImg, pos, speed);	//显示地面，变动ground数组数据	
	pos = pos + speed;
}
void ShowCloud(void)
{
	if(cloud_pos <= 0)cloud_pos = 128;	
	OLED_ShowCloud(cloud_pos, 8, 26, 8, CloudImg);
	if(cloud_pos <= 36)OLED_ClearArea(0, 0, 6, 8);	//清除部分区域不遮盖分数显示 
}

void ShowDinoRun(void)
{
	if(run_flag == 1)OLED_ShowDinoRun(16, 42, 15, 16, DinoFoot2);
	if(run_flag == -2)OLED_ShowDinoRun(16, 42, 15, 16, DinoFoot1);
	run_flag = ~run_flag;	
}

void ShowBigDinoRun(void)
{ 
	if(run_flag == 1)OLED_ShowDinoRun(16, 26, 29, 32, BigDinoFoot2);
	if(run_flag == -2)OLED_ShowDinoRun(16, 26, 29, 32, BigDinoFoot1);
	run_flag = ~run_flag;
}

void ShowDinoDownRun(void)
{
	OLED_ClearArea(16, 42, 15, 16);				//清除部分区域不遮盖BigDino	
		if(run_flag == 1)OLED_ShowDinoRun(16, 48, 22, 10, DinoDown3);
		if(run_flag == -2)OLED_ShowDinoRun(16, 48, 22, 10, DinoDown4);
//	run_flag = ~run_flag;	
}

/* 传入jump_speed[]以变动Y轴数据 */
uint16_t ShowDinoJump(void)
{
	dino_jump_pos = 42 - jump_speed[dino_jump_flag];
	switch(dino_jump_flag)
	{
		case 1: OLED_ShowDinoJump(16, 42-jump_speed[0], 14, 16, DinoJump);break;
		case 2:	OLED_ShowDinoJump(16, 42-jump_speed[1], 14, 16, DinoJump);break;
		case 3: OLED_ShowDinoJump(16, 42-jump_speed[2], 14, 16, DinoJump);break;
		case 4: OLED_ShowDinoJump(16, 42-jump_speed[3], 14, 16, DinoJump);break;
		case 5: OLED_ShowDinoJump(16, 42-jump_speed[4], 14, 16, DinoJump);break;
		case 6: OLED_ShowDinoJump(16, 42-jump_speed[5], 14, 16, DinoJump);break;
		case 7: OLED_ShowDinoJump(16, 42-jump_speed[6], 14, 16, DinoJump);break;
		case 8: OLED_ShowDinoJump(16, 42-jump_speed[7], 14, 16, DinoJump);break;
		case 9: OLED_ShowDinoJump(16, 42-jump_speed[8], 14, 16, DinoJump);break;
		case 10: OLED_ShowDinoJump(16, 42-jump_speed[9], 14, 16, DinoJump);break;
		case 11: OLED_ShowDinoJump(16, 42-jump_speed[10], 14, 16, DinoJump);dino_jump_flag=0;break;
	}
	dino_jump_flag++;
	return dino_jump_pos;
}

/* 按键状态变换 */
uint8_t KeyStatus(void)
{
	
	if(Key_GetNum()==1)KeyNum1 = 1;	
	if(Key_GetNum()==2)KeyNum2 = 2;
	if(Key_GetNum()==3)KeyNum3 = 3;

	return KeyNum1;
}

/* 核心函数：随机障碍显示；按键状态变动恐龙状态（奔跑，跳跃，爬行，变大）；碰撞检测；Power状态 */
void ShowGameCore(void)
{
	/* power */
	if(score % 100 == 0 && score>50)power_flag = 1;		//激活power_flag

	cactus_pos = 128 - pos;		//障碍位置
	crow_pos = 128 - pos;
	//crow flag
	crow_fly_flag = ~crow_fly_flag;  //乌鸦飞行flag
	//key detect
	if(Key_GetNum()== 2)
	{
		dino_jump_finish_flag = 1;	//进入跳跃状态
	}
	/* run status */
	if(dino_jump_finish_flag == 0)	//进入奔跑状态
	{
		barrier_flag_num = pos;
		if(barrier_flag_num >128)	//随机显示障碍
		{	
			barrier_flag_num = 0;
			pos = 0;
			barrier_flag = rand()%4;
		}
		
		if(barrier_flag == 2)barrier2_end_flag = 1;		//高空乌鸦飞行结束flag
		else barrier2_end_flag = 0;
		
		switch(barrier_flag)		//显示障碍
		{
			case 0: OLED_ShowCactus(cactus_pos, 42, 8 ,17, Cactus1);
				break;
			case 1: OLED_ShowCactus(cactus_pos, 42, 15 ,16, Cactus3);
				break;
			case 2:
				high_crow_pos = 128 - pos;
				if(crow_fly_flag == 1)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly1);
			    if(crow_fly_flag == -2)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly2);
				break;
			case 3:	
				if(crow_fly_flag == 1)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly1);
				if(crow_fly_flag == -2)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly2);
				break;
		}
		ShowDinoRun();				//奔跑
	}
	
	/* down run status */
	if(Key_GetNum()== 1 &&	dino_jump_finish_flag != 1)		//进入下趴爬行状态
	{
		OLED_ClearArea(16, 42, 15, 16);						//清除奔跑状态Dino
		barrier_flag_num = pos;
		if(barrier_flag_num >128)
		{	
			barrier_flag_num = 0;
			barrier_flag = rand()%4;
		}
		switch(barrier_flag)
		{
			case 0: OLED_ShowCactus(cactus_pos, 42, 8 ,17, Cactus1);
				break;
			case 1: OLED_ShowCactus(cactus_pos, 42, 15 ,16, Cactus3);
				break;
			case 2:
				high_crow_pos = 128 - pos;
				if(crow_fly_flag == 1)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly1);
				if(crow_fly_flag == -2)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly2);
				break;
			case 3:	
				if(crow_fly_flag == 1)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly1);
				if(crow_fly_flag == -2)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly2);
				break;
		}
		ShowDinoDownRun();			//下趴爬行
	}
	/* game over 判断临界状态 游戏结束 */
	if(barrier_flag != 2 && dino_jump_finish_flag != 1 && barrier2_end_flag != 1 && invincibl_flag == 0) //非高空障碍
	{
		if((cactus_pos >=8 && cactus_pos <= 16) || (crow_pos >= 8 && crow_pos <= 16))
		{
			while(1)
			{
				switch(over_flag)
				{
					case 1: OLED_ShowGameOver(46, 16, 36, 32,GameOver1, over_flag);break;
					case 2: OLED_ShowGameOver(46, 16, 36, 32,GameOver2, over_flag);break;
					case 3: OLED_ShowGameOver(46, 16, 36, 32,GameOver3, over_flag);break;
					case 4: OLED_ShowGameOver(46, 16, 36, 32,GameOver4, over_flag);break;
					case 5: OLED_ShowGameOver(46, 16, 36, 32,GameOver5, over_flag);break;
					case 6: OLED_ShowGameOver(46, 16, 36, 32,GameOver6, over_flag);break;
				}
				over_flag++;
				Delay_ms(25);     
				OLED_Update();
			}	
		}
	}

	if(barrier_flag == 2 && dino_jump_finish_flag != 1 && invincibl_flag == 0)	//高空障碍
	{
		if((high_crow_pos >= 8 && high_crow_pos <= 16) && Key_GetNum() != 1)
		{
			while(1)
			{
				switch(over_flag)
				{
					case 1: OLED_ShowGameOver(46, 16, 36, 32,GameOver1, over_flag);break;
					case 2: OLED_ShowGameOver(46, 16, 36, 32,GameOver2, over_flag);break;
					case 3: OLED_ShowGameOver(46, 16, 36, 32,GameOver3, over_flag);break;
					case 4: OLED_ShowGameOver(46, 16, 36, 32,GameOver4, over_flag);break;
					case 5: OLED_ShowGameOver(46, 16, 36, 32,GameOver5, over_flag);break;
					case 6: OLED_ShowGameOver(46, 16, 36, 32,GameOver6, over_flag);break;
				}
				over_flag++;
				Delay_ms(25);     
				OLED_Update();
			}	
		}
	}
	
	/* jump status */
	if(dino_jump_finish_flag == 1)		//进入跳跃状态
	{	
		//jumping game over
		if(dino_jump_pos >= 32 && (cactus_pos < 30 && cactus_pos > 10) && invincibl_flag == 0)
		{
			while(1 && dino_jump_finish_flag == 1)
			{
				OLED_ShowNum(1, 2, score, 5, OLED_6X8);
				OLED_DrawRectangle(0, 0, 32, 10,OLED_UNFILLED);
				switch(over_flag)
				{
					case 1: OLED_ShowGameOver(46, 16, 36, 32,GameOver1, over_flag);break;
					case 2: OLED_ShowGameOver(46, 16, 36, 32,GameOver2, over_flag);break;
					case 3: OLED_ShowGameOver(46, 16, 36, 32,GameOver3, over_flag);break;
					case 4: OLED_ShowGameOver(46, 16, 36, 32,GameOver4, over_flag);break;
					case 5: OLED_ShowGameOver(46, 16, 36, 32,GameOver5, over_flag);break;
					case 6: OLED_ShowGameOver(46, 16, 36, 32,GameOver6, over_flag);					
				}
				over_flag++;
				Delay_ms(25);     
				OLED_Update();	
				/* reload */
			}
		}	
		dino_jump_pos = 42 - jump_speed[dino_jump_flag];
		barrier_flag_num = pos;
		if(barrier_flag_num >144)
		{	
			pos = 0;
			barrier_flag_num = 0;
			barrier_flag = rand()%4;
		}
		/* rand show barrier */
		if(barrier_flag == 0)
		{			
				OLED_ShowCactus(cactus_pos, 42, 8 ,17, Cactus1);
		}
		if(barrier_flag == 1)OLED_ShowCactus(cactus_pos, 42, 15 ,16, Cactus3);
		if(barrier_flag == 2)
		{
			high_crow_pos = 128 - pos;
			if(crow_fly_flag == 1)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly1);
			if(crow_fly_flag == -2)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly2);
		}
		if(barrier_flag == 3)
		{
			if(crow_fly_flag == 1)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly1);
			if(crow_fly_flag == -2)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly2);
		}
		
		switch(dino_jump_flag)	//显示跳跃过程
		{
			case 1: OLED_ShowDinoJump(16, 42-jump_speed[0], 14, 16, DinoJump);break;
			case 2:	OLED_ShowDinoJump(16, 42-jump_speed[1], 14, 16, DinoJump);break;
			case 3: OLED_ShowDinoJump(16, 42-jump_speed[2], 14, 16, DinoJump);break;
			case 4: OLED_ShowDinoJump(16, 42-jump_speed[3], 14, 16, DinoJump);break;
			case 5: OLED_ShowDinoJump(16, 42-jump_speed[4], 14, 16, DinoJump);break;
			case 6: OLED_ShowDinoJump(16, 42-jump_speed[5], 14, 16, DinoJump);break;
			case 7: OLED_ShowDinoJump(16, 42-jump_speed[6], 14, 16, DinoJump);break;
			case 8: OLED_ShowDinoJump(16, 42-jump_speed[7], 14, 16, DinoJump);break;
			case 9: OLED_ShowDinoJump(16, 42-jump_speed[8], 14, 16, DinoJump);break;
			case 10: OLED_ShowDinoJump(16, 42-jump_speed[9], 14, 16, DinoJump);break;
			case 11: OLED_ShowDinoJump(16, 42-jump_speed[10], 14, 16, DinoJump);dino_jump_flag=0;dino_jump_finish_flag = 0;break;
		}
		dino_jump_flag++;
		if(barrier_flag != 2 && ((42-jump_speed[dino_jump_flag]) >= 40) && invincibl_flag == 0)
		{
			if((crow_pos >= 8 && crow_pos <= 16))
			{
				while(1)
				{
					switch(over_flag)
					{
						case 1: OLED_ShowGameOver(46, 16, 36, 32,GameOver1, over_flag);break;
						case 2: OLED_ShowGameOver(46, 16, 36, 32,GameOver2, over_flag);break;
						case 3: OLED_ShowGameOver(46, 16, 36, 32,GameOver3, over_flag);break;
						case 4: OLED_ShowGameOver(46, 16, 36, 32,GameOver4, over_flag);break;
						case 5: OLED_ShowGameOver(46, 16, 36, 32,GameOver5, over_flag);break;
						case 6: OLED_ShowGameOver(46, 16, 36, 32,GameOver6, over_flag);break;
					}
					over_flag++;
					Delay_ms(25);     
					OLED_Update();
				}	
			}
		}
		if(barrier_flag == 2 && dino_jump_finish_flag != 1 && invincibl_flag == 0)
		{
			if((high_crow_pos >= 8 && high_crow_pos <= 16) && Key_GetNum() != 1)
			{
				while(1)
				{
					switch(over_flag)
					{
						case 1: OLED_ShowGameOver(46, 16, 36, 32,GameOver1, over_flag);break;
						case 2: OLED_ShowGameOver(46, 16, 36, 32,GameOver2, over_flag);break;
						case 3: OLED_ShowGameOver(46, 16, 36, 32,GameOver3, over_flag);break;
						case 4: OLED_ShowGameOver(46, 16, 36, 32,GameOver4, over_flag);break;
						case 5: OLED_ShowGameOver(46, 16, 36, 32,GameOver5, over_flag);break;
						case 6: OLED_ShowGameOver(46, 16, 36, 32,GameOver6, over_flag);break;
					}
					over_flag++;
					Delay_ms(25);     
					OLED_Update();
				}	
			}
		}
	}
	/********************* big dino *********************/
	if(power_flag == 1 && KeyNum3 == 3)		//变大状态检测
	{
		dino_jump_finish_flag = 3;
		invincibl_flag = 1;		//大招开始标志
		power_timer_flag--;
		
		barrier_flag_num = pos;
		if(barrier_flag_num >128)
		{	
			barrier_flag_num = 0;
			barrier_flag = rand()%4;
		}
		switch(barrier_flag)
		{
			case 0: OLED_ShowCactus(cactus_pos, 42, 8 ,17, Cactus1);
				break;
			case 1: OLED_ShowCactus(cactus_pos, 42, 15 ,16, Cactus3);
				break;
			case 2:
				high_crow_pos = 128 - pos;
				if(crow_fly_flag == 1)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly1);
				if(crow_fly_flag == -2)OLED_ShowCrowFly(high_crow_pos, 29, 22, 16,CrowFly2);
				break;
			case 3:	
				if(crow_fly_flag == 1)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly1);
				if(crow_fly_flag == -2)OLED_ShowCrowFly(crow_pos, 42, 22, 16,CrowFly2);
				break;
		}
		if(power_timer_flag == 0)
		{
			power_timer_flag = 32;
			power_flag = 0;
			KeyNum3 = 0;
			invincibl_flag = 0;			//大招结束标志
			dino_jump_finish_flag = 0;	//进入奔跑状态
		}
		if(cactus_pos >= 16 && cactus_pos <= 35 && invincibl_flag != 0)OLED_ClearArea(16, 26, 29, 32); 	//临界状态检测
		ShowBigDinoRun();		//大恐龙奔跑
	}
	pos = pos + cactus_speed;		//位置=位置+速度
}

void ShowCrowFly(void)	//乌鸦飞行
{
	crow_pos = 128 - pos;
	
	OLED_ShowCrowFly(crow_pos, 36, 22, 16,CrowFly1);
	
	pos = pos + cactus_speed;
}

/*  IRQ service program */
void TIM2_IRQHandler(void)	//中断程序
{
	/* enter IRQ every 10ms */
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET) 
	{
		KeyNum1 = Key_GetNum();		//实时获取按键状态并赋值给KeyNum1
		cloud_flag--;				//云彩标志位自减以缓慢移动
		if(cloud_flag == 0)
		{
			cloud_pos--;
			cloud_flag = 2;
		}
		num ++;
		uint8_t Dino_Count;
		Dino_Count++;
		if(Dino_Count == 50)		//留出足够时间以完成跳跃
		{
			if(dino_jump_finish_flag == 1)
			{
				dino_jump_flag++;
				if(dino_jump_flag == 11)dino_jump_finish_flag=0;
			}
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	 
	}
}

