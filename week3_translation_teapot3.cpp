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
void Timer(int value) {
	if (arrowIdx < 360)arrowIdx++;
	else arrowIdx = 0;
	glutTimerFunc(10, Timer, 1);
}
void DrawObject() {
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(3.0);

	glColor3f(1, 0, 0);
	double radius = 1.0;
	glBegin(GL_LINES);
	glVertex2i(0, 0);
	glVertex2i(0, 1);

	glColor3f(0, 1, 0);
	glVertex2i(0, 0);
	glVertex2i(1, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();


}
void DrawPoints(int x, int y) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);//색상 설정
	glPointSize(10.0);//포인트 크기 설정
	glBegin(GL_POINTS);//포인트 그리기를 시작
	glVertex2f(x, y);//포인트 좌표 설정
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
void Drawtranslation() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	DrawObject();

	glLineWidth(1.0);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.2);
	
	glColor3f(1, 0, 1);
	glTranslatef(0.5, 0, 0);
	glRotatef(arrowIdx, 0, 1,1);
	glutWireTeapot(0.2);


	glColor3f(0, 0, 1);
	glLoadIdentity();
	glRotatef(arrowIdx, 0, 1, 1);
	glTranslatef(0.5, 0, 0);
	glutWireTeapot(0.2);

	glutSwapBuffers();

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
	glutDisplayFunc(Drawtranslation);
	glutKeyboardFunc(KeyDown);
	glutMouseFunc(Mouse);
	glutSpecialFunc(KeySpecial);
	glutMotionFunc(Motion);
	glutIdleFunc(Drawtranslation);
	glutTimerFunc(1, Timer, 1);
	glutMainLoop();
}