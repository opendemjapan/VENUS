#include "vend.h"

MSYMMPOS msPos[MAX_SYMM_POS];
int mnPos;                  //--- Number of symmetry operations

MCELLPARAM mcCell;

ATOMTYPE lAtom[MAX_ATOMS_IN_ASYMM];
int nAtom;                  //--- Number of Atoms in Assymetric Unit

ATOMSPEC AtomT[MAX_ATOMS_SPEC];
int tAtom;                  //--- Number of Atomic Specifications

BONDSINI bini[MAX_BONDS_SPEC];
int bond_sNum;              //--- Number of Bonds Specifications

ATOM m[MAX_NUM_OF_ATOMS];
int nXYZ;

ATOMBONDS bAtom[MAX_NUM_OF_BONDS];
int nBonds;

float matrix[3][3];
float bound[6];

int atoms_stack;              //--- atoms stacks  
int atoms_slice;              //--- atoms slices
int bonds_stack;             //--- bonds stacks  
int bonds_slice;             //--- bonds slices
float BNCL[3];               //--- Bond Color (RGB)
float bRadii[2];                //--- Bonds Radius
int radii_type;
int model_type;
int bonds_type;


char mod_fName[256];
int mod_format;

void set_initital_model_parameters();
int readModelfile(char *fname);
int make_model_atom_pos();
void make_orto_matrix();
void make_orto_model_atom(int NPX[3]);
int searchBonds(float maxPar);
void get_atom_R_and_colors();

