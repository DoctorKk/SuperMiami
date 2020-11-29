#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"



// ȫ�ֱ���
static double winwidth, winheight;   // ���ڳߴ�
static int    menuquick;
static int    menuabout;
static int    menustart;
static int    menurank;
static int    chosen;
static int    death;
static int    name_score;
static int    whether_name;
static int    whether_score;
static char name[30];
static char initname[30];
static char newname[30];
static char allname[1000][30];
static int allscore[1000];
static char allinfo[1000][40];
static int warning;



char* drawButtons(){

	static char * menuListGame[] = {"��Ϸ",  
		"����|F1", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"��ҳ| ESC",
		"ѡ��| F2",
		"�˳�| F4"};
	static char * menuListOther[] = {"����",
		"������Ա|F3"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double _x = 0; //fH/8;
	double _y = winheight;
	double _h = fH*1.5; // �ؼ��߶�
	double _w = TextStringWidth(menuListGame[0])*2; // �ؼ����
	double wlist = TextStringWidth(menuListOther[1])*1.2;
	double xindent = winheight/20; // ����
	int    selection;
	
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����	
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();
	death = 0;
	

	
	
	if (!menustart   &&  !menuabout && !menuquick&&!menurank) {
		SetPointSize(100);
		SetPenColor("Red");
		drawLabel(1.85 , 3.35, "SUPER MIAMI");
		SetPointSize(10);
		drawLabel(3- TextStringWidth("�û���") , 6.35, "�û���");
		if( textbox(GenUIID(0),3,6.25,w,h,name,sizeof(name)) )	{
		    sscanf(name,"%s",initname);
    	}	
		SetPenColor("Blue");
		drawLabel(winwidth / 2 - TextStringWidth("��ҳ") / 2, 0.1, "��ҳ");
		if (button(GenUIID(0), 1, 0.5 + h, w, h, "������Ϸ")){
			chosen = -1;
			warning=0;
		}
		if (button(GenUIID(0), 1, 0.3, w, h, "��ʼ��Ϸ")){
			warning=0;
			menustart = 1;
		}
		
		if (button(GenUIID(0), 7, 0.3, w, h, "�˳�")){
			exit(-1);
			warning=0;
		}
		if (button(GenUIID(0), 7, 6.25, w, h, "���а�")){
			menurank=1;
			warning=0;
		}
		if (button(GenUIID(0), 7, 6.25-h-0.2, w, h, "�ر�����")){
			mciSendString("close Untitled.mp3", NULL, 0, NULL );
		}
		if (button(GenUIID(0), 7, 6.25-2*h-0.4, w, h, "������")){
			mciSendString("close Untitled.mp3", NULL, 0, NULL );
			mciSendString("play Untitled.mp3",NULL,0,NULL);
		}
		
	}
	/*
	if(menuquick) {
		drawLabel(winwidth / 2 - TextStringWidth("���ٿ�ʼ����") / 2, 0.1, "���ٿ�ʼ����");
		if (button(GenUIID(0), 6.5, 0.3, w - 1, h, "��ʼ")) {
			menuquick = 0;
			chosen = -1;
		}
		if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "����")) {
			menuquick = 0;
		}
	}
	*/
	if (menustart) {
		SetPenColor("Blue");
		drawLabel(winwidth / 2 - TextStringWidth("��ѡ��ؿ�") / 2, 6, "��ѡ��ؿ�");
		drawLabel(winwidth / 2 - TextStringWidth("ѡ�ؽ���") / 2, 0.1, "ѡ�ؽ���");
		if (button(GenUIID(0), 0.5, 0.3, w-1, h, "����")) {
			menustart = 0;
			warning=0;
		}
		
		if (button(GenUIID(0), 2*winwidth/13, 2, winwidth/13, 0.8, "6")){
			chosen = 6;
		}
		if (button(GenUIID(0), 4*winwidth/13, 2, winwidth/13, 0.8, "7")){
			chosen = 7;
		}
		if (button(GenUIID(0), 6*winwidth/13, 2, winwidth/13, 0.8, "8")){
			chosen = 8;
		}
		if (button(GenUIID(0), 8*winwidth/13, 2, winwidth/13, 0.8, "9")){
			chosen = 9;
		}
		if (button(GenUIID(0), 10*winwidth/13, 2, winwidth/13, 0.8, "10")){
			chosen = 10;
		}
		
		if (button(GenUIID(0), 2*winwidth/13, 3.6, winwidth/13, 0.8, "1")){
			chosen = 1;
		}
		if (button(GenUIID(0), 4*winwidth/13, 3.6, winwidth/13, 0.8, "2")){
			chosen = 2;
		}
		if (button(GenUIID(0), 6*winwidth/13, 3.6, winwidth/13, 0.8, "3")){
			chosen = 3;
		}
		if (button(GenUIID(0), 8*winwidth/13, 3.6, winwidth/13, 0.8, "4")){
			chosen = 4;
		}
		if (button(GenUIID(0), 10*winwidth/13, 3.6, winwidth/13, 0.8, "5")){
			chosen = 5;
		}
	}

	if (menuabout) {
		SetPenColor("Blue");
		drawLabel(winwidth / 2 - TextStringWidth("������Ա") / 2, 6, "������Ա");
		drawLabel(winwidth / 2 - TextStringWidth("������") / 2, 5, "������");
		drawLabel(winwidth / 2 - TextStringWidth("����") / 2, 4, "����");
		drawLabel(winwidth / 2 - TextStringWidth("����") / 2, 3, "����");
		drawLabel(winwidth / 2 - TextStringWidth("������Ա����") / 2, 0.1, "������Ա����");
		if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "����")) {
			menuabout = 0;
			warning=0;
		}
	}	
	if(menurank) {
		int i,j,k,tmp,index,num;
		char tmp_[40];
		int max=-1;
		FILE *fp;
		int init_y = 5;
		drawLabel(winwidth*3/4  - TextStringWidth("���а����") / 2, 0.1, "���а����");
		if(fp=fopen("���а�.txt","r")){
			i=0;
			drawLabel(winwidth / 2 - TextStringWidth("�߷����а�") / 2, 6, "�߷����а�");
			rewind(fp);
			num=0;
			while(!feof(fp)){
				
				fscanf(fp,"%s %d",allname[i],&allscore[i]);
				i++;
			}
			num=i;
			rewind(fp);
			//
			i=0;
			while(!feof(fp)){
				fgets(allinfo[i],400,fp);
				i++;
			}
			k=0;
			
			while(k<5&&k<num){
				max=-1;
				for(i=k;i<1000;i++){
				    if(allscore[i] > max){
				    	index=i;
				    	max = allscore[i];
				    }			
			    }				
			drawLabel(winwidth / 2 - TextStringWidth(allinfo[index]) / 2, init_y, allinfo[index]);
			     allscore[index]=allscore[k];
				 strcpy(allinfo[index],allinfo[k]);
			init_y=init_y-0.00001;
			k++;
			}			
			fclose(fp);
		}
		else{
			drawLabel(winwidth / 2 - TextStringWidth("�߷����а�") / 2, 6, "�߷����а�");
			drawLabel(winwidth / 2 - TextStringWidth("��������") / 2, 5, "��������");
		}
		if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "����")) {
			warning=0;
			menurank = 0;
		}
	}
    
	
	// menu bar
	drawMenuBar(0,_y-_h,winwidth,_h);
	// File �˵�
	selection = menuList(GenUIID(0), _x, _y-_h, _w, wlist, _h, menuListGame, sizeof(menuListGame)/sizeof(menuListGame[0]));
	if( selection>0 ) selectedLabel = menuListGame[selection];
	if(selection==1){
		if(strcmp(name,"")){
		warning=0;
		menuquick=1;
		menustart=0;
		menuabout=0;
		menurank=0;
		if (button(GenUIID(0), 6.5, 0.3, _w - 1, _h, "��ʼ")) {

			menuquick = 0;

			chosen = -1;

		}

		if (button(GenUIID(0), 0.5, 0.3, _w - 1, _h, "����")) {
			warning=0;
			menuquick = 0;

		}
		}
		else {
			if(!menustart   &&  !menuabout && !menuquick&&!menurank)
			    warning=1;
		}
	}
	if(selection==2){
		warning=0;
		menuquick=0;
		menustart=0;
		menuabout=0;
		menurank=0;
		if (button(GenUIID(0), 1, 0.5 + _h, _w, _h, "������Ϸ"))

			menuquick = 1;

		if (button(GenUIID(0), 1, 0.3, _w, _h, "��ʼ��Ϸ"))

			menustart = 1;

		if (button(GenUIID(0), 1, 6.25, _w, _h, "����"))

			menuabout = 1;

		if (button(GenUIID(0), 7, 6.25, _w, _h, "�˳�"))

			exit(-1);
	}
	if(selection==3){
		warning=0;
		menuquick=0;
		menustart=1;
		menuabout=0;
		menurank=0;
		drawLabel(winwidth / 2 - TextStringWidth("��ѡ��ؿ�") / 2, 6, "��ѡ��ؿ�");

		

		if (button(GenUIID(0), 0.5, 0.3, _w-1, _h, "����")) {
			warning=0;
			menustart = 0;

		}

		

		if (button(GenUIID(0), 2*winwidth/13, 2, winwidth/13, 0.8, "6")){

			chosen = 6;

		}

		if (button(GenUIID(0), 4*winwidth/13, 2, winwidth/13, 0.8, "7")){

			chosen = 7;

		}

		if (button(GenUIID(0), 6*winwidth/13, 2, winwidth/13, 0.8, "8")){

			chosen = 8;

		}

		if (button(GenUIID(0), 8*winwidth/13, 2, winwidth/13, 0.8, "9")){

			chosen = 9;

		}

		if (button(GenUIID(0), 10*winwidth/13, 2, winwidth/13, 0.8, "10")){

			chosen = 10;

		}

		

		if (button(GenUIID(0), 2*winwidth/13, 3.6, winwidth/13, 0.8, "1")){

			chosen = 1;

		}

		if (button(GenUIID(0), 4*winwidth/13, 3.6, winwidth/13, 0.8, "2")){

			chosen = 2;

		}

		if (button(GenUIID(0), 6*winwidth/13, 3.6, winwidth/13, 0.8, "3")){

			chosen = 3;

		}

		if (button(GenUIID(0), 8*winwidth/13, 3.6, winwidth/13, 0.8, "4")){

			chosen = 4;

		}

		if (button(GenUIID(0), 10*winwidth/13, 3.6, winwidth/13, 0.8, "5")){

			chosen = 5;

		}

	}
	if( selection==4 )
		exit(-1); // choose to exit
	
	// Other �˵�
	selection = menuList(GenUIID(0),_x+_w,  _y-_h, _w, wlist,_h, menuListOther,sizeof(menuListOther)/sizeof(menuListOther[0]));
	if( selection>0 ) selectedLabel = menuListOther[selection];
	if( selection==1 ){
		warning=0;
		menuquick=0;
		menustart=0;
		menuabout=1;
		menurank=0;
		drawLabel(winwidth / 2 - TextStringWidth("������Ա") / 2, 6, "������Ա");

		drawLabel(winwidth / 2 - TextStringWidth("������") / 2, 5, "������");

		drawLabel(winwidth / 2 - TextStringWidth("����") / 2, 4, "����");

		drawLabel(winwidth / 2 - TextStringWidth("����") / 2, 3, "����");

		if (button(GenUIID(0), 0.5, 0.3, _w - 1, _h, "����")) {
			warning=0;
			menuabout = 0;

		}
	}
	
	return name;
}

void DIE(){
	double w = winwidth / 5;
	double h = GetFontHeight() * 2; 
	if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "����")){
		menuquick=0;
		menustart=0;
		menuabout=0;
		menurank=0;
		chosen = 0;
		death = 0;
		warning=0;
	}
	
	SetPointSize(100);
	SetPenColor("Red");
	drawLabel(winwidth / 2 - TextStringWidth("��") / 2, winheight/2, "��");
	SetPointSize(10);
	SetPenColor("Blue");
	return;
}

void SUCCESS(){
	double w = winwidth / 5;
	double h = GetFontHeight() * 2; 
	if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "����")){
		warning=0;
		menuquick=0;
		menustart=0;
		menuabout=0;
		menurank=0;
		chosen = 0;
		death = 0;
	}
	
	SetPointSize(80);
	SetPenColor("Red");
	drawLabel(winwidth / 2 - TextStringWidth("Success!") / 2, winheight/2, "Success!");
	SetPointSize(10);
	SetPenColor("Blue");
	return;
}