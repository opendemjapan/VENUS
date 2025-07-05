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
GLUI_RadioGroup *mess_type_radio1;

// Control
int message_export_dialog_ID;


//===========================================================//
//                 Message Dialog No 2                       //
//===========================================================//
// Objects
GLUI *messglui2;
GLUI_Panel *mess_main_panel2;
GLUI_StaticText *message0;
GLUI_StaticText *message1;
GLUI_StaticText *message2;
GLUI_StaticText *message3;

// Control
int message_export_dialog2_ID;

//===========================================================//
//                 Message Dialog No 3                       //
//===========================================================//
// Objects
GLUI *messglui3;
GLUI_Panel *mess_main_panel3;
GLUI_StaticText *message03;
GLUI_StaticText *message13;
GLUI_StaticText *message23;
GLUI_StaticText *message33;
GLUI_StaticText *message43;

// Control
int message_export_dialog3_ID;


//===========================================================//
//                 Message Dialog No 1                       //
//===========================================================//

//********************  get_message_dialog_id1()  ***************************//
int get_message_dialog_id1()
{
	return(message_export_dialog_ID);
}

/**************************************** mess_export_control() *************/
void mess_export_control( int control)
{
	switch(control)
	{
	case(0):{
		message_export_dialog_ID = mess_type_radio1->get_int_val() + 1;
		messglui1->close();
		break;
			}
	}
}

/**************************************** open_message_dialog1() ************/
void open_message_dialog1(int ID)
{
	message_export_dialog_ID = -1;

	messglui1 = GLUI_Master.create_glui("File export",0,180,140 );
	messglui1->set_main_gfx_window( ID );

	mess_main_panel1 = messglui1->add_panel("",GLUI_PANEL_RAISED);

	mess_type_radio1 = messglui1->add_radiogroup_to_panel(mess_main_panel1,NULL);
	messglui1->add_radiobutton_to_group(mess_type_radio1,"Export with isotropic atomic displacement parameters");
	messglui1->add_radiobutton_to_group(mess_type_radio1,"Export with anisotropic atomic displacement parameters");
	mess_type_radio1->set_int_val(0);

	messglui1->add_button("OK", 0, mess_export_control);

//	glutMainLoop();
}


//===========================================================//
//                 Message Dialog No 2                       //
//===========================================================//

//********************  get_message_dialog_id1()  ***************************//
int get_message_dialog_id2()
{
	return(message_export_dialog2_ID);
}

/**************************************** mess_export_control2() *************/
void mess_export_control2( int control)
{
	switch(control)
	{
	case(0):{
		message_export_dialog2_ID = 1;
		messglui2->close();
		break;
			}
	}
}

/**************************************** open_message_dialog2() ************/
void init_message2_dialog(int mode)
{
	char line[256];

	switch(mode)
	{
	case(1):{
		sprintf(line,"Error during reading data from the file.");
		message1->set_name(line);
		sprintf(line,"See an error message in the Output Window");
		message2->set_name(line);
		sprintf(line,"and click [OK].");
		message3->set_name(line);
		break;
			}
	case(2):{
		sprintf(line,"Error during writing data to the file.");
		message1->set_name(line);
		sprintf(line,"See an error message in the Output Window");
		message2->set_name(line);
		sprintf(line,"and click [OK].");
		message3->set_name(line);
		break;
			}
	}

}

/**************************************** open_message_dialog2() ************/
void open_message_dialog2(int ID, int mode)
{
	message_export_dialog2_ID = -1;

	messglui2 = GLUI_Master.create_glui("VICS message",0,180,140 );
	messglui2->set_main_gfx_window( ID );

	mess_main_panel2 = messglui2->add_panel("",GLUI_PANEL_RAISED);
	message0 = messglui2->add_statictext_to_panel(mess_main_panel2,"WARNING!");
	messglui2->add_separator_to_panel(mess_main_panel2);
	message1 = messglui2->add_statictext_to_panel(mess_main_panel2,"");
	message2 = messglui2->add_statictext_to_panel(mess_main_panel2,"");
	message3 = messglui2->add_statictext_to_panel(mess_main_panel2,"");
	messglui2->add_separator_to_panel(mess_main_panel2);

	message0->set_alignment(GLUI_ALIGN_CENTER);message0->set_w(1);
	message1->set_alignment(GLUI_ALIGN_CENTER);message1->set_w(1);
	message2->set_alignment(GLUI_ALIGN_CENTER);message2->set_w(1);
	message3->set_alignment(GLUI_ALIGN_CENTER);message3->set_w(1);

	messglui2->add_button("OK", 0, mess_export_control2);

	init_message2_dialog(mode);
}





//===========================================================//
//                 Message Dialog No 3                       //
//===========================================================//

//********************  get_message_dialog_id3()  ***************************//
int get_message_dialog_id3()
{
	return(message_export_dialog3_ID);
}

/**************************************** mess_export_control3() *************/
void mess_export_control3( int control)
{
	switch(control)
	{
	case(0):{
		message_export_dialog3_ID = 1;
		messglui3->close();
		break;
			}
	}
}

/**************************************** open_message_dialog3() ************/
void init_message3_dialog(int mode,char *msg)
{
	char line[256];
    
	switch(mode){
	case(0):{
		sprintf(line,"Wrong space-group name!");
		message13->set_name(line);
		sprintf(line,"Instead of the space-group name, a space-group number");
		message23->set_name(line);
		sprintf(line,"has been input. Thouth the setting number is assumed to be 1,");
		message33->set_name(line);
		sprintf(line,"you can change it in the Edit dialog box.");
		message43->set_name(line);
		break;
			}
	case(1):{
		sprintf(line,"Error during reading data from the file:");
		message13->set_name(line);
		sprintf(line,msg);
		message23->set_name(line);
		sprintf(line,"Wrong file name. File not found.");
		message33->set_name(line);
		break;
			}
	case(2):{
		sprintf(line,"Part of atoms outside the boundary box");
		message23->set_name(line);
		sprintf(line,"specified in file c04d.");
		message33->set_name(line);
		break;
			}
	}
}

/**************************************** open_message_dialog3() ************/
void open_message_dialog3(int ID, int mode,char *msg)
{
	message_export_dialog3_ID = -1;

	messglui3 = GLUI_Master.create_glui("VICS message",0,180,140 );
	messglui3->set_main_gfx_window( ID );

	mess_main_panel3 = messglui3->add_panel("",GLUI_PANEL_RAISED);
	message03 = messglui3->add_statictext_to_panel(mess_main_panel3,"WARNING!");
	messglui3->add_separator_to_panel(mess_main_panel3);
	message13 = messglui3->add_statictext_to_panel(mess_main_panel3,"");
	message23 = messglui3->add_statictext_to_panel(mess_main_panel3,"");
	message33 = messglui3->add_statictext_to_panel(mess_main_panel3,"");
	message43 = messglui3->add_statictext_to_panel(mess_main_panel3,"");
	messglui3->add_separator_to_panel(mess_main_panel3);

	message03->set_alignment(GLUI_ALIGN_CENTER);message03->set_w(1);
	message13->set_alignment(GLUI_ALIGN_CENTER);message13->set_w(1);
	message23->set_alignment(GLUI_ALIGN_CENTER);message23->set_w(1);
	message33->set_alignment(GLUI_ALIGN_CENTER);message33->set_w(1);
	message43->set_alignment(GLUI_ALIGN_CENTER);message43->set_w(1);

	messglui3->add_button("OK", 0, mess_export_control3);

	init_message3_dialog(mode,msg);
}

