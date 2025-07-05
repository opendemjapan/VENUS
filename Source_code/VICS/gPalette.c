#include <stdio.h>
#include <GL/glut.h>
#include "GlutPalette.h"

static float paletteRGB[3];
static float paletteHLS[3];
int PalettedialogID;

int ParentWindow;
int windowID;
int motionID = 0;
int windw, windh, frameW;
float cW, cHi;

//-------------------------------------------------------------------
void string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}

//-------------------------------------------------------------------
void _drawButton(float x, float y, char bTitle[10], int state)
{
	float x1,x2,y1,y2;
	int dx;

	x1 = x;
	x2 = x + DEF_BUTTON_WIDTH;
	y1 = y;
	y2 = y + DEF_BUTTON_HEIGHT;
	dx = glutBitmapLength(GLUT_BITMAP_HELVETICA_12,bTitle);

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
	string_bitmap(GLUT_BITMAP_HELVETICA_12, bTitle);
}


//-------------------------------------------------------------------
double max_of(float Red, float Green, float Blue)
{
	double max;
	max = (Red > Green) ? Red : Green;
	max = (Blue > max) ? Blue : max;
	return(max);
}


//-------------------------------------------------------------------
double min_of(float Red, float Green, float Blue)
{
	double min;
	min = (Red < Green) ? Red : Green;
	min = (Blue < min) ? Blue : min;
	return(min);
}
 
                                                        
//-------------------------------------------------------------------
void RGB2HLS(float r, float g, float b)
{
	float max_value,min_value;
	double diff,r_dist,g_dist,b_dist;
	double small_value = 0.0000001;

	max_value = max_of(r,g,b);
	min_value = min_of(r,g,b);
	diff = max_value - min_value;

	paletteHLS[1] = (max_value + min_value)/2;
	if (fabs(diff) <= 0.0000001) {
		paletteHLS[0] = 1; //undefined;
		paletteHLS[2] = 0; //undefined;
	}
	else {
		if (paletteHLS[1] < 0.5) paletteHLS[2] = diff/(max_value + min_value);
		else paletteHLS[2] = diff / (2 - max_value - min_value);      
		r_dist = (max_value - r) / diff;
		g_dist = (max_value - g) / diff;
		b_dist = (max_value - b) / diff;
		if      (r == max_value) paletteHLS[0] = b_dist - g_dist;
		else if (g == max_value) paletteHLS[0] = 2 + r_dist - b_dist;
		else if (b == max_value) paletteHLS[0] = 4 + g_dist - r_dist;
		paletteHLS[0] *= 60;
		if (paletteHLS[0] < 0) paletteHLS[0] += 360;
	}
}

//-------------------------------------------------------------------
float HLSValue(float m1, float m2, float hue)
{
	if (hue >= 360) hue -= 360;
	if (hue < 0)    hue += 360;
	if (hue < 60)        return m1 + (m2 - m1)*hue/60;
	else if (hue <180)   return m2;
	else if (hue < 240)  return m1 + (m2 - m1)*(240 - hue)/60;
	else                 return m1;
}

//-------------------------------------------------------------------
void HLS2RGB(float h, float l, float s)
{
	float m1,m2;
	if (l <= 0.5) m2 = l * (1 + s);
	         else m2 = l + s - l*s;
    m1 = 2 * l - m2;
	if (s == 0) paletteRGB[0] = paletteRGB[1] = paletteRGB[2] = l;
	else {
		paletteRGB[0] = HLSValue(m1, m2, h + 120);
		paletteRGB[1] = HLSValue(m1, m2, h);
		paletteRGB[2] = HLSValue(m1, m2, h - 120);
	}
}

//-------------------------------------------------------------------
void drawHLS(int pWinW, int pWinH)
{
	float h,s,l;
	float x,y;
	int i,j;
	l = 0.5;
	glNewList(1,GL_COMPILE);
	glPointSize(2);
	glBegin(GL_POINTS);
	for (i=0; i<pWinW; i++){
		for (j=0; j<pWinH; j++){
			h = ((float)i / (float)pWinW)*360;
			s = (float)j / (float)pWinH;
			HLS2RGB(h,l,s);
			x = h * pWinW / 360;
			y = s * pWinH;
			glColor3f(paletteRGB[0],paletteRGB[1],paletteRGB[2]);
			glVertex3f(x,y,0);
		}
	}
	glEnd();
	glEndList();
}

//-------------------------------------------------------------------
void drawHLScell(int pWinW, int pWinH)
{
	glNewList(2,GL_COMPILE);
	glLineWidth(2.0);
	glColor3d(1,1,0);
	glBegin(GL_LINE_LOOP);
	  glVertex3f(0,0,0);
	  glVertex3f(0,pWinH,0);
	  glVertex3f(pWinW,pWinH,0);
	  glVertex3f(pWinW,0,0);
	glEnd();
	glEndList();
}


