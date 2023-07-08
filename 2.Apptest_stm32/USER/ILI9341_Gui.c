#include "stm32f10x.h"
#include "ILI9341_Gui.h"
#include "ILI9341_Driver.h"
#include "ILI9341_Define.h"
#include "string.h"


void TFT_Clear(u16 Color){
  unsigned int i,m;  
	TFT_SetWindows(0,0,ili_dev.width-1,ili_dev.height-1);   
	TFT_PIN_CS_RESET;
	TFT_PIN_DC_SET;
	
	for(i=0;i<ili_dev.height;i++){
    for(m=0;m<ili_dev.width;m++){	
			TFT_Send_Data_16Bit(Color);
		}
	}
	TFT_PIN_CS_SET;
} 

void TFT_Gui_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color){  	
	u16 i,j;			
	u16 width=ex-sx+1; 		
	u16 height=ey-sy+1;		
	TFT_SetWindows(sx,sy,ex,ey);
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			TFT_Send_Data_16Bit(color);	
		}
	}
	TFT_SetWindows(0,0,ili_dev.width-1,ili_dev.height-1);
}


/*****************************************************************************
 * @name       :void TFT_Gui_Direction(u8 direction)
 * @date       :2018-08-09 
 * @function   :Setting the display direction of TFT screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/ 
void TFT_Gui_Direction(u8 direction)
{ 
			ili_dev.setxcmd=0x2A;
			ili_dev.setycmd=0x2B;
			ili_dev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			ili_dev.width=TFT_W;
			ili_dev.height=TFT_H;		
			TFT_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			ili_dev.width=TFT_H;
			ili_dev.height=TFT_W;
			TFT_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			ili_dev.width=TFT_W;
			ili_dev.height=TFT_H;	
			TFT_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			ili_dev.width=TFT_H;
			ili_dev.height=TFT_W;
			TFT_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	

/*****************************************************************************
 * @name       :void TFT_SetCursor(u16 Xpos, u16 Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void TFT_SetCursor(u16 Xpos, u16 Ypos){	  	    			
	TFT_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/*****************************************************************************
 * @name       :void DrawPoint(u16 x,u16 y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void DrawPoint(u16 x, u16 y){
	TFT_SetCursor(x,y);
	TFT_Send_Data_16Bit(POINT_COLOR); 
}

/*******************************************************************
 * @name       :void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2){
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x = x2-x1;  
	delta_y = y2-y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x > 0) incx = 1; 
	else if(delta_x == 0) incx=0; 
	else {incx = -1; delta_x = -delta_x;} 
	if(delta_y > 0) incy = 1; 
	else if(delta_y == 0) incy = 0;
	else{incy = -1; delta_y = -delta_y;} 
	if(delta_x > delta_y) distance = delta_x; 
	else distance = delta_y; 
	for(t=0; t <= distance + 1; t++){  
		DrawPoint(uRow, uCol);
		xerr += delta_x; 
		yerr += delta_y; 
		if(xerr > distance){ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr > distance){ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
} 

/*****************************************************************************
 * @name       :void TFT_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void TFT_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2){
	TFT_DrawLine(x1,y1,x2,y1);
	TFT_DrawLine(x1,y1,x1,y2);
	TFT_DrawLine(x1,y2,x2,y2);
	TFT_DrawLine(x2,y1,x2,y2);
}  

/*******************************************************************
 * @name       :void TFT_Gui_DrawPoint(u16 x,u16 y,u16 color)
 * @date       :2018-08-09 
 * @function   :draw a point in TFT screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
********************************************************************/
void TFT_Gui_DrawPoint(u16 x,u16 y,u32 color){
	TFT_SetCursor(x,y);
	TFT_Send_Data_16Bit(color); 
}

