#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "ball.h"
#include "table.h"
#define break 20

extern Table table;
extern double speed;
SYSTEMTIME old;
int count=break;

// table constructor
Table::Table()
{
	// set ball colours 16个小球的颜色都不同 
	balls[0].setColour(0.9, 0.9, 0.9); // white
	balls[1].setColour(1.0, 0.9, 0.2); // yellow
	balls[2].setColour(0.1, 0.2, 0.7); // dark blue
	balls[3].setColour(1.0, 0.0, 0.0); // red
	balls[4].setColour(0.4, 0.0, 0.5); // dark purple
	balls[5].setColour(1.0, 0.5, 0.3); // light orange
	balls[6].setColour(0.2, 0.8, 0.4); // green
	balls[7].setColour(1.0, 0.5, 0.8); // light purple
	balls[8].setColour(0.0, 0.0, 0.0); // black
	balls[9].setColour(0.6, 0.5, 0.2); // dark yellow
	balls[10].setColour(0.2, 0.5, 1.0); // blue
	balls[11].setColour(1.0, 0.2, 0.3); // light red
	balls[12].setColour(0.7, 0.0, 0.7); // purple
	balls[13].setColour(0.8, 0.3, 0.0); // orange
	balls[14].setColour(0.0, 0.3, 0.2); // dark green
	balls[15].setColour(0.5, 0.0, 0.0); // dark red

	// initialize
	time = 0;
	stickAngle = 90;
	player = 0;
	curNumber = 16;
	isPlay = false;
	isChange = false;
	reset();
	balls[0].setScore(-1);
}

// change stick angle
void Table::setStickAngle(double angle)
{
	stickAngle = angle;
}

int Table::getPlayer()
{
	return player;
}

void Table::switchPlayer()
{
	player = !player;
	GetLocalTime(&old);
}

int Table::getScores(int player)
{
	return scores[player];
}

void Table::setScores(int player, int offset)
{
	scores[player] += offset;	
}

// reset game state
void Table::reset()
{
	isPlay = false;
	// initial position for cue ball
	balls[0].setPosition(2, 0);
	balls[0].setoPosition(2, 0);
	balls[0].setSpeed(0, 0);
	balls[0].setVisible(true);
	balls[0].setoVisible(true);

	// initial position for other balls
	int k = 1;
	for (int i = 1; i < 6; i++)
	{
		for (int j = 1; j < i + 1; j++)
		{
			// use triangle shape
			double r = balls[k].getRadius();
			double x = (6 - i * 2) * r - 2;
			double z = (j * 2 - i - 1) * r;
			balls[k].setPosition(x, z);
			balls[k].setoPosition(x, z);
			balls[k].setSpeed(0, 0);
			balls[k].setVisible(true);
			balls[k].setoVisible(true);
			k++;
		}
	}
	scores[0] = 0;
	scores[1] = 0;
}

// return number of visible balls
int Table::visibleNumber()
{
	int number = 0;
	for (int i = 0; i < 16; i++)
	{
		if (balls[i].getVisible()) number++;
	}
	return number;
}

// return balls motion state
bool Table::moving()
{
	// detect moving balls
	for (int i = 0; i < 16; i++)
	{
		if (balls[i].moving()) return true;
	}

	// no balls are moving
	return false;
}


// update balls
void Table::update(int currentTime)
{
	double timeStep = 0.001; // update interval in seconds
	int i, j;
	// update the time until it catches up with current time
	while (time < currentTime)
	{
		// collision detection for each ball
		for (i = 0; i < 16; i++)   //遍历所有球
		{
			// collide ball with holes and cushions
			balls[i].collideHoles();
			balls[i].collideCushions();
			//if (balls[i].getVisible() == false) {//遇到小球不可见
			//	flag = true;
			//}
			// collide ball with other balls
			for (j = 0; j < i; j++)
			{
				balls[i].collideBall(balls[j]);
			}
		}

		// update ball speeds and positions
		for (i = 0; i < 16; i++)
		{
			balls[i].updateSpeed(timeStep);
			balls[i].updatePosition(timeStep);
		}

		// if cue ball is hidden, place it at the centre
		if (!moving() && !balls[0].getVisible())
		{
			balls[0].setPosition(0, 0);
			balls[0].setSpeed(0, 0);
			balls[0].setVisible(true);
			restore();
			table.switchPlayer();
		}

		// if all balls are hidden, reset the game
		if (visibleNumber() == 1) reset();

		time++;
	}
}

