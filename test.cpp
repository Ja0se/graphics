#include <stdio.h>
#include <cmath>
#include <vector>
#include <random>
#include "GL/freeglut.h"
#include <queue>
GLsizei width = 1000, height = 1000;
double PI = 3.14159265;
int hitIdx = -1;
bool useBVH = true;
bool drawInternalBV = false;
double triLength = 30.0;
int triNum = 100;
using xy = double[2];
struct Ball
{
    double x, y;
    double rad;
};
struct Triangle
{
    xy pt[3];
};
std::vector<Triangle> tris;
bool IsCollision(Ball b0, Ball b1);
bool IsCollision(Triangle t0, Triangle t1);
void DrawBall(Ball b, bool isFill = false);
void DrawTriangle(Triangle b, bool isFill = false);
void Barycentric(xy p, xy a, xy b, xy c, double& u, double& v, double& w);

// ---------------------------- BV BVH BVTT -------------------------------
class BallBV
{
public:
    Ball BV;
    std::vector<Triangle> tris;
    BallBV* children[2] = { nullptr,nullptr };
    BallBV(std::vector<Triangle> inputData);
    bool isLeaf();
    void SetBV();
};

class BallBVH
{
public:
    BallBV* root = nullptr;
    BallBVH(std::vector<Triangle> inputData);
};

class BallBVTT
{
public:
    BallBVH* bvh0, * bvh1;
    BallBVTT(BallBVH* b0, BallBVH* b1);
    std::vector<std::pair<Triangle, Triangle>> GetCollisions();
};

BallBVTT::BallBVTT(BallBVH* b0, BallBVH* b1)
{
    bvh0 = b0;
    bvh1 = b1;
}

std::vector<std::pair<Triangle, Triangle>> BallBVTT::GetCollisions()
{
    std::vector<std::pair<Triangle, Triangle>> collisions;
    std::queue<std::pair<BallBV*, BallBV*>> q;
    q.push({ bvh0->root, bvh1->root });

    while (!q.empty())
    {
        auto pair = q.front();
        q.pop();

        if (drawInternalBV)
        {
            glColor3f(0, 1, 0);
            DrawBall(pair.first->BV, false);
        }

        if (IsCollision(pair.first->BV, pair.second->BV))
        {
            if (!pair.first->isLeaf() && !pair.second->isLeaf())
            {
                if (pair.first->tris.size() > pair.second->tris.size())
                    for (auto c : pair.first->children)
                        q.push({ c,pair.second });
                else
                    for (auto c : pair.second->children)
                        q.push({ pair.first,c });
            }
            else if (!pair.first->isLeaf() && pair.second->isLeaf())
            {
                for (auto c : pair.first->children)
                    q.push({ c,pair.second });
            }
            else if (pair.first->isLeaf() && !pair.second->isLeaf())
            {
                for (auto c : pair.second->children)
                    q.push({ pair.first,c });
            }
            else
            {
                for (auto& fc : pair.first->tris)
                    for (auto& sc : pair.second->tris)
                        if (IsCollision(fc, sc))
                            collisions.push_back({ fc,sc });
            }
        }
    }
    return collisions;
}

BallBVH::BallBVH(std::vector<Triangle> inputData)
{
    root = new BallBV(inputData);
}

BallBV::BallBV(std::vector<Triangle> inputData)
{
    tris = inputData;
    SetBV();
    if (tris.size() > 2)
    {
        std::vector<Triangle> child0, child1;

        double xMin = 1E+10, xMax = -1E+10;
        for (auto& tri : tris)
        {
            if (tri.pt[0][0] < xMin)
                xMin = tri.pt[0][0];
            if (tri.pt[0][0] > xMax)
                xMax = tri.pt[0][0];
        }
        double xMid = (xMin + xMax) / 2.0;
        for (auto& tri : tris)
        {
            if (tri.pt[0][0] < xMid)
                child0.push_back(tri);
            else
                child1.push_back(tri);
        }
        if (child0.empty() || child1.empty())
            return;
        if (!child0.empty())
            children[0] = new BallBV(child0);
        if (!child1.empty())
            children[1] = new BallBV(child1);
    }
}

