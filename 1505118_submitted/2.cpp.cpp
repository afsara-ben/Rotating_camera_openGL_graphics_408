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

Point cameraPos,u,r,l;


/* FUNCTIONS */
void drawAxes_();
void drawGrid_();
void specialKeyListener(int key, int x, int y);
void keyboardListener(unsigned char key, int x, int y);
void mouseListener(int button, int state, int x, int y);
void init();
void display();


Point wheelPos;
int wheelRadius;
int wheelAngle;
float wheelMoveBy;
int wheelSegments;


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
    int width = perimeter/segments;

    glPushMatrix();
    {

        glTranslatef(0,0,radius); //moves wheel above xy plane
        glRotatef(90,0,1,0); //makes the wheel stand upright

        glPushMatrix();
        {

            glTranslatef(pos.x,pos.y, pos.z); //goes forward/backward

            glRotatef(angle,1,0,0); //for angular rotation to change direction

            glRotatef(-rotateBy,0,0,1); //wheel rotates while moving forward/backward



            //generate points
            for(i=0; i<=segments; i++)
            {
                points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
                points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
            }

            //draw segments using generated points
            for(i=0; i<segments; i++)
            {
                glPushMatrix();
                {
                    //first circle
                    glBegin(GL_LINES);
                    {
                        glVertex3f(points[i].x,points[i].y,radius/4);
                        glVertex3f(points[i+1].x,points[i+1].y,radius/4);
                    }
                    glEnd();

                    //second circle
                    glBegin(GL_LINES);
                    {
                        glVertex3f(points[i].x,points[i].y,-radius/4);
                        glVertex3f(points[i+1].x,points[i+1].y,-radius/4);
                    }
                    glEnd();
                }
                glPopMatrix();
            }

            //drawing the rectangles
            for(i=0; i<segments; i++)
            {
                glPushMatrix();
                {
                    glColor3f((double)i/(double)segments+0.5, (double)i/(double)segments, (double)i/(double)segments);
                    glBegin(GL_QUADS);
                    glVertex3f(points[i].x,points[i].y,-radius/4);
                    glVertex3f(points[i+1].x,points[i+1].y,-radius/4);
                    glVertex3f(points[i+1].x,points[i+1].y,radius/4);
                    glVertex3f(points[i].x,points[i].y,radius/4);

                    glEnd();
                }
                glPopMatrix();
            }

            //drawing the spikes
            glPushMatrix();
            glColor3f(1,0.4,1);
            glRotatef(90,1,0,0);
            glTranslatef(0,0,0);
            glBegin(GL_QUADS);
            glVertex3f(radius,width/2,0);
            glVertex3f(radius,-width/2,0);
            glVertex3f(-radius,-width/2,0);
            glVertex3f(-radius,width/2,0);
            glEnd();
            glPopMatrix();

            glPushMatrix();
            glColor3f(1,0.4,1);
            glRotatef(90,0,1,0);
            glRotatef(90,0,0,1);
            glTranslatef(0,0,0);
            glBegin(GL_QUADS);
            glVertex3f(radius,width/2,0);
            glVertex3f(radius,-width/2,0);
            glVertex3f(-radius,-width/2,0);
            glVertex3f(-radius,width/2,0);
            glEnd();
            glPopMatrix();

        }
        glPopMatrix();
    }
    glPopMatrix();
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

        wheelPos.y += cos(angInRadian) ;
        wheelPos.z += sin(angInRadian);
        wheelMoveBy += 2.0;
        break;

    case 'd':
        wheelAngle +=2 ;
        break;

    case 'w':
        wheelPos.y -= cos(angInRadian) ;
        wheelPos.z -= sin(angInRadian);
        wheelMoveBy -= 2.0;
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


    cameraPos.x = 110;
    cameraPos.y = 110;
    cameraPos.z = 60;

    wheelPos.x = 0;
    wheelPos.y = 0;
    wheelPos.z = 0;
    wheelAngle = 0;
    wheelMoveBy = 0;
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

    drawWheel_1(wheelRadius, wheelSegments, wheelMoveBy, wheelAngle, wheelPos);


    ///flush
    glutSwapBuffers();

}
void animate()
{
    //angle+=0.05;

    glutPostRedisplay();

}

void reshape(int x, int y)
{
    glViewport(0,0,(GLsizei)x, (GLsizei)y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10,+10,-10,+10);
    glMatrixMode(GL_MODELVIEW);
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
    //glutReshapeFunc(reshape);


    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();

    return 0;

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
