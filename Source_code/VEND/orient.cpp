//***************************************************************************//
//                      CRYSTAL ORIENTATION DIALOG                           //
//---------------------------------------------------------------------------//
// File: orient.cpp                                                          //
// Date: 2002.04.11                                                          //
//---------------------------------------------------------------------------//
// Control:                                                                  //
//   orient_dialog_ID = -1 -- Open Crystal Orientation Dialog                //
//                       1 -- Close Crystal Orientation Dialog ("OK")        //
//                       0 -- Close Crystal Orientation Dialog ("Cancel")    //
//***************************************************************************//
#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>
#include "vend.h"


// Objects
GLUI *oglui;                                //--- GLUI window
GLUI_Panel *o_main_panel;                   //--- ORIENTATION Main Panel
GLUI_Panel *o_btn_panel;                    //--- ORIENTATION OK / Cancel Btn Panel
GLUI_Panel *o_matrix_panel;                 //--- ORIENTATION Matrix Panel
GLUI_Panel *o_up_mode_panel;                //--- ORIENTATION Up Direction Selection Panel
GLUI_Panel *o_view_panel;                   //--- ORIENTATION View Direction Panel
GLUI_Panel *o_up_panel;                     //--- ORIENTATION Up Direction Panel

GLUI_StaticText *matrixX;                   //--- ORIENTATION Matrix
GLUI_StaticText *matrixY;
GLUI_StaticText *matrixZ;

GLUI_RadioGroup *o_cell_type_radio;         //--- ORIENTATION Unit Cell Type

GLUI_RadioGroup *o_up_vector_radio;         //--- ORIENTATION <UP> vector selection Type

GLUI_EditText *o_viewZ1;                    //--- ORIENTATION Direction
GLUI_EditText *o_viewZ2;
GLUI_EditText *o_viewZ3;

GLUI_EditText *o_viewY1;                    //--- ORIENTATION Up Direction
GLUI_EditText *o_viewY2;
GLUI_EditText *o_viewY3;

// Control
int orient_dialog_ID;                    //--- ORIENTATION dialog control ID
int p_window_ID;

float c_orientation[16], Orientation[16];
float cell[6];
char o_line_cell[25];

double M[9],MR[9];
double oV;
double a,b,c,ar,br,cr;

XYZ c_vector[3],r_vector[3];
XYZ cAx[3],rAx[3];

float RotMat[16],RotMatY[16];

