#include "sl.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

int socketDescriptor;			  // descriptorul de socket pentru server
struct sockaddr_in server;	// structura folosita pentru conectare  

int FontScris, stagii[10], InAPP = 1;
char IP_Conectare[20], bufferMesaj[20];
int porturiDevice[4], texturiDevice[4], DeviceID;

struct Culoare{
	float r;
	float g;
	float b;
	float a;
	Culoare(int red, int green, int blue, int alpha) : r(red * 1.0/255), g(green * 1.0/255), b(blue * 1.0/255), a(alpha * 1.0/255) {}
	Culoare(){
		this->r = this->g = this->b = this->a = 1.0;
	}
};

struct Pozitie{
	float X;
	float Y;
	float W;
	float H;
	Pozitie(float x, float y, float w, float h) : X(x), Y(y), W(w), H(h) {}
	Pozitie() {
		this->X = this->Y = 400;
		this->W = this->H = 50;
			
	}
};

struct Buton{
	Pozitie pozitieButon;
	Culoare CuloareFundal;
	Culoare CuloareOutline;
	Culoare CuloareText;
	float grosimeOutline;
	char textButon[100];

	void setButtonOutline(float grosime){
		this->grosimeOutline = grosime;
	}

	void setPozitieButon(float x, float y, float w, float h){
		this->pozitieButon.X = x;
		this->pozitieButon.Y = y;
		this->pozitieButon.W = w;
		this->pozitieButon.H = h;
	}

	void setButtonText(char* mesaj){
		strcpy(this->textButon, mesaj);
	}
	
	void setCuloareText(int red, int green, int blue, int alpha){
		this->CuloareText.r = red * 1.0/255;
		this->CuloareText.g = green * 1.0/255;
		this->CuloareText.b = blue * 1.0/255;
		this->CuloareText.a = alpha * 1.0/255;	
	}
	

	void setCuloareFundal(int red, int green, int blue, int alpha){
		this->CuloareFundal.r = red * 1.0/255;
		this->CuloareFundal.g = green * 1.0/255;
		this->CuloareFundal.b = blue * 1.0/255;
		this->CuloareFundal.a = alpha * 1.0/255;	
	}
	
	void setCuloareOutline(int red, int green, int blue, int alpha){
		this->CuloareOutline.r = red * 1.0/255;
		this->CuloareOutline.g = green * 1.0/255;
		this->CuloareOutline.b = blue * 1.0/255;
		this->CuloareOutline.a = alpha * 1.0/255;	
	}

	void deseneazaButon(){
		slSetForeColor(this->CuloareOutline.r, this->CuloareOutline.g, this->CuloareOutline.b, this->CuloareOutline.a);
		slRectangleFill(this->pozitieButon.X, this->pozitieButon.Y, this->pozitieButon.W + this->grosimeOutline, this->pozitieButon.H + this->grosimeOutline);
		slSetForeColor(this->CuloareFundal.r, this->CuloareFundal.g, this->CuloareFundal.b, this->CuloareFundal.a);
		slRectangleFill(this->pozitieButon.X, this->pozitieButon.Y, this->pozitieButon.W, this->pozitieButon.H);
		slSetForeColor(this->CuloareText.r, this->CuloareText.g, this->CuloareText.b, this->CuloareText.a);
		
		slSetTextAlign(SL_ALIGN_CENTER);
		slSetFont(FontScris, 50);
		float lungime = slGetTextWidth(this->textButon);
		float latime = slGetTextHeight(this->textButon);

		slText(this->pozitieButon.X, this->pozitieButon.Y - 2*this->grosimeOutline, this->textButon);
	}
	Buton(){
		this->pozitieButon.X = this->pozitieButon.Y = 100;
		this->pozitieButon.W = this->pozitieButon.H = 50;
		this->setButtonText("Mesaj");
		this->setCuloareFundal(255, 255, 255, 255);
		this->setCuloareOutline(255, 0, 0, 255);
		this->setCuloareText(0, 255, 0, 255);
	}	
};

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

