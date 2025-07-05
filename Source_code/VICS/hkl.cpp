//******************************************************************************//
//                      (hkl) PLANE PROPERTIES DIALOG                           //
//------------------------------------------------------------------------------//
// File: hkl.cpp                                                                //
// Date: 2002.01.30                                                             //
//------------------------------------------------------------------------------//
// Control:                                                                     //
//   hkl_proper_dialog_ID = -1 -- Open (hkl) Plane Properties Dialog            //
//                           1 -- Close (hkl) Plane Properties Dialog ("OK")    //
//                           0 -- Close (hkl) Plane Properties Dialog ("Cancel")//
//                                                                              //
//******************************************************************************//
#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>

#define PI						3.14159 

// Objects
GLUI *hklglui;                                //--- (HKL)PLANE Propertires GLUI window
GLUI_Panel *hkl_main_panel;                   //--- (HKL)PLANE Main Panel
GLUI_Panel *hkl_set_panel;                    //--- (HKL)PLANE Settings Panel
GLUI_Panel *hkl_set1_panel;                    //--- (HKL)PLANE Settings Panel
GLUI_Panel *hkl_origin_panel;                 //--- (HKL)PLANE Origin Settings Panel
GLUI_Panel *hkl_prop_panel;                   //--- (HKL)PLANE Propertires Panel
GLUI_Panel *hkl_color_panel;                  //--- (HKL)PLANE Colors Panel
GLUI_Panel *hkl_out_color_panel;              //--- (HKL)PLANE Outline Colors Panel
GLUI_Panel *hkl_btn_panel;                    //--- (HKL)PLANE "OK"/"Cancel" Panel
GLUI_RadioGroup *hkl_origin;                  //--- (HKL)PLANE Origin
GLUI_Checkbox *hkl_show_plane;                //--- (HKL)PLANE Show (h k l) Plane ID
GLUI_Checkbox *hkl_inv_show_plane;            //--- (HKL)PLANE Show (-h -k -l) Plane ID
GLUI_EditText *hkl_index_H;                   //--- (HKL)PLANE h,k,l Indexes
GLUI_EditText *hkl_index_K;
GLUI_EditText *hkl_index_L;
GLUI_Button *apply_btn;
GLUI_Button *clr_btn;
GLUI_Spinner *hkl_color_R;                    //--- (HKL)PLANE Colors (RGB) 
GLUI_Spinner *hkl_color_G; 
GLUI_Spinner *hkl_color_B; 
GLUI_Spinner *hkl_trans_spin;                      //--- (HKL)PLANE Opacity 
GLUI_Checkbox *hkl_show_out_plane;            //--- (HKL)PLANE Show Outline Plane ID
GLUI_EditText *hkl_o_width;                 //--- (HKL)PLANE Outline Width 
GLUI_Checkbox *hkl_prev_check;                //--- (HKL)PLANE Preview Checkbox;

GLUI_Button *hkl_Cancel_btn;
GLUI_Button *hkl_OK_btn;

GLUI_StaticText *hkl_d_label;

// Control
int hkl_proper_Dialog_ID;                    //--- (HKL)PLANE dialog control ID
int hkl_preview = 1;                         //--- (HKL)PLANE properties preview control 

int hkl_show;                                //--- (HKL)PLANE show ID
int orig_hkl;                                //--- (HKL)PLANE origin ID
int show_inv_hkl;                            //--- (HKL)PLANE Show (-h -k -l) Control ID
int show_out_hkl;                            //--- (HKL)PLANE Show Outline Control ID
int hkl_h,hkl_k,hkl_l;                       //--- (HKL)PLANE h,k,l
float hkl_R,hkl_G,hkl_B;                     //--- (HKL)PLANE R,G,B
float hkl_T;                                 //--- (HKL)PLANE Opacity
float hkl_out_w;                             //--- (HKL)PLANE Outline Width

int old_hkl_show;
int old_orig_hkl;
int old_show_inv_hkl;
int old_show_out_hkl;
int old_hkl_h,old_hkl_k,old_hkl_l;
float old_hkl_R,old_hkl_G,old_hkl_B;
float old_hkl_T;
float old_hkl_out_w;

//--- Color Palette
GLUI_Button *hcolor_palette_btn;
int hkl_parent_window;
int hkl_palette_ID;
float hklRGB[3];
double D;
float pcell[6];


