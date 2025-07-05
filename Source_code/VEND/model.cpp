//***************************************************************************//
//                      MODEL PROPERTIES DIALOG                              //
//---------------------------------------------------------------------------//
// File: atoms.cpp                                                           //
// Date: 2002.08.14                                                          //
//---------------------------------------------------------------------------//

#include <GL/glui.h>
#include <GL/glut.h>
#include "vend.h"

//-----------------------------------------------------
//  Atoms Specifications
//-----------------------------------------------------
ATOMSPEC typeAtom[MAX_ATOMS_SPEC];
int numOfType;                  //--- Number of Atomic Specifications

// Objects
GLUI *modelglui;
GLUI_Panel *model_main_panel;
GLUI_Panel *model_btn_panel;
GLUI_Button *model_OK_btn;
GLUI_Button *model_Cancel_btn;

//-----  Model
GLUI_Panel *model_type_panel;
GLUI_RadioGroup *model_type_radio;              //--- BONDS Type Radio
//-----  Atoms
GLUI_Panel *atom_label_panel;                  //--- ATOMS Label Panel
GLUI_Panel *atom_color_panel;                  //--- ATOMS Colors Panel
GLUI_Listbox *atom_label_lst;                  //--- ATOMS Names List
GLUI_Spinner *atom_radius_spin;                //--- ATOMS Radius
GLUI_Spinner *atom_stack_spin;                 //--- ATOMS Stacks
GLUI_Spinner *atom_slice_spin;                 //--- ATOMS Slices 
GLUI_Spinner *atom_R_color_spin;               //--- ATOMS Colors (RGB)
GLUI_Spinner *atom_G_color_spin;
GLUI_Spinner *atom_B_color_spin;
GLUI_Button *acolor_palette_btn;
//-----  Bonds
GLUI_Panel *bond_par_panel;                    //--- BONDS Paraneters panel
GLUI_Panel *bond_color_panel;                  //--- BONDS Colors Panel
GLUI_Panel *bond_type_panel;                   //--- BONDS Type Panel
GLUI_Spinner *bond_radius1_spin;               //--- BONDS Radius (B&S)
GLUI_Spinner *bond_radius2_spin;               //--- BONDS Radius (Stick)
GLUI_Spinner *bond_stack_spin;                 //--- BONDS Stacks
GLUI_Spinner *bond_slice_spin;                 //--- BONDS Slices 
GLUI_Spinner *bond_R_color_spin;               //--- BONDS Colors (RGB)
GLUI_Spinner *bond_G_color_spin;
GLUI_Spinner *bond_B_color_spin;
GLUI_Button *bcolor_palette_btn;
GLUI_RadioGroup *bond_type_radio;              //--- BONDS Type Radio

// Control
int model_proper_Dialog_ID;
int model_parent_window;
int model_palette_ID;

int mType;
int aStack,aSlice,bStack,bSlice;
float bRad[2],bRGB[3];
int bond_type;
int atom_label_ID;


float mRGB[3];