void meniuConectare(){
	int lungime = 0;
	float passedTime = 0, passedTimeDis = 0, passedTimeText = 0, passedTime1 = 0, timer = 0.2, timerDis = 2, timer1 = 0.5, timerText = 0.25;
	int ok = 1;
	char mesajIP[20] = "";

	Buton CasetaText;
	CasetaText.setButtonOutline(6);
	CasetaText.setButtonText("");
	CasetaText.setPozitieButon(400, 300, 800 - CasetaText.grosimeOutline, 60);
	CasetaText.setCuloareFundal(19, 100, 33, 255);
	CasetaText.setCuloareOutline(255, 255, 255, 200);
	CasetaText.setCuloareText(0, 255, 0, 255);
	
	slSetFont(FontScris, 48);	
	while(stagii[1]){
		passedTimeText += slGetDeltaTime();
		passedTime += slGetDeltaTime();
		passedTimeDis += slGetDeltaTime() * ok * 1/2;

		char textWelcome[60] = "   Bine ati venit!   \nIntroduceti adresa IP: ";
		char textValidare[50] = "Apasati \"Enter\" \npentru validare!";

		slSetFontSize(48);
		slSetForeColor(0, 1, 0, 1);
		slSetTextAlign(SL_ALIGN_CENTER);
		slText(400, 500, textWelcome);
		
		if(passedTimeDis > 1){
			ok *= -1;
			passedTimeDis = 1;
		}
		else if(passedTimeDis < 0){
			ok *= -1;
			passedTimeDis = 0;
		}


		slSetForeColor(0, 1, 0, passedTimeDis);
		slText(400, 100, textValidare);
		
		slSetForeColor(1, 1, 1, 0);

		for(int i = 46; i<=57 && passedTimeText > timerText; i++){
			if(slGetKey((char)i) && lungime < 19 && i != 47){
				passedTimeText = 0;
				mesajIP[lungime] = (char)i;
				lungime++;
				mesajIP[lungime] = '\0';
			}
		}
		if(slGetKey(SL_KEY_BACKSPACE) && passedTimeText > timerText){
			passedTimeText = 0;
			if(lungime >= 1){
				lungime --;
				mesajIP[lungime] = '\0';
			}
		}
		if(slGetKey(SL_KEY_ENTER)){
			//InAPP = 0;
			stagii[1] = 0;
			strcpy(IP_Conectare, CasetaText.textButon);
		}

		CasetaText.setButtonText(mesajIP);
		CasetaText.deseneazaButon();
		if(passedTime > timer){
			passedTime1 += slGetDeltaTime();
			if(passedTime1 < timer1){
				slSetForeColor(0, 1, 1, 1);
				slRectangleFill(CasetaText.pozitieButon.X + slGetTextWidth(CasetaText.textButon)/2 + 10, CasetaText.pozitieButon.Y, 10, CasetaText.pozitieButon.H);
				slSetForeColor(1, 1, 1, 0);
			}
			else{
				passedTime = passedTime1 = 0;
			}
		}
		
		slRender();
	}
	stagii[1] = 0;
	stagii[2] = 1;
}

