#include <stdio.h>
#include <vector>
#include "GL/freeglut.h"
#include <math.h>
//#include "CGLab.h"

struct pt { double x; double y; };
int width = 500, height = 500;
int arrowIdx = 1;
double PI = 3.14159265;
std::vector<pt> pts;

void DrawObject() {
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(3.0);

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(0, 1);
	glEnd();

	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	glVertex2i(-1, 0);
	glVertex2i(1, 0);
	glEnd();

	glColor3f(0, 1, 0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	for (auto&pt:pts) {
		glVertex2f(pt.x, pt.y);
	}
	glEnd();
	glFlush();


}
void DrawPoints(int x, int y) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);//색상 설정
	glPointSize(10.0);//포인트 크기 설정
	glBegin(GL_POINTS);//포인트 그리기를 시작
	glVertex2f(x,y);//포인트 좌표 설정
	glEnd();//포인트 그리기 끝
	glFlush();
}
void KeyDown(unsigned char key, int x, int y)
{
	switch (key) {
	case 'p': case 'P':
		printf("Hello world!\n");
		break;
	case 'r': case 'R':
		pts.clear();
		break;
	}


	glutPostRedisplay();
}
void KeySpecial(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		arrowIdx++;
		break;
	case GLUT_KEY_DOWN:
		arrowIdx--;
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
	pts.push_back({ xd,yd });
	glutPostRedisplay();
}

void Init()
{
	glViewport(0, 0, width, height);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
	int mode = GLUT_RGB | GLUT_SINGLE;

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
	glutIdleFunc(DrawObject);
	glutMainLoop();
}