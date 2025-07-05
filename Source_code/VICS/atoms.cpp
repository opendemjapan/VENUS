//***************************************************************************//
//                      ATOMS PROPERTIES DIALOG                              //
//---------------------------------------------------------------------------//
// File: atoms.cpp                                                           //
// Date: 2002.01.11                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   atoms_proper_dialog_ID = -1 -- Open Atoms Properties Dialog             //
//                             1 -- Close Atoms Properties Dialog ("OK")     //
//                             0 -- Close Atoms Properties Dialog ("Cancel") //
//                           200 -- Stacks,Slices,...                        //
//                           201 -- Radius                                   //
//                           202 -- Colors (RGB)                             //
//                           203 -- Atom Show Type (Balls/Therm. Ellipsoid)  //
//                           204 -- Thermal Ellipsoids Probability           //
//                                                                           //
//   atom_type_ID = 0 --- Sphere                                             // 
//                  1 --- Thermal Ellipsoid                                  // 
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>
#include "vics.h"

//-----------------------------------------------------
//  Atoms Specifications
//-----------------------------------------------------
ATOMSPEC typeAtom[MAX_ATOMS_SPEC],old_typeAtom[MAX_ATOMS_SPEC];
int numOfType;                  //--- Number of Atomic Specifications


// Objects
GLUI *atomglui;                                //--- ATOMS Propertires GLUI window
GLUI_Panel *atom_main_panel;                   //--- ATOMS Main Panel
GLUI_Panel *atom_btn_panel;                    //--- ATOMS "OK"/"Cancel" Panel
GLUI_Panel *atom_label_panel;                  //--- ATOMS Label Panel
GLUI_Panel *atom_par_panel;                    //--- ATOMS Paraneters panel
GLUI_Panel *atom_color_panel;                  //--- ATOMS Colors Panel
GLUI_Listbox *atom_label_lst;                  //--- ATOMS Names List
GLUI_RadioGroup *atom_type_radio;              //--- ATOMS Type Radio
GLUI_Spinner *atom_stack_spin;                 //--- ATOMS Stacks
GLUI_Spinner *atom_slice_spin;                 //--- ATOMS Slices 
GLUI_Spinner *atom_radius_spin;                //--- ATOMS Radius
GLUI_Spinner *atom_R_color_spin;               //--- ATOMS Colors (RGB)
GLUI_Spinner *atom_G_color_spin;
GLUI_Spinner *atom_B_color_spin;
GLUI_Checkbox *atom_prev_check;                //--- ATOMS Preview Checkbox;
GLUI_Checkbox *atom_show_teout_check;          //--- ATOMS Show Thermal Ellipsoids Outline;
GLUI_Spinner *atom_thermal_prob_spin;          //--- ATOMS Thermal Ellipsoids Probability

GLUI_Button *atom_OK_btn;
GLUI_Button *atom_Cancel_btn;

// Live Variables
int aStack,old_aStack;                         //--- ATOMS Stacks  
int aSlice,old_aSlice;                         //--- ATOMS Slices
int aType, old_aType;                          //--- ATOMS Type (Sphere / Ellipsoid)
int tProb, old_tProb;                        //--- Thermal Ellipsoids Probability
int aOutline, old_aOutline;                    //--- Thermal Ellipsoids Outline

// Control
int atoms_proper_Dialog_ID;                    //--- ATOMS dialog control ID
int atoms_preview;                         //--- Atoms properties preview control 
int atom_label_ID;

int aniso_type;

//--- Color Palette
GLUI_Button *acolor_palette_btn;
int atom_parent_window;
int atom_palette_ID;
int H_D_ID[2];
int numHD;

/**************************************** get_atom_dialog_pos() ************/
void get_atom_dialog_pos(int &x, int &y)
{
	glutSetWindow(atomglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  set_atom_prop_dialog_id()  **************************//
void set_atom_prop_dialog_id(int ID)
{
	atoms_proper_Dialog_ID = ID;
}

//********************  get_atom_prop_dialog_id()  **************************//
int get_atom_prop_dialog_id()
{
	return(atoms_proper_Dialog_ID);
}

//********************  get_atom_prop_preview_id()  **************************//
int get_atom_prop_preview_id()
{
	return(atoms_preview);
}

/**************************************** set_atom_colors() *****************/
void set_atom_colors(float RGB[3])
{
	typeAtom[atom_label_ID].color[0] = RGB[0];
	typeAtom[atom_label_ID].color[1] = RGB[1];
	typeAtom[atom_label_ID].color[2] = RGB[2];
	atom_R_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[0]*100));
	atom_G_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[1]*100));
	atom_B_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[2]*100));
	acolor_palette_btn->enable();
	atom_Cancel_btn->enable();
	atom_OK_btn->enable();
}