// 绘制字体
void glDrawString(unsigned char *str)
{
	GLYPHMETRICSFLOAT pgmf[1];

	HDC hDC = wglGetCurrentDC();
	HFONT hFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 1;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfOutPrecision = OUT_TT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN;
	lstrcpy(lf.lfFaceName, "宋体");
	hFont = CreateFontIndirect(&lf);
	//设置当前字体
	SelectObject(wglGetCurrentDC(), hFont);


	DWORD dwChar;
	int ListNum;
	for (size_t i = 0; i<strlen((char *)str); i++)
	{
		if (IsDBCSLeadByte(str[i]))
		{
			dwChar = (DWORD)((str[i] << 8) | str[i + 1]);
			i++;
		}
		else
			dwChar = str[i];
		ListNum = glGenLists(1);
		wglUseFontOutlines(hDC, dwChar, 1, ListNum, 0.0, 0.1f, WGL_FONT_POLYGONS, pgmf);
		glCallList(ListNum);
		glDeleteLists(ListNum, 1);
	}
}

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void Table::paintScore() {
	glPushMatrix();
	selectFont(24, ANSI_CHARSET, "Comic Sans MS");
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos3f(-4.5f, 0.4f, 2.0f);// 高度
	glTranslated(-3, 0, 0);
	char str1[100] = "P1:";
	char temp1[1000];
	sprintf_s(temp1, "%d", scores[1]);
	strcat_s(str1, temp1);
	//glTranslated(0,0, 0);
	drawString(str1);
	glPopMatrix();

	glPushMatrix();
	selectFont(24, ANSI_CHARSET, "Comic Sans MS");
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos3f(-4.5f, 0.4f, -1.5f);// 高度
	glTranslated(-3, 0, 0);
	char str2[1000] = "P2:";
	char temp[100];

	sprintf_s(temp, "%d", scores[0]);
	strcat_s(str2, temp);
	//glTranslated(0,0, 0);
	drawString(str2);
	glPopMatrix();
}