bool BallBV::isLeaf()
{
    return children[0] == nullptr && children[1] == nullptr;
}

void BallBV::SetBV()
{
    double xAvg = 0, yAvg = 0;
    for (auto& tri : tris)
    {
        xAvg += tri.pt[0][0];
        yAvg += tri.pt[0][1];
    }
    xAvg /= tris.size();
    yAvg /= tris.size();

    double maxDist = 0.0;
    for (auto& tri : tris)
    {
        for (int i = 0; i < 3; i++)
        {
            double dist = sqrt(pow(xAvg - tri.pt[i][0], 2) + pow(yAvg - tri.pt[i][1], 2));
            if (dist > maxDist)
                maxDist = dist;
        }
    }

    maxDist += 5.0;
    BV.rad = maxDist;
    BV.x = xAvg;
    BV.y = yAvg;
}

BallBVH* bvh0, * bvh1;
BallBVTT* bvtt;
BallBV* redBV = nullptr;
// ---------------------------- BV BVH BVTT (End) -------------------------------

void DrawBall(Ball b, bool isFill)
{
    glPushMatrix();
    glTranslated(b.x, b.y, 0);
    isFill ? glBegin(GL_POLYGON) : glBegin(GL_LINE_STRIP);
    int res = 30;
    for (int i = 0; i <= res; i++)
        glVertex2d(b.rad * cos(PI * 2.0 * double(i) / double(res)), b.rad * sin(PI * 2.0 * double(i) / double(res)));
    glEnd();
    glPopMatrix();
}

void DrawTriangle(Triangle t, bool isFill)
{
    isFill ? glBegin(GL_POLYGON) : glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 3; i++)
        glVertex2d(t.pt[i][0], t.pt[i][1]);
    if (!isFill)
        glVertex2d(t.pt[0][0], t.pt[0][1]);
    glEnd();
}

bool IsCollision(Ball b0, Ball b1)
{
    double dist = sqrt(pow(b0.x - b1.x, 2) + pow(b0.y - b1.y, 2));
    return dist < b0.rad + b1.rad;
}

bool IsCollision(Triangle b0, Triangle b1)
{
    xy a, b, c;
    for (int i = 0; i < 2; i++)
    {
        a[i] = b0.pt[0][i]; b[i] = b0.pt[1][i]; c[i] = b0.pt[2][i];
    }
    for (int pIdx = 0; pIdx < 3; pIdx++)
    {
        xy p;
        p[0] = b1.pt[pIdx][0];
        p[1] = b1.pt[pIdx][1];
        double u, v, w;
        Barycentric(p, a, b, c, u, v, w);
        if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
            return true;
    }

    for (int i = 0; i < 2; i++)
    {
        a[i] = b1.pt[0][i]; b[i] = b1.pt[1][i]; c[i] = b1.pt[2][i];
    }
    for (int pIdx = 0; pIdx < 3; pIdx++)
    {
        xy p;
        p[0] = b0.pt[pIdx][0];
        p[1] = b0.pt[pIdx][1];
        double u, v, w;
        Barycentric(p, a, b, c, u, v, w);
        if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1)
            return true;
    }

    return false;
}

int hit_index(int x, int y)
{
    int tx = redBV->BV.x - x;
    int ty = redBV->BV.y - y;

    xy mouse;
    mouse[0] = x; mouse[1] = y;
    double u, v, w;
    Barycentric(mouse, redBV->tris[0].pt[0], redBV->tris[0].pt[1], redBV->tris[0].pt[2], u, v, w);

    if (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1) return 0;
    return -1;
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width, 0, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100, 900);

    for (int i = 0; i < triNum; i++)
    {
        Triangle tri;
        tri.pt[0][0] = dis(gen);
        tri.pt[0][1] = dis(gen);

        for (int j = 0; j < 2; j++)
        {
            tri.pt[1][j] = tri.pt[0][j];
            tri.pt[2][j] = tri.pt[0][j];
        }
        tri.pt[1][0] += triLength;
        tri.pt[2][1] += triLength;
        tris.push_back(tri);
    }

    bvh0 = new BallBVH(tris);

    std::vector<Triangle> redTris;
    Triangle redTri;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
            redTri.pt[i][j] = 100;
    redTri.pt[1][0] += triLength;
    redTri.pt[2][1] += triLength;
    redTris.push_back(redTri);

    bvh1 = new BallBVH(redTris);
    redBV = bvh1->root;

    bvtt = new BallBVTT(bvh0, bvh1);
}