/**************************************** get_model_dialog_pos() ************/
void get_model_dialog_pos(int &x, int &y)
{
	glutSetWindow(modelglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}



//********************  set_model_proper_dialog_id()  ***********************//
void set_model_proper_dialog_id(int ID)
{
	model_proper_Dialog_ID = ID;
}

//********************  get_model_atom_colors()  ****************************//
void get_model_atom_colors(float RGB[3])
{
	RGB[0] = typeAtom[atom_label_ID].color[0];
	RGB[1] = typeAtom[atom_label_ID].color[1];
	RGB[2] = typeAtom[atom_label_ID].color[2];
}

/**************************************** get_model_prop_params() ***********/
void get_model_prop_params(int &model, int &Stack1, int &Slice1, int &Stack2, int &Slice2,  
						   float bColor[3],float &bR1,float &bR2,int &tbond)
{
	Stack1 = aStack = atom_stack_spin->get_int_val();
	Slice1 = aSlice = atom_slice_spin->get_int_val();
	Stack2 = bStack = bond_stack_spin->get_int_val();
	Slice2 = bSlice = bond_slice_spin->get_int_val();
	bR1 = bRad[0] = bond_radius1_spin->get_float_val();
	bR2 = bRad[1] = bond_radius2_spin->get_float_val();
	bRGB[0] = (float)bond_R_color_spin->get_int_val()/100;
	bRGB[1] = (float)bond_G_color_spin->get_int_val()/100;
	bRGB[2] = (float)bond_B_color_spin->get_int_val()/100;
	bColor[0] = bRGB[0];
	bColor[1] = bRGB[1];
	bColor[2] = bRGB[2];
	mType = model_type_radio->get_int_val();
	if (mType == 1) model = 4;
	else model = 0;
	tbond = bond_type = bond_type_radio->get_int_val();
}

/**************************************** get_model_atom_params() ***********/
void get_model_atom_params(int ID, float RGB[3], float &R)
{
	RGB[0] = typeAtom[ID].color[0];
	RGB[1] = typeAtom[ID].color[1];
	RGB[2] = typeAtom[ID].color[2];
	R = typeAtom[ID].R;
}



//********************  set_model_atom_colors()  ****************************//
void set_model_atom_colors(float RGB[3])
{
	typeAtom[atom_label_ID].color[0] = RGB[0];
	typeAtom[atom_label_ID].color[1] = RGB[1];
	typeAtom[atom_label_ID].color[2] = RGB[2];
	atom_R_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[0]*100));
	atom_G_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[1]*100));
	atom_B_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[2]*100));
	acolor_palette_btn->enable();
	bcolor_palette_btn->enable();
	model_Cancel_btn->enable();
	model_OK_btn->enable();
}


//********************  get_model_bond_colors()  ****************************//
void get_model_bond_colors(float RGB[3])
{
	bRGB[0] = (float)bond_R_color_spin->get_int_val()/100;
	bRGB[1] = (float)bond_G_color_spin->get_int_val()/100;
	bRGB[2] = (float)bond_B_color_spin->get_int_val()/100;
	RGB[0] = bRGB[0];
	RGB[1] = bRGB[1];
	RGB[2] = bRGB[2];
}

//********************  set_model_bond_colors()  ****************************//
void set_model_bond_colors(float RGB[3])
{
	bRGB[0] = RGB[0];
	bRGB[1] = RGB[1];
	bRGB[2] = RGB[2];
	bond_R_color_spin->set_int_val((int)(bRGB[0]*100));
	bond_G_color_spin->set_int_val((int)(bRGB[1]*100));
	bond_B_color_spin->set_int_val((int)(bRGB[2]*100));
	acolor_palette_btn->enable();
	bcolor_palette_btn->enable();
	model_Cancel_btn->enable();
	model_OK_btn->enable();
}


//********************  get_model_proper_dialog_id()  ***********************//
int get_model_proper_dialog_id()
{
	return(model_proper_Dialog_ID);
}


/**************************************** model_prop_control() ***************/
void model_prop_control( int control)
{
	switch(control)
	{
	case(0):{model_proper_Dialog_ID = 1; modelglui->close();break;}
	case(1):{model_proper_Dialog_ID = 0; modelglui->close();break;}
	case(10):{
		atom_label_ID = atom_label_lst->get_int_val();
		atom_radius_spin->set_float_val(typeAtom[atom_label_ID].R);
		atom_R_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[0]*100));
		atom_G_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[1]*100));
		atom_B_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[2]*100));
		break;
			 }
	case(11):{
		model_proper_Dialog_ID = -1;
		typeAtom[atom_label_ID].R = atom_radius_spin->get_float_val();
		break;
			 }
	case(12):{
		model_proper_Dialog_ID = -1;
		typeAtom[atom_label_ID].color[0] = (float)atom_R_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[1] = (float)atom_G_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[2] = (float)atom_B_color_spin->get_int_val()/100;
		break;
			 }
	case(15):{
		model_proper_Dialog_ID = 15;
		typeAtom[atom_label_ID].color[0] = (float)atom_R_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[1] = (float)atom_G_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[2] = (float)atom_B_color_spin->get_int_val()/100;
		acolor_palette_btn->disable();
		bcolor_palette_btn->disable();
		model_Cancel_btn->disable();
		model_OK_btn->disable();
		break;
			 }
	case(20):{
		model_proper_Dialog_ID = 20;
		acolor_palette_btn->disable();
		bcolor_palette_btn->disable();
		model_Cancel_btn->disable();
		model_OK_btn->disable();
		break;
			 }
	}
}


