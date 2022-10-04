#include <cstdio>

#include "gl/freeglut.h"
#include <cmath>

int window_width = 500;
int window_height = 500;
int sceneIdx = 0;
double PI = 3.14159265;

void MyInit() {
	// glEnable(GL_DEPTH_TEST);
}
void MySpecial(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT)
	{
		sceneIdx--;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		sceneIdx++;
	}

	glutPostRedisplay();
}
void MyReshape(int w, int h) {
	window_width = w;
	window_height = h;

	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0, GLfloat(window_width) / GLfloat(window_height), 1.0, 50.0);
	glutPostRedisplay();
}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//double theta = PI * 2.0 * double(sceneIdx) / 100.0;
	gluLookAt(3, 3.0, 3, 0, 0, 0, 0, 1, 0);
	glEnable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3d(-0.5, 0, -0.5);
	glVertex3d(0.5, 0, -0.5);
	glVertex3d(0.5, 0, 0.5);
	glVertex3d(-0.5, 0, 0.5);
	glEnd();

	if ((sceneIdx / 100) % 2 == 0)
	{
		glColor3f(0, 1, 0);
		glBegin(GL_TRIANGLES);
		glVertex3d(-0.5, 0, -0.5);
		glVertex3d(0.5, 0, -0.5);
		glVertex3d(0, 0.5, 0);
		glEnd();
		glColor3f(0, 0, 1);
		glBegin(GL_TRIANGLES);
		glVertex3d(-0.5, 0, -0.5);
		glVertex3d(-0.5, 0, 0.5);
		glVertex3d(0, 0.5, 0);
		glEnd();
		glColor3f(1, 0, 1);
		glBegin(GL_TRIANGLES);
		glVertex3d(-0.5, 0, 0.5);
		glVertex3d(0.5, 0, 0.5);
		glVertex3d(0, 0.5, 0);
		glEnd();
		glColor3f(1, 1, 0);
		glBegin(GL_TRIANGLES);
		glVertex3d(0.5, 0, 0.5);
		glVertex3d(0.5, 0, -0.5);
		glVertex3d(0, 0.5, 0);
		glEnd();
	}

	glutSwapBuffers();
	glFlush();
}

void Timer(int value)
{
	sceneIdx++;
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(" ");
	MyInit();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutSpecialFunc(MySpecial);
	glutIdleFunc(MyDisplay);
	glutTimerFunc(10, Timer, 1);
	glutMainLoop();
	return 0;
}