void alegeDispozitiv(){
	char mesajDevice[200] = "Alegeti dispozitivul\n  pentru conectare: ";
	char mesajDeviceAles[200] = "";

	Pozitie pozitieSpeaker(150, 150, 150, 150), pozitieBec(400, 150, 150, 150), pozitieTV(650, 150, 150, 150);
	
	slSetFont(FontScris, 40);
	slSetTextAlign(SL_ALIGN_LEFT);
	int ok = 1;
	slSetBackColor(0,0,0);
	DeviceID = 0;
	while(stagii[2]){

		slSetFontSize(30);
		slSetForeColor(0, 1, 0, 1);
		slText(200, 500, mesajDevice);
		slSetFontSize(50);

		if(mouseOver(pozitieSpeaker.X, pozitieSpeaker.Y, pozitieSpeaker.W, pozitieSpeaker.H)){
			slSetForeColor(1, 1, 1, 1);
			strcpy(mesajDeviceAles, "Difuzor");
			slSprite(texturiDevice[1], pozitieSpeaker.X, pozitieSpeaker.Y, pozitieSpeaker.W + 20, pozitieSpeaker.H + 20);
			slText(300, 350, mesajDeviceAles);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)){
				DeviceID = 1;
				stagii[2] = 0;
			}
		}
		else{
			slSetForeColor(1, 1, 1, 0.2);
			strcpy(mesajDeviceAles, "");
			slSprite(texturiDevice[1], pozitieSpeaker.X, pozitieSpeaker.Y, pozitieSpeaker.W, pozitieSpeaker.H);
			slText(300, 350, mesajDeviceAles);
		}
		if(mouseOver(pozitieBec.X, pozitieBec.Y, pozitieBec.W, pozitieBec.H)){
			slSetForeColor(1, 1, 1, 1);
			strcpy(mesajDeviceAles, "Iluminat");
			slSprite(texturiDevice[2], pozitieBec.X, pozitieBec.Y, pozitieBec.W + 20, pozitieBec.H + 20);
			slText(300, 350, mesajDeviceAles);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)){
				DeviceID = 2;
				stagii[2] = 0;
			}
		}
		else{
			slSetForeColor(1, 1, 1, 0.2);
			strcpy(mesajDeviceAles, "");
			slSprite(texturiDevice[2], pozitieBec.X, pozitieBec.Y, pozitieBec.W, pozitieBec.H);
			slText(300, 350, mesajDeviceAles);
		}
		if(mouseOver(pozitieTV.X, pozitieTV.Y, pozitieTV.W, pozitieTV.H)){
			slSetForeColor(1, 1, 1, 1);
			strcpy(mesajDeviceAles, "Televizor");
			slSprite(texturiDevice[3], pozitieTV.X, pozitieTV.Y, pozitieTV.W + 20, pozitieTV.H + 20);
			slText(300, 350, mesajDeviceAles);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)){
				DeviceID = 3;
				stagii[2] = 0;
			}
		}
		else{
			slSetForeColor(1, 1, 1, 0.2);
			strcpy(mesajDeviceAles, "");
			slSprite(texturiDevice[3], pozitieTV.X, pozitieTV.Y, pozitieTV.W, pozitieTV.H);
			slText(300, 350, mesajDeviceAles);
		}
		slSetForeColor(1, 1, 1, 1);
		slRender();
	}
	stagii[2] = 0;
	stagii[DeviceID + 2] = 1;
}


