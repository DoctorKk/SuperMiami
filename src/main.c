#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <time.h>
#include <Windows.h>
#include <olectl.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include"imgui.c"
#include "pattern.c"
#include "Map.c"
#include"MENU.c"
#pragma comment(lib, "WINMM.LIB")
#define MAX 25


#define dx GetWindowWidth()/100
#define dy GetWindowHeight()/100
#define Theta sqrt(3)/3




void init();
void init_back();
void display();
void DisplayClear();

void KeyboardEventProcess(int key, int event);
void MouseEventProcess(int x, int y, int button, int event);
void CharEventProcess(char ch);
void TimeEventProcess(int timerID);

void Draw();
void Draw_MC();
void Draw_Enemy();
void Draw_Bullet_MC();
void Mc_Death();
void Draw_Bullet_Enemy();
void Draw_Background();
void Enemy_Judge();
void Enemy_AI();
void Death_Judge();
void Enemy_Clear();
int Mc_bullet_Judge(int i);

void MAP(int i);
void BIG_MAP();
void MAP_MOVE(char d);
int MAP_JUDGE(int i);

int Enemy_Fire_Judge(int i);
int Enemy_Move_Judge(int i, char direc);
int Enemy_Bullet_Judge(int i);
int Mc_Judge(char direct);
int Enemy_AI_Move(int i);
int Enemy_AI_Move_UD(int i);
void hero(double x, double y);
void F_Create_enemy(int j);
int Enem_AI_Move_LR(int i);


void drawMenu();
void startTimer(int id,int timeinterval);
void read(char name[], int score);

int MY_TIMER = 1;
int MOVE = 0;
int TIME = 100;
static int enable_rotation = 1;
double cx, cy, enemy_bullet_direction[999],Mc_bullet_direction[999];
int judge, enemy_bullet_judge[999] = {0,0,0,0,0,0}, enemy_bullet_start = 0,Mc_bullet_sum = 0;
int enemy_direction = 0, time_judge = 0,  death_sum = 0, start, sum = 0;

static int map[10][2];

time_t timep, _timep;

struct tm *p, *_p;


struct Score {
	int score;
	char name[30];
	struct Score *next;
};
struct Score *h;


struct Character{
	double x;
	double y;
	double k;
	int fire;
	int prefire;
	int death;

}Mc;



struct bullet{
	double x;
	double y;
	double k;
	double shot;
	
	int fire;
}M_bullet[999], enemy_bullet[3000];

struct BackG{
	double x;
	double y;
	double l;
}block;



static char name[30]="";

static int score;

void Main(){
	init();
	SetWindowTitle("SUPER MIAMI");
	mciSendString("play Untitled.mp3",NULL,0,NULL);
	mciSendString("play Untitled.mp3",NULL,0,NULL);
	time(&timep);
	p = gmtime(&timep);
	
	registerMouseEvent(MouseEventProcess); 
	registerKeyboardEvent(KeyboardEventProcess);
	registerCharEvent(CharEventProcess);
	registerTimerEvent(TimeEventProcess);
	
	startTimer(MY_TIMER, TIME/10);

}




void read(char name[], int score)
{
	FILE *fp;
	struct Score *tmp, *p;
	
	if ((fp = fopen("排行榜.txt", "a"))) {
		fprintf(fp,"%s %d\n",name,score);
		
		fclose(fp);
	}
	else {
		fp = fopen("排行榜.txt", "w");
			fprintf(fp,"%s %d\n",name,score);
			
		fclose(fp);
	}
}

void init(){
	InitGraphics();
	Mc.x = _width/60;
	Mc.y = _height/2;
	Mc.fire = 0;
	Mc.death = 0;
	Mc.prefire = 0;
	cx = 0;
	cy = 0;
	judge = 0;
	start = 0;
}


