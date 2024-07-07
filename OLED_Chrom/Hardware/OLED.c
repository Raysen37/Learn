#include "stm32f10x.h"                  // Device header
#include "OLED_Font.h"
#include "stdio.h"
#include "Delay.h"
#include "stdlib.h"
#define OLED_SCL(x) 	GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)(x))
#define OLED_SDA(x)     GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)(x))
void OLED_GPIO_Init(void)	//GPIO初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	OLED_SCL(1);
	OLED_SDA(1);
}

void OLED_I2C_Start(void)	//IIC开始
{
	OLED_SCL(1);
	OLED_SDA(1);
	OLED_SDA(0);
	OLED_SCL(0);
}

void OLED_I2C_Stop(void)	//IIC结束
{
	OLED_SCL(0);
	OLED_SDA(0);
	OLED_SCL(1);
	OLED_SDA(1);
}

void OLED_I2C_SendByte(uint8_t Byte)	//IIC发送一个字节
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		OLED_SDA(Byte&(0x80>>i));
		OLED_SCL(1);
		OLED_SCL(0);
	}
	OLED_SCL(1);
	OLED_SCL(0);
}

void OLED_Write_Command(uint8_t Command)	//OLED写命令
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		
	OLED_I2C_SendByte(0x00);		
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

void OLED_Write_Data(uint8_t Data)			//OLED写数据
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		
	OLED_I2C_SendByte(0x40);		
	OLED_I2C_SendByte(Data); 
	OLED_I2C_Stop();
}

void OLED_SetCursor(uint8_t y,uint8_t x)	//设置光标位置
{
	OLED_Write_Command(0xB0|y);
	OLED_Write_Command(0x10|((x&0xF0)>>4));
	OLED_Write_Command(0x00|((x&0x0F)));
}

void OLED_Clear(void)	//OLED清屏
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		OLED_SetCursor(i,0);
		for(j=0;j<128;j++)
		{
			OLED_Write_Data(0x00);
		}
	}
}

void OLED_Fill(void)	//OLED满屏
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		OLED_SetCursor(i,0);
		for(j=0;j<128;j++)
		{
			OLED_Write_Data(0xFF);
		}
	}
}

void OLED_OPEN(void)	//打开OLED
{
	OLED_Write_Command(0x8D);
	OLED_Write_Command(0x14);
	OLED_Write_Command(0xAF);
}

void OLED_OFF(void)		//关闭OLED
{
	OLED_Write_Command(0x8D);
	OLED_Write_Command(0x10);
	OLED_Write_Command(0xAE);
}
void OLED_Char6x8(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode)	//OLED显示单个6*8字符（ascll码）
{	uint8_t j;
	chr=chr-' ';
	OLED_SetCursor(Line-1,(Column*6)-6);
	for(j=0;j<6;j++)
	{
		if(!mode)
		{
			OLED_Write_Data(asc_6x8[chr][j]);
		}
		else if(mode)
		{
			OLED_Write_Data(~asc_6x8[chr][j]);
		}
	}
}

void OLED_Char6x12(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode) //OLED显示单个6*12字符（ascll码）
{
	uint8_t i,j,u=0;
	chr=chr-' ';
	for(i=0;i<2;i++)
	{	
		OLED_SetCursor((Line*2+i)-2,(Column*6)-6);
		for(j=0;j<6;j++)
		{	
			if(!mode)
			{
			OLED_Write_Data(asc_6x12[chr][u]);
			}
			else if(mode)
			{
			OLED_Write_Data(~asc_6x12[chr][u]);
			}
			u++;
		}		
	}
}
void OLED_Char8x16(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode) //OLED显示单个8*16字符（ascll码）
{
	uint8_t i,j,u=0;
	chr=chr-' ';
	for(i=0;i<2;i++)
	{
		OLED_SetCursor((Line*2+i)-2,(Column*8)-8);
		for(j=0;j<8;j++)
		{
			if(!mode)
			{
				OLED_Write_Data(asc_8x16[chr][u]);	
			}
			else if(mode)
			{
				OLED_Write_Data(~asc_8x16[chr][u]);
			}
			u++;
		}
	}
}

