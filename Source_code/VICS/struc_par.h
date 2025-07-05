#include "vics.h"

char progvar[256];
//-------------------   Windows Positions, Size (Graphic Window)
int save_pos_ID;
int menu_x,menu_y;                               //--- Menu Dialog Window Position;
int tools_x,tools_y;                             //--- Tools Dialog Window Position;
int graph_x,graph_y;                             //--- Graphic Window Position;
int graph_w,graph_h;                             //--- Graphic Window Size

//-------------------   DIALOG Window Positions
// dwp[0][ID] - x position; dwp[1][ID] - y position
// ID:
// 0 - about dialog
// 1 - atoms dialog
// 2 - hkl dialog
// 3 - new_edit dialog
// 4 - orient dialog
// 5 - ortho_origin dialog
// 6 - poly dialog
// 7 - prefer dialog
// 8 - range dialog
// 9 - rendering dialog
//10 - sbonds dialog
//11 - spin dialog
//12 - stepwise dialog
//13 - unit cell dialog
//14 - bonds dialog
int dwp[2][15],cdwp[2][15];
int save_dpos_ID;




//-----------------------------------------------------
//  Bonds Parameters
//-----------------------------------------------------
ATOMBONDS bAtom[MAX_NUM_OF_BONDS];
int nBonds;
int mol_bond;
int hydrogen_bonds_id;
int hbond_line_type;
//-----------------------------------------------------
//  Symmetry operations (x,y,z....)
//-----------------------------------------------------
SYMMOPER sOperation[MAX_SYMM_POS];

//-----------------------------------------------------
//  Symmetry 
//-----------------------------------------------------
SYMMPOS sPos[MAX_SYMM_POS];
int nPos;                   //--- Number of symmetry operations
int NumSpGr;                //--- Space Group Number
int Setting;                //--- Space Group Setting

//-----------------------------------------------------
//  Atoms Properties
//-----------------------------------------------------
MAINPARAM dPar[96];
int radii_type;

//-----------------------------------------------------
//  Unit Cell
//-----------------------------------------------------
CELLPARAM cCell;
double ortho_M[3][3];        //--- Orthogonal MAtrix
float uCell[8][3];           //--- Orthogonal XYZ for Unit Cell Outline
float uAxis[3][3];           //--- Orthogonal XYZ for "Orientation Ball"

//-----------------------------------------------------
//  Structure Parameters
//-----------------------------------------------------
ATOMTYPE lAtom[MAX_ATOMS_IN_ASYMM];
int nAtom;                  //--- Number of Atoms in Assymetric Unit
int Btype;                  //--- Type of Anizotropic Thermal Parameter (U / beta)
int use_aniso;
int use_iso;

//-----------------------------------------------------
//  Thermal Ellipsoids Parameters
//-----------------------------------------------------
ELLIPS ellAtom[MAX_ATOMS_IN_ASYMM];
float V[3];
float VV[3][3];
float termMatrix[16];

//-----------------------------------------------------
//  Atoms Specifications
//-----------------------------------------------------
ATOMSPEC AtomT[MAX_ATOMS_SPEC];
int tAtom;                  //--- Number of Atomic Specifications


//-----------------------------------------------------
//  Bonds Specifications
//-----------------------------------------------------
BONDSINI bini[MAX_BONDS_SPEC];
int bond_sNum;              //--- Number of Bonds Specifications
//-----------------------------------------------------
//  Atoms
//-----------------------------------------------------
ATOM m[MAX_NUM_OF_ATOMS];
int nXYZ;
int num_Spin;
//-----------------------------------------------------
//  Polyhedra
//-----------------------------------------------------
POLYGON p, tp[MAX_POLYHEDRA];
int nPoly;
int poly_list[MAX_POLYHEDRA];
int n[MAX_PFV];
int n_list;
int face_list[MAX_PF][MAX_PFV]; 


//-----------------------------------------------------
//  Phases Specification
//-----------------------------------------------------
PHASEINI phases[MAX_NUM_OF_PHASES];
int nPhase;


//-----------------------------------------------------
//  Deleted Objects Specification
//-----------------------------------------------------
  DELOBJECT del_Object;


//-----------------------------------------------------
//  Dialogs Control
//-----------------------------------------------------
//-----------------------------------------------------
//  Additional
//-----------------------------------------------------
int fileID;                   //--- Input Data File Format
int sfileID;                  //--- Save Data File Format
char FileName[256];            //--- Input Data File Name
char *arg_fname;
char outFileName[256];         //--- Output File Name
int e_fileID;
char e_FileName[256];            //--- Input Data File Name
char t_FileName[256];            //--- Input Data File Name