void display(){
	DisplayClear();
	if(death == 1) {
		DIE();
		start = 0;
	}
	else{
	if(chosen>0){
		if(start == 0){
			Mc.x = _width/60;
			Mc.y = _height/2;
			Mc.death = 0;
			Enemy_Clear();
			MAP(chosen-1);
			
			enemy_sum = map[chosen-1][0];
			back_sum = map[chosen-1][1];
			start = 1;

		}
		Draw();
	}
	else if(chosen == -1){
		if(strcmp(name, "")){
			if(start ==0 ){
			Mc.x = _width/60;
			Mc.y = _height/2;
			Mc.death = 0;
			Enemy_Clear();
			BIG_MAP();
			start = 1;
			warning=0;
		}
		
		Draw();
		}
		else{
			menuquick = 0;
			chosen = 0;
			warning=1;
			
		}
	}
	else{
		if(warning&&!menustart   &&  !menuabout && !menuquick&&!menurank)
			drawLabel(4.3- TextStringWidth("please input the name") , 5.9, "please input the name");
		strcpy(name, drawButtons());

	}
}

	time(&_timep);
	_p = gmtime(&_timep);
	if(p->tm_hour == _p->tm_hour){
		if((_p->tm_min - p->tm_min)>=5){
			mciSendString("close Untitled.mp3", NULL, 0, NULL );
			mciSendString("play Untitled.mp3",NULL,0,NULL);
		
		}
	}
	else if((_p->tm_min+60- p->tm_min)>=5){
		mciSendString("close Untitled.mp3", NULL, 0, NULL );
		mciSendString("play Untitled.mp3",NULL,0,NULL);

	}
}

void KeyboardEventProcess(int key, int event){
	MOVE = 0;
	if(chosen!=0){

	switch(event){
		case BUTTON_DOWN:
			Mc.prefire = 0;
			MOVE = 1;
			switch(key){
				case 0x57 :
					if(Mc_Judge('w'))
						break;
					if(chosen==-1 && Mc.y>=3*_height/4)
						MAP_MOVE('w');
					else if(Mc.y+ dy/2+_width/120>=_height){
						break;
					}
					else	
						Mc.y = Mc.y + dy/2;
					break;
				case 0x53 :
					if(Mc_Judge('s'))
						break;
					if(chosen==-1 && Mc.y<=_height/4)
						MAP_MOVE('s');	
					else if(Mc.y- dy/2-_width/120<=0){
						break;
					}
					else		
						Mc.y = Mc.y - dy/2;
					break;
				case 0x41 :
					if(Mc_Judge('a'))
						break;
					if(chosen==-1 && Mc.x<=_width/4)
						MAP_MOVE('a');
					else if(Mc.x- dx/2-_width/60<=0){
						break;
					}
					else	
						Mc.x = Mc.x - dx/2;
					break;
				case 0x44 :
					if(Mc_Judge('d'))
						break;
					if(chosen==-1 && Mc.x>=3*_width/4)
						MAP_MOVE('d');
					else if(Mc.x+ dx/2>=_width){
						break;
					}
					else	
						Mc.x = Mc.x + dx/2;
					break;
				case VK_ESCAPE:
					warning=0;
					menustart=0;
					menuabout=0;
					menuquick=0;
					menurank=0;
					chosen = 0;
					start = 0;
					score = 0;
					break;
				
			}
			break;
			
		
		case BUTTON_UP:
			MOVE = 0;
			break;

	}
	}
	else{
		switch(event){
		case BUTTON_DOWN:
			switch(key){
				case VK_F1:
					if(!strcmp(name,"")){
					chosen = -1;
					warning=0;
					}
					break;
				case VK_F2:
					chosen = 0;
					menustart = 1;
					warning=0;
					break;
				case VK_F3:
					menuabout = 1;
					warning=0;
					break;
				case VK_F4:
					exit(0);
				case VK_ESCAPE:
					menustart=0;
					menuabout=0;
					menuquick=0;
					menurank=0;
					chosen = 0;
					start = 0;
					score = 0;
					break;
			
			}
			break;
		}
	}

	uiGetKeyboard(key, event);
	display();
}


void MouseEventProcess(int x, int y, int button, int event){
	uiGetMouse(x,y,button,event);
	if(chosen!=0){
	cx = ScaleXInches(x);
	cy = _height - ScaleXInches(y);
	Mc.k = (cy - Mc.y)/(cx - Mc.x);
	switch(event){
		case BUTTON_DOWN: 
			switch(button){
				case LEFT_BUTTON:
					if(Mc.prefire == 0){
					if(judge == 0){
						judge = 1;
						
						M_bullet[Mc_bullet_sum].k = (cy-Mc.y)/(cx-Mc.x);
						M_bullet[Mc_bullet_sum].x = Mc.x;
						M_bullet[Mc_bullet_sum].y = Mc.y;
						M_bullet[Mc_bullet_sum].shot = 0;
			}
					if(cx>Mc.x) Mc_bullet_direction[Mc_bullet_sum] = 1;
					else Mc_bullet_direction[Mc_bullet_sum] = -1;
					Mc.fire = 1;
					Mc_bullet_sum++;
					if(Mc.prefire == 0) Mc.prefire = 1;
					break;
			}
			}
		case BUTTON_UP:
			switch(button){
				case LEFT_BUTTON:
					judge = 0;
					break;
			}
	}
	}
	
}


