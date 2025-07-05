#include "vend.h"
  
MSLICES slice[6][MAXPOLYGON],plane[3][MAXPOLYGON];

FILE *stream;

float Normal[3];             //--- (hkl) Plane Normal
float Origin[3];             //--- (hkl) Plane Origin
float hkl_o_axis[3][3];      //--- (hkl) Plane Orthogonal Asis
float hkl_ortho[3];

float uCello[8][3];          //---  Unit Cell Outline
float uTranso[3][3];         //---  Unit Cell Translations
float ISO[6][MAXVERT];
char sign[MAXVERT];


int shape(float cell[6], float rho[MAX_PIX][MAX_PIX][MAX_PIX],float origin[3], float conlevel, int maxNP[3],
	      int PX0, int PY0, int PZ0, int NPX, int NPY, int NPZ, int ID);

void set_Ortho_Cell(int PX0, int PY0, int PZ0, int NPX, int NPY, int NPZ);
void set_Ortho_Translations(int NPX, int NPY, int NPZ);

int section(int ID, float rho[MAX_PIX][MAX_PIX][MAX_PIX], float origin[3], float conlevel, float intlevel, int maxNP[3],
		     int PX0, int PY0, int PZ0, int NPX, int NPY, int NPZ,float Dmin, float Dmax, int type_ID);

int make_shortcut_plane(int IP, int ID, float rho[MAX_PIX][MAX_PIX][MAX_PIX], float origin[3], 
						float conlevel, float intlevel, float position,
						int maxNP[3], float Dmin, float Dmax);

int make_plane(int IP, short hkl[3], float rho[MAX_PIX][MAX_PIX][MAX_PIX], 
			           float conlevel, float intlevel, 
					   float position, int maxNP[3], 
					   float Dmin, float Dmax, float cell[6]);