char Title[70];               //---  Title of Structure
float BKCL[3];                //--- Background Color (RGB)  
float range[6];               //--- Strucute Boundaries
int MODL;                     //--- Model Type
int dot_surf_ID;
int HKLplane[3];              //--- (HKL) Plane 
float origin[3];              //--- Origin
int total_select;             //--- number of objects
int projection_type;          //--- 0 - Perspective; 1 - Parallel

//---------------------------------------------------------------------------//
float perspective;            //--- Perspective projection level
float scale;                  //--- Scale Factor
float obj_pos[3];             //--- Opject Position
float rotation[16];           //--- Rotation Matrix
float c_rotation[16];
float u_rotation[16];

//---------------------------------------------------------------------------//

//-----------------------------------------------------
//  Rendering ( General Properties )
//-----------------------------------------------------
float m_atom_ambient[4];        //--- Atom Ambient
float m_atom_diffuse[4];        //--- Atom Diffuse
float m_atom_specular[4];       //--- Atom Specular
float m_atom_shininess;         //--- Atom Shininess

float m_bond_ambient[4];        //--- Bond Ambient
float m_bond_diffuse[4];        //--- Bond Diffuse
float m_bond_specular[4];       //--- Bond Specular
float m_bond_shininess;         //--- Bond Shininess

float m_poly_ambient[4];        //--- Poly Ambient
float m_poly_diffuse[4];        //--- Poly Diffuse
float m_poly_specular[4];       //--- Poly Specular
float m_poly_shininess;         //--- Poly Shininess

float m_plane_ambient[4];       //--- (hkl) Plane Ambient
float m_plane_diffuse[4];       //--- (hkl) Plane Diffuse
float m_plane_specular[4];      //--- (hkl) Plane Specular
float m_plane_shininess;        //--- (hkl) Plane Shininess

float l_direction[16];          //--- Lights Direction
float l_dir[3];
float l_position[4];            //--- Lights Position
float l_ambient[4];             //--- Lights Ambient
float l_diffuse[4];             //--- Lights Diffuse

//-----------------------------------------------------
//  Atoms Properties
//-----------------------------------------------------
int atom_type;
int atoms_stack;              //--- atoms stacks  
int atoms_slice;              //--- atoms slices
int out_atoms_stack;         //--- atoms stacks for output graphics
int out_atoms_slice;         //--- atoms slices for output graphics
int TEOL;                    //--- Show Thermal Ellipsoids outline
int Probability;           //--- Probability for thermal ellipsoids

//-----------------------------------------------------
//  Bonds Properties
//-----------------------------------------------------
int bonds_type;              //--- bonds mode
int bonds_stack;             //--- bonds stacks  
int bonds_slice;             //--- bonds slices
int out_bonds_stack;         //--- bonds stacks for output graphics
int out_bonds_slice;         //--- bonds slices for output graphics
float bonds_line_width;      //--- bonds line width (for bond_mode = 2,3)
float BNCL[3];               //--- Bond Color (RGB)
float bRadii[2];             //--- Bonds Radius (0 - for "B&S"; 1 - for "Stick")

//-----------------------------------------------------
//  Polyhedra Properties
//-----------------------------------------------------
int poly_type;               //--- Polyhedra Type
float poly_trans;            //--- Polyhedra Opacity
int poly_outline;            //--- Show Polyhedra Outline (1 - "Yes" ; 0 - "No") 
float poly_outline_width;    //--- Polyhedra Outline Width
float poly_line_color[3];    //--- Polyhedra Outline Colors (RGB)

//-----------------------------------------------------
//  Unit Cell Outline Properties
//-----------------------------------------------------
int SHCL;                     //--- Show unit Cell outline
float ucell_width;           //--- Unit Cell Outline Width
int ucell_type;
float ucell_color[3];        //--- Unit Cell Outline Colors (RGB)

//-----------------------------------------------------
//  Compass Properties
//-----------------------------------------------------
int show_axis;               //--- Show Compass ID
int def_show_axis;
int show_axis_label;         //--- Show Compass ID
int def_show_axis_label;

//-----------------------------------------------------
//  Atom Vector Properties
//-----------------------------------------------------
float spin_H;
float spin_RGB[3];

//-----------------------------------------------------
//  (hkl) Plane Properties
//-----------------------------------------------------
int show_hkl;                //--- Show (hkl) Plane ID
int origin_hkl;              //--- (hkl) Plane Origin (Unit Cell - 0 / Boundaries - 1)
float shift_hkl[3];          //--- (hkl) Plane Shift from Center (for origin_hkl = 1)
int hkl[3];                  //--- h,k,l
float o_range[8][3];         //--- Boundaries orthogonal coordinates
float o_center[3];           //--- Center of boundaries
float hkl_o_axis[3][3];      //--- Orthogonal a/h; b/k; c/l