void CharEventProcess(char ch){
	uiGetChar(ch);
	
	display();
}

void TimeEventProcess(int timerID){
	if (timerID == MY_TIMER && enable_rotation){
	
		
		display();
	}
	
}


void Draw(){
	Draw_MC();
	Draw_Bullet_MC();
	Draw_Background();
	Draw_Enemy();
}



void Draw_MC(){
	MovePen(Mc.x, Mc.y);
	
	if(1){
		DrawArc(_width/120, 0, 360);
		pattern(Mc.x, Mc.y, _width/120);
	if(Mc.prefire == 0){	
		if(cx>=Mc.x){
			MovePen(Mc.x+(_width/120/sqrt(1+Mc.k*Mc.k))-_width/120, Mc.y+Mc.k*_width/120/sqrt(1+Mc.k*Mc.k));
			
			DrawLine(2.5*dx/sqrt(1+Mc.k*Mc.k) ,2.5*Mc.k*dx/sqrt(1+Mc.k*Mc.k) );
			MovePen(Mc.x+(_width/120/sqrt(1+Mc.k*Mc.k))-_width/120+3*dx/sqrt(1+Mc.k*Mc.k)+0.5*dx, Mc.y+Mc.k*_width/120/sqrt(1+Mc.k*Mc.k)+3*Mc.k*dx/sqrt(1+Mc.k*Mc.k));
			DrawArc(dx/2, 0, 360);
		}
		else {
			
			MovePen((Mc.x-(_width/120/sqrt(1+Mc.k*Mc.k))-_width/120), (Mc.y-Mc.k*_width/120/sqrt(1+Mc.k*Mc.k)));
			DrawLine(-2.5*dx/sqrt(1+Mc.k*Mc.k) ,-2.5*Mc.k*dx/sqrt(1+Mc.k*Mc.k) );
			MovePen((Mc.x-(_width/120/sqrt(1+Mc.k*Mc.k))-_width/120)-2.5*dx/sqrt(1+Mc.k*Mc.k)+0.5*dx-0.5*dx/sqrt(1+Mc.k*Mc.k),(Mc.y-Mc.k*_width/120/sqrt(1+Mc.k*Mc.k))-3*Mc.k*dx/sqrt(1+Mc.k*Mc.k));
			DrawArc(dx/2, 0, 360);
		}
	}
	}
	else{
	DrawLine(_width/60, 0);
	DrawLine(0, _height/30);
	DrawLine(-_width/60, 0);
	DrawLine(0, -_height/30);
	}
	Mc_Death();
	if(Mc.death == 1) {
		death = 1;
		if(chosen == -1)
		    read(name, score);
		strcpy(name,"");
	}	
}



void Draw_Enemy(){
	int i;
	for(i=0;i<enemy_sum;i++){
		if(enemy[i].death==0){
			MovePen(enemy[i].x, enemy[i].y);
			DrawArc(_width/120, 0, 360);
	}
	}
	Enemy_Judge();
	Draw_Bullet_Enemy();
	Enemy_AI();
	Death_Judge();

}


void Draw_Bullet_MC(){
		int i;
		double k,_k;
		if(Mc.fire == 1){
			
		for(i=0;i<Mc_bullet_sum;i++){
			k = (Theta-M_bullet[i].k)/(1-Theta*M_bullet[i].k);
			_k = (M_bullet[i].k-Theta)/(1+Theta*M_bullet[i].k);
			if(!M_bullet[i].shot){
			MovePen(M_bullet[i].x, M_bullet[i].y);
			DrawArc(dx/2,0,360);
			
			MovePen(M_bullet[i].x-1*Mc_bullet_direction[i]*0.5*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k) - 0.5*dx, M_bullet[i].y-1*M_bullet[i].k*Mc_bullet_direction[i]*0.5*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k));
			DrawLine(-1*Mc_bullet_direction[i]*5*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k), -1*M_bullet[i].k*Mc_bullet_direction[i]*5*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k));
			}
			if(Mc_bullet_Judge(i)){
			if(MOVE == 1){
				M_bullet[i].x = M_bullet[i].x + Mc_bullet_direction[i]*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k);
				M_bullet[i].y = M_bullet[i].y + Mc_bullet_direction[i]*M_bullet[i].k*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k);
			}
			else{
				M_bullet[i].x = M_bullet[i].x + Mc_bullet_direction[i]*0.1*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k);
				M_bullet[i].y = M_bullet[i].y + Mc_bullet_direction[i]*M_bullet[i].k*0.1*dx/sqrt(1+M_bullet[i].k*M_bullet[i].k);	
			}
			}
				
		}
		}

}


