//***************************************************************************//
//                        INFORMATION DIALOG                                 //
//---------------------------------------------------------------------------//
// File: info.cpp                                                            //
// Date: 2002.02.25                                                          //
//***************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *infoglui;                                  //--- GLUI window
GLUI_Panel *info_main_panel;                     //--- Main Dialog Panel

//--- Control
int info_dialog_ID;

/**************************************** get_about_dialog_pos() ************/
void get_info_dialog_pos(int &x, int &y)
{
	glutSetWindow(infoglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  get_info_dialog_id()  *******************************//
int get_info_dialog_id()
{
	return(info_dialog_ID);
}

//********************  info_control_cb()  **********************************//
void info_control_cb( int control )
{
	switch(control)
	{
	case(0):{
		infoglui->close();
		info_dialog_ID = 1;
		break;
			}
	}
}

//********************  open_info_dialog()  *********************************//
void open_info_dialog(int ID,char fname[256], char title[80], int Grid[3], float minDen, float maxDen,
					  int dpX, int dpY)
{
	char cmd[256];

	info_dialog_ID = -1;
	infoglui = GLUI_Master.create_glui("Information",0,dpX,dpY);
	infoglui->set_main_gfx_window( ID );
	info_main_panel = infoglui->add_panel("",GLUI_PANEL_RAISED);

	sprintf(cmd,"File name: %s",fname);
	infoglui->add_statictext_to_panel(info_main_panel,cmd);

	infoglui->add_separator_to_panel(info_main_panel);

	sprintf(cmd,"Title: %s",title);
	infoglui->add_statictext_to_panel(info_main_panel,cmd);

	sprintf(cmd,"Grid: %i x %i x %i",Grid[0],Grid[1],Grid[2]);
	infoglui->add_statictext_to_panel(info_main_panel,cmd);

	sprintf(cmd,"F(min): %f",minDen);
	infoglui->add_statictext_to_panel(info_main_panel,cmd);

	sprintf(cmd,"F(max): %f",maxDen);
	infoglui->add_statictext_to_panel(info_main_panel,cmd);

	infoglui->add_button("OK", 0, info_control_cb);
}
