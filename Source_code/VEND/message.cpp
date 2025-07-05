//***************************************************************************//
//                      MESSAGE DIALOGS                                      //
//---------------------------------------------------------------------------//
// File: message.cpp                                                         //
// Date: 2002.02.19                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   mes_export_dialog_ID = -1 -- Open Export Message Dialog                 //
//                           1 -- Close Export Message Dialog ("OK")         //
//                                                                           //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>


//===========================================================//
//                 Message Dialog No 1                       //
//===========================================================//
// Objects
GLUI *messglui1;
GLUI_Panel *mess_main_panel1;
GLUI_StaticText *message01;
GLUI_StaticText *message11;
GLUI_StaticText *message21;
GLUI_StaticText *message31;

// Control
int message_export_dialog1_ID;



//===========================================================//
//                 Message Dialog No 1                       //
//===========================================================//

//********************  get_message_dialog_id1()  ***************************//
int get_message_dialog_id1()
{
	return(message_export_dialog1_ID);
}

/**************************************** mess_export_control1() *************/
void mess_export_control1( int control)
{
	switch(control)
	{
	case(0):{
		message_export_dialog1_ID = 1;
		messglui1->close();
		break;
			}
	}
}

/**************************************** open_message_dialog1() ************/
void init_message_dialog1(int mode,char *msg)
{
	char line[256];
    
	switch(mode){
	case(0):{
		sprintf(line,"Error during reading data from the file:");
		message11->set_name(line);
		sprintf(line,msg);
		message21->set_name(line);
		sprintf(line,"Wrong file name. File not found.");
		message31->set_name(line);
		break;
			}
	case(1):{
		sprintf(line,"Error during reading data from the file:");
		message11->set_name(line);
		sprintf(line,"Dimensions of cuboids in *.vcs and *.scat");
		message21->set_name(line);
		sprintf(line,"files do not agree with each other.");
		message31->set_name(line);
		break;
			}
	}
}

/**************************************** open_message_dialog1() ************/
void open_message_dialog1(int ID, int mode,char *msg)
{
	message_export_dialog1_ID = -1;

	messglui1 = GLUI_Master.create_glui("VEND message",0,180,140 );
	messglui1->set_main_gfx_window( ID );

	mess_main_panel1 = messglui1->add_panel("",GLUI_PANEL_RAISED);
	message01 = messglui1->add_statictext_to_panel(mess_main_panel1,"WARNING!");
	messglui1->add_separator_to_panel(mess_main_panel1);
	message11 = messglui1->add_statictext_to_panel(mess_main_panel1,"");
	message21 = messglui1->add_statictext_to_panel(mess_main_panel1,"");
	message31 = messglui1->add_statictext_to_panel(mess_main_panel1,"");
	messglui1->add_separator_to_panel(mess_main_panel1);

	message01->set_alignment(GLUI_ALIGN_CENTER);message01->set_w(1);
	message11->set_alignment(GLUI_ALIGN_CENTER);message11->set_w(1);
	message21->set_alignment(GLUI_ALIGN_CENTER);message21->set_w(1);
	message31->set_alignment(GLUI_ALIGN_CENTER);message31->set_w(1);

	messglui1->add_button("OK", 0, mess_export_control1);

	init_message_dialog1(mode,msg);

//	glutMainLoop();
}







