#include <stdio.h>
#include <GL/glut.h>

#define DEFAULT_POLY_WIND_WIDTH	380
#define DEFAULT_POLY_WIND_HEIGHT	300
#define DEF_BUTTON_WIDTH	88
#define DEF_BUTTON_HEIGHT	27


int ParentWindow;
int windowID;
int windw, windh;

int PolygonModeID;
int Polygon_ID;




//-------------------------------------------------------------------
int get_Polygon_ID()
{
	return(Polygon_ID);
}


//-------------------------------------------------------------------
void draw_string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}

//-------------------------------------------------------------------
void drawButton(float x, float y, char bTitle[10], int state)
{
	float x1,x2,y1,y2;
	int dx;

	x1 = x;
	x2 = x + DEF_BUTTON_WIDTH;
	y1 = y;
	y2 = y + DEF_BUTTON_HEIGHT;
	dx = glutBitmapLength(GLUT_BITMAP_HELVETICA_12,bTitle);
    glLineWidth(2);
 	glColor3d(0.8f,0.8f,0.8f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	  glVertex3f(x1,y1,0);
	  glVertex3f(x1,y2,0);
	  glVertex3f(x2,y2,0);
	  glVertex3f(x2,y1,0);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (state==0) glColor3d(1,1,1);
	else glColor3d(0,0,0);
	glBegin(GL_LINES);
	  glVertex3f(x1,y1,0);
	  glVertex3f(x1,y2,0);
	  glVertex3f(x1,y2,0);
	  glVertex3f(x2,y2,0);
	glEnd();
	if (state==0) glColor3d(0,0,0);
	else glColor3d(1,1,1);
	glBegin(GL_LINES);
	  glVertex3f(x1,y1,0);
	  glVertex3f(x2,y1,0);
	  glVertex3f(x2,y1,0);
	  glVertex3f(x2,y2,0);
	glEnd();
	glRasterPos2f(x1 + (x2-x1)/2 - (float)dx/2, y1 + (y2-y1)/2 - 4);
	draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, bTitle);
}


//-------------------------------------------------------------------
void drawTetrahedra(float R, int mode)
{
	int i;
	float v[4][3];
	float atomR;
	atomR = (float)(windw-20)/45.0;
	v[0][0] =  0.0; v[0][1] =  1.0; v[0][2] =  0.0;
	v[1][0] =  1.0; v[1][1] = -0.5; v[1][2] =  0.0;
	v[2][0] = -0.5; v[2][1] = -0.5; v[2][2] =  0.8660254;
	v[3][0] = -0.5; v[3][1] = -0.5; v[3][2] = -0.8660254;

	for (i=0; i<4; i++) {v[i][0] *= R; v[i][1] *= R; v[i][2] *= R;}
//------------------------------------ Draw Central Atom
	if (mode<4){
		glEnable(GL_LIGHTING);
		glColor3f(1,0,1);
		glutSolidSphere(atomR,20,20);
	}
//-------------------------------------------------  Draw Atoms
	if (mode<3){
		glEnable(GL_LIGHTING);
		glColor3f(0,1,0);
		for (i=0; i<4; i++){
			glPushMatrix();
			glTranslatef(v[i][0],v[i][1],v[i][2]);
			glutSolidSphere(atomR,20,20);
			glPopMatrix();
		}
	}
//-------------------------------------------------  Draw Bonds
	if ((mode<2) || (mode == 5)){
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		glLineWidth(3);
		for (i=0; i<4; i++){
			glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(v[i][0],v[i][1],v[i][2]);
			glEnd();
		}
	}
//------------------------------------------------- Draw Polyhedra
	if ((mode>0)&&(mode<5)){
		glDisable(GL_LIGHTING);
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0,0,0);
		glBegin(GL_TRIANGLE_STRIP);
		  for (i=0; i<4; i++) glVertex3f(v[i][0],v[i][1],v[i][2]);
		  glVertex3f(v[0][0],v[0][1],v[0][2]);
		glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1,0,1,0.3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLE_STRIP);
		  glNormal3d(0,0,-1);
		  for (i=0; i<4; i++) glVertex3f(v[i][0],v[i][1],v[i][2]);
		  glVertex3f(v[0][0],v[0][1],v[0][2]);
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}
	glDisable(GL_LIGHTING);
}

