#include <stdio.h>
#include <vector>
#include "GL/freeglut.h"
struct pt { double x; double y; double z; };
int width = 500, height = 500;
int arrowIdx0 = 0, arrowIdx1 = 0;
bool isPerspective = false;
double PI = 3.14159265;
double PI20 = 3.14159265 * 2.0;
std::vector<pt> pts;
void DrawAxis()
{
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glColor3f(0, 1, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 1);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
}
void DrawCircle(double ctrX, double ctrY, double radius) {

	int res = 100;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < res; i++)
	{
		double theta = PI * 2.0 * double(i) / double(res);
		double x = radius * cos(theta) + ctrX;
		double y = radius * sin(theta) + ctrY;
		glVertex2d(x, y);
	}
	glEnd();
}
void DrawQuad(double length, double z)
{
	glBegin(GL_QUADS);
	glVertex3d(-length, -length, z);
	glVertex3d(length, -length, z);
	glVertex3d(length, length, z);
	glVertex3d(-length, length, z);
	glEnd();
}

void DrawZBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(2, 2, 2, 0, 0, 0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3.0, 3.0, -3.0, 3.0, -10.0, 10.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5 + 0.1 * arrowIdx, 20.0 + 0.1 * arrowIdx);

	// glEnable(GL_DEPTH_TEST);
	// glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < 5; i++)
	{
		double z = 0.1 * double(i);
		printf("z %f\n", z);
		glColor3f(1.0 - (1.0 / 4) * double(i), 0, (1.0 / 4) * double(i));
		DrawQuad(2.0, -z);
	}
	glutSwapBuffers();
}

void DrawObject() {
	printf("aN %d %d\n", arrowIdx0, arrowIdx1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double wAdd = 0.0;
	double hAdd = 0.0;

	if (isPerspective)
		glFrustum(-0.4 - wAdd, 0.4 + wAdd, -0.4 - hAdd, 0.4 + hAdd, 1.5, 100.0);
	else
		glOrtho(-3.0 - wAdd, 3.0 + wAdd, -3.0 - hAdd, 3.0 + hAdd, 0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	pt cameraPos;
	double value = PI20 * double(arrowIdx1 % 360) / 360.0;
	cameraPos.x = 3.0 * cos(value);
	cameraPos.y = 1.0 + 0.1 * double(arrowIdx0);
	cameraPos.z = 3.0 * sin(value);
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 0, 0, 0, 0, 1, 0);

	glColor3f(0, 0, 0);
	glLineWidth(1.0);
	glutWireTeapot(1.0);
	for (int i = 1; i <= 10; i++)
	{
		glPushMatrix();
		glTranslated(0, 0, double(-2 * i));
		if (i % 3 == 0)
			glColor3f(1, 0, 0);
		else if (i % 3 == 1)
			glColor3f(0, 1, 0);
		else
			glColor3f(0, 0, 1);

		glutWireTeapot(1.0);
		glPopMatrix();
	}
	glFlush();
}

void Init()
{
	glViewport(0, 0, width, height);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

void KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p': case 'P':
		isPerspective ^= true;
		break;
	}
	glutPostRedisplay();
}
void KeySpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		arrowIdx0++;
		break;
	case GLUT_KEY_DOWN:
		arrowIdx0--;
		break;
	case GLUT_KEY_LEFT:
		arrowIdx1++;
		break;
	case GLUT_KEY_RIGHT:
		arrowIdx1--;
		break;
	}
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		printf("Mouse Click (%d %d)\n", x, y);

	glutPostRedisplay();
}
void Motion(int x, int y)
{
	printf("Mouse motion (%d %d)\n", x, y);

	double xd, yd;
	xd = x / 500.0 * 4.0 - 2.0;
	yd = y / 500.0 * 4.0 - 2.0;
	yd *= -1.0;

	glutPostRedisplay();
}

void Timer(int value)
{
	glutTimerFunc(30, Timer, 1);
}
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	glutPostRedisplay();
}
int main(int argc, char** argv) {
	int mode = GLUT_RGBA | GLUT_DEPTH;

	glutInit(&argc, argv);
	glutInitDisplayMode(mode);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OpenGL");
	glutSetWindowTitle("DAU CG");

	Init();
	glutDisplayFunc(DrawObject);
	glutKeyboardFunc(KeyDown);
	glutMouseFunc(Mouse);
	glutSpecialFunc(KeySpecial);
	glutMotionFunc(Motion);
	// glutReshapeFunc(Reshape);
	// glutIdleFunc(DrawObject);
	// glutTimerFunc(1, Timer, 1);
	glutMainLoop();
}