void seteazaStareLight(){
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);      // cream socketul pentru conectare
	server.sin_family = AF_INET;                             // descriem structura utilizata pentru conectarea la server   
  	server.sin_addr.s_addr = inet_addr(IP_Conectare);
  	server.sin_port = htons(porturiDevice[DeviceID]);

  	connect(socketDescriptor,(struct sockaddr *)& server, sizeof(struct sockaddr));
  										// aici facem conectarea la server
	char mesaj[100] = "";
	char mesajValoare[20] = "Valoare: ";
	char mesajServer[10];		// Aici primim de la server starea curenta
	char mesajCanal[10];

	float passedTime = 0, timer = 0.5, alpha;

	int okTema = 0, okAtinsButon = 0;
	Buton CasetaText, CasetaFundal, CasetaSave, CasetaBack;

	CasetaText.setButtonOutline(10);
	CasetaText.setButtonText("");
	CasetaText.setPozitieButon(400, 200, 300 - CasetaText.grosimeOutline, 60);
	CasetaText.setCuloareFundal(255, 255, 0, 255);
	CasetaText.setCuloareOutline(0, 0, 0, 255);
	CasetaText.setCuloareText(0, 255, 0, 255);

	CasetaFundal.setButtonOutline(0);
	CasetaFundal.setButtonText("");
	CasetaFundal.setPozitieButon(400, 300, 800, 600);
	CasetaFundal.setCuloareFundal(255, 255, 255, 255);
	CasetaFundal.setCuloareOutline(255, 255, 255, 255);
	CasetaFundal.setCuloareText(0, 255, 0, 255);

	CasetaSave.setButtonOutline(10);
	CasetaSave.setButtonText("Save");
	CasetaSave.setPozitieButon(400, 100, 200, 60);
	CasetaSave.setCuloareFundal(255, 255, 0, 100);
	CasetaSave.setCuloareOutline(255, 0, 0, 255);
	CasetaSave.setCuloareText(0, 0, 0, 255);

	slSetFont(FontScris, 40);
	slSetBackColor(1, 1, 1);
	
	read(socketDescriptor, mesajServer, 255);
	
	if(!strcmp(mesajServer, "ON")){
		okTema = 0;
	}
	else{
		okTema = 1;
	}
	strcpy(mesaj, "    Setati lumina \napasand butonul galben!");

	while(stagii[4]){
		passedTime += slGetDeltaTime();

		if(mouseOver(CasetaText.pozitieButon.X, CasetaText.pozitieButon.Y, CasetaText.pozitieButon.W, CasetaText.pozitieButon.H)){
			okAtinsButon = 1;
			CasetaText.setCuloareFundal(255, 255, 0, 100);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT) && passedTime > timer){
				okTema = 1-okTema;
				passedTime = 0;
				if(okTema){
					strcpy(bufferMesaj, "OFF");
					write(socketDescriptor, bufferMesaj, 255);
				}
				else{
					strcpy(bufferMesaj, "ON");
					write(socketDescriptor, bufferMesaj, 255);
				}
			}
		}
		else{
			CasetaText.setCuloareFundal(255, 255, 0, 255);
		}
		if(mouseOver(CasetaSave.pozitieButon.X, CasetaSave.pozitieButon.Y, CasetaSave.pozitieButon.W, CasetaSave.pozitieButon.H)){
			CasetaSave.setCuloareFundal(255, 255, 0, 255);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)){

				write(socketDescriptor, bufferMesaj, 255);
				stagii[4] = 0;
			}
		}
		else{
			CasetaSave.setCuloareFundal(255, 255, 0, 100);
		}	
				
		CasetaFundal.deseneazaButon();
									
		if(okTema){
					
			strcpy(mesajValoare, "Valoare: OFF");
			slSetTextAlign(SL_ALIGN_CENTER);
			alpha += slGetDeltaTime() * 100;
							
			if(alpha > 255){
				alpha = 255;
			}
			CasetaFundal.setCuloareFundal(0, 0, 0, (int)alpha);
			CasetaText.setCuloareOutline(255, 255, 255, 255);
					
			slSetForeColor(1, 1, 1, 1);
			slText(400, 500, mesaj);
			slText(400, 300, mesajValoare);
		}
		else{	
			strcpy(mesajValoare, "Valoare: ON");
			alpha -= slGetDeltaTime() * 100;
					
			if(alpha < 0){
				alpha = 0;
			}
			CasetaText.setCuloareOutline(0, 0, 0, 255);
			CasetaFundal.setCuloareFundal(0, 0, 0, (int)alpha);
					
			slSetForeColor(0, 0, 0, 1);
			slText(400, 500, mesaj);
			slText(400, 300, mesajValoare);
		}
		CasetaSave.deseneazaButon();
		CasetaText.deseneazaButon();
		slRender();
	}

	strcpy(bufferMesaj, "exit");
	write(socketDescriptor, bufferMesaj, 255);
	stagii[4] = 0; InAPP = 0;
	//close(socketDescriptor);
}


