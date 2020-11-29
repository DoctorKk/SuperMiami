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



// 全局变量
static double winwidth, winheight;   // 窗口尺寸
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

	static char * menuListGame[] = {"游戏",  
		"快速|F1", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"首页| ESC",
		"选关| F2",
		"退出| F4"};
	static char * menuListOther[] = {"其它",
		"开发人员|F3"};
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double _x = 0; //fH/8;
	double _y = winheight;
	double _h = fH*1.5; // 控件高度
	double _w = TextStringWidth(menuListGame[0])*2; // 控件宽度
	double wlist = TextStringWidth(menuListOther[1])*1.2;
	double xindent = winheight/20; // 缩进
	int    selection;
	
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度	
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();
	death = 0;
	

	
	
	if (!menustart   &&  !menuabout && !menuquick&&!menurank) {
		SetPointSize(100);
		SetPenColor("Red");
		drawLabel(1.85 , 3.35, "SUPER MIAMI");
		SetPointSize(10);
		drawLabel(3- TextStringWidth("用户名") , 6.35, "用户名");
		if( textbox(GenUIID(0),3,6.25,w,h,name,sizeof(name)) )	{
		    sscanf(name,"%s",initname);
    	}	
		SetPenColor("Blue");
		drawLabel(winwidth / 2 - TextStringWidth("首页") / 2, 0.1, "首页");
		if (button(GenUIID(0), 1, 0.5 + h, w, h, "快速游戏")){
			chosen = -1;
			warning=0;
		}
		if (button(GenUIID(0), 1, 0.3, w, h, "开始游戏")){
			warning=0;
			menustart = 1;
		}
		
		if (button(GenUIID(0), 7, 0.3, w, h, "退出")){
			exit(-1);
			warning=0;
		}
		if (button(GenUIID(0), 7, 6.25, w, h, "排行榜")){
			menurank=1;
			warning=0;
		}
		if (button(GenUIID(0), 7, 6.25-h-0.2, w, h, "关闭音乐")){
			mciSendString("close Untitled.mp3", NULL, 0, NULL );
		}
		if (button(GenUIID(0), 7, 6.25-2*h-0.4, w, h, "打开音乐")){
			mciSendString("close Untitled.mp3", NULL, 0, NULL );
			mciSendString("play Untitled.mp3",NULL,0,NULL);
		}
		
	}
	/*
	if(menuquick) {
		drawLabel(winwidth / 2 - TextStringWidth("快速开始界面") / 2, 0.1, "快速开始界面");
		if (button(GenUIID(0), 6.5, 0.3, w - 1, h, "开始")) {
			menuquick = 0;
			chosen = -1;
		}
		if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "返回")) {
			menuquick = 0;
		}
	}
	*/
	if (menustart) {
		SetPenColor("Blue");
		drawLabel(winwidth / 2 - TextStringWidth("请选择关卡") / 2, 6, "请选择关卡");
		drawLabel(winwidth / 2 - TextStringWidth("选关界面") / 2, 0.1, "选关界面");
		if (button(GenUIID(0), 0.5, 0.3, w-1, h, "返回")) {
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
		drawLabel(winwidth / 2 - TextStringWidth("开发人员") / 2, 6, "开发人员");
		drawLabel(winwidth / 2 - TextStringWidth("李晓文") / 2, 5, "李晓文");
		drawLabel(winwidth / 2 - TextStringWidth("赵洋") / 2, 4, "赵洋");
		drawLabel(winwidth / 2 - TextStringWidth("刘澈") / 2, 3, "刘澈");
		drawLabel(winwidth / 2 - TextStringWidth("开发人员界面") / 2, 0.1, "开发人员界面");
		if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "返回")) {
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
		drawLabel(winwidth*3/4  - TextStringWidth("排行榜界面") / 2, 0.1, "排行榜界面");
		if(fp=fopen("排行榜.txt","r")){
			i=0;
			drawLabel(winwidth / 2 - TextStringWidth("高分排行榜") / 2, 6, "高分排行榜");
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
			drawLabel(winwidth / 2 - TextStringWidth("高分排行榜") / 2, 6, "高分排行榜");
			drawLabel(winwidth / 2 - TextStringWidth("暂无数据") / 2, 5, "暂无数据");
		}
		if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "返回")) {
			warning=0;
			menurank = 0;
		}
	}
    
	
	// menu bar
	drawMenuBar(0,_y-_h,winwidth,_h);
	// File 菜单
	selection = menuList(GenUIID(0), _x, _y-_h, _w, wlist, _h, menuListGame, sizeof(menuListGame)/sizeof(menuListGame[0]));
	if( selection>0 ) selectedLabel = menuListGame[selection];
	if(selection==1){
		if(strcmp(name,"")){
		warning=0;
		menuquick=1;
		menustart=0;
		menuabout=0;
		menurank=0;
		if (button(GenUIID(0), 6.5, 0.3, _w - 1, _h, "开始")) {

			menuquick = 0;

			chosen = -1;

		}

		if (button(GenUIID(0), 0.5, 0.3, _w - 1, _h, "返回")) {
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
		if (button(GenUIID(0), 1, 0.5 + _h, _w, _h, "快速游戏"))

			menuquick = 1;

		if (button(GenUIID(0), 1, 0.3, _w, _h, "开始游戏"))

			menustart = 1;

		if (button(GenUIID(0), 1, 6.25, _w, _h, "关于"))

			menuabout = 1;

		if (button(GenUIID(0), 7, 6.25, _w, _h, "退出"))

			exit(-1);
	}
	if(selection==3){
		warning=0;
		menuquick=0;
		menustart=1;
		menuabout=0;
		menurank=0;
		drawLabel(winwidth / 2 - TextStringWidth("请选择关卡") / 2, 6, "请选择关卡");

		

		if (button(GenUIID(0), 0.5, 0.3, _w-1, _h, "返回")) {
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
	
	// Other 菜单
	selection = menuList(GenUIID(0),_x+_w,  _y-_h, _w, wlist,_h, menuListOther,sizeof(menuListOther)/sizeof(menuListOther[0]));
	if( selection>0 ) selectedLabel = menuListOther[selection];
	if( selection==1 ){
		warning=0;
		menuquick=0;
		menustart=0;
		menuabout=1;
		menurank=0;
		drawLabel(winwidth / 2 - TextStringWidth("开发人员") / 2, 6, "开发人员");

		drawLabel(winwidth / 2 - TextStringWidth("李晓文") / 2, 5, "李晓文");

		drawLabel(winwidth / 2 - TextStringWidth("赵洋") / 2, 4, "赵洋");

		drawLabel(winwidth / 2 - TextStringWidth("刘澈") / 2, 3, "刘澈");

		if (button(GenUIID(0), 0.5, 0.3, _w - 1, _h, "返回")) {
			warning=0;
			menuabout = 0;

		}
	}
	
	return name;
}

void DIE(){
	double w = winwidth / 5;
	double h = GetFontHeight() * 2; 
	if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "返回")){
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
	drawLabel(winwidth / 2 - TextStringWidth("蔡") / 2, winheight/2, "蔡");
	SetPointSize(10);
	SetPenColor("Blue");
	return;
}

void SUCCESS(){
	double w = winwidth / 5;
	double h = GetFontHeight() * 2; 
	if (button(GenUIID(0), 0.5, 0.3, w - 1, h, "返回")){
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