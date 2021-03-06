#include<stdio.h>
#include<GLUT/glut.h>
#include<math.h>
#define pi 3.14
GLsizei winwidth=600,winht=600;
GLfloat xwcmin=0.0,xwcmax=130.0,ywcmin=0.0,ywcmax=130.0;
typedef struct wcpt3d
{
	GLfloat x,y,z;
};
void bino(GLint n,GLint *c)
{
	GLint i,j;
	for(i=0;i<=n;i++)
	{
		c[i]=1;
		for(j=n;j>=i+1;j--)
			c[i] *= j;
		for(j=n-i;j>=2;j--)
			c[i] /= j;
	}
}
void computeBezier(GLfloat u,struct wcpt3d *bezpt,GLint nctrlpts,struct wcpt3d *ctrlpts,GLint *c)
{
	GLint i,n=nctrlpts-1;
	GLfloat bezBlend;
	bezpt->x=bezpt->y=bezpt->z=0.0;
	for(i=0;i<nctrlpts;i++)
	{
		bezBlend = c[i]*pow(u,i)*pow(1-u,n-i);
		bezpt->x += ctrlpts[i].x*bezBlend;
		bezpt->y += ctrlpts[i].y*bezBlend;
		bezpt->z += ctrlpts[i].z*bezBlend;
	}
}
void bezier(struct wcpt3d *ctrlpts,GLint nctrlpts,GLint nBezCurvePts)
{
	struct wcpt3d bezcurvept;
	GLfloat u;
	GLint *c,k;
	c=(GLint*)malloc(nctrlpts);
	bino(nctrlpts-1,c);
	glBegin(GL_LINE_STRIP);
		for(k=0;k<=nBezCurvePts;k++)
		{
			u = (GLfloat)k/(GLfloat)nBezCurvePts;
			computeBezier(u,&bezcurvept,nctrlpts,ctrlpts,c);
			glVertex2f(bezcurvept.x,bezcurvept.y);
		}
	glEnd();
	free(c);
}
void display()
{
	GLint nctrlpts=4,nBezCurvePts=20;
	static float theta = 0;
	struct wcpt3d ctrlpts[4]={{20,100,0},{30,110,0},{50,90,0},{60,100,0}};
	ctrlpts[1].x += 10*sin(theta*pi/180.0);
	ctrlpts[1].y += 5*sin(theta*pi/180.0);
	ctrlpts[2].x -= 10*sin((theta+30)*pi/180.0);
	ctrlpts[2].y -= 10*sin((theta+30)*pi/180.0);
	ctrlpts[3].x -= 4*sin(theta*pi/180.0);
	ctrlpts[3].y += sin((theta-30)*pi/180.0);
	theta += 0.10;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glPointSize(5.0);
	glPushMatrix();
	glLineWidth(5);
	glColor3f(1.0,0.5,0.0); //orange
	for(int i=0;i<8;i++)
	{
		glTranslatef(0,-0.8,0);
		bezier(ctrlpts,nctrlpts,nBezCurvePts);
	}
	glColor3f(1.0,1.0,1.0); //white
	for(int i=0;i<8;i++)
	{
		glTranslatef(0.0,-0.8,0.0);
		bezier(ctrlpts,nctrlpts,nBezCurvePts);
	}
	glColor3f(0.0,0.5,0.0);  //green
	for(int i=0;i<8;i++)
	{
		glTranslatef(0.0,-0.8,0.0);
		bezier(ctrlpts,nctrlpts,nBezCurvePts);
	}
	glPopMatrix();
	glColor3f(0.7,0.5,0.3);
	glLineWidth(5);
	glBegin(GL_LINES);
		glVertex2f(20,100);
		glVertex2f(20,40);
	glEnd();
	glFlush();
	glutPostRedisplay();
	glutSwapBuffers();
}
void winReshapeFunc(GLint newwidth,GLint newht)
{
	glViewport(0,0,newwidth,newht);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xwcmin,xwcmax,ywcmin,ywcmax);
	glClear(GL_COLOR_BUFFER_BIT);
}
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(winwidth,winht);
	glutCreateWindow("FLAG");
	glutDisplayFunc(display);
	glutReshapeFunc(winReshapeFunc);
	glutMainLoop();
}

