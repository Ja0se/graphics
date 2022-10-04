#include "GL/freeglut.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <random>

float g_fDistance = -200.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;

struct pt { double x, y, z; };

static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

struct face { int i[3]; int n[3]; };
std::vector<pt> pts;
std::vector<pt> normals;
std::vector<face> faces;
std::vector<face> inps;

struct Ball {
    double maxX, maxY, maxZ;
    double minX = 1E+10, minY = 1E+10, minZ = 1E+10;
};

int depth = 1;
int mdepth = 0;

void parse(const std::string& str, std::vector<std::string>& values, std::string& delimiter)
{
    std::string::size_type Fpos = str.find_first_not_of(delimiter, 0);
    std::string::size_type Lpos = str.find_first_of(delimiter, Fpos);
    while (std::string::npos != Fpos || std::string::npos != Lpos)
    {
        values.push_back(str.substr(Fpos, Lpos - Fpos));
        Fpos = str.find_first_not_of(delimiter, Lpos);
        Lpos = str.find_first_of(delimiter, Fpos);
    }
}

void LoadFile()
{
    std::string fileName = "bunny.obj";
    std::ifstream ifs(fileName);
    if (ifs.fail())
        std::cerr << "File read error!" << std::endl;
    char line[100];
    std::string delimiter = " ";
    std::string slash = "/";
    while (!ifs.eof())
    {
        ifs.getline(line, 100);
        std::string s = line;

        if (s[0] == '#')
            printf("Comment : %s\n", s.c_str());
        else if (s[0] == 'v' && s[1] != 't' && s[1] != 'n')
        {
            std::vector<std::string> tmp;
            parse(s, tmp, delimiter);
            pts.push_back({ std::stod(tmp[1]), std::stod(tmp[2]), std::stod(tmp[3]) });
        }
        else if (s[0] == 'v' && s[1] == 'n')
        {
            std::vector<std::string> tmp;
            parse(s, tmp, delimiter);
            normals.push_back({ std::stod(tmp[1]), std::stod(tmp[2]), std::stod(tmp[3]) });
        }
        else if (s[0] == 'f')
        {
            face F;
            std::vector<std::string> tmp;
            parse(s, tmp, slash);
            std::vector<std::string> tmp2;
            parse(tmp[0], tmp2, delimiter);
            F.i[0] = std::stoi(tmp2[1]) - 1;
            tmp2.clear();
            parse(tmp[1], tmp2, delimiter);
            F.n[0] = std::stoi(tmp2[0]) - 1;
            F.i[1] = std::stoi(tmp2[1]) - 1;
            tmp2.clear();
            parse(tmp[2], tmp2, delimiter);
            F.n[1] = std::stoi(tmp2[0]) - 1;
            F.i[2] = std::stoi(tmp2[1]) - 1;
            F.n[2] = std::stoi(tmp[3]) - 1;
            faces.push_back(F);
        }
        else
        {
            printf("Not processed : %s\n", s.c_str());
        }
    }
}

class BallBV {
public:
    Ball BV;
    BallBV* children[2] = { nullptr,nullptr }; // chid
    BallBV(std::vector<face> inputData);
    bool isLeaf();
    void SetBV();
};

class BallBVH {
public:
    BallBV* root = nullptr; // root
    BallBVH(std::vector<face> inputData);
};

BallBVH::BallBVH(std::vector<face> inputData) {
    root = new BallBV(inputData);
}

BallBV::BallBV(std::vector<face> inputData) {// 안에 저장할 값만 변경해주고 교수님 코드와 동일 합니다.
    inps = inputData;
    SetBV();
    if (inps.size() > 2)
    {
        std::vector<face> child0, child1;

        double xMin = 1E+10, xMax = -1E+10;

        for (auto& inp : inps)
        {
            double min = 1E+10, max = -1E+10;
            for (int i = 0; i < 3; i++) {
                if (pts[inp.i[i]].x > max)
                    max = pts[inp.i[i]].x;
                if (pts[inp.i[i]].x < min)
                    min = pts[inp.i[i]].x;
            }
            if (max > xMax)
                xMax = max;
            if (min < xMin)
                xMin = min;
        }
        double xMid = (xMin + xMax) / 2.0;
        for (auto& inp : inps)
        {
            double min = 1E+10, max = -1E+10;
            for (int i = 0; i < 3; i++) {
                if (pts[inp.i[i]].x > max)
                    max = pts[inp.i[i]].x;
                if (pts[inp.i[i]].x < min)
                    min = pts[inp.i[i]].x;
            }
            if ((max + min) / 2 < xMid)
                child0.push_back(inp);//자식의 중간값이 자른값보다 작을 경우 왼쪽 트리에 삽입
            else
                child1.push_back(inp);//자식의 중간값이 자른값 즉xMid값보다 클 경우 오른쪽 트리에 삽입
        }
        if (child0.empty() || child1.empty())
            return;//자식이 없으면 리턴
        if (!child0.empty())
            children[0] = new BallBV(child0);//왼쪽 자식이 있으면 왼쪽 트리에 자식 넣기
        if (!child1.empty())
            children[1] = new BallBV(child1);//오른쪽 자식이 있으면 오른쪽 트리에 자식 넣기 

    }
}

bool BallBV::isLeaf() {
    return children[0] == nullptr && children[1] == nullptr;
}

