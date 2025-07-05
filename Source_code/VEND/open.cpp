//***************************************************************************//
//                              OPEN FILE DIALOG                             //
//---------------------------------------------------------------------------//
// File: open.cpp                                                            //
// Date: 2002.01.31                                                          //
//***************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>

#include "winopen.h"

//--- Objects
GLUI *openfile;                               //--- GLUI window
GLUI_Panel *openfile_panel;                   //--- Main Dialog Panel
GLUI_Panel *btn_panel;                        //--- OK/Cancel Panel
GLUI_EditText *openfile_name_txt;             //--- File Name
GLUI_Listbox *openfile_format_lst;            //--- List of File Formats
GLUI_Button *brouse_btn;                      //--- Brouse Button
GLUI_EditText *ini_level_edit;                //--- Initial Minimum Level
GLUI_Button *ok_btn, *cancel_btn;

//--- Control
int openfile_Dialog_ID;
int file_ID;
char fName[80];
float ini_level;

//********************  get_file_id()  **************************************//
void get_file_info(int &format, char *fname, float &level)
{
	format = file_ID;
	sprintf(fname,"%s",fName);
	level = ini_level_edit->get_float_val();
}


//********************  get_file_dialog_id()  *******************************//
int get_file_dialog_id()
{
	return(openfile_Dialog_ID);
}

//********************  set_file_dialog_id()  *******************************//
void set_file_dialog_id(int ID)
{
	openfile_Dialog_ID = ID;
}


//********************  openfile_control_cb()  ******************************//
void openfile_control_cb( int control )
{
	int i;
	FILE *fptr;


	switch(control)
	{
	case(0):{
		sprintf(fName,"");
		sprintf(fName,"%s",openfile_name_txt->get_text());
		file_ID = openfile_format_lst->get_int_val();
		if (strlen(fName) == 0) openfile_Dialog_ID = 0;
		else {
			fptr = fopen(fName,"r");
			if( fptr == NULL ) openfile_Dialog_ID = 2;
			else { 
				fclose(fptr);
				ok_btn->disable();
				cancel_btn->disable();
				brouse_btn->disable();
				openfile_panel->disable();
				openfile->close();
				openfile_Dialog_ID = 1;
			}
		}
		break;
			}
	case(1):{
		openfile->close();
		openfile_Dialog_ID = 0;
		break;
			}
	case(10):{
		file_ID = openfile_format_lst->get_int_val();
		i = open_file_dialog(file_ID,fName,0);
		if (i > 0) {
			openfile_Dialog_ID = -1;
			fptr = fopen(fName,"r");
			if( fptr == NULL ) openfile_Dialog_ID = 2;
			else {
				fclose(fptr);
				openfile_Dialog_ID = 1;
				openfile->close();
				openfile_Dialog_ID = 1;
				openfile_name_txt->set_text(fName);
				openfile_format_lst->set_int_val(file_ID);
				if (file_ID == 2) ini_level_edit->disable();
				else ini_level_edit->enable();
			}
		}
		else {
			openfile_Dialog_ID = -1;
		}
		break;
			  }
	case(20):{
		openfile_Dialog_ID = -1;
		file_ID = openfile_format_lst->get_int_val();
//		if (file_ID == 2) ini_level_edit->disable();
//		else ini_level_edit->enable();
		break;
			 }
	}
}

//********************  init_filename_dialog()  *****************************//
void init_filename_dialog()
{
//	file_ID = 0;
	sprintf(fName,"");
	ini_level = 1.0f;
	openfile_format_lst->set_int_val(file_ID);
	openfile_name_txt->set_text(fName);
	ini_level_edit->set_float_val(ini_level);
}

//********************  open_filename_dialog()  *****************************//
void open_filename_dialog(int ID, int f_format)
{
	file_ID = f_format;
	openfile_Dialog_ID = -1;
	openfile = GLUI_Master.create_glui("Open",0,180,140 );
	openfile->set_main_gfx_window( ID );
	openfile_panel = openfile->add_panel("",GLUI_PANEL_RAISED);

	openfile_name_txt = openfile->add_edittext_to_panel(openfile_panel,"File name :",GLUI_EDITTEXT_TEXT);
	openfile_name_txt->set_w(300);
//----------------------------------------------------------------------------------------------------
	openfile_format_lst = openfile->add_listbox_to_panel(openfile_panel,"    Format :",NULL,20,openfile_control_cb);
	openfile_format_lst->add_item( 0,"MEM (*.den)");
	openfile_format_lst->add_item( 1,"Gaussian 98 (*.cube)");
	openfile_format_lst->add_item( 2,"SCAT (*.scat)");
	openfile_format_lst->add_item( 3,"WIEN2k (*.rho)");
	openfile_format_lst->add_item( 4,"VEND (*.vnd)");
	openfile_format_lst->add_item( 5,"VEND 3D (*.*ed)");
	openfile_format_lst->add_item( 6,"Energy band (*.eb)");
	openfile_format_lst->set_int_val(0);
//----------------------------------------------------------------------------------------------------
	openfile->add_separator_to_panel(openfile_panel);
	ini_level_edit = openfile->add_edittext_to_panel(openfile_panel,"Initial isosurface level :",GLUI_EDITTEXT_FLOAT);
	ini_level_edit->set_alignment(GLUI_ALIGN_RIGHT);
//----------------------------------------------------------------------------------------------------
	openfile->add_column_to_panel(openfile_panel,false);
//----------------------------------------------------------------------------------------------------
	brouse_btn = openfile->add_button_to_panel(openfile_panel,"Browse...", 10, openfile_control_cb);
	brouse_btn->set_w(50);
//----------------------------------------------------------------------------------------------------
	btn_panel = openfile->add_panel("",GLUI_PANEL_NONE);
	ok_btn = openfile->add_button_to_panel(btn_panel,"OK", 0, openfile_control_cb);
	openfile->add_column_to_panel(btn_panel,false);
	cancel_btn = openfile->add_button_to_panel(btn_panel,"Cancel", 1, openfile_control_cb);
//----------------------------------------------------------------------------------------------------
	init_filename_dialog();
}
