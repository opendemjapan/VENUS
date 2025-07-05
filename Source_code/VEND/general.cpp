//**********************************************************************************//
//                            GENERAL OPTIONS                                       //
//----------------------------------------------------------------------------------//
// File: general.cpp                                                                //
// Date: 2001.12.18                                                                 //
//----------------------------------------------------------------------------------//
// Control:                                                                         //
//   control_ID_ID = -1 -- Open General Properties Dialog                           //
//                    1 -- Close Bonds Properties Dialog ("OK")                     //
//                                                                                  //
//**********************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *rend;                                //--- GLUI window

GLUI_Panel *rend_panel;                    //--- Main Panel
GLUI_Panel *lights_rend_panel;             //--- Lights rending Panel
GLUI_Panel *background_rend_panel;         //--- Background rending Panel

GLUI_Spinner *light_ambient;                 //--- Ligts Ambient
GLUI_Spinner *light_diffuse;                 //--- Ligts Diffuse
GLUI_Rotation *light_direction;              //--- Lights Direction
GLUI_Translation *light_position;            //--- Lights Position

GLUI_RadioGroup *gen_back_radio;            //--- Background ID 
GLUI_Spinner *backround_color_R;             //--- Background Red Color
GLUI_Spinner *backround_color_G;             //--- Background Green Color
GLUI_Spinner *backround_color_B;             //--- Background Blue Color

GLUI_Checkbox *gen_comp_check;               //--- Compass Checkbox;
GLUI_Checkbox *gen_comp_label_check;         //--- Compass labels Checkbox;

GLUI_Button *gen_OK_btn;

GLUI_Panel *projection_render_panel;         //--- Background Projection Panel
GLUI_RadioGroup *projection_tipe_radio;
GLUI_Spinner *projection_spin;



//--- Live Variabels
int	genral_dialog_ID;
int gen_bkg_ID;

//--- Global Variables
float lights_direction[16];                      
float lights_ambient;
float lights_diffuse;
float background_RGB[9];

int compass,lcompass;

//--- Color Palette
GLUI_Button *gcolor_palette_btn;
int gen_parent_window;
int gen_palette_ID;
float gRGB[3];

int rproj_type;
float rpersp;

