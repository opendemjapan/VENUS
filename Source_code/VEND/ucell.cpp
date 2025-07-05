//***************************************************************************//
//                   UNIT CELL OUTLINE PROPERTIES DIALOG                     //
//---------------------------------------------------------------------------//
// File: ucell.cpp                                                           //
// Date: 2002.01.16                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   ucell_proper_dialog_ID = -1 -- Open uCell Properties Dialog             //
//                             1 -- Close uCell Properties Dialog ("OK")     //
//                             0 -- Close uCell Properties Dialog ("Cancel") //
//                           500 -- Get uCell Properties                     //
//                                                                           //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>


// Objects
GLUI *ucellglui;                                //--- UCELL Propertires GLUI window
GLUI_Panel *ucell_main_panel;                   //--- UCELL Main Panel
GLUI_Panel *ucell_prop_panel;                   //--- UCELL Properties Panel
GLUI_Panel *ucell_color_panel;                  //--- UCELL Colors Panel
GLUI_Panel *ucell_type_panel;
GLUI_Checkbox *ucell_show_check;                //--- UCELL Show Outline;
GLUI_Spinner *ucell_R_color_spin;               //--- UCELL Colors (RGB)
GLUI_Spinner *ucell_G_color_spin;
GLUI_Spinner *ucell_B_color_spin;
GLUI_EditText *ucell_width_txt;                 //--- UCELL Outline Width

GLUI_RadioGroup *ucell_type_radio;              //--- ATOMS Type Radio


GLUI_Button *ucell_OK_btn;


// Live Variables
int show_cell;                                  //--- Show Unit Cell Outline ( 0 - "Yes"; 1 - "No" )
float cell_width;                               //--- Unit Cell Outline Width
int cell_type;
float cell_color[3];                            //--- Unit Cell Outline Colors (RGB)

// Control
int ucell_proper_Dialog_ID;                     //--- UCELL dialog control ID

//--- Color Palette
GLUI_Button *ucolor_palette_btn;
int ucell_parent_window;
int ucell_palette_ID;
float uRGB[3];