/**************************************** get_atom_colors() *****************/
void get_atom_colors(float RGB[3])
{
	RGB[0] = typeAtom[atom_label_ID].color[0];
	RGB[1] = typeAtom[atom_label_ID].color[1];
	RGB[2] = typeAtom[atom_label_ID].color[2];
}

/**************************************** get_atom_prop_main_params() *******/
void get_atom_prop_main_params(int &Stack, int &Slice, int &Type, int &Outline, int &Prob)
{
	Stack = atom_stack_spin->get_int_val();
	Slice = atom_slice_spin->get_int_val();
	Prob = atom_thermal_prob_spin->get_int_val();
	Outline = atom_show_teout_check->get_int_val();
	Type = aType;
}


/**************************************** get_cur_R_RGB() ***************/
void get_type_R_RGB(int ID, float &R, float color[3])
{
	R = typeAtom[ID].R;
	color[0] = typeAtom[ID].color[0];
	color[1] = typeAtom[ID].color[1];
	color[2] = typeAtom[ID].color[2];
}

/**************************************** atom_prop_control() ***************/
void atom_prop_control( int control)
{
	int i;
	switch(control)
	{
	case(0):{
		atoms_proper_Dialog_ID = 1; atomglui->close(); 
		break; 
			}
	case(1):{
		aStack = old_aStack;
		aSlice = old_aSlice;
		aType = old_aType;
		tProb = old_tProb;
		aOutline = old_aOutline;
		for(i=0; i<numOfType; i++){
			typeAtom[i].color[0] = old_typeAtom[i].color[0];
			typeAtom[i].color[1] = old_typeAtom[i].color[1];
			typeAtom[i].color[2] = old_typeAtom[i].color[2];
			typeAtom[i].R = old_typeAtom[i].R;
		}
		atoms_proper_Dialog_ID = 0; 
		atomglui->close();  
		break; 
			}
	case(10):{
		atoms_proper_Dialog_ID = -1;
		atom_label_ID = atom_label_lst->get_int_val();
		atom_radius_spin->set_float_val(typeAtom[atom_label_ID].R); 
		atom_R_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[0]*100));
		atom_G_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[1]*100));
		atom_B_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[2]*100));
		aType = atom_type_radio->get_int_val();
		if (aType == 0){
			atom_radius_spin->enable();
			atom_thermal_prob_spin->disable();
		}
		else {
			atom_radius_spin->disable();
			atom_thermal_prob_spin->enable();
			for(i=0; i<numHD; i++){
				if (atom_label_ID == H_D_ID[i]) {
					atom_radius_spin->enable();
					atom_thermal_prob_spin->disable();
					break;
				}
			}
		}
		break;
			 }
	case(200):{
		atoms_proper_Dialog_ID = 200;
		aStack = atom_stack_spin->get_int_val();
		aSlice = atom_slice_spin->get_int_val();
		tProb = atom_thermal_prob_spin->get_int_val();
		aOutline = atom_show_teout_check->get_int_val();
		break;
			  }
	case(201):{
		atoms_proper_Dialog_ID = 201;
		typeAtom[atom_label_ID].R = atom_radius_spin->get_float_val();
		break;
			  }
	case(202):{
		atoms_proper_Dialog_ID = 202;
		typeAtom[atom_label_ID].color[0] = (float)atom_R_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[1] = (float)atom_G_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[2] = (float)atom_B_color_spin->get_int_val()/100;
		break;
			  }
	case(203):{
		atoms_proper_Dialog_ID = 203;
		aType = atom_type_radio->get_int_val();
		if (aType == 0){
			atom_radius_spin->enable();
			atom_thermal_prob_spin->disable();
		}
		else {
			atom_radius_spin->disable();
			atom_thermal_prob_spin->enable();
			for(i=0; i<numHD; i++){
				if (atom_label_ID == H_D_ID[i]) {
					atom_radius_spin->enable();
					atom_thermal_prob_spin->disable();
					break;
				}
			}
		}
		break;
			  }
	case(300):{
		atoms_proper_Dialog_ID = 300;
		typeAtom[atom_label_ID].color[0] = (float)atom_R_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[1] = (float)atom_G_color_spin->get_int_val()/100;
		typeAtom[atom_label_ID].color[2] = (float)atom_B_color_spin->get_int_val()/100;
		acolor_palette_btn->disable();
		atom_Cancel_btn->disable();
		atom_OK_btn->disable();
		break;
			  }
	}
}


