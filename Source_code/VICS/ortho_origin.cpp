//***************************************************************************//
//               ORIGIN SETTINGS FO ORTHORHOMBIC SPACE GROUPS                //
//                           (48, 50, 59, 68, 70)                            //
//---------------------------------------------------------------------------//
// File: ortho_origin.cpp                                                    //
// Date: 2002.11.11                                                          //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>

// Objects
GLUI *orig;      
GLUI_Panel *orig_panel;

GLUI_EditText *orig_number;
GLUI_EditText *orig_info;
GLUI_Spinner *orig_spin;

// Control
int orig_Dialog_ID;
int sp_num,sp_org;
GLUI_String sp_name;
GLUI_String sp_info;

/**************************************** get_origin_dialog_pos() ************/
void get_origin_dialog_pos(int &x, int &y)
{
	glutSetWindow(orig->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


/**************************************** get_ortho_origin_dialog_id() ******/
int get_ortho_origin_dialog_id()
{
	return(orig_Dialog_ID);
}

/**************************************** get_ortho_origin() ****************/
int get_ortho_origin_dialog()
{
	return(sp_org-1);
}

/**************************************** update_info() *********************/
void update_info()
{
	int ID;

	sprintf(sp_info,"");
	ID = sp_num*10 + sp_org;
	switch(ID)
	{
	case(481):{
		sprintf(sp_info,"Origin at 222, at 1/4,1/4,1/4 from -1");
		break;
			  }
	case(482):{
		sprintf(sp_info,"Origin at -1, at -1/4,-1/4,-1/4 from 222");
		break;
			  }
	case(501):{
		sprintf(sp_info,"Origin at 222/n, at 1/4,1/4,0 from -1");
		break;
			  }
	case(502):{
		sprintf(sp_info,"Origin at -1, at -1/4,-1/4,0 from 222");
		break;
			  }
	case(591):{
		sprintf(sp_info,"Origin at mm2/n, at 1/4,1/4,0 from -1");
		break;
			  }
	case(592):{
		sprintf(sp_info,"Origin at 2(1)2(1)n, at -1/4,-1/4,0 from mm2");
		break;
			  }
	case(681):{
		sprintf(sp_info,"Origin at 222 at 2/n2/n2, at 0,1/4,1/4 from -1");
		break;
			  }
	case(682):{
		sprintf(sp_info,"Origin at nca, at 0,-1/4,-1/4 from 222");
		break;
			  }
	case(701):{
		sprintf(sp_info,"Origin at 222, at -1/8,-1/8,-1/8 from -1");
		break;
			  }
	case(702):{
		sprintf(sp_info,"Origin at -1 at ddd, at 1/8,1/8,1/8 from 222");
		break;
			  }
	}
	orig_info->set_text(sp_info);
}

/**************************************** orig_control_cb() *****************/
void orig_control_cb( int control)
{
	switch(control)
	{
	case(0):{
		orig_Dialog_ID = 1; orig->close(); 
		break; 
			}
	case(10):{
		orig_Dialog_ID = -1;
		sp_org = orig_spin->get_int_val();
		update_info();
		break;
			 }
	}
}

/**************************************** set_ortho_origin_dialog() *********/
void set_ortho_origin_dialog(int number, int orgID)
{
	sprintf(sp_info,"");
	sp_num = number;
	sp_org = orgID+1;
}

/**************************************** init_ortho_origin_dialog() ********/
void init_ortho_origin_dialog()
{
	orig_number->set_int_val(sp_num);
	orig_spin->set_int_val(sp_org);
	update_info();
}

/**************************************** open_ortho_origin_dialog() ********/
void open_ortho_origin_dialog(int ID, int dpX, int dpY)
{
	orig_Dialog_ID = -1;

	orig = GLUI_Master.create_glui("Origin Choice",0,dpX, dpY );
	orig->set_main_gfx_window( ID );

	orig_panel = orig->add_panel("",GLUI_PANEL_RAISED);

	orig_number = orig->add_edittext_to_panel(orig_panel,"Space group number: ",GLUI_EDITTEXT_INT,&sp_num);
	orig_number->disable();

	orig->add_separator_to_panel(orig_panel);

	orig_spin = orig->add_spinner_to_panel(orig_panel,"Origin: ",GLUI_SPINNER_INT,NULL,10,orig_control_cb);
	orig_spin->set_int_limits(1,2,GLUI_LIMIT_CLAMP);
	orig_info = orig->add_edittext_to_panel(orig_panel,"Info: ",GLUI_EDITTEXT_TEXT,&sp_info);
	orig_info->set_w(400);

	orig->add_button("OK", 0, orig_control_cb);
	init_ortho_origin_dialog();
}

