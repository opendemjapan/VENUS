#include <GL/glui.h>
#include <GL/glut.h>

#define TRUE	1
#define FALSE	0


//-------------------   GLUI Window Settings
int   main_window;                                //--- Main GLUI window ID
int tx, ty, tw, th;                               //--- GLUI viewport_area
float xy_aspect;                                  //--- GLUI windows aspect
//int init_w, init_h;                               //--- Initial Window size
//int init_pos_x, init_pos_y;                       //--- Initial Window position
//-------------------  GLUT Mouse Settings
int last_x, last_y;                               //--- Last Mouse Position



//---------------------------------------------------------------------------//
//                       GLUI Statusbar Subwindow                            //
//---------------------------------------------------------------------------//
GLUI *gluisub;                                       //--- Statusbar Subwindow 
GLUI_StaticText *statusbar;
char statustext[55];

//---------------------------------------------------------------------------//
//                          GLUI Menu Window                                 //
//---------------------------------------------------------------------------//
GLUI *menu_glui;                                     //--- GLUI Menu window
GLUI_Panel *menu_panel;                              //--- Main Menu panel
//-------------------   GLUI Main Menu
//--- File
GLUI_Button *open_btn;                               //--- Open File Button
GLUI_Button *save_btn;                               //--- Save File Button
GLUI_Button *exp_btn;                                //--- Export File Button 
//--- Options
GLUI_Button *info_btn;                               //--- Information Button
GLUI_Button *proj_btn;                               //--- 2D distribution Button
GLUI_Button *atom_btn;                               //--- Show Atom Button
//--- Help
GLUI_Button *pref_btn;                               //--- Preferency Button
GLUI_Button *about_btn;                              //--- About Button
GLUI_Button *help_btn;                               //--- Help Button
//--- Exit
GLUI_Button *exit_btn;                               //--- Exit Button


//---------------------------------------------------------------------------//
//                          GLUI Tools Window                                //
//---------------------------------------------------------------------------//
GLUI *glui;                                          //--- GLUI Tools window
GLUI_Panel *main_panel;                              //--- Main GLUI panel
//----------------------------------------  Properties Rollout Settings
GLUI_Rollout *rollout_prop;                          //--- Properties Rollout

//----------------------------------------  Model Panel Settings
GLUI_Panel *model_panel;                             //--- Model Panel
GLUI_RadioGroup *model_radio;

//----------------------------------------  Projection Type Settings
//int projection_type = 0;                             //--- Projection Type ID
//float perspective = 1.0;
//----------------------------------------  Show Isosurface ID
GLUI_Checkbox *show_iso_check;
//----------------------------------------  Show Isosurface ID
GLUI_Checkbox *show_sls_check;
//----------------------------------------  Show Atoms ID
GLUI_Checkbox *show_model_check;
int show_model = 0;

//----------------------------------------  Orientation
GLUI_Button *bOrient;


GLUI_Button *stw_btn;

//---------------------------------------------------------------------------//
//                           Tools Panel Settings                            //
//---------------------------------------------------------------------------//
GLUI_Panel *tools_panel;
GLUI_Button *bR;                                 //--- Rotate
GLUI_Button *bM;                                 //--- Magnify
GLUI_Button *bT;                                 //--- Translate
int tools_mode = 0;                              //--- Tools ID

//---------------------------------------------------------------------------//
//                           Rotation Panel Settings                         //
//---------------------------------------------------------------------------//
GLUI_Panel *rotation_panel;
GLUI_Panel *rotation_mode_panel;
GLUI_RadioGroup *rotation_tipe_radio;            //--- Rotation Tipe [Continue / Step]
GLUI_Button *bFree;                              //--- Free Rotation
GLUI_Button *bX;                                 //--- X-axis
GLUI_Button *bY;                                 //--- Y-axis
GLUI_Button *bZ;                                 //--- Z-axis
int rotation_mode;                               //--- Rotation ID
int rotation_type;                               //--- Rotation Type ID
int auto_rot_step;                               //--- Auto Rotation Step Value

//---------------------------------------------------------------------------//
//                           2D- and 3D- Projection Windows                  //
//---------------------------------------------------------------------------//
int window_2D_ID;
int window_3D_ID;

GLUI_Button *Mbtn;

//-----------------------------------------------------
//  Dialog Control
//-----------------------------------------------------
int	arg_openfile_ID;
int	openfile_dialog_ID;
int savefile_dialog_ID;
int isosurface_Dialog_ID;
int unitcell_Dialog_ID;
int info_Dialog_ID;
int sli_Dialog_ID;
int image_Dialog_ID;
int pref_Dialog_ID;
int	genral_Dialog_ID;
int prj_Dialog_ID;
int cont_Dialog_ID;
int model_Dialog_ID;
int corient_dialog_ID;
int about_dialog_ID;
int stepwise_dialog_ID;
int color_dialog_ID;
int model_proper_dialog_ID;
int message_dialog_ID1;

int chk_image_ID;
