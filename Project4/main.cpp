#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "ball.h"
#include "table.h"
#include "sound.h"

Table table;
int value = 2;
int timers;
double speed = 1;
double cameraAngle1 = -90;//ƽ��Ƕ�
double cameraAngle2 = 25;//�����Ƕ�

						 // handle key presses
void keyboard(unsigned char key, int x, int y)//���̼����¼�
{
	switch (key)
	{
	case ' ':
		// reset game on Space key
		table.reset();
		break;
	case 27:
		// exit on Escape key
		exit(0);
		break;
	}
}

// handle special key presses
void special(int key, int x, int y)//�������ҷ��������������Ƕȣ�ʵ��λ�ñ任
{
	// rotate the camera
	switch (key)
	{
	case GLUT_KEY_LEFT:
		cameraAngle1 += 5;
		break;
	case GLUT_KEY_RIGHT:
		cameraAngle1 -= 5;
		break;
	case GLUT_KEY_UP:
		cameraAngle2 += 5;
		break;
	case GLUT_KEY_DOWN:
		cameraAngle2 -= 5;
	}

	// limit the angle
	if (cameraAngle2 < 10) cameraAngle2 = 10;//��ֹ�굽���׺ͳ����컨��
	if (cameraAngle2 > 80) cameraAngle2 = 80;
}
void selectMyFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}
void drawTextString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
					   // Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

//void paintSpeed(double speed) {
//	glPushMatrix();
//	selectMyFont(24, ANSI_CHARSET, "Comic Sans MS");
//	//glClear(GL_COLOR_BUFFER_BIT);
//	//glColor3f(0.5f, 0.0f, 0.0f);
//	//glRasterPos3f(-4.5f, 0.2f, 1.0f);// �߶�
//	//glTranslated(-3, 0, 0);
//	char str1[100] = "Speed:";
//	char temp1[1000];
//	int percent = (int)(speed / 18 * 100);
//	printf("%d\n", percent);
//	sprintf_s(temp1, "%d", percent);
//	strcat_s(str1, temp1);
//	//glTranslated(0,0, 0);
//	drawTextString(str1);
//	glPopMatrix();
//
//}
void timer(int timerid)
{
	if (timers == 1 && value == 1)
	{
		speed += 0.3;
		//paintSpeed(speed);
	}
	else if (timers == 2 && value == 1)
	{
		if (!table.moving()) table.shoot();
		speed = 1;
	}

	if (timers == 1) value = 1;
	else if (timers == 2) value = 2;
	
	glutTimerFunc(10, timer, 1);//10 ms 
}

// handle mouse clicks
void mouse(int button, int state, int x, int y)//�������¼�
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //����������������Ұ���,��ʼ��ʱ
	{
		timers = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		timers = 2;
		shootsound();
	}
}

// handle mouse motion
void passive(int x, int y)//����ƶ���û�а��£����Ƹ˵��ƶ�
{
	// adjust stick angle by left/right mouse movement
	int centre = glutGet(GLUT_WINDOW_WIDTH) / 2;
	table.setStickAngle((x - centre) * 0.2 - cameraAngle1);
}