/**************************************** get_ucell_dialog_pos() ************/
void get_ucell_dialog_pos(int &x, int &y)
{
	glutSetWindow(ucellglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

//********************  set_ucell_prop_dialog_id()  **************************//
void set_ucell_prop_dialog_id(int ID)
{
	ucell_proper_Dialog_ID = ID;
}


//********************  get_ucell_prop_dialog_id()  **************************//
int get_ucell_prop_dialog_id()
{
	return(ucell_proper_Dialog_ID);
}


/**************************************** set_ucell_colors() ****************/
void set_ucell_colors(float RGB[3])
{
	uRGB[0] = RGB[0];
	uRGB[1] = RGB[1];
	uRGB[2] = RGB[2];
	ucell_R_color_spin->set_int_val((int)(uRGB[0]*100));
	ucell_G_color_spin->set_int_val((int)(uRGB[1]*100));
	ucell_B_color_spin->set_int_val((int)(uRGB[2]*100));
	ucolor_palette_btn->enable();
	ucell_OK_btn->enable();
}

/**************************************** get_ucell_colors() ****************/
void get_ucell_colors(float RGB[3])
{
	RGB[0] = uRGB[0];
	RGB[1] = uRGB[1];
	RGB[2] = uRGB[2];
}


/**************************************** init_ucell_prop_dialog() **********/
void get_ucell_prop(int &show_ID, float &width, float &R, float &G, float &B, int &type)
{
	show_ID = ucell_show_check->get_int_val();
	width = ucell_width_txt->get_float_val();
	R = (float)ucell_R_color_spin->get_int_val()/100;
	G = (float)ucell_G_color_spin->get_int_val()/100;
	B = (float)ucell_B_color_spin->get_int_val()/100;
	type = ucell_type_radio->get_int_val();
}

/**************************************** ucell_prop_control_prop_control() ***************/
void ucell_prop_control( int control)
{
	switch(control)
	{
	case(0):{ucell_proper_Dialog_ID = 1; ucellglui->close(); break; }
	case(500):{ucell_proper_Dialog_ID = 500;break;}
	case(600):{
		ucell_proper_Dialog_ID = 600;
		uRGB[0] = (float)ucell_R_color_spin->get_int_val()/100;
		uRGB[1] = (float)ucell_G_color_spin->get_int_val()/100;
		uRGB[2] = (float)ucell_B_color_spin->get_int_val()/100;
		ucolor_palette_btn->disable();
		ucell_OK_btn->disable();
		break;
			  }
	}
}

/**************************************** init_ucell_prop_dialog() **********/
void set_ucell_prop_dialog(int show_ID, float width, float color[3], int type)
{
	show_cell = show_ID;
	cell_width = width;
	cell_type = type;
	cell_color[0] = color[0];
	cell_color[1] = color[1];
	cell_color[2] = color[2];
}

/**************************************** open_ucell_prop_dialog() **********/
void open_ucell_prop_dialog(int ID, int dpX, int dpY)
{
	ucell_proper_Dialog_ID = -1;
	ucell_parent_window = ID;
	ucell_palette_ID = 0;

	ucellglui = GLUI_Master.create_glui("Unit Cell",0,dpX,dpY );
	ucellglui->set_main_gfx_window( ID );

	ucell_main_panel = ucellglui->add_panel("",GLUI_PANEL_RAISED);
	ucell_main_panel->set_alignment(GLUI_ALIGN_LEFT);

//--------------------------------------------------------------------------
	ucell_show_check = ucellglui->add_checkbox_to_panel(ucell_main_panel,"Show unit cell edges",
		                                                NULL,500,ucell_prop_control);
//--------------------------------------------------------------------------
	ucell_type_panel = ucellglui->add_panel_to_panel(ucell_main_panel,"Line type",GLUI_PANEL_EMBOSSED);
	ucell_type_radio = ucellglui->add_radiogroup_to_panel(ucell_type_panel,NULL,500,ucell_prop_control);
	ucellglui->add_radiobutton_to_group(ucell_type_radio,"Solid lines");
	ucellglui->add_radiobutton_to_group(ucell_type_radio,"Dotted lines");
	ucellglui->add_radiobutton_to_group(ucell_type_radio,"Dashed lines");

//--------------------------------------------------------------------------
	ucell_width_txt  = ucellglui->add_edittext_to_panel(ucell_main_panel,"Width: ",GLUI_EDITTEXT_FLOAT,
                		                                                NULL,500,ucell_prop_control);

//--------------------------------------------------------------------------
	ucellglui->add_column_to_panel(ucell_main_panel,false);
//--------------------------------------------------------------------------
	ucell_color_panel = ucellglui->add_panel_to_panel(ucell_main_panel,"Color",GLUI_PANEL_EMBOSSED);
	ucell_R_color_spin = ucellglui->add_spinner_to_panel(ucell_color_panel,"Red (%): ",GLUI_SPINNER_INT,
		                                                NULL,500,ucell_prop_control);
	ucell_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	ucell_G_color_spin = ucellglui->add_spinner_to_panel(ucell_color_panel,"Green (%): ",GLUI_SPINNER_INT,
		                                                NULL,500,ucell_prop_control);
	ucell_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	ucell_B_color_spin = ucellglui->add_spinner_to_panel(ucell_color_panel,"Blue (%): ",GLUI_SPINNER_INT,
		                                                NULL,500,ucell_prop_control);
	ucell_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	ucellglui->add_separator_to_panel(ucell_color_panel);
	ucolor_palette_btn = ucellglui->add_button_to_panel(ucell_color_panel,"Palette",600,ucell_prop_control);

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
	ucell_OK_btn = ucellglui->add_button("OK", 0, ucell_prop_control);

	ucell_show_check->set_int_val(show_cell);
	ucell_width_txt->set_float_val(cell_width);
	ucell_type_radio->set_int_val(cell_type);
	ucell_R_color_spin->set_int_val((int)(cell_color[0]*100));
	ucell_G_color_spin->set_int_val((int)(cell_color[1]*100));
	ucell_B_color_spin->set_int_val((int)(cell_color[2]*100));
}