void OLED_Char12x24(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode) //OLED显示单个12*24字符（ascll码）
{
	uint8_t i,j,u=0;
	chr=chr-' ';
	for(i=0;i<3;i++)
	{	OLED_SetCursor((Line+i)-1,(Column*12)-12);
		for(j=0;j<12;j++)
		{	
			if(!mode)
			{
			OLED_Write_Data(asc_12x24[chr][u]);
			}
			else if(mode)
			{
			OLED_Write_Data(~asc_12x24[chr][u]);	
			}
			u++;
		}
	}
}

void OLED_6x8String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode)//OLED显示6*8字符串（ascll码）
{
	uint8_t i;		
	
	for(i=0;String[i]!='\0';i++)
	{
		OLED_Char6x8(Line,Column+i,String[i],mode);
	}
}
void OLED_6x12String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode)//OLED显示6*12字符串（ascll码）
{
	uint8_t i;		
	
	for(i=0;String[i]!='\0';i++)
	{
		OLED_Char6x12(Line,Column+i,String[i],mode);
	}
}

void OLED_8x16String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode)//OLED显示8*16字符串（ascll码）
{
	uint8_t i;		
	for(i=0;String[i]!='\0';i++)
	{
		OLED_Char8x16(Line,Column+i,String[i],mode);
	}
}

void OLED_12x24String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode)//OLED显示12*24字符串（ascll码）
{
	uint8_t i;		
	for(i=0;String[i]!='\0';i++)
	{
		OLED_Char12x24(Line,Column+i,String[i],mode);
	}
}
void OLED_ShowString(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t size,uint8_t mode)//OLED显示各种大小字符串
{
	switch(size)
	{
		case 1:Line%=9;Column%=22;if(!Column)Column=1;OLED_6x8String(Line,Column,String,mode);break;
		case 2:Line%=5;Column%=22;if(!Line)Line=1;if(!Column)Column=1;OLED_6x12String(Line,Column,String,mode);break;	
		case 3:Line%=5;Column%=17;if(!Line)Line=1;if(!Column)Column=1;OLED_8x16String(Line,Column,String,mode);break;
		case 4:Line%=7;Column%=11;if(!Line)Line=1;if(!Column)Column=1;OLED_12x24String(Line,Column,String,mode);break;	
		default:Line%=7;Column%=11;if(!Line)Line=1;if(!Column)Column=1;OLED_12x24String(Line,Column,String,mode);OLED_12x24String(Line,Column,String,mode);break;
	}
}

