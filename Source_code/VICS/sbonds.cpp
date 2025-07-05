//***************************************************************************//
//                      SEARCH BONDS DIALOG                                  //
//---------------------------------------------------------------------------//
// File: sbonds.cpp                                                          //
// Date: 2002.01.11                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   sbonds_Dialog_ID = -1 -- Open Search Bonds Dialog                       //
//                             1 -- Close Search Bonds Dialog ("OK")         //
//                             0 -- Close Search Bonds Dialog ("Cancel")     //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>
#include "vics.h"

typedef struct sbondstag
{
	int search_mode;
	char A1[3];               //--- Atomic Symbol for Atom 1
	float d;                  //--- Max. Bonds distance
	char A2[3];               //--- Atomic Symbol for Atom 2
	int show_poly;
	int boundary_mode;
} SBONDS;

typedef struct sbatag
{
	char simb[3];
} SBONDSATOM;


GLUI *bonds;
GLUI_Panel *bonds_panel;
GLUI_Panel *bonds_spec_panel;
GLUI_Panel *bonds_show_panel;
GLUI_Panel *bonds_button_panel;
GLUI_Listbox *Atom1List;
GLUI_Listbox *Atom2List;
GLUI_EditText *bond_d_txt;
GLUI_EditText *Atom_txt;
GLUI_EditText *Atom1_txt;
GLUI_EditText *Atom2_txt;
GLUI_EditText *Atom_d_txt;
GLUI_Button *b_add_btn;
GLUI_Button *b_next_btn;
GLUI_Button *b_prev_btn;
GLUI_Button *b_edit_btn;
GLUI_Button *b_del_btn;
GLUI_StaticText *bonds_num_info;

GLUI_Panel *search_mode_panel;
GLUI_RadioGroup *search_bonds_mode;         //--- Search method

GLUI_Listbox *PolyShowList;
GLUI_Listbox *BoundyShowList;

GLUI_Checkbox *bound_search_check;
GLUI_Checkbox *hbound_check;


// Control
int bonds_Dialog_ID;                    //--- SBONDS dialog control ID
int cbondspec;
int totalbondspec = 0;
int totalAtomSpec = 0;
SBONDS sbini[MAX_BONDS_SPEC];
SBONDSATOM mAtom[MAX_ATOMS_IN_ASYMM];

int search_mode = 0;
float Mol_bond;
int hbond_ID = 0;

