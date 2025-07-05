/*
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 */


/* includes */
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <GL/glut.h>
//#include "trackball.h"


/* globals */
static GLuint    tb_lasttime;
static GLfloat   tb_lastposition[3];

static GLfloat   tb_angle = 0.0;
static GLfloat   tb_angleS = 0.0;
static GLfloat   tb_axis[3];
static GLfloat   tb_axisS[3];
static GLfloat   tb_transform[4][4], rot_Matrix[4][4];

static GLuint    tb_width;
static GLuint    tb_height;

static GLint     tb_button = -1;
static GLboolean tb_tracking = GL_FALSE;
static GLboolean tb_animate = GL_TRUE;

int lastX,lastY,newX,newY;
int rotation_ID;

int tb_step_active = 0;
int tb_step_mode = 0;


/* functions */
static void
_tbPointToVector(int x, int y, int width, int height, float v[3], int ID )
{
  float d, a;

  /* project x, y onto a hemi-sphere centered within width, height. */
  switch(ID) {
	  case (3):
	  case (0): { 
		  v[0] = (2.0 * x - width) / width;
		  v[1] = (height - 2.0 * y) / height;
		  break;
				}
	  case (1): {
		  v[0] = 0;
		  v[1] = (height - 2.0 * y) / height;
		  break;
				}
	  case (2): {
		  v[0] = (2.0 * x - width) / width;
		  v[1] = 0;
		  break;
				}

  }
  d = sqrt(v[0] * v[0] + v[1] * v[1]);
  if (ID == 3){d = 1.0;}
  v[2] = cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0));
  a = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
}

static void
_tbAnimate(void)
{
  glutPostRedisplay();
}

void
_tbStartMotion(int x, int y, int button, int time, int ID)
{
  assert(tb_button != -1);

  tb_tracking = GL_TRUE;
  tb_lasttime = time;
  _tbPointToVector(x, y, tb_width, tb_height, tb_lastposition, ID);
}


void
_tbStopMotion(int button, unsigned time)
{
  assert(tb_button != -1);

  tb_tracking = GL_FALSE;

  if (time == tb_lasttime && tb_animate) {
    glutIdleFunc(_tbAnimate);
  } else {
    tb_angle = 0.0;
    if (tb_animate)
      glutIdleFunc(0);
  }
}

void
_tbClickMotion(int button, unsigned time)
{
  assert(tb_button != -1);

  tb_tracking = GL_TRUE;
}



void
tbAnimate(GLboolean animate)
{
  tb_animate = animate;
}

void
tbInit(GLuint button, float rotation[16])
{
  tb_button = button;
  tb_angle = 0.0;
  lastX = 0;
  lastY = 0;
  /* put the identity in the trackball transform */
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixf(rotation);
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
  glPopMatrix();

}

void
tbUpdate(int ID, GLuint button, float rotation[16])
{
	if (ID == 0) tbInit(button, rotation);
	else {
		tb_button = button;
		tb_angle = 0.0;
		glPushMatrix();
		glMultMatrixf(rotation);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
		glPopMatrix();
	}
}

void
tbMatrix()
{
	float tba;
	
	assert(tb_button != -1);
	if ((rotation_ID == 0) && (newX == lastX) && (newY == lastY)) tba = 0;
	else tba = tb_angle;

	glPushMatrix();
	glLoadIdentity();
	glRotatef(tba, tb_axis[0], tb_axis[1], tb_axis[2]);
	glMultMatrixf((GLfloat *)tb_transform);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)rot_Matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
	glMultMatrixf((GLfloat *)tb_transform);
	
	lastX = newX;
	lastY = newY;
}

void
tbReshape(int width, int height)
{
  assert(tb_button != -1);

  tb_width  = width;
  tb_height = height;
}

int
tbMouse(int button, int state, int x, int y, int ID, int Type)
{
	int control = 0;
	assert(tb_button != -1);
	newX = x;
	newY = y;
	lastX = newX;
	lastY = newY;
	if (state == GLUT_DOWN && button == tb_button) 
		_tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME), ID);
	else if (state == GLUT_UP && button == tb_button) {
		if (Type < 2) {
			_tbStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
			control = 1;
		}
		else _tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME), ID);
	}
	rotation_ID = Type;
	return(control);
}

void
tbMotion(int x, int y, int ID)
{
  GLfloat current_position[3], dx, dy, dz;
  newX = x;
  newY = y;

  assert(tb_button != -1);
  if (tb_tracking == GL_FALSE)
    return;

  _tbPointToVector(x, y, tb_width, tb_height, current_position,ID);

  /* calculate the angle to rotate by (directly proportional to the
     length of the mouse movement */
	  dx = current_position[0] - tb_lastposition[0];
	  dy = current_position[1] - tb_lastposition[1];
	  dz = current_position[2] - tb_lastposition[2];
	  tb_angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);

  /* calculate the axis of rotation (cross product) */
  switch(ID)
  {
  case(0):
  case(1):
  case(2):
  case(3):{
	  tb_axis[0] = tb_lastposition[1] * current_position[2] - 
		           tb_lastposition[2] * current_position[1];
	  tb_axis[1] = tb_lastposition[2] * current_position[0] - 
		           tb_lastposition[0] * current_position[2];
	  tb_axis[2] = tb_lastposition[0] * current_position[1] - 
		           tb_lastposition[1] * current_position[0];
	  break;
		  }
  }

  /* reset for next time */
  tb_lasttime = glutGet(GLUT_ELAPSED_TIME);
  tb_lastposition[0] = current_position[0];
  tb_lastposition[1] = current_position[1];
  tb_lastposition[2] = current_position[2];

  /* remember to draw new position */
  glutPostRedisplay();
}



void
tbClickMotion(int x, int y, int ID, float angle)
{
	GLfloat current_position[3];

  _tbPointToVector(x, y, tb_width, tb_height, current_position,ID);
  tb_angle = angle;
  tb_axis[0] = -current_position[1];
  tb_axis[1] =  current_position[0];
  tb_axis[2] =  0;
  glutPostRedisplay();

}


void getOrientation(float rotMatrix[16])
{
	int i,j,k;

	k = 0;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			rotMatrix[k] = tb_transform[i][j];
			k++;
		}
	}
}

void 
tbStepW(int step_active, int step_mode, float angle, float rot_axis[3])
{
	int i;
	tb_angleS = angle;
	tb_step_active = step_active;
	tb_step_mode = step_mode;
	for(i=0; i<3; i++) tb_axisS[i] = rot_axis[i];
	glPushMatrix();
	glLoadIdentity();
	glRotatef(tb_angleS, tb_axisS[0], tb_axisS[1], tb_axisS[2]);
	glMultMatrixf((GLfloat *)tb_transform);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)rot_Matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
	glMultMatrixf((GLfloat *)tb_transform);
}



