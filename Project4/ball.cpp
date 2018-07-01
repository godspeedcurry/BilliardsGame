#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "ball.h"
#include "table.h"

extern Table table;

// quadric object for GLU functions
GLUquadricObj* quadricObject = NULL;

// draw sphere using specified size
void drawSphere(double radius)
{
	//二次曲线目标
	int slices = 60;//经度
	int stacks = 60;//纬度
					//迷惑
					//这个值设的越大画出来的球就越光滑
	gluSphere(quadricObject, radius, slices, stacks);
}

// draw cuboid using specified sizes
void drawCuboid(double width, double height, double depth)
{
	glPushMatrix();//堆栈 保存当前坐标系
	glScaled(width, height, depth);//表示将当前图形沿x,y,z轴分别放大为原来的n倍
	glutSolidCube(1);//实心
	glPopMatrix();//恢复原来的坐标系
}

// draw closed cylinder using specified sizes
void drawCylinder(double baseRadius, double topRadius, double height)//画杆子
{
	int slices = 30;
	int stacks = 1;

	// first cylinder cap  底部灯罩
	glPushMatrix();
	//glRotated(0, 0, 1, 0);
	gluDisk(quadricObject, 0, baseRadius, slices, stacks);
	glPopMatrix();

	//// cylinder side     
	gluCylinder(quadricObject, baseRadius, topRadius, height, slices, stacks);//灯罩主体，杆子主体

																			  //// second cylinder cap 顶部灯盖
	glPushMatrix();
	glTranslated(0, 0, height);
	gluDisk(quadricObject, 0, topRadius, slices, stacks);
	glPopMatrix();
}

// change current shininess
void setShininess(double s)
{
	// set specular shininess
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	// set specular colour
	float colour[4];
	colour[0] = 5 * (float)s;
	colour[1] = 5 * (float)s;
	colour[2] = 5 * (float)s;//球的镜面反射 很亮
	colour[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colour);
}

// ball constructor 构造函数
Ball::Ball()
{
	setColour(0, 0, 0);
	setPosition(0, 0);
	setoPosition(0, 0);
	setSpeed(0, 0);
	setVisible(true);
	setoVisible(true);
	setScore(1);
	radius = 0.1;
}

// change ball colour
void Ball::setColour(double r0, double g0, double b0)
{
	r = r0;
	g = g0;
	b = b0;
}

// change ball position
void Ball::setPosition(double x0, double z0)
{
	x = x0;
	z = z0;
}

void Ball::setoPosition(double x0, double z0)
{
	ox = x0;
	oz = z0;
}

// return ball position
double Ball::getX()
{
	return x;
}

// return ball position
double Ball::getZ()
{
	return z;
}

// return ball original position
double Ball::getoX()
{
	return ox;
}

// return ball original position
double Ball::getoZ()
{
	return oz;
}

// change ball speed
void Ball::setSpeed(double sx0, double sz0)
{
	sx = sx0;
	sz = sz0;
}

// change ball visibility state
void Ball::setVisible(bool v)//是否可见
{
	visible = v;
}

// return ball visibility state
bool Ball::getVisible()
{
	return visible;
}

// change ball original visibility state
void Ball::setoVisible(bool v)//是否可见
{
	ovisible = v;
}

// return ball original visibility state
bool Ball::getoVisible()
{
	return ovisible;
}

int Ball::getScore()
{
	return score;
}

void Ball::setScore(int dscore)
{
	score = dscore;
}

// return ball radius
double Ball::getRadius()
{
	return radius;
}

// return ball motion state
bool Ball::moving()
{
	if (!visible) return false;
	return sx != 0 || sz != 0;
}

// calculate distance to specified position
double Ball::distanceTo(double px, double pz)
{
	// use 2D distance
	double dx = x - px;
	double dz = z - pz;
	return sqrt(dx * dx + dz * dz);
}

