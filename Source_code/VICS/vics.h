#ifndef _VICS_H_
#define _VICS_H_

#define INIFILE		"elements.ini"         //"iniparam"
#define SPGRA       "spgra.dat"            //Spgra"
#define SPGRO       "spgro.dat"            //"Spgro"
#define DEFPREF     "preferences_VICS.ini"
#define TEMPLATE    "template.ins"

//-----------------------------------------------------
//  RIETAN *.INS FILE DEFINITIONS
//-----------------------------------------------------
#define PHNAME	"PHNAME"               //--- Phase name
#define VNS		"VNS"                  //--- Sp.Gr Number and Setting
#define CELQ1	"CELQ"                 //--- Unit Cell Parameters
#define CELQ2	"CELLQ"                //--- Unit Cell Parameters

//-----------------------------------------------------
//  MAIN DEFINITIONS
//-----------------------------------------------------
#define PI						 3.1415927 
#define SPI8					78.9658352 
#define SPI2					19.7392088 
#define RAD						 0.0174533 


#define MAX_SYMM_POS			201        //-- Max. Symmetry operations
#define MAX_NUMBER_OF_GROUPS	400
#define BVS_PARAM				37
//-----------------------------------------------------
#define MAX_ATOMS_SPEC		99		   //--- Max. Number of Atomic Specifications
#define MAX_NUM_OF_ATOMS	9000       //--- Max. Number of Atoms 
#define MAX_ATOMS_IN_ASYMM	1200       //--- Max. Number of Atoms in Assim. Units
#define MAX_POS_FOR_ATOM    192        //--- Max. Number of Atomic Positions for 1 Atom
#define MAX_BONDS_FOR_ATOM	25         //--- Max. Number of Bonds for 1 Atom
#define MAX_POLY_FOR_ATOM	8          //--- Max. Number of Polyhedra for 1 Atom
//-----------------------------------------------------
#define MAX_BONDS_SPEC		21	       //--- Max. Number of Bonds Specifications
#define MAX_NUM_OF_BONDS	12000      //--- Max. Number of Bonds
//-----------------------------------------------------
#define MAX_POLYHEDRA		2500       //--- Max. Number of Polyhedras
#define MAX_V			128            //--- Max. Number of Vert. for 1 Poly
#define MAX_PF			50             //--- Max. Number of Faces for 1 Poly
#define MAX_PFV			100              //--- Max. Number of Vert. for 1 Face
//-----------------------------------------------------
#define MAX_NUM_OF_PHASES	3		   //--- Max. Number of Phases (for *.CIF *.INS files)
//-----------------------------------------------------
#define MAX_PICK_OBJ	100            //--- Max. Number of Selected Objects
#define MAX_DELETED_OBJ	500            //--- Max. Number of Deleted Objects
//-----------------------------------------------------
#define MAX_BOND_DIST    35









//-----------------------------------------------------
//  3D-vector
//-----------------------------------------------------
typedef struct vectordtag
{
	double x;
	double y;
	double z;
} XYZ;


typedef struct vectorftag
{
	float x;
	float y;
	float z;
} VECTOR3D;



//-----------------------------------------------------
//  Unit Cell Symmetry
//-----------------------------------------------------
typedef struct celltag
{
  char lType;                 //--- Lattice Type
  char SpGr[20];              //--- Space Group Name
  int SpGrN;                  //--- Space Group Number
  int Setting;                //--- Setting
} CELL;

//-----------------------------------------------------
//  Symmetry operations (x,y,z....)
//-----------------------------------------------------
typedef struct symmopertag
{
	char symm1[10];
	char symm2[10];
	char symm3[10];
} SYMMOPER;

//-----------------------------------------------------
//  Symmetry 
//-----------------------------------------------------
typedef struct symmpostag
{
	float t[3];               //--- translations
	int a[3][3];              //--- rotation matrix
} SYMMPOS;

//-----------------------------------------------------
//  Atoms Properties
//-----------------------------------------------------
typedef struct mainparamtag
{
  int atomN;                  //--- Atomic Number
  char atsymb[3];             //--- Atomic Symbol
  float Ra;                   //--- Radius
  float Rv;                   //--- Radius
  float Ri;                   //--- Radius
  float RGB[3];               //--- Color (RGB)
} MAINPARAM;

//-----------------------------------------------------
//  Atoms Specifications
//-----------------------------------------------------
typedef struct atomspectag
{
	char symb[3];             //--- Atomic Symbol
	float R;                  //--- Atomic Radius
	float color[3];           //--- Atomic Color (RGB)
} ATOMSPEC;