void seteazaStareTV(){
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);      // cream socketul pentru conectare
	server.sin_family = AF_INET;                             // descriem structura utilizata pentru conectarea la server   
  	server.sin_addr.s_addr = inet_addr(IP_Conectare);
  	server.sin_port = htons(porturiDevice[DeviceID]);

  	connect(socketDescriptor,(struct sockaddr *)& server, sizeof(struct sockaddr));  // aici facem conectarea la server
  	
  	char mesajCanal[10] = "";								
	char mesaj[100] = "";
	char mesajValoare[100] = "Canal curent: ";
	char mesajServer[100];		// Aici primim de la server starea curenta
	char textValidare[50] = "Apasati \"Enter\" \npentru validare!";

	strcpy(mesaj, "Introduceti \ncanalul nou:");
	
	float passedTime = 0, timer = 0.5, alpha;

	Buton CasetaText, CasetaSave, CasetaBack;

	CasetaText.setButtonOutline(10);
	CasetaText.setButtonText("");
	CasetaText.setPozitieButon(400, 250, 200, 60);
	CasetaText.setCuloareFundal(255, 255, 0, 255);
	CasetaText.setCuloareOutline(0, 0, 0, 255);
	CasetaText.setCuloareText(0, 0, 0, 255);

	CasetaSave.setButtonOutline(10);
	CasetaSave.setButtonText("Save");
	CasetaSave.setPozitieButon(400, 100, 200, 60);
	CasetaSave.setCuloareFundal(255, 255, 0, 255);
	CasetaSave.setCuloareOutline(255, 0, 0, 255);
	CasetaSave.setCuloareText(0, 0, 0, 255);

	slSetBackColor(0, 0, 0);
	
	read(socketDescriptor, mesajServer, 100);

	slSetFont(FontScris, 40);
	slSetTextAlign(SL_ALIGN_CENTER);
	strcat(mesajValoare, mesajServer);

	int lungime = 0;
	while(stagii[5]){

		passedTime += slGetDeltaTime();

		for(int i = 48; i<=57; i++){
			if(slGetKey((char)i) && lungime < 3 && passedTime > timer){
				mesajCanal[lungime] = (char)i;
				lungime++;
				mesajCanal[lungime] = '\0';
				passedTime = 0;
				write(socketDescriptor, mesajCanal, 255);
			}
		}
		if(slGetKey(SL_KEY_BACKSPACE) && passedTime > timer){
			passedTime = 0;
			lungime--;
			mesajCanal[lungime] = '\0';
			write(socketDescriptor, mesajCanal, 255);
		}
		if(mouseOver(CasetaSave.pozitieButon.X, CasetaSave.pozitieButon.Y, CasetaSave.pozitieButon.W, CasetaSave.pozitieButon.H)){
			CasetaSave.setCuloareFundal(255, 255, 0, 255);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT) && passedTime > timer){
				passedTime = 0;
				stagii[5] = 0;
				write(socketDescriptor, mesajCanal, 255);
			}
		}
		CasetaText.setButtonText(mesajCanal);

		slSetForeColor(0, 1, 0, 1);
		slText(400, 500, mesajValoare);

		slSetForeColor(1, 1, 1, 1);
		slText(400, 400, mesaj);

		CasetaSave.deseneazaButon();
		CasetaText.deseneazaButon();

		slRender();
	}
	strcpy(bufferMesaj, "exit");
	write(socketDescriptor, bufferMesaj, 255);
	stagii[5] = 0;
	InAPP = 0;
}

