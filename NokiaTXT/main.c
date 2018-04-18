#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>

#include "Nokia/Nokia.h"
#include "Nokia/Images.h"

#define BUF_SIZE 500

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


int main(int argc, char*argv[]) {
	int fd;
	if(argc == 1){
		printf("Reading from STDIN\n");
		fd = STDIN_FILENO;
	}
	else if(argc == 2){
		printf("Reading the file %s\n",argv[1]);
		fd = open(argv[1], O_RDONLY);
	}
	else{
		printf("Usage : %s [path]\n",argv[0]);
	}
	wiringPiSetup();
	cb_registerPins(writeNokiaPin);
	initNokiaPin();
	lcdReset();
	lcdInit();
	char buf[BUF_SIZE];
	buf[BUF_SIZE] = 0;
	while(1){
		int n = read(fd, &buf, BUF_SIZE-1);
		if(n <= 0)break;
		lcdPrint((const char*)buf);
	}
	return 0;
}
