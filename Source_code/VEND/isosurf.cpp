//***************************************************************************//
//                        ISOSURFACE CONTROL DIALOG                          //
//---------------------------------------------------------------------------//
// File: isosurf.cpp                                                         //
// Date: 2002.02.22                                                          //
//***************************************************************************//

#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *isoglui;                                  //--- GLUI window
GLUI_Panel *iso_main_panel;                     //--- ISOSURFACE Main Dialog Panel
GLUI_Panel *iso_main_panel1;                     //--- ISOSURFACE Main Dialog Panel
GLUI_Panel *iso_btn_panel;                      //--- ISOSURFACE Button Panel (OK / Cancel)
GLUI_EditText *iso_level_txt;                   //--- ISOSURFACE Level
GLUI_EditText *sec_level_txt;                   //--- ISOSURFACE Scale Level

GLUI_RadioGroup *make_range_group;              //--- ISOSURFACE Boundaries ID (0 - Box; 1 - Cell)

GLUI_Panel *iso_range_panel;                    //--- ISOSURFACE Plot range Panel (Cell)
GLUI_EditText *range_x_min, *range_x_max;       //--- ISOSURFACE Plot range
GLUI_EditText *range_y_min, *range_y_max;       //--- 
GLUI_EditText *range_z_min, *range_z_max;       //--- 
GLUI_Button *apply_btn;                         //--- ISOSURFACE "Apply" Button

GLUI_Panel *iso_box_panel;                      //--- ISOSURFACE Plot range Panel (Box)
GLUI_Spinner *iso_box_a;                        //--- ISOSURFACE Number of Boxex along A -direction
GLUI_Spinner *iso_box_b;                        //--- ISOSURFACE Number of Boxex along B -direction
GLUI_Spinner *iso_box_c;                        //--- ISOSURFACE Number of Boxex along C -direction

GLUI_Panel *iso_surf_panel;                     //--- ISOSURFACE Settings Panel (Box)
GLUI_RadioGroup *iso_p_m_group;                //--- ISOSURFACE (+/-) 

GLUI_Panel *iso_origin_panel;                   //--- ISOSURFACE Origin Panel
GLUI_Spinner *iso_origin_a;                     //--- ISOSURFACE Origin
GLUI_Spinner *iso_origin_b;                     //--- 
GLUI_Spinner *iso_origin_c;                     //--- 

GLUI_Panel *iso_material_panel;                 //--- ISOSURFACE Material Properties Panel
GLUI_RadioGroup *iso_color_group;               //--- ISOSURFACE Color Control
GLUI_Spinner *iso_color_R;                      //--- ISOSURFACE Colors (RGB)
GLUI_Spinner *iso_color_G;
GLUI_Spinner *iso_color_B;

GLUI_Spinner *iso_mat_opacity;                  //--- ISOSURFACE Material Opacity
GLUI_EditText *iso_mat_shinin;                  //--- ISOSURFACE Material Shininess

GLUI_Checkbox *iso_prev_check;                  //--- ISOSURFACE Preview Control

GLUI_Button *iso_OK_btn;
GLUI_Button *iso_Cancel_btn;

//--- Control
int iso_surface_Dialog_ID;
int old_make_range_ID,make_range_ID;
int color_ID;
float level_min, level_max;                     //--- Min. and Max. Isosurface level

float isolevel,old_isolevel;                    //--- ISOSURFACE Level
float seclevel,old_seclevel;                    //--- ISOSURFACE Intensity Level
int   isobox[3],old_isobox[3];
float isorange[6],old_isorange[6];              //--- ISOSURFACE Plot Range
float isoorig[3],old_isoorig[3];                //--- ISOSURFACE Origin
float isoamb[4],old_isoamb[4];                  //--- ISOSURFACE Ambient Colors
float isodif[4],old_isodif[4];                  //--- ISOSURFACE Diffuse Colors
float isospe[4],old_isospe[4];                  //--- ISOSURFACE Specular Colors
float isoemi[4],old_isoemi[4];                  //--- ISOSURFACE Emission Colors
float isoshin,old_isoshin;                      //--- ISOSURFACE Shininess
int iso_type, iso_type_ID;