/*****************************************************************************
 * @name       :void TFT_Draw_Circle_8(int xc, int yc, int x, int y, u16 c)
 * @date       :2018-08-09 
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								x:the x coordinate relative to the Circular center 
								y:the y coordinate relative to the Circular center 
								c:the color value of the circle
 * @retvalue   :None
******************************************************************************/  
void TFT_Draw_Circle_8(int xc, int yc, int x, int y, u16 c){
	TFT_Gui_DrawPoint(xc + x, yc + y, c);

	TFT_Gui_DrawPoint(xc - x, yc + y, c);

	TFT_Gui_DrawPoint(xc + x, yc - y, c);

	TFT_Gui_DrawPoint(xc - x, yc - y, c);

	TFT_Gui_DrawPoint(xc + y, yc + x, c);

	TFT_Gui_DrawPoint(xc - y, yc + x, c);

	TFT_Gui_DrawPoint(xc + y, yc - x, c);

	TFT_Gui_DrawPoint(xc - y, yc - x, c);
}

/*****************************************************************************
 * @name       :void TFT_Gui_Circle(int xc, int yc,u16 c,int r, int fill)
 * @date       :2018-08-09 
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/  
void TFT_Gui_Circle(int xc, int yc,u16 c,int r, int fill){
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	if (fill){
		while (x <= y) {
			for (yi = x; yi <= y; yi++){
				TFT_Draw_Circle_8(xc, yc, x, yi, c);
			}
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} 
	else {
		while (x <= y) {
			TFT_Draw_Circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

/*****************************************************************************
 * @name       :void TFT_Gui_Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void TFT_Gui_Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2){
	TFT_DrawLine(x0,y0,x1,y1);
	TFT_DrawLine(x1,y1,x2,y2);
	TFT_DrawLine(x2,y2,x0,y0);
}

static void _swap(u16 *a, u16 *b){
	u16 tmp;
  tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :void ILI9431_Gui_Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-09 
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void ILI9431_Gui_Fill_Triangel(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2){
	u16 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1){
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2){
    _swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  if (y0 > y1) {
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
  }
	if(y0 == y2){ 
		a = b = x0;
		if(x1 < a){
			a = x1;
    }
    else if(x1 > b){
			b = x1;
    }
    if(x2 < a){
			a = x2;
    }
		else if(x2 > b){
			b = x2;
    }
		TFT_Gui_Fill(a,y0,b,y0,POINT_COLOR);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2){
		last = y1; 
	}
  else{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b){
			_swap(&a,&b);
		}
		TFT_Gui_Fill(a,y,b,y,POINT_COLOR);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b){
			_swap(&a,&b);
		}
		TFT_Gui_Fill(a,y,b,y,POINT_COLOR);
	}
}

/*****************************************************************************
 * @name       :u32 mypow(u8 m,u8 n)
 * @date       :2018-08-09 
 * @function   :get the nth power of m (internal call)
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
******************************************************************************/ 
u32 mypow(u8 m,u8 n){
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void TFT_Gui_Img(u16 x,u16 y, u16 picxel_X, u16 picxel_Y ,const unsigned char *p){
  int i; 
	unsigned char picH,picL; 
	TFT_SetWindows(x,y,x+picxel_X-1,y+picxel_Y-1);
	for(i=0;i<picxel_X*picxel_Y	;i++){	
	 	picL=*(p+i*2);	
		picH=*(p+i*2+1);				
		TFT_Send_Data_16Bit(picH<<8|picL);  						
	}	
	TFT_SetWindows(0,0,ili_dev.width-1,ili_dev.height-1);
}

void Gui_DrawbmpUser(u16 x,u16 y, u16 picxelX, u16 picxelY ,const unsigned char *p) //显示40*40 QQ图片
{
  int i; 
	unsigned char picH,picL; 
	TFT_SetWindows(x,y,x+picxelX-1,y+picxelY-1);// 70*85: chinh theo picxel cua image
	for(i=0;i<picxelX*picxelY;i++)  // 70*85: chinh theo picxel cua image
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		TFT_Send_Data_16Bit(picH<<8|picL);  						
	}	
	TFT_SetWindows(0,0,ili_dev.width-1,ili_dev.height-1);//恢复显示窗口为全屏	
}

