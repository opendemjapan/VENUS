//***************************************************************************//
//                               ABOUT DIALOG                                //
//---------------------------------------------------------------------------//
// File: about.cpp                                                           //
// Date: 2002.05.24                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   about_dialog_ID = -1 -- Open About Dialog                               //
//                      1 -- Close About Dialog ("OK")                       //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>

// Objects
GLUI *about;                                   //--- ABOUT GLUI window
GLUI_Panel *about_panel;                       //--- ABOUT Main Panel
GLUI_Panel *welcome_panel;                     //--- ABOUT Welcome Panel
GLUI_Panel *warranty_panel;                    //--- ABOUT Welcome Panel

GLUI_StaticText *line1;
GLUI_StaticText *line2;
GLUI_StaticText *line3;
GLUI_StaticText *line4;
GLUI_StaticText *line5;

// Control
int about_Dialog_ID;

/**************************************** get_about_dialog_pos() ************/
void get_about_dialog_pos(int &x, int &y)
{
	glutSetWindow(about->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}



/**************************************** get_about_dialog_id() *************/
int get_about_dialog_id()
{
	return(about_Dialog_ID);
}

/**************************************** about_control() *******************/
void about_control( int control)
{
	switch(control)
	{
	case(0):{about_Dialog_ID = 1; about->close(); break;}
	}
}

/**************************************** open_about_dialog() ***************/
void open_about_dialog(int ID, int dpX, int dpY)
{
	about_Dialog_ID = -1;

	about = GLUI_Master.create_glui("About VEND",0,dpX,dpY );
	about->set_main_gfx_window( ID );

	welcome_panel = about->add_panel("",GLUI_PANEL_RAISED);
	warranty_panel = about->add_panel("",GLUI_PANEL_EMBOSSED);
	about_panel = about->add_panel("",GLUI_PANEL_NONE);

	welcome_panel->set_alignment(GLUI_ALIGN_CENTER);
	warranty_panel->set_alignment(GLUI_ALIGN_CENTER);
	about_panel->set_alignment(GLUI_ALIGN_CENTER);

	about->add_button_to_panel(about_panel,"OK", 0, about_control);
//-------------------------------------------------------------------------------------------//
	about->add_separator_to_panel(welcome_panel);
	line1 = about->add_statictext_to_panel(welcome_panel,"VEND");
	line2 = about->add_statictext_to_panel(welcome_panel,"Visualization of Electron and Nuclear Density");
	line3 = about->add_statictext_to_panel(welcome_panel,"Ver. 3.0");
	about->add_separator_to_panel(welcome_panel);
	line4 = about->add_statictext_to_panel(welcome_panel,"Copyright (C) 2003");
	line5 = about->add_statictext_to_panel(welcome_panel,"Ruben A. Dilanian and Fujio Izumi");
	line1->set_alignment(GLUI_ALIGN_CENTER);line1->set_w(1);
	line2->set_alignment(GLUI_ALIGN_CENTER);line2->set_w(1);
	line3->set_alignment(GLUI_ALIGN_CENTER);line3->set_w(1);
	line4->set_alignment(GLUI_ALIGN_CENTER);line4->set_w(1);
	line5->set_alignment(GLUI_ALIGN_CENTER);line5->set_w(1);
//-------------------------------------------------------------------------------------------//
	about->add_statictext_to_panel(warranty_panel,"THERE IS NO WARRANTY FOR THIS PROGRAM.");
	about->add_statictext_to_panel(warranty_panel,"THE ENTIRE RISK AS TO ITS QUALITY AND");
	about->add_statictext_to_panel(warranty_panel,"PERFORMANCE IS WITH YOU.");
	about->add_statictext_to_panel(warranty_panel,"");
	about->add_statictext_to_panel(warranty_panel,"WE REQUEST YOU TO CHECK THIS PROGRAM");
	about->add_statictext_to_panel(warranty_panel,"FOR BUGS AND REPORT THEM, WHICH HELPS");
	about->add_statictext_to_panel(warranty_panel,"US DEVELOP SCIENTIFIC FREE SOFTWARE.");
//-------------------------------------------------------------------------------------------//
//	glutMainLoop();
}


