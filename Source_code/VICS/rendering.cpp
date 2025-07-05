//**********************************************************************************//
//                            RENDERING OPTIONS                                     //
//----------------------------------------------------------------------------------//
// File: rendering.cpp                                                              //
// Date: 2001.12.18                                                                 //
//----------------------------------------------------------------------------------//
// Control:                                                                         //
//   control_ID_ID = -1 -- Open General Properties Dialog                           //
//                    1 -- Close Bonds Properties Dialog ("OK")                     //
//                  100 -- lights rendering (ambient, diffuse, direction)           //
//                  101 -- atoms  rendering (ambient, diffuse, specular, shineness) //
//                  102 -- bonds  rendering (ambient, diffuse, specular, shineness) //
//                  103 -- poly   rendering (ambient, diffuse, specular, shineness) //
//                  104 -- plane  rendering (ambient, diffuse, specular, shineness) //
//                  105 -- Background Colors(RGB)                                   //
//                  106 -- Compass                                                  //
//                                                                                  //
//   objectID = 0 -- Atoms (Materiala Rendering)                                    //
//              1 -- Bonds (Materiala Rendering)                                    //
//              2 -- Polyhedra (Materiala Rendering)                                //
//              3 -- HKL-Plane (Materiala Rendering)                                //
//**********************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *render;                                //--- GLUI window

GLUI_Panel *render_panel;                    //--- Main Panel
GLUI_Panel *material_render_panel;           //--- Materials Rendering Panel 
GLUI_Panel *lights_render_panel;             //--- Lights Rendering Panel
GLUI_Panel *background_render_panel;         //--- Background Rendering Panel
GLUI_Panel *projection_render_panel;         //--- Background Projection Panel

GLUI_Listbox *object_render_list;            //--- List of Objects

GLUI_Spinner *material_ambient;              //--- Materials Ambient 
GLUI_Spinner *material_diffuse;              //--- Materials Diffuse
GLUI_Spinner *material_specular;             //--- Materials Specular 
GLUI_Spinner *material_shineness;            //--- Materials Shineness

GLUI_Spinner *light_ambient;                 //--- Ligts Ambient
GLUI_Spinner *light_diffuse;                 //--- Ligts Diffuse
GLUI_Rotation *light_direction;              //--- Lights Direction
GLUI_Translation *light_position;            //--- Lights Position

GLUI_Spinner *backround_color_R;              //--- Background Red Color
GLUI_Spinner *backround_color_G;              //--- Background Green Color
GLUI_Spinner *backround_color_B;              //--- Background Blue Color

GLUI_Checkbox *render_comp_check;             //--- Compass Checkbox;
GLUI_Checkbox *render_comp_label_check;       //--- Compass Labels Checkbox;

GLUI_RadioGroup *projection_tipe_radio;
GLUI_Spinner *projection_spin;

GLUI_Listbox *radii_render_list;            //--- List of Objects

GLUI_Spinner *fading_render_spin;           //--- Depth-cueing  


GLUI_Button *render_OK_btn;

//--- Live Variabels
int objectID;                                //--- Selected object control
int mat_amb;                                 //--- for	Materials Ambient
int mat_dif;                                 //--- for	Materials Diffuse
int mat_spec;                                //--- for	Materials Specular
int mat_shin;                                //--- for	Materials Shineness
int l_amb;                                   //--- for	Lights Ambient
int l_dif;                                   //--- for	Lights Diffuse
int bkgr[3];                                 //--- for Background Colors(RGB)


//--- Global Variables
float mat_ambient[4];
float mat_diffuse[4];
float mat_specular[4];
float mat_shineness[4];
float lights_direction[16];                      
float lights_ambient;
float lights_diffuse;
float background_RGB[3];
int   rcompass,Lcompass;
int   radiiT;
int fogD;
///float fogD;

int rproj_type;
float rpersp;

//--- Variables
int control_ID = -1;                         //--- Control ID

//--- Color Palette
GLUI_Button *color_palette_btn;
int render_parent_window;
int render_palette_ID;