/**************************************** get_sbonds_dialog_pos() ************/
void get_sbonds_dialog_pos(int &x, int &y)
{
	glutSetWindow(bonds->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}

//********************  get_bond_dialog_id()  *******************************//
int get_bond_dialog_id()
{
	return(bonds_Dialog_ID);
}

//********************  get_new_number_bond_spec()  *************************//
int get_new_number_bond_spec()
{
	return(totalbondspec);
}

//********************  get_h_bond_id()  ************************************//
int get_h_bond_id()
{
	hbond_ID = hbound_check->get_int_val();
	return (hbond_ID);
}

//********************  get_new_bond_spec()  ********************************//
void get_new_bond_spec(int ID, char Symb1[2], char Symb2[2], float &d, int &smode, int &bmode, int &pmode)
{
	sprintf(Symb1,"");
	sprintf(Symb1,"%s",sbini[ID].A1);
	sprintf(Symb2,"");
	sprintf(Symb2,"%s",sbini[ID].A2);
	d = sbini[ID].d;
	smode = sbini[ID].search_mode;
	bmode = sbini[ID].boundary_mode;
	pmode = sbini[ID].show_poly;
}


//********************  bonds_control_cb()  *********************************//
void bonds_control_cb( int control )
{
	char pos[40];
	int i;

	switch(control)
	{
	case(0):{
		bonds_Dialog_ID = 1;
		bonds->close();
		break;
			}
	case(1):{
		bonds_Dialog_ID = 0;
		bonds->close();
		break;
			}
//--- "Add" Button
	case(10):{
		if ((totalbondspec+1) <= MAX_BONDS_SPEC){
			cbondspec = totalbondspec;
			sprintf(sbini[cbondspec].A1,"");
			sprintf(sbini[cbondspec].A2,"");
			switch(search_mode){
			case(0):{
				sprintf(sbini[cbondspec].A1,"%s",&Atom1List->curr_text);
				sprintf(sbini[cbondspec].A2,"%s",&Atom2List->curr_text);
				break;
					}
			case(1):{
				sprintf(sbini[cbondspec].A1,"%s",&Atom1List->curr_text);
				sprintf(sbini[cbondspec].A2,"%s","XX");
				break;
					}
			case(2):{
				sprintf(sbini[cbondspec].A1,"%s","XX");
				sprintf(sbini[cbondspec].A2,"%s","XX");
				break;
					}
			}
			sbini[cbondspec].search_mode = search_mode;
			sbini[cbondspec].d = bond_d_txt->get_float_val();
			sbini[cbondspec].boundary_mode = bound_search_check->get_int_val();
			if (search_mode == 2) sbini[cbondspec].show_poly = 0;
			else sbini[cbondspec].show_poly = 1;
//--------------------------------------------------------------
			sprintf(pos,"");
			sprintf(pos,"%2s",sbini[cbondspec].A1);
			Atom1_txt->set_text(pos);
			sprintf(pos,"");
			sprintf(pos,"%2s",sbini[cbondspec].A2);
			Atom2_txt->set_text(pos);
//--------------------------------------------------------------
			Atom_d_txt->set_float_val(sbini[cbondspec].d);
			BoundyShowList->set_int_val(sbini[cbondspec].boundary_mode);
			PolyShowList->set_int_val(sbini[cbondspec].show_poly);
			totalbondspec++;
			sprintf(pos,"");
			sprintf(pos,"Number of bond specifications = %i",totalbondspec);
			bonds_num_info->set_name(pos);
			if (sbini[cbondspec].search_mode == 2) PolyShowList->disable();
			else PolyShowList->enable();
			if (sbini[cbondspec].search_mode != 0) BoundyShowList->disable();
			else BoundyShowList->enable();
		}
		break;
			 }
//--- "Next (>)" Button
	case(11):{
		cbondspec++;
//--------------------------------------------------------------
		sprintf(pos,"");
		sprintf(pos,"%2s",sbini[cbondspec].A1);
		Atom1_txt->set_text(pos);
		sprintf(pos,"");
		sprintf(pos,"%2s",sbini[cbondspec].A2);
		Atom2_txt->set_text(pos);
//--------------------------------------------------------------
		Atom_d_txt->set_float_val(sbini[cbondspec].d);
		BoundyShowList->set_int_val(sbini[cbondspec].boundary_mode);
		PolyShowList->set_int_val(sbini[cbondspec].show_poly);
		if (sbini[cbondspec].search_mode == 2) PolyShowList->disable();
		else PolyShowList->enable();
		if (sbini[cbondspec].search_mode != 0) BoundyShowList->disable();
		else BoundyShowList->enable();
		break;
			 }
//--- "Prev (<)" Button
	case(12):{
		cbondspec--;
//--------------------------------------------------------------
		sprintf(pos,"");
		sprintf(pos,"%2s",sbini[cbondspec].A1);
		Atom1_txt->set_text(pos);
		sprintf(pos,"");
		sprintf(pos,"%2s",sbini[cbondspec].A2);
		Atom2_txt->set_text(pos);
//--------------------------------------------------------------
		Atom_d_txt->set_float_val(sbini[cbondspec].d);
		BoundyShowList->set_int_val(sbini[cbondspec].boundary_mode);
		PolyShowList->set_int_val(sbini[cbondspec].show_poly);
		if (sbini[cbondspec].search_mode == 2) PolyShowList->disable();
		else PolyShowList->enable();
		if (sbini[cbondspec].search_mode != 0) BoundyShowList->disable();
		else BoundyShowList->enable();
		break;
			 }
//--- "Edit" Button
	case(13):{
		sbini[cbondspec].d = Atom_d_txt->get_float_val();
		sbini[cbondspec].show_poly = PolyShowList->get_int_val();
		sbini[cbondspec].boundary_mode = BoundyShowList->get_int_val();
		sbini[cbondspec].show_poly = PolyShowList->get_int_val();
		break;
			 }
//--- "Delete" Button
	case(14):{
		if(cbondspec < (totalbondspec-1)){
			for (i = cbondspec; i<(totalbondspec-1); i++){
				sprintf(sbini[i].A1,"");
				sprintf(sbini[i].A2,"");
				sprintf(sbini[i].A1,"%s",sbini[i+1].A1);
				sprintf(sbini[i].A2,"%s",sbini[i+1].A2);
				sbini[i].d = sbini[i+1].d;
				sbini[i].search_mode = sbini[i+1].search_mode;
				sbini[i].boundary_mode = sbini[i+1].boundary_mode;
				sbini[i].show_poly = sbini[i+1].show_poly;
			}
//--------------------------------------------------------------
			sprintf(pos,"");
			sprintf(pos,"%2s",sbini[cbondspec].A1);
			Atom1_txt->set_text(pos);
			sprintf(pos,"");
			sprintf(pos,"%2s",sbini[cbondspec].A2);
			Atom2_txt->set_text(pos);
//--------------------------------------------------------------
			Atom_d_txt->set_float_val(sbini[cbondspec].d);
			BoundyShowList->set_int_val(sbini[cbondspec].boundary_mode);
			PolyShowList->set_int_val(sbini[cbondspec].show_poly);
			if (sbini[cbondspec].search_mode == 2) PolyShowList->disable();
			else PolyShowList->enable();
			if (sbini[cbondspec].search_mode != 0) BoundyShowList->disable();
			else BoundyShowList->enable();
			totalbondspec--;
		}
		else {
			cbondspec--;
//--------------------------------------------------------------
			sprintf(pos,"");
			sprintf(pos,"%2s",sbini[cbondspec].A1);
			Atom1_txt->set_text(pos);
			sprintf(pos,"");
			sprintf(pos,"%2s",sbini[cbondspec].A2);
			Atom2_txt->set_text(pos);
//--------------------------------------------------------------
			Atom_d_txt->set_float_val(sbini[cbondspec].d);
			BoundyShowList->set_int_val(sbini[cbondspec].boundary_mode);
			PolyShowList->set_int_val(sbini[cbondspec].show_poly);
			if (sbini[cbondspec].search_mode == 2) PolyShowList->disable();
			else PolyShowList->enable();
			if (sbini[cbondspec].search_mode != 0) BoundyShowList->disable();
			else BoundyShowList->enable();
			totalbondspec--;
		}
		if (totalbondspec == 0){
//--------------------------------------------------------------
			sprintf(pos,"");
			Atom1_txt->set_text(pos);
			Atom2_txt->set_text(pos);
//--------------------------------------------------------------
			Atom_d_txt->set_float_val(0);
			BoundyShowList->set_int_val(0);
			BoundyShowList->enable();
			PolyShowList->set_int_val(0);
			PolyShowList->enable();
		}
		sprintf(pos,"");
		sprintf(pos,"Number of bond specifications = %i",totalbondspec);
		bonds_num_info->set_name(pos);
		break;
			 }
	case(20):{
		switch (search_mode)
		{
		case(0):{
			Atom1List->enable();
			Atom2List->enable();
			bound_search_check->set_int_val(0);
			bound_search_check->enable();
			break;
				}
		case(1):{
			Atom1List->enable();
			Atom2List->disable();
			bound_search_check->set_int_val(1);
			bound_search_check->disable();
			break;
				}
		case(2):{
			Atom1List->disable();
			Atom2List->disable();
			bound_search_check->set_int_val(1);
			bound_search_check->disable();
			break;
				}
		}
		break;
			 }
	}
	if ((cbondspec == (totalbondspec-1)) || (totalbondspec == 0)) b_next_btn->disable();
	else b_next_btn->enable();
	if ((cbondspec == 0) || (totalbondspec == 0)) b_prev_btn->disable();
	else b_prev_btn->enable();
	if (totalbondspec > 0) {
		b_edit_btn->enable();
		b_del_btn->enable();
	}
	else {
		b_edit_btn->disable();
		b_del_btn->disable();
	}
}


//********************  set_bonds_to_bonds_dialog()  ************************//
void set_bonds_to_bonds_dialog(int ID, char Symb1[2],char Symb2[2], float d, int search_mode_ID,
							   int bound_mode_ID, int poly_mode_ID)
{
	sprintf(sbini[ID].A1,"");
	sprintf(sbini[ID].A1,"%2s",Symb1);
	sprintf(sbini[ID].A2,"");
	sprintf(sbini[ID].A2,"%2s",Symb2);
	sbini[ID].d = d;
	sbini[ID].search_mode = search_mode_ID;
	sbini[ID].boundary_mode = bound_mode_ID;
	sbini[ID].show_poly = poly_mode_ID;
}

//********************  set_atoms_to_bonds_dialog()  ************************//
void set_atoms_to_bonds_dialog(int ID, char Symb[2])
{
	sprintf(mAtom[ID].simb,""); 
	sprintf(mAtom[ID].simb,"%2s",Symb); 
}


//********************  init_bonds_dialog()  ********************************//
void init_bonds_dialog(float mol, int numAtom,int numBond,int hb_id)
{
	totalAtomSpec = numAtom;
	totalbondspec = numBond;
	cbondspec = 0;
	search_mode = 0;
	Mol_bond = mol;
	hbond_ID = hb_id;
}

//********************  set_atoms_list()  ***********************************//
void set_atoms_list()
{
	int i,j;
	int Num = 0;
	int control;
	int h_control = 0;
	char pos[40];
	char *pos1;

	h_control = 0;
	for(i=0; i<totalAtomSpec; i++) {
		pos1 = mAtom[i].simb;
		if (pos1[0] == ' ') pos1++;
		if ((strstr(pos1, "H") != NULL) && (strlen(pos1) == 1)) h_control = 1;
		if (i == 0){
			Atom1List->add_item(0,mAtom[i].simb);
			Atom2List->add_item(0,mAtom[i].simb);
			Num++;
		}
		else {
			control = 0;
			for(j=0; j<i; j++){
				if (strstr(mAtom[i].simb, mAtom[j].simb) != NULL){
					control = 1;
					break;
				}
			}
			if (control == 0) {
				Atom1List->add_item(Num,mAtom[i].simb);
				Atom2List->add_item(Num,mAtom[i].simb);
				Num++;
			}
		}
	}
	if (h_control == 1) {
		hbound_check->set_int_val(hbond_ID);
		hbound_check->enable();
	}
	else {
		hbond_ID = 0;
		hbound_check->set_int_val(hbond_ID);
		hbound_check->disable();
	}
	Atom1List->set_int_val(0);
	Atom2List->set_int_val(0);
	sprintf(pos,"");
	sprintf(pos,"Number of bond specifications = %i",totalbondspec);
	bonds_num_info->set_name(pos);
	if (totalbondspec > 0){
		b_prev_btn->disable();
		b_edit_btn->enable();
		b_del_btn->enable();
		if (totalbondspec > 1) b_next_btn->enable();
//--------------------------------------------------------------
		sprintf(pos,"");
		sprintf(pos,"%2s",sbini[0].A1);
		Atom1_txt->set_text(pos);
		sprintf(pos,"");
		sprintf(pos,"%2s",sbini[0].A2);
		Atom2_txt->set_text(pos);
//--------------------------------------------------------------
		Atom_d_txt->set_float_val(sbini[0].d);
		BoundyShowList->set_int_val(sbini[0].boundary_mode);
		PolyShowList->set_int_val(sbini[0].show_poly);
	}
	bond_d_txt->set_float_val(Mol_bond);
	if (sbini[0].search_mode == 2) PolyShowList->disable();
	else PolyShowList->enable();
	if (sbini[0].search_mode != 0) BoundyShowList->disable();
	else BoundyShowList->enable();
}

//********************  open_bonds_dialog()  ********************************//
void open_bonds_dialog(int ID, int dpX, int dpY)
{
	bonds_Dialog_ID = -1;
	bonds = GLUI_Master.create_glui("Search Bonds",0,dpX,dpY );
	bonds->set_main_gfx_window( ID );

	bonds_num_info = bonds->add_statictext("");
	bonds_panel = bonds->add_panel("",GLUI_PANEL_RAISED);

	bonds_spec_panel = bonds->add_panel_to_panel(bonds_panel,"Add new bonds",GLUI_PANEL_EMBOSSED);

	search_mode_panel = bonds->add_panel_to_panel(bonds_spec_panel,"Search mode",GLUI_PANEL_EMBOSSED);
	search_bonds_mode = bonds->add_radiogroup_to_panel(search_mode_panel, &search_mode,20,bonds_control_cb);
	bonds->add_radiobutton_to_group(search_bonds_mode,"Search A2 bonded to A1 ");
	bonds->add_radiobutton_to_group(search_bonds_mode,"Search atoms bonded to A1");
	bonds->add_radiobutton_to_group(search_bonds_mode,"Search molecules");
	search_mode_panel->set_alignment(GLUI_ALIGN_LEFT);

	Atom1List = bonds->add_listbox_to_panel(bonds_spec_panel,"A1: ");
	Atom1List->set_alignment(GLUI_ALIGN_RIGHT);
	Atom2List = bonds->add_listbox_to_panel(bonds_spec_panel,"A2: ");
	Atom2List->set_alignment(GLUI_ALIGN_RIGHT);
	bond_d_txt = bonds->add_edittext_to_panel(bonds_spec_panel,"Max. distance: ",GLUI_EDITTEXT_FLOAT);
	bond_d_txt->set_float_val(0); 
	bond_d_txt->set_w(162);
	bond_d_txt->set_alignment(GLUI_ALIGN_RIGHT);

	bonds->add_separator_to_panel(bonds_spec_panel);
    bound_search_check = bonds->add_checkbox_to_panel(bonds_spec_panel,"Search beyond the boundary");
	bound_search_check->set_int_val(0);
	bound_search_check->set_alignment(GLUI_ALIGN_LEFT);


	bonds->add_separator_to_panel(bonds_spec_panel);
	b_add_btn = bonds->add_button_to_panel(bonds_spec_panel,"Add",10,bonds_control_cb);

	bonds->add_column_to_panel(bonds_panel,false);
//----------------------------------------------------------------------------------------------------
	bonds_show_panel = bonds->add_panel_to_panel(bonds_panel,"Search list",GLUI_PANEL_EMBOSSED);

	b_next_btn = bonds->add_button_to_panel(bonds_show_panel,">",11,bonds_control_cb);
	b_next_btn->set_w(55);
	b_prev_btn = bonds->add_button_to_panel(bonds_show_panel,"<",12,bonds_control_cb);
	b_prev_btn->set_w(55);
	b_edit_btn = bonds->add_button_to_panel(bonds_show_panel,"Update",13,bonds_control_cb);
	b_edit_btn->set_w(55);
	b_del_btn = bonds->add_button_to_panel(bonds_show_panel,"Delete",14,bonds_control_cb);
	b_del_btn->set_w(55);
	b_next_btn->disable();
	b_prev_btn->disable();
	b_edit_btn->disable();
	b_del_btn->disable();

	bonds->add_column_to_panel(bonds_show_panel,true);

	Atom1_txt = bonds->add_edittext_to_panel(bonds_show_panel,"A1: ",GLUI_EDITTEXT_TEXT);
	Atom1_txt->disable();
	Atom1_txt->set_w(125);
	Atom1_txt->set_alignment(GLUI_ALIGN_RIGHT);

	Atom2_txt = bonds->add_edittext_to_panel(bonds_show_panel,"A2: ",GLUI_EDITTEXT_TEXT);
	Atom2_txt->disable();
	Atom2_txt->set_w(125);
	Atom2_txt->set_alignment(GLUI_ALIGN_RIGHT);

	Atom_d_txt = bonds->add_edittext_to_panel(bonds_show_panel," Max. distance: ",GLUI_EDITTEXT_FLOAT);
	Atom_d_txt->set_w(160);
	Atom_d_txt->set_alignment(GLUI_ALIGN_RIGHT);

	PolyShowList = bonds->add_listbox_to_panel(bonds_show_panel," Show polyhedra: ");
	PolyShowList->add_item(0,"No");
	PolyShowList->add_item(1,"Yes");
	PolyShowList->set_alignment(GLUI_ALIGN_RIGHT);

	BoundyShowList = bonds->add_listbox_to_panel(bonds_show_panel," Beyond the boundary: ");
	BoundyShowList->add_item(0,"No");
	BoundyShowList->add_item(1,"Yes");
	BoundyShowList->set_alignment(GLUI_ALIGN_RIGHT);
//----------------------------------------------------------------------------------------------------
    hbound_check = bonds->add_checkbox_to_panel(bonds_panel,"Show hydrogen bonds");
	hbound_check->set_int_val(hbond_ID);
	hbound_check->disable();
	bonds->add_separator_to_panel(bonds_panel);
//----------------------------------------------------------------------------------------------------


	bonds->add_button_to_panel(bonds_panel,"OK", 0, bonds_control_cb);
	bonds->add_button_to_panel(bonds_panel,"Cancel", 1, bonds_control_cb);

	switch (search_mode)
	{
	case(0):{
		Atom1List->enable();
		Atom2List->enable();
		break;
			}
	case(1):{
		Atom1List->enable();
		Atom2List->disable();
		break;
			}
	case(2):{
		Atom1List->disable();
		Atom2List->disable();
		break;
			}
	}

	set_atoms_list();
}

