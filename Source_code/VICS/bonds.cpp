//***************************************************************************//
//                      BONDS PROPERTIES DIALOG                              //
//---------------------------------------------------------------------------//
// File: bonds.cpp                                                           //
// Date: 2002.01.11                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   bonds_proper_dialog_ID = -1 -- Open Bonds Properties Dialog             //
//                             1 -- Close Bonds Properties Dialog ("OK")     //
//                             0 -- Close Bonds Properties Dialog ("Cancel") //
//                           300 -- Get Bonds Properties                     //
//                                                                           //
//   bond_type_ID = 0 --- 1 Color Cylinder                                   // 
//                  1 --- 2 Color Cylinder                                   // 
//                  2 --- Color Line                                         // 
//                  3 --- Gradient Color Line                                // 
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>

// Objects
GLUI *bondglui;                                //--- BONDS Propertires GLUI window
GLUI_Panel *bond_main_panel;                   //--- BONDS Main Panel
GLUI_Panel *bond_btn_panel;                    //--- BONDS "OK"/"Cancel" Panel
GLUI_Panel *bond_type_panel;                   //--- BONDS Type Panel
GLUI_Panel *bond_par_panel;                    //--- BONDS Paraneters panel
GLUI_Panel *bond_color_panel;                  //--- BONDS Colors Panel
GLUI_RadioGroup *bond_type_radio;              //--- BONDS Type Radio
GLUI_Panel *hbond_type_panel;                  //--- BONDS Type Panel (Hydrogen)
GLUI_RadioGroup *hbond_type_radio;             //--- BONDS Type Radio (Hydrogen)
GLUI_Spinner *bond_stack_spin;                 //--- BONDS Stacks
GLUI_Spinner *bond_slice_spin;                 //--- BONDS Slices 
GLUI_Spinner *bond_radius1_spin;               //--- BONDS Radius (B&S)
GLUI_Spinner *bond_radius2_spin;               //--- BONDS Radius (Stick)
GLUI_Spinner *bond_width_spin;                 //--- BONDS Width (for bond_type_ID = 2,3)
GLUI_Spinner *bond_R_color_spin;               //--- BONDS Colors (RGB)
GLUI_Spinner *bond_G_color_spin;
GLUI_Spinner *bond_B_color_spin;
GLUI_Checkbox *bond_prev_check;                //--- BONDS Preview Checkbox;

GLUI_Button *bond_OK_btn;
GLUI_Button *bond_Cancel_btn;


// Live Variables
int bond_type_ID, old_bond_type_ID;            //--- BONDS Type ID
int hbond_type_ID, old_hbond_type_ID;          //--- BONDS Type ID (Hydrogen)
int bStack, old_bStack;                        //--- BONDS Stacks  
int bSlice, old_bSlice;                        //--- BONDS Slices
float bRadius[2], old_bRadius[2];              //--- BONDS Radius: 0 -- for B&S; 1 -- for Stick
float bLineWidth, old_bLineWidth;              //--- BONDS Line Width (for bond_type_ID = 2,3)
int bond_R_color, bond_G_color, bond_B_color;  //--- BONDS Color (RGB)
int old_bond_RGB[3];
int Model;

// Control
int bonds_proper_Dialog_ID;                    //--- Bonds dialog control ID
int bonds_preview = 1;                         //--- Bonds properties preview control 

//--- Color Palette
GLUI_Button *bcolor_palette_btn;
int bond_parent_window;
int bond_palette_ID;
float bRGB[3];


