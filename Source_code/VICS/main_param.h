#include <GL/glui.h>
#include <GL/glut.h>
#include "vics.h"

//-------------------   GLUI Window Settings
int   main_window;                                //--- Main GLUI window ID
int tx, ty, tw, th;                               //--- GLUI viewport_area
float xy_aspect;                                  //--- GLUI windows aspect
int last_x, last_y;                               //--- Mouse Last Position

//-------------------   GLUI Statusbar Subwindow
GLUI *gluisub;                                       //--- Statusbar Subwindow 
GLUI_StaticText *statusbar,*statusbarinfo;
char infotext[55];
char statustext[55];


//-------------------   GLUI Tools Window
GLUI *glui;                                          //--- GLUI Tools window
GLUI_Panel *main_panel;                              //--- Main GLUI panel
//---------------------------------------------------------------------------//
//                           Properties Rollout Settings                     //
//---------------------------------------------------------------------------//
GLUI_Rollout *rollout_prop;                          //--- Properties Rollout

//---------------------------------------------------------------------------//
//                           Model Panel Settings                            //
//---------------------------------------------------------------------------//
GLUI_Panel *model_panel;                             //--- Model Panel
GLUI_RadioGroup *model_radio;
int model_mode = 0;                                  //--- Model ID
GLUI_Checkbox *dot_surf_check;

//---------------------------------------------------------------------------//
//                           Compass Settings                                //
//---------------------------------------------------------------------------//
float shift_compass[2];

//---------------------------------------------------------------------------//
//                           Crystal Orientation Settings                    //
//---------------------------------------------------------------------------//
GLUI_Button *bOrient;                           //--- Set Orientation


//---------------------------------------------------------------------------//
//                           Tools Panel Settings                            //
//---------------------------------------------------------------------------//
GLUI_Panel *tools_panel;
GLUI_Button *bR;                                 //--- Rotate
GLUI_Button *bM;                                 //--- Magnify
GLUI_Button *bT;                                 //--- Translate
GLUI_Button *bD;                                 //--- Bond Distance
GLUI_Button *bA;                                 //--- Bond Angle
GLUI_Button *bS;                                 //--- Select
int tools_mode;                                  //--- Tools ID

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
GLUI_Button *b100;                               //--- [100] direction
GLUI_Button *b010;                               //--- [010] direction
GLUI_Button *b001;                               //--- [001] direction
int rotation_mode;                               //--- Rotation ID
int rotation_type;                               //--- Rotation Type ID
int auto_rot_step;                               //--- Auto Rotation Step Value


//-------------------   GLUI Menu Window
GLUI *menu_glui;                                     //--- GLUI Menu window
GLUI_Panel *menu_panel;                              //--- Main Menu panel
//-------------------   GLUI Main Menu
//--- File
GLUI_Button *new_btn;                                //--- New File Button
GLUI_Button *open_btn;                               //--- Open File Button
GLUI_Button *save_btn;                               //--- Save File Button
GLUI_Button *img_btn;                                //--- Export Image Button 
GLUI_Button *exp_btn;                                //--- Export File Button 
//--- Edit
GLUI_Button *edit_btn;                               //--- Edit Data Button 
GLUI_Button *bonds_btn;                              //--- Edit Bonds Button 
GLUI_Button *hkl_btn;                                //--- (hkl) Plane Button 
//--- Help
GLUI_Button *pref_btn;
GLUI_Button *about_btn;                              //--- About Button
//GLUI_Button *help_btn;                               //--- Help Button
GLUI_Button *exit_btn;                               //--- Quit Button
int model_pick=0; // What part is picked
int pick_obj[MAX_PICK_OBJ];
int objNum;


GLUI_Button *stw_btn;



//-----------------------------------------------------
//  Dialogs Control
//-----------------------------------------------------
int new_edit_dialog_ID;       //--- New/Edit Dialog Control
int openfile_dialog_ID;       //--- Open File Dialog Control
int plot_range_dialog_ID;     //--- Plot Range Dialog Control
int bonds_dialog_ID;          //--- Make/Edit Bonds Dialog Control
int atoms_prop_dialog_ID;     //--- Atoms Properties Dialog Control
int general_prop_dialog_ID;   //--- General Properties Dialog Control
int bonds_prop_dialog_ID;     //--- Bonds Properties Dialog Control
int poly_prop_dialog_ID;      //--- Polyhedra Properties Dialog Control
int ucell_prop_dialog_ID;     //--- Unit Cell Outline Properties Dialog Control
int hkl_prop_dialog_ID;       //--- (hkl) Plane Properties Dialog Control
int prefernces_dialog_ID;     //--- Preferences Dialog Control
int message_dialog_ID1;
int message_dialog_ID2;
int message_dialog_ID3;
int image_dialog_ID;          //--- Export Image Dialog Control
int	arg_openfile_ID;
int corient_dialog_ID;        //--- Crystal Orientation Dailog Control
int about_dialog_ID;          //--- About Dialog Control
int stepwise_dialog_ID;       //--- Stepwise Rotation Dialog Control
int spin_dialog_ID;           //--- Spin Dialog Control
int color_dialog_ID;          //--- Color Palette Dialog Control
int poly_type_dialog_ID;
int chk_image_ID;
int orig_dialog_ID;


