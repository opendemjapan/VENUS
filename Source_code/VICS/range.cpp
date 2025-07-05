//***************************************************************************//
//                         PLOT RANGE DIALOG                                 //
//---------------------------------------------------------------------------//
// File: range.cpp                                                           //
// Date: 2002.01.11                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   plot_range_dialog_ID =   -1 -- Open Plot Range Dialog                   //
//                             1 -- Close Plot Range Dialog ("OK")           //
//***************************************************************************//
#include <GL/glui.h>
#include <GL/glut.h>

// Objects
GLUI *prglui;                               //--- Plot Range GLUI window
//-------------------------------------------- Plot Range
GLUI_Panel *plot_range_panel;
GLUI_Panel *block_panel;
GLUI_EditText *range_Xmin_txt;
GLUI_EditText *range_Xmax_txt;
GLUI_EditText *range_Ymin_txt;
GLUI_EditText *range_Ymax_txt;
GLUI_EditText *range_Zmin_txt;
GLUI_EditText *range_Zmax_txt;

// Control
int plot_range_Dialog_ID;                    //--- Plot Range dialog control ID
float prange[6];

/**************************************** get_about_dialog_pos() ************/
void get_range_dialog_pos(int &x, int &y)
{
	glutSetWindow(prglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}



/**************************************** get_plot_range_param() ************/
void get_plot_range_param(float range[6])
{
	int i;
	for(i=0; i<6; i++) range[i] = prange[i];
}

/**************************************** get_plot_range_dialog_id() ********/
int get_plot_range_dialog_id()
{
	return(plot_range_Dialog_ID);
}

/**************************************** plot_range_control() **************/
void plot_range_control( int control)
{
	switch(control)
	{
	case(0):{plot_range_Dialog_ID = 1; prglui->close(); break; }
	}
}

/**************************************** set_plot_range_param() ************/
void set_plot_range_param(float range[6])
{
	int i;
	for(i=0; i<6; i++) prange[i] = range[i];
}

/**************************************** open_plot_range_dialog() **********/
void open_plot_range_dialog(int ID, int dpX, int dpY)
{
	plot_range_Dialog_ID = -1;

	prglui = GLUI_Master.create_glui("Plot Range",0,dpX,dpY );
	prglui->set_main_gfx_window( ID );

//--------------------------------------------------------------------------
	plot_range_panel = prglui->add_panel("Range of fractional coordinates",GLUI_PANEL_EMBOSSED);
	plot_range_panel->set_alignment(GLUI_ALIGN_LEFT);
//--------------------------------------------------------------------------
	block_panel = prglui->add_panel_to_panel(plot_range_panel,"",GLUI_PANEL_NONE);
	range_Xmin_txt = prglui->add_edittext_to_panel(block_panel,"x (min): ",GLUI_EDITTEXT_FLOAT,&prange[0]);
	prglui->add_separator_to_panel(block_panel);
	range_Ymin_txt = prglui->add_edittext_to_panel(block_panel,"y (min): ",GLUI_EDITTEXT_FLOAT,&prange[2]);
	prglui->add_separator_to_panel(block_panel);
	range_Zmin_txt = prglui->add_edittext_to_panel(block_panel,"z (min): ",GLUI_EDITTEXT_FLOAT,&prange[4]);
	prglui->add_column_to_panel(block_panel,false);
	range_Xmax_txt = prglui->add_edittext_to_panel(block_panel,"x (max): ",GLUI_EDITTEXT_FLOAT,&prange[1]);
	prglui->add_separator_to_panel(block_panel);
	range_Ymax_txt = prglui->add_edittext_to_panel(block_panel,"y (max): ",GLUI_EDITTEXT_FLOAT,&prange[3]);
	prglui->add_separator_to_panel(block_panel);
	range_Zmax_txt = prglui->add_edittext_to_panel(block_panel,"z (max): ",GLUI_EDITTEXT_FLOAT,&prange[5]);
//--------------------------------------------------------------------------
	
	prglui->add_button("OK", 0, plot_range_control);

}
