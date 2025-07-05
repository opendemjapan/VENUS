//***************************************************************************//
//                           PREFERENCES DIALOG                              //
//---------------------------------------------------------------------------//
// File: atoms.cpp                                                           //
// Date: 2002.02.18                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   preferences_dialog_ID = -1 -- Open Preferences Dialog                   //
//                            1 -- Close Preferences Dialog ("OK")           //
//                            0 -- Preferences Dialog ("Cancel")             //
//                                                                           //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>

// Objects
GLUI *prefglui;                                //--- PREFERENCES GLUI window
GLUI_Panel *pref_main_panel;                   //--- PREFERENCES Main Panel

GLUI_Panel *pref_back_panel;                   //--- PREFERENCES Background Settings Panel
GLUI_RadioGroup *pref_back_radio;              //--- PREFERENCES Background ID 
                                               //--- 0 --- Main Window
                                               //--- 1 --- 2D-projection Window
                                               //--- 2 --- 3D-projection Window
GLUI_Spinner *back_R_color_spin;               //--- PREFERENCES Unit Background Colors (RGB)
GLUI_Spinner *back_G_color_spin;
GLUI_Spinner *back_B_color_spin;

GLUI_Panel *pref_cell_panel;                   //--- PREFERENCES Unit Cell Outline Settings Panel
GLUI_Spinner *cell_R_color_spin;               //--- PREFERENCES Unit Cell Outline Colors (RGB)
GLUI_Spinner *cell_G_color_spin;
GLUI_Spinner *cell_B_color_spin;
GLUI_EditText *pref_cell_width;                //--- PREFERENCES Unit Cell Outline Width

GLUI_RadioGroup *pref_show_type_radio;         //--- PREFERENCES Show Type ID (Default / Current)

GLUI_Button *pref_set_default;                 //--- PREFERENCES Set Default Settings Button
GLUI_Button *pref_save_default;                //--- PREFERENCES Save as Default Settings Button


GLUI_Panel *pref_setting_panel;
GLUI_Checkbox *show_comp_check;                //--- PREFERENCES Show Compass Checkbox;
GLUI_Checkbox *show_comp_label_check;          //--- PREFERENCES Show Compass labels Checkbox;
GLUI_EditText *pref_stw_step;                  //--- PREFERENCES Stepwise rotation step

GLUI_Listbox *pref_format_lst;

GLUI_Checkbox *save_pos_check;                 //--- PREFERENCES Save Windows Positions Checkbox;
GLUI_Checkbox *save_dialog_check;              //--- PREFERENCES Save Dialog Windows Positions Checkbox;

// Control
int preferences_dialog_ID;                     //--- PREFERENCES dialog control ID
int bkg_ID;                                    //--- PREFERENCES Background control ID
int show_pref_ID;                              //--- PREFERENCES show control ID (default / current)

float u_width[2];
float bkg_color[2][9],u_color[2][3];

int pref_compass[2],pref_lcompass[2];
float stw_angle;
int file_format;
int save_win_pos;
int save_dialog_pos;

/**************************************** get_about_dialog_pos() ************/
void get_pref_dialog_pos(int &x, int &y)
{
	glutSetWindow(prefglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}



//********************  preferency_get_def_param()  *************************//
void preferency_get_def_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float &width, 
							  int &comp, float &stw_step, int &format_ID, int &pos, int &dpos)
{
	int i;
	for(i=0; i<3; i++){
		bkg1[i] = bkg_color[0][i];
		bkg2[i] = bkg_color[0][i+3];
		bkg3[i] = bkg_color[0][i+6];
		cell[i] = u_color[0][i];
	}
	width = u_width[0];
	comp = 0;
	if ((pref_compass[0] == 1) && (pref_lcompass[0] == 1)) comp = 1;
	if ((pref_compass[0] == 1) && (pref_lcompass[0] == 0)) comp = 2;
	stw_step = pref_stw_step->get_float_val();
	format_ID = pref_format_lst->get_int_val();
	save_win_pos = save_pos_check->get_int_val();
	pos = save_win_pos;
	save_dialog_pos = save_dialog_check->get_int_val();
	dpos = save_dialog_pos;
}