void Mc_Death(){
	int i,j;
	double h,l;
	
	for(i=0;i<enemy_sum;i++){	
			
		h = (Mc.y - enemy_bullet[i].y -enemy_bullet[i].k*( Mc.x -_width/120- enemy_bullet[i].x))/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k);
		if(h<0) h = -h;
		l = sqrt((enemy_bullet[i].x - Mc.x + _width/120)*(enemy_bullet[i].x - Mc.x + _width/120)+ (Mc.y - enemy_bullet[i].y)*(Mc.y - enemy_bullet[i].y));
		if(h<=_width/120 && l<=1.414*_width/120){
			Mc.death = 1;
			break;
			}
			
		}
	
}

void Draw_Bullet_Enemy(){
	int i;
	for(i=0;i<enemy_sum;i++){
	if(enemy_bullet_judge[i] == 1){
		enemy_bullet[i].k = enemy[i].k;
		enemy_bullet_judge[i] = 2;
		if(enemy_bullet[i].x<Mc.x)
		{
			enemy_bullet_direction[i] = 1;
		}
		else{
			enemy_bullet_direction[i] = -1;
		}
	}
	if((enemy_bullet[i].x<=_width && enemy_bullet[i].x>=0 && enemy_bullet[i].y<=_height && enemy_bullet[i].y>=0) && Enemy_Bullet_Judge(i)){
			MovePen(enemy_bullet[i].x, enemy_bullet[i].y);
			DrawLine(dx/3, 0);
			DrawLine(0, dx/3);
			DrawLine(-dx/3, 0);
			DrawLine(0, -dx/3);
			if(MOVE == 1){
				enemy_bullet[i].x = enemy_bullet[i].x + enemy_bullet_direction[i]*0.5*dx/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k);
				enemy_bullet[i].y = enemy_bullet[i].y + enemy_bullet_direction[i]*enemy_bullet[i].k*0.5*dx/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k);
			}
			else{
				enemy_bullet[i].x = enemy_bullet[i].x + enemy_bullet_direction[i]*0.01*dx/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k);
				enemy_bullet[i].y = enemy_bullet[i].y + enemy_bullet_direction[i]*enemy_bullet[i].k*0.01*dx/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k);
			}
			DrawLine(-1*enemy_bullet_direction[i]*5*dx/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k), -1*enemy_bullet_direction[i]*5*enemy_bullet[i].k*dx/sqrt(1+enemy_bullet[i].k*enemy_bullet[i].k));
			
		}
	else{
		enemy_bullet_judge[i] = 0;
		enemy_bullet[i].x = -2;
		enemy_bullet[i].y = -2;
		enemy_bullet_direction[i]=-1;
	}
}
}

void Draw_Background(){
	
	int i,j;
	double x_min, x_max, y_min, y_max;
	if(chosen > 0){
	for(i=0;i<back_sum;i++){
		MovePen(back[i].x, back[i].y);
		DrawLine(back[i].l_x, 0);
		DrawLine(0, back[i].l_y);
		DrawLine(-back[i].l_x, 0);
		DrawLine(0, -back[i].l_y);
		for(j=0;j<=(int)(back[i].l_y/(_width/50));j++){
			MovePen(back[i].x, back[i].y+back[i].l_y-j*_width/50);
			DrawLine((back[i].l_x/back[i].l_y)*j*_width/50, j*_width/50);
			MovePen(back[i].x+back[i].l_x, back[i].y+j*_width/50);
			DrawLine(-(back[i].l_x/back[i].l_y)*j*_width/50, -j*_width/50);
		}

	}
	}
	else if(chosen == -1){
		for(i=0;i<back_sum;i++){
			x_min = back[i].x;
			x_max = back[i].x + back[i].l_x;
			y_min = back[i].y;
			y_max = back[i].y + back[i].l_y;
			if(x_min>=0 && x_min<=_width){
				if((y_min>=0 && y_min<=_height) ||(y_max>=0 && y_max<=_height)){
					MovePen(back[i].x, back[i].y);
					DrawLine(back[i].l_x, 0);
					DrawLine(0, back[i].l_y);
					DrawLine(-back[i].l_x, 0);
					DrawLine(0, -back[i].l_y);
					continue;
				}
			}
			else if(x_max>=0 && x_max<=_width){
				if((y_min>=0 && y_min<=_height) ||(y_max>=0 && y_max<=_height)){
					MovePen(back[i].x, back[i].y);
					DrawLine(back[i].l_x, 0);
					DrawLine(0, back[i].l_y);
					DrawLine(-back[i].l_x, 0);
					DrawLine(0, -back[i].l_y);
					continue;
				}
			
			}
			
		}
		
		
	}
}

