//***************************************************************************//
//                        2D PROPERTIES DIALOG                               //
//---------------------------------------------------------------------------//
// File: slice.cpp                                                           //
// Date: 2002.03.11                                                          //
//***************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *prgglui;                                  //--- GLUI window
GLUI_Panel *prg_main_panel;                     //--- Main Dialog Panel
GLUI_Panel *prg_short_panel;                    //--- PROJECTION Shortcuts Panel
GLUI_Panel *prg_plane_panel;                    //--- PROJECTION Select Plane Panel
GLUI_Panel *prg_cell_panel;                     //--- PROJECTION Cell Panel
GLUI_Panel *prg_project_panel;                  //--- PROJECTION Projection Panel

GLUI_Panel *prg_hkl_panel;                      //--- PROJECTION (hkl) Plane Panel
GLUI_EditText *prg_h_txt;
GLUI_EditText *prg_k_txt;
GLUI_EditText *prg_l_txt;
GLUI_EditText *prg_orig_txt;



GLUI_Panel *prg_palette_panel;                  //--- PROJECTION Palette Panel
GLUI_Panel *prg_scale_panel;                    //--- PROJECTION Scale Panel

GLUI_RadioGroup *prg_plane_group;               //--- PROJECTION Plane Group
GLUI_RadioGroup *prg_cell_group;                //--- PROJECTION Cell Group
GLUI_RadioGroup *prg_palette_group;             //--- PROJECTION Palette Group
GLUI_RadioGroup *prg_stype_group;               //--- PROJECTION Scale Type Group

GLUI_Spinner *prg_project_spin_from;            //--- PROJECTION Projection "FROM" Position
GLUI_Spinner *prg_project_spin_to;              //--- PROJECTION Projection "TO" Position

GLUI_Spinner *prg_2Dlevel_spin;                 //--- PROJECTION SECLEV Scale
GLUI_Spinner *prg_3Dlevel_spin;                 //--- PROJECTION 3DSECLEV Scale

GLUI_Checkbox *prg_show_2D_win;                 //--- PROJECTION Show 2D-Projection Window
GLUI_Checkbox *prg_show_3D_win;                 //--- PROJECTION Show 3D-Projection Window
GLUI_Checkbox *prg_show_2D_grid;

GLUI_Panel *prg_cont_panel;                     //--- PROJECTION Contour map Panel
GLUI_Button *cont_btn;                          //--- PROJECTION Contour map settings
GLUI_Checkbox *prg_cont_show;                   //--- PROJECTION Show Contour Map

GLUI_Panel *prg_color_panel;                    //--- PROJECTION Outline color Panel
GLUI_Spinner *prg_color_R;                      //--- PROJECTION Outline colors
GLUI_Spinner *prg_color_G;
GLUI_Spinner *prg_color_B;
GLUI_Button *prg_color_palette_btn;

GLUI_EditText *prg_width_txt;                   //--- PROJECTION Outline width


GLUI_Button *prg_app_btn;
GLUI_Button *prg_OK_btn;




//--- Control
int prg_dialog_ID;
int prg_plane_ID = 0;
int prg_cell_ID = 0;
int prg_pal_ID = 0;

int prg_cont = 0;

int prg_show_2D_ID = 0;
int prg_show_3D_ID = 0;
int prg_grid = 0;

float p_projection[2];
int p_ind_hkl[3] = {1,0,0};
float p_orig;
float p_2Dint_scale = 1.0;
float p_3Dint_scale = 1.0;
float prg_RGB[3] = {1,1,1};
float prg_w = 1.0;