void Barycentric(xy p, xy a, xy b, xy c, double& u, double& v, double& w)
{
    xy v0, v1, v2;
    for (int i = 0; i < 2; i++)
    {
        v0[i] = b[i] - a[i];
        v1[i] = c[i] - a[i];
        v2[i] = p[i] - a[i];
    }
    double d00 = v0[0] * v0[0] + v0[1] * v0[1];
    double d01 = v0[0] * v1[0] + v0[1] * v1[1];
    double d11 = v1[0] * v1[0] + v1[1] * v1[1];
    double d20 = v2[0] * v0[0] + v2[1] * v0[1];
    double d21 = v2[0] * v1[0] + v2[1] * v1[1];

    double denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
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
    glClear(GL_COLOR_BUFFER_BIT);

    printf("USEBVH : %s\n", useBVH ? "YES" : "NO");
    printf("DrawBV : %s\n\n\n\n", drawInternalBV ? "YES" : "NO");

    glLineWidth(1.0);
    if (useBVH)
    {
        auto collisions = bvtt->GetCollisions();
        glColor3f(0, 1, 0);
        for (auto& pair : collisions)
            DrawTriangle(pair.first, true);
    }
    else
    {
        glColor3f(0, 1, 1);
        for (auto& tri : tris)
            if (IsCollision(tri, bvh1->root->tris[0]))
                DrawTriangle(tri, true);
    }

    glLineWidth(1.0);
    glColor3f(0, 0, 1);
    for (auto& ball : tris)
        DrawTriangle(ball, false);

    glLineWidth(5.0);
    glColor3f(1, 0, 0);
    DrawTriangle(redBV->tris[0]);
    glLineWidth(1.0);
    if (drawInternalBV && useBVH)
        DrawBall(redBV->BV);

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
            hitIdx = hit_index(x, height - y);
            break;
        case GLUT_UP:
            hitIdx = -1;
            break;
        default: break;
        }
    }
    glutPostRedisplay();
}

// void glutMotionFunc(void (*func)(int x, int y));
void mouse_move_callback(GLint x, GLint y)
{
    if (hitIdx != -1)
    {
        redBV->BV.x = x;
        redBV->BV.y = height - y;
        redBV->tris[0].pt[0][0] = redBV->BV.x;
        redBV->tris[0].pt[0][1] = redBV->BV.y;
        redBV->tris[0].pt[1][0] = redBV->tris[0].pt[0][0];
        redBV->tris[0].pt[1][1] = redBV->tris[0].pt[0][1];
        redBV->tris[0].pt[2][0] = redBV->tris[0].pt[0][0];
        redBV->tris[0].pt[2][1] = redBV->tris[0].pt[0][1];
        redBV->tris[0].pt[1][0] += triLength;
        redBV->tris[0].pt[2][1] += triLength;
    }
    glutPostRedisplay();
}

// void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void keyboard_callback(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'i': case 'I':
        drawInternalBV ^= true;
        break;
    case 'b': case 'B':
        useBVH ^= true;
        break;
    case (27): exit(0); break;
    default: break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("Circle BVH");
    init();
    glutReshapeFunc(reshape_callback);
    glutMouseFunc(mouse_callback);
    glutMotionFunc(mouse_move_callback);
    glutDisplayFunc(display_callback);
    glutKeyboardFunc(keyboard_callback);
    glutMainLoop();
    return 0;
}
