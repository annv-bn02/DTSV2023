#ifndef _ILI9341_GUI_H_
#define _ILI9341_GUI_H_

#include <stm32f10x.h>


void DrawPoint(u16 x, u16 y);
static void _swap(u16 *a, u16 *b);

void TFT_Clear(u16 Color);
void TFT_Gui_Direction(u8 direction);
void TFT_Gui_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);
void TFT_SetCursor(u16 Xpos, u16 Ypos);
void TFT_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void TFT_Gui_DrawPoint(u16 x,u16 y,u32 color);
void TFT_Draw_Circle_8(int xc, int yc, int x, int y, u16 c);
void TFT_Gui_Circle(int xc, int yc,u16 c,int r, int fill);
void TFT_Gui_Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void ILI9431_Gui_Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void TFT_Gui_Img(u16 x,u16 y, u16 picxel_X, u16 picxel_Y ,const unsigned char *p);

void TFT_Show_6x8_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);						/*SIZE 8*/
void TFT_Show_6x12_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);					/*SIZE 12*/
void TFT_Show_8x16_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);					/*SIZE 16*/
void TFT_Show_12x24_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);					/*SIZE 24*/
void TFT_Show_16x32_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);					/*SIZE 32*/
void TFT_Show_24x48_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);					/*SIZE 48*/
void TFT_Print(u16 x, u16 y, u16 fc, u16 bc, u8 size, u8 *p, u8 mode);		/*HIEN THI CHU THUONG*/

void TFT_Show_8x16B_Char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);					/*SIZE 16 - BOLD*/
void TFT_Show_12x24B_Char(u16 x, u16 y, u16 fc, u16 bc, u8 num, u8 mode);			/*SIZE 24 - BOLD*/
void TFT_Show_16x32B_char(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 mode);				/*SIZE 32 - BOLD*/
void TFT_Print_Bold(u16 x, u16 y, u16 fc, u16 bc, u8 size, u8 *p, u8 mode);/*HIEN THI CHU IN DAM*/


u32 mypow(u8 m,u8 n);
void TFT_Print_Num(u16 x, u16 y, u16 fc, u16 bc, u8 size, u8 bold, u32 num, u8 len, u8 mode);

void TFT_Print_char_Center(u16 x1, u16 y1, u16 x2, u16 y2, u16 fc, u16 bc, u8 size, u8 type_text, u8 *str, u8 mode);
void TFT_Clear_Pos(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color);
void TFT_Print_Center(u16 x1, u16 y1, u16 x2, u16 y2, u16 fc, u16 bc, u8 size, u8 type_text, u8 *str, u8 mode);

void Gui_DrawbmpUser(u16 x,u16 y, u16 picxelX, u16 picxelY ,const unsigned char *p);


#endif



