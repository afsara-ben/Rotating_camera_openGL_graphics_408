#include <stdio.h>
#include<stdlib.h>
#include<math.h>

#include<windows.h>
#include<GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawGrid;
int drawAxes;
float angle;

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=5.0f;

struct Point
{
    float x,y,z;
};

Point pos,u,r,l;

void drawSphere(double radius, int slices, int stacks)
{
    struct Point points[100][100];
    int i,j;
    double h,r;
    for(i=0; i<stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x = r*cos(((double)j/(double)slices) * 2 * pi);
            points[i][j].y = r*sin(((double)j/(double)slices) * 2 * pi);
            points[i][j].z = h;

        }
    }
    ///draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f((double)i/(double)stacks+0.5,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void drawAxes_()
{
    if(drawAxes == 1)
    {

        glBegin(GL_LINES);

        ///red x
        glColor3f(1.0,0,0);
        glVertex3f(100,0,0);
        glVertex3f(-100,0,0);

        ///green y
        glColor3f(0,1.0,0);
        glVertex3f(0,100,0);
        glVertex3f(0,-100,0);

        ///blue z
        glColor3f(0,0,1.0);
        glVertex3f(0,0,100);
        glVertex3f(0,0,-100);

        glEnd();
    }

}

void drawGrid_()
{

    int i;
    if(drawGrid == 1)
    {

        glColor3f(1,0.6,0.6);
        glBegin(GL_LINES);

        for(i = -8; i <=8; i++)
        {
            if(i ==0 )
                continue;
            ///lines parallel to y axis
            glVertex3f(i*10,-90,0);
            glVertex3f(i*10,90,0);

            ///lines parallel to x axis
            glVertex3f(-90,i*10,0);
            glVertex3f(90,i*10,0);
        }

        glEnd();
    }
}

void drawSS()
{


}