//********************  preferency_get_cur_param()  *************************//
void preferency_get_cur_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float &width, 
							  int &comp, float &stw_step, int &format_ID, int &pos, int &dpos)
{
	int i;
	for(i=0; i<3; i++){
		bkg1[i] = bkg_color[1][i];
		bkg2[i] = bkg_color[1][i+3];
		bkg3[i] = bkg_color[1][i+6];
		cell[i] = u_color[1][i];
	}
	width = u_width[1];
	comp = 0;
	if ((pref_compass[1] == 1) && (pref_lcompass[1] == 1)) comp = 1;
	if ((pref_compass[1] == 1) && (pref_lcompass[1] == 0)) comp = 2;
	stw_step = pref_stw_step->get_float_val();
	format_ID = pref_format_lst->get_int_val();
	save_win_pos = save_pos_check->get_int_val();
	pos = save_win_pos;
	save_dialog_pos = save_dialog_check->get_int_val();
	dpos = save_dialog_pos;
}


//********************  get_preferences_dialog_id()  ************************//
void set_preferences_dialog_id(int ID)
{
	preferences_dialog_ID = ID;
}



//********************  get_preferences_dialog_id()  ************************//
int get_preferences_dialog_id()
{
	return(preferences_dialog_ID);
}


//********************  preferency_control_cb()  ****************************//
void preferency_control_cb( int control)
{
	int i;
	switch(control)
	{
	case(0):{
		preferences_dialog_ID = 1; prefglui->close(); 
		break; 
			}
	case(1):{
		preferences_dialog_ID = -1;
		show_pref_ID = pref_show_type_radio->get_int_val();
		if (show_pref_ID == 0){
			pref_set_default->disable();
			pref_save_default->set_name("Save");
		}
		else {
			pref_set_default->enable();
			pref_save_default->set_name("Save as defaults");
		}
		back_R_color_spin->set_int_val((int)(bkg_color[show_pref_ID][0+3*bkg_ID]*100));
		back_G_color_spin->set_int_val((int)(bkg_color[show_pref_ID][1+3*bkg_ID]*100));
		back_B_color_spin->set_int_val((int)(bkg_color[show_pref_ID][2+3*bkg_ID]*100));

		cell_R_color_spin->set_int_val((int)(u_color[show_pref_ID][0]*100));
		cell_G_color_spin->set_int_val((int)(u_color[show_pref_ID][1]*100));
		cell_B_color_spin->set_int_val((int)(u_color[show_pref_ID][2]*100));

		pref_cell_width->set_float_val(u_width[show_pref_ID]);
		show_comp_check->set_int_val(pref_compass[show_pref_ID]);
		break;
			}
	case(2):{
		preferences_dialog_ID = -1;
		for(i=0; i<3; i++){
			bkg_color[1][i] = bkg_color[0][i];
			bkg_color[1][i+3] = bkg_color[0][i+3];
			bkg_color[1][i+6] = bkg_color[0][i+6];
			u_color[1][i] = u_color[0][i];
		}
		u_width[1] = u_width[0];
		if (show_pref_ID == 1){
			back_R_color_spin->set_int_val((int)(bkg_color[show_pref_ID][0+3*bkg_ID]*100));
			back_G_color_spin->set_int_val((int)(bkg_color[show_pref_ID][1+3*bkg_ID]*100));
			back_B_color_spin->set_int_val((int)(bkg_color[show_pref_ID][2+3*bkg_ID]*100));

			cell_R_color_spin->set_int_val((int)(u_color[show_pref_ID][0]*100));
			cell_G_color_spin->set_int_val((int)(u_color[show_pref_ID][1]*100));
			cell_B_color_spin->set_int_val((int)(u_color[show_pref_ID][2]*100));
			pref_cell_width->set_float_val(u_width[show_pref_ID]);
		}
		break;
			}
	case(3):{
		preferences_dialog_ID = 3;
		if (show_pref_ID != 0){
		for(i=0; i<3; i++){
			bkg_color[0][i] = bkg_color[1][i];
			bkg_color[0][i+3] = bkg_color[1][i+3];
			bkg_color[0][i+6] = bkg_color[1][i+6];
			u_color[0][i] = u_color[1][i];
		}
		u_width[0] = u_width[1];
		}
		if (show_pref_ID == 0){
			back_R_color_spin->set_int_val((int)(bkg_color[show_pref_ID][0+3*bkg_ID]*100));
			back_G_color_spin->set_int_val((int)(bkg_color[show_pref_ID][1+3*bkg_ID]*100));
			back_B_color_spin->set_int_val((int)(bkg_color[show_pref_ID][2+3*bkg_ID]*100));

			cell_R_color_spin->set_int_val((int)(u_color[show_pref_ID][0]*100));
			cell_G_color_spin->set_int_val((int)(u_color[show_pref_ID][1]*100));
			cell_B_color_spin->set_int_val((int)(u_color[show_pref_ID][2]*100));
			pref_cell_width->set_float_val(u_width[show_pref_ID]);
		}
		break;
			}
	case(10):{
		preferences_dialog_ID = -1;
		bkg_ID = pref_back_radio->get_int_val();
		back_R_color_spin->set_int_val((int)(bkg_color[show_pref_ID][0+3*bkg_ID]*100));
		back_G_color_spin->set_int_val((int)(bkg_color[show_pref_ID][1+3*bkg_ID]*100));
		back_B_color_spin->set_int_val((int)(bkg_color[show_pref_ID][2+3*bkg_ID]*100));
		break;
			 }
	case(20):{
		preferences_dialog_ID = -1;
		bkg_color[show_pref_ID][0+3*bkg_ID] = (float)back_R_color_spin->get_int_val()/100;
		bkg_color[show_pref_ID][1+3*bkg_ID] = (float)back_G_color_spin->get_int_val()/100;
		bkg_color[show_pref_ID][2+3*bkg_ID] = (float)back_B_color_spin->get_int_val()/100;
		break;
			 }
	case(30):{
		preferences_dialog_ID = -1;
		u_width[show_pref_ID] = pref_cell_width->get_float_val();
		break;
			 }
	case(40):{
		preferences_dialog_ID = -1;
		u_color[show_pref_ID][0] = (float)cell_R_color_spin->get_int_val()/100;
		u_color[show_pref_ID][1] = (float)cell_G_color_spin->get_int_val()/100;
		u_color[show_pref_ID][2] = (float)cell_B_color_spin->get_int_val()/100;
		break;
			 }
	case(50):{
		preferences_dialog_ID = -1;
		pref_compass[show_pref_ID] = show_comp_check->get_int_val();
		pref_lcompass[show_pref_ID] = show_comp_label_check->get_int_val();
		if (pref_compass[show_pref_ID] == 0) show_comp_label_check->disable();
		else show_comp_label_check->enable();

		break;
			 }
	}
}