void Enemy_Clear(){
	int i;
	for(i=0;i<enemy_sum;i++){
		enemy[i].death = 1;
		enemy_bullet[i].x = -10;
		enemy_bullet[i].y = -10;
		
	}
	Mc_bullet_sum = 0;
	Mc_bullet_sum = 0;
	death_sum = 0;
	enemy_sum = 0;

}

void Enemy_Judge(){
	int i = 0, j = 0;
	for(i=0;i<enemy_sum;i++){
		if(enemy[i].death==0){
		enemy[i].k = (enemy[i].y - Mc.y)/(enemy[i].x - Mc.x);
		if(Enemy_Fire_Judge(i))
		{
		}
		else{
			if(enemy_bullet_judge[i] == 0){
			enemy_bullet[i].x = enemy[i].x;
			enemy_bullet[i].y = enemy[i].y;
			enemy_bullet_judge[i] = 1;
			break;
		}
	
		}

		}
	}


}

int Enemy_Fire_Judge( int i){
	int j;
	
	double k_max_1, k_max_2, k_min_1, k_min_2;
		for(j=0;j<back_sum;j++){
			
			if((back[j].x>=enemy[i].x && back[j].x>=Mc.x ) || ((back[j].x+back[j].l_x)<=enemy[i].x && (back[j].x+back[j].l_x)<=Mc.x) || (back[j].y>=enemy[i].y && back[j].y>=Mc.y) || ((back[j].y+back[j].l_y)<=enemy[i].y && (back[j].y+back[j].l_y)<=Mc.y))	
				continue;
			
			
			
				k_max_1 = (enemy[i].y-back[j].y)/(enemy[i].x-_width/120-back[j].x-back[j].l_x);
				k_min_1 = (enemy[i].y-back[j].y-back[j].l_y)/(enemy[i].x-_width/120-back[j].x);
			
				k_max_2 = (enemy[i].y-back[j].y)/(enemy[i].x-_width/120-back[j].x);
				k_min_2 = (enemy[i].y-back[j].y-back[j].l_y)/(enemy[i].x-_width/120-back[j].x-back[j].l_x);
			if(k_max_1<k_min_1){
				double temp = k_max_1;
				k_max_1 = k_min_1;
				k_min_1 = temp;
			}
			if(k_max_2<k_min_2){
				double temp = k_max_2;
				k_max_2 = k_min_2;
				k_min_2 = temp;
			}
			
			if((k_max_1*k_min_1)>=0){
				if(enemy[i].k>=k_min_1 &&enemy[i].k<=k_max_1) {
					return 1;
				}
			}
			else{
				if(back[j].x<enemy[i].x && (back[j].x+back[j].l_x)>enemy[i].x){
					if(enemy[i].k>=k_max_1 || enemy[i].k<=k_min_1)
						return 1;
				}
				else{
					if(enemy[i].k>=k_min_1 &&enemy[i].k<=k_max_1) {
					return 1;
				}
				}
			}
			
			if((k_max_2*k_min_2)>=0){
				if(enemy[i].k>=k_min_2 &&enemy[i].k<=k_max_2) {
					return 1;
				}
			}
			else{
				if(back[j].x<enemy[i].x && (back[j].x+back[j].l_x)>enemy[i].x){
					if(enemy[i].k>=k_max_2 || enemy[i].k<=k_min_2)
						return 1;
				}
				else{
					if(enemy[i].k>=k_min_2 &&enemy[i].k<=k_max_2) {
					return 1;
				}
				}
			}
			
		}
	
	return 0;
}