void specialKeyListener(int key, int x, int y)
{
    printf("%d\n",key);
    float fraction = 3.0f;

    switch(key)
    {
    //backward
    case GLUT_KEY_DOWN :

        pos.x += l.x + fraction;
        pos.y += l.y + fraction;

        break;

    //forward
    case GLUT_KEY_UP :

        pos.x -= l.x + fraction;
        pos.y -= l.y + fraction;
        break;

    case GLUT_KEY_LEFT :
        pos.x -= 2.0;
        pos.y += 2.0;
//       angle -= 0.1f;
//        l.x = sin(angle);
//        l.y = -cos(angle);
        break;

    case GLUT_KEY_RIGHT :
        pos.x += 2.0;
        pos.y -= 2.0;

//        angle += 0.1f;
//        l.x = sin(angle);
//        l.y= -cos(angle);

        break;

    case GLUT_KEY_PAGE_UP:
        pos.z += 3.0;
        break;

    case GLUT_KEY_PAGE_DOWN:
        pos.z -= 3.0;
        break;


    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}

Point cartesianToPolar(Point vec)
{
    Point polarVec;
    int r,theta;
    int x = vec.x;
    int y = vec.y;

    r = sqrt(x*x + y*y);
    theta = atan(y/x);

    polarVec.x = r*cos(theta);
    polarVec.y = r*sin(theta);
    polarVec.z = vec.z;

    return polarVec;
}

Point crossProduct(const Point &vec1, const Point &vec2)
{

    Point res;
    res.x = vec1.y*vec2.z-vec2.y*vec1.z;
    res.y = -(vec1.z*vec2.x-vec2.z*vec1.x);
    res.z = vec1.x*vec2.y-vec2.x*vec1.y;

    return res;
}
void keyboardListener(unsigned char key, int x, int y)
{
    printf("%c\n",key);
    double storeAngle;
    Point l_, u_, r_;

    switch(key)
    {
///rotate left
    case '1':

//        l_=l, r_=r, u_=u;
//        storeAngle = angle;
//        //angle*=-1;

//        printf("%f %f %f \n",r.x,r.y,r.z);
//
//        r_.x = r.x*cos(angle) - r.y * sin(angle);
//        r_.y = r.y*cos(angle) + r.x * sin(angle);
//
//
//        r.x = r_.x;
//        r.y = r_.y;
//
//        l=crossProduct(u,r);
//        printf("l.x = %f, l.y = %f, l.z = %f\n",l.x,l.y,l.z);
//       // angle = storeAngle;

///        l.x = l.x*cos(-angle) + r.x*sin(-angle);
///        l.y = l.y*cos(-angle) + r.y*sin(-angle);
/////        l.z = l.z*cos(-angle) + r.z*sin(-angle);
//
        l_=l,r_=r,u_=u;
        storeAngle = angle;
        angle*=-1;

        r.x=r_.x*cos(angle)+l_.x*sin(angle);
        r.y=r_.y*cos(angle)+l_.y*sin(angle);
        r.z=r_.z*cos(angle)+l_.z*sin(angle);

        l=crossProduct(u,r);
        angle = storeAngle;

//        r.x = l.x*cos(-angle) + r.x*sin(-angle);
//        r.y = l.y*cos(-angle) + r.y*sin(-angle);
//        r.z = l.z*cos(-angle) + r.z*sin(-angle);
//        l=crossProduct(u,r);
        break;

    case '2':

        r_.x = r.x*cos(-angle) - r.y * sin(-angle);
        r_.y = r.y*cos(-angle) + r.x * sin(-angle);


        r.x = r_.x;
        r.y = r_.y;

        l=crossProduct(u,r);
        printf("l.x = %f, l.y = %f, l.z = %f\n",l.x,l.y,l.z);

//        angle += 0.1f;
//        l.x = l.x*sin(angle)*10;
//        l.y = -l.y*cos(angle)*10;
//        printf("l.x = %f, l.y = %f\n",l.x,l.y);
//        l_=l,r_=r,u_=u;
//        storeAngle = angle;
//
//
//        r.x=r_.x*cos(angle)+l_.x*sin(angle);
//        r.y=r_.y*cos(angle)+l_.y*sin(angle);
//        r.z=r_.z*cos(angle)+l_.z*sin(angle);
//
//        l=crossProduct(u,r);
//        angle = storeAngle;
        break;


    case '3':

        break;

    case '4':

        break;

    case '5':

        break;

    case '6':

        break;

    case 'G':
        drawGrid = 1- drawGrid;
        break;

    default:
        break;
    }

}

void mouseListener(int button, int state, int x, int y)
{
    printf("%d %d\n",button, state);
}

void init()
{

    ///initialization
    drawGrid = 0;
    drawAxes = 1;
    cameraHeight = 0.0;
    cameraAngle = 1.0;
    angle = 0.05f;

    pos.x = 100;
    pos.y = 100;
    pos.z = 0;

    l.x = -1/sqrt(2);
    l.y = -1/sqrt(2);
    l.z = 0;

    u.x = 0;
    u.y = 0;
    u.z = 1;

    r.x = -1/sqrt(2);
    r.y = 1/sqrt(2);
    r.z = 0;




    ///clear screen
    glClearColor(0.1,0.5,0.1,0);

    ///set up projection here
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1,1,1000.0);

}


void display()
{
    ///clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ///set up camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(pos.x, pos.y, pos.z,		0,0,0,		0,0,1);
    //gluLookAt(-100,100,200, 0,0,0, 0,1,0);
    gluLookAt(	pos.x, pos.y, pos.z,
                pos.x+l.x, pos.y+l.y,  pos.z+l.z,
                u.x, u.y, u.z);


    glMatrixMode(GL_MODELVIEW);

    ///add objects from here

    drawAxes_();
    drawGrid_();
    drawSS();
    //drawSphere(30,30,50);

///draw

///flush
    glutSwapBuffers();

}
void animate()
{
    //angle+=0.05;

    glutPostRedisplay();

}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(300,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("DEMO");

    init();

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);
    glutMainLoop();

    return 0;

}
