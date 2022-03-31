#include "GL/freeglut.h"
//
//void init();
//void display();

void DrawPoints() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 1, 0);//색상 설정
    glPointSize(10.0);//포인트 크기 설정
    glBegin(GL_POINTS);//포인트 그리기를 시작
    glVertex3f(0, 0, 0);//포인트 좌표 설정
    glEnd();//포인트 그리기 끝
    glFlush();
}

int main(int argc, char** argv) {
    int mode = GLUT_RGB | GLUT_SINGLE;
    glutInit(&argc, argv);//초기화
    glutInitDisplayMode(mode);//mode 설정
    glutInitWindowPosition(100, 100);//모니터의 x,y에서 창 열기
    glutInitWindowSize(400, 400);//창 크기 설정
    glutCreateWindow("OPENGL");//창이름 설정
    glutSetWindowTitle("DAU CG");//window title 설정

    glutDisplayFunc(DrawPoints);//콜백함수
    glutMainLoop();//프로그램이 꺼지지 않게 한다.
    return 1;
}