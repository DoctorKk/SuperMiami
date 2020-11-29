#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"
#include<math.h>

#define PI 3.1415926
#define a 0.05 //六边形边长

static double winwidth, winheight;   // 窗口尺寸
double angle_0 = 0.0;
double angle_1 = 0.0;

void turn(double angle)
{
	angle_1 += angle;
}

void move(double distance)
{
	MovePen((double)GetCurrentX() + distance * cos(angle_1*PI / 180), (double)GetCurrentY() + distance * sin(angle_1*PI / 180));
}

void forward(double distance)
{
	DrawLine( distance * cos(angle_1*PI / 180), distance * sin(angle_1*PI / 180));
}

void pattern(double x, double y, double r)
{
	int i,j;

	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();

	MovePen(x-r, y);//将图案中心定在显示中央
	for (i = 0; i <= 4; i++)//六边形旋转18次
	{
		turn(30.0);
		forward(a);
		//move(a);
		for (j = 0; j < 5; j++)
		{
			turn(60);
			forward(a);
			//move(a);
		}
		turn(30.0);//画一个六边形

		angle_0 += 90.0;//起始角度转过20度
		angle_1 = angle_0;
	}
}
