/* 
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 *
 *
 *  Usage:
 *  
 *  o  call tbInit() in before any other tb call
 *  o  call tbReshape() from the reshape callback
 *  o  call tbMatrix() to get the trackball matrix rotation
 *  o  call tbStartMotion() to begin trackball movememt
 *  o  call tbStopMotion() to stop trackball movememt
 *  o  call tbMotion() from the motion callback
 *  o  call tbAnimate(GL_TRUE) if you want the trackball to continue 
 *     spinning after the mouse button has been released
 *  o  call tbAnimate(GL_FALSE) if you want the trackball to stop 
 *     spinning after the mouse button has been released
 *
 *  Typical setup:
 *
 *
    void
    init(void)
    {
      tbInit(GLUT_MIDDLE_BUTTON);
      tbAnimate(GL_TRUE);
      . . .
    }

    void
    reshape(int width, int height)
    {
      tbReshape(width, height);
      . . .
    }

    void
    display(void)
    {
      glPushMatrix();

      tbMatrix();
      . . . draw the scene . . .

      glPopMatrix();
    }

    void
    mouse(int button, int state, int x, int y)
    {
      tbMouse(button, state, x, y);
      . . .
    }

    void
    motion(int x, int y)
    {
      tbMotion(x, y);
      . . .
    }

    int
    main(int argc, char** argv)
    {
      . . .
      init();
      glutReshapeFunc(reshape);
      glutDisplayFunc(display);
      glutMouseFunc(mouse);
      glutMotionFunc(motion);
      . . .
    }
 *
 * */
#include <GL/glut.h>

/* functions */
void tbInit(GLuint button, float rotation[16]);
void tbUpdate(int ID, GLuint button, float rotation[16]);

void tbMatrix();
void tbReshape(int width, int height);
//void tbMouse(int button, int state, int x, int y, int ID);
int tbMouse(int button, int state, int x, int y, int ID, int Type);
void tbMotion(int x, int y, int ID);
void tbAnimate(GLboolean animate);

void _tbStopMotion(int button, unsigned time);

void getOrientation(float rotMatrix[16]);

void tbMatrixS();
void tbStepW(int step_active, int step_mode, float angle, float rot_axis[3]);
void tbClickMotion(int x, int y, int ID, float angle);
