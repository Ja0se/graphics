#include "GL/freeglut.h"

void init();
void display();

void DrawPoints(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,1,0);
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glVertex3f(0,0,0);
    glEnd();
    glFlush();
}
int main(int argc,char**argv){
    int mode=GLUT_RGB|GLUT_SINGLE;
    glutInit(&argc,argv);
    glutInitDisplayMode(mode);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(400,400);
    glutCreateWindow("OPENGL");
    glutSetWindowTitle("DAU CG");

    glutDisplayFunc(DrawPoints);
    glutMainLoop();
    return 1;
}
