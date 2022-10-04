#include "GL/freeglut.h"
//
//void init();
//void display();

void DrawPoints() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(3.0);

    glColor3f(1, 0, 0);//���� ����
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(0.3, 0);
    glVertex2f(0.15, 0.3);
    glEnd();

    glColor3f(1, 1, 0);//���� ����

    glBegin(GL_POLYGON);
    glVertex2f(0.3,0);
    glVertex2f(0.6, 0);
    glVertex2f(0.6, 0.3);
    glVertex2f(0.3, 0.3);
    glEnd();

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
