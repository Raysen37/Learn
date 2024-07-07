//***************************STM32恐龙快跑游戏***************************//
//***************B站视频中使用芯片STM32f103C8T6最小系统板****************//
							//**初始按键连接引脚**//
							//*跳跃键：GPIOB_Pin_6*//
							//*菜单键：GPIOB_Pin_9*//
							//*确定键：GPIOB_Pin_12*//
					//*注：菜单页面只能在未进入游戏时开启*//
					   //**0.96寸OLED屏幕初始线路连接**//
							 //*SCL：GPIOA_Pin_11*//
							 //*SDA：GPIOA_Pin_12*//
					   //*蜂鸣器信号输出引脚GPIOA_Pin_1*//
				//*一键三连激励作者更新更多有趣的开源作品吧！！！*//	
//视频链接：https://www.bilibili.com/video/BV1Mx4y1u7iQ/?spm_id_from=333.999.0.0&vd_source=9a11174c62398fc8d596888a6f9a1d45
							//*单片机交流群：883948120*//											
							//*作者:科技骨（BiliBili）*//
#include "stm32f10x.h"             
#include "OLED.H"
#include "Key.H"
#include "Delay.h" 
#include "stdio.h"
#include "stdlib.h"
int16_t obstacle_pos=128;//障碍物位置
uint16_t Hs;//最高分
uint16_t score;//当前分数
uint16_t obstacle_ran;//随机植物长度
uint16_t height;//恐龙跳跃高度
uint8_t Menufalg;//菜单标志位
uint8_t MenuSelect;//菜单选择位
uint8_t MenuShow;//菜单显示标志位
uint8_t reset;//重置标志位
uint8_t obstacle_length;//植物长度
uint8_t bmpflag;//显示图片标志位
uint8_t playflag;//游戏、菜单标志位
uint8_t menuclear;//菜单清屏
uint8_t ClearF;//清屏标志位
uint8_t beingspeed;//游戏起始速度
uint8_t mode;//菜单模式
uint8_t Key4;//计KeyNum=4的按下次数
uint8_t speedmax=36;//速度最大值
uint8_t FMQswitch;//蜂鸣器开关
uint8_t KeyNum;//按键值
uint8_t t;//进入次数
uint8_t roll;
uint8_t Speed;//游戏速度
uint8_t deadflag;//死亡标志位
uint8_t begingtemp;//开始速度中间变量
uint8_t speedtep=9;//最大速度中间变量
uint8_t Settlement;//结算动画标志位
void Buzzer_Init()//蜂鸣器初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void Buzzer(uint8_t SW)//蜂鸣器控制
{
	if(SW)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
	}
}
void bostacle_refresh(void)//随机障碍
{
	obstacle_pos=obstacle(obstacle_ran,0);
	if(obstacle_pos+obstacle_length<=0)
	{
	obstacle_ran=rand()%4;
	obstacle_pos=obstacle(obstacle_ran,1);	
	}

	switch (obstacle_ran)//随机三种不同宽度障碍
	{
		case 0: obstacle_length=8;break;
		case 1: obstacle_length=16;break;
		default: obstacle_length=24;break;
	}			
}

void Show_other(void)//显示云、地面、分数
{	
	OLED_ShowString(1,14,"SC:",1,0);
	OLED_ShowString(1,0,"HS:",1,0);
	score++;
	if(score%100==0)Buzzer(1);
	else Buzzer(0);
	OLED_ShowDainamic8x128(7,roll);
	OLED_ShowCloud();
	OLED_ShowNum(1,17,score,5,1);
	OLED_ShowNum(1,4,Hs,5,1);
}
void loop_dead(void)//检测恐龙是否撞到障碍
{
	if((height<=16) && ((obstacle_pos>=16&&obstacle_pos<=32)||(obstacle_pos+obstacle_length>=16 && obstacle_pos+obstacle_length<=32)))deadflag=1;
	if(height<=0)KeyNum=Key();
	if(KeyNum==1 || height>0)
	{
		height=dinojump(0);
	}
	else dinorun();
}

const uint8_t close[]=
{
	3,2,1,1,1,1,2,3
};

