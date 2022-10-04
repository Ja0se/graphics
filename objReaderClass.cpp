#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "gl/freeglut.h"
#include <cmath>
#include <queue>

float g_fDistance = -200.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
bool drawInternalBV = false;

static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

struct pt { double x, y, z; };
struct aabb { pt min, max; };
struct face { int i[3]; int n[3]; };

std::vector<pt> pts;
std::vector<pt> normals;
std::vector<face> faces;
int depth = 1;
int mdepth = 0;
class bunnyBV {
public:
    aabb BV;
    std::vector<face> fa;
    bunnyBV* children[2] = { nullptr,nullptr };
    bunnyBV(std::vector<face> inputData);
    bool isLeaf();
    void SetBV();
};

class bunnyBVH
{
public:
    bunnyBV* root = nullptr;
    bunnyBVH(std::vector<face> inputData);
};

bunnyBVH::bunnyBVH(std::vector<face> inputData) {
    root = new bunnyBV(inputData);
}

bunnyBV::bunnyBV(std::vector<face> inputData)
{
    fa = inputData;
    SetBV();
    if (fa.size() > 2)
    {
        std::vector<face> child0, child1;
        double xMax = -1E+10, yMax = 1E+10, zMax = -1E+10;
        double xMin = 1E+10, yMin = -1E+10, zMin=-1E+10;
        xMax = BV.max.x;
        yMax = BV.max.y;
        zMax = BV.max.z;
        xMin = BV.min.x;
        yMin = BV.min.y;
        zMin = BV.min.z;
        double xMid = (xMin + xMax) / 2.0;
        double yMid = (yMin + yMax) / 2.0;
        double zMid = (zMin + zMax) / 2.0;
        if ((xMax - xMid > yMax - yMid) && (xMax - xMid > zMax - zMid)) {//x축 기준으로 divide
            for (auto& f : fa)
            {
                double Min = 1E+10, Max = -1E+10;
                for (int i = 0; i < 3; i++)
                {
                    int idx = f.i[i];
                    auto& v = pts[idx];
                    Min = min(Min, v.x);
                    Max = max(Max, v.x);
                }
                if ((Max + Min) / 2 < xMid)child0.push_back(f);
                else child1.push_back(f);
            }
        }
        else if ((xMax - xMid < yMax - yMid) &&( yMax - yMid > zMax - zMid)) {//y축 기준
            for (auto& f : fa)
            {
                double Min = 1E+10, Max = -1E+10;
                for (int i = 0; i < 3; i++)
                {
                    int idx = f.i[i];
                    auto& v = pts[idx];
                    Min = min(Min, v.y);
                    Max = max(Max, v.y);
                }
                if ((Max + Min) / 2 < yMid)child0.push_back(f);
                else child1.push_back(f);
            }
        }
        else {//z축 기준
            for (auto& f : fa)
            {
                double Min = 1E+10, Max = -1E+10;
                for (int i = 0; i < 3; i++)
                {
                    int idx = f.i[i];
                    auto& v = pts[idx];
                    Min = min(Min, v.z);
                    Max = max(Max, v.z);
                }
                if ((Max + Min) / 2 < zMid)child0.push_back(f);
                else child1.push_back(f);
            }
        }
        if (child0.empty() || child1.empty())
            return;
        if (!child0.empty())
            children[0] = new bunnyBV(child0);
        if (!child1.empty())
            children[1] = new bunnyBV(child1);
    }
}
bool bunnyBV::isLeaf()
{
    return children[0] == nullptr && children[1] == nullptr;
}
void bunnyBV::SetBV()
{
    double xMin, yMin, zMin, xMax, yMax, zMax;
    xMin = yMin = zMin = 1E+10;
    xMax = yMax = zMax = -1E+10;
    for (auto& f : fa)
    {
        for (int i = 0; i < 3; i++)
        {
            int idx = f.i[i];
            auto& v = pts[idx];
            xMax = max(xMax, v.x);
            yMax = max(yMax, v.y);
            zMax = max(zMax, v.z);
            xMin = min(xMin, v.x);
            yMin = min(yMin, v.y);
            zMin = min(zMin, v.z);
        }
    }

    BV.max = { xMax,yMax,zMax };
    BV.min = { xMin,yMin,zMin };
}
bunnyBVH* bvh;
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

        // Please implement string parsing / case dividing / data storing code...
        int len = s.length();

        //Use this code below or make your nice code!
        if (s[0] == '#')
            printf("Comment : %s\n", s.c_str());
        else if (s[0] == 'v' && s[1] != 't' && s[1] != 'n')// v
        {
            std::vector<float> v;
            int idx = 2;
            std::string tmp = "";
            while (idx < len) {
                tmp = "";
                while (idx < len && s[idx] != ' ') {
                    tmp += s[idx++];
                }
                v.push_back(stof(tmp));
                idx++;
            }
            pts.push_back({ v[0],v[1],v[2] });
        }
        else if (s[0] == 'v' && s[1] == 'n') // vn
        {
            std::vector<float> v;
            int idx = 3;
            std::string tmp;
            while (idx < len) {
                tmp = "";
                while (idx < len && s[idx] != ' ') {
                    tmp += s[idx++];
                }
                idx++;
                v.push_back(stof(tmp));
            }
            normals.push_back({ v[0],v[1],v[2] });
        }
        else if (s[0] == 'f')//f
        {
            face F;
            bool flag = false;
            int idx = 2;
            int cnt = 0;
            while (idx < len) {
                std::string tmp = "";
                while (idx < len && s[idx] != '/' && s[idx] != ' ') {
                    tmp += s[idx++];
                }
                if (!flag) {
                    F.i[cnt] = stoi(tmp) - 1;
                    flag = !flag;
                }
                else {
                    F.n[cnt++] = stoi(tmp) - 1;
                    flag = !flag;
                }
                idx++;
                if (idx < len && s[idx] == '/')idx++;
            }
            faces.push_back(F);

        }
        else
        {
            printf("Not processed : %s\n", s.c_str());
        }
    }
}

