//***************************************************************************//
//                        CONTOUR MAP DIALOG                                 //
//---------------------------------------------------------------------------//
// File: slice.cpp                                                           //
// Date: 2002.10.01                                                          //
//***************************************************************************//
#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *conglui;                               //--- GLUI window
GLUI_Panel *con_main_panel;                  //--- Main Dialog Panel

GLUI_Panel *con_stype_panel;                 //--- 
GLUI_RadioGroup *con_stype_group;            //---

GLUI_EditText *con_MAX_txt;                  //---
GLUI_EditText *con_MIN_txt;                  //---
GLUI_EditText *con_STP_txt;                  //---
GLUI_EditText *con_NUM_txt;                  //---

GLUI_Panel *con_color_panel;                 //--- 
GLUI_Spinner *con_color_R;                   //--- 
GLUI_Spinner *con_color_G;
GLUI_Spinner *con_color_B;

GLUI_EditText *con_lwidth_txt;               //---

GLUI_Checkbox *con_show_only_check;          //---

GLUI_Button *con_OK_btn;

//--- Control
int con_dialog_ID;
int con_stype;
int con_show_only;

float con_lwidth;
float con_MAX[2], con_MIN[2], con_STP;
int con_NUM[2];
float d1,d2;


//--- Color Palette
GLUI_Button *con_color_palette_btn;
int con_parent_window;
int con_palette_ID;
float cRGB[3];

