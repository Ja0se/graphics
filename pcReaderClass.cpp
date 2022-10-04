#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include<algorithm>
#include "gl/freeglut.h"
using namespace std;
float g_fDistance = -1.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;

static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing; 

struct pt { 
    double x, y, z; 
}typedef;
std::vector<pt> pts;
vector<pt> pointbunny;
struct pt Min, Max;
void LoadFile()
{
    freopen("bunnyData.pts", "r", stdin);
    Min.x = Min.y = Min.z = 1234567890;
    Max.x = Max.y = Max.z = -1234567890;
    int N;
    cin >> N;
    pt P;
    while (N--) {
        cin >> P.x >> P.y >> P.z;
        pointbunny.push_back(P);
        Min.x = min(Min.x, P.x);
        Min.y = min(Min.y, P.y);
        Min.z = min(Min.z, P.z);
        Max.x = max(Max.x, P.x);
        Max.y = max(Max.y, P.y);
        Max.z = max(Max.z, P.z);
    }
    //sort(pointbunny.begin(), pointbunny.end());
    //Min = pointbunny[0];
    //Max = pointbunny.back();
}

void DrawModel(void) {
    glColor3f(1, 1, 1);
    glBegin(GL_POINTS);
    for (auto& p : pointbunny) {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glColor3f(0, 0, 1.0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(Min.x, Min.y, Min.z);
    glVertex3f(Max.x, Min.y, Min.z);
    glVertex3f(Max.x, Max.y, Min.z);
    glVertex3f(Min.x, Max.y, Min.z);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(Min.x, Max.y, Max.z);
    glVertex3f(Max.x, Max.y, Max.z);
    glVertex3f(Max.x, Min.y, Max.z);
    glVertex3f(Min.x, Min.y, Max.z);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(Min.x, Max.y, Min.z);
    glVertex3f(Min.x, Max.y, Max.z);
    glVertex3f(Min.x, Min.y, Min.z);
    glVertex3f(Min.x, Min.y, Max.z);
    glVertex3f(Max.x, Max.y, Min.z);
    glVertex3f(Max.x, Max.y, Max.z);
    glVertex3f(Max.x, Min.y, Min.z);
    glVertex3f(Max.x, Min.y, Max.z);
    glEnd();
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    DrawModel();

    glutPostRedisplay();
    glutSwapBuffers();
}

void MyInit() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    LoadFile();
}

void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MySpecial(int key, int x, int y) {
    if (key == GLUT_KEY_PAGE_UP)
    {
        g_fDistance -= 0.1f;
    }
    else if (key == GLUT_KEY_PAGE_DOWN)
    {
        g_fDistance += 0.1f;
    }

    glutPostRedisplay();
}

void MyMouse(int button, int state, int x, int y) {
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Point Cloud Renderer");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    glutSpecialFunc(MySpecial);
    
    MyInit();

    glutMainLoop();
    return 0;
}