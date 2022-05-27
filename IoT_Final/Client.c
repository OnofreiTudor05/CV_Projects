#include "sl.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

int mouseOver(double x, double y, double w, double h){
	double xMouse = slGetMouseX();
	double yMouse = slGetMouseY();
	
	x = x - w/2;
	y = y - h/2;

	if(xMouse >= x && xMouse <= x + w && yMouse >= y && yMouse <= y + h){
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]){
	int valMin = 0, valMax = 100, ok;
	double posX = 100, posY = 100, posCircle = 100;
	double value;



// ajustare pozitii -- desenarea se face prin precizarea punctelor de centru, nu prin precizarea punctului stanga-jos


	slWindow(800, 600, "Client", 0);
	while(!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)){
		
		if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)){
			ok = 1;
		}
		else{
			ok = 0;
		}
		if(ok && mouseOver(posX, posY, 100, 50)){
			posCircle = slGetMouseX();
			value = (valMax - valMin) * (posCircle - posX + 50) * 1.0 / 100;
		}
		slSetForeColor(1, 1, 1, 1);
		slRectangleFill(posX, posY, 100, 50);
		slSetForeColor(1, 0, 0, 1);
		
		slCircleFill(posCircle, posY, 25, 50);
		
		slRender();
		system("clear");
		printf("Value: %f\n", value);
	}

	slClose();
	return 0;
}