void OLED_ShowChinese(uint8_t Line,uint8_t Column,uint8_t n,uint8_t mode)//OLED显示中文
{
		uint8_t i,j,u=0;
		for(i=0;i<2;i++)
		{	
			OLED_SetCursor(Line*2-2+i,Column*16-16);
			for(j=0;j<16;j++)
			{
				if(mode==0)
				{
				OLED_Write_Data(ChineseFont[n][u]);
				u++;
				}
				else
				{
				OLED_Write_Data(~ChineseFont[n][u]);
				u++;
				}
		}		
	}
}
void OLED_ShowMenuChinese(uint8_t Line,uint8_t Column,uint8_t n,uint8_t mode)//OLED显示菜单中文
{
	uint8_t i,j,u=0;
	
		for(i=0;i<2;i++)
		{	
			OLED_SetCursor(Line*2-2+i,Column*16-16);
			for(j=0;j<16;j++)
			{
				if(mode==0)
				{
				OLED_Write_Data(ChineseMenu[n][u]);
				u++;
				}
				else
				{
				OLED_Write_Data(~ChineseMenu[n][u]);
				u++;
				}
		}		
	}
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length,uint8_t size)//OLED显示各种大小数字
{
	uint8_t i;
	switch(size)
	{
		case 1:for(i=0;i<Length;i++){OLED_Char6x8(Line,Column+i,Number/OLED_Pow(10,Length-1-i)%10+'0',0);}break;
		case 2:for(i=0;i<Length;i++){OLED_Char6x12(Line,Column+i,Number/OLED_Pow(10,Length-1-i)%10+'0',0);}break;
		case 3:for(i=0;i<Length;i++){OLED_Char8x16(Line,Column+i,Number/OLED_Pow(10,Length-1-i)%10+'0',0);}break;
		case 4:for(i=0;i<Length;i++){OLED_Char12x24(Line,Column+i,Number/OLED_Pow(10,Length-1-i)%10+'0',0);}break;
		default:for(i=0;i<Length;i++){OLED_Char12x24(Line,Column+i,Number/OLED_Pow(10,Length-1-i)%10+'0',0);}break;
	}
}
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)//OLED显示字符型数字
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_Char6x12(Line, Column, '+',0);
		Number1 = Number;
	}
	else
	{
		OLED_Char6x12(Line, Column, '-',0);
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_Char6x12(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0',0);
	}
}

void OLED_ShowDainamic8x128(uint8_t Line,uint8_t n)//地面显示
{
	static uint8_t pos=0;
	uint8_t speed=5;
	uint16_t length=sizeof(Font8x192);
	uint8_t i;
	OLED_SetCursor(7,0);
	for(i=0;i<128;i++)
	{
		OLED_Write_Data(Font8x192[(i+pos)%length]);
	}
	pos=pos+speed;
}
int16_t obstacle(uint8_t ver,uint8_t reset)//障碍显示
{
	uint8_t speed=5;
	int16_t length=0;
	int16_t start_x=0;
	static int16_t pos=128;
	uint16_t u=0;
	int16_t i,j;
	if (reset == 1)
	{
		pos = 128;
		return 128;
	}
	switch(ver)
	{   case 0:length=8; break;
		case 1:length=16;break;
		default:length=24;break;
	}
	for(i=0;i<2;i++)
	{
		if(pos<0)
		{
			start_x=-pos;
			OLED_SetCursor(5+i,0);
		}
		else
		{	
			OLED_SetCursor(5+i,pos);
		}
		for(j=start_x;j<length+speed;j++)
		{
			if (pos+j>127) break;
			u=i*length+j;
			if(j<length)
			{
			switch(ver)
			{ 	case 0:OLED_Write_Data(CACTUS_1[u]);break;
				case 1:OLED_Write_Data(CACTUS_2[u]);break;
				case 2:OLED_Write_Data(CACTUS_3[u]);break;
				case 3:OLED_Write_Data(CACTUS_4[u]);break;
			}
			}else OLED_Write_Data(0x00);
		}
	}
	pos = pos - speed;
	return pos + speed;
}


void clear_jump(uint8_t Line,uint8_t flag)//flag=0为跳，flag=1为降
{
	uint8_t i;
	if(flag==0 && Line<4)
	{
		OLED_SetCursor(Line+3,16);
		for(i=0;i<16;i++)
		{
			OLED_Write_Data(0x00);
		}
	}
	else if(flag && Line>2)
	{
		OLED_SetCursor(Line-1,16);
		for(i=0;i<16;i++)
		{	
			OLED_Write_Data(0x00);
		}
	}

}