//--- Color Palette
GLUI_Button *icolor_palette_btn;
int iso_parent_window;
int iso_palette_ID;
float iRGB[3];


/**************************************** get_about_dialog_pos() ************/
void get_iso_dialog_pos(int &x, int &y)
{
	glutSetWindow(isoglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  set_isosurface_dialog_id()  *************************//
void set_isosurface_dialog_id(int ID)
{
	iso_surface_Dialog_ID = ID;
}


//********************  get_isosurface_dialog_id()  *************************//
int get_isosurface_dialog_id()
{
	return(iso_surface_Dialog_ID);
}

//********************  get_isosurface_pm_type   ****************************//
int get_isosurface_pm_type()
{
	return(iso_p_m_group->get_int_val());
}

//********************  get_isosurface_colors()  ****************************//
void get_isosurface_colors(float RGB[3])
{
	RGB[0] = iRGB[0];
	RGB[1] = iRGB[1];
	RGB[2] = iRGB[2];
}

//********************  set_isosurface_colors()  ****************************//
void set_isosurface_colors(float RGB[3])
{
	iRGB[0] = RGB[0];
	iRGB[1] = RGB[1];
	iRGB[2] = RGB[2];
	iso_color_R->set_int_val((int)(iRGB[0]*100));
	iso_color_G->set_int_val((int)(iRGB[1]*100));
	iso_color_B->set_int_val((int)(iRGB[2]*100));
	switch(color_ID)
	{
	case(0):{
		isoamb[0] = iRGB[0];
		isoamb[1] = iRGB[1];
		isoamb[2] = iRGB[2];
		break;
			}
	case(1):{
		isodif[0] = iRGB[0];
		isodif[1] = iRGB[1];
		isodif[2] = iRGB[2];
		break;
			}
	case(2):{
		isospe[0] = iRGB[0];
		isospe[1] = iRGB[1];
		isospe[2] = iRGB[2];
		break;
			}
	case(3):{
		isoemi[0] = iRGB[0];
		isoemi[1] = iRGB[1];
		isoemi[2] = iRGB[2];
		break;
			}
	}
	icolor_palette_btn->enable();
	iso_OK_btn->enable();
	iso_Cancel_btn->enable();
	iso_color_group->enable();
}



//********************  get_isosurface_preview_id()  ************************//
int get_isosurface_preview_id()
{
	return(iso_prev_check->get_int_val());
}

//********************  get_isosurface_material_param()  ********************//
void get_isosurface_material_param(float amb[4], float dif[4], float spec[4], float emis[4], float &shin)
{
	int i;

	for(i=0; i<4; i++){
		amb[i] = isoamb[i];
		dif[i] = isodif[i];
		spec[i] = isospe[i];
		emis[i] = isoemi[i];
	}
	shin = isoshin;
}

//********************  get_isosurface_plot_range()  ************************//
void get_isosurface_plot_range(float org[3], int &range_ID, int boxes[3], float range[6],float &level,float &scale)
{
	int i;
	for(i=0; i<3; i++) org[i] = isoorig[i];
	for(i=0; i<6; i++) range[i] = isorange[i];
	for(i=0; i<3; i++) boxes[i] = isobox[i];
	range_ID = make_range_ID; 
	level = isolevel;
	scale = seclevel;
}

//********************  isosurface_control_cb()  ****************************//
void isosurface_control_cb( int control )
{
	int i;

	switch(control)
	{
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(0):{
		isoglui->close();
		iso_surface_Dialog_ID = 0;
		break;
			}
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(1):{
		isoglui->close();
		iso_surface_Dialog_ID = 1;
		make_range_ID = old_make_range_ID;
		isolevel = old_isolevel;
		seclevel = old_seclevel;
		for(i=0; i<3; i++) isoorig[i] = old_isoorig[i];
		for (i=0; i<6; i++) {isorange[i] = old_isorange[i];}
		for (i=0; i<4; i++) {
			isoamb[i] = old_isoamb[i];
			isodif[i] = old_isodif[i];
			isospe[i] = old_isospe[i];
			isoemi[i] = old_isoemi[i];
		}
		for(i=0; i<3; i++) {isobox[i] = old_isobox[i];}
		isoshin = old_isoshin;
		break;
			}
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(10):{
		iso_surface_Dialog_ID = 10;
		isoorig[0] = iso_origin_a->get_float_val();
		isoorig[1] = iso_origin_b->get_float_val();
		isoorig[2] = iso_origin_c->get_float_val();

		isorange[0] = range_x_min->get_float_val();
		isorange[1] = range_x_max->get_float_val();
		isorange[2] = range_y_min->get_float_val();
		isorange[3] = range_y_max->get_float_val();
		isorange[4] = range_z_min->get_float_val();
		isorange[5] = range_z_max->get_float_val();
		isolevel = iso_level_txt->get_float_val();
		seclevel = sec_level_txt->get_float_val();
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(20):{
		iso_surface_Dialog_ID = -1;
		color_ID = iso_color_group->get_int_val();
		switch(color_ID)
		{
		case(0):{
			iso_color_R->set_int_val((int)(isoamb[0]*100));
			iso_color_G->set_int_val((int)(isoamb[1]*100));
			iso_color_B->set_int_val((int)(isoamb[2]*100));
			break;
				}
		case(1):{
			iso_color_R->set_int_val((int)(isodif[0]*100));
			iso_color_G->set_int_val((int)(isodif[1]*100));
			iso_color_B->set_int_val((int)(isodif[2]*100));
			break;
				}
		case(2):{
			iso_color_R->set_int_val((int)(isospe[0]*100));
			iso_color_G->set_int_val((int)(isospe[1]*100));
			iso_color_B->set_int_val((int)(isospe[2]*100));
			break;
				}
		case(3):{
			iso_color_R->set_int_val((int)(isoemi[0]*100));
			iso_color_G->set_int_val((int)(isoemi[1]*100));
			iso_color_B->set_int_val((int)(isoemi[2]*100));
			break;
				}
		}
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(30):{
		iso_surface_Dialog_ID = 100;
		switch(color_ID)
		{
		case(0):{
			isoamb[0] = (float)iso_color_R->get_int_val()/100;
			isoamb[1] = (float)iso_color_G->get_int_val()/100;
			isoamb[2] = (float)iso_color_B->get_int_val()/100;
			break;
				}
		case(1):{
			isodif[0] = (float)iso_color_R->get_int_val()/100;
			isodif[1] = (float)iso_color_G->get_int_val()/100;
			isodif[2] = (float)iso_color_B->get_int_val()/100;
			break;
				}
		case(2):{
			isospe[0] = (float)iso_color_R->get_int_val()/100;
			isospe[1] = (float)iso_color_G->get_int_val()/100;
			isospe[2] = (float)iso_color_B->get_int_val()/100;
			break;
				}
		case(3):{
			isoemi[0] = (float)iso_color_R->get_int_val()/100;
			isoemi[1] = (float)iso_color_G->get_int_val()/100;
			isoemi[2] = (float)iso_color_B->get_int_val()/100;
			break;
				}
		}
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(40):{
		iso_surface_Dialog_ID = 100;
		isodif[3] = (float)iso_mat_opacity->get_int_val()/100;
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(50):{
		iso_surface_Dialog_ID = 100;
		isoshin = (float)iso_mat_shinin->get_float_val();
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(60):{
		iso_surface_Dialog_ID = -1;
		make_range_ID = make_range_group->get_int_val();
		switch(make_range_ID)
		{
		case(0):{
			iso_origin_panel->enable();
			iso_range_panel->disable();
			isorange[0] = isorange[2] = isorange[4] = 0;
			isorange[1] = isorange[3] = isorange[5] = 1;
			range_x_min->set_float_val(isorange[0]);
			range_y_min->set_float_val(isorange[2]);
			range_z_min->set_float_val(isorange[4]);
			range_x_max->set_float_val(isorange[1]);
			range_y_max->set_float_val(isorange[3]);
			range_z_max->set_float_val(isorange[5]);
			break;
				}
		case(1):{
			iso_origin_panel->disable();
			iso_range_panel->enable();
			isoorig[0] = isoorig[1] = isoorig[2] = 0;
			iso_origin_a->set_float_val(isoorig[0]);
			iso_origin_b->set_float_val(isoorig[1]);
			iso_origin_c->set_float_val(isoorig[2]);
			range_x_min->enable();range_x_max->enable();
			range_y_min->enable();range_y_max->enable();
			range_z_min->enable();range_z_max->enable();
			if (isobox[0] > 1) {range_x_min->disable();range_x_max->disable();}
			if (isobox[1] > 1) {range_y_min->disable();range_y_max->disable();}
			if (isobox[2] > 1) {range_z_min->disable();range_z_max->disable();}
			break;
				}
		}
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(61):{
		iso_surface_Dialog_ID = 61;
		isobox[0] = iso_box_a->get_int_val();
		isobox[1] = iso_box_b->get_int_val();
		isobox[2] = iso_box_c->get_int_val();
		if (make_range_ID == 1) {

			range_x_min->enable();range_x_max->enable();
			range_y_min->enable();range_y_max->enable();
			range_z_min->enable();range_z_max->enable();
			if (isobox[0] > 1) {
				range_x_min->disable();range_x_max->disable();
				isorange[0] = 0;isorange[1] = 1;
				range_x_min->set_float_val(isorange[0]);
				range_x_max->set_float_val(isorange[1]);
			}
			if (isobox[1] > 1) {
				range_y_min->disable();range_y_max->disable();
				isorange[2] = 0;isorange[3] = 1;
				range_y_min->set_float_val(isorange[2]);
				range_y_max->set_float_val(isorange[3]);
			}
			if (isobox[2] > 1) {
				range_z_min->disable();range_z_max->disable();
				isorange[4] = 0;isorange[5] = 1;
				range_z_min->set_float_val(isorange[4]);
				range_z_max->set_float_val(isorange[5]);
			}
		}
		break;
			 }
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	case(70):{
		iso_surface_Dialog_ID = 200;
		iRGB[0] = (float)iso_color_R->get_int_val()/100;
		iRGB[1] = (float)iso_color_G->get_int_val()/100;
		iRGB[2] = (float)iso_color_B->get_int_val()/100;
		icolor_palette_btn->disable();
		iso_OK_btn->disable();
		iso_Cancel_btn->disable();
		iso_color_group->disable();
		break;
			 }
	}
}


//********************  set_isosurface_param()  *****************************//
void set_isosurface_param(float dmin, float dmax, float org[3], float level,float scale, 
						  int range_ID, int boxes[3], float range[6], 
						  float amb[4], float dif[4], float spec[4], float emis[4], 
						  float shin, int isot, int iso_ID)
{
	int i;

	level_min = dmin;
	level_max = dmax;

	iso_type = isot;
	iso_type_ID = iso_ID;
	for(i=0; i<3; i++) old_isoorig[i] = isoorig[i] = org[i];

	old_isolevel = isolevel = level;
	old_seclevel = seclevel = scale;
	old_make_range_ID = make_range_ID = range_ID;
	old_isobox[0] = isobox[0] = boxes[0]; 
	old_isobox[1] = isobox[1] = boxes[1]; 
	old_isobox[2] = isobox[2] = boxes[2]; 

	for (i=0; i<6; i++) {old_isorange[i] = isorange[i] = range[i];}
	for (i=0; i<4; i++) {
		old_isoamb[i] = isoamb[i] = amb[i];
		old_isodif[i] = isodif[i] = dif[i];
		old_isospe[i] = isospe[i] = spec[i];
		old_isoemi[i] = isoemi[i] = emis[i];
	}
	old_isoshin = isoshin = shin;
}

//********************  set_isosurface_param()  *****************************//
void init_isosurface_dialog()
{
	iso_origin_a->set_float_val(isoorig[0]);
	iso_origin_b->set_float_val(isoorig[1]);
	iso_origin_c->set_float_val(isoorig[2]);

	iso_level_txt->set_float_val(isolevel);
	iso_level_txt->set_float_limits(level_min,level_max,GLUI_LIMIT_CLAMP);
	sec_level_txt->set_float_val(seclevel);


	iso_box_a->set_int_val(isobox[0]);
	iso_box_b->set_int_val(isobox[1]);
	iso_box_c->set_int_val(isobox[2]);
	range_x_min->set_float_val(isorange[0]);
	range_y_min->set_float_val(isorange[2]);
	range_z_min->set_float_val(isorange[4]);
	range_x_max->set_float_val(isorange[1]);
	range_y_max->set_float_val(isorange[3]);
	range_z_max->set_float_val(isorange[5]);

	make_range_group->set_int_val(make_range_ID);
	if(make_range_ID == 0) {iso_origin_panel->enable();iso_range_panel->disable();}
	else {iso_origin_panel->disable();iso_range_panel->enable();}

	color_ID = 0;
	iso_color_group->set_int_val(color_ID);
	iso_color_R->set_int_val((int)(isoamb[0]*100));
	iso_color_G->set_int_val((int)(isoamb[1]*100));
	iso_color_B->set_int_val((int)(isoamb[2]*100));

	iso_mat_opacity->set_int_val((int)(isodif[3]*100));
	iso_mat_shinin->set_float_val(isoshin);

	iso_p_m_group->set_int_val(iso_type);
	if (iso_type_ID == 1) iso_p_m_group->enable();
	else iso_p_m_group->disable();
	iso_prev_check->set_int_val(1);

}

//********************  open_isosurface_dialog()  ***************************//
void open_isosurface_dialog(int ID, int dpX, int dpY)
{
	iso_surface_Dialog_ID = -1;
	iso_parent_window = ID;
	iso_palette_ID = 0;

	isoglui = GLUI_Master.create_glui("Isosurfaces",0,dpX,dpY );
	isoglui->set_main_gfx_window( ID );
	iso_main_panel = isoglui->add_panel("",GLUI_PANEL_RAISED);

	iso_box_panel = isoglui->add_panel_to_panel(iso_main_panel,"Number of cells",GLUI_PANEL_EMBOSSED);
	iso_box_panel->set_alignment(GLUI_ALIGN_LEFT);
	iso_box_a = isoglui->add_spinner_to_panel(iso_box_panel," a axis: ",GLUI_SPINNER_INT,NULL,61,isosurface_control_cb);
	iso_box_b = isoglui->add_spinner_to_panel(iso_box_panel," b axis: ",GLUI_SPINNER_INT,NULL,61,isosurface_control_cb);
	iso_box_c = isoglui->add_spinner_to_panel(iso_box_panel," c axis: ",GLUI_SPINNER_INT,NULL,61,isosurface_control_cb);
	iso_box_a->set_int_limits(1,10,GLUI_LIMIT_CLAMP);
	iso_box_b->set_int_limits(1,10,GLUI_LIMIT_CLAMP);
	iso_box_c->set_int_limits(1,10,GLUI_LIMIT_CLAMP);

	iso_surf_panel = isoglui->add_panel_to_panel(iso_main_panel,"",GLUI_PANEL_EMBOSSED);
	iso_level_txt = isoglui->add_edittext_to_panel(iso_surf_panel,"Isosurface level: ",GLUI_EDITTEXT_FLOAT);
	iso_level_txt->set_alignment(GLUI_ALIGN_RIGHT);
	iso_level_txt->set_w(60);
	sec_level_txt = isoglui->add_edittext_to_panel(iso_surf_panel,"f(section): ",GLUI_EDITTEXT_FLOAT);
	sec_level_txt->set_float_limits(0.000001,1,GLUI_LIMIT_CLAMP);
	sec_level_txt->set_alignment(GLUI_ALIGN_RIGHT);
	sec_level_txt->set_w(60);
	isoglui->add_separator_to_panel(iso_surf_panel);
	iso_p_m_group = isoglui->add_radiogroup_to_panel(iso_surf_panel,NULL);
	isoglui->add_radiobutton_to_group(iso_p_m_group,"Positive & negative data");
	isoglui->add_radiobutton_to_group(iso_p_m_group,"Positive data");
	isoglui->add_radiobutton_to_group(iso_p_m_group,"Negative data");

	apply_btn = isoglui->add_button_to_panel(iso_main_panel,"Apply", 10, isosurface_control_cb);


	isoglui->add_column_to_panel(iso_main_panel,false);

	make_range_group = isoglui->add_radiogroup_to_panel(iso_main_panel,NULL,60,isosurface_control_cb);
	isoglui->add_radiobutton_to_group(make_range_group,"Shift the origin");
	isoglui->add_radiobutton_to_group(make_range_group,"Change boundaries");


	iso_origin_panel = isoglui->add_panel_to_panel(iso_main_panel,"Origin",GLUI_PANEL_EMBOSSED);
	iso_origin_panel->set_alignment(GLUI_ALIGN_LEFT);
	iso_origin_a = isoglui->add_spinner_to_panel(iso_origin_panel," x: ",GLUI_SPINNER_FLOAT);
	iso_origin_b = isoglui->add_spinner_to_panel(iso_origin_panel," y: ",GLUI_SPINNER_FLOAT);
	iso_origin_c = isoglui->add_spinner_to_panel(iso_origin_panel," z: ",GLUI_SPINNER_FLOAT);
	iso_origin_a->set_float_limits(0,1,GLUI_LIMIT_CLAMP);
	iso_origin_b->set_float_limits(0,1,GLUI_LIMIT_CLAMP);
	iso_origin_c->set_float_limits(0,1,GLUI_LIMIT_CLAMP);

	iso_range_panel = isoglui->add_panel_to_panel(iso_main_panel,"Inside boundaries",GLUI_PANEL_EMBOSSED);
	iso_range_panel->set_alignment(GLUI_ALIGN_LEFT);

	range_x_min = isoglui->add_edittext_to_panel(iso_range_panel,"x(min): ",GLUI_EDITTEXT_FLOAT);
	range_x_min->set_float_limits(0,2,GLUI_LIMIT_CLAMP);
	range_x_min->set_w(60);
	range_y_min = isoglui->add_edittext_to_panel(iso_range_panel,"y(min): ",GLUI_EDITTEXT_FLOAT);
	range_y_min->set_float_limits(0,2,GLUI_LIMIT_CLAMP);
	range_y_min->set_w(60);
	range_z_min = isoglui->add_edittext_to_panel(iso_range_panel,"z(min): ",GLUI_EDITTEXT_FLOAT);
	range_z_min->set_float_limits(0,2,GLUI_LIMIT_CLAMP);
	range_z_min->set_w(60);

	isoglui->add_column_to_panel(iso_range_panel,false);

	range_x_max = isoglui->add_edittext_to_panel(iso_range_panel,"x(max): ",GLUI_EDITTEXT_FLOAT);
	range_x_max->set_float_limits(0,2,GLUI_LIMIT_CLAMP);
	range_x_max->set_w(60);
	range_y_max = isoglui->add_edittext_to_panel(iso_range_panel,"y(max): ",GLUI_EDITTEXT_FLOAT);
	range_y_max->set_float_limits(0,2,GLUI_LIMIT_CLAMP);
	range_y_max->set_w(60);
	range_z_max = isoglui->add_edittext_to_panel(iso_range_panel,"z(max): ",GLUI_EDITTEXT_FLOAT);
	range_z_max->set_float_limits(0,2,GLUI_LIMIT_CLAMP);
	range_z_max->set_w(60);

	iso_main_panel1 = isoglui->add_panel("",GLUI_PANEL_RAISED);

	iso_material_panel = isoglui->add_panel_to_panel(iso_main_panel1,"Color",GLUI_PANEL_EMBOSSED);
	iso_material_panel->set_alignment(GLUI_ALIGN_LEFT);

	iso_color_group = isoglui->add_radiogroup_to_panel(iso_material_panel,NULL,20,isosurface_control_cb);
	isoglui->add_radiobutton_to_group(iso_color_group,"Ambient");
	isoglui->add_radiobutton_to_group(iso_color_group,"Diffuse");
	isoglui->add_radiobutton_to_group(iso_color_group,"Specular");
	isoglui->add_radiobutton_to_group(iso_color_group,"Emission");

//	isoglui->add_separator_to_panel(iso_material_panel);
	isoglui->add_column_to_panel(iso_material_panel,false);

	iso_color_R = isoglui->add_spinner_to_panel(iso_material_panel," Red (%): ",GLUI_SPINNER_INT,
		                                                             NULL,30,isosurface_control_cb);
	iso_color_G = isoglui->add_spinner_to_panel(iso_material_panel," Green (%): ",GLUI_SPINNER_INT,
		                                                             NULL,30,isosurface_control_cb);
	iso_color_B = isoglui->add_spinner_to_panel(iso_material_panel," Blue (%): ",GLUI_SPINNER_INT,
		                                                             NULL,30,isosurface_control_cb);
	iso_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	iso_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	iso_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	iso_color_R->set_alignment(GLUI_ALIGN_RIGHT);
	iso_color_G->set_alignment(GLUI_ALIGN_RIGHT);
	iso_color_B->set_alignment(GLUI_ALIGN_RIGHT);

	isoglui->add_separator_to_panel(iso_material_panel);
	icolor_palette_btn = isoglui->add_button_to_panel(iso_material_panel,"Palette",70,isosurface_control_cb);

	isoglui->add_column_to_panel(iso_main_panel1,false);

	iso_mat_opacity = isoglui->add_spinner_to_panel(iso_main_panel1," Opacity (%): ",GLUI_SPINNER_INT,
		                                                             NULL,40,isosurface_control_cb);
	iso_mat_opacity->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	iso_mat_opacity->set_alignment(GLUI_ALIGN_RIGHT);
	iso_mat_opacity->set_w(170);

	iso_mat_shinin = isoglui->add_edittext_to_panel(iso_main_panel1,"Shininess: ",GLUI_EDITTEXT_FLOAT,
		                                                            NULL,50,isosurface_control_cb);
	iso_mat_shinin->set_float_limits(0,300,GLUI_LIMIT_CLAMP);
	iso_mat_shinin->set_alignment(GLUI_ALIGN_RIGHT);
	iso_mat_shinin->set_w(170);

	isoglui->add_separator_to_panel(iso_main_panel1);
	iso_prev_check = isoglui->add_checkbox_to_panel(iso_main_panel1,"Preview");

    
	iso_btn_panel = isoglui->add_panel("",GLUI_PANEL_NONE);
	iso_OK_btn = isoglui->add_button_to_panel(iso_btn_panel,"OK", 0, isosurface_control_cb);
	isoglui->add_column_to_panel(iso_btn_panel,false);
	iso_Cancel_btn = isoglui->add_button_to_panel(iso_btn_panel,"Cancel", 1, isosurface_control_cb);

	init_isosurface_dialog();
}