// collide with table holes
void Ball::collideHoles()
{
	// X and Z positions of 6 holes
	double holes[6][2] =
	{
		{ 4, 2 },
	    { 4, -2 },
	    { -4, 2 },
	    { -4, -2 },
	    { 0, 2 },
	    { 0, -2 }
	};
	int i;
	// for each hole
	for (i = 0; i < 6; i++)
	{
		// check distance to hole
		if (distanceTo(holes[i][0], holes[i][1]) < 0.3 && visible) {			
			table.setScores(table.getPlayer(), score);
			visible = false;//进洞判断，设置成不可见
			break;
		}
	}
}

// collide with table cushions
void Ball::collideCushions()
{
	// ignore hidden balls
	if (!visible) return;
	//边界碰撞反射，速度改变 只要改变方向即可
	// reflect the speed vector components
	if (x - radius < -3.9 && sx < 0) sx = -sx;//下边
	if (x + radius > +3.9 && sx > 0) sx = -sx;//上边
	if (z - radius < -1.9 && sz < 0) sz = -sz;//左边
	if (z + radius > +1.9 && sz > 0) sz = -sz;//右边
}

// collide with other ball
void Ball::collideBall(Ball& other)//这里涉及到台球的碰撞
{
	// ignore hidden balls
	if (!visible) return;//有一个不可见就return
	if (!other.visible) return;

	// no collision for distant balls
	double distance = distanceTo(other.x, other.z);
	if (distance * 0.5 > radius) return;//距离大于0.2,超过球心距离之和，不碰撞

										// normalized vector between the balls
										////if(distance < 0.001) return;
										//标准化距离x,z
	double ux = (other.x - x) / distance;//cosθ
	double uz = (other.z - z) / distance;//sinθ

										 // normal speed components
	double n1 = sx * ux + sz * uz;//分解速度到球心连线上
	double n2 = other.sx * ux + other.sz * uz;

	// ignore balls moving away
	if (n1 < n2) return;//跑不过的就不追了 

						// exchange normal speed components
						//碰撞的过程
	sx += n2 * ux - n1 * ux;
	sz += n2 * uz - n1 * uz;
	other.sx += n1 * ux - n2 * ux;
	other.sz += n1 * uz - n2 * uz;
}

// change ball speed over time
void Ball::updateSpeed(double timeStep)
{
	// ignore hidden balls
	if (!visible) return;

	double s = sqrt(sx * sx + sz * sz); // speed magnitude
	double k = 4; // coefficient of friction  摩擦阻力

				  // stop slow balls
	if (s < k * timeStep)//停止球的运动
	{
		sx = 0;
		sz = 0;
		return;
	}

	// apply friction
	double f = 1 - k / s * timeStep;//v=v0-at=v0-ugt  k/s只是一个系数
	if (f < 0) f = 0;
	sx *= f;
	sz *= f;
}

// change ball position over time
void Ball::updatePosition(double timeStep)//逐步移动
{
	// ignore hidden balls
	if (!visible) return;

	// change position according to speed
	x += sx * timeStep;
	z += sz * timeStep;
}

// draw ball
void Ball::draw()//画球
{
	if (!visible) return;//看不到的不画

						 // draw shadow
	glColor3d(0.1, 0.2, 0.3); // gray colour
	setShininess(0.0); // not shiny
	glPushMatrix();
	glTranslated(x, 0, z);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(radius, 0.01, 20, 1);
	glPopMatrix();

	// draw ball
	glColor3d(r, g, b); // ball colour
	setShininess(1.0); // shiny
	glPushMatrix();
	glTranslated(x, radius, z);
	glRotated(180, 1, 0, 0);
	drawSphere(radius);
	glPopMatrix();
}

void Ball::resetPosition()
{
	x = ox;
	z = oz;
}


void Ball::resetVisible()
{
	if (visible== false && ovisible == true && score > 0)
	{
		table.setScores(table.getPlayer(), -score);
	}
	visible = ovisible;
}