void OLED_DeadAnimation(uint8_t newbest)//结算动画页面
{
	uint8_t i,j;
	if(Key()==4)reset=4;
	if(t==0)
	{	if(!Settlement)
		{
			for(i=0;i<8;i++)
			{
				OLED_SetCursor(i,0);
				for(j=0;j<128;j++)
				{
					OLED_Write_Data(0xFF);
					Delay_ms(close[i]);
				}
			}			
		}
		OLED_Clear();
		if(newbest==0)
		{
		OLED_ShowChinese(2,1,2,0);
		OLED_ShowChinese(2,2,3,0);
		OLED_ShowChinese(2,3,4,0);
		OLED_ShowString(2,7,"...",3,0);		
		}
		else 
		{
		OLED_ShowChinese(2,1,7,0);
		OLED_ShowChinese(2,2,8,0);
		OLED_ShowChinese(2,3,9,0);
		OLED_ShowString(2,7,"!!!",3,0);			
		}

		OLED_ShowString(7,2,"SC:",1,0);
		OLED_ShowNum(7,5,score,5,1);
		OLED_ShowDino(0,70);
		t++;
	}
}
void live(void)//游戏页面
{
	bostacle_refresh();
	loop_dead();
	Show_other();
	Speed=(score/30)+beingspeed;
	if(Speed>=speedmax)Speed=speedmax;
	Delay_ms(37-Speed);	
}
void die(void)//碰撞障碍后
{	
	uint8_t nowbest=0;
	if(score>Hs)nowbest=1;
	else nowbest=0;
	OLED_DeadAnimation(nowbest);	
	if(reset==4)
	{
		deadflag=0;
		height=0;
		obstacle(obstacle_ran,1);
		height=dinojump(1);
		t=0;
		reset=0;
		OLED_Clear();
		if(nowbest)Hs=score;
		score=0;
	}	
}

void Menu1(void)//返回游戏
{
	if(KeyNum==4)
		{
		mode=7;
		playflag=0;
		bmpflag=0;
		MenuShow=0;
		MenuSelect=0;
		menuclear=0;	
		OLED_Clear();
		}
}
void Menu2(void)//设置初始速度
{	
	OLED_ShowMenuChinese(1,1,4,0);OLED_ShowMenuChinese(1,2,5,0);OLED_ShowMenuChinese(1,3,6,0);OLED_ShowMenuChinese(1,4,7,0);//初始速度
	OLED_ShowNum(3,8,begingtemp,1,3);
	if(KeyNum==1){begingtemp++;begingtemp%=10;beingspeed=begingtemp*3;};
	if(KeyNum==4){Key4++;if(Key4>1){mode=7;Key4=0;}OLED_Clear();}
}
void Menu3(void)//设置速度上限
{
	OLED_ShowMenuChinese(1,1,8,0);OLED_ShowMenuChinese(1,2,9,0);OLED_ShowMenuChinese(1,3,10,0);OLED_ShowMenuChinese(1,4,11,0);
	OLED_ShowNum(3,8,speedtep,1,3);
	if(KeyNum==1){speedtep--;if(speedtep<=1)speedtep=1;speedmax=speedtep*4;}
	if(KeyNum==4){Key4++;if(Key4>1){mode=7;Key4=0;}OLED_Clear();}	
}
void Menu4(void)//蜂鸣器开关
{
	OLED_ShowMenuChinese(1,1,12,0);OLED_ShowMenuChinese(1,2,13,0);OLED_ShowMenuChinese(1,3,14,0);OLED_ShowMenuChinese(1,4,15,0);OLED_ShowMenuChinese(1,5,16,0);
	if(FMQswitch==0)
	{
		OLED_ShowString(3,7,"ON ",3,0);
	}
	else 
	OLED_ShowString(3,7,"OFF ",3,0);
	if(KeyNum==1){FMQswitch++;FMQswitch%=2;}
	if(KeyNum==4){Key4++;if(Key4>1){mode=7;Key4=0;}OLED_Clear();}	
}
void Menu5(void)//结算动画开关
{
	OLED_ShowMenuChinese(1,1,17,0);OLED_ShowMenuChinese(1,2,18,0);OLED_ShowMenuChinese(1,3,19,0);OLED_ShowMenuChinese(1,4,20,0);OLED_ShowMenuChinese(1,5,21,0);OLED_ShowMenuChinese(1,6,22,0);
	if(FMQswitch==0)
	{
		OLED_ShowString(3,7,"ON ",3,0);
	}
	else 
	{
	OLED_ShowString(3,7,"OFF",3,0);
	Settlement=~Settlement;
	}
	if(KeyNum==1){FMQswitch++;FMQswitch%=2;}
	if(KeyNum==4){Key4++;if(Key4>1){mode=7;Key4=0;}OLED_Clear();}	
}
void Menu6(void)//关机
{
	OLED_OFF();
}