/**************************************** set_model_prop_params() ***********/
void set_model_prop_params(int model, int Stack1, int Slice1, int Stack2, int Slice2,  
						   float bColor[3],float bR1,float bR2,int Num, int tbond)
{
	aStack = Stack1;
	aSlice = Slice1;
	bStack = Stack2;
	bSlice = Slice2;
	bRad[0] = bR1;
	bRad[1] = bR2;
	bRGB[0] = bColor[0];
	bRGB[1] = bColor[1];
	bRGB[2] = bColor[2];

	if (model == 4) mType = 1;
	else mType = 0;

	numOfType = Num;
	bond_type = tbond;
}

/**************************************** set_model_prop_params() ***********/
void set_model_atom_params(int ID, char *symb, float RGB[3], float R)
{
	typeAtom[ID].color[0] = RGB[0];
	typeAtom[ID].color[1] = RGB[1];
	typeAtom[ID].color[2] = RGB[2];
	typeAtom[ID].R = R;

	sprintf(typeAtom[ID].symb,"");
	sprintf(typeAtom[ID].symb,"%s",symb);
}


/**************************************** init_model_prop_dialog() **********/
void init_model_prop_dialog()
{
	int i;

	atom_label_ID = 0;
	for(i=0; i<numOfType; i++) atom_label_lst->add_item(i,typeAtom[i].symb);
	atom_label_lst->set_int_val(atom_label_ID);
	atom_radius_spin->set_float_val(typeAtom[0].R);
	atom_R_color_spin->set_int_val((int)(typeAtom[0].color[0]*100));
	atom_G_color_spin->set_int_val((int)(typeAtom[0].color[1]*100));
	atom_B_color_spin->set_int_val((int)(typeAtom[0].color[2]*100));

	atom_stack_spin->set_int_val(aStack);
	atom_slice_spin->set_int_val(aSlice);

	bond_radius1_spin->set_float_val(bRad[0]);
	bond_radius2_spin->set_float_val(bRad[1]);
	bond_stack_spin->set_int_val(bStack);
	bond_slice_spin->set_int_val(bSlice);
	bond_R_color_spin->set_int_val((int)(bRGB[0]*100));
	bond_G_color_spin->set_int_val((int)(bRGB[1]*100));
	bond_B_color_spin->set_int_val((int)(bRGB[2]*100));
	bond_type_radio->set_int_val(bond_type);
	model_type_radio->set_int_val(mType);
}


