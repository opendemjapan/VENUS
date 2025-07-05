#ifndef _VEND_H_
#define _VEND_H_

#define PALFILE				"palette.ini"            //--- Palette file name
#define PREFFILE			"preferences_VEND.ini"  //--- Preferency file name
#define PI					3.14159
//-----------------------------------------------------
#define MAX_PIX				260         //--- Max. Number of Pixels : GRID[MAX_PIX][MAX_PIX][MAX_PIX]
//#define MAX_NASYM			900000      //--- Max. Number of Pixels in Asymmetric Unit
#define MAX_SYMM_OP			48          //--- Max. Number of Symmetry operations
//#define MAXVERT				1800000     //--- Max. Number of Vertex
#define MAXVERT				2500000     //--- Max. Number of Vertex
//#define MAXPOLYGON			80000      //--- Max. Number of Polygons
#define MAXPOLYGON			100000      //--- Max. Number of Polygons
//-----------------------------------------------------
#define MAX_SYMM_POS		201			//--- Max. Symmetry operations
#define MAX_ATOMS_IN_ASYMM	101			//--- Max. Number of Atoms in Assim. Units
#define MAX_ATOMS_SPEC		99			//--- Max. Number of Atomic Specifications
#define MAX_BONDS_SPEC		21			//--- Max. Number of Bonds Specifications
#define MAX_BONDS_FOR_ATOM	12			//--- Max. Number of Bonds for 1 Atom
#define MAX_NUM_OF_ATOMS	9000		//--- Max. Number of Atoms 
#define MAX_NUM_OF_BONDS	12000		//--- Max. Number of Bonds

//*******************************************************************//
//                         3D - SHAPE & SLICES                       //
//*******************************************************************//

//-----------------------------------------------------
//  Symmetry Matrix
//-----------------------------------------------------
typedef struct {
  int t[3];
  int a[3][3];
} SYMMPOS;

typedef struct {
	double x;
	double y;
	double z;
} XYZ1;

typedef struct {
	float x;
	float y;
	float z;
} XYZ;

typedef struct {
	XYZ p[3];
} TRIANGLE;

typedef struct {
	XYZ p[8];
	float val[8];
} GRIDCELL;

typedef struct {
	XYZ p;
	float val;
} GRIDPOINT;

typedef struct {
	short Index[9];
	XYZ p[9];
} SLICES;

typedef struct {
	short nV;
	SLICES sl;
} MSLICES;

//*******************************************************************//
//                         STRUCTURE MODEL                           //
//*******************************************************************//
//-----------------------------------------------------
//  Symmetry 
//-----------------------------------------------------
typedef struct msymmpostag
{
	float t[3];               //--- translations
	int a[3][3];              //--- rotation matrix
} MSYMMPOS;

//-----------------------------------------------------
//  Unit Cell
//-----------------------------------------------------
typedef struct cellparamtag
{
  int SpGrN;
  int Setting;
  float a[6];                 //--- Unit Cell Parameters
  float ortho_a[3];           //--- Orthogonal Parameters
  float max_P; 
} MCELLPARAM;

//-----------------------------------------------------
//  Structure Parameters
//-----------------------------------------------------
typedef struct atomtypetag
{
  char  name[7];              //--- Atomic Label
  float x;                    //--- Fractional coordinates (x, y, z)
  float y;                    // 
  float z;                    //
  float R;                    //--- Radius
  float color[3];             //--- Color (RGB)
  char  simb[3];              //--- Atomic Symbol
} ATOMTYPE;

//-----------------------------------------------------
//  Atoms Specifications
//-----------------------------------------------------
typedef struct atomspectag
{
	float color[3];           //--- Atomic Color (RGB)
	float R;                  //--- Atomic Radius
	char symb[3];             //--- Atomic Symbol
} ATOMSPEC;

//-----------------------------------------------------
//  Bonds Specifications
//-----------------------------------------------------
typedef struct bondsinitag
{
	int search_mode;          //--- Search mode
	char A1[2];               //--- Atomic Symbol for Atom 1
	float d;                  //--- Max. Bonds distance
	char A2[2];               //--- Atomic Symbol for Atom 2
} BONDSINI;

//-----------------------------------------------------
//  Atoms
//-----------------------------------------------------
typedef struct atomtag
{
  int Num;
  float x;
  float y;
  float z;
  float xo;
  float yo;
  float zo;
  short bondNum;
  short b_list[MAX_BONDS_FOR_ATOM];
  short bondatom;
  char show;
} ATOM;

//-----------------------------------------------------
//  Bonds
//-----------------------------------------------------
typedef struct atombondstag
{
  int Atom1;
  int Atom2;
  double dist;
  double FI;
} ATOMBONDS;

#endif
