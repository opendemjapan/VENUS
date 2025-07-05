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
#include "winopen.h"


// Objects
GLUI *prefglui;                                //--- PREFERENCES GLUI window
GLUI_Panel *pref_main_panel;                   //--- PREFERENCES Main Panel
GLUI_Panel *pref_objres_panel;                 //--- PREFERENCES Main Panel
GLUI_Panel *pref_screenres_panel;              //--- PREFERENCES Main Panel
GLUI_Panel *pref_outputres_panel;              //--- PREFERENCES Main Panel
GLUI_EditText *pref_atom_s_stack;              //--- PREFERENCES Atoms Stacks (Screen)
GLUI_EditText *pref_atom_s_slice;              //--- PREFERENCES Atoms Slices (Screen)
GLUI_EditText *pref_atom_o_stack;              //--- PREFERENCES Atoms Stacks (Output)
GLUI_EditText *pref_atom_o_slice;              //--- PREFERENCES Atoms Slices (Output)
GLUI_EditText *pref_bond_s_stack;              //--- PREFERENCES Bonds Stacks (Screen)
GLUI_EditText *pref_bond_s_slice;              //--- PREFERENCES Bonds Slices (Screen)
GLUI_EditText *pref_bond_o_stack;              //--- PREFERENCES Bonds Stacks (Output)
GLUI_EditText *pref_bond_o_slice;              //--- PREFERENCES Bonds Slices (Output)

GLUI_RadioGroup *pref_show_type_radio;         //--- PREFERENCES Show Type ID (Default / Current)

GLUI_Panel *pref_object_settings;              //--- PREFERENCES Line Width Settings
GLUI_RadioGroup *pref_object_radio;            //--- PREFERENCES Objects ID

GLUI_EditText *pref_l_width;                   //--- PREFERENCES Object Line Width
GLUI_Spinner  *pref_opacity;                   //--- PREFERENCES Object Opacity

GLUI_Panel *pref_colors_settings;              //--- PREFERENCES Color Settings
GLUI_Spinner *pref_color_R;                    //--- PREFERENCES Objects Colors (RGB)
GLUI_Spinner *pref_color_G;                    //--- 
GLUI_Spinner *pref_color_B;                    //--- 

GLUI_Panel *pref_bonds_r_settings;             //--- PREFERENCES Bonds Radius Settings
GLUI_EditText *pref_bond_rad1;                 //--- PREFERENCES Bond Radius1 (for B&S model)
GLUI_EditText *pref_bond_rad2;                 //--- PREFERENCES Bond Radius1 (for Stics model)

GLUI_Button *pref_set_default;                 //--- PREFERENCES Set Default Settings Button
GLUI_Button *pref_save_default;                //--- PREFERENCES Save as Default Settings Button

GLUI_Checkbox *show_comp_check;                //--- PREFERENCES Show Compass Checkbox;
GLUI_Checkbox *show_comp_label_check;          //--- PREFERENCES Show Compass Labels Checkbox;

GLUI_EditText *pref_stw_step;                  //--- PREFERENCES Stepwise rotation step

GLUI_Panel *pref_object_settings1;
GLUI_EditText *pref_max_bond;                  //--- PREFERENCES Max bond distance
GLUI_Checkbox *startup_search_check;

GLUI_Listbox *pref_format_lst;


GLUI_Panel *pref_template_panel;
GLUI_EditText *pref_temp_name;

GLUI_Panel *pref_btn_panel;
GLUI_Button *pref_OK_btn;

GLUI_Checkbox *save_pos_check;                 //--- PREFERENCES Save Windows Positions Checkbox;
GLUI_Checkbox *save_dialog_check;              //--- PREFERENCES Save Dialog Windows Positions Checkbox;

// Control
int prefernces_Dialog_ID;                     //--- PREFERENCES dialog control ID
int object_ID;                                 //--- PREFERENCES objects control ID
int show_pref_ID;                              //--- PREFERENCES show control ID (default / current)

int a_Stick_s[2],a_Slice_s[2],a_Stick_o[2],a_Slice_o[2];
int b_Stick_s[2],b_Slice_s[2],b_Stick_o[2],b_Slice_o[2];
float u_Width[2],p_Width[2],h_Width[2],b_Width[2];
float p_Trans[2],h_Trans[2];
float bkg_color[2][3],u_color[2][3],p_color[2][3],h_color[2][3],b_color[2][3];
float b_Radius[2][2];
int compass[2],c_comp;
int lcompass[2],lc_comp;
float stw_angle;
float bond_dist;
int startup_search;
int file_format;
char export_File_Name[256];
int save_win_pos;
int save_dialog_pos;

