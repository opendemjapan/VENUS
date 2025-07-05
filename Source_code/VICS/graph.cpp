#include <GL/glui.h>
#include <GL/glut.h>


GLUI *expglui;
GLUI_Panel *exp_panel;                                 //--- Main Panel
GLUI_EditText *exp_file_name;
GLUI_EditText *exp_file_format;
GLUI_Spinner  *exp_smoothing;
GLUI_Spinner  *exp_scale;


GLUI *msgglui;
GLUI_Panel *msg_panel;                                 //--- Main Panel
GLUI_StaticText *mline1;
GLUI_StaticText *mline2;
GLUI_StaticText *mline3;
int msg_Image_ID;

//GLUI_StaticText *exp_progress;

//--- Control
int graph_dialog_ID;
int main_window_ID;


int e_scale;
int e_smoothing;

//char e_name[sizeof(GLUI_String)];
//char e_format_name[sizeof(GLUI_String)];
char e_name[256];
char e_format_name[5];


/**************************************** _update_file_title() ************/
void _update_file_title(char *name)
{
	char *pdest;

	pdest = strrchr(name, '\\');
        if (pdest == NULL){
           pdest = strrchr(name, '/');
           if (pdest == NULL) pdest = name;
	}
	pdest++;
       	sprintf(e_name,"%s",pdest);
}


//********************  set_isosurface_dialog_id()  *************************//
void set_graph_dialog_id(int ID)
{
	graph_dialog_ID = ID;
}

//********************  get_graph_dialog_id()  ******************************//
int get_graph_dialog_id()
{
	return(graph_dialog_ID);
}


/**************************************** get_export_param() ****************/
void get_export_param(int &smoothing, int &scale)
{
	smoothing = exp_smoothing->get_int_val();
	scale = exp_scale->get_int_val();
}


/**************************************** export_control_cb() ***************/
void export_control_cb( int control )
{
	switch (control)
	{
	case (0):{
		graph_dialog_ID = 1;
		expglui->close();
		break;
			 }
	case(10):{
		graph_dialog_ID = 10;
		expglui->close();
		break;
			}
	}
}

/**************************************** init_Export_Dialog ****************/
void set_Export_param(char *name, int format)
{
	sprintf(e_name,"");
	sprintf(e_format_name,"");

	_update_file_title(name);
	e_scale = 1;
	e_smoothing = 1;
	switch(format)
	{
	case(0):
		sprintf(e_format_name,"BMP");
		break;
	case(1):
		sprintf(e_format_name,"EPS");
		break;
	case(2):
		sprintf(e_format_name,"JPG");
		break;
	case(3):
		sprintf(e_format_name,"JP2");
		break;
	case(4):
		sprintf(e_format_name,"PPM");
		break;
	case(5):
		sprintf(e_format_name,"RAW");
		break;
	case(6):
		sprintf(e_format_name,"RGB");
		break;
	case(7):
		sprintf(e_format_name,"TGA");
		break;
	case(8):
		sprintf(e_format_name,"TIF");
		break;
	}
}

/**************************************** init_Export_Dialog ****************/
void init_Export_Dialog()
{
	exp_file_name->set_text(e_name);
	exp_file_format->set_text(e_format_name);
	exp_smoothing->set_int_val(e_smoothing);
	exp_scale->set_int_val(e_scale);
}

/**************************************** Export_Dialog() ****************/
void Export_Dialog(int ID)
{
	graph_dialog_ID = -1;
	expglui = GLUI_Master.create_glui("Export Graphics",0,180,140 );
	expglui->set_main_gfx_window( ID );
	exp_panel = expglui->add_panel("",GLUI_PANEL_RAISED);
//--------------------------------------------------------------------------------
	exp_file_name = expglui->add_edittext_to_panel(exp_panel,"File name:",GLUI_EDITTEXT_TEXT);
	exp_file_name->set_alignment(GLUI_ALIGN_RIGHT);
	exp_file_name->set_w(280);
	exp_file_name->disable();

	exp_file_format = expglui->add_edittext_to_panel(exp_panel," File format:",GLUI_EDITTEXT_TEXT);
	exp_file_format->set_alignment(GLUI_ALIGN_LEFT);
	exp_file_format->disable();

	expglui->add_separator_to_panel(exp_panel);

	exp_smoothing = expglui->add_spinner_to_panel(exp_panel," Smoothing:",GLUI_SPINNER_INT);
	exp_smoothing->set_int_limits(1,10,GLUI_LIMIT_CLAMP);
	exp_smoothing->set_alignment(GLUI_ALIGN_LEFT);

	exp_scale = expglui->add_spinner_to_panel(exp_panel," Scale:",GLUI_SPINNER_INT);
	exp_scale->set_int_limits(1,10,GLUI_LIMIT_CLAMP);
	exp_scale->set_alignment(GLUI_ALIGN_LEFT);

	expglui->add_separator_to_panel(exp_panel);

	expglui->add_button_to_panel(exp_panel,"Save", 10, export_control_cb);
//--------------------------------------------------------------------------------

	expglui->add_button("Close", 0, export_control_cb);

	init_Export_Dialog();
}







//********************  get_msg_export_dialog_id()  *************************//
int get_msg_export_dialog_id()
{
	return(msg_Image_ID);
}

/**************************************** msg_control_cb() ******************/
void msg_control_cb( int control )
{
	switch (control)
	{
	case (0):{
		msgglui->close();
		msg_Image_ID = 1;
		break;
			 }
	}
}

/**************************************** Msg_Export_Dialog() ***************/
void Msg_Export_Dialog(int ID)
{
	msg_Image_ID = -1;
	msgglui = GLUI_Master.create_glui("Export Graphics",0,180,140 );
	msgglui->set_main_gfx_window( ID );
	msg_panel = msgglui->add_panel("",GLUI_PANEL_RAISED);
	msgglui->add_button("OK", 0, msg_control_cb);

	msgglui->add_separator_to_panel(msg_panel);
	mline1 = msgglui->add_statictext_to_panel(msg_panel,"A graphics file from the Graphics Window");
	mline2 = msgglui->add_statictext_to_panel(msg_panel,"has successfully been created.");
	msgglui->add_separator_to_panel(msg_panel);
	mline3 = msgglui->add_statictext_to_panel(msg_panel,"Click OK to continue.");

	mline1->set_alignment(GLUI_ALIGN_CENTER);
	mline2->set_alignment(GLUI_ALIGN_CENTER);
	mline3->set_alignment(GLUI_ALIGN_CENTER);


}

