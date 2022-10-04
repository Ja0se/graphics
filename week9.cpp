#include "GL/freeglut.h"
#include "BezierCurve2d.h"
#include "Utils.h"

using namespace MN;

GLsizei width = 1000, height = 1000;
int edit_ctrlpts_idx = -1;
bool isDrawControlMesh = true;
bool isDottedLine = false;
BezierCurve2d::Ptr bezier;

int hit_index(BezierCurve2d::Ptr curve, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		GLfloat tx = curve->getCpts()[i][0] - x;
		GLfloat ty = curve->getCpts()[i][1] - y;
		if ((tx * tx + ty * ty) < 30) return i;
	}
	return -1;
}

void init()
{
	std::vector<Vec2> cpts;
	cpts.push_back({ 50,100 });
	cpts.push_back({ 200,300 });
	cpts.push_back({ 400,300 });
	cpts.push_back({ 550,100 });
	bezier = BezierCurve2d::createPtr(3, cpts);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);
}

void reshape_callback(GLint nw, GLint nh)
{
	width = nw;
	height = nh;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}
void display_callback()
{
#define RES 100
	/* curve */
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(0, 0, 0);
	if (isDottedLine)
		glBegin(GL_LINES);
	else
		glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= RES; i++)
	{
		const float t = (float)i / (float)RES;
		Vec2 pt = bezier->evaluate(t);
		glVertex2d(pt[0], pt[1]);
	}
	glEnd();

	/* control mesh */
	if (isDrawControlMesh)
	{
		glColor3ub(255, 0, 0);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 4; i++)
		{
			Vec2 pt = bezier->getCpts()[i];
			glVertex2d(pt[0], pt[1]);
		}
		glEnd();
	}

	/* control pts */
	glColor3ub(0, 0, 255);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
	{
		Vec2 pt = bezier->getCpts()[i];
		glVertex2d(pt[0], pt[1]);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for(int i=0;i<100;i++)
	{
		float x = 2 * 3.141592 * i / 100;
		glVertex2f(400.0+100.0*sin(x), 400.0+100.0*cos(x));
	}
	glEnd();

	glutSwapBuffers();
}

// void glutMouseFunc(void (*func)(int button, int state, int x, int y));
void mouse_callback(GLint button, GLint action, GLint x, GLint y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		switch (action)
		{
			case GLUT_DOWN:
				edit_ctrlpts_idx = hit_index(bezier, x, height - y);
				break;
			case GLUT_UP:
				edit_ctrlpts_idx = -1;
				break;
			default: break;
		}
	}
	glutPostRedisplay();
}

// void glutMotionFunc(void (*func)(int x, int y));
void mouse_move_callback(GLint x, GLint y)
{
	if (edit_ctrlpts_idx != -1)
	{
		auto cpts = bezier->getCpts();
		cpts[edit_ctrlpts_idx][0] = x;
		cpts[edit_ctrlpts_idx][1] = height-y;
		bezier->setCpts(cpts);
		bezier->updateDerivMat();
	}
	glutPostRedisplay();
}

// void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void keyboard_callback(unsigned char key, int x, int y)
{
	std::vector<Vec2> cpts;
	switch(key)
	{
	case 'i': case 'I':
		cpts.push_back({ 50,100 });
		cpts.push_back({ 200,300 });
		cpts.push_back({ 400,300 });
		cpts.push_back({ 550,100 });
		bezier->setCpts(cpts);
		bezier->updateDerivMat();
		break;
	case 'l': case 'L':
		isDottedLine ^= true;
		break;
	case 'c': case 'C':
		isDrawControlMesh ^= true;
		break;
	case (27): exit(0); break;
	default: break;
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Bezier Editor");

	init();
	glutReshapeFunc(reshape_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(mouse_move_callback);
	glutDisplayFunc(display_callback);
	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();



	return 0;
}
