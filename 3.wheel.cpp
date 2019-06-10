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


struct Point
{
    float x,y,z;
};

Point pos,u,r,l;


/* FUNCTIONS */
void drawAxes_();
void drawGrid_();
void specialKeyListener(int key, int x, int y);
void keyboardListener(unsigned char key, int x, int y);
void mouseListener(int button, int state, int x, int y);
void init();
void display();


Point wheelpos;
int wheelRadius;
int wheelAngle;
float wheelDist;
int wheelSegments;


/*
    The wheel is made with only rectangles
    and translating and rotating the
    rectangular slabs
*/

void drawWheel(int radius, int segments,  float angle, float dist, Point pos )
{
    int i;
    float perimeter = 2*pi*radius;
    double rotateBy = 360*dist/perimeter;

    glPushMatrix();
    {

        glTranslatef(0,0,radius);
        glRotated(90,0,1,0);
        glPushMatrix();
        {

            glTranslatef(pos.x,pos.y,pos.z);
            glRotatef(angle,1,0,0);
            glRotatef(rotateBy,0,0,1);


            float width = perimeter / segments;
            //width /= 2;

            //draw segments using generated points
            for(i=0; i<segments; i++)
            {

                glPushMatrix();
                {
                    glColor3f((double)i/(double)segments+0.5, (double)i/(double)segments, (double)i/(double)segments);

                    glRotatef((((double)i/(double)segments)*360.0),0,0,1);
                    glTranslatef(radius,0,0);
                    //glRotatef(180,0,0,1);

                    //glRotated(270,1,0,0);
                    glBegin(GL_QUADS);
                    glVertex3f(0,width, width);
                    glVertex3f(0,width,-width);
                    glVertex3f(0,-width, -width);
                    glVertex3f(0,-width, width);
                    glEnd();

                }
                glPopMatrix();

            }
            // the two spikes
            glBegin(GL_QUADS);
            glVertex3f(0,radius, width);
            glVertex3f(0,radius,-width);
            glVertex3f(0,-radius, -width);
            glVertex3f(0,-radius, width);
            glEnd();

            glRotatef(90,0,0,1);

            glBegin(GL_QUADS);
            glVertex3f(0,radius, width);
            glVertex3f(0,radius,-width);
            glVertex3f(0,-radius, -width);
            glVertex3f(0,-radius, width);
            glEnd();
        }
        glPopMatrix();
    }
    glPopMatrix();

}

/*
    The wheel is made with only two circles
    and rectangles in the middle
*/

void drawWheel_1(int radius, int segments, int dist,int angle, Point pos)
{
    int i;

    struct Point points[100];
    float perimeter = 2*pi*radius;
    //float moveBy = perimeter * angle/360;
    float rotateBy = 360*dist/perimeter;

    glPushMatrix();
    {

        glTranslatef(0,0,20);
        glRotatef(90,0,1,0);

        glPushMatrix();
        {

            glTranslatef(pos.x,pos.y, pos.z);

            glRotatef(angle,1,0,0);
            glRotatef(-rotateBy,0,0,1);

            //generate points
            for(i=0; i<=segments; i++)
            {
                points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
                points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
            }

            //draw segments using generated points
            for(i=0; i<segments; i++)
            {
                //first circle
                glBegin(GL_LINES);
                {
                    glVertex3f(points[i].x,points[i].y,0);
                    glVertex3f(points[i+1].x,points[i+1].y,0);
                }
                glEnd();

                //second circle
                glBegin(GL_LINES);
                {
                    glVertex3f(points[i].x,points[i].y,10);
                    glVertex3f(points[i+1].x,points[i+1].y,10);
                }
                glEnd();
            }

            //drawing the rectangles
            for(i=0; i<segments; i++)
            {
                glColor3f((double)i/(double)segments+0.5, (double)i/(double)segments, (double)i/(double)segments);
                glBegin(GL_QUADS);
                glVertex3f(points[i].x,points[i].y,0);
                glVertex3f(points[i+1].x,points[i+1].y,0);
                glVertex3f(points[i+1].x,points[i+1].y,10);
                glVertex3f(points[i].x,points[i].y,10);

                glEnd();
            }
        }
        glPopMatrix();
    }
    glPopMatrix();
}

/*
    tried to draw the wheel using trianglular fans
*/

//void drawWheel()
//{
//    float x,y;
//
//    int radius = 20;
//    int segs = 40;
//    int step = (360 / segs) * 3.1416/pi;
//    float prev_x, prev_y;
// glColor3f(0,0,1);


//glPushMatrix();
//glRotatef(-90,0,1,0);
//drawWheelBody(radius, segs);
//    drawCircle(radius,segs);
//    drawCircle(radius+10,segs);

//glTranslatef(0,0,20);
//drawCircle(radius,segs);
//drawCircle(radius+10,segs);
//    glPopMatrix();

//    glPushMatrix();
//    glRotatef(-90,0,1,0);
//    for(i=0; i<segs; ++i)
//    {
//        prev_x = x;
//        prev_y = y;
//
//        x = sin(step * i) * radius;
//        y = cos(step * i) * radius;
//
//        glColor3f((double)i/(double)segs+0.5, (double)i/(double)segs, (double)i/(double)segs);
//        glBegin(GL_TRIANGLES);
//        glVertex3f(10, 0, 0);
//
//        glVertex3f(x+10, y, 0);
//        glVertex3f(prev_x+10, prev_y,0);
//        glEnd();
//    }
//    //glVertex3f(radius, 10, 10);
//    glPopMatrix();

//}


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


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        cameraHeight -= 3.0;
        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        break;

    default:
        break;
    }
}

void keyboardListener(unsigned char key, int x, int y)
{
    printf("%c\n",key);
    Point l_, u_, r_;
    float angInRadian = wheelAngle * pi/180; //converting degree to radian
    switch(key)
    {

    case 's':

        wheelpos.y += cos(angInRadian) ;
        wheelpos.z += sin(angInRadian);
        wheelDist += 2.0;
        break;

    case 'd':
        wheelAngle +=2 ;
        break;

    case 'w':
        wheelpos.y -= cos(angInRadian) ;
        wheelpos.z -= sin(angInRadian);
        wheelDist -= 2.0;
        break;

    case 'a':
        wheelAngle -=2 ;
        break;

    case 'q':
        exit(1);
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
    drawGrid = 1;
    drawAxes = 1;
    cameraHeight = 40.0;
    cameraAngle = 1.0;
    //angle = 0.05f;


    pos.x = 110;
    pos.y = 110;
    pos.z = 40;

    wheelpos.x = 0;
    wheelpos.y = 0;
    wheelpos.z = 0;
    wheelAngle = 0;
    wheelDist = 0;
    wheelRadius = 20;
    wheelSegments = 20;

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

    gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);


    glMatrixMode(GL_MODELVIEW);

    ///add objects from here

    drawAxes_();
    drawGrid_();

    drawWheel_1(wheelRadius, wheelSegments, wheelDist, wheelAngle, wheelpos);
    //drawWheel(wheelRadius,wheelSegments, wheelAngle,wheelDist, wheelpos);
    //drawWheel(wheelpos, wheelAngle, wheelDist, wheelRadius);

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

    glutCreateWindow("DEMO_WHEEL");

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
