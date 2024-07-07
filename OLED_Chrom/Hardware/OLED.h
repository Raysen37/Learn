#ifndef _OLED__H
#define _OLED__H

void OLED_GPIO_Init(void);
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
void OLED_I2C_SendByte(uint8_t Byte);
void OLED_Write_Command(uint8_t Command);
void OLED_Write_Data(uint8_t Data);
void OLED_SetCursor(uint8_t y,uint8_t x);
void OLED_Clear(void);
void OLED_Fill(void);
void OLED_Init(void);
void OLED_OPEN(void);
void OLED_OFF(void);
void OLED_Char6x8(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void OLED_Char6x12(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void OLED_Char8x16(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void OLED_Char12x24(uint8_t Line,uint8_t Column,uint8_t chr,uint8_t mode);
void OLED_6x8String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void OLED_6x12String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void OLED_8x16String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void OLED_12x24String(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t mode);
void OLED_ShowString(uint8_t Line,uint8_t Column,uint8_t *String,uint8_t size,uint8_t mode);
void OLED_ShowChinese(uint8_t Line,uint8_t Column,uint8_t n,uint8_t mode);
void OLED_ShowDainamic8x128(uint8_t Line,uint8_t n);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length,uint8_t size);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowCloud(void);
void dinorun(void);
int dinojump(char reset);
void clear_jump(void);
int obstacle(uint8_t ver,uint8_t reset);
void OLED_ShowDino(uint8_t Line,uint8_t Column);
void OLED_ShowBMP(void);
void OLED_ShowMenuChinese(uint8_t Line,uint8_t Column,uint8_t n,uint8_t mode);
#endif