//-------------------------------------------------------------------
void drawRGB(int pWinW, int pWinH, float h, float s)
{
	float l;
	float y;
	int i;

	glNewList(3,GL_COMPILE);
	glLineWidth(2.0);
	for (i=0; i<pWinH; i++){
		l = (float)i / (float)pWinH;
		HLS2RGB(h,l,s);
		y = (float)i;
		glColor3f(paletteRGB[0],paletteRGB[1],paletteRGB[2]);
		glBegin(GL_LINES);
		glVertex3f(pWinW,y,0);
		glVertex3f(windw,y,0);
		glEnd();
	}
	glEndList();
}

//-------------------------------------------------------------------
void drawRGBcell(int pWinW, int pWinH)
{
	glNewList(4,GL_COMPILE);
	glLineWidth(2.0);
	glColor3d(1,1,0);
	glBegin(GL_LINE_LOOP);
	  glVertex3f(pWinW,0,0);
	  glVertex3f(pWinW,pWinH,0);
	  glVertex3f(windw,pWinH,0);
	  glVertex3f(windw,0,0);
	glEnd();
	glEndList();
}

//-------------------------------------------------------------------
void drawCross(int pWinW, int pWinH, int x, int y)
{
	glNewList(5,GL_COMPILE);
	glLineWidth(1.0);
	glColor3f(1-paletteRGB[0],1-paletteRGB[1],1-paletteRGB[2]);
	glBegin(GL_LINES);
	glVertex3f(1,pWinH-y,0);
	glVertex3f(pWinW-1,pWinH-y,0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x,1,0);
	glVertex3f(x,pWinH-1,0);
	glEnd();
	glEndList();
}

//-------------------------------------------------------------------
void drawRGBline(int pWinW, int pWinH, float l)
{
	glNewList(6,GL_COMPILE);
	glLineWidth(1.0);
	if ((paletteRGB[0]==0.5) && (paletteRGB[1]==0.5) && (paletteRGB[2]==0.5)) glColor3f(0,0,0);
	else glColor3f(1-paletteRGB[0],1-paletteRGB[1],1-paletteRGB[2]);
	glBegin(GL_LINES);
	glVertex3f(pWinW,l*pWinH,0);
	glVertex3f(windw,l*pWinH,0);
	glEnd();
	glEndList();
}

