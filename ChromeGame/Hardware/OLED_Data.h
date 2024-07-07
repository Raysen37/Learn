#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*中文字符字节宽度*/
#define OLED_CHN_CHAR_WIDTH			3		//UTF-8编码格式给3，GB2312编码格式给2

/*字模基本单元*/
typedef struct 
{
	char Index[OLED_CHN_CHAR_WIDTH + 1];	//汉字索引
	uint8_t Data[32];						//字模数据
} ChineseCell_t;

/*ASCII字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
/*按照上面的格式，在这个位置加入新的图像数据声明*/
extern const uint8_t GroundImg[1024];
extern const int8_t CloudImg[26];
extern const uint8_t DinoJump[128];
extern const uint8_t Cactus1[128];
extern const uint8_t Cactus2[64];
extern const uint8_t Cactus3[64];
extern const uint8_t DinoFoot1[30];
extern const uint8_t DinoFoot2[30];
extern const uint8_t BigDinoFoot1[128];
extern const uint8_t BigDinoFoot2[128];
extern const uint8_t Point[30];
extern const uint8_t GameOver1[145];
extern const uint8_t GameOver2[145];
extern const uint8_t GameOver3[145];
extern const uint8_t GameOver4[145];
extern const uint8_t GameOver5[145];
extern const uint8_t GameOver6[145];
extern const uint8_t DinoDown1[70];
extern const uint8_t DinoDown2[70];
extern const uint8_t DinoDown3[44];
extern const uint8_t DinoDown4[44];
extern const uint8_t CrowFly1[44];
extern const uint8_t CrowFly2[44];
//...

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