void Enemy_AI(){
	int i;
	double e_d;
	for(i=0;i<enemy_sum;i++){
	if(enemy[i].death==0){	
		if(enemy[i].x<Mc.x)
		{
			e_d = 1;
		}
		else if(Mc.x<enemy[i].x){
			e_d = -1;
		}
		else{
			e_d = 0;
		}
		if(!Enemy_Fire_Judge(i)){
			if(MOVE == 1){
			if(e_d!=0){
				if(Enemy_AI_Move(i)){
					enemy[i].k = (enemy[i].y - Mc.y)/(enemy[i].x - Mc.x);
					enemy[i].x = enemy[i].x + e_d*0.1*dx/sqrt(1+enemy[i].k*enemy[i].k);
					enemy[i].y = enemy[i].y + e_d*enemy[i].k*0.1*dx/sqrt(1+enemy[i].k*enemy[i].k);
				}
				else if(Enemy_AI_Move_UD(i))
				{
					if(Mc.y>enemy[i].y ){
					enemy[i].y = enemy[i].y + 0.1*dx;
					}
					else if(Mc.y<enemy[i].y ) enemy[i].y = enemy[i].y - 0.1*dx;
				
				}
				else if(Enem_AI_Move_LR(i)){
					if(Mc.x>enemy[i].x) enemy[i].x = enemy[i].x + 0.1*dx;
					else enemy[i].x = enemy[i].x - 0.1*dx;
				}
			}
			else if(Mc.y>enemy[i].y){
				enemy[i].y = enemy[i].y + 0.1*dx;
			}
			else enemy[i].y = enemy[i].y - 0.1*dx;
		}
	
		}
	}
	}
}

int Enemy_AI_Move(int i){
	double x_min, x_max, y_min, y_max,e_d;
	int j;
	if(enemy[i].x<Mc.x)
		{
			e_d = 1;
		}
	else if(Mc.x<enemy[i].x){
			e_d = -1;
		}
	enemy[i].k = (enemy[i].y - Mc.y)/(enemy[i].x - Mc.x);
	x_min = enemy[i].x + e_d*0.1*dx/sqrt(1+enemy[i].k*enemy[i].k)-_width/60;
	x_max = enemy[i].x + e_d*0.1*dx/sqrt(1+enemy[i].k*enemy[i].k);
	y_min = enemy[i].y + e_d*enemy[i].k*0.1*dx/sqrt(1+enemy[i].k*enemy[i].k)-_width/120;
	y_max = enemy[i].y + e_d*enemy[i].k*0.1*dx/sqrt(1+enemy[i].k*enemy[i].k)+_width/120;

	for(j=0;j<back_sum;j++){
		if((x_min>back[j].x) && (x_min<(back[j].x+back[j].l_x))){
			if((y_min>back[j].y && y_min<(back[j].y+back[j].l_y)) || (y_max>back[j].y && y_max<(back[j].y+back[j].l_y))){
				return 0;
			}
		}
		else if((x_max>back[j].x) && (x_max<(back[j].x+back[j].l_x))){
			if((y_min>back[j].y && y_min<(back[j].y+back[j].l_y)) || (y_max>back[j].y && y_max<(back[j].y+back[j].l_y))){
				return 0;
			}
		}
		
	
	}
	return 1;
}

int Enemy_AI_Move_UD(int i){
	int j;
	double y_max_up = (enemy[i].y+0.1*dx+_width/120);
	double y_min_up = y_max_up - _width/60;
	double y_max_down = y_max_up - 0.2*dx;
	double y_min_down = y_min_up - 0.2*dx;
	
	double x_min= enemy[i].x -_width/60;
	double x_max= enemy[i].x;
	
	for(j=0;j<back_sum;j++){
		if(y_max_up>back[j].y && y_max_up<(back[j].y+back[j].l_y))
		{
			if(x_min>back[j].x && x_min<(back[j].x+back[j].l_x))
				return 0;
			if(x_max>back[j].x && x_max<(back[j].x+back[j].l_x))
				return 0;
		}
		if(y_min_up>back[j].y && y_min_up<(back[j].y+back[j].l_y)){
			if(x_min>back[j].x && x_min<(back[j].x+back[j].l_x))
				return 0;
			if(x_max>back[j].x && x_max<(back[j].x+back[j].l_x))
				return 0;
		}
	}
	return 1;
}