//********************  preferency_set_def_param()  *************************//
void preferency_set_def_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float width, 
							  int comp, float stw_step, int format_ID, int pos, int dpos)
{
	int i;
	for(i=0; i<3; i++){
		bkg_color[0][i] = bkg1[i];
		bkg_color[0][i+3] = bkg2[i];
		bkg_color[0][i+6] = bkg3[i];
		u_color[0][i] = cell[i];
	}
	u_width[0] = width;
	switch(comp)
	{
	case(0):{
		pref_compass[0] = 0;
		pref_lcompass[0] = 0;
		break;
			}
	case(1):{
		pref_compass[0] = 1;
		pref_lcompass[0] = 1;
		break;
			}
	case(2):{
		pref_compass[0] = 1;
		pref_lcompass[0] = 0;
		break;
			}
	}
	show_pref_ID = 1;
	bkg_ID = 0;
	stw_angle = stw_step;
	file_format = format_ID;
	save_win_pos = pos;
    save_dialog_pos = dpos;
}

//********************  preferency_set_cur_param()  *************************//
void preferency_set_cur_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float width, int comp)
{
	int i;
	for(i=0; i<3; i++){
		bkg_color[1][i] = bkg1[i];
		bkg_color[1][i+3] = bkg2[i];
		bkg_color[1][i+6] = bkg3[i];
		u_color[1][i] = cell[i];
	}
	u_width[1] = width;
	switch(comp)
	{
	case(0):{
		pref_compass[1] = 0;
		pref_lcompass[1] = 0;
		break;
			}
	case(1):{
		pref_compass[1] = 1;
		pref_lcompass[1] = 1;
		break;
			}
	case(2):{
		pref_compass[1] = 1;
		pref_lcompass[1] = 0;
		break;
			}
	}

}

