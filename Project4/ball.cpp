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
	//��������Ŀ��
	int slices = 60;//����
	int stacks = 60;//γ��
					//�Ի�
					//���ֵ���Խ�󻭳��������Խ�⻬
	gluSphere(quadricObject, radius, slices, stacks);
}

// draw cuboid using specified sizes
void drawCuboid(double width, double height, double depth)
{
	glPushMatrix();//��ջ ���浱ǰ����ϵ
	glScaled(width, height, depth);//��ʾ����ǰͼ����x,y,z��ֱ�Ŵ�Ϊԭ����n��
	glutSolidCube(1);//ʵ��
	glPopMatrix();//�ָ�ԭ��������ϵ
}

// draw closed cylinder using specified sizes
void drawCylinder(double baseRadius, double topRadius, double height)//������
{
	int slices = 30;
	int stacks = 1;

	// first cylinder cap  �ײ�����
	glPushMatrix();
	//glRotated(0, 0, 1, 0);
	gluDisk(quadricObject, 0, baseRadius, slices, stacks);
	glPopMatrix();

	//// cylinder side     
	gluCylinder(quadricObject, baseRadius, topRadius, height, slices, stacks);//�������壬��������

																			  //// second cylinder cap �����Ƹ�
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
	colour[2] = 5 * (float)s;//��ľ��淴�� ����
	colour[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colour);
}

// ball constructor ���캯��
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
void Ball::setVisible(bool v)//�Ƿ�ɼ�
{
	visible = v;
}

// return ball visibility state
bool Ball::getVisible()
{
	return visible;
}

// change ball original visibility state
void Ball::setoVisible(bool v)//�Ƿ�ɼ�
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
			visible = false;//�����жϣ����óɲ��ɼ�
			break;
		}
	}
}

// collide with table cushions
void Ball::collideCushions()
{
	// ignore hidden balls
	if (!visible) return;
	//�߽���ײ���䣬�ٶȸı� ֻҪ�ı䷽�򼴿�
	// reflect the speed vector components
	if (x - radius < -3.9 && sx < 0) sx = -sx;//�±�
	if (x + radius > +3.9 && sx > 0) sx = -sx;//�ϱ�
	if (z - radius < -1.9 && sz < 0) sz = -sz;//���
	if (z + radius > +1.9 && sz > 0) sz = -sz;//�ұ�
}

// collide with other ball
void Ball::collideBall(Ball& other)//�����漰��̨�����ײ
{
	// ignore hidden balls
	if (!visible) return;//��һ�����ɼ���return
	if (!other.visible) return;

	// no collision for distant balls
	double distance = distanceTo(other.x, other.z);
	if (distance * 0.5 > radius) return;//�������0.2,�������ľ���֮�ͣ�����ײ

										// normalized vector between the balls
										////if(distance < 0.001) return;
										//��׼������x,z
	double ux = (other.x - x) / distance;//cos��
	double uz = (other.z - z) / distance;//sin��

										 // normal speed components
	double n1 = sx * ux + sz * uz;//�ֽ��ٶȵ�����������
	double n2 = other.sx * ux + other.sz * uz;

	// ignore balls moving away
	if (n1 < n2) return;//�ܲ����ľͲ�׷�� 

						// exchange normal speed components
						//��ײ�Ĺ���
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
	double k = 4; // coefficient of friction  Ħ������

				  // stop slow balls
	if (s < k * timeStep)//ֹͣ����˶�
	{
		sx = 0;
		sz = 0;
		return;
	}

	// apply friction
	double f = 1 - k / s * timeStep;//v=v0-at=v0-ugt  k/sֻ��һ��ϵ��
	if (f < 0) f = 0;
	sx *= f;
	sz *= f;
}

// change ball position over time
void Ball::updatePosition(double timeStep)//���ƶ�
{
	// ignore hidden balls
	if (!visible) return;

	// change position according to speed
	x += sx * timeStep;
	z += sz * timeStep;
}

// draw ball
void Ball::draw()//����
{
	if (!visible) return;//�������Ĳ���

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