/**************************************** get_prefer_dialog_pos() ************/
void get_prefer_dialog_pos(int &x, int &y)
{
	glutSetWindow(prefglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

//********************  get_preferences_dialog_id()  ************************//
int get_preferences_dialog_id()
{
	return(prefernces_Dialog_ID);
}

//********************  set_preferences_dialog_id()  ************************//
void set_preferences_dialog_id(int ID)
{
	prefernces_Dialog_ID = ID;
}

//**************************************** get_preferences_cur_param() ******//
void get_preferences_cur_param(int &a_St_s, int &a_Sl_s, int &a_St_o, int &a_Sl_o,
							   int &b_St_s, int &b_Sl_s, int &b_St_o, int &b_Sl_o,
							   float &u_w, float &p_w, float &h_w, float &b_w,
							   float &p_t, float &h_t,
							   float &bkg_c_R, float &bkg_c_G, float &bkg_c_B,
							   float &u_c_R, float &u_c_G, float &u_c_B,
							   float &p_c_R, float &p_c_G, float &p_c_B,
							   float &h_c_R, float &h_c_G, float &h_c_B,
							   float &b_c_R, float &b_c_G, float &b_c_B,
							   float &b_r_1, float &b_r_2, int &comp, float &stw_step, 
							   float &bond, int &startup_S,int &format_ID, 
							   char *export_fileN, int &pos, int &dpos)
{
	a_St_s = a_Stick_s[1];
	a_Sl_s = a_Slice_s[1];
	a_St_o = a_Stick_o[1];
	a_Sl_o = a_Slice_o[1];

	b_St_s = b_Stick_s[1];
	b_Sl_s = b_Slice_s[1];
	b_St_o = b_Stick_o[1];
	b_Sl_o = b_Slice_o[1];

	u_w = u_Width[1];
	p_w = p_Width[1];
	h_w = h_Width[1];
	b_w = b_Width[1];

	p_t = p_Trans[1];
	h_t = h_Trans[1];

	bkg_c_R = bkg_color[1][0];bkg_c_G = bkg_color[1][1];bkg_c_B = bkg_color[1][2];
	u_c_R = u_color[1][0];u_c_G = u_color[1][1];u_c_B = u_color[1][2];
	p_c_R = p_color[1][0];p_c_G = p_color[1][1];p_c_B = p_color[1][2];
	h_c_R = h_color[1][0];h_c_G = h_color[1][1];h_c_B = h_color[1][2];
	b_c_R = b_color[1][0];b_c_G = b_color[1][1];b_c_B = b_color[1][2];

	b_r_1 = b_Radius[1][0];
	b_r_2 = b_Radius[1][1];

	comp = 0;
	if ((compass[1] == 1) && (lcompass[1] == 1)) comp = 1;
	else if ((compass[1] == 1) && (lcompass[1] == 0)) comp = 2;

	stw_step = pref_stw_step->get_float_val();
	bond = pref_max_bond->get_float_val();
	startup_S = startup_search_check->get_int_val();

	format_ID = pref_format_lst->get_int_val();
	sprintf(export_File_Name,"");
	sprintf(export_File_Name,"%s",pref_temp_name->get_text());
	sprintf(export_fileN,"%s",export_File_Name);

	save_win_pos = save_pos_check->get_int_val();
	pos = save_win_pos;
	save_dialog_pos = save_dialog_check->get_int_val();
	dpos = save_dialog_pos;
}

//**************************************** get_preferences_def_param() ******//
void get_preferences_def_param(int &a_St_s, int &a_Sl_s, int &a_St_o, int &a_Sl_o,
							   int &b_St_s, int &b_Sl_s, int &b_St_o, int &b_Sl_o,
							   float &u_w, float &p_w, float &h_w, float &b_w,
							   float &p_t, float &h_t,
							   float &bkg_c_R, float &bkg_c_G, float &bkg_c_B,
							   float &u_c_R, float &u_c_G, float &u_c_B,
							   float &p_c_R, float &p_c_G, float &p_c_B,
							   float &h_c_R, float &h_c_G, float &h_c_B,
							   float &b_c_R, float &b_c_G, float &b_c_B,
							   float &b_r_1, float &b_r_2, int &comp, float &stw_step, 
							   float &bond, int &startup_S,int &format_ID, 
							   char *export_fileN, int &pos, int &dpos)
{
	a_St_s = a_Stick_s[0];
	a_Sl_s = a_Slice_s[0];
	a_St_o = a_Stick_o[0];
	a_Sl_o = a_Slice_o[0];

	b_St_s = b_Stick_s[0];
	b_Sl_s = b_Slice_s[0];
	b_St_o = b_Stick_o[0];
	b_Sl_o = b_Slice_o[0];

	u_w = u_Width[0];
	p_w = p_Width[0];
	h_w = h_Width[0];
	b_w = b_Width[0];

	p_t = p_Trans[0];
	h_t = h_Trans[0];

	bkg_c_R = bkg_color[0][0];bkg_c_G = bkg_color[0][1];bkg_c_B = bkg_color[0][2];
	u_c_R = u_color[0][0];u_c_G = u_color[0][1];u_c_B = u_color[0][2];
	p_c_R = p_color[0][0];p_c_G = p_color[0][1];p_c_B = p_color[0][2];
	h_c_R = h_color[0][0];h_c_G = h_color[0][1];h_c_B = h_color[0][2];
	b_c_R = b_color[0][0];b_c_G = b_color[0][1];b_c_B = b_color[0][2];

	b_r_1 = b_Radius[0][0];
	b_r_2 = b_Radius[0][1];

	comp = 0;
	if ((compass[0] == 1) && (lcompass[0] == 1)) comp = 1;
	else if ((compass[0] == 1) && (lcompass[0] == 0)) comp = 2;
	stw_step = pref_stw_step->get_float_val();
	bond = pref_max_bond->get_float_val();
	startup_S = startup_search_check->get_int_val();
	format_ID = pref_format_lst->get_int_val();
	sprintf(export_File_Name,"");
	sprintf(export_File_Name,"%s",pref_temp_name->get_text());
	sprintf(export_fileN,"%s",export_File_Name);
	save_win_pos = save_pos_check->get_int_val();
	pos = save_win_pos;
	save_dialog_pos = save_dialog_check->get_int_val();
	dpos = save_dialog_pos;
}



//**************************************** preferences_control() ************//
void preferences_control( int control)
{
	int i,j;
	switch(control)
	{
	case(0):{
		prefernces_Dialog_ID = 1; prefglui->close(); 
		break; 
			}
	case(1):{
		prefernces_Dialog_ID = -1;
		a_Stick_s[1] = a_Stick_s[0];
		a_Slice_s[1] = a_Slice_s[0];
		a_Stick_o[1] = a_Stick_o[0];
		a_Slice_o[1] = a_Slice_o[0];

		b_Stick_s[1] = b_Stick_s[0];
		b_Slice_s[1] = b_Slice_s[0];
		b_Stick_o[1] = b_Stick_o[0];
		b_Slice_o[1] = b_Slice_o[0];

		u_Width[1] = u_Width[0];
		p_Width[1] = p_Width[0];
		h_Width[1] = h_Width[0];
		b_Width[1] = b_Width[0];

		p_Trans[1] = p_Trans[0];
		h_Trans[1] = h_Trans[0];

		compass[1] = compass[0];

		for(i=0; i<3; i++){
			bkg_color[1][i] = bkg_color[0][i];
			u_color[1][i] = u_color[0][i];
			p_color[1][i] = p_color[0][i];
			h_color[1][i] = h_color[0][i];
			b_color[1][i] = b_color[0][i];
		}
		b_Radius[1][0] = b_Radius[0][0];
		b_Radius[1][1] = b_Radius[0][1];
		
		if (show_pref_ID == 1){
			pref_atom_s_stack->set_int_val(a_Stick_s[show_pref_ID]);
			pref_atom_s_slice->set_int_val(a_Slice_s[show_pref_ID]);
			pref_bond_s_stack->set_int_val(b_Stick_s[show_pref_ID]);
			pref_bond_s_slice->set_int_val(b_Slice_s[show_pref_ID]);
			pref_atom_o_stack->set_int_val(a_Stick_o[show_pref_ID]);
			pref_atom_o_slice->set_int_val(a_Slice_o[show_pref_ID]);
			pref_bond_o_stack->set_int_val(b_Stick_o[show_pref_ID]);
			pref_bond_o_slice->set_int_val(b_Slice_o[show_pref_ID]);
			switch(object_ID)
			{
//------------------------------------------------- Background
			case(0):{
				pref_color_R->set_int_val((int)(bkg_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(bkg_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(bkg_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- Unit Cell
			case(1):{
				pref_l_width->set_float_val(u_Width[show_pref_ID]);
				pref_color_R->set_int_val((int)(u_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(u_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(u_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- Polyhedra
			case(2):{
				pref_l_width->set_float_val(p_Width[show_pref_ID]);
				pref_opacity->set_int_val((int)(p_Trans[show_pref_ID]*100));
				pref_color_R->set_int_val((int)(p_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(p_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(p_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- (hkl) Plane
			case(3):{
				pref_l_width->set_float_val(h_Width[show_pref_ID]);
				pref_opacity->set_int_val((int)(h_Trans[show_pref_ID]*100));
				pref_color_R->set_int_val((int)(h_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(h_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(h_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- Bond
			case(4):{
				pref_l_width->set_float_val(b_Width[show_pref_ID]);
				pref_color_R->set_int_val((int)(b_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(b_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(b_color[show_pref_ID][2]*100));
				pref_bond_rad1->set_float_val(b_Radius[show_pref_ID][0]);
				pref_bond_rad2->set_float_val(b_Radius[show_pref_ID][1]);
				break;
					}
			}

		}
		break;
			}
//------------------------------------------------------ Show settings (default / current)
	case(10):{
		prefernces_Dialog_ID = -1;
		show_pref_ID = pref_show_type_radio->get_int_val();
		if (show_pref_ID == 0) {
			pref_set_default->disable();
			pref_save_default->set_name("Save");
		}
		else {
			pref_set_default->enable();
			pref_save_default->set_name("Save as defaults");
		}
		pref_atom_s_stack->set_int_val(a_Stick_s[show_pref_ID]);
		pref_atom_s_slice->set_int_val(a_Slice_s[show_pref_ID]);
		pref_bond_s_stack->set_int_val(b_Stick_s[show_pref_ID]);
		pref_bond_s_slice->set_int_val(b_Slice_s[show_pref_ID]);
		pref_atom_o_stack->set_int_val(a_Stick_o[show_pref_ID]);
		pref_atom_o_slice->set_int_val(a_Slice_o[show_pref_ID]);
		pref_bond_o_stack->set_int_val(b_Stick_o[show_pref_ID]);
		pref_bond_o_slice->set_int_val(b_Slice_o[show_pref_ID]);
		show_comp_check->set_int_val(compass[show_pref_ID]);
		show_comp_label_check->set_int_val(lcompass[show_pref_ID]);
		if (compass[show_pref_ID] == 0) show_comp_label_check->disable();
		else show_comp_label_check->enable();
		switch(object_ID)
		{
//------------------------------------------------- Background
		case(0):{
			pref_color_R->set_int_val((int)(bkg_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(bkg_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(bkg_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- Unit Cell
		case(1):{
			pref_l_width->set_float_val(u_Width[show_pref_ID]);
			pref_color_R->set_int_val((int)(u_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(u_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(u_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- Polyhedra
		case(2):{
			pref_l_width->set_float_val(p_Width[show_pref_ID]);
			pref_opacity->set_int_val((int)(p_Trans[show_pref_ID]*100));
			pref_color_R->set_int_val((int)(p_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(p_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(p_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- (hkl) Plane
		case(3):{
			pref_l_width->set_float_val(h_Width[show_pref_ID]);
			pref_opacity->set_int_val((int)(h_Trans[show_pref_ID]*100));
			pref_color_R->set_int_val((int)(h_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(h_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(h_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- Bond
		case(4):{
			pref_l_width->set_float_val(b_Width[show_pref_ID]);
			pref_color_R->set_int_val((int)(b_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(b_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(b_color[show_pref_ID][2]*100));
			pref_bond_rad1->set_float_val(b_Radius[show_pref_ID][0]);
			pref_bond_rad2->set_float_val(b_Radius[show_pref_ID][1]);
			break;
				}
		}
		break;
			 }
//------------------------------------------------------ Save as default
	case(2):{
		prefernces_Dialog_ID = 2;
		if (show_pref_ID != 0){
		a_Stick_s[0] = a_Stick_s[1];
		a_Slice_s[0] = a_Slice_s[1];
		a_Stick_o[0] = a_Stick_o[1];
		a_Slice_o[0] = a_Slice_o[1];

		b_Stick_s[0] = b_Stick_s[1];
		b_Slice_s[0] = b_Slice_s[1];
		b_Stick_o[0] = b_Stick_o[1];
		b_Slice_o[0] = b_Slice_o[1];

		u_Width[0] = u_Width[1];
		p_Width[0] = p_Width[1];
		h_Width[0] = h_Width[1];
		b_Width[0] = b_Width[1];

		p_Trans[0] = p_Trans[1];
		h_Trans[0] = h_Trans[1];

		compass[0] = compass[1];
		for(i=0; i<3; i++){
			bkg_color[0][i] = bkg_color[1][i];
			u_color[0][i] = u_color[1][i];
			p_color[0][i] = p_color[1][i];
			h_color[0][i] = h_color[1][i];
			b_color[0][i] = b_color[1][i];
		}
		b_Radius[0][0] = b_Radius[1][0];
		b_Radius[0][1] = b_Radius[1][1];
		}
		
		if (show_pref_ID == 0){
			pref_atom_s_stack->set_int_val(a_Stick_s[show_pref_ID]);
			pref_atom_s_slice->set_int_val(a_Slice_s[show_pref_ID]);
			pref_bond_s_stack->set_int_val(b_Stick_s[show_pref_ID]);
			pref_bond_s_slice->set_int_val(b_Slice_s[show_pref_ID]);
			pref_atom_o_stack->set_int_val(a_Stick_o[show_pref_ID]);
			pref_atom_o_slice->set_int_val(a_Slice_o[show_pref_ID]);
			pref_bond_o_stack->set_int_val(b_Stick_o[show_pref_ID]);
			pref_bond_o_slice->set_int_val(b_Slice_o[show_pref_ID]);
			switch(object_ID)
			{
//------------------------------------------------- Background
			case(0):{
				pref_color_R->set_int_val((int)(bkg_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(bkg_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(bkg_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- Unit Cell
			case(1):{
				pref_l_width->set_float_val(u_Width[show_pref_ID]);
				pref_color_R->set_int_val((int)(u_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(u_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(u_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- Polyhedra
			case(2):{
				pref_l_width->set_float_val(p_Width[show_pref_ID]);
				pref_opacity->set_int_val((int)(p_Trans[show_pref_ID]*100));
				pref_color_R->set_int_val((int)(p_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(p_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(p_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- (hkl) Plane
			case(3):{
				pref_l_width->set_float_val(h_Width[show_pref_ID]);
				pref_opacity->set_int_val((int)(h_Trans[show_pref_ID]*100));
				pref_color_R->set_int_val((int)(h_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(h_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(h_color[show_pref_ID][2]*100));
				break;
					}
//------------------------------------------------- Bond
			case(4):{
				pref_l_width->set_float_val(b_Width[show_pref_ID]);
				pref_color_R->set_int_val((int)(b_color[show_pref_ID][0]*100));
				pref_color_G->set_int_val((int)(b_color[show_pref_ID][1]*100));
				pref_color_B->set_int_val((int)(b_color[show_pref_ID][2]*100));
				pref_bond_rad1->set_float_val(b_Radius[show_pref_ID][0]);
				pref_bond_rad2->set_float_val(b_Radius[show_pref_ID][1]);
				break;
					}
			}

		}
		break;
			}
//------------------------------------------------------ Select Object
	case(20):{
		prefernces_Dialog_ID = -1;
		object_ID = pref_object_radio->get_int_val();
		if (object_ID == 2) pref_colors_settings->set_name("Color (outline)");
		else pref_colors_settings->set_name("Color");
		switch(object_ID)
		{
//------------------------------------------------- Background
		case(0):{
			pref_l_width->disable();
			pref_opacity->disable();
			pref_colors_settings->enable();
			pref_bonds_r_settings->disable();
			pref_color_R->set_int_val((int)(bkg_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(bkg_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(bkg_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- Unit Cell
		case(1):{
			pref_l_width->enable();
			pref_opacity->disable();
			pref_colors_settings->enable();
			pref_bonds_r_settings->disable();
			pref_l_width->set_float_val(u_Width[show_pref_ID]);
			pref_color_R->set_int_val((int)(u_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(u_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(u_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- Polyhedra
		case(2):{
			pref_l_width->enable();
			pref_opacity->enable();
			pref_colors_settings->enable();
			pref_bonds_r_settings->disable();
			pref_l_width->set_float_val(p_Width[show_pref_ID]);
			pref_opacity->set_int_val((int)(p_Trans[show_pref_ID]*100));
			pref_color_R->set_int_val((int)(p_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(p_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(p_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- (hkl) Plane
		case(3):{
			pref_l_width->enable();
			pref_opacity->enable();
			pref_colors_settings->enable();
			pref_bonds_r_settings->disable();
			pref_l_width->set_float_val(h_Width[show_pref_ID]);
			pref_opacity->set_int_val((int)(h_Trans[show_pref_ID]*100));
			pref_color_R->set_int_val((int)(h_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(h_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(h_color[show_pref_ID][2]*100));
			break;
				}
//------------------------------------------------- Bond
		case(4):{
			pref_l_width->enable();
			pref_opacity->disable();
			pref_colors_settings->enable();
			pref_bonds_r_settings->enable();
			pref_l_width->set_float_val(b_Width[show_pref_ID]);
			pref_color_R->set_int_val((int)(b_color[show_pref_ID][0]*100));
			pref_color_G->set_int_val((int)(b_color[show_pref_ID][1]*100));
			pref_color_B->set_int_val((int)(b_color[show_pref_ID][2]*100));
			pref_bond_rad1->set_float_val(b_Radius[show_pref_ID][0]);
			pref_bond_rad2->set_float_val(b_Radius[show_pref_ID][1]);
			break;
				}
		}
		break;
			 }
//------------------------------------------------------ Line Width
	case(30):{
		prefernces_Dialog_ID = -1;
		switch(object_ID){
		case(1):{u_Width[show_pref_ID] = pref_l_width->get_float_val();break;}   //--- Unit Cell
		case(2):{p_Width[show_pref_ID] = pref_l_width->get_float_val();break;}   //--- Poly Outline
		case(3):{h_Width[show_pref_ID] = pref_l_width->get_float_val();break;}   //--- (hkl) plane
		case(4):{b_Width[show_pref_ID] = pref_l_width->get_float_val();break;}
		}
		break;
			 }
//------------------------------------------------------ Opacity
	case(40):{
		prefernces_Dialog_ID = -1;
		switch(object_ID){
		case(2):{p_Trans[show_pref_ID] = (float)pref_opacity->get_int_val()/100;break;}
		case(3):{h_Trans[show_pref_ID] = (float)pref_opacity->get_int_val()/100;break;}
		}
		break;
			 }
//------------------------------------------------------ Colors
	case(50):{
		prefernces_Dialog_ID = -1;
		switch(object_ID){
		case(0):{                    //--- Background
			bkg_color[show_pref_ID][0] = (float)pref_color_R->get_int_val()/100;
			bkg_color[show_pref_ID][1] = (float)pref_color_G->get_int_val()/100;
			bkg_color[show_pref_ID][2] = (float)pref_color_B->get_int_val()/100;
			break;
				}
		case(1):{                    //--- Unit Cell Outline
			u_color[show_pref_ID][0] = (float)pref_color_R->get_int_val()/100;
			u_color[show_pref_ID][1] = (float)pref_color_G->get_int_val()/100;
			u_color[show_pref_ID][2] = (float)pref_color_B->get_int_val()/100;
			break;
				}
		case(2):{                    //--- Poly Outline
			p_color[show_pref_ID][0] = (float)pref_color_R->get_int_val()/100;
			p_color[show_pref_ID][1] = (float)pref_color_G->get_int_val()/100;
			p_color[show_pref_ID][2] = (float)pref_color_B->get_int_val()/100;
			break;
				}
		case(3):{                    //--- (hkl) Plane Outline
			h_color[show_pref_ID][0] = (float)pref_color_R->get_int_val()/100;
			h_color[show_pref_ID][1] = (float)pref_color_G->get_int_val()/100;
			h_color[show_pref_ID][2] = (float)pref_color_B->get_int_val()/100;
			break;
				}
		case(4):{                    //--- Bond
			b_color[show_pref_ID][0] = (float)pref_color_R->get_int_val()/100;
			b_color[show_pref_ID][1] = (float)pref_color_G->get_int_val()/100;
			b_color[show_pref_ID][2] = (float)pref_color_B->get_int_val()/100;
			break;
				}
		}
		break;
			 }
//------------------------------------------------------ Bond Radius
	case(60):{
		prefernces_Dialog_ID = -1;
		b_Radius[show_pref_ID][0] = pref_bond_rad1->get_float_val();
		b_Radius[show_pref_ID][1] = pref_bond_rad2->get_float_val();
		break;
			 }
//------------------------------------------------------ Bond Radius
	case(70):{
		prefernces_Dialog_ID = -1;
		a_Stick_s[show_pref_ID] = pref_atom_s_stack->get_int_val();
		a_Slice_s[show_pref_ID] = pref_atom_s_slice->get_int_val();
		b_Stick_s[show_pref_ID] = pref_bond_s_stack->get_int_val();
		b_Slice_s[show_pref_ID] = pref_bond_s_slice->get_int_val();
		a_Stick_o[show_pref_ID] = pref_atom_o_stack->get_int_val();
		a_Slice_o[show_pref_ID] = pref_atom_o_slice->get_int_val();
		b_Stick_o[show_pref_ID] = pref_bond_o_stack->get_int_val();
		b_Slice_o[show_pref_ID] = pref_bond_o_slice->get_int_val();
		break;
			 }
//------------------------------------------------------ Compass
	case(80):{
		prefernces_Dialog_ID = -1;
		compass[show_pref_ID] = show_comp_check->get_int_val();
		lcompass[show_pref_ID] = show_comp_label_check->get_int_val();
		if (compass[show_pref_ID] == 0) show_comp_label_check->disable();
		else show_comp_label_check->enable();
		break;
			 }
//------------------------------------------------------ Template
	case(90):{
		i = 0;
		j = open_templ_dialog(0, export_File_Name);
		if (j > 0) pref_temp_name->set_text(export_File_Name);
		break;
			 }
	}
}

//**************************************** set_preferences_cur_param() ******//
void set_preferences_cur_param(int a_St_s, int a_Sl_s, int a_St_o, int a_Sl_o,
							   int b_St_s, int b_Sl_s, int b_St_o, int b_Sl_o,
							   float u_w, float p_w, float h_w, float b_w,
							   float p_t, float h_t,
							   float bkg_c[3], float u_c[3], float p_c[3], float h_c[3], float b_c[3],
							   float b_r[2], int comp)
{
	int i;

	a_Stick_s[1] = a_St_s;
	a_Slice_s[1] = a_Sl_s;
	a_Stick_o[1] = a_St_o;
	a_Slice_o[1] = a_Sl_o;

	b_Stick_s[1] = b_St_s;
	b_Slice_s[1] = b_Sl_s;
	b_Stick_o[1] = b_St_o;
	b_Slice_o[1] = b_Sl_o;

	u_Width[1] = u_w;
	p_Width[1] = p_w;
	h_Width[1] = h_w;
	b_Width[1] = b_w;

	p_Trans[1] = p_t;
	h_Trans[1] = h_t;

	for(i=0; i<3; i++){
		bkg_color[1][i] = bkg_c[i];
		u_color[1][i] = u_c[i];
		p_color[1][i] = p_c[i];
		h_color[1][i] = h_c[i];
		b_color[1][i] = b_c[i];
	}
	b_Radius[1][0] = b_r[0];
	b_Radius[1][1] = b_r[1];

	switch(comp)
	{
	case(0):{
		compass[1] = 0;
		lcompass[1] = 0;
		break;
			}
	case(1):{
		compass[1] = 1;
		lcompass[1] = 1;
		break;
			}
	case(2):{
		compass[1] = 1;
		lcompass[1] = 0;
		break;
			}
	}
}


//**************************************** set_preferences_def_param() ******//
void set_preferences_def_param(int a_St_s, int a_Sl_s, int a_St_o, int a_Sl_o,
							   int b_St_s, int b_Sl_s, int b_St_o, int b_Sl_o,
							   float u_w, float p_w, float h_w, float b_w,
							   float p_t, float h_t,
							   float bkg_c[3], float u_c[3], float p_c[3], float h_c[3], float b_c[3],
							   float b_r[2], int comp, float stw_step, float bond, int startup_S,
							   int format_ID, char *export_fileN,int pos, int dpos)
{
	int i;

	a_Stick_s[0] = a_St_s;
	a_Slice_s[0] = a_Sl_s;
	a_Stick_o[0] = a_St_o;
	a_Slice_o[0] = a_Sl_o;

	b_Stick_s[0] = b_St_s;
	b_Slice_s[0] = b_Sl_s;
	b_Stick_o[0] = b_St_o;
	b_Slice_o[0] = b_Sl_o;


	u_Width[0] = u_w;
	p_Width[0] = p_w;
	h_Width[0] = h_w;
	b_Width[0] = b_w;

	p_Trans[0] = p_t;
	h_Trans[0] = h_t;

	for(i=0; i<3; i++){
		bkg_color[0][i] = bkg_c[i];
		u_color[0][i] = u_c[i];
		p_color[0][i] = p_c[i];
		h_color[0][i] = h_c[i];
		b_color[0][i] = b_c[i];
	}
	b_Radius[0][0] = b_r[0];
	b_Radius[0][1] = b_r[1];

	switch(comp)
	{
	case(0):{
		compass[0] = 0;
		lcompass[0] = 0;
		break;
			}
	case(1):{
		compass[0] = 1;
		lcompass[0] = 1;
		break;
			}
	case(2):{
		compass[0] = 1;
		lcompass[0] = 0;
		break;
			}
	}

	stw_angle = stw_step;
	bond_dist = bond;
	startup_search = startup_S;
	file_format = format_ID;

	sprintf(export_File_Name,"");
	sprintf(export_File_Name,"%s",export_fileN);

	save_win_pos = pos;
	save_dialog_pos = dpos;
}

//**************************************** init_preferences_dialog() ********//
void init_preferences_dialog()
{
	object_ID = 0;
	show_pref_ID = 1;

	pref_atom_s_stack->set_int_val(a_Stick_s[show_pref_ID]);
	pref_atom_s_slice->set_int_val(a_Slice_s[show_pref_ID]);
	pref_bond_s_stack->set_int_val(b_Stick_s[show_pref_ID]);
	pref_bond_s_slice->set_int_val(b_Slice_s[show_pref_ID]);
	pref_atom_o_stack->set_int_val(a_Stick_o[show_pref_ID]);
	pref_atom_o_slice->set_int_val(a_Slice_o[show_pref_ID]);
	pref_bond_o_stack->set_int_val(b_Stick_o[show_pref_ID]);
	pref_bond_o_slice->set_int_val(b_Slice_o[show_pref_ID]);

	pref_l_width->set_float_val(p_Width[show_pref_ID]);
	pref_opacity->set_int_val((int)(p_Trans[show_pref_ID]*100));
	pref_color_R->set_int_val((int)(bkg_color[show_pref_ID][0]*100));
	pref_color_G->set_int_val((int)(bkg_color[show_pref_ID][1]*100));
	pref_color_B->set_int_val((int)(bkg_color[show_pref_ID][2]*100));

	pref_bond_rad1->set_float_val(b_Radius[show_pref_ID][0]);
	pref_bond_rad2->set_float_val(b_Radius[show_pref_ID][1]);

	pref_object_radio->set_int_val(object_ID);
	pref_show_type_radio->set_int_val(show_pref_ID);

	pref_l_width->disable();
	pref_opacity->disable();
	pref_colors_settings->enable();
	pref_bonds_r_settings->disable();

	show_comp_check->set_int_val(compass[show_pref_ID]);
	show_comp_label_check->set_int_val(lcompass[show_pref_ID]);
	if (compass[0] == 0) show_comp_label_check->disable();
	else show_comp_label_check->enable();

	pref_stw_step->set_float_val(stw_angle);
	pref_max_bond->set_float_val(bond_dist);

	startup_search_check->set_int_val(startup_search);
	pref_format_lst->set_int_val(file_format);

	pref_temp_name->set_text(export_File_Name);

	save_pos_check->set_int_val(save_win_pos);
	save_dialog_check->set_int_val(save_dialog_pos);
}


//**************************************** open_preferences_dialog() ********//
void open_preferences_dialog(int ID, int dpX, int dpY)
{
	prefernces_Dialog_ID = -1;

	prefglui = GLUI_Master.create_glui("Preferences",0,dpX,dpY );
	prefglui->set_main_gfx_window( ID );

	pref_main_panel = prefglui->add_panel("",GLUI_PANEL_RAISED);
	pref_main_panel->set_alignment(GLUI_ALIGN_LEFT);
//--------------------------------------------------------------------------
	pref_template_panel = prefglui->add_panel_to_panel(pref_main_panel,"Template file for *.ins",GLUI_PANEL_EMBOSSED);
	pref_temp_name = prefglui->add_edittext_to_panel(pref_template_panel,"File:",GLUI_EDITTEXT_TEXT);
	pref_temp_name->set_w(260);
	prefglui->add_button_to_panel(pref_template_panel,"Browse", 90, preferences_control);

//--------------------------------------------------------------------------
	pref_objres_panel = prefglui->add_panel_to_panel(pref_main_panel,"Object resolution",GLUI_PANEL_EMBOSSED);
	pref_screenres_panel = prefglui->add_panel_to_panel(pref_objres_panel,"Screen",GLUI_PANEL_EMBOSSED);
	pref_atom_s_stack = prefglui->add_edittext_to_panel(pref_screenres_panel,"Atom stacks:",GLUI_EDITTEXT_INT,
		                                                                 NULL,70,preferences_control);
	pref_atom_s_slice = prefglui->add_edittext_to_panel(pref_screenres_panel,"Atom slices:",GLUI_EDITTEXT_INT,
		                                                                 NULL,70,preferences_control);
	prefglui->add_separator_to_panel(pref_screenres_panel);
	pref_bond_s_stack = prefglui->add_edittext_to_panel(pref_screenres_panel,"Bond stacks:",GLUI_EDITTEXT_INT,
		                                                                 NULL,70,preferences_control);
	pref_bond_s_slice = prefglui->add_edittext_to_panel(pref_screenres_panel,"Bond slices:",GLUI_EDITTEXT_INT,
		                                                                 NULL,70,preferences_control);
	pref_atom_s_stack->set_w(70);pref_atom_s_stack->set_alignment(GLUI_ALIGN_RIGHT);
	pref_atom_s_slice->set_w(70);pref_atom_s_slice->set_alignment(GLUI_ALIGN_RIGHT);
	pref_bond_s_stack->set_w(70);pref_bond_s_stack->set_alignment(GLUI_ALIGN_RIGHT);
	pref_bond_s_slice->set_w(70);pref_bond_s_slice->set_alignment(GLUI_ALIGN_RIGHT);

	prefglui->add_column_to_panel(pref_objres_panel,false);

	pref_outputres_panel = prefglui->add_panel_to_panel(pref_objres_panel,"Graphics files",GLUI_PANEL_EMBOSSED);
	pref_atom_o_stack = prefglui->add_edittext_to_panel(pref_outputres_panel,"Atom stacks:",GLUI_EDITTEXT_INT,
		                                                                  NULL,70,preferences_control);
	pref_atom_o_slice = prefglui->add_edittext_to_panel(pref_outputres_panel,"Atom slices:",GLUI_EDITTEXT_INT,
		                                                                  NULL,70,preferences_control);
	prefglui->add_separator_to_panel(pref_outputres_panel);
	pref_bond_o_stack = prefglui->add_edittext_to_panel(pref_outputres_panel,"Bond stacks:",GLUI_EDITTEXT_INT,
		                                                                  NULL,70,preferences_control);
	pref_bond_o_slice = prefglui->add_edittext_to_panel(pref_outputres_panel,"Bond slices:",GLUI_EDITTEXT_INT,
		                                                                  NULL,70,preferences_control);
	pref_atom_o_stack->set_w(70);pref_atom_o_stack->set_alignment(GLUI_ALIGN_RIGHT);
	pref_atom_o_slice->set_w(70);pref_atom_o_slice->set_alignment(GLUI_ALIGN_RIGHT);
	pref_bond_o_stack->set_w(70);pref_bond_o_stack->set_alignment(GLUI_ALIGN_RIGHT);
	pref_bond_o_slice->set_w(70);pref_bond_o_slice->set_alignment(GLUI_ALIGN_RIGHT);
//--------------------------------------------------------------------------
	pref_show_type_radio = prefglui->add_radiogroup_to_panel(pref_main_panel,NULL,10,preferences_control);
	prefglui->add_radiobutton_to_group(pref_show_type_radio,"Show default settings");
	prefglui->add_radiobutton_to_group(pref_show_type_radio,"Show current settings");
//--------------------------------------------------------------------------
	prefglui->add_column_to_panel(pref_main_panel,false);
//--------------------------------------------------------------------------
	pref_object_settings = prefglui->add_panel_to_panel(pref_main_panel,"Settings",GLUI_PANEL_EMBOSSED);
	pref_object_settings->set_alignment(GLUI_ALIGN_LEFT);
	pref_object_radio = prefglui->add_radiogroup_to_panel(pref_object_settings,NULL,20,preferences_control);
	prefglui->add_radiobutton_to_group(pref_object_radio,"Background");
	prefglui->add_radiobutton_to_group(pref_object_radio,"Unit cell");
	prefglui->add_radiobutton_to_group(pref_object_radio,"Polyhedra");
	prefglui->add_radiobutton_to_group(pref_object_radio,"Lattice plane");
	prefglui->add_radiobutton_to_group(pref_object_radio,"Bonds");

//--------------------------------------------------------------------------
	prefglui->add_separator_to_panel(pref_object_settings);
	show_comp_check = prefglui->add_checkbox_to_panel(pref_object_settings,"Show compass",NULL,80,preferences_control);
	show_comp_label_check = prefglui->add_checkbox_to_panel(pref_object_settings,"Show axis labels",NULL,80,preferences_control);

	prefglui->add_separator_to_panel(pref_object_settings);
	startup_search_check = prefglui->add_checkbox_to_panel(pref_object_settings,"Startup search for bonds",NULL);
	pref_max_bond =  prefglui->add_edittext_to_panel(pref_object_settings,"Max. distance: ",GLUI_EDITTEXT_FLOAT);
	pref_max_bond->set_w(80);
	pref_max_bond->set_alignment(GLUI_ALIGN_RIGHT);

	prefglui->add_separator_to_panel(pref_object_settings);

	pref_stw_step = prefglui->add_edittext_to_panel(pref_object_settings,"Rotation step: ",GLUI_EDITTEXT_FLOAT);
	pref_stw_step->set_w(80);
	pref_stw_step->set_alignment(GLUI_ALIGN_RIGHT);

	prefglui->add_separator_to_panel(pref_object_settings);
	
	pref_format_lst = prefglui->add_listbox_to_panel(pref_object_settings,"Default ext.: ",NULL);
	pref_format_lst->set_alignment(GLUI_ALIGN_RIGHT);
	pref_format_lst->add_item(0,"vcs");
	pref_format_lst->add_item(1,"amc");
	pref_format_lst->add_item(2,"cc1, cc2");
	pref_format_lst->add_item(3,"cif");
	pref_format_lst->add_item(4,"cmt");
	pref_format_lst->add_item(5,"cry");
	pref_format_lst->add_item(6,"cssr, css");
	pref_format_lst->add_item(7,"fdt, csd");
	pref_format_lst->add_item(8,"cube, cub");
	pref_format_lst->add_item(9,"ics");
	pref_format_lst->add_item(10,"mol");
	pref_format_lst->add_item(11,"min");
	pref_format_lst->add_item(12,"mld");
	pref_format_lst->add_item(13,"pdb");
	pref_format_lst->add_item(14,"ins");
	pref_format_lst->add_item(15,"struct");
	pref_format_lst->add_item(16,"xyz");
	pref_format_lst->add_item(17,"asse");
	pref_format_lst->add_item(18,"(f01)");
	pref_format_lst->add_item(19,"(file07.dat)");
//--------------------------------------------------------------------------
	prefglui->add_column_to_panel(pref_object_settings,true);

//--------------------------------------------------------------------------
	pref_l_width = prefglui->add_edittext_to_panel(pref_object_settings,"Line width: ",GLUI_EDITTEXT_FLOAT,
		                                                                      NULL,30,preferences_control);
	pref_l_width->set_alignment(GLUI_ALIGN_LEFT);

	pref_opacity = prefglui->add_spinner_to_panel(pref_object_settings,"Opacity (%): ",GLUI_SPINNER_INT,
		                                                                      NULL,40,preferences_control);
	pref_opacity->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	pref_opacity->set_alignment(GLUI_ALIGN_LEFT);

	pref_colors_settings = prefglui->add_panel_to_panel(pref_object_settings,"Color",GLUI_PANEL_EMBOSSED);
	pref_colors_settings->set_alignment(GLUI_ALIGN_LEFT);
	pref_color_R = prefglui->add_spinner_to_panel(pref_colors_settings,"Red (%): ",GLUI_SPINNER_INT,
		                                                                      NULL,50,preferences_control);
	pref_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	pref_color_G = prefglui->add_spinner_to_panel(pref_colors_settings," Green (%): ",GLUI_SPINNER_INT,
		                                                                      NULL,50,preferences_control);
	pref_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	pref_color_B = prefglui->add_spinner_to_panel(pref_colors_settings," Blue (%): ",GLUI_SPINNER_INT,
		                                                                      NULL,50,preferences_control);
	pref_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	pref_color_R->set_alignment(GLUI_ALIGN_LEFT);
	pref_color_G->set_alignment(GLUI_ALIGN_LEFT);
	pref_color_B->set_alignment(GLUI_ALIGN_LEFT);

	pref_bonds_r_settings = prefglui->add_panel_to_panel(pref_object_settings,"Bond radius",GLUI_PANEL_EMBOSSED);
	pref_bonds_r_settings->set_alignment(GLUI_ALIGN_LEFT);
	pref_bond_rad1 = prefglui->add_edittext_to_panel(pref_bonds_r_settings,"B&S, poly model: ",GLUI_EDITTEXT_FLOAT,
		                                                                    NULL,60,preferences_control);
	pref_bond_rad1->set_alignment(GLUI_ALIGN_RIGHT);
	pref_bond_rad2 = prefglui->add_edittext_to_panel(pref_bonds_r_settings,"Stick model: ",GLUI_EDITTEXT_FLOAT,
		                                                                    NULL,70,preferences_control);
	pref_bond_rad2->set_alignment(GLUI_ALIGN_RIGHT);
	pref_bond_rad1->set_w(70);
	pref_bond_rad2->set_w(70);

	prefglui->add_separator_to_panel(pref_object_settings);
	save_pos_check = prefglui->add_checkbox_to_panel(pref_object_settings,"Save bars/window positions",NULL);
	save_dialog_check = prefglui->add_checkbox_to_panel(pref_object_settings,"Save dialog bar positions",NULL);

//--------------------------------------------------------------------------
	pref_btn_panel = prefglui->add_panel("",GLUI_PANEL_NONE);

	pref_set_default = prefglui->add_button_to_panel(pref_btn_panel,"Restore default settings", 1, preferences_control);
	prefglui->add_column_to_panel(pref_btn_panel,false);
	pref_save_default = prefglui->add_button_to_panel(pref_btn_panel,"Save as defaults", 2, preferences_control);
	pref_set_default->set_w(150);
	pref_save_default->set_w(150);
	prefglui->add_column_to_panel(pref_btn_panel,true);
	pref_OK_btn = prefglui->add_button_to_panel(pref_btn_panel,"OK", 0, preferences_control);

	init_preferences_dialog();
}