//********************  preferency_set_cur_param()  *************************//
void init_preferences_dialog()
{
	pref_show_type_radio->set_int_val(show_pref_ID);
	pref_back_radio->set_int_val(bkg_ID);

	back_R_color_spin->set_int_val((int)(bkg_color[1][0]*100));
	back_G_color_spin->set_int_val((int)(bkg_color[1][1]*100));
	back_B_color_spin->set_int_val((int)(bkg_color[1][2]*100));

	cell_R_color_spin->set_int_val((int)(u_color[1][0]*100));
	cell_G_color_spin->set_int_val((int)(u_color[1][1]*100));
	cell_B_color_spin->set_int_val((int)(u_color[1][2]*100));

	pref_cell_width->set_float_val(u_width[1]);
	pref_stw_step->set_float_val(stw_angle);

	show_comp_check->set_int_val(pref_compass[1]);
	show_comp_label_check->set_int_val(pref_lcompass[1]);
	if (pref_compass[1] == 0) show_comp_label_check->disable();
	else show_comp_label_check->enable();
	pref_format_lst->set_int_val(file_format);
	save_pos_check->set_int_val(save_win_pos);
	save_dialog_check->set_int_val(save_dialog_pos);
}



//********************  open_preferences_dialog()  **************************//
void open_preferences_dialog(int ID, int dpX, int dpY)
{
	preferences_dialog_ID = -1;

	prefglui = GLUI_Master.create_glui("Preferences",0,dpX,dpY );
	prefglui->set_main_gfx_window( ID );

	pref_main_panel = prefglui->add_panel("",GLUI_PANEL_RAISED);
	pref_main_panel->set_alignment(GLUI_ALIGN_LEFT);
//--------------------------------------------------------------------------
	pref_back_panel = prefglui->add_panel_to_panel(pref_main_panel,"Background color",GLUI_PANEL_EMBOSSED);
	pref_back_panel->set_alignment(GLUI_ALIGN_LEFT);

	pref_back_radio = prefglui->add_radiogroup_to_panel(pref_back_panel,NULL,10,preferency_control_cb);
	prefglui->add_radiobutton_to_group(pref_back_radio,"Graphics Window");
	prefglui->add_radiobutton_to_group(pref_back_radio,"2D Map Window");
	prefglui->add_radiobutton_to_group(pref_back_radio,"Bird's-Eye View Window");

	prefglui->add_column_to_panel(pref_back_panel,true);
	
	back_R_color_spin = prefglui->add_spinner_to_panel(pref_back_panel,"Red (%): ",GLUI_SPINNER_INT,
		                                                                NULL,20,preferency_control_cb);
	back_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	back_G_color_spin = prefglui->add_spinner_to_panel(pref_back_panel,"Green (%): ",GLUI_SPINNER_INT,
		                                                                NULL,20,preferency_control_cb);
	back_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	back_B_color_spin = prefglui->add_spinner_to_panel(pref_back_panel,"Blue (%): ",GLUI_SPINNER_INT,
		                                                                NULL,20,preferency_control_cb);
	back_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
//--------------------------------------------------------------------------
	pref_cell_panel = prefglui->add_panel_to_panel(pref_main_panel,"Unit cell",GLUI_PANEL_EMBOSSED);
	pref_cell_panel->set_alignment(GLUI_ALIGN_LEFT);

	pref_cell_width = prefglui->add_edittext_to_panel(pref_cell_panel,"Width:",GLUI_EDITTEXT_FLOAT,
		                                                                    NULL,30,preferency_control_cb);
	pref_cell_width->set_w(120);

	prefglui->add_column_to_panel(pref_cell_panel,true);
	
	cell_R_color_spin = prefglui->add_spinner_to_panel(pref_cell_panel,"Red (%): ",GLUI_SPINNER_INT,
		                                                                NULL,40,preferency_control_cb);
	cell_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	cell_G_color_spin = prefglui->add_spinner_to_panel(pref_cell_panel,"Green (%): ",GLUI_SPINNER_INT,
		                                                                NULL,40,preferency_control_cb);
	cell_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	cell_B_color_spin = prefglui->add_spinner_to_panel(pref_cell_panel,"Blue (%): ",GLUI_SPINNER_INT,
		                                                                NULL,40,preferency_control_cb);
	cell_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
//--------------------------------------------------------------------------
	pref_setting_panel = prefglui->add_panel_to_panel(pref_main_panel,"Settings",GLUI_PANEL_EMBOSSED);
	pref_setting_panel->set_alignment(GLUI_ALIGN_LEFT);

	pref_stw_step =  prefglui->add_edittext_to_panel(pref_setting_panel,"Rotation step: ",GLUI_EDITTEXT_FLOAT);
	pref_stw_step->set_w(150);
	pref_stw_step->set_alignment(GLUI_ALIGN_RIGHT);

	pref_format_lst = prefglui->add_listbox_to_panel(pref_setting_panel,"Default ext.: ",NULL);
	pref_format_lst->set_alignment(GLUI_ALIGN_RIGHT);
	pref_format_lst->add_item(0,"den");
	pref_format_lst->add_item(1,"cube");
	pref_format_lst->add_item(2,"scat");
	pref_format_lst->add_item(3,"rho");
	pref_format_lst->add_item(4,"vnd");
	pref_format_lst->add_item(5,"*ed");
	pref_format_lst->add_item(6,"eb");

	prefglui->add_column_to_panel(pref_setting_panel,true);

	show_comp_check = prefglui->add_checkbox_to_panel(pref_setting_panel,"Show compass",NULL,50,preferency_control_cb);
	show_comp_label_check = prefglui->add_checkbox_to_panel(pref_setting_panel,"Show axis labels",NULL,50,preferency_control_cb);

//--------------------------------------------------------------------------
	prefglui->add_column_to_panel(pref_main_panel,true);
//--------------------------------------------------------------------------
	prefglui->add_separator_to_panel(pref_main_panel);
	save_pos_check = prefglui->add_checkbox_to_panel(pref_main_panel,"Save bars/window positions",NULL);
	save_dialog_check = prefglui->add_checkbox_to_panel(pref_main_panel,"Save dialog box positions",NULL);

	prefglui->add_separator_to_panel(pref_main_panel);
	pref_show_type_radio = prefglui->add_radiogroup_to_panel(pref_main_panel,NULL,1,preferency_control_cb);
	prefglui->add_radiobutton_to_group(pref_show_type_radio,"Show default settings");
	prefglui->add_radiobutton_to_group(pref_show_type_radio,"Show current settings");

	prefglui->add_separator_to_panel(pref_main_panel);

	pref_set_default = prefglui->add_button_to_panel(pref_main_panel,"Restore default settings", 2, preferency_control_cb);
	pref_save_default = prefglui->add_button_to_panel(pref_main_panel,"Save as defaults", 3, preferency_control_cb);
	pref_set_default->set_w(150);
	pref_save_default->set_w(150);

	prefglui->add_separator_to_panel(pref_main_panel);

	prefglui->add_button_to_panel(pref_main_panel,"OK", 0, preferency_control_cb);
//--------------------------------------------------------------------------
	init_preferences_dialog();
}