//-------------------------------------------------------------------
void drawColorBox()
{
	float x1,x2,y1,y2,dx;

	glNewList(7,GL_COMPILE);
	dx = (frameW - windw)/6;
	x1 = windw + dx;
	x2 = frameW - dx;
	y1 = cHi - dx/2;
	y2 = cHi - 4*dx;
	glColor3d(paletteRGB[0],paletteRGB[1],paletteRGB[2]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	  glVertex3f(x1,y1,0);
	  glVertex3f(x1,y2,0);
	  glVertex3f(x2,y2,0);
	  glVertex3f(x2,y1,0);
	glEnd();
	glLineWidth(2);
	glColor3d(1,1,1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	  glVertex3f(x1,y1,0);
	  glVertex3f(x1,y2,0);
	  glVertex3f(x2,y2,0);
	  glVertex3f(x2,y1,0);
	glEnd();
	glEndList();
}

//-------------------------------------------------------------------
void drawOKbutton(int ID)
{
	float x1,y1,dx;
	glNewList(8,GL_COMPILE);
	dx = (frameW - windw)/6;
	x1 = windw + dx;
	y1 = dx/2;
	_drawButton(x1,y1,"OK",ID); 
	glEndList();
}

//-------------------------------------------------------------------
void showRGBvalue()
{
	char d_line[30];
	glNewList(9,GL_COMPILE);
	glColor3d(0,0,0);
	sprintf(d_line,"R: %i",(int)(paletteRGB[0]*100));
	glRasterPos2f(windw + (frameW - windw)/6, cHi - 5*(frameW - windw)/6);
	string_bitmap(GLUT_BITMAP_HELVETICA_12, d_line);

	sprintf(d_line,"G: %i",(int)(paletteRGB[1]*100));
	glRasterPos2f(windw + (frameW - windw)/6, cHi - 6*(frameW - windw)/6);
	string_bitmap(GLUT_BITMAP_HELVETICA_12, d_line);

	sprintf(d_line,"B: %i",(int)(paletteRGB[2]*100));
	glRasterPos2f(windw + (frameW - windw)/6, cHi - 7*(frameW - windw)/6);
	string_bitmap(GLUT_BITMAP_HELVETICA_12, d_line);
	glEndList();
}


//-------------------------------------------------------------------
int get_palette_ID()
{
	return(PalettedialogID);
}

//-------------------------------------------------------------------
void _MouseFunc(int button, int state, int x, int y)
{
	float x1,x2,y1,y2,dx;

	PalettedialogID = -1;
	if ((button==GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		dx = (frameW - windw)/6;
		x1 = windw + dx;
		x2 = frameW - dx;
		y1 = dx/2;
		y2 = 2*dx;
		motionID = 1;
		if ((x > 0) && ( x < cW) && (y > 0) && (y < cHi)){
			paletteHLS[0] = x * 360 / cW;
			paletteHLS[2] = (cHi - y) / cHi;
			drawRGB(cW,cHi,paletteHLS[0],paletteHLS[2]);
            drawCross(cW, cHi, x, y);
			HLS2RGB(paletteHLS[0],paletteHLS[1],paletteHLS[2]);
            drawColorBox();
			showRGBvalue();
			glutPostRedisplay();
		}
		else
		if ((x > cW) && ( x < windw) && (y > 0) && (y < cHi)){
			paletteHLS[1] = (cHi - y) / cHi;
			drawRGBline(cW,cHi,paletteHLS[1]);
			HLS2RGB(paletteHLS[0],paletteHLS[1],paletteHLS[2]);
            drawColorBox();
			showRGBvalue();
			glutPostRedisplay();
		}
		else
		if ((x > x1) && (x < x2) && (cHi - y > y1) && (cHi - y < y2)) {
			drawOKbutton(1);
			PalettedialogID = 0;
			glutDestroyWindow(windowID);
  		    glutSetWindow(ParentWindow);
			glutPostRedisplay();
 		}

	}
	else {
		motionID = 0;
		drawOKbutton(0);
		glutPostRedisplay();
	}
}


//-------------------------------------------------------------------
void MouseMotion(int x, int y)
{
	if (motionID==1){
		if ((x > 0) && ( x < cW) && (y > 0) && (y < cHi)){
			paletteHLS[0] = x * 360 / cW;
			paletteHLS[2] = (cHi - y) / cHi;
			drawRGB(cW,cHi,paletteHLS[0],paletteHLS[2]);
            drawCross(cW, cHi, x, y);
			HLS2RGB(paletteHLS[0],paletteHLS[1],paletteHLS[2]);
            drawColorBox();
			showRGBvalue();
		}
		if ((x > cW) && ( x < windw) && (y > 0) && (y < cHi)){
			paletteHLS[1] = (cHi - y) / cHi;
			drawRGBline(cW,cHi,paletteHLS[1]);
			HLS2RGB(paletteHLS[0],paletteHLS[1],paletteHLS[2]);
            drawColorBox();
			showRGBvalue();
		}
		glutPostRedisplay();
	}
}

//-------------------------------------------------------------------
void _reshape(int wid, int ht)
{
    glutReshapeWindow( frameW, windh );
	glutPostRedisplay();
}

//-------------------------------------------------------------------
void _redraw(void)
{
	int i;
	glClearColor(0.7f,0.7f,0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-(float)frameW/2,-(float)windh/2,0);
	for (i=0; i<9; i++) glCallList(i+1);
	glPopMatrix();
	glFinish();
	glutSwapBuffers();	
}


//-------------------------------------------------------------------
void InitPaletteDialog(float RGB[3])
{
	int i;
	PalettedialogID = -1;
	for(i=0; i<3; i++) paletteRGB[i] = RGB[i];
	RGB2HLS(paletteRGB[0],paletteRGB[1],paletteRGB[2]);
	windw = DEFAULT_WIND_WIDTH;
	windh = DEFAULT_WIND_HEIGHT;
}

//-------------------------------------------------------------------
void OpenPaletteWindow(int winID, int PosX, int PosY)
{
	PalettedialogID = -1;
	frameW = 1.5*windw;
	cW = (float)windh;
	cHi = (float)windh;
	ParentWindow = winID;
//............................................................................
	glutInitWindowSize(frameW, windh);
	glutInitWindowPosition(PosX,PosY);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	windowID = glutCreateWindow("Palette");
	glutReshapeFunc(_reshape);
	glutDisplayFunc(_redraw);
	glutMouseFunc(_MouseFunc);
	glutMotionFunc(MouseMotion);
//............................................................................
	glViewport(0, 0, frameW, windh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(float)frameW/2, (float)frameW/2, -(float)windh/2, (float)windh/2, -1, 7);
	glMatrixMode(GL_MODELVIEW);
//............................................................................
	drawHLS(cHi, cHi);
	drawRGB(cHi, cHi, paletteHLS[0], paletteHLS[2]);
	drawHLScell(cHi, cHi);
    drawRGBcell(cHi, cHi);
	drawCross(cW,cHi,paletteHLS[0]*cW/360,cHi-cHi*paletteHLS[2]);
    drawRGBline(cW,cHi, paletteHLS[1]);
	HLS2RGB(paletteHLS[0],paletteHLS[1],paletteHLS[2]);
    drawColorBox();
    drawOKbutton(0);
    showRGBvalue();
}


//-------------------------------------------------------------------
void GetPaletteRGB(float RGB[3])
{
	int i;
	for(i=0; i<3; i++) RGB[i] = paletteRGB[i];
}