void seteazaStareSpeaker(){
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);      // cream socketul pentru conectare
	server.sin_family = AF_INET;                             // descriem structura utilizata pentru conectarea la server   
  	server.sin_addr.s_addr = inet_addr(IP_Conectare);
  	server.sin_port = htons(porturiDevice[DeviceID]);

  	connect(socketDescriptor,(struct sockaddr *)& server, sizeof(struct sockaddr));  // aici facem conectarea la server
  	
  	char mesajCanal[10] = "";								
	char mesaj[100] = "";
	char mesajValoare[100] = "Volum curent: ";
	char mesajServer[100];		// Aici primim de la server starea curenta
	
	strcpy(mesaj, "Setati volumul nou:");
	
	float passedTime = 0, timer = 0.5, alpha, posCircle, value;
	int ok;

	Buton CasetaText, CasetaSave, CasetaBack;

	CasetaText.setButtonOutline(10);
	CasetaText.setButtonText("");
	CasetaText.setPozitieButon(400, 250, 200, 60);
	CasetaText.setCuloareFundal(255, 255, 0, 255);
	CasetaText.setCuloareOutline(0, 0, 0, 255);
	CasetaText.setCuloareText(0, 0, 0, 255);

	CasetaSave.setButtonOutline(10);
	CasetaSave.setButtonText("Save");
	CasetaSave.setPozitieButon(400, 100, 200, 60);
	CasetaSave.setCuloareFundal(255, 255, 0, 255);
	CasetaSave.setCuloareOutline(255, 0, 0, 255);
	CasetaSave.setCuloareText(0, 0, 0, 255);

	slSetBackColor(0, 0, 0);
	
	read(socketDescriptor, mesajServer, 100);
	value = atoi(mesajServer);
	posCircle = value / 100 * CasetaText.pozitieButon.W + (CasetaText.pozitieButon.X - CasetaText.pozitieButon.W/2);

	slSetFont(FontScris, 40);
	slSetTextAlign(SL_ALIGN_CENTER);
	strcat(mesajValoare, mesajServer);

	int lungime = 0;
	while(stagii[3]){

		passedTime += slGetDeltaTime();

		if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT)){
			ok = 1;
		}
		else{
			ok = 0;
		}
		if(ok && mouseOver(CasetaText.pozitieButon.X, CasetaText.pozitieButon.Y, CasetaText.pozitieButon.W, CasetaText.pozitieButon.H)){
			CasetaText.setCuloareFundal(255, 255, 0, 200);
			posCircle = slGetMouseX();
			value = (posCircle - CasetaText.pozitieButon.X + CasetaText.pozitieButon.W/2) * 100 / CasetaText.pozitieButon.W;
			sprintf(mesajCanal, "%d", (int)value);
			write(socketDescriptor, mesajCanal, 255);
		}
		if(!mouseOver(CasetaText.pozitieButon.X, CasetaText.pozitieButon.Y, CasetaText.pozitieButon.W, CasetaText.pozitieButon.H)){
			CasetaText.setCuloareFundal(255, 255, 0, 255);
		}

		if(mouseOver(CasetaSave.pozitieButon.X, CasetaSave.pozitieButon.Y, CasetaSave.pozitieButon.W, CasetaSave.pozitieButon.H)){
			CasetaSave.setCuloareFundal(255, 255, 0, 255);
			if(slGetMouseButton(SL_MOUSE_BUTTON_LEFT) && passedTime > timer){
				passedTime = 0;
				stagii[3] = 0;
				sprintf(mesajCanal, "%d", (int)value);
				write(socketDescriptor, mesajCanal, 255);	
			}
		}
		else{
			CasetaSave.setCuloareFundal(255, 255, 0, 100);
		}
		
		slSetForeColor(0, 1, 0, 1);
	
		slSetForeColor(0, 1, 0, 1);
		sprintf(mesajValoare, "%s%d", "Volum curent: ", (int)value);
		slText(400, 500, mesajValoare);

		slSetForeColor(1, 1, 1, 1);
		slText(400, 400, mesaj);

		CasetaSave.deseneazaButon();
		CasetaText.deseneazaButon();

		if(ok){
			slSetForeColor(0, 1, 0, 0.5);
		}
		else{
			slSetForeColor(0, 1, 0, 1);
		}
		slCircleFill(posCircle, CasetaText.pozitieButon.Y, CasetaText.pozitieButon.H/2, 50);
	
		slRender();
	}
	strcpy(bufferMesaj, "exit");
	sprintf(mesajCanal, "%d", (int)value);
	write(socketDescriptor, mesajCanal, 10);	
	stagii[3] = 0;
	InAPP = 0;
}


int main(int argc, char* argv[]){
	int valMin = 0, valMax = 100, ok;
	double posX = 100, posY = 100, posCircle = 100;
	double value;

	for(int i = 1; i<=3; i++){
		stagii[i] = 0;
		porturiDevice[i] = 2907 + i;
	}
	stagii[1] = 1;

	slWindow(800, 600, "Client", false);
	texturiDevice[1] = slLoadTexture("_resources/_Imagini/SpeakerFFF.png");
	texturiDevice[2] = slLoadTexture("_resources/_Imagini/BecFF.png");
	texturiDevice[3] = slLoadTexture("_resources/_Imagini/TVF.png");

	FontScris = slLoadFont("_resources/_Fonts/white_rabbit.ttf");
	
	slSetFont(FontScris, 20);		
	
	while(InAPP){
		if(stagii[1]){
			meniuConectare();
		}
		if(stagii[2]){
			alegeDispozitiv();
		}
		if(stagii[3]){
			seteazaStareSpeaker();
	
		}
		if(stagii[4]){
			seteazaStareLight();
		}
		if(stagii[5]){
			seteazaStareTV();
		}
	}
	
	slClose();
	return 0;
}