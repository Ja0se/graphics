#include "GL/freeglut.h"
//
//void init();
//void display();

void DrawPoints() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 1, 0);//���� ����
    glPointSize(10.0);//����Ʈ ũ�� ����
    glBegin(GL_POINTS);//����Ʈ �׸��⸦ ����
    glVertex3f(0, 0, 0);//����Ʈ ��ǥ ����
    glEnd();//����Ʈ �׸��� ��
    glFlush();
}

int main(int argc, char** argv) {
    int mode = GLUT_RGB | GLUT_SINGLE;
    glutInit(&argc, argv);//�ʱ�ȭ
    glutInitDisplayMode(mode);//mode ����
    glutInitWindowPosition(100, 100);//������� x,y���� â ����
    glutInitWindowSize(400, 400);//â ũ�� ����
    glutCreateWindow("OPENGL");//â�̸� ����
    glutSetWindowTitle("DAU CG");//window title ����

    glutDisplayFunc(DrawPoints);//�ݹ��Լ�
    glutMainLoop();//���α׷��� ������ �ʰ� �Ѵ�.
    return 1;
}