void BallBV::SetBV() { //AABB크기를 설정해줌
    for (auto& inp : inps)
    {
        double minX = 1E+10, maxX = -1E+10;
        double minY = 1E+10, maxY = -1E+10;
        double minZ = 1E+10, maxZ = -1E+10;
        for (int i = 0; i < 3; i++) {
            if (pts[inp.i[i]].x > maxX)
                maxX = pts[inp.i[i]].x; // 입력받은 face의 I의 x값이 maxx보다 크면 maxx에 입력받은 값을 저장
            if (pts[inp.i[i]].x < minX)
                minX = pts[inp.i[i]].x; //위와 동일
            if (pts[inp.i[i]].y > maxY)
                maxY = pts[inp.i[i]].y; //위와 동일
            if (pts[inp.i[i]].y < minY)
                minY = pts[inp.i[i]].y; //위와 동일
            if (pts[inp.i[i]].z > maxZ)
                maxZ = pts[inp.i[i]].z; //위와 동일
            if (pts[inp.i[i]].z < minZ)
                minZ = pts[inp.i[i]].z; //위와 동일
        }
        if (BV.maxX < maxX) {// BV.maxx즉 class BallBV에 있는 Ball BV를 사용하여Ball구조에 있는 값에 저장해준다
            BV.maxX = maxX;
        }
        if (BV.minX > minX) {
            BV.minX = minX;
        }
        if (BV.maxY < maxY) {
            BV.maxY = maxY;
        }
        if (BV.minY > minY) {
            BV.minY = minY;
        }
        if (BV.maxZ < maxZ) {
            BV.maxZ = maxZ;
        }
        if (BV.minZ > minZ) {
            BV.minZ = minZ;
        }
    }
}

BallBVH* bvh;

int maxDepth(BallBV bv, int dd) {
    if (bv.isLeaf() == 1) {
        dd++;
        return dd;
    }
    else {
        dd++;
        int r = maxDepth(*bv.children[0], dd);
        int l = maxDepth(*bv.children[1], dd);
        if (r > l) dd = r;
        else dd = l;
        return dd;
    }
}

void DrawAABB(BallBV bv) {// AABB그리기
    glColor3f(0, 0, 1);

    glBegin(GL_LINE_LOOP);
    glVertex3f(bv.BV.minX, bv.BV.minY, bv.BV.maxZ);
    glVertex3f(bv.BV.maxX, bv.BV.minY, bv.BV.maxZ);
    glVertex3f(bv.BV.maxX, bv.BV.maxY, bv.BV.maxZ);
    glVertex3f(bv.BV.minX, bv.BV.maxY, bv.BV.maxZ);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(bv.BV.minX, bv.BV.minY, bv.BV.minZ);
    glVertex3f(bv.BV.maxX, bv.BV.minY, bv.BV.minZ);
    glVertex3f(bv.BV.maxX, bv.BV.maxY, bv.BV.minZ);
    glVertex3f(bv.BV.minX, bv.BV.maxY, bv.BV.minZ);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(bv.BV.minX, bv.BV.maxY, bv.BV.maxZ);
    glVertex3f(bv.BV.minX, bv.BV.maxY, bv.BV.minZ);
    glVertex3f(bv.BV.maxX, bv.BV.maxY, bv.BV.maxZ);
    glVertex3f(bv.BV.maxX, bv.BV.maxY, bv.BV.minZ);
    glVertex3f(bv.BV.minX, bv.BV.minY, bv.BV.maxZ);
    glVertex3f(bv.BV.minX, bv.BV.minY, bv.BV.minZ);
    glVertex3f(bv.BV.maxX, bv.BV.minY, bv.BV.maxZ);
    glVertex3f(bv.BV.maxX, bv.BV.minY, bv.BV.minZ);
    glEnd();
}

void DrawDFS(BallBV bv, int dd) { //깊이우선탐색을 사용하여 자식이 1일 경우 AABB를 그리고 리턴
    if (bv.isLeaf() == 1 || dd == depth) {
        DrawAABB(bv);
        return;
    }
    else {//자식이 1이 아닐 경우 왼쪽자식과 오른쪽자식의 AABB를 그려준다.
        dd++;
        DrawDFS(*bv.children[0], dd);
        DrawDFS(*bv.children[1], dd);
        DrawAABB(bv);
    }
}

void DrawModel(void) {
    glColor3f(1, 1, 1);
    for (auto f : faces)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 3; i++)
        {
            int idx = f.i[i];
            int nIdx = f.n[i];
            auto v = pts[idx];
            auto n = normals[nIdx];
            glNormal3d(n.x, n.y, n.z);
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();
    }
    DrawDFS(*bvh->root, 1); //클래스경로를 받기 때문에 *사용
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    //    glRotatef(-90.0, 1.0, 0.0, 0.0);
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
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    LoadFile();
    bvh = new BallBVH(faces);
    mdepth = maxDepth(*bvh->root, 0);

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
    if (key == GLUT_KEY_UP)
    {
        g_fDistance -= 5.0f;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        g_fDistance += 5.0f;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        depth++;
        if (depth >= mdepth) depth = mdepth;
        std::cout << depth << std::endl;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        depth--;
        if (depth <= 0) depth = 1;
        std::cout << depth << std::endl;
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
    glutCreateWindow("OBJ Loader");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    glutSpecialFunc(MySpecial);

    MyInit();

    glutMainLoop();
    return 0;
}