//-----------------------------------------------------
//  Bonds Specifications
//-----------------------------------------------------
typedef struct bondsinitag
{
	int search_mode;          //--- Search mode
	int show_poly_mode;       //--- Show Polyhedra mode
	int boundary_mode;        //--- Show Polyhedra mode
	char A1[3];               //--- Atomic Symbol for Atom 1
	float d;                  //--- Max. Bonds distance
	char A2[3];               //--- Atomic Symbol for Atom 2
} BONDSINI;

//-----------------------------------------------------
//  Structure Parameters
//-----------------------------------------------------
typedef struct atomtypetag
{
  char  name[7];              //--- Atomic Label
  float g;                    //--- Occupances
  float x;                    //--- Fractional coordinates (x, y, z)
  float y;                    // 
  float z;                    //
  float ex;                   //--- ESD
  float ey;                   //
  float ez;                   //
  float R;                    //--- Radius
  float color[3];             //--- Color (RGB)
  char  simb[3];              //--- Atomic Symbol
  int CN;                     //--- Coordination Number
  float B[6];                 //--- Anizotropic Thermal Parameter
  float Biso;                 //--- Izotropic Thermal Parameter
} ATOMTYPE;

//-----------------------------------------------------
//  Thermal Ellipsoids Parameters
//-----------------------------------------------------
typedef struct ellipstypetag
{
	float axis[3];           //--- main axis of thermal ellipsoids
	float axis_dir[3][3];    //--- rotation matrix
} ELLIPS;

//-----------------------------------------------------
//  Atoms
//-----------------------------------------------------
typedef struct atomtag
{
  int Num;
  int Index;
  float x;
  float y;
  float z;
  char show;
  int symnum;                  //--- Symmetry Operations Number
  int T[3];                     //--- Extra translations
  short bondNum;
  short b_list[MAX_BONDS_FOR_ATOM];
  short bondatom;
  short polyNum;
  short p_list[MAX_POLY_FOR_ATOM];
  int   poly_show;             //--- Polyhedra Show Control
  short poly_mode_show;        //--- 
  short show_spin;
  float spin_dir[3];
} ATOM;

//-----------------------------------------------------
//  Bonds  Parameters
//-----------------------------------------------------
typedef struct atombondstag
{
  int Atom1;                  //--- ID number for Atom1
  int Atom2;                  //--- ID number for Atom2
  double dist;                //--- Bonds distance (Atom1 - Atom2)
  double FI;                  //--- Bond direction (from Atom1 to Atom2)
  char show;                  //--- Show bond ID
  char hydr;
} ATOMBONDS;

//-----------------------------------------------------
//  Unit Cell
//-----------------------------------------------------
typedef struct cellparamtag
{
  char lType;                 //--- Lattice Type
  float a[6];                 //--- Unit Cell Parameters
  float ea[6];                //--- ESD
  float ortho_a[3];           //--- Orthogonal Parameters
  float max_P; 
  char SpGr[20];              //--- Space Group Name
  int SpGrN;                  //--- Space Group Number
  int Setting;                //--- Setting
} CELLPARAM;



//-----------------------------------------------------
//  Polyhedra
//-----------------------------------------------------
typedef struct polyfacetag
{
  int vN;
  float fVector[3];
  float v[MAX_PFV][3]; 
} POLYFACE;

typedef struct polygontag
{
  int atomNum;
  int fN;
  char show;
  POLYFACE face[MAX_PF];
  float center[3];
  float color[3];
} POLYGON;

//====================================================================
//  if (Coordination Number > 0)
// poly_show       Show Polyhedra    Show Atoms         Show Bonds
// -------------------------------------------------------------------
//    1                 Yes          poly_mode_show    poly_mode_show 
//    0                 No                 No               No
//====================================================================

//-----------------------------------------------------
//  Phases Specification
//-----------------------------------------------------
typedef struct phaseinitag
{
	char name[80];
} PHASEINI;

//-----------------------------------------------------
//  Deleted Objects Specification
//-----------------------------------------------------
typedef struct delobjecttag
{
	int nAtom;
	int del_Atom[MAX_DELETED_OBJ];
	int nBonds;
	int del_Bonds[MAX_DELETED_OBJ];
	int nPoly;
	int del_Poly[MAX_DELETED_OBJ];
} DELOBJECT;



#endif