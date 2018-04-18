/*
 * Nokia.c
 *
 *  Created on: 28 mai 2016
 *      Author: D01000100
 */
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "Nokia.h"
#include "Graphics.h"

#define VMODE 0x22
#define HMODE 0x20
#define DISPLAY_CTL 0x0D
#define SETY 0x27
#define SETX 0x7f
#define PERIOD 0

#define Hbytes 84
#define Vbytes 6

uint8_t frameBuffer[Hbytes][Vbytes] = {{0}};

outputHandler_t pinsLCD = nullptr;

void cb_registerPins(outputHandler_t fctPtr/*, int index*/)
{
	pinsLCD = fctPtr;/*
	if(index<eNbrOfPins)
	{
		pinsLCD[index] = fctPtr;
	}*/
}

void lcdReset()
{
	pinsLCD(eRST,false);
	pinsLCD(eRST,true);
}

void shiftData(uint8_t data)
{
	for(int i=0;i<8;i++)
	{
		//bool DINstate = (((data>>i) & 0x01) == 0x01)?true:false;	//LSB FIRST
		bool DINstate = (((data<<i) & 0x80) == 0x80)?true:false;	//MSB FIRST
		pinsLCD(eDIN,DINstate);

		//Tick
		pinsLCD(eCLK,true);
		//usleep(PERIOD);
		delay(PERIOD);

		//Tack
		pinsLCD(eCLK,false);
		//usleep(PERIOD);
		delay(PERIOD);
	}
}

void lcdWrite(bool DCval, uint8_t data)
{
	pinsLCD(eDC,DCval);
	pinsLCD(eCE,false);
	shiftData(data);
	pinsLCD(eCE,true);
}

void lcdInit()
{
	lcdReset();

	lcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
	lcdWrite(LCD_C, 0xc8 );  // Set LCD Vop (Contrast).
	lcdWrite(LCD_C, 0x06 );  // Set Temp coefficent. //0x04
	lcdWrite(LCD_C, 0x13 );  // LCD bias mode 1:48. //0x13
	lcdWrite(LCD_C, 0x20 );  // LCD Basic Commands
	lcdWrite(LCD_C, 0x0C );  // LCD in normal mode.

	lcdClear();
}

void lcdChar(char character)
{
  if(character>=0x20 && character<=0x7e){
    lcdWrite(LCD_D, 0x00);
    for (int index = 0; index < 5; index++)
    {
      lcdWrite(LCD_D, ASCII[character - 0x20][index]);
    }
    lcdWrite(LCD_D, 0x00);
  }
}

void lcdPrint(const char *str){
	for(int i=0; str[i] != '\0'; i++){
		lcdChar(str[i]);
	}
}

void lcdClear(){
	for(int i=0;i<Hbytes*Vbytes;i++){
		lcdWrite(LCD_D, 0x00);
	}
}

void gr_updateScreen()
{
	lcdInit();
	for(int i=0;i<Vbytes;i++)
	{
		for(int j=0;j<Hbytes;j++)
		{
			lcdWrite(LCD_D, frameBuffer[j][i]);
		}
	}
}

void gr_drawImage(const uint8_t*image)
{
	//lcdInit();
	int i=0;
	for(int h=0;h<Hbytes;h++)
	{
		for(int v=0;v<Vbytes;v++)
		{
			frameBuffer[h][v]=image[i++];
		}
	}
	/*
	for(int i=0;i<504;i++)
	{
		lcdWrite(LCD_D, image[i]);
	}*/
	/*
	for(int i=0;i<Vbytes;i++)
	{
		for(int j=0;j<Hbytes;j++)
		{
			frameBuffer[j][i] = image[i+j];
		}
	}*/
}

void gr_setPixel(int x, int y)
{
	if(x<=Hbytes-1 && y<=Vbytes*8-1)
	{
		frameBuffer[x][y/8] |= (0x01<<(y%8));
	}
}