/**************************************** open_model_prop_dialog() **********/
void open_model_prop_dialog(int ID, int dpX, int dpY)
{
	model_proper_Dialog_ID = -1;
	model_parent_window = ID;
	model_palette_ID = 0;

	modelglui = GLUI_Master.create_glui("Model",0,dpX,dpY );
	modelglui->set_main_gfx_window( ID );

	model_main_panel = modelglui->add_panel("",GLUI_PANEL_RAISED);
	model_main_panel->set_alignment(GLUI_ALIGN_LEFT);

//--------------------------------------------------------------------------
	atom_label_panel = modelglui->add_panel_to_panel(model_main_panel,"Atoms",GLUI_PANEL_EMBOSSED);
	
	atom_label_lst = modelglui->add_listbox_to_panel(atom_label_panel,"Symbol: ",NULL,10,model_prop_control);
	atom_label_lst->set_alignment(GLUI_ALIGN_RIGHT);
	
	modelglui->add_separator_to_panel(atom_label_panel);

	atom_radius_spin = modelglui->add_spinner_to_panel(atom_label_panel,"Radius:",GLUI_SPINNER_FLOAT,NULL,
		                                               11,model_prop_control);
	atom_radius_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	atom_radius_spin->set_speed(0.1);
	atom_radius_spin->set_alignment(GLUI_ALIGN_RIGHT);
	atom_stack_spin = modelglui->add_spinner_to_panel(atom_label_panel,"Stacks: ",GLUI_SPINNER_INT,NULL);
	atom_stack_spin->set_int_limits(1,50,GLUI_LIMIT_CLAMP);
	atom_stack_spin->set_alignment(GLUI_ALIGN_RIGHT);
	atom_slice_spin = modelglui->add_spinner_to_panel(atom_label_panel,"Slices: ",GLUI_SPINNER_INT,NULL);
	atom_slice_spin->set_int_limits(3,50,GLUI_LIMIT_CLAMP);
	atom_slice_spin->set_alignment(GLUI_ALIGN_RIGHT);

	atom_color_panel = modelglui->add_panel_to_panel(atom_label_panel,"Color",GLUI_PANEL_EMBOSSED);
	atom_R_color_spin = modelglui->add_spinner_to_panel(atom_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL,
		                                               12,model_prop_control);
	atom_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	atom_G_color_spin = modelglui->add_spinner_to_panel(atom_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL,
		                                               12,model_prop_control);
	atom_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	atom_B_color_spin = modelglui->add_spinner_to_panel(atom_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL,
		                                               12,model_prop_control);
	atom_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	modelglui->add_separator_to_panel(atom_color_panel);
	acolor_palette_btn = modelglui->add_button_to_panel(atom_color_panel,"Palette",15,model_prop_control);
//--------------------------------------------------------------------------
	model_type_panel = modelglui->add_panel_to_panel(model_main_panel,"Model",GLUI_PANEL_EMBOSSED);
	model_type_radio = modelglui->add_radiogroup_to_panel(model_type_panel,NULL);
	modelglui->add_radiobutton_to_group(model_type_radio,"Ball-and-stick");
	modelglui->add_radiobutton_to_group(model_type_radio,"Stick");
//--------------------------------------------------------------------------
	modelglui->add_column_to_panel(model_main_panel,false);
//--------------------------------------------------------------------------

	bond_par_panel = modelglui->add_panel_to_panel(model_main_panel,"Bonds",GLUI_PANEL_EMBOSSED);

	bond_radius1_spin = modelglui->add_spinner_to_panel(bond_par_panel,"Radius (B&S): ",GLUI_SPINNER_FLOAT,NULL);
	bond_radius1_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	bond_radius1_spin->set_speed(0.1);
	bond_radius1_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bond_radius2_spin = modelglui->add_spinner_to_panel(bond_par_panel,"Radius (stick): ",GLUI_SPINNER_FLOAT,NULL);
	bond_radius2_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	bond_radius2_spin->set_speed(0.1);
	bond_radius2_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bond_stack_spin = modelglui->add_spinner_to_panel(bond_par_panel,"Stacks: ",GLUI_SPINNER_INT,NULL);
	bond_stack_spin->set_int_limits(1,50,GLUI_LIMIT_CLAMP);
	bond_stack_spin->set_alignment(GLUI_ALIGN_RIGHT);
	bond_slice_spin = modelglui->add_spinner_to_panel(bond_par_panel,"Slices: ",GLUI_SPINNER_INT,NULL);
	bond_slice_spin->set_int_limits(3,50,GLUI_LIMIT_CLAMP);
	bond_slice_spin->set_alignment(GLUI_ALIGN_RIGHT);

	bond_color_panel = modelglui->add_panel_to_panel(bond_par_panel,"Color",GLUI_PANEL_EMBOSSED);
	bond_R_color_spin = modelglui->add_spinner_to_panel(bond_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL);
	bond_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	bond_G_color_spin = modelglui->add_spinner_to_panel(bond_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL);
	bond_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	bond_B_color_spin = modelglui->add_spinner_to_panel(bond_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL);
	bond_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	modelglui->add_separator_to_panel(bond_color_panel);
	bcolor_palette_btn = modelglui->add_button_to_panel(bond_color_panel,"Palette",20,model_prop_control);

	bond_type_panel = modelglui->add_panel_to_panel(bond_par_panel,"Bond style",GLUI_PANEL_EMBOSSED);
	bond_type_radio = modelglui->add_radiogroup_to_panel(bond_type_panel,NULL,30,model_prop_control);
	modelglui->add_radiobutton_to_group(bond_type_radio,"Unicolor cylinder");
	modelglui->add_radiobutton_to_group(bond_type_radio,"Bicolor cylinder");


//--------------------------------------------------------------------------
	model_btn_panel = modelglui->add_panel("",GLUI_PANEL_NONE);
	model_OK_btn = modelglui->add_button_to_panel(model_btn_panel,"OK", 0, model_prop_control);
	modelglui->add_column_to_panel(model_btn_panel,false);
	model_Cancel_btn = modelglui->add_button_to_panel(model_btn_panel,"Cancel", 1, model_prop_control);

	init_model_prop_dialog();

}