//-------------------------------------------------------------------
void drawPolyhedra()
{
	int i,j;
	float dx,dy;
	dy = 0;
	j = 0;
	glNewList(2,GL_COMPILE);
	dx = (float)(windw-20)/3.0;
    glPushMatrix();
	glTranslatef(10,windh-20,0);
	for (i=0; i<6; i++){
		if (i>2) {dy = -dx; j=3;}
		glPushMatrix();
		glTranslatef(dx*(i-j)+dx/2,-dx/2+dy,40);
  	    glRotatef(10,1,0,0);
  	    glRotatef(20,0,1,0);
		drawTetrahedra(45, i);
		glPopMatrix();
	}
	glPopMatrix();
    glEndList();
}

//-------------------------------------------------------------------
void drawSelectedFrame(int frameID)
{
	int j;
	float dx,dy;
	dy = 0;
	j = 0;
	glNewList(3,GL_COMPILE);
	glDisable(GL_LIGHTING);
	dx = (float)(windw-20)/3.0;
	if (frameID>2) {dy = -dx; j = 3;}
	glPushMatrix();
	glTranslatef(10,windh-10,0);

	glLineWidth(3);
	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(dx*(frameID-j),0,0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0,0+dy,0);
	glVertex3f(0,-dx+dy,0);
	glVertex3f(dx,-dx+dy,0);
	glVertex3f(dx,0+dy,0);
	glEnd();
	glPopMatrix();

	glColor3f(1,1,0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
	glTranslatef(dx*(frameID-j),0,0);
	glBegin(GL_POLYGON);
	glVertex3f(0,0+dy,0);
	glVertex3f(0,-dx+dy,0);
	glVertex3f(dx,-dx+dy,0);
	glVertex3f(dx,0+dy,0);
	glEnd();
	glPopMatrix();
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEndList();
}

//-------------------------------------------------------------------
void drawFrame()
{
	int i,j;
	float dx,dy;
	glNewList(1,GL_COMPILE);
	glDisable(GL_LIGHTING);
	dx = (float)(windw-20)/3.0;
    glPushMatrix();
	glTranslatef(10,windh-10,-2);

	glLineWidth(2);
	glColor3f(0,0,1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	dy = 0;
	j = 0;
	for (i=0; i<6; i++){
		if (i>2) {dy = -dx; j=3;}
	    glPushMatrix();
		glTranslatef(dx*(i-j),0,0);
		glBegin(GL_POLYGON);
		glVertex3f(0,0+dy,0);
		glVertex3f(0,-dx+dy,0);
		glVertex3f(dx,-dx+dy,0);
		glVertex3f(dx,0+dy,0);
		glEnd();
		glPopMatrix();
	}
	glColor3f(1,1,1);
	dy = 0;
	j = 0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (i=0; i<6; i++){
		if (i>2) {dy = -dx; j=3;}
	    glPushMatrix();
		glTranslatef(dx*(i-j),0,0);
		glBegin(GL_POLYGON);
		glVertex3f(0,0+dy,0);
		glVertex3f(0,-dx+dy,0);
		glVertex3f(dx,-dx+dy,0);
		glVertex3f(dx,0+dy,0);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEndList();
}

//-------------------------------------------------------------------
void drawOKButton(int ID)
{
	float x,y,dx;
	dx = (float)DEF_BUTTON_WIDTH/2;
	x = (float)windw/2 - dx;
	y = 10;
	glNewList(4,GL_COMPILE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	drawButton(x, y, "OK", ID);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEndList();
}

//-------------------------------------------------------------------
void MouseFunc(int button, int state, int x, int y)
{
	float x0,y0,dx;
	float x1,x2,y1,y2;

	dx = (float)(windw-20)/3.0;
	x0 = 10;
	y0 = 10;

	x1 = (float)windw/2 - (float)DEF_BUTTON_WIDTH/2;
	x2 = (float)windw/2 + (float)DEF_BUTTON_WIDTH/2;
	y1 = 10;
	y2 = 10 + (float)DEF_BUTTON_HEIGHT;

	if ((button==GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		Polygon_ID = -1;
		if ((y>=y0) && (y<=y0+dx)){
			if ((x>=x0) && (x<=x0+dx)) PolygonModeID = 0;
			else
			if ((x>=x0+dx) && (x<=x0+2*dx)) PolygonModeID = 1;
			else
			if ((x>=x0+2*dx) && (x<=x0+3*dx)) PolygonModeID = 2;
			drawSelectedFrame(PolygonModeID);
			glutPostRedisplay();
		}
		else
		if ((y>y0+dx) && (y<=y0+2*dx)){
			if ((x>=x0) && (x<=x0+dx)) PolygonModeID = 3;
			else
			if ((x>=x0+dx) && (x<=x0+2*dx)) PolygonModeID = 4;
			else
			if ((x>=x0+2*dx) && (x<=x0+3*dx)) PolygonModeID = 5;
			drawSelectedFrame(PolygonModeID);
			glutPostRedisplay();
		}
		else
		if ((x > x1) && (x < x2) && (windh-y > y1) && (windh-y < y2)) {
            drawOKButton(1);
			glutDestroyWindow(windowID);
			Polygon_ID = 1;
  		    glutSetWindow(ParentWindow);
			glutPostRedisplay();
 		}
	}
	else {
		Polygon_ID = -1;
		drawOKButton(0);
		glutPostRedisplay();
	}
}

//-------------------------------------------------------------------
void reshape(int wid, int ht)
{
    glutReshapeWindow( windw, windh );
	glutPostRedisplay();
}

//-------------------------------------------------------------------
void redraw(void)
{
	int i;
	glClearColor(0.7f,0.7f,0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-(float)windw/2,-(float)windh/2,0);
	for (i=0; i<4; i++) glCallList(i+1);
	glPopMatrix();
	glFinish();
	glutSwapBuffers();	
}


//-------------------------------------------------------------------
void InitPolygonDialog(int type)
{
	PolygonModeID = type;
	windw = DEFAULT_POLY_WIND_WIDTH;
	windh = DEFAULT_POLY_WIND_HEIGHT;
}

//-------------------------------------------------------------------
void OpenPolygonWindow(int winID, int PosX, int PosY)
{
	float pos[4] = {0,0,0,1};
	float dir[3] = {0,0,1};
	GLfloat mat_specular[] = {1,1,1,1};

	ParentWindow = winID;
	Polygon_ID = -1;
//............................................................................
	glutInitWindowSize(windw, windh);
	glutInitWindowPosition(PosX,PosY);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	windowID = glutCreateWindow("Polyhedral Style");
	glutReshapeFunc(reshape);
	glutDisplayFunc(redraw);
	glutMouseFunc(MouseFunc);
//............................................................................
	glViewport(0, 0, windw, windh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(float)windw/2, (float)windw/2, -(float)windh/2, (float)windh/2, -(float)windw/2, (float)windw/2);
	glMatrixMode(GL_MODELVIEW);
//............................................................................
	pos[0] = windh;
	pos[2] = windh;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
//............................................................................
    drawFrame();
	drawPolyhedra();
    drawSelectedFrame(PolygonModeID);
    drawOKButton(0);
//............................................................................
}


//-------------------------------------------------------------------
int GetPolygonType()
{
	return(PolygonModeID);
}