/**************************************** get_about_dialog_pos() ************/
void get_contour_dialog_pos(int &x, int &y)
{
	glutSetWindow(conglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  set_contour_dialog_id()  ****************************//
void set_contour_dialog_id(int ID)
{
	con_dialog_ID = ID;
}


//********************  get_contour_dialog_id()  ****************************//
int get_contour_dialog_id()
{
	return(con_dialog_ID);
}


//********************  get_contour_colors()  *******************************//
void get_contour_colors(float RGB[3])
{
	RGB[0] = cRGB[0];
	RGB[1] = cRGB[1];
	RGB[2] = cRGB[2];
}

//********************  set_contour_colors()  *******************************//
void set_contour_colors(float RGB[3])
{
	cRGB[0] = RGB[0];
	cRGB[1] = RGB[1];
	cRGB[2] = RGB[2];
	con_color_R->set_int_val((int)(cRGB[0]*100));
	con_color_G->set_int_val((int)(cRGB[1]*100));
	con_color_B->set_int_val((int)(cRGB[2]*100));

	con_color_palette_btn->enable();
	con_OK_btn->enable();
}

//********************  get_contour_dialog()  *******************************//
void get_contour_dialog(int &stype, float cMAX[2], float cMIN[2], int cNUM[2], float &cSTP, 
						float RGB[3], float &width, int &show_only)
{
	int i;
	stype = con_stype;
	for(i=0; i<2; i++){
		cNUM[i] = con_NUM[i];
		cMAX[i] = con_MAX[i];
		cMIN[i] = con_MIN[i];
	}
	cSTP = con_STP;
	RGB[0] = cRGB[0]; RGB[1] = cRGB[1]; RGB[2] = cRGB[2];
	width = con_lwidth;
	show_only = con_show_only;
}



//********************  con_control_cb()  ***********************************//
void con_control_cb( int control )
{
	switch(control)
	{
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(0):{
		conglui->close();
		con_dialog_ID = 1;
		con_stype = con_stype_group->get_int_val();
		con_MAX[con_stype] = con_MAX_txt->get_float_val();
		con_MIN[con_stype] = con_MIN_txt->get_float_val();
		con_STP = con_STP_txt->get_float_val(); 
		con_NUM[con_stype] = con_NUM_txt->get_int_val(); 
		cRGB[0] = (float)con_color_R->get_int_val()/100;
		cRGB[1] = (float)con_color_G->get_int_val()/100;
		cRGB[2] = (float)con_color_B->get_int_val()/100;
		con_lwidth = con_lwidth_txt->get_float_val();
		con_show_only = con_show_only_check->get_int_val();
		break;
			}
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(10):{
		con_dialog_ID = -1;
		con_stype = con_stype_group->get_int_val();
		con_MAX_txt->set_float_val(con_MAX[con_stype]);
		con_MIN_txt->set_float_val(con_MIN[con_stype]);
		con_STP_txt->set_float_val(con_STP);
		con_NUM_txt->set_int_val(con_NUM[con_stype]);
		if (con_stype == 1) {
			con_MAX_txt->set_name("C: ");
			con_MIN_txt->set_name("X: ");
			con_STP_txt->disable();
			con_NUM_txt->enable();
		}
		else {
			con_MAX_txt->set_name("MAX: ");
			con_MIN_txt->set_name("MIN: ");
			con_STP_txt->enable();
			con_NUM_txt->disable();
		}
		break;
			 }
	case(20):{     //--------------------------------------  MAX, MIN
		con_dialog_ID = -1;
		con_MAX[con_stype] = con_MAX_txt->get_float_val();
		con_MIN[con_stype] = con_MIN_txt->get_float_val();
		con_STP = con_STP_txt->get_float_val();
		con_NUM[con_stype] = con_NUM_txt->get_int_val();
		break;
			 }
	case(21):{     //--------------------------------------  NStep
		con_dialog_ID = -1;
		con_MAX[con_stype] = con_MAX_txt->get_float_val();
		con_MIN[con_stype] = con_MIN_txt->get_float_val();
		con_NUM[con_stype] = con_NUM_txt->get_int_val();
		if (con_stype == 0){
			con_STP = (con_MAX[0] - con_MIN[0])/(float)con_NUM[0];
			con_STP_txt->set_float_val(con_STP);
		}
		break;
			 }
	case(22):{     //--------------------------------------  Step
		con_dialog_ID = -1;
		con_MAX[0] = con_MAX_txt->get_float_val();
		con_MIN[0] = con_MIN_txt->get_float_val();
		con_STP = con_STP_txt->get_float_val();
		con_NUM[0] = (int)((con_MAX[0] - con_MIN[0])/con_STP);
		con_NUM_txt->set_int_val(con_NUM[0]);
		break;
			 }
	case(100):{
		con_dialog_ID = 100;
		cRGB[0] = (float)con_color_R->get_int_val()/100;
		cRGB[1] = (float)con_color_G->get_int_val()/100;
		cRGB[2] = (float)con_color_B->get_int_val()/100;
		con_color_palette_btn->disable();
		con_OK_btn->disable();
		break;
			  }
	}
}


//********************  set_contour_dialog()  *******************************//
void set_contour_dialog(int stype, float cMAX[2], float cMIN[2], int cNUM[2], float cSTP,
						float RGB[3], float width, int show_only)
{
	con_stype = stype;
	con_MAX[0] = cMAX[0];
	con_MAX[1] = cMAX[1];
	con_MIN[0] = cMIN[0];
	con_MIN[1] = cMIN[1];
	con_STP = cSTP;
	con_NUM[0] = cNUM[0];
	con_NUM[1] = cNUM[1];
	cRGB[0] = RGB[0]; cRGB[1] = RGB[1]; cRGB[2] = RGB[2];
	con_lwidth = width;
	con_show_only = show_only;
}

//********************  init_contour_dialog()  ******************************//
void init_contour_dialog()
{
	con_stype_group->set_int_val(con_stype);
	con_MAX_txt->set_float_val(con_MAX[con_stype]);
	con_MIN_txt->set_float_val(con_MIN[con_stype]);
	con_STP_txt->set_float_val(con_STP);
	con_NUM_txt->set_int_val(con_NUM[con_stype]);

	con_color_R->set_int_val(int(cRGB[0]*100));
	con_color_G->set_int_val(int(cRGB[1]*100));
	con_color_B->set_int_val(int(cRGB[2]*100));

	con_lwidth_txt->set_float_val(con_lwidth);
	con_show_only_check->set_int_val(con_show_only);

	if (con_stype == 1) {
		con_MAX_txt->set_name("C: ");
		con_MIN_txt->set_name("X: ");
		con_STP_txt->disable();
		con_NUM_txt->enable();
	}
	else {
		con_MAX_txt->set_name("MAX: ");
		con_MIN_txt->set_name("MIN: ");
		con_STP_txt->enable();
		con_NUM_txt->disable();
	}
}


//********************  open_contour_dialog()  ******************************//
void open_contour_dialog(int ID, int dpX, int dpY)
{
	con_dialog_ID = -1;
	con_parent_window = ID;
	con_palette_ID = 0;

	conglui = GLUI_Master.create_glui("Contour Lines",0,dpX,dpY);
	conglui->set_main_gfx_window( ID );
	con_main_panel = conglui->add_panel("",GLUI_PANEL_RAISED);

//-----------------------------------------------------------------------------
	con_stype_panel = conglui->add_panel_to_panel(con_main_panel,"Mode",GLUI_PANEL_EMBOSSED);
	con_stype_panel->set_alignment(GLUI_ALIGN_LEFT);

	con_stype_group = conglui->add_radiogroup_to_panel(con_stype_panel,NULL,10,con_control_cb);
	conglui->add_radiobutton_to_group(con_stype_group,"Normal");
	conglui->add_radiobutton_to_group(con_stype_group,"Exponent");
	conglui->add_separator_to_panel(con_main_panel);
	con_MAX_txt = conglui->add_edittext_to_panel(con_main_panel,"MAX: ",GLUI_EDITTEXT_FLOAT,NULL,20,con_control_cb);
	con_MIN_txt = conglui->add_edittext_to_panel(con_main_panel,"MIN: ",GLUI_EDITTEXT_FLOAT,NULL,20,con_control_cb);
	conglui->add_separator_to_panel(con_main_panel);
	con_STP_txt = conglui->add_edittext_to_panel(con_main_panel,"Step: ",GLUI_EDITTEXT_FLOAT,NULL,22,con_control_cb);
	con_NUM_txt = conglui->add_edittext_to_panel(con_main_panel,"N: ",GLUI_EDITTEXT_INT,NULL,21,con_control_cb);
	con_NUM_txt->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	con_MAX_txt->set_alignment(GLUI_ALIGN_RIGHT);
	con_MIN_txt->set_alignment(GLUI_ALIGN_RIGHT);
	con_STP_txt->set_alignment(GLUI_ALIGN_RIGHT);
	con_NUM_txt->set_alignment(GLUI_ALIGN_RIGHT);
//-----------------------------------------------------------------------------
	conglui->add_column_to_panel(con_main_panel,true);
//-----------------------------------------------------------------------------
	con_color_panel = conglui->add_panel_to_panel(con_main_panel,"Color",GLUI_PANEL_EMBOSSED);
	con_color_panel->set_alignment(GLUI_ALIGN_LEFT);

	con_color_R = conglui->add_spinner_to_panel(con_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL);
	con_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	con_color_G = conglui->add_spinner_to_panel(con_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL);
	con_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	con_color_B = conglui->add_spinner_to_panel(con_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL);
	con_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	conglui->add_separator_to_panel(con_color_panel);
	con_color_palette_btn = conglui->add_button_to_panel(con_color_panel,"Palette",100,con_control_cb);

	con_lwidth_txt = conglui->add_edittext_to_panel(con_main_panel,"Width: ",GLUI_EDITTEXT_FLOAT);

	conglui->add_separator_to_panel(con_main_panel);

	con_show_only_check = conglui->add_checkbox_to_panel(con_main_panel,"Show only contour lines");
//-----------------------------------------------------------------------------
	con_OK_btn = conglui->add_button("OK", 0, con_control_cb);

	init_contour_dialog();
}