/**************************************** get_general_dialog_pos() ************/
void get_general_dialog_pos(int &x, int &y)
{
	glutSetWindow(render->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

//********************  get_render_control_id()  ****************************//
void set_render_control_id(int ID)
{
	control_ID = ID;
}


//********************  get_render_control_id()  ****************************//
int get_render_control_id()
{
	return(control_ID);
}


//********************  get_projection_perspective()  ***********************//
void get_projection_perspective(int &proj_type, float &persp)
{
	proj_type = rproj_type;
	persp = rpersp;
}

//********************  get_atom_rendering()  *******************************//
void get_atom_rendering(float m_a_a[4], float m_a_d[4], float m_a_s[4], float &m_a_sh)
{
	int i;
	for(i=0; i<3; i++){
		m_a_a[i] = mat_ambient[0];
		m_a_d[i] = mat_diffuse[0];
		m_a_s[i] = mat_specular[0];
		m_a_sh   = mat_shineness[0];
	}
}

//********************  set_bkg_colors()  ***********************************//
void set_bkg_colors(float RGB[3])
{
	background_RGB[0] = RGB[0];
	background_RGB[1] = RGB[1];
	background_RGB[2] = RGB[2];
	bkgr[0] = (int)(background_RGB[0]*100);
	bkgr[1] = (int)(background_RGB[1]*100);
	bkgr[2] = (int)(background_RGB[2]*100);
	backround_color_R->set_int_val(bkgr[0]);
	backround_color_G->set_int_val(bkgr[1]);
	backround_color_B->set_int_val(bkgr[2]);
	color_palette_btn->enable();
	render_OK_btn->enable();

}

//********************  get_bkg_colors()  ***********************************//
void get_bkg_colors(float RGB[3])
{
	RGB[0] = background_RGB[0] = (float)bkgr[0]/100;
	RGB[1] = background_RGB[1] = (float)bkgr[1]/100;
	RGB[2] = background_RGB[2] = (float)bkgr[2]/100;
}


//********************  get_bond_rendering()  *******************************//
void get_bond_rendering(float m_b_a[4], float m_b_d[4], float m_b_s[4], float &m_b_sh)
{
	int i;
	for(i=0; i<3; i++){
		m_b_a[i] = mat_ambient[1];
		m_b_d[i] = mat_diffuse[1];
		m_b_s[i] = mat_specular[1];
		m_b_sh   = mat_shineness[1];
	}
}

//********************  get_poly_rendering()  *******************************//
void get_poly_rendering(float m_p_a[4], float m_p_d[4], float m_p_s[4], float &m_p_sh)
{
	int i;
	for(i=0; i<3; i++){
		m_p_a[i] = mat_ambient[2];
		m_p_d[i] = mat_diffuse[2];
		m_p_s[i] = mat_specular[2];
		m_p_sh   = mat_shineness[2];
	}
}

//********************  get_poly_rendering()  *******************************//
void get_plane_rendering(float m_h_a[4], float m_h_d[4], float m_h_s[4], float &m_h_sh)
{
	int i;
	for(i=0; i<3; i++){
		m_h_a[i] = mat_ambient[3];
		m_h_d[i] = mat_diffuse[3];
		m_h_s[i] = mat_specular[3];
		m_h_sh   = mat_shineness[3];
	}
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
void get_background_rendering(float &R, float &G, float &B)
{
	R = background_RGB[0];
	G = background_RGB[1];
	B = background_RGB[2];
}

//********************  get_compass_rendering()  *************************//
int get_compass_rendering()
{

	if ((rcompass == 1) && (Lcompass == 1)) return(1);
	if ((rcompass == 1) && (Lcompass == 0)) return(2);
	return(0);
}

//********************  get_radii_rendering()  ******************************//
int get_radii_rendering()
{
	int ID;

	ID = radiiT;
	return(ID);
}

//********************  get_fog_rendering()  ********************************//
float get_fog_rendering()
{
	float F;

	F = 1.0 + 0.5*((float)fogD - 1.0)/99.0;
	return(F);
}




//********************  render_control_cb()  ********************************//
void render_control_cb( int control )
{
	switch (control)
	{
	case (0):{control_ID = 1; render->close();break;}
	case(10):{
		control_ID = -1;
		mat_amb = (int)(mat_ambient[objectID] * 100);
		mat_dif = (int)(mat_diffuse[objectID] * 100);
		mat_spec = (int)(mat_specular[objectID] * 100);
		mat_shin = (int)(mat_shineness[objectID] * 100 / 128);
		material_ambient->set_int_val(mat_amb);
		material_diffuse->set_int_val(mat_dif);
		material_specular->set_int_val(mat_spec);
		material_shineness->set_int_val(mat_shin);
		break;
			 }
	case(20):{
		control_ID = 100 + objectID + 1;
		mat_ambient[objectID] = (float)mat_amb/100;
		mat_diffuse[objectID] = (float)mat_dif/100;
		mat_specular[objectID] = (float)mat_spec/100;
		mat_shineness[objectID] = (float)(mat_shin * 128 / 100);
		break;
			 }
	case(30):{
		control_ID = 100;
		lights_ambient = (float)l_amb/100;
		lights_diffuse = (float)l_dif/100;
		break;
			 }
	case(40):{
		control_ID = 105;
		background_RGB[0] = (float)bkgr[0]/100;
		background_RGB[1] = (float)bkgr[1]/100;
		background_RGB[2] = (float)bkgr[2]/100;
		break;
			 }
	case(50):{
		control_ID = 106;
		rcompass = render_comp_check->get_int_val();
		Lcompass = render_comp_label_check->get_int_val();
		if (rcompass == 0) render_comp_label_check->disable();
		else render_comp_label_check->enable();
		break;
			 }
	case(60):{
		control_ID = 107;
		if (rproj_type == 0) projection_spin->enable();
		else projection_spin->disable();
		break;
			 }
	case(70):{
		control_ID = 108;
		color_palette_btn->disable();
		render_OK_btn->disable();
		break;
			 }
	case(80):{
		control_ID = 109;
		radiiT = radii_render_list->get_int_val();
		break;
			 }
	case(90):{
		control_ID = 110;
		fogD = fading_render_spin->get_int_val();
		break;
			 }
	}
}

//********************  init_rendering_dialog()  ****************************//
void init_rendering_dialog(float m_a_a[4], float m_a_d[4], float m_a_s[4], float m_a_sh,
						   float m_b_a[4], float m_b_d[4], float m_b_s[4], float m_b_sh,
						   float m_p_a[4], float m_p_d[4], float m_p_s[4], float m_p_sh,
						   float m_h_a[4], float m_h_d[4], float m_h_s[4], float m_h_sh,
						   float l_a[4],   float l_d[4], float l_dir[16],
						   float RGB[3], int comp,int proj_type, float persp, 
						   int r_type, float fogi)
{
	int i;
	mat_ambient[0] = m_a_a[0];
	mat_ambient[1] = m_b_a[0];
	mat_ambient[2] = m_p_a[0];
	mat_ambient[3] = m_h_a[0];

	mat_diffuse[0] = m_a_d[0];
	mat_diffuse[1] = m_b_d[0];
	mat_diffuse[2] = m_p_d[0];
	mat_diffuse[3] = m_h_d[0];

	mat_specular[0] = m_a_s[0];
	mat_specular[1] = m_b_s[0];
	mat_specular[2] = m_p_s[0];
	mat_specular[3] = m_h_s[0];

	mat_shineness[0] = m_a_sh;
	mat_shineness[1] = m_b_sh;
	mat_shineness[2] = m_p_sh;
	mat_shineness[3] = m_h_sh;

	lights_ambient  = l_a[0];
	lights_diffuse  = l_d[0];

	for(i=0; i<16; i++) lights_direction[i] = l_dir[i];

	mat_amb =  (int)(mat_ambient[0] * 100);
	mat_dif =  (int)(mat_diffuse[0] * 100);
	mat_spec = (int)(mat_specular[0] * 100);
	mat_shin = (int)(mat_shineness[0] * 100 / 128);
	l_amb =    (int)(lights_ambient * 100);
	l_dif =    (int)(lights_diffuse * 100);

	background_RGB[0] = RGB[0];
	background_RGB[1] = RGB[1];
	background_RGB[2] = RGB[2];
	bkgr[0] = (int)(background_RGB[0]*100);
	bkgr[1] = (int)(background_RGB[1]*100);
	bkgr[2] = (int)(background_RGB[2]*100);

	switch(comp)
	{
	case(0):{
		rcompass = 0;
		Lcompass = 0;
		break;
			}
	case(1):{
		rcompass = 1;
		Lcompass = 1;
		break;
			}
	case(2):{
		rcompass = 1;
		Lcompass = 0;
		break;
			}
	}

	rproj_type = proj_type;
	rpersp = persp;
	radiiT = r_type;
	fogD = (int)(1.0 + 99.0*(fogi - 1)/0.5);
}


//********************  open_rendering_dialog()  ****************************//
void open_rendering_dialog(int ID, int dpX, int dpY)
{
//--- Initial Settings
	render_palette_ID = 0;

	control_ID = -1;
    objectID = 0;

	render_parent_window = ID;
	render = GLUI_Master.create_glui("General",0,dpX,dpY );
	render->set_main_gfx_window( ID );

	
	render_panel = render->add_panel("",GLUI_PANEL_RAISED);

//---------------------------------------------------------------------------
	material_render_panel = render->add_panel_to_panel(render_panel,"Materials",GLUI_PANEL_EMBOSSED);

	object_render_list = render->add_listbox_to_panel(material_render_panel,"Objects: ",&objectID,10,render_control_cb); 
	object_render_list->add_item(0,"Atoms");
	object_render_list->add_item(1,"Bonds");
	object_render_list->add_item(2,"Polyhedra");
	object_render_list->add_item(3,"Lattice plane");

	render->add_separator_to_panel(material_render_panel);

	material_ambient = render->add_spinner_to_panel(material_render_panel,"Ambient (%): ",
		                                            GLUI_SPINNER_INT,&mat_amb,20,render_control_cb);
	material_ambient->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	material_ambient->set_alignment(GLUI_ALIGN_RIGHT);
	
	material_diffuse = render->add_spinner_to_panel(material_render_panel,"Diffuse (%): ",
		                                            GLUI_SPINNER_INT,&mat_dif,20,render_control_cb);
	material_diffuse->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	material_diffuse->set_alignment(GLUI_ALIGN_RIGHT);

	material_specular = render->add_spinner_to_panel(material_render_panel,"Specular (%): ",
		                                             GLUI_SPINNER_INT,&mat_spec,20,render_control_cb);
	material_specular->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	material_specular->set_alignment(GLUI_ALIGN_RIGHT);

	material_shineness = render->add_spinner_to_panel(material_render_panel,"Shininess (%): ",
		                                             GLUI_SPINNER_INT,&mat_shin,20,render_control_cb);
	material_shineness->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	material_shineness->set_alignment(GLUI_ALIGN_RIGHT);
//---------------------------------------------------------------------------
	lights_render_panel = render->add_panel_to_panel(render_panel,"Light",GLUI_PANEL_EMBOSSED);

	light_ambient = render->add_spinner_to_panel(lights_render_panel,"Ambient (%): ",
		                                         GLUI_SPINNER_INT,&l_amb,30,render_control_cb);
	light_ambient->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	light_ambient->set_alignment(GLUI_ALIGN_RIGHT);

	light_diffuse = render->add_spinner_to_panel(lights_render_panel,"Diffuse (%): ",
		                                         GLUI_SPINNER_INT,&l_dif,30,render_control_cb);
	light_diffuse->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	light_diffuse->set_alignment(GLUI_ALIGN_RIGHT);

	render->add_separator_to_panel(lights_render_panel);

	light_direction = render->add_rotation_to_panel(lights_render_panel,"Light direction",
		                                            lights_direction,30,render_control_cb);
//---------------------------------------------------------------------------
	render->add_column_to_panel(render_panel, false);
//---------------------------------------------------------------------------
	background_render_panel = render->add_panel_to_panel(render_panel,"Background",GLUI_PANEL_EMBOSSED);
	backround_color_R = render->add_spinner_to_panel(background_render_panel,"Red (%): ",
		                                         GLUI_SPINNER_INT,&bkgr[0],40,render_control_cb);
	backround_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	backround_color_G = render->add_spinner_to_panel(background_render_panel,"Green (%): ",
		                                         GLUI_SPINNER_INT,&bkgr[1],40,render_control_cb);
	backround_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	backround_color_B = render->add_spinner_to_panel(background_render_panel,"Blue (%): ",
		                                         GLUI_SPINNER_INT,&bkgr[2],40,render_control_cb);
	backround_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	render->add_separator_to_panel(background_render_panel);
	color_palette_btn = render->add_button_to_panel(background_render_panel,"Palette",70,render_control_cb);

//---------------------------------------------------------------------------
	projection_render_panel = render->add_panel_to_panel(render_panel,"Projection",GLUI_PANEL_EMBOSSED);
	projection_tipe_radio = render->add_radiogroup_to_panel(projection_render_panel,&rproj_type,60,render_control_cb);
	render->add_radiobutton_to_group(projection_tipe_radio,"Perspective projection");
	render->add_radiobutton_to_group(projection_tipe_radio,"Parallel projection");
	projection_spin = render->add_spinner_to_panel(projection_render_panel,"Perspecive: ",GLUI_SPINNER_FLOAT, &rpersp,
		                                                                                        60,render_control_cb);
	if (rproj_type == 0) projection_spin->enable();
	else projection_spin->disable();

	projection_spin->set_float_limits(0.5, 3, GLUI_LIMIT_CLAMP);
	
	render->add_separator_to_panel(render_panel);

	render_comp_check = render->add_checkbox_to_panel(render_panel,"Show compass",&rcompass,50,render_control_cb);
	render_comp_label_check = render->add_checkbox_to_panel(render_panel,"Show axis labels",&Lcompass,50,render_control_cb);

	render->add_separator_to_panel(render_panel);

	radii_render_list = render->add_listbox_to_panel(render_panel,"Radii: ",&radiiT,80,render_control_cb); 
	radii_render_list->add_item(0,"Atomic");
	radii_render_list->add_item(1,"Ionic");
	radii_render_list->add_item(2,"van der Waals");
	radii_render_list->set_int_val(radiiT);
	radii_render_list->set_alignment(GLUI_ALIGN_RIGHT);

	
	fading_render_spin = render->add_spinner_to_panel(render_panel,"Depth-cueing (%): ",
		                                         GLUI_SPINNER_INT,NULL,90,render_control_cb);
	fading_render_spin->set_int_limits(1,100,GLUI_LIMIT_CLAMP);
	fading_render_spin->set_int_val(fogD);
	fading_render_spin->set_alignment(GLUI_ALIGN_RIGHT);


//---------------------------------------------------------------------------
	render_OK_btn = render->add_button("OK", 0, render_control_cb);

}

 