/**************************************** get_bonds_dialog_pos() ************/
void get_bonds_dialog_pos(int &x, int &y)
{
	glutSetWindow(bondglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

//********************  set_bond_prop_dialog_id()  **************************//
void set_bond_prop_dialog_id(int ID)
{
	bonds_proper_Dialog_ID = ID;
}

//********************  get_bond_prop_dialog_id()  **************************//
int get_bond_prop_dialog_id()
{
	return(bonds_proper_Dialog_ID);
}

//********************  get_bond_prop_preview_id()  **************************//
int get_bond_prop_preview_id()
{
	return(bonds_preview);
}

//********************  get_bond_colors()  **********************************//
void set_bond_colors(float RGB[3])
{
	bond_R_color_spin->set_int_val((int)(RGB[0]*100));
	bond_G_color_spin->set_int_val((int)(RGB[1]*100));
	bond_B_color_spin->set_int_val((int)(RGB[2]*100));
	bcolor_palette_btn->enable();
	bond_Cancel_btn->enable();
	bond_OK_btn->enable();
	bond_type_panel->enable();
}


//********************  get_bond_colors()  **********************************//
void get_bond_colors(float RGB[3])
{
	RGB[0] = (float)bond_R_color_spin->get_int_val()/100;
	RGB[1] = (float)bond_G_color_spin->get_int_val()/100;
	RGB[2] = (float)bond_B_color_spin->get_int_val()/100;
}


/**************************************** get_bond_main_prop_params() *******/
void get_bond_main_prop_params(int &Mode, int &Stack, int &Slice, float &lWidth, int &hMode)
{
	Mode = bond_type_radio->get_int_val();
	Stack = bond_stack_spin->get_int_val();
	Slice = bond_slice_spin->get_int_val();
	lWidth = bond_width_spin->get_float_val();
	hMode = hbond_type_radio->get_int_val();
}

/**************************************** get_bond_radius_B&S() *************/
float get_bond_prop_radius_B_S()
{
	bRadius[0] = bond_radius1_spin->get_float_val();
	return(bRadius[0]);
}

/**************************************** get_bond_radius_Stick() ***********/
float get_bond_prop_radius_Stick()
{
	bRadius[1] = bond_radius2_spin->get_float_val();
	return(bRadius[1]);
}

/**************************************** get_bond_prop_colors_R() **********/
float get_bond_prop_colors_R()
{
	return((float)bond_R_color_spin->get_int_val()/100);
}

/**************************************** get_bond_prop_colors_G() **********/
float get_bond_prop_colors_G()
{
	return((float)bond_G_color_spin->get_int_val()/100);
}

/**************************************** get_bond_prop_colors_B() **********/
float get_bond_prop_colors_B()
{
	return((float)bond_B_color_spin->get_int_val()/100);
}


/**************************************** bond_prop_control() ***************/
void bond_prop_control( int control)
{
	switch(control)
	{
	case(0):{bonds_proper_Dialog_ID = 1; bondglui->close(); break; }
	case(1):{
		bond_type_radio->set_int_val(old_bond_type_ID);
		bond_stack_spin->set_int_val(old_bStack);
		bond_slice_spin->set_int_val(old_bSlice);
		bond_radius1_spin->set_float_val(old_bRadius[0]);
		bond_radius2_spin->set_float_val(old_bRadius[1]);
		bond_width_spin->set_float_val(old_bLineWidth);
		bond_R_color_spin->set_int_val(old_bond_RGB[0]);
		bond_G_color_spin->set_int_val(old_bond_RGB[1]);
		bond_B_color_spin->set_int_val(old_bond_RGB[2]);
		bonds_proper_Dialog_ID = 0; bondglui->close();  break; 
			}
	case(300):{
		if (Model == 4){
			bond_type_ID = 3;
			bond_type_radio->set_int_val(3);
		}
		if (Model == 5) {
			if (bond_type_ID > 1) bond_type_ID = 0;
			bond_type_radio->set_int_val(bond_type_ID);
		}
		switch(bond_type_ID)
		{
		case(0):{  //--- 1-Color Cylinder
			bond_stack_spin->enable();
			bond_slice_spin->enable();
			bond_radius1_spin->enable();
			bond_radius2_spin->enable();
			bond_width_spin->disable();
			bond_color_panel->enable();
			break;
				}
		case(1):{  //--- 2-Color Cylinder
			bond_stack_spin->enable();
			bond_slice_spin->enable();
			bond_radius1_spin->enable();
			bond_radius2_spin->enable();
			bond_width_spin->disable();
			bond_color_panel->disable();
			break;
				}
		case(2):{  //--- Color line
			bond_stack_spin->disable();
			bond_slice_spin->disable();
			bond_radius1_spin->disable();
			bond_radius2_spin->disable();
			bond_width_spin->enable();
			bond_color_panel->enable();
			break;
				}
		case(3):{  //--- Gradient line
			bond_stack_spin->disable();
			bond_slice_spin->disable();
			bond_radius1_spin->disable();
			bond_radius2_spin->disable();
			bond_width_spin->enable();
			bond_color_panel->disable();
			break;
				}
		}
		bonds_proper_Dialog_ID = 300;
		break;
			 }
	case(301):{bonds_proper_Dialog_ID = 300; break;}
	case(400):{
		bonds_proper_Dialog_ID = 400;
		bcolor_palette_btn->disable();
		bond_Cancel_btn->disable();
		bond_OK_btn->disable();
		bond_type_panel->disable();
		break;
			  }
	case(500):{bonds_proper_Dialog_ID = 300; break;}
	}
}

/**************************************** set_bond_prop_params() ************/
void set_bond_prop_params(int Mode, int Stack, int Slice, float Rad[2], float lWidth, float RGB[3],
						  int aModel, int hMode)
{
	old_bond_type_ID = bond_type_ID = Mode;

	old_hbond_type_ID = hbond_type_ID = hMode;

	old_bStack = bStack = Stack;
	old_bSlice = bSlice = Slice;
	old_bLineWidth = bLineWidth = lWidth;
	old_bRadius[0] = bRadius[0] = Rad[0];
	old_bRadius[1] = bRadius[1] = Rad[1];
	old_bond_RGB[0] = bond_R_color = (int)(RGB[0]*100);
	old_bond_RGB[1] = bond_G_color = (int)(RGB[1]*100);
	old_bond_RGB[2] = bond_B_color = (int)(RGB[2]*100);
	Model = aModel;
}

/**************************************** init_bonds_prop_dialog() **********/
void init_bonds_prop_dialog()
{
	hbond_type_radio->set_int_val(hbond_type_ID);
	if (Model == 4){
		bond_type_ID = 3;
		bond_type_radio->set_int_val(3);
	}
	if (Model == 5) {
		if (bond_type_ID > 1) bond_type_ID = 0;
		bond_type_radio->set_int_val(bond_type_ID);
	}
	switch(bond_type_ID)
	{
	case(0):{  //--- 1-Color Cylinder
		bond_stack_spin->enable();
		bond_slice_spin->enable();
		bond_radius1_spin->enable();
		bond_radius2_spin->enable();
		bond_width_spin->disable();
		bond_color_panel->enable();
		break;
			}
	case(1):{  //--- 2-Color Cylinder
		bond_stack_spin->enable();
		bond_slice_spin->enable();
		bond_radius1_spin->enable();
		bond_radius2_spin->enable();
		bond_width_spin->disable();
		bond_color_panel->disable();
		break;
			}
	case(2):{  //--- Color line
		bond_stack_spin->disable();
		bond_slice_spin->disable();
		bond_radius1_spin->disable();
		bond_radius2_spin->disable();
		bond_width_spin->enable();
		bond_color_panel->enable();
		break;
			}
	case(3):{  //--- Gradient line
		bond_stack_spin->disable();
		bond_slice_spin->disable();
		bond_radius1_spin->disable();
		bond_radius2_spin->disable();
		bond_width_spin->enable();
		bond_color_panel->disable();
		break;
			}
	}
}

/**************************************** open_bonds_prop_dialog() **********/
void open_bonds_prop_dialog(int ID, int dpX, int dpY)
{
	bonds_proper_Dialog_ID = -1;
	bond_parent_window = ID;
	bond_palette_ID = 0;

	bondglui = GLUI_Master.create_glui("Bonds",0,dpX,dpY );
	bondglui->set_main_gfx_window( ID );

	bond_main_panel = bondglui->add_panel("",GLUI_PANEL_RAISED);
	bond_main_panel->set_alignment(GLUI_ALIGN_LEFT);

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
	bond_type_panel = bondglui->add_panel_to_panel(bond_main_panel,"Bond style",GLUI_PANEL_EMBOSSED);
	bond_type_radio = bondglui->add_radiogroup_to_panel(bond_type_panel,&bond_type_ID,300,bond_prop_control);
	bondglui->add_radiobutton_to_group(bond_type_radio,"Unicolor cylinder");
	bondglui->add_radiobutton_to_group(bond_type_radio,"Bicolor cylinder");
	bondglui->add_radiobutton_to_group(bond_type_radio,"Color line");
	bondglui->add_radiobutton_to_group(bond_type_radio,"Gradient line");
//--------------------------------------------------------------------------
	hbond_type_panel = bondglui->add_panel_to_panel(bond_main_panel,"Hydrogen bond style",GLUI_PANEL_EMBOSSED);
	hbond_type_radio = bondglui->add_radiogroup_to_panel(hbond_type_panel,&hbond_type_ID,500,bond_prop_control);
	bondglui->add_radiobutton_to_group(hbond_type_radio,"Solid lines");
	bondglui->add_radiobutton_to_group(hbond_type_radio,"Dotted lines");
	bondglui->add_radiobutton_to_group(hbond_type_radio,"Dashed lines");
	
	bond_prev_check = bondglui->add_checkbox_to_panel(bond_main_panel,"Preview",&bonds_preview);
	bond_prev_check->set_int_val(1);
//--------------------------------------------------------------------------
	bondglui->add_column_to_panel(bond_main_panel,false);
//--------------------------------------------------------------------------
	bond_par_panel = bondglui->add_panel_to_panel(bond_main_panel,"Parameters",GLUI_PANEL_EMBOSSED);
	bond_stack_spin = bondglui->add_spinner_to_panel(bond_par_panel,"Stacks: ",GLUI_SPINNER_INT,NULL,301,bond_prop_control);
	bond_stack_spin->set_int_limits(1,50,GLUI_LIMIT_CLAMP);
	bond_stack_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bond_slice_spin = bondglui->add_spinner_to_panel(bond_par_panel,"Slices: ",GLUI_SPINNER_INT,NULL,301,bond_prop_control);
	bond_slice_spin->set_int_limits(3,50,GLUI_LIMIT_CLAMP);
	bond_slice_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bond_radius1_spin = bondglui->add_spinner_to_panel(bond_par_panel,"Radius (B&S, poly): ",GLUI_SPINNER_FLOAT,NULL,301,bond_prop_control);
	bond_radius1_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	bond_radius1_spin->set_speed(0.1);
	bond_radius1_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bond_radius2_spin = bondglui->add_spinner_to_panel(bond_par_panel,"Radius (stick model): ",GLUI_SPINNER_FLOAT,NULL,301,bond_prop_control);
	bond_radius2_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	bond_radius2_spin->set_speed(0.1);
	bond_radius2_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bondglui->add_separator_to_panel(bond_par_panel);
	bond_width_spin = bondglui->add_spinner_to_panel(bond_par_panel,"Line width: ",GLUI_SPINNER_FLOAT,NULL,301,bond_prop_control);
	bond_width_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	bond_width_spin->set_speed(10.0);
	bond_width_spin->set_alignment(GLUI_ALIGN_RIGHT);
//--------------------------------------------------------------------------
	bondglui->add_column_to_panel(bond_main_panel,false);
//--------------------------------------------------------------------------
	bond_color_panel = bondglui->add_panel_to_panel(bond_main_panel,"Color",GLUI_PANEL_EMBOSSED);
	bond_R_color_spin = bondglui->add_spinner_to_panel(bond_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL,301,bond_prop_control);
	bond_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	bond_G_color_spin = bondglui->add_spinner_to_panel(bond_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL,301,bond_prop_control);
	bond_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	bond_B_color_spin = bondglui->add_spinner_to_panel(bond_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL,301,bond_prop_control);
	bond_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	bondglui->add_separator_to_panel(bond_color_panel);
	bcolor_palette_btn = bondglui->add_button_to_panel(bond_color_panel,"Palette",400,bond_prop_control);

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
	bond_btn_panel = bondglui->add_panel("",GLUI_PANEL_NONE);
	bond_OK_btn = bondglui->add_button_to_panel(bond_btn_panel,"OK", 0, bond_prop_control);
	bondglui->add_column_to_panel(bond_btn_panel,false);
	bond_Cancel_btn = bondglui->add_button_to_panel(bond_btn_panel,"Cancel", 1, bond_prop_control);

	bond_type_radio->set_int_val(bond_type_ID);
	bond_stack_spin->set_int_val(bStack);
	bond_slice_spin->set_int_val(bSlice);
	bond_radius1_spin->set_float_val(bRadius[0]);
	bond_radius2_spin->set_float_val(bRadius[1]);
	bond_width_spin->set_float_val(bLineWidth);
	bond_R_color_spin->set_int_val(bond_R_color);
	bond_G_color_spin->set_int_val(bond_G_color);
	bond_B_color_spin->set_int_val(bond_B_color);

	init_bonds_prop_dialog();
}
