//***************************************************************************//
//                        SLICE PROPERTIES DIALOG                            //
//---------------------------------------------------------------------------//
// File: slice.cpp                                                           //
// Date: 2002.02.28                                                          //
//***************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>

//--- Objects
GLUI *sliglui;                                  //--- GLUI window
GLUI_Panel *sli_main_panel;                     //--- Main Dialog Panel
GLUI_Panel *sli_hkl_panel;                      //--- SLICE (hkl) Plane Panel
GLUI_Panel *sli_hkl_panel1;                      //--- SLICE (hkl) Plane Panel
GLUI_Panel *sli_short_panel;                    //--- SLICE Shortcuts Panel
GLUI_Panel *sli_show_panel;                     //--- SLICE Show Planes Panel
GLUI_Panel *sli_sett_panel;                     //--- SLICE Settings Planes Panel

GLUI_EditText *sli_planeH_txt;                  //--- SLICE (hkl) Plane
GLUI_EditText *sli_planeK_txt;                  //---
GLUI_EditText *sli_planeL_txt;                  //---

GLUI_Spinner *sli_position_spin;                //--- SLICE Plane Position
GLUI_Spinner *sli_opacity_spin;                 //--- SLICE Plane Opacity

GLUI_Spinner *sli_objlevel_spin;                //--- SLICE OBJLEV Scale
GLUI_Spinner *sli_seclevel_spin;                //--- SLICE SECLEV Scale
GLUI_Listbox *sli_palette_list;                 //--- SLICE Palette

GLUI_Button *sli_100_btn;                       //--- SLICE Shortcuts Button for (h00) plane
GLUI_Button *sli_010_btn;                       //--- SLICE Shortcuts Button for (0k0) plane
GLUI_Button *sli_001_btn;                       //--- SLICE Shortcuts Button for (00l) plane

GLUI_Button *add_btn;                           //--- SLICE Add to List Button
GLUI_Button *show_btn;
GLUI_Button *clear_btn;

GLUI_Checkbox *sli_prev_check;                  //--- SLICE Preview Control

GLUI_Checkbox *sli_plane1_check;
GLUI_Checkbox *sli_plane2_check;
GLUI_Checkbox *sli_plane3_check;


//--- Control
int sli_dialog_ID;
int sli_mode_ID = 0;
short num_List = 0;
float opac_hkl;
float pos_hkl[3],c_pos_hkl;
int ind_hkl[3][3],c_ind_hkl[3];
short ind_show_hkl[3];
float obj_scale, int_scale;
int hkl_palette;
GLUI_String list1, list2, list3;
GLUI_String cmd;

