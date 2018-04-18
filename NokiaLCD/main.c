#include <stdio.h>
#include <wiringPi.h>
#include "Nokia/Nokia.h"
#include "Nokia/Images.h"

int nokiaPinMap[eNbrOfPins]={
		7,	// Rst
		0,	// CE
		2,	// DC
		3,	// DIN
		1,	// Clk
		4,	// Light
};

void writeNokiaPin(int pin,bool state){
	digitalWrite(nokiaPinMap[pin],state);
}

void initNokiaPin(){
	for(int i=0;i<eNbrOfPins;i++){
		pinMode(nokiaPinMap[i],OUTPUT);
	}
}

int main() {
	wiringPiSetup();
	cb_registerPins(writeNokiaPin);
	initNokiaPin();
	lcdReset();
	lcdInit();
	lcdChar('A');
	gr_drawImage(LoZimg);
	gr_updateScreen();
	lcdPrint("LOL Mdr :)");
	return 0;
}