int Enem_AI_Move_LR(int i){
	int j;
	
	double x_min_left = enemy[i].x -_width/60 - 0.1*dx;
	double x_max_left = x_min_left + _width/60;
	double x_min_right = x_min_left+0.2*dx;
	double x_max_right = x_max_left + 0.2*dx;
	double y_max = enemy[i].y+_width/120;
	double y_min = enemy[i].y-_width/120;

	
	for(j=0;j<back_sum;j++){
	if(y_max>back[j].y && y_max<(back[j].y+back[j].l_y))
		{
			if(x_min_left>back[j].x && x_min_left<(back[j].x+back[j].l_x))
				return 0;
			if(x_max_right>back[j].x && x_max_right<(back[j].x+back[j].l_x))
				return 0;
		}
		if(y_min>back[j].y && y_min<(back[j].y+back[j].l_y)){
			if(x_min_left>back[j].x && x_min_left<(back[j].x+back[j].l_x))
				return 0;
			if(x_max_right>back[j].x && x_max_right<(back[j].x+back[j].l_x))
				return 0;
		}
	}
	return 1;
	
}

int Mc_Judge(char direct){
	int j;
	double x_min = Mc.x-_width/60;
	double x_max = Mc.x;
	double y_min = Mc.y-_width/120;
	double y_max = Mc.y+_width/120;
	
	switch(direct){
		case 'w':
			for(j=0;j<back_sum;j++){
				if((y_max+dx/2)>back[j].y && ((y_max+dx)<(back[j].y+back[j].l_y))){
					if((x_max>back[j].x && x_max<(back[j].x+back[j].l_x)) || (x_min>back[j].x && x_min<(back[j].x+back[j].l_x)))
						return 1;
				}
				}
			return 0;

		case 's':
			for(j=0;j<back_sum;j++){
				if((y_min-dx/2)<(back[j].y+back[j].l_y) && (y_min-dx)>back[j].y){
					if((x_max>back[j].x && x_max<(back[j].x+back[j].l_x)) || (x_min>back[j].x && x_min<(back[j].x+back[j].l_x)))
						return 1;
				}
				}
			return 0;

		case 'a':
			for(j=0;j<back_sum;j++){
				if((x_min-dx/2)<(back[j].x+back[j].l_x) && (x_min-dx)>back[j].x){
					if((y_min>back[j].y && y_min<(back[j].y+back[j].l_y)) || (y_max>back[j].y && y_max<(back[j].y+back[j].l_y)))
						return 1;
			
				}
			}
			return 0;
		case 'd':
			for(j=0;j<back_sum;j++){
				if((x_max+dx/2)>back[j].x && (x_max+dx)<(back[j].x+back[j].l_x)){
					if((y_min>back[j].y && y_min<(back[j].y+back[j].l_y)) || (y_max>back[j].y && y_max<(back[j].y+back[j].l_y)))
						return 1;
				}
			}
			
			return 0;


}
}

int Enemy_Bullet_Judge(int i){
	int j;
	double x_1 = enemy_bullet[i].x;
	double y_1 = enemy_bullet[i].y;
		for(j=0;j<back_sum;j++){
			if((x_1>=back[j].x) && (x_1<=(back[j].x+back[j].l_x)) && (y_1<=(back[j].y+back[j].l_y)) && (y_1>=back[j].y)){
				return 0;
			}
	}
		return 1;
}

int Mc_bullet_Judge(int i){
	int j;
	double x_1 = M_bullet[i].x;
	double y_1 = M_bullet[i].y;
		for(j=0;j<back_sum;j++){
			if((x_1>=back[j].x) && (x_1<=(back[j].x+back[j].l_x)) && (y_1<=(back[j].y+back[j].l_y)) && (y_1>=back[j].y)){
				return 0;
			}
	}
		return 1;
}

void Death_Judge(){
	int i,j,es = enemy_sum;
	double h,l;
	
	
	for(i=0;i<es;i++){
		for(j=0;j<Mc_bullet_sum;j++){
			if(enemy[i].death == 1) break;
			h = (enemy[i].y - M_bullet[j].y -M_bullet[j].k*( enemy[i].x -_width/120- M_bullet[j].x))/sqrt(1+M_bullet[j].k*M_bullet[j].k);
			if(h<0) h = -h;
			l = sqrt((M_bullet[j].x - enemy[i].x+_width/120)*(M_bullet[j].x - enemy[i].x+_width/120) + (M_bullet[j].y - enemy[i].y)*(M_bullet[j].y - enemy[i].y));
			if(h<=_width/120 && l<=1.414*_width/120){
				
				enemy[i].death = 1;
				death_sum++;
				
				if(death_sum == enemy_sum && chosen>0 && chosen<10){
					chosen++;
					Mc.x = _width/60;
					Mc.y = _height/2;
					
					Enemy_Clear();
					MAP(chosen-1);
			
					enemy_sum = map[chosen-1][0];
					back_sum = map[chosen-1][1];
				}
				if(chosen == -1){
					score++;
					F_Create_enemy(0);
					F_Create_enemy(50);
				}				
				break;
			}
			
		}
	}
}


