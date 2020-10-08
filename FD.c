#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//Standard C libs ^^

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//Helps OS compatilbility ^^

int ph = 15;
int th = 0;
int p = 1;
int mode = 1;
double dim = 30;
double sc = 1;
int fov=100;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

//PRINT TAKEN FROM ex6
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}


void cube(double x,double y,double z,
             double dx,double dy,double dz,
             double cx, double cy, double cz,
             double th, double ph, double zh,
             int type)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(zh,0,0,1);
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  glBegin(type);

  //  Front
  glColor3f(cx,0,0);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glColor3f(0,0,cz);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  //  Back
  glColor3f(0,0,cz);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glColor3f(cx,0,0);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right
  glColor3f(cx,cy,0);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glColor3f(cx,0,0);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left
  glColor3f(0,cy,0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glColor3f(0,cy,cz);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  //  Top
  glColor3f(0,cy,cz);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glColor3f(cx,0,0);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom
  glColor3f(cx,0,cz);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glColor3f(0,cy,cz);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  //  End
  glEnd();
  glPopMatrix();
}


static void sphere(double x,double y,double z,
                   double cx,double cy,double cz,
                   double r) //FROM ex8
{
   const int d=15;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         //glColor3f(th/360*cz,th/360*cz,th/360*cz);
         glColor3f(Sin(th)*Cos(ph)*cx,Sin(ph)*cy,cz);
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}




void frame(double x, double y, double z,
           double dx, double dy, double dz,
           double cx, double cy, double cz,
           double th)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  cube(0,2.5,0,  1,2.5,2,  cx,cy,cz,  0,0,0, GL_QUADS); //torso
  cube(0,-2.5,1.2,  1,2.5,0.8,  cx,cy,cz,  0,0,0, GL_QUADS); //legs
  cube(0,-2.5,-1.2,  1,2.5,0.8,  cx,cy,cz,  0,0,0, GL_QUADS);

  sphere(0,4.5,2,  cx,cy,cz, 1.2);
  sphere(0,4.5,-2,  cx,cy,cz, 1.2);
  cube(0,4.5,-3  ,0.7,0.7,2,  cx,cy,cz,  0,0,0, GL_QUADS);//arms
  cube(0,4.5,3  ,0.7,0.7,2,  cx,cy,cz,  0,0,0, GL_QUADS);
  sphere(0,5.5,0, cx,cy,cz, 1.5); //head
  glPopMatrix();
}

