//******************************************************************************//
//                      POLYHEDRA PROPERTIES DIALOG                             //
//------------------------------------------------------------------------------//
// File: poly.cpp                                                               //
// Date: 2002.01.15                                                             //
//------------------------------------------------------------------------------//
// Control:                                                                     //
//   poly_proper_dialog_ID = -1 -- Open Polyhedra Properties Dialog             //
//                            1 -- Close Polyhedra Properties Dialog ("OK")     //
//                            0 -- Close Polyhedra Properties Dialog ("Cancel") //
//                                                                              //
//******************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>


// Objects
GLUI *polyglui;                                //--- POLY Propertires GLUI window
GLUI_Panel *poly_main_panel;                   //--- POLY Main Panel
GLUI_Panel *poly_type_panel;                   //--- POLY Type Panel
GLUI_Panel *poly_outline_color_panel;          //--- POLY Outline Colors Panel
GLUI_Panel *poly_btn_panel;                    //--- ATOMS "OK"/"Cancel" Panel

GLUI_Button *poly_type_btn;                    //--- POLY Type Button
GLUI_Spinner *poly_trans_spinner;              //--- POLY Opacity 
GLUI_Checkbox *poly_show_outline;              //--- POLY Show Outline ID
GLUI_EditText *poly_o_width;                   //--- POLY Outline Width         

GLUI_Spinner *poly_outlineR_spinner;           //--- POLY Outline Colors(RGB)
GLUI_Spinner *poly_outlineG_spinner;
GLUI_Spinner *poly_outlineB_spinner;

GLUI_Checkbox *poly_prev_check;                //--- POLY Preview Checkbox;
GLUI_Button *OK_btn, *Cancel_btn;

// Control
int poly_proper_Dialog_ID;                    //--- POLY dialog control ID
int poly_preview = 1;                         //--- POLY properties preview control 
int old_poly_type, Poly_type;                 //--- POLY Type
float old_pOpasity, pOpasity;                 //--- POLY Opacity
float old_pLineW, pLineW;                     //--- POLY Outline Width
float old_pLineRGB[3], pLineRGB[3];           //--- POLY Outline Colors (RGB)
int old_pShowLine, pShowLine;                 //--- POLY Show Outline Control

int parent_window;

//--- Color Palette
GLUI_Button *pcolor_palette_btn;
int poly_parent_window;