// draw scene
void display(void)//��ʾ����
{
	// update the game to the current time
	int currentTime = glutGet(GLUT_ELAPSED_TIME);//�������ÿ���֡��
	table.update(currentTime);

	// clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// �����ɫ����������Ȼ�����
	glEnable(GL_DEPTH_TEST);//����֮��,����ʱ��飬��ǰ����ǰ���Ƿ��б�����أ����������ص��������������Ͳ�����ƣ�Ҳ����˵��OpenGL��ֻ������ǰ���һ�㡣

							// window sizes
	int width = glutGet(GLUT_WINDOW_WIDTH);//�õ����ڲ�������
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	// set perspective projection
	glMatrixMode(GL_PROJECTION); //����ǰ����ָ��ΪͶӰ����
	glLoadIdentity(); //Ȼ��Ѿ�����Ϊ��λ����
					  //?????????????????????????????
	double fieldOfView = 50.0;  //ԽС�����ķ�ΧԽС
	double aspectRatio = (double)width / (double)height;//ʵ�ʴ��ڵ��ݺ��
	double nearPlane = 0.1;//�����Ĳ���
	double farPlane = 100.0;//Զ���Ĳ���
	gluPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);

	// set camera
	glMatrixMode(GL_MODELVIEW);//ģ���Ӿ�����
	glLoadIdentity();
	glTranslated(0, 0, -8); //��������(x, y, z)
	glRotated(cameraAngle2, 1, 0, 0);//��������Ƕȵ�������λ�� //ʹͼ����X����ת
	glRotated(cameraAngle1, 0, 1, 0);//ʹͼ����Y����ת

									 // set light colour and position
	glEnable(GL_LIGHTING);//���ù���
	glEnable(GL_LIGHT0);//0�Ź���
	glEnable(GL_LIGHT1);//1�Ź���
						//���һ������Ϊ0��˵���Ƿ����Թ�Դ����0��Ϊλ���Թ�Դ ����̨�ƣ�����˥���� 
	GLfloat lightAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f }; //���ù�Դ�Ļ��������ԣ�Ĭ��ֵ(0, 0, 0, 1)��
	GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//���ù�Դ��ɢ������ԣ�Ĭ��ֵ(1,1,1,1)
	GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //���ù�Դ�ľ��淴������ԣ�Ĭ��ֵ(1, 1, 1, 1)
	GLfloat lightPosition[4] = { 0.0f, 4.0f, 1.0f, 1.0f };//���ù�Դ��λ�ã�Ĭ��ֵ(0,0,1,0)
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// normalise normals automatically
	glEnable(GL_NORMALIZE);//�Զ���λ��������

						   // cause material colours to track current colour
	glEnable(GL_COLOR_MATERIAL);// ʹ����ɫ���� ������һ�л��ɻҰ�
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//������ʹ���ʵ�����ɫ�͵�ǰ��ɫ(��glColor*()����)һ�¡�
	//face����������GL_FRONT, GL_BACK, ��GL_FRONT_AND_BACK(ȱʡֵ)��
	//mode����������GL_AMBIENT, GL_DIFFUSE, GL_AMBIENT_AND_DIFFUSE(ȱʡֵ),
	//GL_SPECULAR, �� GL_EMISSION��face�����ĸ�����Щ����Ĳ�������ɫ��Ҫ
	//�ı�Ϊ��ǰ��ɫ��mode������Щ��������ɫ��Ҫ�ı䡣

	table.draw();

	// refresh window
	glutSwapBuffers();//��������������ָ��
	glutPostRedisplay();//�ػ�ͼ�񣬷���ͼ��ֻ����Ӧ���������Ϣʱ�Ż����ͼ��
						//�������ǽ��и��ӵĻ�ͼ����ʱ���������������Ե���˸������������Ĺؼ���
						//��ʹ���ƵĶ���ͬʱ��������Ļ�ϡ���ν˫���弼���� ��ָʹ�������������� ǰ̨
						//����ͺ�̨���塣ǰ̨���弴���ǿ�������Ļ����̨���������ڴ浱�У���������˵
						//�ǲ��ɼ��ġ�ÿ�ε����л�ͼ�������ں�̨�����н��У� ���������ʱ�� �ѻ��Ƶ�
						//���ս�����Ƶ���Ļ�ϣ� ������ ���ǿ�������GDIԪ��ͬʱ��������Ļ�ϣ��Ӷ���
						//����Ƶ��ˢ�µ��µĻ�����˸���⡣
}

int main(int argc, char* argv[])
{
	// initialise GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);//��ʾģʽ
	glutInitWindowSize(1280, 720);//���ڴ�С	
	glutCreateWindow("Billiard Ball game"); //��������

	// register callbacks ע��һЩ�лص�����
	glutKeyboardFunc(keyboard);//���̹���
	glutSpecialFunc(special);//��Ӧ���̷�����Ƽ�
	glutMouseFunc(mouse);//��깦��
	glutPassiveMotionFunc(passive);//��Ӧ���û�б�����ȥʱ���Ƶ���������
	glutDisplayFunc(display);//��ʾ��ǰ����
	glutTimerFunc(10, timer, 1);
	quadricObject = gluNewQuadric();
	glutMainLoop();//��������¼�����ѭ�����ú���������main�����������һ����䡣
	return 0;
}