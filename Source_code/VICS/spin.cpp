//***************************************************************************//
//                          ATOM VECTOR DIALOG                               //
//---------------------------------------------------------------------------//
// File: spin.cpp                                                            //
// Date: 2002.06.10                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   spin_dialog_ID = -1 -- Open Spin Dialog                                 //
//                     1 -- Close Spin Dialog ("OK")                         //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>

// Objects
GLUI *spinglui;  
GLUI_Panel *spin_main_panel;
GLUI_Panel *spin_dir_panel;
GLUI_Panel *spin_color_panel;

GLUI_EditText *spin_U_txt;
GLUI_EditText *spin_V_txt;
GLUI_EditText *spin_W_txt;

GLUI_Spinner *spin_color_R;
GLUI_Spinner *spin_color_G;
GLUI_Spinner *spin_color_B;

GLUI_Spinner *spin_length_spin;
GLUI_Button *spin_show_btn;
GLUI_Button *spin_OK_btn;

GLUI_Checkbox *spin_show_check;

// Control
int spin_Dialog_ID;
float spin_UVW[3];
float spin_Color[3];
float spin_L;
int atom_spin_Num;
int spin_show;

//--- Color Palette
GLUI_Button *scolor_palette_btn;
int spin_parent_window;
int spin_palette_ID;