/**************************************** get_poly_dialog_pos() ************/
void get_poly_dialog_pos(int &x, int &y)
{
	glutSetWindow(polyglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}



//********************  set_poly_prop_dialog_id()  **************************//
void set_poly_prop_dialog_id(int ID)
{
	poly_proper_Dialog_ID = ID;
}

//********************  get_poly_prop_dialog_id()  **************************//
int get_poly_prop_dialog_id()
{
	return(poly_proper_Dialog_ID);
}

//********************  get_poly_prop_preview_id()  **************************//
int get_poly_prop_preview_id()
{
	return(poly_preview);
}


//********************  get_poly_type()  ************************************//
int get_poly_type()
{
	return(Poly_type);
}

//********************  set_poly_type()  ************************************//
void set_poly_type(int type)
{
	Poly_type = type;
	if ((Poly_type == 0) || (Poly_type == 5)) poly_outline_color_panel->disable();
	else poly_outline_color_panel->enable();
	pcolor_palette_btn->enable();
	poly_type_panel->enable();
	OK_btn->enable();
	Cancel_btn->enable();
}



//********************  get_poly_colors()  **********************************//
void get_poly_colors(float RGB[3])
{
	RGB[0] = pLineRGB[0] = (float)poly_outlineR_spinner->get_int_val()/100;
	RGB[1] = pLineRGB[1] = (float)poly_outlineG_spinner->get_int_val()/100;
	RGB[2] = pLineRGB[2] = (float)poly_outlineB_spinner->get_int_val()/100;
}

//********************  set_poly_colors()  **********************************//
void set_poly_colors(float RGB[3])
{
	pLineRGB[0] = RGB[0];
	pLineRGB[1] = RGB[1];
	pLineRGB[2] = RGB[2];
	poly_outlineR_spinner->set_int_val((int)(pLineRGB[0]*100));
	poly_outlineG_spinner->set_int_val((int)(pLineRGB[1]*100));
	poly_outlineB_spinner->set_int_val((int)(pLineRGB[2]*100));
	OK_btn->enable();
	Cancel_btn->enable();
	pcolor_palette_btn->enable();
	poly_type_panel->enable();
}



/**************************************** get_poly_prop_colors_R() **********/
float get_poly_prop_colors_R()
{
	return(pLineRGB[0]);
}

/**************************************** get_poly_prop_colors_G() **********/
float get_poly_prop_colors_G()
{
	return(pLineRGB[1]);
}

/**************************************** get_poly_prop_colors_B() **********/
float get_poly_prop_colors_B()
{
	return(pLineRGB[2]);
}

/**************************************** get_poly_prop_main_param() ********/
void get_poly_prop_main_param(int &Model, float &Opacity, float &lineW, int &ShowLine)
{
	Model = Poly_type;
	Opacity = pOpasity;
	lineW = pLineW;
	ShowLine = pShowLine;
}


/**************************************** poly_prop_control() ***************/
void poly_prop_control( int control)
{
	switch(control)
	{
	case(0):{poly_proper_Dialog_ID = 1; polyglui->close(); break; }
	case(1):{
		poly_proper_Dialog_ID = 0; 
		Poly_type = old_poly_type;
		pOpasity = old_pOpasity;
		pLineW = old_pLineW;
		pLineRGB[0] = old_pLineRGB[0];
		pLineRGB[1] = old_pLineRGB[1];
		pLineRGB[2] = old_pLineRGB[2];
		pShowLine = old_pShowLine;
		polyglui->close(); 
		break; 
			}
	case(402):{poly_proper_Dialog_ID = 402;pOpasity = (float)poly_trans_spinner->get_int_val()/100;break;}
	case(403):{poly_proper_Dialog_ID = 403;pLineW = poly_o_width->get_float_val();break;}
	case(404):{poly_proper_Dialog_ID = 404;pShowLine = poly_show_outline->get_int_val();break;}
	case(405):{
		poly_proper_Dialog_ID = 405;
		pLineRGB[0] = (float)poly_outlineR_spinner->get_int_val()/100;
		pLineRGB[1] = (float)poly_outlineG_spinner->get_int_val()/100;
		pLineRGB[2] = (float)poly_outlineB_spinner->get_int_val()/100;
		break;
			  }
	case(500):{
		poly_proper_Dialog_ID = 500;
		OK_btn->disable();
		Cancel_btn->disable();
		pcolor_palette_btn->disable();
		poly_type_panel->disable();
		break;
			  }
	case(600):{
		poly_proper_Dialog_ID = 600;
		OK_btn->disable();
		Cancel_btn->disable();
		pcolor_palette_btn->disable();
		poly_type_panel->disable();
		break;
			  }
	}
}

/**************************************** set_poly_prop() *******************/
void set_poly_prop(int Model, float Opacity, float lineW, float lineRGB[3], int ShowLine)
{
	old_poly_type = Poly_type = Model;
	old_pOpasity = pOpasity = Opacity;
	old_pLineW = pLineW = lineW;
	old_pLineRGB[0] = pLineRGB[0] = lineRGB[0];
	old_pLineRGB[1] = pLineRGB[1] = lineRGB[1];
	old_pLineRGB[2] = pLineRGB[2] = lineRGB[2];
	old_pShowLine = pShowLine = ShowLine;
}


/**************************************** init_poly_prop_dialog() ***********/
void init_poly_prop_dialog()
{
	poly_trans_spinner->set_int_val((int)(pOpasity*100));
	poly_show_outline->set_int_val(pShowLine);
	poly_o_width->set_float_val(pLineW);

	poly_outlineR_spinner->set_int_val((int)(pLineRGB[0]*100));
	poly_outlineG_spinner->set_int_val((int)(pLineRGB[1]*100));
	poly_outlineB_spinner->set_int_val((int)(pLineRGB[2]*100));

}

/**************************************** open_poly_prop_dialog() **********/
void open_poly_prop_dialog(int ID, int dpX, int dpY)
{
	parent_window = ID;
	poly_proper_Dialog_ID = -1;

	polyglui = GLUI_Master.create_glui("Polyhedra",0,dpX,dpY );
	polyglui->set_main_gfx_window( ID );

	poly_main_panel = polyglui->add_panel("",GLUI_PANEL_RAISED);
	poly_main_panel->set_alignment(GLUI_ALIGN_LEFT);

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
	poly_type_panel = polyglui->add_panel_to_panel(poly_main_panel," ",GLUI_PANEL_EMBOSSED);

	poly_type_btn = polyglui->add_button_to_panel(poly_type_panel,"Polyhedral style",600,poly_prop_control);
    polyglui->add_separator_to_panel(poly_type_panel);

	poly_trans_spinner = polyglui->add_spinner_to_panel(poly_type_panel,"Opacity (%): ",GLUI_SPINNER_INT,
                                                                        NULL,402,poly_prop_control);
	poly_trans_spinner->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	poly_trans_spinner->set_alignment(GLUI_ALIGN_LEFT);

	poly_o_width = polyglui->add_edittext_to_panel(poly_type_panel,"Outline width: ",GLUI_EDITTEXT_FLOAT,
		                                                                NULL,403,poly_prop_control);
	poly_o_width->set_alignment(GLUI_ALIGN_LEFT);

    polyglui->add_separator_to_panel(poly_type_panel);
	poly_show_outline = polyglui->add_checkbox_to_panel(poly_type_panel,"Show outlines",
		                                                                NULL,404,poly_prop_control);

	poly_prev_check = polyglui->add_checkbox_to_panel(poly_main_panel,"Preview",&poly_preview);
	poly_prev_check->set_int_val(1);

//--------------------------------------------------------------------------
	polyglui->add_column_to_panel(poly_main_panel,false);
//--------------------------------------------------------------------------
    poly_outline_color_panel = polyglui->add_panel_to_panel(poly_main_panel,"Outline color",GLUI_PANEL_EMBOSSED);
	poly_outlineR_spinner = polyglui->add_spinner_to_panel(poly_outline_color_panel,"Red (%): ",GLUI_SPINNER_INT,
                                                                                       NULL,405,poly_prop_control);
	poly_outlineG_spinner = polyglui->add_spinner_to_panel(poly_outline_color_panel,"Green (%): ",GLUI_SPINNER_INT,
                                                                                       NULL,405,poly_prop_control);
	poly_outlineB_spinner = polyglui->add_spinner_to_panel(poly_outline_color_panel,"Blue (%): ",GLUI_SPINNER_INT,
                                                                                       NULL,405,poly_prop_control);
	poly_outlineR_spinner->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	poly_outlineG_spinner->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	poly_outlineB_spinner->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	polyglui->add_separator_to_panel(poly_outline_color_panel);
	pcolor_palette_btn = polyglui->add_button_to_panel(poly_outline_color_panel,"Palette",500,poly_prop_control);

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
	poly_btn_panel = polyglui->add_panel("",GLUI_PANEL_NONE);
	OK_btn = polyglui->add_button_to_panel(poly_btn_panel,"OK", 0, poly_prop_control);
	polyglui->add_column_to_panel(poly_btn_panel,false);
	Cancel_btn = polyglui->add_button_to_panel(poly_btn_panel, "Cancel", 1, poly_prop_control);
	
	init_poly_prop_dialog();
}