int MAP_JUDGE(int i){
	int j;
	double x_min, x_max, y_min, y_max;

	x_min = back[i].x;
	x_max = back[i].x+back[i].l_x;
	y_min = back[i].y;
	y_max = back[i].y+back[i].l_y;
	
	for(j=0;j<i;j++){
		if(y_max>back[j].y && y_max<(back[j].y+back[j].l_y))
		{
			if(x_min>back[j].x && x_min<(back[j].x+back[j].l_x))
				return 0;
			if(x_max>back[j].x && x_max<(back[j].x+back[j].l_x))
				return 0;
		}
		if(y_min>back[j].y && y_min<(back[j].y+back[j].l_y)){
			if(x_min>back[j].x && x_min<(back[j].x+back[j].l_x))
				return 0;
			if(x_max>back[j].x && x_max<(back[j].x+back[j].l_x))
				return 0;
		}
	}
	return 1;
}

void MAP_MOVE(char d){
	int i;

	switch(d){
		case 'w':
			for(i=0;i<back_sum;i++){
				back[i].y = back[i].y - dx;
			}
			for(i=0;i<Mc_bullet_sum;i++){
				M_bullet[i].y-=dx; 
			}
			for(i=0;i<enemy_sum;i++){
				enemy[i].y-=dx;
			}
			for(i=0;i<enemy_sum;i++){
				enemy_bullet[i].y-=dx;
			}
			break;
		case 's':
			for(i=0;i<back_sum;i++){
				back[i].y = back[i].y + dx;
			}
			for(i=0;i<Mc_bullet_sum;i++){
				M_bullet[i].y+=dx; 
			}
			for(i=0;i<enemy_sum;i++){
				enemy[i].y+=dx;
			}
			for(i=0;i<enemy_sum;i++){
				enemy_bullet[i].y+=dx;
			}
			break;
		case 'a' :
			for(i=0;i<back_sum;i++){
				back[i].x = back[i].x + dx;
			}
			for(i=0;i<Mc_bullet_sum;i++){
				M_bullet[i].x+=dx; 
			}
			for(i=0;i<enemy_sum;i++){
				enemy[i].x+=dx;
			}
			for(i=0;i<enemy_sum;i++){
				enemy_bullet[i].x+=dx;
			}
			break;
		case 'd' :
			for(i=0;i<back_sum;i++){
				back[i].x = back[i].x - dx;
			}
			for(i=0;i<Mc_bullet_sum;i++){
				M_bullet[i].x-=dx; 
			}
			for(i=0;i<enemy_sum;i++){
				enemy[i].x-=dx;
			}
			for(i=0;i<enemy_sum;i++){
				enemy_bullet[i].x-=dx;
			}
			break;
	}

}

void F_Create_enemy(int j){
	int i=0;
	enemy_sum++;
	enemy[enemy_sum].death = 0;
	srand((unsigned)time(NULL));
	enemy[enemy_sum].x = _width*((rand()+j)%100)/100;
	srand((unsigned)time(NULL));
	enemy[enemy_sum].y = _height*((rand()+j)%100)/100;
	while(!Enemy_Create_Judge()){
		srand((unsigned)time(NULL));
		enemy[enemy_sum].x = _width*((rand()+i+j)%100)/100;
		srand((unsigned)time(NULL));
		enemy[enemy_sum].y = _height*((rand()+i+j)%100)/100;
		i++;
	}
}

int Enemy_Create_Judge(){
	int j;
	double x_min = enemy[enemy_sum].x-_width/60;
	double x_max = x_min+_width/60;
	double y_min = enemy[enemy_sum].y-_width/120;
	double y_max = y_min+_width/60;
	
	for(j=0;j<back_sum;j++){
		if((x_min>back[j].x) && (x_min<(back[j].x+back[j].l_x))){
			if((y_min>back[j].y && y_min<(back[j].y+back[j].l_y)) || (y_max>back[j].y && y_max<(back[j].y+back[j].l_y))){
				return 0;
			}
		}
		else if((x_max>back[j].x) && (x_max<(back[j].x+back[j].l_x))){
			if((y_min>back[j].y && y_min<(back[j].y+back[j].l_y)) || (y_max>back[j].y && y_max<(back[j].y+back[j].l_y))){
				return 0;
			}
		}
		
	
	}
	return 1;


}