/**************************************** get_spin_dialog_pos() ************/
void get_spin_dialog_pos(int &x, int &y)
{
	glutSetWindow(spinglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


/**************************************** get_spin_color() ******************/
void set_spin_color(float RGB[3])
{
	spin_Color[0] = RGB[0];
	spin_Color[1] = RGB[1];
	spin_Color[2] = RGB[2];
	spin_color_R->set_int_val((int)(spin_Color[0]*100));
	spin_color_G->set_int_val((int)(spin_Color[1]*100));
	spin_color_B->set_int_val((int)(spin_Color[2]*100));
	scolor_palette_btn->enable();
	spin_show_btn->enable();
	spin_OK_btn->enable();
}


/**************************************** set_spin_color() ******************/
void get_spin_color(float RGB[3])
{
	RGB[0] = (float)spin_color_R->get_int_val()/100;
	RGB[1] = (float)spin_color_R->get_int_val()/100;
	RGB[2] = (float)spin_color_R->get_int_val()/100;
}

/**************************************** get_spin_atom_num() ***************/
int get_spin_atom_num()
{
	return(atom_spin_Num);
}

/**************************************** get_spin_dialog_param() ***********/
void get_spin_dialog_param(short &show_ID, float &H, float Dir[3], float RGB[3])
{

	show_ID = spin_show = spin_show_check->get_int_val();

	H = spin_L = spin_length_spin->get_float_val();
	RGB[0] = spin_Color[0] = (float)spin_color_R->get_int_val()/100;
	RGB[1] = spin_Color[1] = (float)spin_color_G->get_int_val()/100;
	RGB[2] = spin_Color[2] = (float)spin_color_B->get_int_val()/100;

	Dir[0] = spin_UVW[0] = spin_U_txt->get_float_val();
	Dir[1] = spin_UVW[1] = spin_V_txt->get_float_val();
	Dir[2] = spin_UVW[2] = spin_W_txt->get_float_val();
}


/**************************************** get_spin_dialog_id() **************/
void set_spin_dialog_id(int ID)
{
	spin_Dialog_ID = ID;
}


/**************************************** get_spin_dialog_id() **************/
int get_spin_dialog_id()
{
	return(spin_Dialog_ID);
}


/**************************************** spin_control() ********************/
void spin_control( int control)
{
	switch(control)
	{
	case(0):{
		spin_Dialog_ID = 1; spinglui->close(); 
		break; 
			}
	case(10):{
		spin_Dialog_ID = 10;
		break; 
			 }
	case(20):{
		spin_Dialog_ID = 20;
		spin_Color[0] = (float)spin_color_R->get_int_val()/100;
		spin_Color[1] = (float)spin_color_G->get_int_val()/100;
		spin_Color[2] = (float)spin_color_B->get_int_val()/100;
		scolor_palette_btn->disable();
		spin_show_btn->disable();
		spin_OK_btn->disable();
		break;
			 }
	}
}

/**************************************** set_spin_dialog_param() ***********/
void set_spin_dialog_param(int Num, short show_ID, float H, float Dir[3], float RGB[3])
{
	int i;

	atom_spin_Num = Num;
	spin_show = show_ID;
	spin_L = H;
	for(i=0; i<3; i++) {
		spin_Color[i] = RGB[i];
		spin_UVW[i] = Dir[i];
	}

}

/**************************************** init_spin_dialog() ****************/
void init_spin_dialog()
{
	spin_show_check->set_int_val(spin_show);

	spin_U_txt->set_float_val(spin_UVW[0]);
	spin_V_txt->set_float_val(spin_UVW[1]);
	spin_W_txt->set_float_val(spin_UVW[2]);

	spin_color_R->set_int_val((int)(spin_Color[0]*100));
	spin_color_G->set_int_val((int)(spin_Color[1]*100));
	spin_color_B->set_int_val((int)(spin_Color[2]*100));

	spin_length_spin->set_float_val(spin_L);
}


/**************************************** open_spin_dialog() *****************/
void open_spin_dialog(int ID, int dpX, int dpY)
{
	spin_Dialog_ID = -1;
	spin_parent_window = ID;
	spin_palette_ID = 0;

	spinglui = GLUI_Master.create_glui("Define Vector",0,dpX,dpY );
	spinglui->set_main_gfx_window( ID );

	spin_main_panel = spinglui->add_panel("",GLUI_PANEL_RAISED);
//---------------------------------------------------------------------
	spin_dir_panel = spinglui->add_panel_to_panel(spin_main_panel,"Direction",GLUI_PANEL_EMBOSSED);
	spin_U_txt  = spinglui->add_edittext_to_panel(spin_dir_panel,"u: ",GLUI_EDITTEXT_FLOAT,&spin_UVW[0]);
	spin_V_txt  = spinglui->add_edittext_to_panel(spin_dir_panel,"v: ",GLUI_EDITTEXT_FLOAT,&spin_UVW[1]);
	spin_W_txt  = spinglui->add_edittext_to_panel(spin_dir_panel,"w: ",GLUI_EDITTEXT_FLOAT,&spin_UVW[2]);
//---------------------------------------------------------------------
	spin_length_spin = spinglui->add_spinner_to_panel(spin_main_panel,"Length: ",GLUI_SPINNER_FLOAT,NULL);
	spin_length_spin->set_int_limits(0,2,GLUI_LIMIT_CLAMP);
//---------------------------------------------------------------------
	spinglui->add_column_to_panel(spin_main_panel,false);
//---------------------------------------------------------------------
	spin_color_panel = spinglui->add_panel_to_panel(spin_main_panel,"Color",GLUI_PANEL_EMBOSSED);
	spin_color_R = spinglui->add_spinner_to_panel(spin_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL);
	spin_color_G = spinglui->add_spinner_to_panel(spin_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL);
	spin_color_B = spinglui->add_spinner_to_panel(spin_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL);
	spin_color_R->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	spin_color_G->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	spin_color_B->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	spinglui->add_separator_to_panel(spin_color_panel);
	scolor_palette_btn = spinglui->add_button_to_panel(spin_color_panel,"Palette",20,spin_control);

//---------------------------------------------------------------------
	spinglui->add_column_to_panel(spin_main_panel,true);
//---------------------------------------------------------------------
    spin_show_check = spinglui->add_checkbox_to_panel(spin_main_panel,"Show vector");
	spinglui->add_separator_to_panel(spin_main_panel);
//---------------------------------------------------------------------
	spin_show_btn = spinglui->add_button_to_panel(spin_main_panel,"Apply",10,spin_control);
//---------------------------------------------------------------------
	spin_OK_btn = spinglui->add_button("OK", 0, spin_control);

	init_spin_dialog();

}