void drawPlayer() {

	if (table.getPlayer() == 1) {
		glPushMatrix();
		selectFont(24, ANSI_CHARSET, "Comic Sans MS");
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.5f, 0.5f, 0.5f);
		glRasterPos3f(-4.5f, 1.0f, -1.2f);// 高度
		glTranslated(-3, 0, 0);
		//glTranslated(0,0, 0);
		drawString("Player 2");
		glPopMatrix();

		glPushMatrix();
		selectFont(24, ANSI_CHARSET, "Comic Sans MS");
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 0.0f, 0.0f);
		glRasterPos3f(-4.5f, 1.0f, 2.1f);// 高度
		//glRasterPos3f(-5.0f, 1.0f, 0.5f);
		//glTranslated(0,0, 0);
		drawString("Player 1");
		glPopMatrix();
	}
	else if (table.getPlayer() == 0) {
		glPushMatrix();
		selectFont(24, ANSI_CHARSET, "Comic Sans MS");
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 0.0f, 0.0f);
		glRasterPos3f(-4.5f, 1.0f, -1.2f);// 高度
		//glRasterPos3f(-5.0f, 0.5f, 0.5f);// 高度
		glTranslated(-3, 0, 0);
		//glTranslated(0,0, 0);
		drawString("Player 2");
		glPopMatrix();

		glPushMatrix();
		selectFont(24, ANSI_CHARSET, "Comic Sans MS");
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.5f, 0.5f, 0.5f);
		glRasterPos3f(-4.5f, 1.0f, 2.1f);// 高度
		//glTranslated(0,0, 0);
		drawString("Player 1");
		glPopMatrix();
	}

}
void paintSpeed(double speed) {
	glPushMatrix();
	selectFont(24, ANSI_CHARSET, "Comic Sans MS");
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.6f, 0.8f);
	glRasterPos3f(-4.5f, 1.0f, 0.5f);// 高度
	glTranslated(-3, 0, 0);
	char str1[100] = "Speed:";
	char temp1[1000];
	if (speed > 18)speed = 18;
	if (speed <= 5) speed = 0;
	int percent = (int)(speed / 18 * 100);
	//printf("%d\n", percent);
	sprintf_s(temp1, "%d", percent);
	strcat_s(str1, temp1);
	//glTranslated(0,0, 0); 
	drawString(str1);
	glPopMatrix();

}
// draw table
void Table::draw()
{
	// draw floor
	glColor3d(0.5, 0.5, 0.4); // grey colour
	setShininess(0.0); // not shiny
	glPushMatrix();
	glTranslated(0, -2, 0);
	drawCuboid(20, 0.1, 20);
	glPopMatrix();

	// draw walls
	glColor3d(0.4, 0.5, 0.4); // light green colour
	glPushMatrix();
	glTranslated(-10, 0, 0); drawCuboid(0.1, 20, 20);
	glTranslated(+20, 0, 0); drawCuboid(0.1, 20, 20);
	glColor3d(0.3, 0.4, 0.3); // dark green colour
	glTranslated(-10, 0, -10); drawCuboid(20, 20, 0.1);
	glTranslated(0, 0, +20); drawCuboid(20, 20, 0.1);
	glPopMatrix();

	// 绘制计分板
	glColor3d(0.4, 0.3, 0.3); // brown
	glPushMatrix();
	glTranslated(-8, 0, 0);
	drawCuboid(0.1, 4, 6.5);//厚度 高度 宽度
	glPopMatrix();

	glColor3d(0.7, 0.7, 0.7); // white
	glPushMatrix();
	glTranslated(-7.9, 0.25, 0);
	drawCuboid(0.1, 2, 6);//厚度 高度 宽度
	glPopMatrix();

	glPushMatrix();
	glTranslated(-8, 2, 3.3);//越小越远， 越大越高，  越大越左
	glRotated(90, 0, 1, 0);
	unsigned char cstr[] = { "Billiards Game" };
	glDrawString(cstr);
	glPopMatrix();

	drawPlayer();
	paintScore();
	paintSpeed(speed);
	// draw 3 lamps
	for (int i = -1; i <= 1; i++)
	{
		glPushMatrix();

		// lamp shade
		glColor3d(1.0, 0.3, 0.2); // red colour
		setShininess(0.0); // not shiny
		glTranslated(i * 2.3, 2.8, 0);
		glRotated(-90, 1, 0, 0);
		drawCylinder(0.8, 0, 1.1);

		// light bulb
		glColor3d(2.0, 2.0, 2.0); // white colour
		drawSphere(0.25);

		// lamp mount
		setShininess(1.0); // shiny
		glTranslated(0, 0, 0.8);
		glColor3d(1.0, 0.8, 0.4); // yellow colour
		drawCylinder(0.2, 0.2, 0.2);
		glTranslated(0, 0, 0.2);
		glColor3d(0.9, 0.7, 0.4); // yellow colour
		drawCylinder(0.2, 0.04, 0.1);
		glColor3d(0.8, 0.6, 0.4); // yellow colour
		drawCylinder(0.04, 0.04, 10);

		glPopMatrix();
	}

	double hsize = 0.5; // hole size

						// draw table surface
	glColor3d(0.3, 0.5, 0.6); // light blue colour
	setShininess(0.0); // not shiny
	glPushMatrix();
	glTranslated(0, -0.1, 0);
	drawCuboid(8.4, 0.2, 4.4);
	glPopMatrix();

	// draw table cushions
	glColor3d(0.1, 0.3, 0.5); // dark blue colour
	glPushMatrix();
	glTranslated(-4, 0, +0); drawCuboid(0.1, 0.2, 4.0 - hsize);
	glTranslated(+8, 0, +0); drawCuboid(0.1, 0.2, 4.0 - hsize);
	glTranslated(-2, 0, -2); drawCuboid(4.0 - hsize, 0.2, 0.1);
	glTranslated(-4, 0, +0); drawCuboid(4.0 - hsize, 0.2, 0.1);
	glTranslated(+0, 0, +4); drawCuboid(4.0 - hsize, 0.2, 0.1);
	glTranslated(+4, 0, +0); drawCuboid(4.0 - hsize, 0.2, 0.1);
	glPopMatrix();

	// draw table edges
	glColor3d(0.3, 0.2, 0.2); // brown colour
	setShininess(1.0); // shiny
	glPushMatrix();
	glTranslated(-4.2, 0, +0.0); drawCuboid(0.3, 0.3, 4.7);
	glTranslated(+8.4, 0, +0.0); drawCuboid(0.3, 0.3, 4.7);
	glTranslated(-4.2, 0, -2.2); drawCuboid(8.2, 0.3, 0.3);
	glTranslated(+0.0, 0, +4.4); drawCuboid(8.2, 0.3, 0.3);
	glPopMatrix();

	// draw table holes
	glColor3d(0.0, 0.0, 0.0); // black colour
	setShininess(0.0); // not shiny
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glTranslated(+4, 2, 0); glutSolidCone(hsize * 0.4, 0.01, 20, 1);
	glTranslated(-4, 0, 0); glutSolidCone(hsize * 0.4, 0.01, 20, 1);
	glTranslated(-4, 0, 0); glutSolidCone(hsize * 0.4, 0.01, 20, 1);
	glTranslated(0, -4, 0); glutSolidCone(hsize * 0.4, 0.01, 20, 1);
	glTranslated(+4, 0, 0); glutSolidCone(hsize * 0.4, 0.01, 20, 1);
	glTranslated(+4, 0, 0); glutSolidCone(hsize * 0.4, 0.01, 20, 1);
	glPopMatrix();

	// draw table legs
	glColor3d(0.2, 0.2, 0.2); // black colour
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(+3.5, +1.5, 0.1); drawCylinder(0.2, 0.2, 2);
	glTranslated(+0.0, -3.0, 0.0); drawCylinder(0.2, 0.2, 2);
	glTranslated(-7.0, +0.0, 0.0); drawCylinder(0.2, 0.2, 2);
	glTranslated(+0.0, +3.0, 0.0); drawCylinder(0.2, 0.2, 2);
	glPopMatrix();

	// draw balls
	for (int b = 0; b < 16; b++)
	{
		balls[b].draw();
	}

	// draw stick
	if (!moving())
	{
		glColor3d(0.7, 0.6, 0.5); // yellow colour
		glPushMatrix();
		glTranslated(balls[0].getX(), 0, balls[0].getZ());
		glRotated(stickAngle, 0, 1, 0);
		if (speed < 18)
		{
			glTranslated(0, 0.15, 0.4+speed*0.04);
		}
		else
		{
			glTranslated(0, 0.15, 0.4+18*0.04);
		}
		glRotated(-5, 1, 0, 0);
		drawCylinder(0.02, 0.06, 5.0);
		glPopMatrix();
	}

	if (!moving()) 
	{//所有球静止了

		if (curNumber == visibleNumber() && isPlay && isChange) {//犯规情况下，球个数不变；没打进，球个数不变这些情况都要交换球权
			switchPlayer();//交换球权
			renew(); //update the original position
		}
		else 
		{
			if (balls[0].getVisible())
			{
				curNumber = visibleNumber();//更新当前数量的球	
				renew();
			}
			else
			{
				restore();
				switchPlayer();
			}			
		}			
		isChange = false;//防止重复更新
	}
	else 
	{
		isChange = true;
	}
	if (!moving())
	{
		SYSTEMTIME now;
		GetLocalTime(&now);
		//printf("old=%d", old.wSecond);
		//printf("now=%d", now.wSecond);
		if (now.wSecond > old.wSecond)
		{
			count = break - (now.wSecond - old.wSecond);
		}
		else
		{
			count = break - (old.wSecond - now.wSecond);
		}
	}
	if (count <= 0)
	{
		switchPlayer();
	}
	glPushMatrix();
	selectFont(24, ANSI_CHARSET, "Comic Sans MS");
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos3f(-4.5f, 0.4f, 0.8f);// 高度
	glTranslated(-3, 0, 0);
	char str2[1000] = "Rest Time:";
	char temp[100];
	sprintf_s(temp, "%ds", count);
	strcat_s(str2, temp);
	drawString(str2);
	glPopMatrix();
}

// restore the last state
void Table::restore()
{
	// restore all balls
	for (int i = 0; i <= 15; i++)
	{
		balls[i].resetPosition();
		balls[i].resetVisible();
	}
}

// renew the state
void Table::renew()
{
	// restore all balls
	for (int i = 0; i <= 15; i++)
	{
		balls[i].setoPosition(balls[i].getX(), balls[i].getZ());
		balls[i].setoVisible(balls[i].getVisible());
	}
}




// shoot cue ball
void Table::shoot()
{
	isPlay = true;
	double a = stickAngle * pi / 180 + pi;
	if (speed > 18) speed = 18;

	//double speed = 18;
	// set cue ball speed along the stick
	balls[0].setSpeed(speed * sin(a), speed * cos(a));
}
