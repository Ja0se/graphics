#include "gl/freeglut.h"
#include<cstdio>
GLfloat Cutoff_light0[1] = { 7.0 };
GLfloat exp = 1.0;
void MyLightInit() {
	GLfloat global_ambient[] = { 0.5, 0.5, 0.5, 1.0 };	//전역 주변반사

	GLfloat Light0_Ambient[] = { 1.0, 1.0, 1.0, 1.0 };	//0번 광원 특성
	GLfloat Light0_Diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat Light0_Specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat Light1_Ambient[] = { 0.3, 0.3, 0.3, 1.0 };	//1번 광원 특성
	GLfloat Light1_Diffuse[] = { 0.3, 0.3, 0.3, 0.3 };
	GLfloat Light1_Specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat Light0_Position[] = { 0.0, 0.0, 1.0, 1.0 };	//0번 광원위치
	GLfloat Light0_Direction[] = { 0.0, 0.0, -1.0 };		//0번 광원 방향
	//GLfloat Light0_Cutoff[] = { Cutoff_light0 };					//0번 광원 각도

	GLfloat Light1_Position[] = { 0.0, 0.0, 1.0, 1.0 };	//1번 광원위치

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light0_Ambient);	//0번 광원 특성할당
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0_Specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, Light1_Ambient);	//1번 광원 특성할당
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1_Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Light1_Specular);

	glLightfv(GL_LIGHT0, GL_POSITION, Light0_Position);	//스포트라이트
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light0_Direction);	//방향
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, Cutoff_light0);	//각도
	printf("%f", exp);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);			//빛 감쇠 정도 [0~128]

	GLfloat material_ambient[] = { 0.0, 0.0, 0.0, 1.0 };	//물체 특성
	GLfloat material_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 127.0 };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);//물체 특성할당
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);//전역주변반사 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //근접시점

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);	//깊이 버퍼 활성화
	glEnable(GL_LIGHTING);		//조명 활성화
	glEnable(GL_LIGHT0);		//0번 광원 활성화 - spotlight
	glEnable(GL_LIGHT1);		//1번 광원 활성화 - staticlight
}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.5, 1.5, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	 //시점변환

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			glPushMatrix();
			glTranslatef(-0.5 + i * 0.3, -0.5 + j * 0.3, 0.0);
			glutSolidSphere(0.2, 50, 50);
			glPopMatrix();
		}
	}
	glDisable(GL_LIGHTING);
	glTranslatef(0.0, 0.0, 1.0);
	glutWireCone(0.05, 0.1, 50, 50);
	glEnable(GL_LIGHTING);
	glFlush();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLsizei)w / (GLsizei)h, 0.1, 100);
}

void KeySpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		Cutoff_light0[0] -= 1.0f;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		Cutoff_light0[0] += 1.0f;
	}
	else if (key == GLUT_KEY_DOWN) {
		exp -= 1.0;
	}
	else if (key == GLUT_KEY_UP) {
		exp += 1.0;
	}
	MyLightInit();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Spotlight");
	glClearColor(0.5, 0.5, 0.5, 0.5);
	MyLightInit();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutSpecialFunc(KeySpecial);
	glutMainLoop();
	return 0;
}