float hkl_rgb[3];            //--- (hkl) Plane Colors (RGB) 
float hkl_trans;             //--- (hkl) Plane Opacity
int show_hkl_inv;            //--- Show (-h -k -l) Plane ID
int show_hkl_out;            //--- Show (hkl) Plane Outline
float hkl_out_width;         //--- (hkl) Plane Outline Width
int cPNum;
float cPlane1[9][3],cPlane2[9][3];
double Dhkl;
float Normal[3];
short move_hkl;                //--- (hkl) Plane Shift ID (0 - No Shift ; 1 - Shift)

//---------------------------------------------------------------------------//
//                           Image Control                                   //
//---------------------------------------------------------------------------//
unsigned char *image,*image1;
char img_fName[256];
int img_format;
int img_scale;
int img_smoothing;
int run_img_export;

//-----------------------------------------------------
//  Crystal Orientation
//-----------------------------------------------------
double VA[3],VF;

//-----------------------------------------------------
//  Stepwise Rotation Control
//-----------------------------------------------------
int stepwise_active;
float stepw_angle;
int stepw_mode;
int stepw_rot_axis;
float stepw_rot[3];

//-----------------------------------------------------
//  Default Settings
//-----------------------------------------------------
int def_atom_s_stick;        //--- Atom sticks (screen)
int def_atom_s_slice;        //--- Atom slices (screen)
int def_atom_o_stick;        //--- Atom sticks (output)
int def_atom_o_slice;        //--- Atom slices (output)

int def_bond_s_stick;        //--- Bond sticks (screen)
int def_bond_s_slice;        //--- Bond slices (screen)
int def_bond_o_stick;        //--- Bond sticks (output)
int def_bond_o_slice;        //--- Bond slices (output)

float def_ucel_line_width;   //--- Unit Cell outline width
float def_poly_line_width;   //--- Poly outline width
float def_hkl_line_width;    //--- (hkl) Plane outline width
float def_bond_line_width;   //--- Bond line width

int def_ucel_type;

float def_bkgr_color[3];     //--- Background colors (RGB)
float def_ucel_color[3];     //--- Unit Cell outline colors (RGB)
float def_poly_color[3];     //--- Poly outline colors (RGB)
float def_hkl_color[3];      //--- (hkl) Plane outline colors (RGB)
float def_bond_color[3];     //--- Bond colors (RGB)

float def_poly_tran;         //--- Poly opacity
float def_hkl_tran;          //--- (hkl) Plane opacity

float def_bond_rad[2];       //--- Bond Radius

float def_mol_bond;          //---
int  def_search_bond;        //---

int def_file_format_ID;
//-----------------------------------------------------
//  LookAt Properties
//-----------------------------------------------------
float eye_dist;

//-----------------------------------------------------
//  Depth-cueting Properties
//-----------------------------------------------------
float fog;
float fog_color[4];


//-----------------------------------------------------
//  RIETAN *.ins file Export mode
//-----------------------------------------------------
int export_mode;

//---------------------------------------------------------------------
// FUNCTIONS
//---------------------------------------------------------------------
int CSSR_ID;
int get_format(char *argv);
int check_format(char *name);
void loadDefPar();
void update_atom_type();
void update_atom_type1();
void load_default_pref();
void save_default_pref();
float maxf(float a, float b, float c);
void init_data_settings();
void init_data_settings1();
int get_group_name_and_XYZ();
void XYZ_to_Matrix(int ID);
void Check_Lattice_Type();

void check_file_ext_exist(int form, char *fName, int mode);

int readVICSfile(char *fname);
int readdata(int ID, char *fname, int PhN);
void get_atom_R_and_colors();
int check_num_phase(char *fname);
void update_symmetry();
void set_ortho_cell();
void multiXYZ();
void get_ortho_XYZ();
void get_ortho_origin();
void setOrthoMatrix();
void setOrthoXYZ();
void setOrigin();

void get_bond_search_mode();
void searchBonds();
int search_hydr_bond();
void CalcCN();
void CalcPolyNum();
void makePoly1();

void set_boundaries_ortho_XYZ();
void makeHKLplane(int ID);

void calc_param_for_therm();
void check_iso_uniso_parameters();
int make_th_ellips(int N);

void save_data(char *fname);
void export_data(int ID, char *fname);

void get_symmetry_string(int N, char *line);

VECTOR3D make_symmetry(int N, int Trans[3], VECTOR3D U);

void check_deleted_atoms();

int bonds_for_molecule(int ID);
void set_atom_R();
float _calc_Poly_Vol(int ID);
float _calc_Poly_elong(int ID, float Vol);
float _calc_Poly_var(int ID, float Vol);

int get_spin_from_file(char *fname);