/**************************************** get_orient_dialog_pos() ************/
void get_orient_dialog_pos(int &x, int &y)
{
	glutSetWindow(oglui->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}


//********************  minf3()  *********************************************//
double minf3(double a, double b, double c)
{
	double d;

	d = (fabs(a) <= fabs(b)) ? a : b;
	d = (fabs(d) <= fabs(c)) ? d : c;
	return(d);
}

//********************  minf2()  *********************************************//
double minf2(double a, double b)
{
	return((fabs(a) <= fabs(b)) ? a : b);
}


//********************  norm_vector()  **************************************//
XYZ cros_vector(XYZ S1, XYZ S2,double Volume)
{
	XYZ S;

	S.x = (S1.y * S2.z - S1.z * S2.y)/Volume;
	S.y = (S1.z * S2.x - S1.x * S2.z)/Volume;
	S.z = (S1.x * S2.y - S1.y * S2.x)/Volume;

	return(S);
}

//********************  norm_vector()  **************************************//
XYZ norm_vector(XYZ S)
{
	double d;
	d = sqrt(S.x*S.x + S.y*S.y + S.z*S.z);
	S.x /= d;
	S.y /= d;
	S.z /= d;
	return(S);
}

//********************  norm_vector()  **************************************//
double check_ortho_vector(XYZ S1, XYZ S2)
{
	double d,d1,d2;

	d1 = S1.x*S1.x + S1.y*S1.y + S1.z*S1.z;
	d2 = S2.x*S2.x + S2.y*S2.y + S2.z*S2.z;
	d = (S1.x*S2.x + S1.y*S2.y + S1.z*S2.z)/sqrt(d1*d2);
	return(d);
}


//********************  make_transl_matrix()  *******************************//
void make_transl_matrix()
{
	double ca,cb,cg,sa,sb,sg,car,cbr;
	double vol;

	a = cell[0];
	b = cell[1];
	c = cell[2];
	ca = cos(cell[3]*PI/180);cb = cos(cell[4]*PI/180);cg = cos(cell[5]*PI/180);
	sa = sin(cell[3]*PI/180);sb = sin(cell[4]*PI/180);sg = sin(cell[5]*PI/180);
	car = (cb * cg - ca) / (sb * sg);
	cbr = (ca * cg - cb) / (sa * sb);

	vol = 1 - ca * ca - cb * cb - cg * cg + 2 * ca * cb * cg;
	oV = a * b * c *sqrt(vol);

	ar = (b * c * sa) / oV;
	br = (a * c * sb) / oV;
	cr = (a * b * sg) / oV;

	MR[0] = a;     MR[3] = 0;          MR[6] = 0;
	MR[1] = b*cg;  MR[4] = b*sg;       MR[7] = 0;
	MR[2] = c*cb;  MR[5] = -c*sb*car;  MR[8] = 1 / cr;

	cAx[0].x = MR[0]; cAx[0].y = MR[3]; cAx[0].z = MR[6];
	cAx[1].x = MR[1]; cAx[1].y = MR[4]; cAx[1].z = MR[7];
	cAx[2].x = MR[2]; cAx[2].y = MR[5]; cAx[2].z = MR[8];

	rAx[0] = cros_vector(cAx[1],cAx[2],oV);
	rAx[1] = cros_vector(cAx[2],cAx[0],oV);
	rAx[2] = cros_vector(cAx[0],cAx[1],oV);

}

//********************  calc_vectors()  *************************************//
void calc_vectors(float o_mat[16])
{
	int i;

	for(i=0; i<3; i++){
		c_vector[i].x = o_mat[i]*MR[0] + o_mat[i+4]*MR[1] + o_mat[i+8]*MR[2];
		c_vector[i].y = o_mat[i]*MR[3] + o_mat[i+4]*MR[4] + o_mat[i+8]*MR[5];
		c_vector[i].z = o_mat[i]*MR[6] + o_mat[i+4]*MR[7] + o_mat[i+8]*MR[8];
	}
	r_vector[0] = cros_vector(c_vector[1],c_vector[2],oV);
	r_vector[1] = cros_vector(c_vector[2],c_vector[0],oV);
	r_vector[2] = cros_vector(c_vector[0],c_vector[1],oV);
	for(i=0; i<3; i++){
		c_vector[i] = norm_vector(c_vector[i]);
		r_vector[i] = norm_vector(r_vector[i]);
	}
}

//********************  set_matrix_tab()  ***********************************//
void set_matrix_tab()
{
	int i;
	char o_lineX[40],o_lineY[40],o_lineZ[40];

	i = o_cell_type_radio->get_int_val();
	sprintf(o_lineX,"");
	sprintf(o_lineY,"");
	sprintf(o_lineZ,"");
	if (i == 0) {
		sprintf(o_lineX,"X: %+10.4f %+10.4f %+10.4f",c_vector[0].x,c_vector[0].y,c_vector[0].z);
		sprintf(o_lineY,"Y: %+10.4f %+10.4f %+10.4f",c_vector[1].x,c_vector[1].y,c_vector[1].z);
		sprintf(o_lineZ,"Z: %+10.4f %+10.4f %+10.4f",c_vector[2].x,c_vector[2].y,c_vector[2].z);
	}
	else {
		sprintf(o_lineX,"X: %+10.4f %+10.4f %+10.4f",r_vector[0].x,r_vector[0].y,r_vector[0].z);
		sprintf(o_lineY,"Y: %+10.4f %+10.4f %+10.4f",r_vector[1].x,r_vector[1].y,r_vector[1].z);
		sprintf(o_lineZ,"Z: %+10.4f %+10.4f %+10.4f",r_vector[2].x,r_vector[2].y,r_vector[2].z);
	}
	matrixX->set_name(o_lineX);
	matrixY->set_name(o_lineY);
	matrixZ->set_name(o_lineZ);
}

//********************  set_ortho_vectorZ()  ********************************//
void set_ortho_vector()
{
	int i,j;
	float h1,k1,l1;
	float h2,k2,l2;
	XYZ VZ,VY,VX;
	double check_ortho;


	h1 = (float)o_viewZ1->get_int_val();
	k1 = (float)o_viewZ2->get_int_val();
	l1 = (float)o_viewZ3->get_int_val();
	h2 = (float)o_viewY1->get_int_val();
	k2 = (float)o_viewY2->get_int_val();
	l2 = (float)o_viewY3->get_int_val();

	i = o_cell_type_radio->get_int_val();

	switch(i)
	{
	case(0):{
		VZ.x = h1 * cAx[0].x + k1 * cAx[1].x + l1 * cAx[2].x; 
		VZ.y = h1 * cAx[0].y + k1 * cAx[1].y + l1 * cAx[2].y; 
		VZ.z = h1 * cAx[0].z + k1 * cAx[1].z + l1 * cAx[2].z; 
		VY.x = h2 * cAx[0].x + k2 * cAx[1].x + l2 * cAx[2].x; 
		VY.y = h2 * cAx[0].y + k2 * cAx[1].y + l2 * cAx[2].y; 
		VY.z = h2 * cAx[0].z + k2 * cAx[1].z + l2 * cAx[2].z; 
		break;
			}
	case(1):{
		VZ.x = h1 * rAx[0].x + k1 * rAx[1].x + l1 * rAx[2].x; 
		VZ.y = h1 * rAx[0].y + k1 * rAx[1].y + l1 * rAx[2].y; 
		VZ.z = h1 * rAx[0].z + k1 * rAx[1].z + l1 * rAx[2].z; 
		VY.x = h2 * rAx[0].x + k2 * rAx[1].x + l2 * rAx[2].x; 
		VY.y = h2 * rAx[0].y + k2 * rAx[1].y + l2 * rAx[2].y; 
		VY.z = h2 * rAx[0].z + k2 * rAx[1].z + l2 * rAx[2].z; 
		break;
			}
	}
	check_ortho = check_ortho_vector(VZ, VY);
	if (fabs(check_ortho) >= 0.1) {
		o_up_vector_radio->set_int_val(0);
		o_viewY1->disable();
		o_viewY2->disable();
		o_viewY3->disable();
		glutPostRedisplay();
	}

	j = o_up_vector_radio->get_int_val();
	switch(j)
	{
	case(1):{VX = cros_vector(VY,VZ,1);break;}
	case(0):{
		if ((fabs(VZ.x) <= 0.01) && (fabs(VZ.y) <= 0.01)) {
			VY.x = 0; VY.y = 1; VY.z = 0;
		}
		else if ((fabs(VZ.x) <= 0.01) && (fabs(VZ.z) <= 0.01)) {
			VY.x = 1; VY.y = 0; VY.z = 0;
		}
		else if ((fabs(VZ.y) <= 0.01) && (fabs(VZ.z) <= 0.01)) {
			VY.x = 0; VY.y = 1; VY.z = 0;
		}
		else {VY.x = VZ.y; VY.y = -VZ.x; VY.z = 0;}
		VX = cros_vector(VY,VZ,1);
		break;
			}
	}
	VZ = norm_vector(VZ);
	VY = norm_vector(VY);
	VX = norm_vector(VX);
	RotMat[0] = VX.x; RotMat[4] = VX.y; RotMat[ 8] = VX.z; RotMat[12] = 0;
	RotMat[1] = VY.x; RotMat[5] = VY.y; RotMat[ 9] = VY.z; RotMat[13] = 0;
	RotMat[2] = VZ.x; RotMat[6] = VZ.y; RotMat[10] = VZ.z; RotMat[14] = 0;
	RotMat[3] = 0;    RotMat[7] = 0;    RotMat[11] = 0;    RotMat[15] = 1;
}

//********************  set_orient_dialog_id()  *****************************//
void set_orient_dialog_id(int ID)
{
	orient_dialog_ID = ID;
}


//********************  get_orient_dialog_id()  *****************************//
int get_orient_dialog_id()
{
	return(orient_dialog_ID);
}


//********************  get_view_point()  ***********************************//
void get_view_point(float matr[16])
{
	int i;
	for(i=0; i<16; i++) matr[i] = RotMat[i];
}

//********************  set_orient_param() **********************************//
void set_orient_param(float c_orient[16])
{
	int i;

	for(i=0; i<16; i++) {Orientation[i] = c_orient[i];}
	calc_vectors(Orientation);
	set_matrix_tab();
}



//********************  orient_control() ************************************//
void orient_control( int control)
{
	int i,j;
	switch(control)
	{
	case(0):{
		orient_dialog_ID = 1; oglui->close(); 
		break; 
			}
	case(1):{
		for(i=0; i<16; i++) RotMat[i] = c_orientation[i];
		orient_dialog_ID = 0; oglui->close(); 
		break; 
			}
	case(10):{	
		orient_dialog_ID = -1;
		i = o_cell_type_radio->get_int_val();
		set_matrix_tab();
		if (i == 0) {
			o_viewZ1->set_name("u1: ");
			o_viewZ2->set_name("v1: ");
			o_viewZ3->set_name("w1: ");
			o_viewY1->set_name("u2: ");
			o_viewY2->set_name("v2: ");
			o_viewY3->set_name("w2: ");
		}
		else {
			o_viewZ1->set_name("h1: ");
			o_viewZ2->set_name("k1: ");
			o_viewZ3->set_name("l1: ");
			o_viewY1->set_name("h2: ");
			o_viewY2->set_name("k2: ");
			o_viewY3->set_name("l2: ");
		}
		break;
			 }
	case(15):{
		orient_dialog_ID = -1;
		i = o_up_vector_radio->get_int_val();
		if (i == 0){
			o_viewY1->disable();
			o_viewY2->disable();
			o_viewY3->disable();
		}
		else {
			o_viewY1->enable();
			o_viewY2->enable();
			o_viewY3->enable();
		}
		break;
			 }
	case(20):{
		j = 1;
		if ((o_viewZ1->get_int_val() == 0) && (o_viewZ2->get_int_val() == 0) && (o_viewZ3->get_int_val() == 0)) j = 0;
		if (o_up_vector_radio->get_int_val() == 1){
			if ((o_viewY1->get_int_val() == 0) && (o_viewY2->get_int_val() == 0) && (o_viewY3->get_int_val() == 0)) j = 0;
		}
		if (j == 0) orient_dialog_ID = -1;
		else {
			set_ortho_vector();
			orient_dialog_ID = 20;
		}
		break;
			 }
	}
}

//********************  init_orient_dialog_param() **************************//
void init_orient_dialog_param(float c_orient[16], float c_cell[6])
{
	int i;

	for(i=0; i<16; i++) {c_orientation[i] = Orientation[i] = c_orient[i];}
	for(i=0; i<6; i++) cell[i] = c_cell[i];
	make_transl_matrix();
	calc_vectors(Orientation);
}

//********************  init_orient_dialog() ********************************//
void init_orient_dialog()
{
	set_matrix_tab();
	o_viewZ1->set_int_val(0);
	o_viewZ2->set_int_val(0);
	o_viewZ3->set_int_val(1);
	o_viewY1->set_int_val(0);
	o_viewY2->set_int_val(1);
	o_viewY3->set_int_val(0);
}



//********************  open_orient_prop_dialog() ***************************//
void open_orient_prop_dialog(int ID, int dpX, int dpY)
{
	orient_dialog_ID = -1;
	p_window_ID = ID;

	oglui = GLUI_Master.create_glui("Orientation",0,dpX,dpY );
	oglui->set_main_gfx_window( ID );

	o_main_panel = oglui->add_panel("",GLUI_PANEL_RAISED);

	o_cell_type_radio = oglui->add_radiogroup_to_panel(o_main_panel,NULL,10,orient_control);
	oglui->add_radiobutton_to_group(o_cell_type_radio,"Lattice vector");
	oglui->add_radiobutton_to_group(o_cell_type_radio,"Plane normal");
	o_cell_type_radio->set_int_val(0);
	oglui->add_separator_to_panel(o_main_panel);

	o_matrix_panel = oglui->add_panel_to_panel(o_main_panel,"Current orientation",GLUI_PANEL_EMBOSSED);
	o_matrix_panel->set_alignment(GLUI_ALIGN_LEFT);
	matrixX = oglui->add_statictext_to_panel(o_matrix_panel,"");
	matrixY = oglui->add_statictext_to_panel(o_matrix_panel,"");
	matrixZ = oglui->add_statictext_to_panel(o_matrix_panel,"");

	oglui->add_separator_to_panel(o_main_panel);

	o_up_mode_panel = oglui->add_panel_to_panel(o_main_panel,"Upward direction",GLUI_PANEL_EMBOSSED);
	o_up_mode_panel->set_alignment(GLUI_ALIGN_CENTER);
	o_up_vector_radio = oglui->add_radiogroup_to_panel(o_up_mode_panel,NULL,15,orient_control);
	oglui->add_radiobutton_to_group(o_up_vector_radio,"Automatic selection");
	oglui->add_radiobutton_to_group(o_up_vector_radio,"Manual selection");
	o_up_vector_radio->set_int_val(1);

	oglui->add_column_to_panel(o_main_panel,false);

	o_view_panel = oglui->add_panel_to_panel(o_main_panel,"View direction",GLUI_PANEL_EMBOSSED);
	o_view_panel->set_alignment(GLUI_ALIGN_LEFT);
	oglui->add_statictext_to_panel(o_view_panel,"Direction of projection");
	o_viewZ1 = oglui->add_edittext_to_panel(o_view_panel,"u1: ",GLUI_EDITTEXT_INT);
	o_viewZ2 = oglui->add_edittext_to_panel(o_view_panel,"v1: ",GLUI_EDITTEXT_INT);
	o_viewZ3 = oglui->add_edittext_to_panel(o_view_panel,"w1: ",GLUI_EDITTEXT_INT);
	oglui->add_separator_to_panel(o_view_panel);
	oglui->add_statictext_to_panel(o_view_panel,"Upward direction on the screen");
	o_viewY1 = oglui->add_edittext_to_panel(o_view_panel,"u2: ",GLUI_EDITTEXT_INT);
	o_viewY2 = oglui->add_edittext_to_panel(o_view_panel,"v2: ",GLUI_EDITTEXT_INT);
	o_viewY3 = oglui->add_edittext_to_panel(o_view_panel,"w2: ",GLUI_EDITTEXT_INT);
	oglui->add_separator_to_panel(o_view_panel);
	oglui->add_button_to_panel(o_view_panel,"Apply", 20, orient_control);


	o_btn_panel = oglui->add_panel("",GLUI_PANEL_RAISED);
	oglui->add_button_to_panel(o_btn_panel,"OK", 0, orient_control);
	oglui->add_column_to_panel(o_btn_panel,false);
	oglui->add_button_to_panel(o_btn_panel,"Cancel", 1, orient_control);

	init_orient_dialog();
}

