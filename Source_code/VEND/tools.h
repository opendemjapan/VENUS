#include "vend.h"

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
// 1 - countour2D dialog
// 2 - image export dialog
// 3 - info dialog
// 4 - isosurf dialog
// 5 - model dialog
// 6 - orientations dialog
// 7 - preferences dialog
// 8 - 2D projections dialog
// 9 - slices dialog
//10 - stepwise dialog
//11 - unit cell dialog
int dwp[2][12],cdwp[2][12];
int save_dpos_ID;


SYMMPOS sPos[MAX_SYMM_OP];

//-----------------------------------------------------
//  Unit Cell
//-----------------------------------------------------
 float cCell[6];              //--- Unit Cell Parameters
 float ortho_cCell[6];
 float max_P;
 double ortho_M[3][3];        //--- Orthogonal MAtrix
 float uCell[8][3];           //--- Orthogonal XYZ for Unit Cell Outline

//-----------------------------------------------------
//  Axis Ball
//-----------------------------------------------------
 float uAxis[3][3];           //--- Orthogonal XYZ for "Orientation Ball"
 float ball_diffuse[3][4];    //--- Material Diffuse

//-----------------------------------------------------
//  File 
//-----------------------------------------------------
int fileID;
int den_file_ID;
char FileName[256];
char *arg_fname;
int def_file_format_ID;

//-----------------------------------------------------
//  Save VEND file
//-----------------------------------------------------
int sfileID;
char sFileName[256];

//-----------------------------------------------------
//  3D-Data 
//-----------------------------------------------------
char Title[80];                          //---  Title
float OBJLEV;                            //--- Isosurface Level for 3D-Data
float SECLEV;
int NASYM;                               //--- Number of Pixels in asym. unit
int NPIX[3];                             //--- Dimension
int NSYM;                                //--- Number of symmetry operations
int NCEN;                                //--- Central Inversion element ID (0 - no; 1 - yes)
int NSUB;                                //--- Number of Lattice (Brave) Translations
int ISUB[3][4];                          //--- Brave Translations (in Pixels
float HMIN,HMAX;                         //--- Min. and Max. of 3D-Data
float DELC[MAX_PIX][MAX_PIX][MAX_PIX];   //--- 3D density
int DELC_ID;
float ORIGIN[3];                         //--- 
int TOTAL_T;                             //--- Number of Isosurfase triangles
int TOTAL_S[6];                          //--- 
int TOTAL_P[3];                          //--- 

//-----------------------------------------------------
//  Rendering
//-----------------------------------------------------
float l_direction[16];
float l_position[4];
float l_dir[3];
float l_ambient[4];
float l_diffuse[4];

//-----------------------------------------------------
//  Orientation
//-----------------------------------------------------
float rotation[16];

//-----------------------------------------------------
//  Isosurface
//-----------------------------------------------------
float mat_ambient[4];                   //--- Material Ambient 
float mat_diffuse[4];                   //--- Material Diffuse
float mat_specular[4];                  //--- Material Specular
float mat_emission[4];                  //--- Material Emission
float mat_shininess;                    //--- Material Shininess
int range_ID;                           //--- Output range ID (0 - Box; 1 - Cell);
float range[6];                         //--- Boundaries  (for range_ID = 1)
int numBox[3];                          //--- Number of Boxes along (a,b,c) (for range_ID = 0)
int iso_pm_parm;
int model_mode;                                  //--- Model ID


int show_iso;                                    //--- Show Isosurface ID
int show_sls;                                    //--- Show Isosurface ID

//-----------------------------------------------------
//  Atoms
//-----------------------------------------------------
float atom_ambient[4];                   //--- Material Ambient 
float atom_specular[4];                  //--- Material Specular
float atom_emission[4];                  //--- Material Emission
float atom_shininess;                    //--- Material Shininess

//-----------------------------------------------------
//  Unit Cell
//-----------------------------------------------------
int unit_cell_show;                       //--- Unit Cell Show ID
float unit_cell_width;                    //--- Unit Cell Outline Width
int unit_cell_type;
int def_unit_cell_type;
float def_unit_cell_width;                //--- Default Unit Cell Outline Width
float unit_cell_color[3];                 //--- Unit Cell Outline Color
float def_unit_cell_color[3];             //--- Default Unit Cell Outline Color

float scale;
float obj_pos[3];
float origin[3];

//-----------------------------------------------------
//  Compass Properties
//-----------------------------------------------------
int show_axis;               //--- Show Compass ID
int def_show_axis;

//-----------------------------------------------------
//  Projection Properties
//-----------------------------------------------------
int projection_type;                             //--- Projection Type ID
float perspective;

//-----------------------------------------------------
//  (hkl) Plane
//-----------------------------------------------------
float objlev_scale;
float seclev_scale;
int palette_hkl;
int num_of_hkl;
float opacity_hkl;
float position_hkl[3],c_position_hkl;
short index_hkl[3][3],c_index_hkl[3];
short index_show_hkl[3];
float hkl_normal[3];
float hkl_origin[3];
int hkl_mode;
int index_c_type, index_type[3];

//-----------------------------------------------------
//  2D Output
//-----------------------------------------------------
float prj_seclev_scale;
float prj_3Dlev_scale;
float prj_project[2];
int prj_plane;
int prj_cell;
int	prj_pal;
int prj_cont_show;
int	prj_show_2D;
int	prj_show_3D;
float prj_RGB[3];
float prj_width;
int prj_hkl[3];
float prj_org;
int old_hkl[3];
int prj_grid;

//-----------------------------------------------------
//  2D Contour Map
//-----------------------------------------------------
int cont_stype;
int cont_show_only;
float conMAX[2], conMIN[2];
int cont_NSTP[2];
float cont_step;
float cont_RGB[3];
float cont_width;

//---------------------------------------------------------------------------//
//                           Image Control                                   //
//---------------------------------------------------------------------------//
unsigned char *image,*image1;
char img_fName[256];
int img_format;
int img_scale;
int img_smoothing;
int run_img_export;
int img_exp_from;



//-----------------------------------------------------
//  Background
//-----------------------------------------------------
float BKCL[3];                //--- Background Color (RGB) for the Main Window
float BKCL2D[3];              //--- Background Color (RGB) for the 2D-Projection Window
float BKCL3D[3];              //--- Background Color (RGB) for the 3D-Projection Window

float def_BKCL[3];            //--- Default Background Color (RGB) for the Main Window
float def_BKCL2D[3];          //--- Default Background Color (RGB) for the 2D-Projection Window
float def_BKCL3D[3];          //--- Default Background Color (RGB) for the 3D-Projection Window

//-----------------------------------------------------
//  Additional
//-----------------------------------------------------
short Pal[100][6];            //--- Palette

//-----------------------------------------------------
//  Stepwise Rotation Control
//-----------------------------------------------------
int stepwise_active;
float stepw_angle;
float def_stepw_angle;
int stepw_mode;
int stepw_rot_axis;
float stepw_rot[3];


//==============================================================
void check_file_ext_exist(int form, char *fName, int mode);
int get_format(char *argv);
int check_format(char *name);
float maxf(float a, float b, float c);
void load_preferency();
void save_preferency();
void load_palette();
void set_initital_parameters();
int read_data_from_file(int ID, char *fname);
void set_orthoMatrix();
void set_Ortho_ABC();
void saveSCRIPT(char *name);








