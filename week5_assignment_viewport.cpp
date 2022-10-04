#include "GL/freeglut.h"
int Width, Height;
void InitLight() {
	GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 2, 3.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void InitVisibility() {
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}
void DrawObject() {
	glColor3f(0.7, 0.7, 0.7);
	glLineWidth(3.0);
	glBegin(GL_LINES);
	for (float i = -2; i <= 2; i += 0.1) {
		glVertex3f(-2, -0.45, i);
		glVertex3f(2, -0.45, i);
		glVertex3f(i, -0.45, -2);
		glVertex3f(i, -0.45, 2);

	}
	
	glEnd();
}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 11.0);

	
	glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, (GLsizei)Width / 2, (GLsizei)Height / 2);
	glPushMatrix();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	DrawObject();
	glutSolidTeapot(0.58);
	
	glPopMatrix();

	glViewport((GLsizei)Width / 2, 0, (GLsizei)Width / 2, (GLsizei)Height / 2);
	glPushMatrix();
	gluLookAt(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	DrawObject();
	glutSolidTeapot(0.58);
	glPopMatrix();

	glViewport(0, (GLsizei)Height / 2, (GLsizei)Width / 2, (GLsizei)Height / 2);
	glPushMatrix();
	gluLookAt(0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	DrawObject();
	glutSolidTeapot(0.58);
	glPopMatrix();

	glViewport((GLsizei)Width / 2, (GLsizei)Height / 2, (GLsizei)Width / 2, (GLsizei)Height / 2);
	glPushMatrix();
	gluLookAt(5, 5, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	DrawObject();
	glutSolidTeapot(0.58);
	glPopMatrix();

	glFlush();
}

void MyReshape(int w, int h) {
	Width = w;
	Height = h;
	
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Z-Buffer Algorithm");
	glClearColor(0.5, 0.5, 0.5, 0.0);
	InitLight();
	InitVisibility();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);

	glutMainLoop();
	return 0;
}