/**************************************** set_atom_prop_params() ************/
void set_atom_prop_main_params(int Num, int Stack, int Slice,int Type, int Outline, int Prob,int check_aniso)
{
	numOfType = Num;
	old_aStack = aStack = Stack;
	old_aSlice = aSlice = Slice;
	old_aType = aType = Type;
	old_aOutline = aOutline = Outline;
	old_tProb = tProb = Prob;
	aniso_type = check_aniso;

}

/**************************************** set_atom_prop_params() ************/
void set_atom_prop_params(int ID, char Symb[3], float Rad, float RGB[3])
{
	old_typeAtom[ID].color[0] = typeAtom[ID].color[0] = RGB[0];
	old_typeAtom[ID].color[1] = typeAtom[ID].color[1] = RGB[1];
	old_typeAtom[ID].color[2] = typeAtom[ID].color[2] = RGB[2];
	old_typeAtom[ID].R = typeAtom[ID].R = Rad;
	sprintf(typeAtom[ID].symb,"");
	sprintf(typeAtom[ID].symb,"%s",Symb);
	sprintf(old_typeAtom[ID].symb,"");
	sprintf(old_typeAtom[ID].symb,"%s",Symb);
}

/**************************************** init_atom_prop_dialog() ***********/
void init_atom_prop_dialog()
{
	int i;

	atom_label_ID = 0;
	atoms_preview = 1;
	for(i=0; i<numOfType; i++) atom_label_lst->add_item(i,typeAtom[i].symb);
	atom_label_lst->set_int_val(atom_label_ID);
	atom_type_radio->set_int_val(aType);
	atom_stack_spin->set_int_val(aStack);
	atom_slice_spin->set_int_val(aSlice);
	atom_show_teout_check->set_int_val(aOutline);
	atom_radius_spin->set_float_val(typeAtom[atom_label_ID].R);
	atom_thermal_prob_spin->set_int_val(tProb);
	if (aType == 0){
		atom_radius_spin->enable();
		atom_thermal_prob_spin->disable();
	}
	else {
		atom_radius_spin->disable();
		atom_thermal_prob_spin->enable();
	}
    atom_R_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[0]*100));
	atom_G_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[1]*100));
	atom_B_color_spin->set_int_val((int)(typeAtom[atom_label_ID].color[2]*100));
	if (aniso_type == 0) {
		aType = 0;
		atom_type_radio->set_int_val(aType);
		atom_thermal_prob_spin->disable();
		atom_thermal_prob_spin->disable();
		atom_type_radio->disable();
		atom_show_teout_check->disable();
	}
	numHD = 0;
	for(i=0; i<numOfType; i++){
		if ((strstr(typeAtom[i].symb, "H") != NULL) && (strlen(typeAtom[i].symb) == 1)) {
			H_D_ID[numHD] = i;
            numHD++;
		}
		if ((strstr(typeAtom[i].symb, "D") != NULL) && (strlen(typeAtom[i].symb) == 1)) {
			H_D_ID[numHD] = i;
            numHD++;
		}
	}
}