void Menu(void)//菜单
{
	KeyNum=Key();
	if(KeyNum==2)
	{
		playflag=0;
		bmpflag=0;
	}
	if(menuclear==0)
	{
	OLED_Clear();menuclear++;
	}
	
	if(!MenuShow)
	{
		if(KeyNum==1)
		{	
			MenuSelect++;
			MenuSelect%=6;
			OLED_Clear();
		}
		OLED_ShowChinese(1,4,0,0);
		OLED_ShowChinese(1,5,1,0);
		switch(MenuSelect)
		{
		case 0:if(MenuSelect==0){
								OLED_ShowString(2,1,"1.",3,1);OLED_ShowMenuChinese(2,2,0,1);OLED_ShowMenuChinese(2,3,1,1);OLED_ShowMenuChinese(2,4,2,1);OLED_ShowMenuChinese(2,5,3,1);//返回游戏
								OLED_ShowString(3,1,"2.",3,0);OLED_ShowMenuChinese(3,2,4,0);OLED_ShowMenuChinese(3,3,5,0);OLED_ShowMenuChinese(3,4,6,0);OLED_ShowMenuChinese(3,5,7,0);//初始速度
								OLED_ShowString(4,1,"3.",3,0);OLED_ShowMenuChinese(4,2,8,0);OLED_ShowMenuChinese(4,3,9,0);OLED_ShowMenuChinese(4,4,10,0);OLED_ShowMenuChinese(4,5,11,0);//速度上限
								}
				else {OLED_ShowString(2,1,"1.",3,0);OLED_ShowMenuChinese(2,2,0,0);OLED_ShowMenuChinese(2,3,1,0);OLED_ShowMenuChinese(2,4,2,0);OLED_ShowMenuChinese(2,5,3,0);}break;//返回游戏}
		case 1:if(MenuSelect==1){
								OLED_ShowString(2,1,"1.",3,0);OLED_ShowMenuChinese(2,2,0,0);OLED_ShowMenuChinese(2,3,1,0);OLED_ShowMenuChinese(2,4,2,0);OLED_ShowMenuChinese(2,5,3,0);//返回游戏
								OLED_ShowString(3,1,"2.",3,1);OLED_ShowMenuChinese(3,2,4,1);OLED_ShowMenuChinese(3,3,5,1);OLED_ShowMenuChinese(3,4,6,1);OLED_ShowMenuChinese(3,5,7,1);//初始速度
								OLED_ShowString(4,1,"3.",3,0);OLED_ShowMenuChinese(4,2,8,0);OLED_ShowMenuChinese(4,3,9,0);OLED_ShowMenuChinese(4,4,10,0);OLED_ShowMenuChinese(4,5,11,0);//速度上限
								}
				else {OLED_ShowString(3,1,"2.",3,0);OLED_ShowMenuChinese(3,2,4,0);OLED_ShowMenuChinese(3,3,5,0);OLED_ShowMenuChinese(3,4,6,0);OLED_ShowMenuChinese(3,5,7,0);}break;//返回游戏}
		case 2:if(MenuSelect==2){
								OLED_ShowString(2,1,"1.",3,0);OLED_ShowMenuChinese(2,2,0,0);OLED_ShowMenuChinese(2,3,1,0);OLED_ShowMenuChinese(2,4,2,0);OLED_ShowMenuChinese(2,5,3,0);//返回游戏
								OLED_ShowString(3,1,"2.",3,0);OLED_ShowMenuChinese(3,2,4,0);OLED_ShowMenuChinese(3,3,5,0);OLED_ShowMenuChinese(3,4,6,0);OLED_ShowMenuChinese(3,5,7,0);//初始速度
								OLED_ShowString(4,1,"3.",3,1);OLED_ShowMenuChinese(4,2,8,1);OLED_ShowMenuChinese(4,3,9,1);OLED_ShowMenuChinese(4,4,10,1);OLED_ShowMenuChinese(4,5,11,1);//速度上限
								}
				else {OLED_ShowString(4,1,"3.",3,0);OLED_ShowMenuChinese(4,2,8,0);OLED_ShowMenuChinese(4,3,9,0);OLED_ShowMenuChinese(4,4,10,0);OLED_ShowMenuChinese(4,5,11,0);};break;//返回游戏}
		case 3:if(MenuSelect==3){
								OLED_ShowString(2,1,"4.",3,1);OLED_ShowMenuChinese(2,2,12,1);OLED_ShowMenuChinese(2,3,13,1);OLED_ShowMenuChinese(2,4,14,1);OLED_ShowMenuChinese(2,5,15,1);OLED_ShowMenuChinese(2,6,16,1);//蜂鸣器开关
								OLED_ShowString(3,1,"5.",3,0);OLED_ShowMenuChinese(3,2,17,0);OLED_ShowMenuChinese(3,3,18,0);OLED_ShowMenuChinese(3,4,19,0);OLED_ShowMenuChinese(3,5,20,0);OLED_ShowMenuChinese(3,6,21,0);OLED_ShowMenuChinese(3,7,22,0);//结算动画开关
								OLED_ShowString(4,1,"6.",3,0);OLED_ShowMenuChinese(4,2,23,0);OLED_ShowMenuChinese(4,3,24,0);//关机
								}
				else {OLED_ShowString(2,1,"4.",3,0);OLED_ShowMenuChinese(2,2,12,0);OLED_ShowMenuChinese(2,3,13,0);OLED_ShowMenuChinese(2,2,14,0);OLED_ShowMenuChinese(2,3,15,1);OLED_ShowMenuChinese(2,3,16,0);}break;
		case 4:if(MenuSelect==4){
								OLED_ShowString(2,1,"4.",3,0);OLED_ShowMenuChinese(2,2,12,0);OLED_ShowMenuChinese(2,3,13,0);OLED_ShowMenuChinese(2,4,14,0);OLED_ShowMenuChinese(2,5,15,0);OLED_ShowMenuChinese(2,6,16,0);//蜂鸣器开关
								OLED_ShowString(3,1,"5.",3,1);OLED_ShowMenuChinese(3,2,17,1);OLED_ShowMenuChinese(3,3,18,1);OLED_ShowMenuChinese(3,4,19,1);OLED_ShowMenuChinese(3,5,20,1);OLED_ShowMenuChinese(3,6,21,1);OLED_ShowMenuChinese(3,7,22,1);//结算动画开关
								OLED_ShowString(4,1,"6.",3,0);OLED_ShowMenuChinese(4,2,23,0);OLED_ShowMenuChinese(4,3,24,0);//关机
								}
				else {OLED_ShowString(3,1,"5.",3,0);OLED_ShowMenuChinese(3,2,17,0);OLED_ShowMenuChinese(3,3,18,0);OLED_ShowMenuChinese(3,4,19,0);OLED_ShowMenuChinese(3,5,20,0);OLED_ShowMenuChinese(3,6,21,0);OLED_ShowMenuChinese(3,7,22,0);}break;//结算动画开关
		case 5:if(MenuSelect==5){
								OLED_ShowString(2,1,"4.",3,0);OLED_ShowMenuChinese(2,2,12,0);OLED_ShowMenuChinese(2,3,13,0);OLED_ShowMenuChinese(2,4,14,0);OLED_ShowMenuChinese(2,5,15,0);OLED_ShowMenuChinese(2,6,16,0);//蜂鸣器开关
								OLED_ShowString(3,1,"5.",3,0);OLED_ShowMenuChinese(3,2,17,0);OLED_ShowMenuChinese(3,3,18,0);OLED_ShowMenuChinese(3,4,19,0);OLED_ShowMenuChinese(3,5,20,0);OLED_ShowMenuChinese(3,6,21,0);OLED_ShowMenuChinese(3,7,22,0);//结算动画开关
								OLED_ShowString(4,1,"6.",3,1);OLED_ShowMenuChinese(4,2,23,1);OLED_ShowMenuChinese(4,3,24,1);//关机
								}
				else {OLED_ShowString(4,1,"6.",3,0);OLED_ShowMenuChinese(4,2,23,0);OLED_ShowMenuChinese(4,3,24,0);}break;//关机
		}
	}
		if(KeyNum==4)
		{	
			if(ClearF==0)
			{
				ClearF++;
				OLED_Clear();
			}
			switch(MenuSelect)
			{
				case 0:mode=1;break;
				case 1:mode=2;break;
				case 2:mode=3;break;
				case 3:mode=4;break;
				case 4:mode=5;break;
				case 5:mode=6;break;
			}
		}		
				switch(mode)
			{
				case 1:Menu1();MenuShow=1;break;
				case 2:Menu2();MenuShow=1;break;
				case 3:Menu3();MenuShow=1;break;
				case 4:Menu4();MenuShow=1;break;
				case 5:Menu5();MenuShow=1;break;
				case 6:Menu6();MenuShow=1;break;
				case 7:MenuShow=0;break;
				default :break;
			}
}
int main(void)
{	
	Key_Init();
	OLED_Init();
	Buzzer_Init();
	end:
	while(1)
	{	
		if(playflag==0)
		{	
			if(bmpflag==0)
			{
			OLED_ShowBMP();
			bmpflag++;			
			}
			KeyNum=Key();
			if(KeyNum==4)
			{
				playflag=1;
				OLED_Clear();
			}
			if(KeyNum==2)
			{
				playflag=2;
			}
		}
		else if(playflag==1)
		{
			if(!deadflag){live();}else{die();goto end;}

		}
		else if(playflag==2)
		{
			Menu();
		}
	}
}