int dinojump(char reset)//恐龙动作（跳）
{	uint8_t speed[]={1,1,3,3,4,4,5,6,7};
	static int8_t spdnum=sizeof(speed)-1;
	static int16_t height=0;
	static uint8_t dire=0;
	uint8_t x,y;
	uint16_t u=0;
	uint8_t offset=0;
	if(reset == 1)
	{
		height = 0;
		dire = 0;
		spdnum = sizeof(speed)-1;
		return 0;
	}
	if(dire==0)//上升标志位
	{
		height+=speed[spdnum];//上升的速度
		spdnum--;
	}
	if(dire==1)//下降标志位
	{
		height-=speed[spdnum];//下降的速度
		spdnum++;
		if(spdnum>sizeof(speed)-1)spdnum=sizeof(speed)-1;
	}
	if(height>=31)
	{
		dire=1;
		height=31;
	}
	if(height<=0)
	{
		dire=0;
		height=0;
	}
	if(height <= 7) offset = 1;
	else if(height <= 15) offset = 2;
	else if(height <= 23) offset = 3;
	else if(height <= 31) offset = 4;
	else offset = 5;
	for(y=0;y<3;y++)
	{
		OLED_SetCursor(5-offset+y,16);
		for(x=0;x<16;x++)
		{	
			u=y*16+x;
			OLED_Write_Data(DINO_JUMP[height%8][u]);
		}
	}
	clear_jump(5-offset,dire);
	return height;
}

void OLED_ShowCloud()//飘动的云
{
	uint8_t j,lenght,speed=3,tempos=0;
	static int16_t height=0,pos=127;//显示的起始位值
	lenght=sizeof(Cloud);
	if(pos+lenght<=-speed)
	{
		pos=127;	
		height=rand()%3;
	}
	if(pos<0)
	{
		tempos=-pos;//pos中间变量
		OLED_SetCursor(height+1,0);
	}
	else
	{
		OLED_SetCursor(height+1,pos);
	}
	
	for(j=tempos;j<lenght+speed;j++)
	{
		if(pos+j>127)break;
		if(j<lenght)
		{
			OLED_Write_Data(Cloud[j]);
		}
		else OLED_Write_Data(0x00);
	}
	pos=pos-speed;
}

void dinorun()//恐龙动作（跑）
{
	static uint8_t change=0;
	uint8_t i,j,u=0;
	change++;
	change%=2;
	for(i=0;i<2;i++)
	{
		OLED_SetCursor(5+i,16);
		for(j=0;j<16;j++)
		{
		u=16*i+j;
		OLED_Write_Data(DINO[change][u]);
		}
	}	
}


void OLED_ShowDino(uint8_t Line,uint8_t Column)	//结算恐龙
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		OLED_SetCursor(Line+i,Column);
		for(j=0;j<56;j++)
		{
			OLED_Write_Data(Dino[i][j]);
		}
	}
}
void OLED_ShowBMP(void)//显示封面
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		OLED_SetCursor(i,0);
		for(j=0;j<128;j++)
		{	
			OLED_Write_Data(BMP[i][j]);
		}
	}
}
void OLED_Init(void)//OLED初始化
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			
	{
		for (j = 0; j < 1000; j++);
	}

	OLED_GPIO_Init();			

	OLED_Write_Command(0xAE);	

	OLED_Write_Command(0xD5);	
	OLED_Write_Command(0x80);

	OLED_Write_Command(0xA8);	
	OLED_Write_Command(0x3F);

	OLED_Write_Command(0xD3);	
	OLED_Write_Command(0x00);

	OLED_Write_Command(0x40);	

	OLED_Write_Command(0xA1);	
	
	OLED_Write_Command(0xC8);	

	OLED_Write_Command(0xDA);	
	OLED_Write_Command(0x12);
	
	OLED_Write_Command(0x81);	
	OLED_Write_Command(0xCF);

	OLED_Write_Command(0xD9);	
	OLED_Write_Command(0xF1);

	OLED_Write_Command(0xDB);	
	OLED_Write_Command(0x30);

	OLED_Write_Command(0xA4);	

	OLED_Write_Command(0xA6);	

	OLED_Write_Command(0x8D);	
	OLED_Write_Command(0x14);

	OLED_Write_Command(0xAF);	

	OLED_Clear();				
}