/**************************************** get_about_dialog_pos() ************/
void get_general_dialog_pos(int &x, int &y)
{
	glutSetWindow(rend->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

//********************  set_genral_control_id()  ****************************//
void set_genral_control_id(int ID)
{
	genral_dialog_ID = ID;
}


//********************  get_genral_control_id()  ****************************//
int get_genral_control_id()
{
	return(genral_dialog_ID);
}


//********************  get_genral_colors()  ********************************//
void get_genral_colors(float RGB[3])
{
	RGB[0] = gRGB[0];
	RGB[1] = gRGB[1];
	RGB[2] = gRGB[2];
}

//********************  set_genral_colors()  ********************************//
void set_genral_colors(float RGB[3])
{
	background_RGB[0+gen_bkg_ID*3] = RGB[0];
	background_RGB[1+gen_bkg_ID*3] = RGB[1];
	background_RGB[2+gen_bkg_ID*3] = RGB[2];
	backround_color_R->set_int_val((int)(background_RGB[0+gen_bkg_ID*3]*100));
	backround_color_G->set_int_val((int)(background_RGB[1+gen_bkg_ID*3]*100));
	backround_color_B->set_int_val((int)(background_RGB[2+gen_bkg_ID*3]*100));
	gcolor_palette_btn->enable();
	gen_OK_btn->enable();
	gen_back_radio->enable();
}



//********************  get_projection_perspective()  ***********************//
void get_projection_perspective(int &proj_type, float &persp)
{
	proj_type = rproj_type;
	persp = rpersp;
}


//********************  get_lights_rendering()  *****************************//
void get_lights_rendering(float l_a[4], float l_d[4], float l_dir[16])
{
	int i;
	for(i=0; i<3; i++){
		l_a[i] = lights_ambient;
		l_d[i] = lights_diffuse;
	}
	for (i=0; i<16; i++) l_dir[i] = lights_direction[i];
}

//********************  get_background_rendering()  *************************//
void get_background_rendering(float RGB1[3],float RGB2[3],float RGB3[3])
{
	int i;
	for(i=0; i<3; i++) {
		RGB1[i] = background_RGB[i];
		RGB2[i] = background_RGB[i+3];
		RGB3[i] = background_RGB[i+6];
	}
}

//********************  get_compas_setting()  *******************************//
int get_compas_setting()
{
	if ((compass == 1) && (lcompass == 1)) return(1);
	if ((compass == 1) && (lcompass == 0)) return(2);
	return(0);
}

//********************  rend_control_cb()  ********************************//
void rend_control_cb( int control )
{
	switch (control)
	{
	case (0):{genral_dialog_ID = 1;rend->close();break;}
	case(10):{
		genral_dialog_ID = 10;
		lights_ambient = (float)light_ambient->get_int_val()/100;
		lights_diffuse = (float)light_diffuse->get_int_val()/100;
		break;
			 }
	case(20):{
		genral_dialog_ID = 20;
		background_RGB[0+gen_bkg_ID*3] = (float)backround_color_R->get_int_val()/100;
		background_RGB[1+gen_bkg_ID*3] = (float)backround_color_G->get_int_val()/100;
		background_RGB[2+gen_bkg_ID*3] = (float)backround_color_B->get_int_val()/100;
		break;
			 }
	case(30):{
		genral_dialog_ID = -1;
		gen_bkg_ID = gen_back_radio->get_int_val();
		backround_color_R->set_int_val((int)(background_RGB[0+gen_bkg_ID*3]*100));
		backround_color_G->set_int_val((int)(background_RGB[1+gen_bkg_ID*3]*100));
		backround_color_B->set_int_val((int)(background_RGB[2+gen_bkg_ID*3]*100));
		break;
			 }
	case(40):{
		genral_dialog_ID = 40;
		if (compass == 0) gen_comp_label_check->disable();
		else gen_comp_label_check->enable();
		break;
			 }
	case(50):{
		genral_dialog_ID = 50;
		gRGB[0] = (float)backround_color_R->get_int_val()/100;
		gRGB[1] = (float)backround_color_G->get_int_val()/100;
		gRGB[2] = (float)backround_color_B->get_int_val()/100;
		gcolor_palette_btn->disable();
		gen_OK_btn->disable();
		gen_back_radio->disable();
		break;
			 }
	case(60):{
		genral_dialog_ID = 60;
		if (rproj_type == 0) projection_spin->enable();
		else projection_spin->disable();
		break;
			 }
	}
}

//********************  set_general_param()  ********************************//
void set_general_param(float l_a[4],   float l_d[4], float l_dir[16], 
					   float RGB1[3], float RGB2[3], float RGB3[3], int comp,
					   int proj_type, float persp)
{
	int i;

	lights_ambient  = l_a[0];
	lights_diffuse  = l_d[0];
	for(i=0; i<16; i++) lights_direction[i] = l_dir[i];

	background_RGB[0] = RGB1[0];
	background_RGB[1] = RGB1[1];
	background_RGB[2] = RGB1[2];

	background_RGB[3] = RGB2[0];
	background_RGB[4] = RGB2[1];
	background_RGB[5] = RGB2[2];

	background_RGB[6] = RGB3[0];
	background_RGB[7] = RGB3[1];
	background_RGB[8] = RGB3[2];

	gen_bkg_ID = 0;

	switch(comp){
	case(0):{
		compass = 0;
		lcompass = 0;
		break;
			}
	case(1):{
		compass = 1;
		lcompass = 1;
		break;
			}
	case(2):{
		compass = 1;
		lcompass = 0;
		break;
			}
	}

	rproj_type = proj_type;
	rpersp = persp;
}


//********************  set_general_param()  ********************************//
void init_general_dialog()
{
	light_ambient->set_int_val((int)(lights_ambient*100));
	light_diffuse->set_int_val((int)(lights_diffuse*100));

	backround_color_R->set_int_val((int)(background_RGB[0]*100));
	backround_color_G->set_int_val((int)(background_RGB[1]*100));
	backround_color_B->set_int_val((int)(background_RGB[2]*100));

	gen_back_radio->set_int_val(gen_bkg_ID);

	gen_comp_check->set_int_val(compass);
	gen_comp_label_check->set_int_val(lcompass);
	if (compass == 0) gen_comp_label_check->disable();
	else gen_comp_label_check->enable();
}


//********************  open_general_dialog()  ******************************//
void open_general_dialog(int ID, int dpX, int dpY)
{
//--- Initial Settings
	genral_dialog_ID = -1;
	gen_parent_window = ID;
	gen_palette_ID = 0;

	rend = GLUI_Master.create_glui("General",0, dpX, dpY );
	rend->set_main_gfx_window( ID );

	rend_panel = rend->add_panel("",GLUI_PANEL_RAISED);

//---------------------------------------------------------------------------
	lights_rend_panel = rend->add_panel_to_panel(rend_panel,"Light",GLUI_PANEL_EMBOSSED);
	lights_rend_panel->set_alignment(GLUI_ALIGN_LEFT);

	light_ambient = rend->add_spinner_to_panel(lights_rend_panel,"Ambient (%):",
		                                         GLUI_SPINNER_INT,NULL,10,rend_control_cb);
	light_ambient->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	light_ambient->set_alignment(GLUI_ALIGN_RIGHT);
	light_diffuse = rend->add_spinner_to_panel(lights_rend_panel,"Diffuse (%):",
		                                         GLUI_SPINNER_INT,NULL,10,rend_control_cb);
	light_diffuse->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	light_diffuse->set_alignment(GLUI_ALIGN_RIGHT);

	rend->add_column_to_panel(lights_rend_panel, true);

	light_direction = rend->add_rotation_to_panel(lights_rend_panel,"Light direction",
		                                            lights_direction,10,rend_control_cb);
//---------------------------------------------------------------------------
	background_rend_panel = rend->add_panel_to_panel(rend_panel,"Background",GLUI_PANEL_EMBOSSED);
	background_rend_panel->set_alignment(GLUI_ALIGN_LEFT);

	gen_back_radio = rend->add_radiogroup_to_panel(background_rend_panel,NULL,30,rend_control_cb);
	rend->add_radiobutton_to_group(gen_back_radio,"Graphics Window");
	rend->add_radiobutton_to_group(gen_back_radio,"2D Map Window");
	rend->add_radiobutton_to_group(gen_back_radio,"Bird's-Eye View Window");

	rend->add_column_to_panel(background_rend_panel,true);

	backround_color_R = rend->add_spinner_to_panel(background_rend_panel,"Red (%):",
		                                         GLUI_SPINNER_INT,NULL,20,rend_control_cb);
	backround_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	backround_color_G = rend->add_spinner_to_panel(background_rend_panel,"Green (%):",
		                                         GLUI_SPINNER_INT,NULL,20,rend_control_cb);
	backround_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	backround_color_B = rend->add_spinner_to_panel(background_rend_panel,"Blue (%):",
		                                         GLUI_SPINNER_INT,NULL,20,rend_control_cb);
	backround_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	rend->add_separator_to_panel(background_rend_panel);
	gcolor_palette_btn = rend->add_button_to_panel(background_rend_panel,"Palette",50,rend_control_cb);


//---------------------------------------------------------------------------
	projection_render_panel = rend->add_panel_to_panel(rend_panel,"Projection",GLUI_PANEL_EMBOSSED);
	projection_render_panel->set_alignment(GLUI_ALIGN_LEFT);

	projection_tipe_radio = rend->add_radiogroup_to_panel(projection_render_panel,&rproj_type,60,rend_control_cb);
	rend->add_radiobutton_to_group(projection_tipe_radio,"Perspective projection");
	rend->add_radiobutton_to_group(projection_tipe_radio,"Parallel projection");
	projection_spin = rend->add_spinner_to_panel(projection_render_panel,"Perspecive: ",GLUI_SPINNER_FLOAT, &rpersp,
		                                                                                        60,rend_control_cb);
	if (rproj_type == 0) projection_spin->enable();
	else projection_spin->disable();

	projection_spin->set_float_limits(0.5, 3, GLUI_LIMIT_CLAMP);
//---------------------------------------------------------------------------
	gen_comp_check = rend->add_checkbox_to_panel(rend_panel,"Show compass",&compass,40,rend_control_cb);
	gen_comp_check->set_alignment(GLUI_ALIGN_LEFT);
	gen_comp_label_check = rend->add_checkbox_to_panel(rend_panel,"Show axis labels",&lcompass,40,rend_control_cb);
	gen_comp_check->set_alignment(GLUI_ALIGN_LEFT);
//---------------------------------------------------------------------------

	gen_OK_btn = rend->add_button("OK", 0, rend_control_cb);

	init_general_dialog();
}

 