/**************************************** get_about_dialog_pos() ************/
void get_prj_dialog_pos(int &x, int &y)
{
	glutSetWindow(prgglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  set_prj_dialog_id()  ********************************//
void set_prj_dialog_id(int ID)
{
	if (ID == -2) {
		prg_dialog_ID = -1;
		prg_app_btn->enable();
		prg_OK_btn->enable();
		cont_btn->enable();
	}
	else prg_dialog_ID = ID;
}

//********************  get_prj_dialog_id()  ********************************//
int get_prj_dialog_id()
{
	return(prg_dialog_ID);
}

//********************  get_prj_colors()  ***********************************//
void get_prj_colors(float RGB[3])
{
	RGB[0] = prg_RGB[0];
	RGB[1] = prg_RGB[1];
	RGB[2] = prg_RGB[2];
}

//********************  set_prj_colors()  ***********************************//
void set_prj_colors(float RGB[3])
{
	prg_RGB[0] = RGB[0];
	prg_RGB[1] = RGB[1];
	prg_RGB[2] = RGB[2];
	prg_color_R->set_int_val((int)(prg_RGB[0]*100));
	prg_color_G->set_int_val((int)(prg_RGB[1]*100));
	prg_color_B->set_int_val((int)(prg_RGB[2]*100));

	prg_app_btn->enable();
	prg_OK_btn->enable();
	cont_btn->enable();
	prg_color_palette_btn->enable();
}





//********************  get_prj_dialog_settings()  **************************//
void get_prj_dialog_settings(int &type, int &dim, int &pal, int &cont, int &show2D, int &show3D,
							 float RGB[3], float &width, int hkl[3], int &showGrid)
{
	type = prg_plane_ID;
	dim = prg_cell_ID;
	pal = prg_pal_ID;
	cont = prg_cont;
	show2D = prg_show_2D_ID;
	show3D = prg_show_3D_ID;
	RGB[0] = prg_RGB[0];
	RGB[1] = prg_RGB[1];
	RGB[2] = prg_RGB[2];
	width = prg_w;
	p_ind_hkl[0] = prg_h_txt->get_int_val();
	p_ind_hkl[1] = prg_k_txt->get_int_val();
	p_ind_hkl[2] = prg_l_txt->get_int_val();
	hkl[0] = p_ind_hkl[0]; 
	hkl[1] = p_ind_hkl[1]; 
	hkl[2] = p_ind_hkl[2]; 
	prg_grid = prg_show_2D_grid->get_int_val();
	showGrid = prg_grid;

}


//********************  get_prj_dialog_param()  *****************************//
void get_prj_dialog_param(float projection[2], float &scale1, float &scale2, float &orig)
{
	scale1 = p_2Dint_scale;
	scale2 = p_3Dint_scale;
	projection[0] = p_projection[0];
	projection[1] = p_projection[1];
//	p_orig = prg_orig_txt->get_float_val();
	orig = p_orig;
}


//********************  prj_control_cb()  ***********************************//
void prj_control_cb( int control )
{
	switch(control)
	{
	case(0):{
		if (prg_plane_ID == 3) {
			p_ind_hkl[0] = prg_h_txt->get_int_val();
			p_ind_hkl[1] = prg_k_txt->get_int_val();
			p_ind_hkl[2] = prg_l_txt->get_int_val();
			if ((p_ind_hkl[0] == 0) && (p_ind_hkl[1] == 0) && (p_ind_hkl[2] == 0)) {
				p_ind_hkl[0] = 1; p_ind_hkl[1] = 0; p_ind_hkl[2] = 0;
				prg_plane_ID = 0;
			}
			if ((p_ind_hkl[0] != 0) && (p_ind_hkl[1] == 0) && (p_ind_hkl[2] == 0)) {
				p_ind_hkl[0] = 1; p_ind_hkl[1] = 0; p_ind_hkl[2] = 0;
				prg_plane_ID = 0;
			}
			if ((p_ind_hkl[0] == 0) && (p_ind_hkl[1] != 0) && (p_ind_hkl[2] == 0)) {
				p_ind_hkl[0] = 0; p_ind_hkl[1] = 1; p_ind_hkl[2] = 0;
				prg_plane_ID = 1;
			}
			if ((p_ind_hkl[0] == 0) && (p_ind_hkl[1] == 0) && (p_ind_hkl[2] != 0)) {
				p_ind_hkl[0] = 0; p_ind_hkl[1] = 0; p_ind_hkl[2] = 1;
				prg_plane_ID = 2;
			}
			prg_plane_group->set_int_val(prg_plane_ID );
			prg_h_txt->set_int_val(p_ind_hkl[0]);
			prg_k_txt->set_int_val(p_ind_hkl[1]);
			prg_l_txt->set_int_val(p_ind_hkl[2]);
			if (prg_plane_ID != 3) {
				prg_hkl_panel->disable();
				prg_cell_panel->enable();
				prg_project_panel->enable();
			}
		}
		prgglui->close();
		prg_dialog_ID = 1;
		break;
			}
	case(10):{
		prg_plane_ID = prg_plane_group->get_int_val();
		p_projection[0] = prg_project_spin_from->get_float_val();
		p_projection[1] = prg_project_spin_to->get_float_val();
		if (p_projection[1] < p_projection[0]){
			p_projection[1] = p_projection[0];
			prg_project_spin_to->set_float_val(p_projection[1]);
		}
		if (prg_plane_ID != 3) {
			prg_hkl_panel->disable();
			prg_cell_panel->enable();
			prg_project_panel->enable();
			switch(prg_plane_ID){
			case(0):{p_ind_hkl[0] = 1; p_ind_hkl[1] = 0; p_ind_hkl[2] = 0; break;}
			case(1):{p_ind_hkl[0] = 0; p_ind_hkl[1] = 1; p_ind_hkl[2] = 0; break;}
			case(2):{p_ind_hkl[0] = 0; p_ind_hkl[1] = 0; p_ind_hkl[2] = 1; break;}
			}
			prg_h_txt->set_int_val(p_ind_hkl[0]);
			prg_k_txt->set_int_val(p_ind_hkl[1]);
			prg_l_txt->set_int_val(p_ind_hkl[2]);
		}
		else {
			prg_hkl_panel->enable();
			prg_cell_panel->disable();
			prg_project_panel->disable();
			prg_cell_ID = 0;
			prg_cell_group->set_int_val(prg_cell_ID);
		}
		prg_dialog_ID = 10;
		break;
			 }
	case(14):{
		prg_dialog_ID = 31;
		break;
			 }
	case(15):{
		prg_app_btn->disable();
		prg_OK_btn->disable();
		cont_btn->disable();
		prg_dialog_ID = 30;
		break;
			 }
	case(20):{
		prg_plane_ID = prg_plane_group->get_int_val();
		prg_cell_ID = prg_cell_group->get_int_val();
		prg_pal_ID = prg_palette_group->get_int_val();
		p_projection[0] = prg_project_spin_from->get_float_val();
		p_projection[1] = prg_project_spin_to->get_float_val();
		if (p_projection[1] < p_projection[0]){
			p_projection[1] = p_projection[0];
			prg_project_spin_to->set_float_val(p_projection[1]);
		}
		prg_cont = prg_cont_show->get_int_val();
		prg_show_2D_ID = prg_show_2D_win->get_int_val();
		prg_show_3D_ID = prg_show_3D_win->get_int_val();
		p_2Dint_scale = (float)prg_2Dlevel_spin->get_int_val()/100;
		p_3Dint_scale = (float)prg_3Dlevel_spin->get_int_val()/100;

		prg_RGB[0] = (float)prg_color_R->get_int_val()/100;
		prg_RGB[1] = (float)prg_color_G->get_int_val()/100;
		prg_RGB[2] = (float)prg_color_B->get_int_val()/100;
		prg_w = prg_width_txt->get_float_val();

		if (prg_plane_ID == 3) {
			p_ind_hkl[0] = prg_h_txt->get_int_val();
			p_ind_hkl[1] = prg_k_txt->get_int_val();
			p_ind_hkl[2] = prg_l_txt->get_int_val();
			if ((p_ind_hkl[0] == 0) && (p_ind_hkl[1] == 0) && (p_ind_hkl[2] == 0)) {
				p_ind_hkl[0] = 1; p_ind_hkl[1] = 0; p_ind_hkl[2] = 0;
				prg_plane_ID = 0;
			}
			if ((p_ind_hkl[0] != 0) && (p_ind_hkl[1] == 0) && (p_ind_hkl[2] == 0)) {
				p_ind_hkl[0] = 1; p_ind_hkl[1] = 0; p_ind_hkl[2] = 0;
				prg_plane_ID = 0;
			}
			if ((p_ind_hkl[0] == 0) && (p_ind_hkl[1] != 0) && (p_ind_hkl[2] == 0)) {
				p_ind_hkl[0] = 0; p_ind_hkl[1] = 1; p_ind_hkl[2] = 0;
				prg_plane_ID = 1;
			}
			if ((p_ind_hkl[0] == 0) && (p_ind_hkl[1] == 0) && (p_ind_hkl[2] != 0)) {
				p_ind_hkl[0] = 0; p_ind_hkl[1] = 0; p_ind_hkl[2] = 1;
				prg_plane_ID = 2;
			}
			prg_plane_group->set_int_val(prg_plane_ID );
			prg_h_txt->set_int_val(p_ind_hkl[0]);
			prg_k_txt->set_int_val(p_ind_hkl[1]);
			prg_l_txt->set_int_val(p_ind_hkl[2]);
			if (prg_plane_ID != 3) {
				prg_hkl_panel->disable();
				prg_cell_panel->enable();
				prg_project_panel->enable();
			}
		}
		prg_dialog_ID = 20;
		break;
			 }
	case(30):{
		prg_show_2D_ID = prg_show_2D_win->get_int_val();
		if (prg_show_2D_ID == 0) {
			prg_grid = 0;
			prg_show_2D_grid->set_int_val(prg_grid);
			prg_show_2D_grid->disable();
		}
		else prg_show_2D_grid->enable();

		break;
			 }
	case(100):{
		prg_dialog_ID = 100;
		prg_RGB[0] = (float)prg_color_R->get_int_val()/100;
		prg_RGB[1] = (float)prg_color_G->get_int_val()/100;
		prg_RGB[2] = (float)prg_color_B->get_int_val()/100;
		prg_app_btn->disable();
		prg_OK_btn->disable();
		cont_btn->disable();
		prg_color_palette_btn->disable();
		break;
			  }
	}
}

//********************  set_prj_dialog_settings()  **************************//
void set_prj_dialog_settings(int type, int dim, int pal, int cont, int show2D, int show3D,
							 float RGB[3], float width, int hkl[3], int showGrid)
{
	prg_plane_ID = type;
	prg_cell_ID = dim;
	prg_pal_ID = pal;
	prg_show_2D_ID = show2D;
	prg_show_3D_ID = show3D;
	prg_cont = cont;
	prg_RGB[0] = RGB[0];
	prg_RGB[1] = RGB[1];
	prg_RGB[2] = RGB[2];
	prg_w = width;
	p_ind_hkl[0] = hkl[0];
	p_ind_hkl[1] = hkl[1];
	p_ind_hkl[2] = hkl[2];
	prg_grid = showGrid;
}

//********************  set_prj_dialog_param()  *****************************//
void set_prj_dialog_param(float projection[2],float scale1,float scale2, float orig)
{
	p_projection[0] = projection[0];
	p_projection[1] = projection[1];
	p_2Dint_scale = scale1;
	p_3Dint_scale = scale2;
	p_orig = orig;
}

//********************  init_prj_dialog()  **********************************//
void init_prj_dialog()
{
	prg_plane_group->set_int_val(prg_plane_ID);
	prg_cell_group->set_int_val(prg_cell_ID);
	prg_palette_group->set_int_val(prg_pal_ID);
	prg_show_2D_win->set_int_val(prg_show_2D_ID);
	prg_show_3D_win->set_int_val(prg_show_3D_ID);
	prg_show_2D_grid->set_int_val(prg_grid);
	if (prg_show_2D_ID == 0) {
		prg_grid = 0;
		prg_show_2D_grid->set_int_val(prg_grid);
		prg_show_2D_grid->disable();
	}
	else prg_show_2D_grid->enable();

	prg_project_spin_from->set_float_val(p_projection[0]);
	prg_project_spin_to->set_float_val(p_projection[1]);

	prg_cont_show->set_int_val(prg_cont);

	prg_2Dlevel_spin->set_int_val((int)(p_2Dint_scale*100));
	prg_3Dlevel_spin->set_int_val((int)(p_3Dint_scale*100));

	prg_color_R->set_int_val((int)(prg_RGB[0]*100));
	prg_color_G->set_int_val((int)(prg_RGB[1]*100));
	prg_color_B->set_int_val((int)(prg_RGB[2]*100));

	prg_width_txt->set_float_val(prg_w);

	if (prg_plane_ID != 3) {
		prg_hkl_panel->disable();
		prg_cell_panel->enable();
		prg_project_panel->enable();
	}
	else {
		prg_hkl_panel->enable();
		prg_cell_panel->disable();
		prg_project_panel->disable();
	}
	switch(prg_plane_ID){
	case(0):{p_ind_hkl[0] = 1; p_ind_hkl[1] = 0; p_ind_hkl[2] = 0; break;}
	case(1):{p_ind_hkl[0] = 0; p_ind_hkl[1] = 1; p_ind_hkl[2] = 0; break;}
	case(2):{p_ind_hkl[0] = 0; p_ind_hkl[1] = 0; p_ind_hkl[2] = 1; break;}
	}
	prg_h_txt->set_int_val(p_ind_hkl[0]);
	prg_k_txt->set_int_val(p_ind_hkl[1]);
	prg_l_txt->set_int_val(p_ind_hkl[2]);
//	prg_orig_txt->set_float_val(p_orig);

}


//********************  open_prj_dialog()  **********************************//
void open_prj_dialog(int ID, int dpX, int dpY)
{
	prg_dialog_ID = -1;
	prgglui = GLUI_Master.create_glui("2D Projection",0,dpX,dpY );
	prgglui->set_main_gfx_window( ID );
	prg_main_panel = prgglui->add_panel("",GLUI_PANEL_RAISED);


	prg_plane_panel = prgglui->add_panel_to_panel(prg_main_panel,"Plane",GLUI_PANEL_EMBOSSED);
	prg_plane_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_plane_group = prgglui->add_radiogroup_to_panel(prg_plane_panel,NULL,10,prj_control_cb);
	prgglui->add_radiobutton_to_group(prg_plane_group,"(100) plane");
	prgglui->add_radiobutton_to_group(prg_plane_group,"(010) plane");
	prgglui->add_radiobutton_to_group(prg_plane_group,"(001) plane");
	prgglui->add_radiobutton_to_group(prg_plane_group,"(hkl) plane");

//-----------------------------------------------------------------------------
	prg_cell_panel = prgglui->add_panel_to_panel(prg_main_panel,"Dimension",GLUI_PANEL_EMBOSSED);
	prg_cell_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_cell_group = prgglui->add_radiogroup_to_panel(prg_cell_panel,NULL);
	prgglui->add_radiobutton_to_group(prg_cell_group,"One unit cell");
	prgglui->add_radiobutton_to_group(prg_cell_group,"2 x 2 unit cells");

	prg_project_panel = prgglui->add_panel_to_panel(prg_main_panel,"Projection",GLUI_PANEL_EMBOSSED);
	prg_project_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_project_spin_from = prgglui->add_spinner_to_panel(prg_project_panel,"From: ",GLUI_SPINNER_FLOAT,NULL,10,prj_control_cb);
	prg_project_spin_to = prgglui->add_spinner_to_panel(prg_project_panel,"To: ",GLUI_SPINNER_FLOAT,NULL,10,prj_control_cb);
	prg_project_spin_from->set_float_limits(0,1,GLUI_LIMIT_CLAMP);
	prg_project_spin_to->set_float_limits(0,1,GLUI_LIMIT_CLAMP);
	prg_project_spin_from->set_alignment(GLUI_ALIGN_RIGHT);
	prg_project_spin_to->set_alignment(GLUI_ALIGN_RIGHT);
//-----------------------------------------------------------------------------
	prg_hkl_panel = prgglui->add_panel_to_panel(prg_main_panel,"(hkl) plane",GLUI_PANEL_EMBOSSED);
	
	prg_h_txt = prgglui->add_edittext_to_panel(prg_hkl_panel,"h = ",GLUI_EDITTEXT_INT);
	prg_k_txt = prgglui->add_edittext_to_panel(prg_hkl_panel,"k = ",GLUI_EDITTEXT_INT);
	prg_l_txt = prgglui->add_edittext_to_panel(prg_hkl_panel,"l = ",GLUI_EDITTEXT_INT);
//	prgglui->add_separator_to_panel(prg_hkl_panel);
//	prg_orig_txt = prgglui->add_edittext_to_panel(prg_hkl_panel,"Origin: ",GLUI_EDITTEXT_FLOAT);
	prg_h_txt->set_w(140);
	prg_k_txt->set_w(140);
	prg_l_txt->set_w(140);
//	prg_orig_txt->set_w(140);
//-----------------------------------------------------------------------------
	prgglui->add_column_to_panel(prg_main_panel,true);

	prg_scale_panel = prgglui->add_panel_to_panel(prg_main_panel,"Scale",GLUI_PANEL_EMBOSSED);
	prg_scale_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_2Dlevel_spin = prgglui->add_spinner_to_panel(prg_scale_panel,"2D map (%): ",GLUI_SPINNER_INT,NULL);
	prg_2Dlevel_spin->set_float_limits(1,100,GLUI_LIMIT_CLAMP);
	prg_2Dlevel_spin->set_alignment(GLUI_ALIGN_RIGHT);
	prg_3Dlevel_spin = prgglui->add_spinner_to_panel(prg_scale_panel,"Bird's-eye view (%): ",GLUI_SPINNER_INT,NULL);
	prg_3Dlevel_spin->set_int_limits(1,100,GLUI_LIMIT_CLAMP);
	prg_3Dlevel_spin->set_alignment(GLUI_ALIGN_RIGHT);

	prgglui->add_separator_to_panel(prg_main_panel);

	prg_color_panel = prgglui->add_panel_to_panel(prg_main_panel,"Outlines color",GLUI_PANEL_EMBOSSED);
	prg_color_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_color_R = prgglui->add_spinner_to_panel(prg_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL);
	prg_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	prg_color_G = prgglui->add_spinner_to_panel(prg_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL);
	prg_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	prg_color_B = prgglui->add_spinner_to_panel(prg_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL);
	prg_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	prgglui->add_separator_to_panel(prg_color_panel);
	prg_color_palette_btn = prgglui->add_button_to_panel(prg_color_panel,"Palette",100, prj_control_cb);

	prg_width_txt = prgglui->add_edittext_to_panel(prg_main_panel,"Outlines width: ",GLUI_EDITTEXT_FLOAT);
	prgglui->add_separator_to_panel(prg_main_panel);

	prg_palette_panel = prgglui->add_panel_to_panel(prg_main_panel,"Palette",GLUI_PANEL_EMBOSSED);
	prg_palette_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_palette_panel->set_w(200);
	prg_palette_group = prgglui->add_radiogroup_to_panel(prg_palette_panel,NULL);
	prgglui->add_radiobutton_to_group(prg_palette_group,"Rainbow");
	prgglui->add_radiobutton_to_group(prg_palette_group,"Invers Rainbow");
	prgglui->add_radiobutton_to_group(prg_palette_group,"Gray");


	prgglui->add_column_to_panel(prg_main_panel,true);

	prg_cont_panel = prgglui->add_panel_to_panel(prg_main_panel,"Contour lines",GLUI_PANEL_EMBOSSED);
	prg_cont_panel->set_alignment(GLUI_ALIGN_LEFT);
	prg_cont_show = prgglui->add_checkbox_to_panel(prg_cont_panel,"Show contour lines",NULL, 14, prj_control_cb);
	prgglui->add_separator_to_panel(prg_cont_panel);
	cont_btn = prgglui->add_button_to_panel(prg_cont_panel,"Settings", 15, prj_control_cb);

//	prgglui->add_column_to_panel(prg_main_panel,true);


	prgglui->add_separator_to_panel(prg_main_panel);
	prg_show_2D_win = prgglui->add_checkbox_to_panel(prg_main_panel,"Show 2D map",NULL, 30, prj_control_cb);
	prg_show_3D_win = prgglui->add_checkbox_to_panel(prg_main_panel,"Show bird's-eye view",NULL);
	prg_show_2D_win->set_alignment(GLUI_ALIGN_LEFT);
	prg_show_3D_win->set_alignment(GLUI_ALIGN_LEFT);
	prgglui->add_separator_to_panel(prg_main_panel);
	prg_show_2D_grid = prgglui->add_checkbox_to_panel(prg_main_panel,"Grid",NULL);

	prgglui->add_separator_to_panel(prg_main_panel);
	prg_app_btn = prgglui->add_button_to_panel(prg_main_panel,"Apply", 20, prj_control_cb);

	prgglui->add_separator_to_panel(prg_main_panel);

	prg_OK_btn = prgglui->add_button_to_panel(prg_main_panel,"Close", 0, prj_control_cb);

//	prg_OK_btn = prgglui->add_button("OK", 0, prj_control_cb);
	init_prj_dialog();
}