void Drawaabb(bunnyBV bv)
{
    pt Min = bv.BV.min;
    pt Max = bv.BV.max;
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
int maxDepth(bunnyBV bv, int d) {
    if (bv.isLeaf()) {
        return ++d;
    }
    int right = maxDepth(*bv.children[0], ++d);
    int left = maxDepth(*bv.children[0], d);
    return left > right ? left : right;
}
void DrawDFS(bunnyBV bv, int d) {
    if (bv.isLeaf() || d == depth) {
        Drawaabb(bv);
        return;
    }
    DrawDFS(*bv.children[0], ++d);
    DrawDFS(*bv.children[1], d);
    Drawaabb(bv);
}

void DrawModel(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    for (auto& f : faces)
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 3; i++)
        {
            int idx = f.i[i];
            int nIdx = f.n[i];
            auto& v = pts[idx];
            auto& n = normals[nIdx];
            glNormal3d(n.x, n.y, n.z);
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();
    }
    DrawDFS(*bvh->root, 1);
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
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
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    LoadFile();
    bvh = new bunnyBVH(faces);
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
    if (key == GLUT_KEY_PAGE_UP)
    {
        g_fDistance -= 5.0f;
    }
    else if (key == GLUT_KEY_PAGE_DOWN)
    {
        g_fDistance += 5.0f;
    }
    else if (key == GLUT_KEY_UP) {
        depth++;
        if (depth >= mdepth)depth = mdepth;
        std::cout << "Maxdepth : " << depth << "\n";
    }
    else if (key == GLUT_KEY_DOWN) {

        depth--;
        if (depth <= 0)depth = 0;
        std::cout << "Mindepth : 0\n";
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