//********************  calc_Dhkl()  ****************************************//
void calc_Dhkl()
{
	double a,b,c;
	double cosa,cosb,cosg,sina,sinb,sing;
	double s1,s2,s3,s12,s13,s23,v,s;
	int h,k,l;
	char d_line[50];

	h = hkl_h;
	k = hkl_k;
	l = hkl_l;
	a = pcell[0];
	b = pcell[1];
	c = pcell[2];
	cosa = cos(pcell[3]*PI/180.0);
	cosb = cos(pcell[4]*PI/180.0);
	cosg = cos(pcell[5]*PI/180.0);
	sina = sqrt(1 - cosa*cosa);
	sinb = sqrt(1 - cosb*cosb);
	sing = sqrt(1 - cosg*cosg);
	s1 = (float)h*sina/a;
	s2 = (float)k*sinb/b;
	s3 = (float)l*sing/c;
	s12 = (float)(h*k)*(cosa*cosb - cosg)/(a*b);
	s13 = (float)(h*l)*(cosa*cosg - cosb)/(a*c);
	s23 = (float)(l*k)*(cosg*cosb - cosa)/(c*b);
	v = 1 - cosa*cosa - cosb*cosb - cosg*cosg + 2*cosa*cosb*cosg;
	s = (s1*s1 + s2*s2 + s3*s3 - 2*(s12 + s13 + s23))/v;
	D = 1.0/sqrt(s);
	sprintf(d_line,"");
	sprintf(d_line,"d = %8.5f Angstrom",D);
	hkl_d_label->set_name(d_line);

}

