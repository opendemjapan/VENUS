//***************************************************************************//
//                            STEPWISE ROTATION DIALOG                       //
//---------------------------------------------------------------------------//
// File: stepwise.cpp                                                        //
// Date: 2002.05.30                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   stepwise_dialog_ID = -1 -- Open Dialog                                  //
//                         1 -- Close Dialog ("OK")                          //
//                         0 -- Close Dialog ("Cancel")                      //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>


// Objects
GLUI *stepw;                              //--- STEPWISE GLUI window
GLUI_Panel *stepw_main_panel;             //--- STEPWISE Main Panel
GLUI_Panel *stepw_axis_btn_panel;         //--- STEPWISE Roation Axis Panel
GLUI_Panel *stepw_btn_panel;              //--- STEPWISE "OK" / "CANCEL" Panel

GLUI_RadioGroup *stepw_type_radio;        //--- STEPWISE Type Radio
GLUI_EditText   *stepw_s_txt;             //--- STEPWISE Step Edit

GLUI_Button *Xbtn, *Ybtn, *Zbtn;          //--- STEPWISE Rotation Axis Button


// Control
int stepwise_Dialog_ID;
float rot_step;
int axis_ID;
int rot_mode;

/**************************************** get_about_dialog_pos() ************/
void get_step_dialog_pos(int &x, int &y)
{
	glutSetWindow(stepw->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

/**************************************** get_stepwise_param() **************/
void get_stepwise_param(int &Mode, int &Axis, float &step)
{
	Mode = stepw_type_radio->get_int_val();
	Axis = axis_ID;
	step = stepw_s_txt->get_float_val();
}

/**************************************** set_stepwise_dialog_id() **********/
void set_stepwise_dialog_id(int ID)
{
	stepwise_Dialog_ID = ID;
}

/**************************************** get_stepwise_dialog_id() **********/
int get_stepwise_dialog_id()
{
	return(stepwise_Dialog_ID);
}

/**************************************** stepw_control() *******************/
void stepw_control( int control)
{
	switch(control)
	{
	case(0):{stepwise_Dialog_ID = 1; stepw->close();break;}
	case(1):{stepwise_Dialog_ID = 0; stepw->close();break;}
	case(12):
	case(11):
	case(10):{
		stepwise_Dialog_ID = 10;
		axis_ID = control - 10;
		break;
			 }
	}
}

/**************************************** init_stepwise_dialog() ************/
void init_stepwise_dialog(float step, int mode)
{
	rot_step = step;
	axis_ID = 0;
	rot_mode = mode;
}

/**************************************** open_stepwise_dialog() ************/
void open_stepwise_dialog(int ID, int dpX, int dpY)
{
	stepwise_Dialog_ID = -1;

	stepw = GLUI_Master.create_glui("Stepwise Rotation",0,dpX,dpY );
	stepw->set_main_gfx_window( ID );

	stepw_main_panel = stepw->add_panel("",GLUI_PANEL_RAISED);
	stepw_main_panel->set_alignment(GLUI_ALIGN_CENTER);
	stepw->add_separator_to_panel(stepw_main_panel);
//------------------------------------------------------------------------------
	stepw_type_radio = stepw->add_radiogroup_to_panel(stepw_main_panel,&rot_mode);
	stepw->add_radiobutton_to_group(stepw_type_radio,"Single step");
	stepw->add_radiobutton_to_group(stepw_type_radio,"Continuous");
	stepw_type_radio->set_alignment(GLUI_ALIGN_LEFT);
//------------------------------------------------------------------------------
	stepw_s_txt = stepw->add_edittext_to_panel(stepw_main_panel,"Step (deg): ",GLUI_EDITTEXT_FLOAT);
	stepw_s_txt->set_float_limits(-180,180, GLUI_LIMIT_CLAMP);
	stepw_s_txt->set_float_val(rot_step); 
	stepw_s_txt->set_alignment(GLUI_ALIGN_LEFT);
//------------------------------------------------------------------------------
	stepw_axis_btn_panel = stepw->add_panel_to_panel(stepw_main_panel,"Rotation axis",GLUI_PANEL_EMBOSSED);
	Xbtn = stepw->add_button_to_panel(stepw_axis_btn_panel,"X axis", 10, stepw_control);
	stepw->add_column_to_panel(stepw_axis_btn_panel, false);
	Ybtn = stepw->add_button_to_panel(stepw_axis_btn_panel,"Y axis", 11, stepw_control);
	stepw->add_column_to_panel(stepw_axis_btn_panel, false);
	Zbtn = stepw->add_button_to_panel(stepw_axis_btn_panel,"Z axis", 12, stepw_control);
	Xbtn->set_w(60);
	Ybtn->set_w(60);
	Zbtn->set_w(60);
//------------------------------------------------------------------------------
	stepw_btn_panel = stepw->add_panel("",GLUI_PANEL_NONE);
	stepw->add_button_to_panel(stepw_btn_panel,"OK", 0, stepw_control);
	stepw->add_column_to_panel(stepw_btn_panel,false);
	stepw->add_button_to_panel(stepw_btn_panel,"Cancel", 1, stepw_control);
}