void stage(double x,double y, double z,
           double dx, double dy, double dz,
           double th)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  cube(0,-5,0,50,5,10,0.1,0.1,0.1,0,0,0, GL_QUADS);
  cube(0,-5,0,50,5,10,1,0,1,0,0,0, GL_LINE_STRIP); //highlight
  cube(0,-5,25,30,5,15,0.2,0.2,0.2,0,0,0, GL_QUADS);
  cube(0,-5,25,30,5,15,1,0,1,0,0,0, GL_LINE_STRIP);
  cube(0,-5,-25,30,5,15,0.2,0.2,0.2,0,0,0, GL_QUADS);
  cube(0,-5,-25,30,5,15,1,0,1,0,0,0, GL_LINE_STRIP);

  glBegin(GL_TRIANGLES);
  glColor3f(1,0,1);

  glVertex3f(50,0,10);
  glVertex3f(30,0,40);
  glVertex3f(30,0,10);

  glVertex3f(50,0,-10);
  glVertex3f(30,0,-40);
  glVertex3f(30,0,-10);

  glVertex3f(-50,0,10);
  glVertex3f(-30,0,40);
  glVertex3f(-30,0,10);

  glVertex3f(-50,0,-10);
  glVertex3f(-30,0,-40);
  glVertex3f(-30,0,-10);

  glVertex3f(50,-10,10);
  glVertex3f(30,-10,40);
  glVertex3f(30,-10,10);

  glVertex3f(50,-10,-10);
  glVertex3f(30,-10,-40);
  glVertex3f(30,-10,-10);

  glVertex3f(-50,-10,10);
  glVertex3f(-30,-10,40);
  glVertex3f(-30,-10,10);

  glVertex3f(-50,-10,-10);
  glVertex3f(-30,-10,-40);
  glVertex3f(-30,-10,-10);

  glEnd();

  glBegin(GL_QUADS);

  glVertex3f(50,0,10);
  glVertex3f(50,-10,10);
  glVertex3f(30,-10,40);
  glVertex3f(30,0,40);

  glVertex3f(-50,0,10);
  glVertex3f(-50,-10,10);
  glVertex3f(-30,-10,40);
  glVertex3f(-30,0,40);

  glVertex3f(50,0,-10);
  glVertex3f(50,-10,-10);
  glVertex3f(30,-10,-40);
  glVertex3f(30,0,-40);

  glVertex3f(-50,0,-10);
  glVertex3f(-50,-10,-10);
  glVertex3f(-30,-10,-40);
  glVertex3f(-30,0,-40);

  glEnd();

  sphere(0,-25,0,0.2,0.1,0.2,24);

  double iy = 0;
  double iz = 0;
  double ix = 0;
  for(int i = 1; i<=400; i+=1)
  {
    ix+=0.1;
    iy+=0.1;
    iz+=0.1;
    cube(ix,-50+(iy*iy)/50,(3*iz)/5,2,0.3,2,1,1,1,60,0,iz+10,GL_QUADS);
    cube(-ix,-50+(iy*iy)/50,(3*iz)/5,2,0.3,2,1,1,1,60,0,-iz-10,GL_QUADS);
    cube(ix,-50+(iy*iy)/50,-(3*iz)/5,2,0.3,2,1,1,1,60,0,iz+10,GL_QUADS);
    cube(-ix,-50+(iy*iy)/50,-(3*iz)/5,2,0.3,2,1,1,1,60,0,-iz-10,GL_QUADS);
  }


  glPopMatrix();
}



void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //clears previous image
  glEnable(GL_DEPTH_TEST);//allow depth testing
  glLoadIdentity(); //loads identity matrix to clear previous transformations
  double Ex = -2*dim*Sin(th)*Cos(ph);
  double Ey = +2*dim        *Sin(ph);
  double Ez = +2*dim*Cos(th)*Cos(ph);
  gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0); //Perspective code borrowed from ex9
  frame(-10,5,0,  1,1,1,  1,1,1, 0);
  frame(20,10,0,  1,2,1,  1,0,1, 80);
  frame(-40,10,0,  1,2,1,  1,1,0, 40);
  stage(1,1,1,1,1,1,0);
  glFlush();
  glutSwapBuffers();
}

static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation

   gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   // else
   //    glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

void special(int key,int x,int y)//from ex6
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
      //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
      //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   Project();
   glutPostRedisplay();
}

void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
  if (ch == 27)
    exit(0);
  else if (ch == '0'){
    th = ph = 0;
    sc = 1; //reset scale and viewing angle
  }
  else if (ch == '+')
    fov++;
  else if (ch == '-') //scale it down
    fov--;
  else if (ch == '1')
    mode = 1;
  else if (ch == '2')
    mode = 2;
  else if (ch == '3')
    mode = 3;
  else if (ch == '4')
    mode = 4;
  Project();
  glutPostRedisplay();
}

void reshape(int width,int height) // !!! BORROWED FROM ex6 !!!
{
  //  Ratio of the width to the height of the window
  double w2h = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();
  //  Orthogonal projection box adjusted for the
  //  aspect ratio of the window
  glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
  Project();
}







int main(int argc, char* argv[])
{
  glutInit(&argc,argv); //Initialize glut with user input
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //Initialize RGB, double buffering, and depth testing
  glutInitWindowSize(500,500); //Initialize window size
  glutCreateWindow("Homework 3 - Zach Morrissey - Scene in 3D"); //Create window with input string as title
  glutDisplayFunc(display); //calls the display function which draws the program
  glutReshapeFunc(reshape);
  glutSpecialFunc(special); //calls a function for arrow key interaction
  glutKeyboardFunc(key); //calls a function for keyboard interaction
  glutMainLoop();

  return 0;
}