/**************************************** get_hkl_dialog_pos() ************/
void get_hkl_dialog_pos(int &x, int &y)
{
	glutSetWindow(hklglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  set_hkl_prop_dialog_id()  ***************************//
void set_hkl_prop_dialog_id(int ID)
{
	hkl_proper_Dialog_ID = ID;
}


//********************  get_hkl_prop_dialog_id()  ***************************//
int get_hkl_prop_dialog_id()
{
	return(hkl_proper_Dialog_ID);
}


//********************  set_hkl_colors()  ***********************************//
void set_hkl_colors(float RGB[3])
{
	hkl_R = RGB[0];
	hkl_G = RGB[1];
	hkl_B = RGB[2];
	hkl_color_R->set_int_val((int)(hkl_R*100));
	hkl_color_G->set_int_val((int)(hkl_G*100));
	hkl_color_B->set_int_val((int)(hkl_B*100));
	hcolor_palette_btn->enable();
	hkl_Cancel_btn->enable();
	hkl_OK_btn->enable();
}

//********************  get_hkl_colors()  ***********************************//
void get_hkl_colors(float RGB[3])
{
	RGB[0] = hkl_R = (float)hkl_color_R->get_int_val()/100;
	RGB[1] = hkl_G = (float)hkl_color_G->get_int_val()/100;
	RGB[2] = hkl_B = (float)hkl_color_B->get_int_val()/100;
}


//********************  get_hkl_prop_preview_id()  **************************//
int get_hkl_prop_preview_id()
{
	return(hkl_preview);
}

//********************  get_hkl_prop_main_param()  **************************//
void get_hkl_prop_main_param(int &ID, int &orig, int &inv_hkl, int &out_hkl, float &oW)
{
	ID = hkl_show;
	orig = orig_hkl;
	inv_hkl = show_inv_hkl;
	out_hkl = show_out_hkl;
	oW = hkl_out_w;
}

//********************  get_hkl_prop_colors()  ******************************//
void get_hkl_prop_colors(float &R, float &G, float &B, float &T)
{
	R = hkl_R;
	G = hkl_G;
	B = hkl_B;
	T = hkl_T;
}

//********************  get_hkl_prop_index()  *******************************//
void get_hkl_prop_index(int &h, int &k, int &l)
{
	h = hkl_h;
	k = hkl_k;
	l = hkl_l;
}


/**************************************** hkl_prop_control() ****************/
void hkl_prop_control( int control)
{
	switch(control)
	{
	case(0):{
		hkl_proper_Dialog_ID = 1; 
		hklglui->close(); 
		break; 
			}
	case(1):{
		hkl_show = old_hkl_show;
		orig_hkl = old_orig_hkl;
		show_inv_hkl = old_show_inv_hkl;
		show_out_hkl = old_show_out_hkl;
		hkl_out_w = old_hkl_out_w;
		hkl_h = old_hkl_h;
		hkl_k = old_hkl_k;
		hkl_l = old_hkl_l;
		hkl_R = old_hkl_R;
		hkl_G = old_hkl_G;
		hkl_B = old_hkl_B;
		hkl_T = old_hkl_T;
		hkl_proper_Dialog_ID = 0; 
		hklglui->close(); 
		break; 
			}
	case(600):{
		hkl_show = hkl_show_plane->get_int_val();
		show_inv_hkl = hkl_inv_show_plane->get_int_val();
		show_out_hkl = hkl_show_out_plane->get_int_val();
		hkl_out_w = hkl_o_width->get_float_val();
		hkl_proper_Dialog_ID = 600; 
		break;
			  }
	case(601):{
		hkl_h = hkl_index_H->get_int_val();
		hkl_k = hkl_index_K->get_int_val();
		hkl_l = hkl_index_L->get_int_val();
		hkl_proper_Dialog_ID = 601; 
		break;
			  }
	case(602):{
		hkl_R = (float)hkl_color_R->get_int_val()/100;
		hkl_G = (float)hkl_color_G->get_int_val()/100;
		hkl_B = (float)hkl_color_B->get_int_val()/100;
		hkl_T = (float)hkl_trans_spin->get_int_val()/100;
		hkl_proper_Dialog_ID = 602; 
		break;
			  }
	case(603):{
		hkl_proper_Dialog_ID = -1;
		hkl_h = hkl_index_H->get_int_val();
		hkl_k = hkl_index_K->get_int_val();
		hkl_l = hkl_index_L->get_int_val();
		calc_Dhkl();
		break;
			  }
	case(604):{
		orig_hkl = hkl_origin->get_int_val();
		if(orig_hkl == 1) hkl_inv_show_plane->disable();
		else hkl_inv_show_plane->enable();
		hkl_proper_Dialog_ID = 604; 
		break;
			  }
	case(700):{
		hkl_proper_Dialog_ID = 700; 
		hcolor_palette_btn->disable();
		hkl_Cancel_btn->disable();
		hkl_OK_btn->disable();
		break;
			  }
	}
}

/**************************************** set_cell_param() ******************/
void set_cell_param(float param[6])
{
	int i;
	for(i=0; i<6; i++) pcell[i] = param[i];
}

/**************************************** set_hkl_prop_dialog() *************/
void set_hkl_prop_dialog(int ID, int orig, int inv_hkl, int out_hkl, int HKL[3], float RGB[3], float trans, float oW)
{
	old_hkl_show = hkl_show = ID;
	old_orig_hkl = orig_hkl = orig;
	old_show_inv_hkl = show_inv_hkl = inv_hkl;
	old_show_out_hkl = show_out_hkl = out_hkl;
	old_hkl_h = hkl_h = HKL[0];
	old_hkl_k = hkl_k = HKL[1];
	old_hkl_l = hkl_l = HKL[2];
	old_hkl_R = hkl_R = RGB[0];
	old_hkl_G = hkl_G = RGB[1];
	old_hkl_B = hkl_B = RGB[2];
	old_hkl_T = hkl_T = trans;
	old_hkl_out_w = hkl_out_w = oW;
}

/**************************************** init_hkl_prop_dialog() ************/
void init_hkl_prop_dialog()
{
	hkl_show_plane->set_int_val(hkl_show);
	hkl_inv_show_plane->set_int_val(show_inv_hkl);
	hkl_show_out_plane->set_int_val(show_out_hkl);
	hkl_o_width->set_float_val(hkl_out_w);

	hkl_origin->set_int_val(orig_hkl);

	hkl_index_H->set_int_val(hkl_h);
	hkl_index_K->set_int_val(hkl_k);
	hkl_index_L->set_int_val(hkl_l);

	hkl_color_R->set_int_val((int)(hkl_R*100));
	hkl_color_G->set_int_val((int)(hkl_G*100));
	hkl_color_B->set_int_val((int)(hkl_B*100));
	hkl_trans_spin->set_int_val((int)(hkl_T*100));

	if(orig_hkl == 1) hkl_inv_show_plane->disable();
	else hkl_inv_show_plane->enable();
	calc_Dhkl();
}


/**************************************** open_hkl_prop_dialog() ************/
void open_hkl_prop_dialog(int ID, int dpX, int dpY)
{
	hkl_proper_Dialog_ID = -1;
	hkl_parent_window = ID;
	hkl_palette_ID = 0;


	hklglui = GLUI_Master.create_glui("Lattice Plane",0,dpX,dpY );

	hkl_show_plane = hklglui->add_checkbox("Show lattice plane",NULL,600,hkl_prop_control);
	hkl_main_panel = hklglui->add_panel("",GLUI_PANEL_RAISED);

//--------------------------------------------------------------------------
	hkl_set_panel = hklglui->add_panel_to_panel(hkl_main_panel,"Miller indices",GLUI_PANEL_EMBOSSED);
	hkl_index_H = hklglui->add_edittext_to_panel(hkl_set_panel,"h: ",GLUI_EDITTEXT_INT,NULL,603,hkl_prop_control);
	hkl_index_K = hklglui->add_edittext_to_panel(hkl_set_panel,"k: ",GLUI_EDITTEXT_INT,NULL,603,hkl_prop_control);
	hkl_index_L = hklglui->add_edittext_to_panel(hkl_set_panel,"l: ",GLUI_EDITTEXT_INT,NULL,603,hkl_prop_control);
	hklglui->add_separator_to_panel(hkl_set_panel);
	hkl_d_label = hklglui->add_statictext_to_panel(hkl_set_panel,"");
	hklglui->add_separator_to_panel(hkl_set_panel);
	apply_btn = hklglui->add_button_to_panel(hkl_set_panel,"Apply", 601, hkl_prop_control);
	hkl_d_label->set_alignment(GLUI_ALIGN_CENTER);
//--------------------------------------------------------------------------
	hkl_origin_panel = hklglui->add_panel_to_panel(hkl_main_panel,"Location",GLUI_PANEL_EMBOSSED);
	hkl_origin = hklglui->add_radiogroup_to_panel(hkl_origin_panel,NULL,604,hkl_prop_control);
	hklglui->add_radiobutton_to_group(hkl_origin,"Fixed");
	hklglui->add_radiobutton_to_group(hkl_origin,"Movable");
	hkl_origin_panel->set_alignment(GLUI_ALIGN_LEFT);
//--------------------------------------------------------------------------
	hklglui->add_separator_to_panel(hkl_main_panel);
	hkl_prev_check = hklglui->add_checkbox_to_panel(hkl_main_panel,"Preview",&hkl_preview);
	hkl_prev_check->set_int_val(1);
//--------------------------------------------------------------------------
	hklglui->add_column_to_panel(hkl_main_panel,false);
//--------------------------------------------------------------------------
	hkl_color_panel = hklglui->add_panel_to_panel(hkl_main_panel,"Color",GLUI_PANEL_EMBOSSED);
	hkl_color_R = hklglui->add_spinner_to_panel(hkl_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL,602,hkl_prop_control);
	hkl_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	hkl_color_G = hklglui->add_spinner_to_panel(hkl_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL,602,hkl_prop_control);
	hkl_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	hkl_color_B = hklglui->add_spinner_to_panel(hkl_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL,602,hkl_prop_control);
	hkl_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	hklglui->add_separator_to_panel(hkl_color_panel);
	hcolor_palette_btn = hklglui->add_button_to_panel(hkl_color_panel,"Palette",700,hkl_prop_control);
//--------------------------------------------------------------------------
	hkl_trans_spin = hklglui->add_spinner_to_panel(hkl_main_panel,"Opacity (%): ",GLUI_SPINNER_INT,NULL,602,hkl_prop_control);
	hkl_trans_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	hkl_trans_spin->set_alignment(GLUI_ALIGN_RIGHT);
	hkl_o_width = hklglui->add_edittext_to_panel(hkl_main_panel,"Outline width: ",GLUI_EDITTEXT_FLOAT,NULL,600,hkl_prop_control);
	hkl_o_width->set_alignment(GLUI_ALIGN_RIGHT);
//--------------------------------------------------------------------------
	hklglui->add_separator_to_panel(hkl_main_panel);
	hkl_inv_show_plane = hklglui->add_checkbox_to_panel(hkl_main_panel,"Show (-h -k -l) plane", NULL,600,hkl_prop_control);
	hkl_show_out_plane = hklglui->add_checkbox_to_panel(hkl_main_panel,"Show outlines", NULL,600,hkl_prop_control);
//--------------------------------------------------------------------------
	hkl_btn_panel = hklglui->add_panel("",GLUI_PANEL_NONE);
	hkl_OK_btn = hklglui->add_button_to_panel(hkl_btn_panel,"OK", 0, hkl_prop_control);
	hklglui->add_column_to_panel(hkl_btn_panel,false);
	hkl_Cancel_btn = hklglui->add_button_to_panel(hkl_btn_panel,"Cancel", 1, hkl_prop_control);

	init_hkl_prop_dialog();
}