/**************************************** get_about_dialog_pos() ************/
void get_sli_dialog_pos(int &x, int &y)
{
	glutSetWindow(sliglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  set_sli_dialog_id()  ********************************//
void set_sli_dialog_id(int ID)
{
	sli_dialog_ID = ID;
}

//********************  get_sli_dialog_id()  ********************************//
int get_sli_dialog_id()
{
	return(sli_dialog_ID);
}

//********************  get_sli_preview_id()  *******************************//
int get_sli_preview_id()
{
	return(sli_prev_check->get_int_val());
}

//********************  get_sli_mode_id()  **********************************//
int get_sli_mode_id()
{
	return(sli_mode_ID);
}

//********************  get_sli_show_param()  *******************************//
void get_sli_show_param(short show_hkl[3])
{
	show_hkl[0] = ind_show_hkl[0] = sli_plane1_check->get_int_val();
	show_hkl[1] = ind_show_hkl[1] = sli_plane2_check->get_int_val();
	show_hkl[2] = ind_show_hkl[2] = sli_plane3_check->get_int_val();
	
}

//********************  get_sli_cur_param()  ********************************//
void get_sli_param(int &Num, float position[3], short hkl[3][3], float &opacity,int &palette, float &scale1, float &scale2)
{
	int i;

	Num = num_List;
	for(i=0; i<num_List; i++){
		position[i] = pos_hkl[i];
		hkl[i][0] = ind_hkl[i][0];hkl[i][1] = ind_hkl[i][1];hkl[i][2] = ind_hkl[i][2];
		opacity = (float)sli_opacity_spin->get_int_val()/100;
		scale1 = sli_objlevel_spin->get_float_val();
		scale2 = sli_seclevel_spin->get_float_val();
	}
	palette = hkl_palette;
}


//********************  get_sli_cur_param()  ********************************//
void get_sli_cur_param(float &position, short hkl[3], float &opacity,int &palette, float &scale1, float &scale2)
{
	position = c_pos_hkl;
	hkl[0] = c_ind_hkl[0];hkl[1] = c_ind_hkl[1];hkl[2] = c_ind_hkl[2];
	opacity = (float)sli_opacity_spin->get_int_val()/100;
	scale1 = sli_objlevel_spin->get_float_val();
	scale2 = sli_seclevel_spin->get_float_val();
	palette = hkl_palette;
}


//********************  sli_control_cb()  ***********************************//
void sli_control_cb( int control )
{
	sprintf(cmd,"");
	switch(control)
	{
	case(0):{
		sliglui->close();
		sli_dialog_ID = 1;
		break;
			}
//------------------------------------------------------------ Show Plane
	case(10):{
		sli_dialog_ID = 10;
		sli_mode_ID = 0;
		c_ind_hkl[0] = sli_planeH_txt->get_int_val();
		c_ind_hkl[1] = sli_planeK_txt->get_int_val();
		c_ind_hkl[2] = sli_planeL_txt->get_int_val();
		c_pos_hkl = sli_position_spin->get_float_val();
		break;
			 }
//------------------------------------------------------------ Add to List
	case(11):{
		sli_dialog_ID = -1;
		c_ind_hkl[0] = sli_planeH_txt->get_int_val();
		c_ind_hkl[1] = sli_planeK_txt->get_int_val();
		c_ind_hkl[2] = sli_planeL_txt->get_int_val();
		c_pos_hkl = sli_position_spin->get_float_val();
		if ((c_ind_hkl[0] == 0) && (c_ind_hkl[1] == 0) && (c_ind_hkl[2] == 0)) break;

		ind_hkl[num_List][0] = c_ind_hkl[0];
		ind_hkl[num_List][1] = c_ind_hkl[1];
		ind_hkl[num_List][2] = c_ind_hkl[2];
		pos_hkl[num_List] = c_pos_hkl;
		
		num_List++;
		sprintf(cmd,"Add to list (%i)",num_List);
		add_btn->set_name(cmd);
		if (num_List == 3) add_btn->disable();
		sli_plane1_check->set_int_val(0);
		sli_plane2_check->set_int_val(0);
		sli_plane3_check->set_int_val(0);
		ind_show_hkl[0] = ind_show_hkl[1] = ind_show_hkl[2] = 0;
		switch(num_List)
		{
		case(1):{
			sprintf(list1,"(%i %i %i)  %5.3f",ind_hkl[0][0],ind_hkl[0][1],ind_hkl[0][2],pos_hkl[0]);
			sli_plane1_check->set_name(list1);
			sli_plane1_check->enable();
			sli_plane1_check->set_int_val(1);
			ind_show_hkl[0] = 1;
			break;
				}
		case(2):{
			sprintf(list2,"(%i %i %i)  %5.3f",ind_hkl[1][0],ind_hkl[1][1],ind_hkl[1][2],pos_hkl[1]);
			sli_plane2_check->set_name(list2);
			sli_plane2_check->enable();
			sli_plane2_check->set_int_val(1);
			ind_show_hkl[1] = 1;
			break;
				}
		case(3):{
			sprintf(list3,"(%i %i %i)  %5.3f",ind_hkl[2][0],ind_hkl[2][1],ind_hkl[2][2],pos_hkl[2]);
			sli_plane3_check->set_name(list3);
			sli_plane3_check->enable();
			sli_plane3_check->set_int_val(1);
			ind_show_hkl[2] = 1;
			break;
				}
		}
		break;
			 }
//------------------------------------------------------------ Show List
	case(12):{
		sli_dialog_ID = 12;
		sli_mode_ID = 1;
		break;
			 }
//------------------------------------------------------------ Clear List
	case(13):{
		sli_dialog_ID = 13;
		num_List = 0;
		sprintf(cmd,"Add to list (%i)",num_List);
		add_btn->set_name(cmd);
		add_btn->enable();
		sprintf(list1,"");
		sprintf(list2,"");
		sprintf(list3,"");
		sli_plane1_check->set_name(list1);sli_plane1_check->set_int_val(0);sli_plane1_check->disable();
		sli_plane2_check->set_name(list2);sli_plane2_check->set_int_val(0);sli_plane2_check->disable();
		sli_plane3_check->set_name(list3);sli_plane3_check->set_int_val(0);sli_plane3_check->disable();
		ind_show_hkl[0] = ind_show_hkl[1] = ind_show_hkl[2] = 0;
		break;
			 }
//------------------------------------------------------------ Shortcuts
	case(20):{                                          //---- (100)
		sli_mode_ID = 0;
		sli_dialog_ID = 20;
		sli_planeH_txt->set_int_val(1);
		sli_planeK_txt->set_int_val(0);
		sli_planeL_txt->set_int_val(0);
		c_ind_hkl[0] = 1;
		c_ind_hkl[1] = 0;
		c_ind_hkl[2] = 0;
		c_pos_hkl = sli_position_spin->get_float_val();
		break;
			 }
	case(21):{                                          //---- (010)
		sli_mode_ID = 0;
		sli_dialog_ID = 21;
		sli_planeH_txt->set_int_val(0);
		sli_planeK_txt->set_int_val(1);
		sli_planeL_txt->set_int_val(0);
		c_ind_hkl[0] = 0;
		c_ind_hkl[1] = 1;
		c_ind_hkl[2] = 0;
		c_pos_hkl = sli_position_spin->get_float_val();
		break;
			 }
	case(22):{                                          //---- (001)
		sli_mode_ID = 0;
		sli_dialog_ID = 22;
		sli_planeH_txt->set_int_val(0);
		sli_planeK_txt->set_int_val(0);
		sli_planeL_txt->set_int_val(1);
		c_ind_hkl[0] = 0;
		c_ind_hkl[1] = 0;
		c_ind_hkl[2] = 1;
		c_pos_hkl = sli_position_spin->get_float_val();
		break;
			 }
//------------------------------------------------------------ Opacity
	case(30):{
		sli_dialog_ID = 30;
		opac_hkl = (float)sli_opacity_spin->get_int_val()/100;
		hkl_palette = sli_palette_list->get_int_val();
			 }
//------------------------------------------------------------ Palette
	case(40):{
		sli_dialog_ID = 40;
		hkl_palette = sli_palette_list->get_int_val();
			 }
	}
}

//********************  init_slices_dialog()  ***************************//
void set_slices_dialog_param(int Num, float opacity, float position[3],short hkl[3][3],short show_hkl[3],
							 int palette,float scale1, float scale2)
{
	int i,j;


	obj_scale = scale1;
	int_scale = scale2;
	num_List = Num;
	opac_hkl = opacity;
	hkl_palette = palette;
	if (num_List == 0) {
		c_pos_hkl = 0.5;
		c_ind_hkl[0] = 1;
		c_ind_hkl[1] = 0;
		c_ind_hkl[2] = 0;
		ind_show_hkl[0] = ind_show_hkl[1] = ind_show_hkl[2] = 0;
	}
	else {
		for(i=0; i<num_List; i++){
			pos_hkl[i] = position[i];
			for(j=0; j<3; j++) ind_hkl[i][j] = hkl[i][j];
			c_pos_hkl = pos_hkl[0];
			c_ind_hkl[0] = ind_hkl[0][0];
			c_ind_hkl[1] = ind_hkl[0][1];
			c_ind_hkl[2] = ind_hkl[0][2];
			ind_show_hkl[0] = show_hkl[0];
			ind_show_hkl[1] = show_hkl[1];
			ind_show_hkl[2] = show_hkl[2];
		}
	}
}


//********************  init_slices_dialog()  ***************************//
void init_slices_dialog()
{
	int i;
	sprintf(cmd,"");
	sli_planeH_txt->set_int_val(c_ind_hkl[0]);
	sli_planeK_txt->set_int_val(c_ind_hkl[1]);
	sli_planeL_txt->set_int_val(c_ind_hkl[2]);
	sli_position_spin->set_float_val(c_pos_hkl);
	sli_opacity_spin->set_int_val((int)(opac_hkl*100));
	sli_objlevel_spin->set_float_val(obj_scale);
	sli_seclevel_spin->set_float_val(int_scale);
	sli_prev_check->set_int_val(1);
	sli_palette_list->set_int_val(hkl_palette);

	sprintf(cmd,"Add to list (%i)",num_List);
	add_btn->set_name(cmd);
	if (num_List == 3) add_btn->disable();
	sprintf(list1,"");
	sprintf(list2,"");
	sprintf(list3,"");
	sli_plane1_check->set_name(list1);sli_plane1_check->set_int_val(0);sli_plane1_check->disable();
	sli_plane2_check->set_name(list2);sli_plane1_check->set_int_val(0);sli_plane2_check->disable();
	sli_plane3_check->set_name(list3);sli_plane1_check->set_int_val(0);sli_plane3_check->disable();
	if (num_List > 0){
		for(i=0; i<num_List; i++){
			switch(i)
			{
			case(0):{
				sprintf(list1,"(%i %i %i)  %5.3f",ind_hkl[0][0],ind_hkl[0][1],ind_hkl[0][2],pos_hkl[0]);
				sli_plane1_check->set_name(list1);
				sli_plane1_check->set_int_val(ind_show_hkl[0]);
				sli_plane1_check->enable();
				break;
					}
			case(1):{
				sprintf(list2,"(%i %i %i)  %5.3f",ind_hkl[1][0],ind_hkl[1][1],ind_hkl[1][2],pos_hkl[1]);
				sli_plane2_check->set_name(list2);
				sli_plane2_check->set_int_val(ind_show_hkl[1]);
				sli_plane2_check->enable();
				break;
					}
			case(2):{
				sprintf(list3,"(%i %i %i)  %5.3f",ind_hkl[2][0],ind_hkl[2][1],ind_hkl[2][2],pos_hkl[2]);
				sli_plane3_check->set_name(list3);
				sli_plane3_check->set_int_val(ind_show_hkl[2]);
				sli_plane3_check->enable();
				break;
					}
			}
		}
	}
}


//********************  open_slices_dialog()  ***************************//
void open_slices_dialog(int ID, int dpX, int dpY)
{
	sprintf(list1,"");
	sprintf(list2,"");
	sprintf(list3,"");
	sprintf(cmd,"Add to list (0)");
	sli_dialog_ID = -1;
	sliglui = GLUI_Master.create_glui("Slices",0,dpX,dpY );
	sliglui->set_main_gfx_window( ID );
	sli_main_panel = sliglui->add_panel("",GLUI_PANEL_RAISED);

	sli_hkl_panel = sliglui->add_panel_to_panel(sli_main_panel,"Planes",GLUI_PANEL_EMBOSSED);
	sli_hkl_panel->set_alignment(GLUI_ALIGN_LEFT);

	sli_hkl_panel1 = sliglui->add_panel_to_panel(sli_hkl_panel,"",GLUI_PANEL_NONE);
	sli_hkl_panel1->set_alignment(GLUI_ALIGN_LEFT);

	sli_planeH_txt = sliglui->add_edittext_to_panel(sli_hkl_panel1,"h: ",GLUI_EDITTEXT_INT);
	sli_planeK_txt = sliglui->add_edittext_to_panel(sli_hkl_panel1,"k: ",GLUI_EDITTEXT_INT);
	sli_planeL_txt = sliglui->add_edittext_to_panel(sli_hkl_panel1,"l: ",GLUI_EDITTEXT_INT);

	sliglui->add_column_to_panel(sli_hkl_panel1,false);

	show_btn = sliglui->add_button_to_panel(sli_hkl_panel1,"Show planes", 10, sli_control_cb);
	add_btn = sliglui->add_button_to_panel(sli_hkl_panel1,cmd, 11, sli_control_cb);
	clear_btn = sliglui->add_button_to_panel(sli_hkl_panel1,"Clear", 13, sli_control_cb);

	sli_short_panel = sliglui->add_panel_to_panel(sli_hkl_panel,"Shortcuts",GLUI_PANEL_EMBOSSED);
	sli_short_panel->set_alignment(GLUI_ALIGN_LEFT);

	sli_100_btn = sliglui->add_button_to_panel(sli_short_panel,"(100)",20,sli_control_cb);
	sli_100_btn->set_w(78);
	sliglui->add_column_to_panel(sli_short_panel,false);
	sli_010_btn = sliglui->add_button_to_panel(sli_short_panel,"(010)",21,sli_control_cb);
	sli_010_btn->set_w(78);
	sliglui->add_column_to_panel(sli_short_panel,false);
	sli_001_btn = sliglui->add_button_to_panel(sli_short_panel,"(001)",22,sli_control_cb);
	sli_001_btn->set_w(78);

	sli_position_spin = sliglui->add_spinner_to_panel(sli_hkl_panel,"Shift: ",GLUI_SPINNER_FLOAT);
	sli_position_spin->set_float_limits(0,1,GLUI_LIMIT_CLAMP);
	sli_position_spin->set_alignment(GLUI_ALIGN_RIGHT);

	sli_sett_panel = sliglui->add_panel_to_panel(sli_main_panel,"Settings",GLUI_PANEL_EMBOSSED);
	sli_sett_panel->set_alignment(GLUI_ALIGN_LEFT);

	sli_objlevel_spin = sliglui->add_spinner_to_panel(sli_sett_panel,"Scale factor for isosurface level: ",GLUI_SPINNER_FLOAT);
	sli_objlevel_spin->set_float_limits(0,1,GLUI_LIMIT_CLAMP);
	sli_objlevel_spin->set_alignment(GLUI_ALIGN_RIGHT);

	sli_seclevel_spin = sliglui->add_spinner_to_panel(sli_sett_panel,"Scale factor for f(section): ",GLUI_SPINNER_FLOAT);
	sli_seclevel_spin->set_float_limits(0.01,1,GLUI_LIMIT_CLAMP);
	sli_seclevel_spin->set_alignment(GLUI_ALIGN_RIGHT);

	sli_palette_list = sliglui->add_listbox_to_panel(sli_sett_panel,"Palette: ",NULL,40,sli_control_cb);
	sli_palette_list->add_item(0,"Rainbow");
	sli_palette_list->add_item(1,"Invers Rainbow");
	sli_palette_list->add_item(2,"Gray");
	sli_palette_list->set_alignment(GLUI_ALIGN_RIGHT);

	sli_opacity_spin = sliglui->add_spinner_to_panel(sli_main_panel,"Opacity (%): ",GLUI_SPINNER_INT,
		                                             NULL,30,sli_control_cb);

	sli_opacity_spin->set_int_limits(0,100,GLUI_LIMIT_CLAMP);


	sli_prev_check = sliglui->add_checkbox_to_panel(sli_main_panel,"Preview");

	sliglui->add_column_to_panel(sli_main_panel,true);
	sli_show_panel = sliglui->add_panel_to_panel(sli_main_panel,"Select planes",GLUI_PANEL_EMBOSSED);
	sli_plane1_check = sliglui->add_checkbox_to_panel(sli_show_panel,list1,NULL,12,sli_control_cb);
	sli_plane2_check = sliglui->add_checkbox_to_panel(sli_show_panel,list2,NULL,12,sli_control_cb);
	sli_plane3_check = sliglui->add_checkbox_to_panel(sli_show_panel,list3,NULL,12,sli_control_cb);

	sliglui->add_button("OK", 0, sli_control_cb);
	init_slices_dialog();
}



