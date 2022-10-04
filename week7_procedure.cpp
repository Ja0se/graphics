#include <cstdio>

#include "gl/freeglut.h"
#include <cmath>

int window_width = 500;
int window_height = 500;
double PI = 3.14159265;
const GLfloat red[] = { 0.8f, 0.0, 0.0, 1.0 };
const GLfloat blue[] = { 0.0, 0.2f, 1.0, 1.0 };
const GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat polished[] = { 100.0 };
const GLfloat dull[] = { 0.0 };
const GLfloat light_pos[] = { 0.0, 0.0, 0.0, 1.0 };
int sceneIdx = 0;
int arrowIdx0 = 0;
int arrowIdx1 = 0;
void MyInit() {
	//주석 무시할것 잘못써진거임
	GLfloat Light1_Ambient[] = { 0.3,  0.3,  0.3, 1.0 };	//1번 광원 특성
	GLfloat Light1_Diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat Light1_Specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light1_Ambient);	//1번 광원 특성할당
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light1_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light1_Specular);

	GLfloat ambi[] = { 1.0, 1.0, 1.0, 1.0 };	//0번 광원 특성
	GLfloat diff[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };

	
	GLfloat lightPos[] = { 0.0, 0.0, 1.0, 1.0 };	//0번 광원위치
	GLfloat lightDir[] = { 0.0, 0.0, -1.0 };		//0번 광원 방향

	lightPos[0] = -0.2;
	ambi[0] = 1.0; ambi[1] = 0.0; ambi[2] = 0.0;
	diff[0] = 1.0; diff[1] = 0.0; diff[2] = 0.0;
	spec[0] = 1.0; spec[1] = 0.0; spec[2] = 0.0;
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambi);	//0번 광원 특성할당
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);	//스포트라이트
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);	//방향
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 4.0);	//각도
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0);			//빛 감쇠 정도 [0~128]

	lightPos[0] = 0.2;
	ambi[0] = 0.0; ambi[1] = 1.0; ambi[2] = 0.0;
	diff[0] = 0.0; diff[1] = 1.0; diff[2] = 0.0;
	spec[0] = 0.0; spec[1] = 1.0; spec[2] = 0.0;
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambi);	//0번 광원 특성할당
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT2, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos);	//스포트라이트
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDir);	//방향
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 4.0);	//각도
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.0);			//빛 감쇠 정도 [0~128]

	lightPos[0] = 0;
	lightPos[1] = 0.2;
	ambi[0] = 0.0; ambi[1] = 0.0; ambi[2] = 1.0;
	diff[0] = 0.0; diff[1] = 0.0; diff[2] = 1.0;
	spec[0] = 0.0; spec[1] = 0.0; spec[2] = 1.0;
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambi);	//0번 광원 특성할당
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT3, GL_SPECULAR, spec);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPos);	//스포트라이트
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightDir);	//방향
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 4.0);	//각도
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 1.0);			//빛 감쇠 정도 [0~128]

	//glShadeModel(GL_FLAT);
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);	//깊이 버퍼 활성화
	glEnable(GL_LIGHTING);		//조명 활성화
	glEnable(GL_LIGHT0);		//0번 광원 활성화
	glEnable(GL_LIGHT1);		//1번 광원 활성화
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

}
void MySpecial(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT)
		arrowIdx0--;
	else if (key == GLUT_KEY_RIGHT)
		arrowIdx0++;
	else if (key == GLUT_KEY_UP)
		arrowIdx1++;
	else if (key == GLUT_KEY_DOWN)
		arrowIdx1--;

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
	double theta = PI * 2.0 * double(sceneIdx) / 100.0;
	gluLookAt(0, 0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(-0.3 + cos(theta), 0, 3.0, 0, 0, 0, 0, 1, 0);

	GLfloat material_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat material_diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 127.0 };
	GLfloat material_emission_blue[] = { 0,0,1.0,1.0 };
	GLfloat material_emission[] = { 0.0,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	for (int i = 0; i < 5; i++)
	{
		//else glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_red);
		for (int j = 0; j < 5; j++)
		{
			if (i == j) glMaterialfv(GL_FRONT, GL_EMISSION, material_emission_blue);
			else glMaterialfv(GL_FRONT, GL_EMISSION,material_emission);
			
			glPushMatrix();
			glTranslatef(-0.3 + sin(PI * 2.0 * double(arrowIdx0) / 300.0), 0, 0);
			glTranslatef(-0.5 + i * 0.3, -0.5 + j * 0.3, 0.0);
			glutSolidSphere(0.2, 100, 100);
			glPopMatrix();
		}
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
	glutCreateWindow("Three Spotlights");
	MyInit();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutSpecialFunc(MySpecial);
	glutIdleFunc(MyDisplay);
	glutTimerFunc(50, Timer, 1);
	glutMainLoop();
	return 0;
}