/**************************************** open_atoms_prop_dialog() **********/
void open_atoms_prop_dialog(int ID, int dpX, int dpY)
{
	atoms_proper_Dialog_ID = -1;
	atom_parent_window = ID;
	atom_palette_ID = 0;

	atomglui = GLUI_Master.create_glui("Atoms",0,dpX,dpY );
	atomglui->set_main_gfx_window( ID );

	atom_main_panel = atomglui->add_panel("",GLUI_PANEL_RAISED);
	atom_main_panel->set_alignment(GLUI_ALIGN_LEFT);

//--------------------------------------------------------------------------
	atom_label_panel = atomglui->add_panel_to_panel(atom_main_panel,"",GLUI_PANEL_EMBOSSED);
	atom_label_lst = atomglui->add_listbox_to_panel(atom_label_panel,"Symbol: ",NULL,10,atom_prop_control);
	atom_label_lst->set_alignment(GLUI_ALIGN_RIGHT);
	atomglui->add_separator_to_panel(atom_label_panel);
	atom_radius_spin = atomglui->add_spinner_to_panel(atom_label_panel,"Radius:",GLUI_SPINNER_FLOAT,NULL,201,atom_prop_control);
	atom_radius_spin->set_float_limits(0,10,GLUI_LIMIT_CLAMP);
	atom_radius_spin->set_speed(0.1);
	atom_radius_spin->set_alignment(GLUI_ALIGN_RIGHT);
	atom_color_panel = atomglui->add_panel_to_panel(atom_label_panel,"Color",GLUI_PANEL_EMBOSSED);
	atom_R_color_spin = atomglui->add_spinner_to_panel(atom_color_panel,"Red (%): ",GLUI_SPINNER_INT,NULL,202,atom_prop_control);
	atom_R_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	atom_G_color_spin = atomglui->add_spinner_to_panel(atom_color_panel,"Green (%): ",GLUI_SPINNER_INT,NULL,202,atom_prop_control);
	atom_G_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	atom_B_color_spin = atomglui->add_spinner_to_panel(atom_color_panel,"Blue (%): ",GLUI_SPINNER_INT,NULL,202,atom_prop_control);
	atom_B_color_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);

	atomglui->add_separator_to_panel(atom_color_panel);
	acolor_palette_btn = atomglui->add_button_to_panel(atom_color_panel,"Palette",300,atom_prop_control);

//--------------------------------------------------------------------------
	atom_prev_check = atomglui->add_checkbox_to_panel(atom_main_panel,"Preview",&atoms_preview);
	atom_prev_check->set_int_val(1);
//--------------------------------------------------------------------------
	atomglui->add_column_to_panel(atom_main_panel,false);
//--------------------------------------------------------------------------
	atom_par_panel = atomglui->add_panel_to_panel(atom_main_panel,"Parameters",GLUI_PANEL_EMBOSSED);
	atom_stack_spin = atomglui->add_spinner_to_panel(atom_par_panel,"Stacks: ",GLUI_SPINNER_INT,NULL,200,atom_prop_control);
	atom_stack_spin->set_int_limits(1,50,GLUI_LIMIT_CLAMP);
	atom_slice_spin = atomglui->add_spinner_to_panel(atom_par_panel,"Slices: ",GLUI_SPINNER_INT,NULL,200,atom_prop_control);
	atom_slice_spin->set_int_limits(3,50,GLUI_LIMIT_CLAMP);
	atom_thermal_prob_spin = atomglui->add_spinner_to_panel(atom_par_panel,"Probability (%): ",GLUI_SPINNER_INT,&tProb,
		                                                    200,atom_prop_control);
	atom_thermal_prob_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);
	atom_thermal_prob_spin->set_speed(0.1);


	atom_type_radio = atomglui->add_radiogroup_to_panel(atom_main_panel,NULL,203,atom_prop_control);
	atomglui->add_radiobutton_to_group(atom_type_radio,"Show as balls");
	atomglui->add_radiobutton_to_group(atom_type_radio,"Show as thermal ellipsoids");
	atomglui->add_separator_to_panel(atom_main_panel);
    atom_show_teout_check = atomglui->add_checkbox_to_panel(atom_main_panel,"Show outlines of ellipsoids",
		                                                    &aOutline,200,atom_prop_control);
//--------------------------------------------------------------------------
	atom_btn_panel = atomglui->add_panel("",GLUI_PANEL_NONE);
	atom_OK_btn = atomglui->add_button_to_panel(atom_btn_panel,"OK", 0, atom_prop_control);
	atomglui->add_column_to_panel(atom_btn_panel,false);
	atom_Cancel_btn = atomglui->add_button_to_panel(atom_btn_panel,"Cancel", 1, atom_prop_control);

	init_atom_prop_dialog();
}
