#include <string.h>
#include <stdio.h>
#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>

#include "main_param.h"
#include "about.h"
#include "contour2D.h"
#include "general.h"
#include "graph.h"
#include "info.h"
#include "isosurface.h"
#include "message.h"
#include "model.h"
#include "open.h"
#include "orient.h"
#include "prefer.h"
#include "projection2D.h"
#include "slice.h"
#include "stepw.h"
#include "ucell.h"
#include "winopen.h"


extern "C"
{
#include "bitmaplib.h"
#include "d2dwindow.h"
#include "d3dwindow.h"
#include "GlutPalette.h"
#include "shape.h"
#include "struc.h"
#include "tools.h"
#include "trackball.h"
}

int motion = 0;
int orientation = 0;
int start_VEND = 0;
int start_VICS = 0;

//**************************  initTrackBall()   *****************************//
void initTrackBall(void)
{
	tbInit(GLUT_LEFT_BUTTON,rotation);
	tbAnimate(GL_FALSE);
}


/**************************************** _update_window_title() ************/
void _update_window_title()
{
	char *pdest;
	char nTitle[300];

	sprintf(nTitle,"");
	pdest = strrchr(FileName, '\\');
    if (pdest == NULL) pdest = strrchr(FileName, '/');
    if (pdest != NULL){
       pdest++;
       sprintf(nTitle,"Graphics: %s",pdest);
	}
    else {
		if (strlen(FileName) > 0) sprintf(nTitle,"Graphics: %s",FileName);
		else sprintf(nTitle,"Graphics");
	}
	glutSetWindowTitle(nTitle);
}


/**************************************** updateStatusBar() *****************/
void updateStatusBar(void)
{
	char info2[20],info1[10],info3[17];

	sprintf(info1,"[%ix%i]",tw,th);
//--------------------------------------------------- For Auto/Mnual Rotation Switch
	switch(rotation_type)
	{
	case (0):{sprintf(info2,"Drag");break;}
	case (1):{sprintf(info2,"Push");break;}
	case (2):{sprintf(info2,"Click");break;}
	}
//--------------------------------------------------- For Rotation
	sprintf(info3,"");
		switch(rotation_mode)
			{
			case (0):{sprintf(info3,"(Free rotation)");break;}
			case (1):{sprintf(info3,"(X axis)");break;}
			case (2):{sprintf(info3,"(Y axis)");break;}
			case (3):{
                              if (rotation_type == 2) sprintf(info3,"(Free rotation)");
                              else sprintf(info3,"(Z axis)");
                              break;
                                 }
			}
//--------------------------------------------------- For Manipulation
	switch(tools_mode){
		case (0):{sprintf(statustext,"%s %s %s",info1,info2,info3);break;}
		case (1):{sprintf(statustext,"%s %s",info1,"Magnify");break;}
		case (2):{sprintf(statustext,"%s %s",info1,"Translate");break;}
	}
	statusbar->set_name(statustext);
}


/**************************************** updateStatusBar1() *****************/
void updateStatusBar1(void)
{
	char info2[20],info1[10],info3[17];

	sprintf(info1,"[%ix%i]",tw,th);
	sprintf(info2,"");
	sprintf(info2,"Stepwise rotation");
//--------------------------------------------------- For Rotation
	sprintf(info3,"");
	if (stepwise_active == 1) {
		switch(stepw_rot_axis)
		{
		case (0):{sprintf(info3,"(X axis)");break;}
		case (1):{sprintf(info3,"(Y axis)");break;}
		case (2):{sprintf(info3,"(Z axis)");break;}
		}
	}
	sprintf(statustext,"%s %s %s",info1,info2,info3);
	statusbar->set_name(statustext);
}


//**************************  draw_string_bitmap()   ************************//
void draw_string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}




//**************************  print_out_info()   ****************************//
void print_out_info()
{
	printf("-------------------------------------------------------\n");
	printf("Title                %s\n",Title);
	printf("Dimension            %i %i %i\n",NPIX[0],NPIX[1],NPIX[2]);
	printf("\n");
	printf("Lattice parameters\n");
	printf("\n");
	printf("%8.5f %8.5f %8.5f %8.4f %8.4f %8.4f\n",cCell[0],cCell[1],cCell[2],
		                                           cCell[3],cCell[4],cCell[5]);
	printf("-------------------------------------------------------\n\n");
}


//**************************  create_Atom_list() ****************************//

void create_Atom_list()
{
	int i,k,l,I1,I2;
	float R;
	float ar_scale;
	float br_scale;
	float bond_R;
	float x,y,z;
	float dx,dy; 
	float a_color[] = {1,1,1,1};
	GLUquadricObj *quadObj;
	int J1,J2;
	float R1,R2,dist1,dist2;

	if (radii_type != 2) ar_scale = 0.4/max_P;
	else ar_scale = 0.25/max_P;
	br_scale = 0.2/max_P;
	if (model_type == 4) bond_R = br_scale*bRadii[1];
	else bond_R = br_scale*bRadii[0];

	bound[0] = ORIGIN[0]; bound[1] = numBox[0] + ORIGIN[0];
	bound[2] = ORIGIN[1]; bound[3] = numBox[1] + ORIGIN[1];
	bound[4] = ORIGIN[2]; bound[5] = numBox[2] + ORIGIN[2];

	k = make_model_atom_pos();
	if (k == 0) {
		show_model_check->enable();
		Mbtn->enable();
	}
	else {
		show_model_check->disable();
		Mbtn->disable();
		show_model = 0;
		start_VICS = 0;
	}

	glNewList(13,GL_COMPILE);
	glEndList();
	glNewList(13,GL_COMPILE_AND_EXECUTE);
	if (range_ID == 1){
		for(i=0; i<nXYZ; i++){
			m[i].show = 'y';
			if ((m[i].x < range[0]) || (m[i].x > numBox[0]-1+range[1]) ||
				(m[i].y < range[2]) || (m[i].y > numBox[1]-1+range[3]) ||
				(m[i].z < range[4]) || (m[i].z > numBox[2]-1+range[5])) m[i].show = 'n';
		}
	}
	if (k == 0){
		make_orto_model_atom(NPIX);
		l = searchBonds(max_P);
		if (model_type == 0){
			for(i=0; i<nXYZ; i++){
				if (m[i].show == 'y'){
					I1 = m[i].Num;
					a_color[0] = lAtom[I1].color[0];
					a_color[1] = lAtom[I1].color[1];
					a_color[2] = lAtom[I1].color[2];
					R=ar_scale*lAtom[I1].R;
					x = m[i].xo / max_P;
					y = m[i].yo / max_P;
					z = m[i].zo / max_P;
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_color);
					glPushMatrix();
					glTranslatef(x,y,z);
					glutSolidSphere(R,atoms_stack,atoms_slice);
					glPopMatrix();
				}
			}
		}
		if (l == 0){
			for(i=0;i<nBonds;i++){
				quadObj = gluNewQuadric();
				gluQuadricDrawStyle(quadObj, GLU_FILL);
				I1 = bAtom[i].Atom1;
				I2 = bAtom[i].Atom2;
				dx = (m[I2].xo - m[I1].xo) / max_P;
				dy = (m[I2].yo - m[I1].yo) / max_P;
				if (dx==0 && dy==0) {dx = 1; dy = 0;}
				switch(bonds_type){
				case(0):{
					a_color[0] = BNCL[0];
					a_color[1] = BNCL[1];
					a_color[2] = BNCL[2];
					x = m[I1].xo / max_P;
					y = m[I1].yo / max_P;
					z = m[I1].zo / max_P;
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_color);
					glPushMatrix();
					glTranslatef(x,y,z);
					glRotated(bAtom[i].FI, -dy/bAtom[i].dist, dx/bAtom[i].dist, 0);
					gluCylinder(quadObj,bond_R, bond_R, bAtom[i].dist,bonds_slice,bonds_stack); 
					glPopMatrix();
					break;
						}
				case(1):{
					J1 = m[I1].Num;
					J2 = m[I2].Num;
					R1 = ar_scale*lAtom[J1].R;
					R2 = ar_scale*lAtom[J2].R;
					dist1 = R1 + (bAtom[i].dist - R1 - R2)/2;
					dist2 = bAtom[i].dist - dist1;
					x = m[I1].xo / max_P;
					y = m[I1].yo / max_P;
					z = m[I1].zo / max_P;
					glPushMatrix();
					glTranslatef(x,y,z);
					glRotated(bAtom[i].FI, -dy/bAtom[i].dist, dx/bAtom[i].dist, 0);
					a_color[0] = lAtom[J1].color[0];
					a_color[1] = lAtom[J1].color[1];
					a_color[2] = lAtom[J1].color[2];
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_color);
					gluCylinder(quadObj,bond_R, bond_R, dist1,bonds_slice,bonds_stack); 
					glTranslated( 0, 0, dist1);
					a_color[0] = lAtom[J2].color[0];
					a_color[1] = lAtom[J2].color[1];
					a_color[2] = lAtom[J2].color[2];
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, a_color);
					gluCylinder(quadObj,bond_R, bond_R, dist2,bonds_slice,bonds_stack); 
					glPopMatrix();
					break;
						}
				}
				gluDeleteQuadric(quadObj);
			}
		}
	}
	glEndList();
	glEnable(GL_LIGHTING);
	glLineWidth(1);
}

//**************************  create_Axis_List() ****************************//
void create_Axis_List()
{
	int i,j;
	float d[3], cf, alpha[3];
	float colors[3][4],diffuse[4];
	char Axis_label1[2],Axis_label2[2],Axis_label3[2];
	double bd;
	float label_color;

	colors[0][0] = 1;colors[0][1] = 0;colors[0][2] = 0;colors[0][3] = 1;
	colors[1][0] = 0;colors[1][1] =	1;colors[1][2] = 0;colors[1][3] = 1;
	colors[2][0] = 0;colors[2][1] = 0;colors[2][2] = 1;colors[2][3] = 1;

//	if((fileID == 5) || (fileID == 6) || (fileID == 8)){
	if((fileID == 1) || (fileID == 2) || (fileID == 6)){
		sprintf(Axis_label1,"x");
		sprintf(Axis_label2,"y");
		sprintf(Axis_label3,"z");
	}
	else {
		sprintf(Axis_label1,"a");
		sprintf(Axis_label2,"b");
		sprintf(Axis_label3,"c");
	}
	for(i=0; i<3; i++){
		d[i] = (float)sqrt(uAxis[i][0]*uAxis[i][0] + uAxis[i][1]*uAxis[i][1] + uAxis[i][2]*uAxis[i][2]);
		cf = uAxis[i][2]/d[i];
		alpha[i] = acos(cf)*180/PI;
	}

	bd = sqrt(BKCL[0]*BKCL[0] + BKCL[1]*BKCL[1] + BKCL[2]*BKCL[2]);
	if (bd < (sqrt(3)/2)) label_color = 1; else label_color = 0;

	glNewList(12,GL_COMPILE);
	if (show_axis > 0){
//-------------------------------------- Draw Axis Cone and Labels
		for(i=0; i<3; i++){
			glPushMatrix();
			glRotatef(-alpha[i],uAxis[i][1],-uAxis[i][0],0);
			glTranslatef(0,0,d[i]);
			for(j=0; j<4; j++) diffuse[j] = colors[i][j];
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
			glutSolidCone(0.02,0.04,11,1);
			if (show_axis == 1){
				glDisable( GL_LIGHTING );
				glColor3f(label_color,label_color,label_color);
				glTranslatef(0,0,0.06);
				glPushMatrix();
				glRasterPos2f(0.0,0.0);
				if (i == 0) draw_string_bitmap(GLUT_BITMAP_9_BY_15, Axis_label1);
				if (i == 1) draw_string_bitmap(GLUT_BITMAP_9_BY_15, Axis_label2);
				if (i == 2) draw_string_bitmap(GLUT_BITMAP_9_BY_15, Axis_label3);
				glPopMatrix();
				glEnable( GL_LIGHTING );
			}
			glPopMatrix();
		}
//-------------------------------------- Draw Central Ball
		glPushMatrix();
		diffuse[0] = 0.9;
		diffuse[1] = 0.9;
		diffuse[2] = 0.9;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glutSolidSphere(0.015,20,20);
//-------------------------------------- Draw Axis Line
		glDisable( GL_LIGHTING );
		glLineWidth(1.5);
		glBegin( GL_LINES );
		  glColor3f( 1.0, 0.0, 0.0 );
		  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( uAxis[0][0], uAxis[0][1], uAxis[0][2] ); /* X axis      */
		  glColor3f( 0.0, 1.0, 0.0 );
		  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( uAxis[1][0], uAxis[1][1], uAxis[1][2] ); /* X axis      */
		  glColor3f( 0.0, 0.0, 1.0 );
		  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( uAxis[2][0], uAxis[2][1], uAxis[2][2] ); /* X axis      */
		glEnd();
		glEnable( GL_LIGHTING );
		glPopMatrix();
	}
	glEndList();
	glEnable(GL_LIGHTING);
	glLineWidth(1);
}

//**************************  crate_plane_list()   ***************************//
void crate_plane_list(int ID)
{
	int i,j;
	int I1;
	float RGB[3];
	float x,y,z;

	glNewList(9+ID,GL_COMPILE_AND_EXECUTE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_LIGHTING);
	for (i=0; i<TOTAL_P[ID]; i++){
		glBegin(GL_POLYGON);
		for(j=0; j<plane[ID][i].nV; j++){
			I1 = plane[ID][i].sl.Index[j];
			switch(palette_hkl)
			{
			case(0):{
				RGB[0] = (float)Pal[I1][0] / 255;
				RGB[1] = (float)Pal[I1][1] / 255;
				RGB[2] = (float)Pal[I1][2] / 255;
				break;
					}
			case(1):{
				RGB[0] = 1 - (float)Pal[I1][0] / 255;
				RGB[1] = 1 - (float)Pal[I1][1] / 255;
				RGB[2] = 1 - (float)Pal[I1][2] / 255;
				break;
					}
			case(2):{
				RGB[0] = 1 - (float)Pal[I1][3] / 255;
				RGB[1] = 1 - (float)Pal[I1][4] / 255;
				RGB[2] = 1 - (float)Pal[I1][5] / 255;
				break;
					}
			}
			x = plane[ID][i].sl.p[j].x / max_P;
			y = plane[ID][i].sl.p[j].y / max_P;
			z = plane[ID][i].sl.p[j].z / max_P;
			glColor4f(RGB[0],RGB[1],RGB[2],opacity_hkl);
			glVertex3f(x,y,z);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEndList();
	glEnable(GL_LIGHTING);
	glLineWidth(1);
}

//**************************  crate_slice_list()   ***************************//
void crate_slice_list(int ID)
{

//--------------------------------//
//  ID = 0 --- (100) plane        //
//  ID = 1 --- (010) plane        //
//  ID = 2 --- (001) plane        //
//--------------------------------//
	int i,j;
	int I1;
	float RGB[3];
	float x,y,z;

	glNewList(3+ID,GL_COMPILE_AND_EXECUTE);
//	if (show_iso == 0){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);
		for (i=0; i<TOTAL_S[ID]; i++){
			glBegin(GL_POLYGON);
			for(j=0; j<slice[ID][i].nV; j++){
				I1 = slice[ID][i].sl.Index[j];
				RGB[0] = (float)Pal[I1][0] / 255;
				RGB[1] = (float)Pal[I1][1] / 255;
				RGB[2] = (float)Pal[I1][2] / 255;
				x = slice[ID][i].sl.p[j].x / max_P;
				y = slice[ID][i].sl.p[j].y / max_P;
				z = slice[ID][i].sl.p[j].z / max_P;
				glColor3f(RGB[0],RGB[1],RGB[2]);
				glVertex3f(x,y,z);
			}
			glEnd();
		}
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1);
//	}
	glEndList();
	glLineWidth(1);
	glEnable(GL_LIGHTING);
}


//**************************  crate_3D_list()   *****************************//
void crate_3D_list()
{
	int i;

	float shape1[4];
	float shape2[4];

	for(i=0;i<4; i++) shape1[i] = mat_diffuse[i];
	shape2[0] = 1 - mat_diffuse[0];
	shape2[1] = mat_diffuse[1];
	shape2[2] = 1 - mat_diffuse[2];
	shape2[3] = mat_diffuse[3];

	glNewList(2,GL_COMPILE);
	if (model_mode == 2) {
		glPointSize(1);
		glEnable(GL_POINT_SMOOTH);
	}
	switch(model_mode)
	{
	case(0):{glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);break;}
	case(1):{glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);break;}
	case(2):{glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);break;}
	}
	glLineWidth(0.1);
	glBegin(GL_TRIANGLES);
	switch(iso_pm_parm){
	case(0):{
		for (i=0; i<TOTAL_T; i++){
			if (((fileID == 0) && (den_file_ID == 1)) || (fileID == 1) || (fileID == 3) || (fileID == 2) || (fileID == 6) || (fileID == 5)) {
				if (sign[i] == '-') glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shape2);
				else glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shape1);
			}
			glNormal3f(ISO[3][i],ISO[4][i],ISO[5][i]);
			glVertex3f(ISO[0][i]/max_P,ISO[1][i]/max_P,ISO[2][i]/max_P);
		}
		break;
			}
	case(1):{
		for (i=0; i<TOTAL_T; i++){
			if (((fileID == 0) && (den_file_ID == 1)) || (fileID == 1)|| (fileID == 3) || (fileID == 2) || (fileID == 6) || (fileID == 5)) {
				if (sign[i] != '-') {
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shape1);
					glNormal3f(ISO[3][i],ISO[4][i],ISO[5][i]);
					glVertex3f(ISO[0][i]/max_P,ISO[1][i]/max_P,ISO[2][i]/max_P);
				}
			}
			else {
				glNormal3f(ISO[3][i],ISO[4][i],ISO[5][i]);
				glVertex3f(ISO[0][i]/max_P,ISO[1][i]/max_P,ISO[2][i]/max_P);
			}
		}
		break;
			}
	case(2):{
		for (i=0; i<TOTAL_T; i++){
			if (sign[i] == '-') {
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, shape2);
				glNormal3f(ISO[3][i],ISO[4][i],ISO[5][i]);
				glVertex3f(ISO[0][i]/max_P,ISO[1][i]/max_P,ISO[2][i]/max_P);
			}
		}
		break;
			}
	}
	glEnd();
	glEndList();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (model_mode == 2) glDisable(GL_POINT_SMOOTH);
	glLineWidth(1);
	glEnable(GL_LIGHTING);
}

//****************************************************************//
//                         draw3DCell()                           //
//****************************************************************//
void crate_cell_list()
{
	int i;
	glDisable(GL_LIGHTING);
	glNewList(1,GL_COMPILE);
	if (unit_cell_show == 1){
		glDisable(GL_LIGHTING);
		glColor3f(unit_cell_color[0],unit_cell_color[1],unit_cell_color[2]);
		glLineWidth(unit_cell_width);
		switch(unit_cell_type)
		{
		case(1):{  //--- Dotted line
			glEnable (GL_LINE_STIPPLE);
			glLineStipple (1, 0x0101);
			break;
				}
		case(2):{  //--- Dashed line
			glEnable (GL_LINE_STIPPLE);
			glLineStipple (1, 0x00FF);
			break;
				}
		}
		glBegin( GL_LINE_LOOP );
		for(i=0; i<8; i++) {glVertex3f(uCello[i][0]/max_P,uCello[i][1]/max_P,uCello[i][2]/max_P);}
		glEnd();
		glBegin( GL_LINES );
		glVertex3f(uCello[0][0]/max_P,uCello[0][1]/max_P,uCello[0][2]/max_P);
		glVertex3f(uCello[3][0]/max_P,uCello[3][1]/max_P,uCello[3][2]/max_P);
		glVertex3f(uCello[1][0]/max_P,uCello[1][1]/max_P,uCello[1][2]/max_P);
		glVertex3f(uCello[6][0]/max_P,uCello[6][1]/max_P,uCello[6][2]/max_P);
		glVertex3f(uCello[2][0]/max_P,uCello[2][1]/max_P,uCello[2][2]/max_P);
		glVertex3f(uCello[5][0]/max_P,uCello[5][1]/max_P,uCello[5][2]/max_P);
		glVertex3f(uCello[4][0]/max_P,uCello[4][1]/max_P,uCello[4][2]/max_P);
		glVertex3f(uCello[7][0]/max_P,uCello[7][1]/max_P,uCello[7][2]/max_P);
		glEnd();
		glEnable(GL_LIGHTING);
		if (unit_cell_type > 0) glDisable (GL_LINE_STIPPLE);
	}
	glEndList();
	glEnable(GL_LIGHTING);
	glLineWidth(0.5);
}

//**************************  clear_display_list()   **************************//
void clear_display_list()
{
	int i;
	glNewList(1,GL_COMPILE);
	glEndList();

	glNewList(2,GL_COMPILE);
	glEndList();

	for(i=0; i<6; i++) {
		glNewList(i+3,GL_COMPILE);
		glEndList();
	}
	for(i=0; i<3; i++) {
		glNewList(9+i,GL_COMPILE);
		glEndList();
	}
	glNewList(12,GL_COMPILE);
	glEndList();

}


//**************************  set_display_list()   **************************//
void set_display_list()
{
	int i;

	if (TOTAL_T > 0){
		create_Axis_List();
		crate_cell_list();
		crate_3D_list();
		for(i=0; i<6; i++) crate_slice_list(i);
	}
}


//**************************  load_data()   *********************************//
void load_data(int ID)
{
	int i;
	int x1,y1,z1,x2,y2,z2;

	clear_display_list();
	set_orthoMatrix();
	set_Ortho_ABC();
	TOTAL_T = 0;
	for(i=0; i<6; i++) TOTAL_S[i] = 0;
	for(i=0; i<3; i++) TOTAL_P[i] = 0;
	x1 = (int)(range[0]*NPIX[0]);
	y1 = (int)(range[2]*NPIX[1]);
	z1 = (int)(range[4]*NPIX[2]);
	x2 = (int)(range[1]*NPIX[0]);
	y2 = (int)(range[3]*NPIX[1]);
	z2 = (int)(range[5]*NPIX[2]);
	TOTAL_T = shape(cCell,DELC,ORIGIN,(float)fabs(OBJLEV),NPIX,x1,y1,z1,x2,y2,z2,DELC_ID);
	if (TOTAL_T > 0){
		if (fileID != 6) {
			for(i=0; i<6; i++) {
				TOTAL_S[i] = section(i,DELC,ORIGIN,(float)fabs(OBJLEV),SECLEV,NPIX,x1,y1,z1,x2,y2,z2,HMIN,HMAX,iso_pm_parm);
			}
		}
		set_Ortho_Cell(0,0,0,NPIX[0],NPIX[1],NPIX[2]);
		set_Ortho_Translations(NPIX[0],NPIX[1],NPIX[2]);
		clear_display_list();
		set_display_list();
		origin[0] = -(float)numBox[0]*ortho_cCell[0]/(2*max_P);
		origin[1] = -(float)numBox[1]*ortho_cCell[1]/(2*max_P);
		origin[2] = -(float)numBox[2]*ortho_cCell[2]/(2*max_P);
	}
}

//**************************  set_light_prop()   ****************************//
void set_light_prop()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glMultMatrixf(l_direction);
	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l_dir);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

//**************************  set_material_prop()   *************************//
void set_material_prop()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

//**************************  set_material_prop()   *************************//
void set_atom_prop()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, atom_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, atom_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, atom_emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, atom_shininess);
}

/***************************************** VEND_Rotation() ******************/
void VEND_Rotation(int x, int y)
{
	tbMotion(x,y,rotation_mode);
}

/***************************************** VEND_Magnitude() *****************/
void VEND_Magnitude(int x, int y)
{
	float s;

	s = (float)(y - last_y);
	switch(projection_type)
	{
//------------------------------------------------ Perspective projection
	case(0):{obj_pos[2] +=0.01*s;break;}
//------------------------------------------------ Parallel projection
	case(1):{scale +=0.001*(-s);if (scale < 0) scale = 0;break;}
	}

	last_y = y;
	glutPostRedisplay(); 
}

/***************************************** VEND_Translate() *****************/
void VEND_Translate(int x, int y)
{
	float x1,y1;
	x1 = (float)(x - last_x);
	y1 = (float)(last_y - y);
    obj_pos[0] +=0.005*x1;
    obj_pos[1] +=0.005*y1;
	last_x = x;
	last_y = y;
	glutPostRedisplay(); 
}

/***************************************** VEND_Mouse_Motion() **************/
void VEND_Mouse_Motion(int x, int y )
{
	if ((tools_mode == 0) && (rotation_type == 2)) return;
	if (motion == 1){
		switch(tools_mode)
		{
		case(0):{VEND_Rotation(x,y); break;}
		case(1):{VEND_Magnitude(x,y); break;}
		case(2):{VEND_Translate(x,y); break;}
		}
	}
}

/**************************************** VEND_Idle() ***********************/
void VEND_Idle( void )
{
	int i,j,k,file_control,preview;
	float RGB[3];

	if ( glutGetWindow() != main_window ) glutSetWindow(main_window);  

//------------------------------------------------------------------------- 
	if (run_img_export == 2){
		switch(img_exp_from){
		case(1):{
			glutSetWindow(window_2D_ID);
			D2D_reshape(400,400);
			break;
				}
		case(2):{
			glutSetWindow(window_3D_ID);
			D3D_reshape(400,400);
			break;
				}
		}
		run_img_export = 1;
	}
//------------------------------------------------------------------------- Export Image Msg Dialog_1
	if (chk_image_ID == -2){
		chk_image_ID = -1;
		Msg_Export_Dialog(main_window,img_exp_from);
	}
//------------------------------------------------------------------------- Export Image Msg Dialog
	if (chk_image_ID == -1){
		i = get_msg_export_dialog_id();
		if (i == 1) chk_image_ID = 0;
	}
//------------------------------------------------------------------------- Color Palette Dialog
	if (color_dialog_ID != 0){
		i = get_palette_ID();
		if (i == 0){
			switch(color_dialog_ID)
			{
//-----------------------------------------------------  Background colors
			case(101):{
				GetPaletteRGB(RGB);
				set_genral_colors(RGB);
				get_background_rendering(BKCL,BKCL2D,BKCL3D);
				create_Axis_List();
				if (window_2D_ID != -1){
					glutSetWindow(window_2D_ID);
					glutPostRedisplay();
					glutSetWindow(main_window);
				}
				if (window_3D_ID != -1){
					glutSetWindow(window_3D_ID);
					glutPostRedisplay();
					glutSetWindow(main_window);
				}
				break;
			}
//-----------------------------------------------------  Isosurface colors
			case(102):{
				GetPaletteRGB(RGB);
				set_isosurface_colors(RGB);
				get_isosurface_material_param(mat_ambient,mat_diffuse,mat_specular,mat_emission,mat_shininess);
				break;
					  }
//-----------------------------------------------------  Unit Cell colors
			case(103):{
				GetPaletteRGB(unit_cell_color);
				set_ucell_colors(unit_cell_color);
				crate_cell_list();
				break;
					  }
//-----------------------------------------------------  Model Atoms colors
			case(104):{
				GetPaletteRGB(RGB);
				set_model_atom_colors(RGB);
				break;
					  }
//-----------------------------------------------------  Model Bonds colors
			case(105):{
				GetPaletteRGB(RGB);
				set_model_bond_colors(RGB);
				break;
					  }
//-----------------------------------------------------  Contour map colors
			case(106):{
				GetPaletteRGB(RGB);
				set_contour_colors(RGB);
				break;
					  }
//-----------------------------------------------------  Projections outline colors
			case(107):{
				GetPaletteRGB(RGB);
				set_prj_colors(RGB);
				break;
					  }
			}
			color_dialog_ID = 0;
		}
	}
//------------------------------------------------------------------------- Message Dialog 1
	if (message_dialog_ID1 != 0){
		i = get_message_dialog_id1();
		if (i == 1){
			message_dialog_ID1 = 0;
		}
	}
//------------------------------------------------------------------------- About Dialog
	if (about_dialog_ID != 0){
		i = get_about_dialog_id();
		if (i == 1) {
			get_about_dialog_pos(dwp[0][0], dwp[1][0]);
			about_dialog_ID = 0;
			open_btn->enable();
			save_btn->enable();
			exp_btn->enable();
			info_btn->enable();
			proj_btn->enable();
			atom_btn->enable();
			pref_btn->enable();
			about_btn->enable();
			exit_btn->enable();
		}
	}
//------------------------------------------------------------------------- Read File Name from ARG
	if(arg_openfile_ID != 0) {
		arg_openfile_ID = 0;
//		fileID = 0;
		sprintf(FileName,"");
		sprintf(FileName,"%s",arg_fname);
		file_control =  read_data_from_file(fileID, FileName);
		if (window_2D_ID != -1) {close2Dwindow(window_2D_ID);window_2D_ID = -1;}
		if (window_3D_ID != -1) {close3Dwindow(window_3D_ID);window_3D_ID = -1;}
		if (file_control >= 0) {
			_update_window_title();
			initTrackBall();
			glutSetWindow(main_window);
			print_out_info();
			set_initital_model_parameters();
			sprintf(mod_fName,"");
			mod_format = 0;
			model_radio->set_int_val(model_mode);
			show_iso_check->set_int_val(show_iso);
			show_sls_check->set_int_val(show_sls);
			show_model = 0;
			show_model_check->set_int_val(show_model);
			show_model_check->disable();
			Mbtn->disable();
			set_light_prop();
			set_material_prop();
			load_data(file_control);
			start_VEND = 1;
		}
	}
//------------------------------------------------------------------------- Open File Dialog
	if(openfile_dialog_ID != 0) {
		i = get_file_dialog_id();
		switch(i){
		case(0):{
			openfile_dialog_ID = 0;
			break;
				}
		case(1):{
			openfile_dialog_ID = 0;
			get_file_info(fileID, FileName, OBJLEV);
			glutPostRedisplay();
			file_control =  read_data_from_file(fileID, FileName);
			rotation_type = 0;
			rotation_tipe_radio->set_int_val(rotation_type);
			updateStatusBar();
			if (window_2D_ID != -1) {close2Dwindow(window_2D_ID);window_2D_ID = -1;}
			if (window_3D_ID != -1) {close3Dwindow(window_3D_ID);window_3D_ID = -1;}
			if (file_control >= 0) {
				_update_window_title();
				initTrackBall();
				glutSetWindow(main_window);
				print_out_info();
				set_initital_model_parameters();
				sprintf(mod_fName,"");
				mod_format = 0;
				model_radio->set_int_val(model_mode);
				show_iso_check->set_int_val(show_iso);
				show_sls_check->set_int_val(show_sls);
				show_model = 0;
				show_model_check->set_int_val(show_model);
				show_model_check->disable();
				Mbtn->disable();
				set_light_prop();
				set_material_prop();
				load_data(file_control);
				start_VEND = 1;

			}
			break;
				}
		case(2):{
			set_file_dialog_id(-1);
			get_file_info(fileID, FileName, OBJLEV);
			message_dialog_ID1 = -1;
			open_message_dialog1(main_window,0,FileName);
			break;
				}
		}
	}
//------------------------------------------------------------------------- General control
	if(genral_Dialog_ID != 0) {
		i = get_genral_control_id();
		if (i == 1) genral_Dialog_ID = 0;
		switch(i)
		{
		case(1):{
			genral_Dialog_ID = 0;
			get_general_dialog_pos(dwp[0][2], dwp[1][2]);
			glutSetWindow(main_window);
			break;
				}
		case(10):{
			set_genral_control_id(-1);
			get_lights_rendering(l_ambient,l_diffuse,l_direction);
			set_light_prop();
			break;
				 }
		case(20):{
			set_genral_control_id(-1);
			get_background_rendering(BKCL,BKCL2D,BKCL3D);
			create_Axis_List();
			if (window_2D_ID != -1){
				glutSetWindow(window_2D_ID);
				glutPostRedisplay();
				glutSetWindow(main_window);
			}
			if (window_3D_ID != -1){
				glutSetWindow(window_3D_ID);
				glutPostRedisplay();
				glutSetWindow(main_window);
			}
			break;
				 }
		case(40):{
			set_genral_control_id(-1);
			show_axis = get_compas_setting();
			create_Axis_List();
			break;
				 }
		case(50):{
			get_genral_colors(RGB);
			set_genral_control_id(-1);
			color_dialog_ID = 101;
			InitPaletteDialog(RGB);
			get_general_dialog_pos(dwp[0][2], dwp[1][2]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][2],dwp[1][2]);
			break;
				 }
		case(60):{
			get_projection_perspective(projection_type, perspective);
			set_genral_control_id(-1);
			break;
				 }

		}
	}
//------------------------------------------------------------------------- Isosurface control
	if(isosurface_Dialog_ID != 0) {
		i = get_isosurface_dialog_id();
		preview = get_isosurface_preview_id();
		switch(i)
		{
		case(0):{
			isosurface_Dialog_ID = 0;
			get_iso_dialog_pos(dwp[0][4], dwp[1][4]);
			glutSetWindow(main_window);
			break;
				}
		case(1):{
			isosurface_Dialog_ID = 0;
			get_iso_dialog_pos(dwp[0][4], dwp[1][4]);
			get_isosurface_plot_range(ORIGIN,range_ID,numBox,range,OBJLEV,SECLEV);
			get_isosurface_material_param(mat_ambient,mat_diffuse,mat_specular,mat_emission,mat_shininess);
			iso_pm_parm = get_isosurface_pm_type();
			origin[0] = -(float)numBox[0]*ortho_cCell[0]/(2*max_P);
			origin[1] = -(float)numBox[1]*ortho_cCell[1]/(2*max_P);
			origin[2] = -(float)numBox[2]*ortho_cCell[2]/(2*max_P);
			load_data(1);
			if (start_VICS == 1) create_Atom_list();
			break;
				}
		case(10):{
			get_isosurface_plot_range(ORIGIN,range_ID,numBox,range,OBJLEV,SECLEV);
			iso_pm_parm = get_isosurface_pm_type();
			set_isosurface_dialog_id(-1);
			load_data(1);
			if (start_VICS == 1) create_Atom_list();
			break;
				 }
		case(61):{
			get_isosurface_plot_range(ORIGIN,range_ID,numBox,range,OBJLEV,SECLEV);
			set_isosurface_dialog_id(-1);
			origin[0] = -(float)numBox[0]*ortho_cCell[0]/(2*max_P);
			origin[1] = -(float)numBox[1]*ortho_cCell[1]/(2*max_P);
			origin[2] = -(float)numBox[2]*ortho_cCell[2]/(2*max_P);
//			load_data(1);
			if (start_VICS == 1) create_Atom_list();
			break;
				 }
		case(100):{
			if (preview == 1) {
				get_isosurface_material_param(mat_ambient,mat_diffuse,mat_specular,mat_emission,mat_shininess);
				set_isosurface_dialog_id(-1);
			}
			break;
				  }
		case(200):{
			get_isosurface_colors(RGB);
			set_isosurface_dialog_id(-1);
			color_dialog_ID = 102;
			InitPaletteDialog(RGB);
			get_iso_dialog_pos(dwp[0][4], dwp[1][4]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][4],dwp[1][4]);
			break;
				  }
		}
	}
//------------------------------------------------------------------------- Slices control
	if(sli_Dialog_ID != 0) {
		i = get_sli_dialog_id();
		if (i == 1) sli_Dialog_ID = 0;
		switch(i){
		case(1):{
			sli_Dialog_ID = 0;
			get_sli_dialog_pos(dwp[0][9], dwp[1][9]);
			glutSetWindow(main_window);
			hkl_mode = get_sli_mode_id();
			get_sli_param(num_of_hkl,position_hkl,index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
			get_sli_show_param(index_show_hkl);
			break;
				}
		case(10):{
			hkl_mode = get_sli_mode_id();
			if (hkl_mode == 0){
				get_sli_cur_param(c_position_hkl,c_index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
				j = 3;
				if ((c_index_hkl[0] != 0) && (c_index_hkl[1] == 0) && (c_index_hkl[2] == 0)) j = 0;
				if ((c_index_hkl[0] == 0) && (c_index_hkl[1] != 0) && (c_index_hkl[2] == 0)) j = 1;
				if ((c_index_hkl[0] == 0) && (c_index_hkl[1] == 0) && (c_index_hkl[2] != 0)) j = 2;
				index_c_type = j;
				switch(j){
				case(0):{
					TOTAL_P[0] = 0;
					TOTAL_P[0] = make_shortcut_plane(0,0,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,c_position_hkl,
						                             NPIX,HMIN,HMAX);
					crate_plane_list(0);
					break;
						}
				case(1):{
					TOTAL_P[0] = 0;
					TOTAL_P[0] = make_shortcut_plane(0,1,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,c_position_hkl,
						                             NPIX,HMIN,HMAX);
					crate_plane_list(0);
					break;
						}
				case(2):{
					TOTAL_P[0] = 0;
					TOTAL_P[0] = make_shortcut_plane(0,2,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,c_position_hkl,
						                             NPIX,HMIN,HMAX);
					crate_plane_list(0);
					break;
					}
				case(3):{
					TOTAL_P[0] = 0;
					TOTAL_P[0] = make_plane(0,c_index_hkl,DELC, objlev_scale*OBJLEV, seclev_scale*SECLEV,
						                    c_position_hkl, NPIX,HMIN,HMAX,cCell);
					crate_plane_list(0);
					break;
						}
				}
			}
			set_sli_dialog_id(-1);
			break;
				 }
		case(12):{
			hkl_mode = get_sli_mode_id();
			get_sli_param(num_of_hkl,position_hkl,index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
			get_sli_show_param(index_show_hkl);
			for(i=0; i<num_of_hkl; i++){
				TOTAL_P[i] = 0;
				j = 0;
				if ((index_hkl[i][0] != 0) && (index_hkl[i][1] == 0) && (index_hkl[i][2] == 0)) j = 1;
				if ((index_hkl[i][0] == 0) && (index_hkl[i][1] != 0) && (index_hkl[i][2] == 0)) j = 2;
				if ((index_hkl[i][0] == 0) && (index_hkl[i][1] == 0) && (index_hkl[i][2] != 0)) j = 3;
				index_type[i] = j;
				if (index_show_hkl[i] == 1) {
					switch(j)
					{
					case(1):{
						TOTAL_P[i] = make_shortcut_plane(i,0,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,
							                             position_hkl[i],NPIX,HMIN,HMAX);
						break;
							} 
					case(2):{
						TOTAL_P[i] = make_shortcut_plane(i,1,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,
							                                 position_hkl[i],NPIX,HMIN,HMAX);
						break;
							}
					case(3):{
						TOTAL_P[i] = make_shortcut_plane(i,2,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,
							                             position_hkl[i],NPIX,HMIN,HMAX);
						break;
							}
					case(0):{
						c_index_hkl[0] = index_hkl[i][0];
						c_index_hkl[1] = index_hkl[i][1];
						c_index_hkl[2] = index_hkl[i][2];
						c_position_hkl = position_hkl[i];
						TOTAL_P[i] = make_plane(i,c_index_hkl,DELC, objlev_scale*OBJLEV, seclev_scale*SECLEV,
						                        c_position_hkl, NPIX,HMIN,HMAX,cCell);
						break;
							}
					}
				}
				crate_plane_list(i);
			}
			set_sli_dialog_id(-1);
			break;
				 }
		case(13):{
			for(i=0; i<3; i++){
				TOTAL_P[i] = 0;
				crate_plane_list(i);
			}
			set_sli_dialog_id(-1);
			break;
				 }
		case(22):
		case(21):
		case(20):{
			hkl_mode = get_sli_mode_id();
			get_sli_cur_param(c_position_hkl,c_index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
			j = i - 20;
			index_c_type = j;
			TOTAL_P[0] = 0;
			TOTAL_P[0] = make_shortcut_plane(0,i-20,DELC,ORIGIN,objlev_scale*OBJLEV,seclev_scale*SECLEV,c_position_hkl,
				                             NPIX,HMIN,HMAX);
			crate_plane_list(0);
			set_sli_dialog_id(-1);
			break;
				 }
		case(30):{
			preview = get_sli_preview_id();
			if (preview == 1){
				hkl_mode = get_sli_mode_id();
				if (hkl_mode == 0){
					get_sli_cur_param(c_position_hkl,c_index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
					crate_plane_list(0);
				}
				else {
					get_sli_param(num_of_hkl,position_hkl,index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
					for(i=0; i<num_of_hkl; i++) crate_plane_list(i);

				}
			}
			set_sli_dialog_id(-1);
			break;
				 }
		case(40):{
			preview = get_sli_preview_id();
			if (preview == 1){
				hkl_mode = get_sli_mode_id();
				if (hkl_mode == 0){
					get_sli_cur_param(c_position_hkl,c_index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
					crate_plane_list(0);
				}
				else {
					get_sli_param(num_of_hkl,position_hkl,index_hkl,opacity_hkl,palette_hkl,objlev_scale,seclev_scale);
					for(i=0; i<num_of_hkl; i++) crate_plane_list(i);

				}
			}
			set_sli_dialog_id(-1);
			break;
				 }
		}
	}
//------------------------------------------------------------------------- Unit Cell Outline control
	if(unitcell_Dialog_ID != 0) {
		i = get_ucell_prop_dialog_id();
		switch(i)
		{
		case(1):{
			unitcell_Dialog_ID = 0; 
			get_ucell_dialog_pos(dwp[0][11], dwp[1][11]);
			glutSetWindow(main_window);
			break;
				}
		case(500):{
			get_ucell_prop(unit_cell_show,unit_cell_width,unit_cell_color[0],unit_cell_color[1],unit_cell_color[2],
				           unit_cell_type);
			crate_cell_list();
			set_ucell_prop_dialog_id(-1);
			break;
				  }
		case(600):{
			get_ucell_colors(unit_cell_color);
			set_ucell_prop_dialog_id(-1);
			color_dialog_ID = 103;
			InitPaletteDialog(unit_cell_color);
			get_ucell_dialog_pos(dwp[0][11], dwp[1][11]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][11],dwp[1][11]);
			break;
				  }
		}
	}
//------------------------------------------------------------------------- Model properties control
	if(model_proper_dialog_ID != 0) {
		i = get_model_proper_dialog_id();
		switch(i)
		{
		case(0):{
			model_proper_dialog_ID = 0; 
			get_model_dialog_pos(dwp[0][5],dwp[1][5]);
			glutSetWindow(main_window);
			break;
				}
		case(1):{
			glutSetWindow(main_window);
			model_proper_dialog_ID = 0;
			get_model_prop_params(model_type, atoms_stack,atoms_slice, bonds_stack,bonds_slice,  
						   BNCL,bRadii[0],bRadii[1],bonds_type);
			for(i=0; i<tAtom; i++) get_model_atom_params(i,AtomT[i].color,AtomT[i].R);
			get_atom_R_and_colors();
			create_Atom_list();
			get_model_dialog_pos(dwp[0][5],dwp[1][5]);
			break;
				}
		case(15):{
			set_model_proper_dialog_id(-1);
			get_model_atom_colors(RGB);
			color_dialog_ID = 104;
			InitPaletteDialog(RGB);
			get_model_dialog_pos(dwp[0][5],dwp[1][5]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][5],dwp[1][5]);
			break;
				 }
		case(20):{
			set_model_proper_dialog_id(-1);
			get_model_bond_colors(RGB);
			color_dialog_ID = 105;
			InitPaletteDialog(RGB);
			get_model_dialog_pos(dwp[0][5],dwp[1][5]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][5],dwp[1][5]);
			break;
				 }
		}
	}
//------------------------------------------------------------------------- Info control
	if(info_Dialog_ID != 0) {
		i = get_info_dialog_id();
		if (i == 1) {
			info_Dialog_ID = 0;
			get_info_dialog_pos(dwp[0][3], dwp[1][3]);
			glutSetWindow(main_window);
		}
	}
//------------------------------------------------------------------------- Preferency control
	if(pref_Dialog_ID != 0) {
		i = get_preferences_dialog_id();
		switch(i)
		{
		case(1):{
			pref_Dialog_ID = 0;
			get_pref_dialog_pos(dwp[0][7], dwp[1][7]);
			glutSetWindow(main_window);
			preferency_get_cur_param(BKCL,BKCL2D,BKCL3D,unit_cell_color,unit_cell_width,
				                     show_axis,stepw_angle,def_file_format_ID,save_pos_ID,save_dpos_ID);
			save_pos_ID = 0;
			save_dpos_ID = 0;
			set_display_list();
			break;
				}
		case(3):{
			set_preferences_dialog_id(-1);
			preferency_get_def_param(def_BKCL,def_BKCL2D,def_BKCL3D,def_unit_cell_color,def_unit_cell_width,
				                     def_show_axis,stepw_angle,def_file_format_ID,save_pos_ID,save_dpos_ID);
			preferency_get_cur_param(BKCL,BKCL2D,BKCL3D,unit_cell_color,unit_cell_width,
				                     show_axis,stepw_angle,def_file_format_ID,save_pos_ID,save_dpos_ID);
			if (save_pos_ID == 1){
					k = glutGetWindow();
					glutSetWindow(main_window);
					graph_w = glutGet(GLUT_WINDOW_WIDTH); graph_h = glutGet(GLUT_WINDOW_HEIGHT);
					graph_x = glutGet(GLUT_WINDOW_X); graph_y = glutGet(GLUT_WINDOW_Y);
					j = menu_glui->get_glut_window_id();
					glutSetWindow(j);
					menu_x = glutGet(GLUT_WINDOW_X); menu_y = glutGet(GLUT_WINDOW_Y);
					j = glui->get_glut_window_id();
					glutSetWindow(j);
					tools_x = glutGet(GLUT_WINDOW_X); tools_y = glutGet(GLUT_WINDOW_Y);
					glutSetWindow(k);
			}
			if (save_dpos_ID == 1) {
				for(j=0; j<12; j++) {cdwp[0][j] = dwp[0][j];cdwp[1][j] = dwp[1][j];}
			}
			save_preferency();
			break;
				}
		}
	}
//------------------------------------------------------------------------- Image Control
	if(image_Dialog_ID != 0) {
		i = get_graph_dialog_id();
		if (i == 1) {image_Dialog_ID = 0;run_img_export = 0;}
		if (i == 10){
			image_Dialog_ID = 0;
			set_graph_dialog_id(-1);
			get_export_param(img_smoothing,img_scale,img_exp_from);
			switch(img_exp_from)
			{
			case(0):{run_img_export = 1;break;}
			case(1):{
				glutSetWindow(window_2D_ID);
				D2D_reshape(400,400);
				run_img_export = 2;
				break;
					}
			case(2):{
				glutSetWindow(window_3D_ID);
				D3D_reshape(400,400);
				run_img_export = 2;
				break;
					}
			}
		}
	}
//------------------------------------------------------------------------- 2D-Projection control_1
	if (cont_Dialog_ID != 0){
		i = get_contour_dialog_id();
		if (i == 1){
			cont_Dialog_ID = 0;
			get_contour_dialog_pos(dwp[0][1], dwp[1][1]);
			glutSetWindow(main_window);
			get_contour_dialog(cont_stype,conMAX,conMIN,cont_NSTP,cont_step, 
				               cont_RGB, cont_width, cont_show_only);
			set_prj_dialog_id(-2);
		}
		if (i == 100){
			get_contour_colors(RGB);
			set_contour_dialog_id(-1);
			color_dialog_ID = 106;
			InitPaletteDialog(RGB);
			get_contour_dialog_pos(dwp[0][1], dwp[1][1]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][1], dwp[1][1]);
		}
	}
//------------------------------------------------------------------------- 2D-Projection control
	if(prj_Dialog_ID != 0) {
		i = get_prj_dialog_id();
		switch(i){
		case(1):{
			prj_Dialog_ID = 0;
			get_prj_dialog_pos(dwp[0][8], dwp[1][8]);
			glutSetWindow(main_window);
			get_prj_dialog_settings(prj_plane,prj_cell,prj_pal,prj_cont_show,prj_show_2D,prj_show_3D,
				                    prj_RGB, prj_width, prj_hkl,prj_grid);
			get_prj_dialog_param(prj_project,prj_seclev_scale,prj_3Dlev_scale,prj_org);
			break;
				}
		case(10):{
			conMAX[0] = conMAX[1] = conMIN[0] = conMIN[1] = -1;
			set_prj_dialog_id(-1);
			break;
				 }
		case(20):{
			set_prj_dialog_id(-1);
			get_prj_dialog_settings(prj_plane,prj_cell,prj_pal,prj_cont_show,prj_show_2D,prj_show_3D,
				                    prj_RGB, prj_width,prj_hkl,prj_grid);
			get_prj_dialog_param(prj_project,prj_seclev_scale,prj_3Dlev_scale,prj_org);
			if (prj_cont_show == 0) cont_show_only = 0;
			switch(prj_show_2D)
			{
			case(0):{
				if (window_2D_ID != -1) close2Dwindow(window_2D_ID);
				window_2D_ID = -1;
				break;
					}
			case(1):{
				if (window_2D_ID == -1) {
					window_2D_ID = open2Dwindow(main_window);
					set_palette(Pal);
				}
				glutSetWindow(window_2D_ID);
				clear_color_bar();
				clear_map_cell();
				clear_contor_list();
				clear_grid_cell();
				if (prj_plane != 3) map_H00_Cell(prj_plane, prj_project);
				else map_HKL_Cell(prj_hkl,prj_org);
				if (cont_show_only == 0) {
					draw_H00_Cell(prj_plane,prj_cell,prj_RGB,prj_width);
					D2D_fill_H00_Cell(prj_plane, prj_cell, prj_pal, prj_seclev_scale);
					D2D_Color_Bar(prj_pal,prj_seclev_scale,prj_RGB,prj_width);
				}
				if (prj_cont_show == 1){
					map_H00_Contour(prj_plane,prj_cell,cont_stype,conMAX,conMIN,cont_NSTP,cont_step,
                                   cont_RGB,cont_width);
					}
				if (prj_grid == 1) D2D_fill_Grid(prj_plane, prj_cell, prj_RGB);
				glutPostRedisplay();
				break;
					}
			}
			switch(prj_show_3D)
			{
			case(0):{
				if (window_3D_ID != -1) close3Dwindow(window_3D_ID);
				window_3D_ID = -1;
				break;
					}
			case(1):{
				if (window_3D_ID == -1) {
					window_3D_ID = open3Dwindow(main_window);
					set_3Dpalette(Pal);
				}
				glutSetWindow(window_3D_ID);
				if (prj_plane != 3) map_H00_3D(prj_plane, prj_project);
				else map_HKL_3D(prj_hkl,prj_org);
				D3D_fill_H00_Cell(prj_plane, prj_pal, prj_3Dlev_scale,prj_seclev_scale,prj_RGB, prj_width);
				glutPostRedisplay();
				break;
					}
			}
			break;
				 }
		case(30):{
			set_prj_dialog_id(-1);
			get_prj_dialog_settings(prj_plane,prj_cell,prj_pal,prj_cont_show,prj_show_2D,prj_show_3D,
				                    prj_RGB, prj_width, prj_hkl,prj_grid);
			map_H00_Cell(prj_plane, prj_project);
			if ((conMAX[0] == -1) && (conMAX[1] == -1) && (conMIN[0] == -1) && (conMIN[1] == -1)) {
				conMAX[0] = get_MAX_den();
				conMIN[0] = get_MIN_den();
				conMAX[1] = (get_MIN_den() > 0.01) ? get_MIN_den() : 0.01;
				conMIN[1] = 3.0;
				cont_NSTP[0] = 10;
				cont_NSTP[1] = (int)(1 + ( log(get_MAX_den()) - log(conMAX[1]) ) / log(conMIN[1])) ;
				cont_step = (conMAX[0] - conMIN[0])/(float)cont_NSTP[0];
			}
			set_contour_dialog(cont_stype,conMAX,conMIN,cont_NSTP,cont_step,
				  			   cont_RGB,cont_width,cont_show_only);
			cont_Dialog_ID = -1;
			open_contour_dialog(main_window,dwp[0][1],dwp[1][1]);
			break;
				 }
		case(31):{
			set_prj_dialog_id(-1);
			get_prj_dialog_settings(prj_plane,prj_cell,prj_pal,prj_cont_show,prj_show_2D,prj_show_3D,
				                    prj_RGB, prj_width, prj_hkl,prj_grid);
			if (prj_plane != 3) map_H00_Cell(prj_plane, prj_project);
			else map_HKL_Cell(prj_hkl,prj_org);
			if ((conMAX[0] == -1) && (conMAX[1] == -1) && (conMIN[0] == -1) && (conMIN[1] == -1)) {
				conMAX[0] = get_MAX_den();
				conMIN[0] = get_MIN_den();
				conMAX[1] = (get_MIN_den() > 0.01) ? get_MIN_den() : 0.01;
				conMIN[1] = 3.0;
				cont_NSTP[0] = 10;
				cont_NSTP[1] = (int)(1 + ( log(get_MAX_den()) - log(conMAX[1]) ) / log(conMIN[1])) ;
				cont_step = (conMAX[0] - conMIN[0])/(float)cont_NSTP[0];
			}
			break;
				 }
		case(100):{
			get_prj_colors(RGB);
			set_prj_dialog_id(-1);
			color_dialog_ID = 107;
			InitPaletteDialog(RGB);
			get_prj_dialog_pos(dwp[0][8], dwp[1][8]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][8], dwp[1][8]);
			break;
				  }
		}
	}
//------------------------------------------------------------------------- Model Structure control
	if(model_Dialog_ID != 0) {
		model_Dialog_ID = 0;
		sprintf(mod_fName,"");
		mod_format = 0;
		i = open_file_dialog(mod_format, mod_fName, 1);
		glutSetWindow(main_window);
		if (i > 0){
			j = readModelfile(mod_fName);
			switch(j){
			case(0):{
				if (fabs(mcCell.a[0]-cCell[0]) > 0.01) j = -1;
				if (fabs(mcCell.a[1]-cCell[1]) > 0.01) j = -1;
				if (fabs(mcCell.a[2]-cCell[2]) > 0.01) j = -1;
				if (j == -1) {
					message_dialog_ID1 = -1;
					open_message_dialog1(main_window,1,"");
					Mbtn->disable();
					show_model = 0;
					show_model_check->set_int_val(show_model);
					show_model_check->disable();
					start_VICS = 0;
				}
				if (j == 0) {
					make_orto_matrix();
					create_Atom_list();
					Mbtn->enable();
					show_model = 0;
					show_model_check->set_int_val(show_model);
					show_model_check->enable();
					start_VICS = 1;
				}
				break;
					}
			case(-1):{
				Mbtn->disable();
				show_model = 0;
				show_model_check->set_int_val(show_model);
				show_model_check->disable();
				start_VICS = 0;
				break;
					 }
			}
		}
	}
//------------------------------------------------------------------------- Crystal Orientation Dialog
	if (corient_dialog_ID != 0){
		i = get_orient_dialog_id();
		if ((i == 0) || (i == 1)) {
			corient_dialog_ID = 0; orientation = 0;
			get_orient_dialog_pos(dwp[0][6], dwp[1][6]);
			glutSetWindow(main_window);
		}
		if (i == -1) orientation = 1;
		switch(i){
		case(0):{
			get_view_point(rotation);
			tbUpdate(0,GLUT_LEFT_BUTTON, rotation);
			break;
				}
		case(20):{
			get_view_point(rotation);
			tbUpdate(0,GLUT_LEFT_BUTTON, rotation);
			set_orient_dialog_id(-1);
			getOrientation(rotation);
			set_orient_param(rotation);
			break;
				 }
		}
	}
//------------------------------------------------------------------------- Stepwise Rotation Dialog
	if (stepwise_dialog_ID != 0){
		i = get_stepwise_dialog_id();
		if ((i == 1) || (i == 0)) {
			stepwise_dialog_ID = 0;
			get_step_dialog_pos(dwp[0][10], dwp[1][10]);
			glutSetWindow(main_window);
			stepwise_active = 0;
			rotation_tipe_radio->enable();
			bFree->enable();
			bX->enable();
			bY->enable();
			bZ->enable();
			get_stepwise_param(stepw_mode, stepw_rot_axis, stepw_angle);
			updateStatusBar();
		}
		if (i == 10){
			set_stepwise_dialog_id(-1);
			stepwise_active = 1;
			get_stepwise_param(stepw_mode, stepw_rot_axis, stepw_angle);
			switch(stepw_rot_axis)
			{
			case(0):{                            //--- X axis
				stepw_rot[0] = 1;
				stepw_rot[1] = 0;
				stepw_rot[2] = 0;
				break;
					}
			case(1):{                            //--- Y axis
				stepw_rot[0] = 0;
				stepw_rot[1] = 1;
				stepw_rot[2] = 0;
				break;
					}
			case(2):{                            //--- Z axis
				stepw_rot[0] = 0;
				stepw_rot[1] = 0;
				stepw_rot[2] = 1;
				break;
					}
			}
			tbStepW(stepwise_active,stepw_mode,stepw_angle,stepw_rot);
		}
	}

	glutPostRedisplay();
}


//**************************  VEND_RenderScene()   **************************//
void VEND_RenderScene( void )
{
	int i,j,k,l;


	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0f, -4.6f );
//----------------------------------------------------------- XYZ Axis
	glPushMatrix();
	  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      switch(projection_type)
	  {
	  case(0):{glTranslatef( -xy_aspect*0.72, -0.72,0);break;}
	  case(1):{glTranslatef( -xy_aspect*0.64, -0.64,0);break;}
	  }
	  if (stepwise_active == 1){
		  if (stepw_mode == 1) tbStepW(stepwise_active,stepw_mode,stepw_angle, stepw_rot);
		  else tbMatrix();
	  }
	  else tbMatrix();
	  glCallList(12);
	glPopMatrix();


	glPushMatrix();
	  glScalef( scale,scale,1 );
	  glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
	  if (projection_type == 0) glScalef( 1,1,perspective );
      else glScalef( 1,1,1);
	  if (stepwise_active == 1){
		  if (stepw_mode == 1) tbStepW(stepwise_active,stepw_mode,stepw_angle,stepw_rot);
		  else tbMatrix();
	  }
	  else tbMatrix();
	  glTranslatef(origin[0],origin[1],origin[2]);

	  for(i=0; i<numBox[0]; i++){
		  for(j=0; j<numBox[1]; j++){
			  for(k=0; k<numBox[2]; k++){
				  glPushMatrix();
				  glTranslatef((float)i*uTranso[0][0]/max_P,(float)i*uTranso[0][1]/max_P,(float)i*uTranso[0][2]/max_P);
				  glTranslatef((float)j*uTranso[1][0]/max_P,(float)j*uTranso[1][1]/max_P,(float)j*uTranso[1][2]/max_P);
				  glTranslatef((float)k*uTranso[2][0]/max_P,(float)k*uTranso[2][1]/max_P,(float)k*uTranso[2][2]/max_P);
//--------------------------------------------------unit cell
				  glCallList(1);
//--------------------------------------------------atom
				  if (show_model == 1) {
					  if ((i == 0) && (j == 0) && (k == 0)){
						  set_atom_prop();
						  glCallList(13);
					  }
				  }
//--------------------------------------------------isosurface
				  if (show_iso == 1){
				  	  set_material_prop();
				  	  if (mat_diffuse[3] < 1) {
				  		  glEnable(GL_ALPHA_TEST);
						  glEnable(GL_BLEND);
						  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					  }
					  glCallList(2);
					  if (mat_diffuse[3] < 1) {glDisable(GL_ALPHA_TEST);glDisable(GL_BLEND);}
//--------------------------------------------------slices (boundaries)
					  if (show_sls == 1) {
					  if (range[0] > 0) {glCallList(3);if (range[1] == 1) glCallList(6);}
					  else {if (i == 0) glCallList(3);}
					  if (range[1] < 1) {glCallList(6);if (range[0] == 0) glCallList(3);}
					  else {
						  if (i == 0){
							  glPushMatrix();
							  glTranslatef((float)(numBox[0]-1)*uTranso[0][0]/max_P,
							               (float)(numBox[0]-1)*uTranso[0][1]/max_P,
								     	   (float)(numBox[0]-1)*uTranso[0][2]/max_P);
							  glCallList(6);
							  glPopMatrix();
						  }
					  }
//------------------------------------------------------
					  if (range[2] > 0) {glCallList(4);if (range[3] == 1) glCallList(7);}
					  else {if (j == 0) glCallList(4);}
					  if (range[3] < 1) {glCallList(7);if (range[2] == 0) glCallList(4);}
					  else {
						  if (j == 0){
							  glPushMatrix();
							  glTranslatef((float)(numBox[1]-1)*uTranso[1][0]/max_P,
							               (float)(numBox[1]-1)*uTranso[1][1]/max_P,
								     	   (float)(numBox[1]-1)*uTranso[1][2]/max_P);
							  glCallList(7);
							  glPopMatrix();
						  }
					  }
//------------------------------------------------------
					  if (range[4] > 0) {glCallList(5);if (range[5] == 1) glCallList(8);}
					  else {if (k == 0) glCallList(5);}
					  if (range[5] < 1) {glCallList(8);if (range[4] == 0) glCallList(5);}
					  else {
						  if (k == 0){
							  glPushMatrix();
							  glTranslatef((float)(numBox[2]-1)*uTranso[2][0]/max_P,
							               (float)(numBox[2]-1)*uTranso[2][1]/max_P,
								    	   (float)(numBox[2]-1)*uTranso[2][2]/max_P);
							  glCallList(8);
							  glPopMatrix();
						  }
					  }
					  }
				  }
//--------------------------------------------------plane
				  if (opacity_hkl < 1) {
					  glEnable(GL_ALPHA_TEST);
					  glEnable(GL_BLEND);
					  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				  }
				  if (hkl_mode == 0) {
					  switch(index_c_type)
					  {
					  case(0):{if (i == 0) glCallList(9); break;}
					  case(1):{if (j == 0) glCallList(9); break;}
					  case(2):{if (k == 0) glCallList(9); break;}
					  case(3):{if ((i == 0) && (j == 0) && (k == 0)) glCallList(9); break;}
					  }
				  }
				  else {
					  for(l=0; l<num_of_hkl; l++) {
						  switch(index_type[l])
						  {
						  case(1):{if (i == 0) glCallList(9+l); break;}
						  case(2):{if (j == 0) glCallList(9+l); break;}
						  case(3):{if (k == 0) glCallList(9+l); break;}
					      case(0):{if ((i == 0) && (j == 0) && (k == 0)) glCallList(9+l); break;}
						  }
					  }
				  }
				  if (opacity_hkl < 1) {glDisable(GL_ALPHA_TEST);glDisable(GL_BLEND);}
				  glPopMatrix();

			  }
		  }
	  }


	glPopMatrix();
}


//**************************  VEND_WindowDump()   ***************************//
int VEND_WindowDump(FILE *fptr, int shiftX, int shiftY)
{
	int i,j,k,l,M;
	int I1,I2,J1,J2,II,III, JJJ;
	int Sum[3];
	int R0,G0,B0;
	int R[8],G[8],B[8];
	long offset, offset2;

	offset = shiftY*(3*tw*th*img_scale) + shiftX*3*tw;
	offset2 = 3*tw*(img_scale-1);

	image = (unsigned char *)malloc(24*tw*th);
   	if (image == NULL) {
		fprintf(stderr,"Failed to allocate memory for image\n");
		return(-1);
	}
	image1 = (unsigned char *)malloc(24*tw*th);
   	if (image1 == NULL) {
		fprintf(stderr,"Failed to allocate memory for image\n");
		return(-1);
	}

	glFinish();                          /* Finish all OpenGL commands */
	glPixelStorei(GL_PACK_ALIGNMENT, 4); /* Force 4-byte alignment */
	glReadBuffer(GL_BACK_LEFT);
	glReadPixels(tx,ty,tw,th, GL_RGB, GL_UNSIGNED_BYTE,image);

	for(l=0; l<img_smoothing-1; l++){
		for(j=0; j<th; j++){
			for(i=0; i<tw; i++){
				M = 8;
				I1 = -1;I2 =  1;
				J1 = -1;J2 =  1;
				if ((i==0) && (j==0))                 {I1 = 0; I2 = 1; J1 = 0; J2 = 1; M = 3;}
				if ((i==tw-1) && (j==0))              {I1 = -1; I2 = 0; J1 = 0; J2 = 1; M = 3;}
				if ((i==0) && (j==th-1))              {I1 = 0; I2 = 1; J1 = -1; J2 = 0; M = 3;}
				if ((i==tw-1) && (j==th-1))           {I1 = -1; I2 = 0; J1 = -1; J2 = 0; M = 3;}
				if ((i==0) && (j!=0) && (j!=th-1))    {I1 = 0; I2 = 1; J1 = -1; J2 = 1; M = 5;}
				if ((i==tw-1) && (j!=0) && (j!=th-1)) {I1 = -1; I2 = 0; J1 = -1; J2 = 1; M = 5;}
				if ((i!=0) && (i!=tw-1) && (j==0))    {I1 = -1; I2 = 1; J1 = 0; J2 = 1; M = 5;}
				if ((i!=0) && (i!=tw-1) && (j==th-1)) {I1 = -1; I2 = 1; J1 = -1; J2 = 0; M = 5;}
				R0 = image[3*j*tw+3*i+0];
				G0 = image[3*j*tw+3*i+1];
				B0 = image[3*j*tw+3*i+2];
				II = 0;
				for(III=I1; III<I2+1; III++){
					for(JJJ=J1; JJJ<J2+1; JJJ++){
						if ((III==0) && (JJJ==0)){II = II;}
						else {
							R[II] = image[3*(j+JJJ)*tw+3*(i+III)+0];
							G[II] = image[3*(j+JJJ)*tw+3*(i+III)+1];
							B[II] = image[3*(j+JJJ)*tw+3*(i+III)+2];
							II++;
						}
					}
				}
				Sum[0] = Sum[1] = Sum[2] = 0;
				for(k=0; k<M; k++) {
					Sum[0] = Sum[0] + (R[k]-R0);
					Sum[1] = Sum[1] + (G[k]-G0);
					Sum[2] = Sum[2] + (B[k]-B0);
				}
				image[3*j*tw+3*i+0] = R0 + Sum[0]/M;;
				image[3*j*tw+3*i+1] = G0 + Sum[1]/M;
				image[3*j*tw+3*i+2] = B0 + Sum[2]/M;
			}
		}
	}

	fseek( fptr, offset, SEEK_SET);
	for (j=0;j<th;j++) {
		for (i=0;i<tw;i++) {
			if (img_smoothing > 1){
				putc(image[3*j*tw+3*i+0],fptr);
				putc(image[3*j*tw+3*i+1],fptr);
				putc(image[3*j*tw+3*i+2],fptr);
			}
			else {
				putc(image[3*j*tw+3*i+0],fptr);
				putc(image[3*j*tw+3*i+1],fptr);
				putc(image[3*j*tw+3*i+2],fptr);
			}
		}
		fseek( fptr, offset2, SEEK_CUR);
	}
  
	free(image);
	free(image1);
	return(0);
}

//**************************  VEND_window_dump()   **************************//
int VEND_window_dump()
{
	int i,j;
    FILE *fptr;
	int num[2],pixels[2],max_pixels, max_win;

	glutSetWindow(main_window);
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS,num); 
	pixels[0] = tw*img_scale;
	pixels[1] = th*img_scale;
	max_pixels = (pixels[0] >= pixels[1]) ? pixels[0] : pixels[1];
	max_win = (tw >= th) ? tw : th;
	if (max_pixels > num[0]) img_scale = num[0] / max_win;

	if ((fptr = tmpfile()) == NULL) {
		fprintf(stderr,"Failed to open file for window dump\n");
		return(-1);
	}
	printf("Window dump...\n");
	for (j=0; j<img_scale; j++) {
		printf("%i :  ",j);
		for (i=0; i<img_scale; i++) {
			printf("%i ",i);
			glViewport(-i*tw,-j*th+ty,img_scale*tw,img_scale*th);
			xy_aspect = (float)tw / (float)th;
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			switch(projection_type)
			{
			case(0):{glFrustum( -xy_aspect*.02, xy_aspect*.02, -.02, .02, 0.1, 100.0 );break;}
			case(1):{glOrtho( -xy_aspect*0.8, xy_aspect*0.8, -0.8, 0.8, 0.1, 10.0 );break;}
			}
			glMatrixMode( GL_MODELVIEW );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			VEND_RenderScene();
			VEND_WindowDump(fptr,i,j);
		}
		printf("\n");
	}
	rewind( fptr );
	printf("Save image...\n");
	switch(img_format)
	{
	case(2):{
		write_JPEG_file (img_fName, fptr, tw*img_scale, th*img_scale);
		break;
			}
	case(3):{
		write_JPEG2000_file(img_fName, fptr, tw*img_scale, th*img_scale);
		break;
			}
	case(8):
	case(7):
	case(6):
	case(5):
	case(4):
	case(1):
	case(0):{
		WriteBitmap(img_fName, fptr, tw*img_scale, th*img_scale, img_format);
		break;
			}
	}
	fclose(fptr);

	glViewport( tx, ty, tw, th );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	VEND_RenderScene();
	run_img_export = 0;
	printf("OK!!!\n");
	return(0);
}


//**************************  VEND_2Dwindow_dump()   ************************//
int VEND_2Dwindow_dump()
{
	int i,j;
    FILE *fptr;
	int num[2],pixels[2],max_pixels, max_win;


	glutSetWindow(window_2D_ID);
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS,num); 
	pixels[0] = tw*img_scale;
	pixels[1] = th*img_scale;
	max_pixels = pixels[0];
	max_win = tw;
	if (max_pixels > num[0]) img_scale = num[0] / max_win;

	if ((fptr = tmpfile()) == NULL) {
		fprintf(stderr,"Failed to open file for window dump\n");
		return(-1);
	}
	printf("\nWindow dump...\n");
	for (j=0; j<img_scale; j++) {
		printf("%i :  ",j);
		for (i=0; i<img_scale; i++) {
			printf("%i ",i);
			glViewport(-i*tw,-j*th,img_scale*tw,img_scale*th);
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			glOrtho(-0.75, 0.75, -0.75, 0.75, -0.75, 0.75);
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			D2D_RenderScene();
			VEND_WindowDump(fptr,i,j);
		}
		printf("\n");
	}
	rewind( fptr );
	printf("Save image...\n");
	switch(img_format)
	{
	case(2):{
		write_JPEG_file (img_fName, fptr, tw*img_scale, th*img_scale);
		break;
			}
	case(3):{
		write_JPEG2000_file(img_fName, fptr, tw*img_scale, th*img_scale);
		break;
			}
	case(8):
	case(7):
	case(6):
	case(5):
	case(4):
	case(1):
	case(0):{
		WriteBitmap(img_fName, fptr, tw*img_scale, th*img_scale, img_format);
		break;
			}
	}
	fclose(fptr);

	run_img_export = 0;
	printf("OK!!!\n");

	glViewport( 0, 0, tw, th );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	D2D_RenderScene();
	glutSetWindow(main_window);
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );

	return(0);
}

//**************************  VEND_3Dwindow_dump()   ************************//
int VEND_3Dwindow_dump()
{
	int i,j;
    FILE *fptr;
	int num[2],pixels[2],max_pixels, max_win;

	glutSetWindow(window_3D_ID);
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS,num); 
	pixels[0] = tw*img_scale;
	pixels[1] = th*img_scale;
	max_pixels = (pixels[0] >= pixels[1]) ? pixels[0] : pixels[1];
	max_win = (tw >= th) ? tw : th;
	if (max_pixels > num[0]) img_scale = num[0] / max_win;

	if ((fptr = tmpfile()) == NULL) {
		fprintf(stderr,"Failed to open file for window dump\n");
		return(-1);
	}
	printf("\nWindow dump...\n");
	for (j=0; j<img_scale; j++) {
		printf("%i :  ",j);
		for (i=0; i<img_scale; i++) {
			printf("%i ",i);
			glViewport(-i*tw,-j*th,img_scale*tw,img_scale*th);
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			D3D_RenderScene();
			VEND_WindowDump(fptr,i,j);
		}
		printf("\n");
	}
	rewind( fptr );
	printf("Save image...\n");
	switch(img_format)
	{
	case(2):{
		write_JPEG_file (img_fName, fptr, tw*img_scale, th*img_scale);
		break;
			}
	case(3):{
		write_JPEG2000_file(img_fName, fptr, tw*img_scale, th*img_scale);
		break;
			}
	case(8):
	case(7):
	case(6):
	case(5):
	case(4):
	case(1):
	case(0):{
		WriteBitmap(img_fName, fptr, tw*img_scale, th*img_scale, img_format);
		break;
			}
	}
	fclose(fptr);

	run_img_export = 0;
	printf("OK!!!\n");

	glViewport( 0, 0, tw, th );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	D3D_RenderScene();
	glutSetWindow(main_window);
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );

	return(0);
}


//**************************  VEND_Keyboard()   *****************************//
void VEND_Keyboard(unsigned char Key, int x, int y)
{
}


//**************************  VEND_Mouse()   ********************************//
void VEND_Mouse(int button, int button_state, int x, int y )
{
	int animateID;

	motion = 0;

	if (orientation == 1) return;
	animateID = tbMouse(button, button_state, x, y, rotation_mode,rotation_type);
	if (animateID == 1) glutIdleFunc(VEND_Idle);
	if ( button == GLUT_LEFT_BUTTON  && button_state == GLUT_DOWN ){
		motion = 1;
		last_x = x;
		last_y = y;

		switch(tools_mode)
		{
		case(0):{glutSetCursor(GLUT_CURSOR_CROSSHAIR); break;}
		case(1):{glutSetCursor(GLUT_CURSOR_UP_DOWN); break;}
		case(2):{glutSetCursor(GLUT_CURSOR_INFO); break;}
		}
		if ((tools_mode == 0) && (rotation_type == 2)) {
			tbClickMotion(x,y,rotation_mode,fabs(stepw_angle));
		}
	}
	if ( button == GLUT_LEFT_BUTTON  && button_state == GLUT_UP ){
		motion = 0;
        glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	if (motion == 1) stw_btn->disable();
	else if ((tools_mode == 0) && (rotation_type == 2)) stw_btn->disable();
	else stw_btn->enable();
}


//**************************  VEND_Reshape()   ******************************//
void VEND_Reshape( int x, int y )
{
	float R,dR1,dR2;
	GLint iR;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	R = 4*(float)tw/4;
	iR = 4*(tw/4);
	if (R != iR) {
		dR1 = R - iR;
		dR2 = (iR+4) - R;
		if (dR2 <= dR1) iR = iR + 4;
	}
    glutReshapeWindow( iR, ty + th);

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	tbReshape(tw, th);
	xy_aspect = (float)tw / (float)th;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	switch(projection_type)
	{
	case(0):{glFrustum( -xy_aspect*.02, xy_aspect*.02, -.02, .02, .1, 15.0 );break;}
	case(1):{glOrtho( -xy_aspect*0.8, xy_aspect*0.8, -0.8, 0.8, 0.1, 10.0 );break;}
	}
	glMatrixMode( GL_MODELVIEW );
	updateStatusBar();
	glutPostRedisplay();
}


//**************************  VEND_Display()   ******************************//
void VEND_Display( void )
{
	int chk = -1;
	glClearColor( BKCL[0], BKCL[1], BKCL[2], 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	switch(projection_type)
	{
	case(0):{glFrustum( -xy_aspect*.02, xy_aspect*.02, -.02, .02, .1, 15.0 );break;}
	case(1):{glOrtho( -xy_aspect*0.8, xy_aspect*0.8, -0.8, 0.8, 0.1, 10.0 );break;}
	}
//--- Get Image Settings and Start Export
	if( run_img_export == 1){
		switch(img_exp_from){
		case(0):{chk = VEND_window_dump();break;}
		case(1):{chk = VEND_2Dwindow_dump();break;}
		case(2):{chk = VEND_3Dwindow_dump();break;}
		}
		if (chk == 0) {
			chk = -1;
			chk_image_ID = -2;
		}
	}

	VEND_RenderScene();

	glFinish();
	glutSwapBuffers(); 
}


//**************************************** VEND_control() *******************/
void VEND_control( int control )
{
	int i;
	switch(control)
	{
//------------------------------------------------------------- Open File
	case(1):{
		if (openfile_dialog_ID == 0) {
			openfile_dialog_ID = -1;
			fileID = def_file_format_ID;
			open_filename_dialog(main_window,fileID);
		}
		break;
			}
//------------------------------------------------------------- Save VEND File
	case(2):{
		if (start_VEND == 0) break;
		sfileID = 0;
		sprintf(sFileName,"");
		i = save_file_dialog(sfileID, sFileName, 0);
		if (i > 0) {
			check_file_ext_exist(sfileID, sFileName,0);
			getOrientation(rotation);
			saveSCRIPT(sFileName);
		}
		break;
			}
//------------------------------------------------------------- Image Export
	case(3):{
		if (start_VEND == 0) break;
		if (image_Dialog_ID == 0) {
			sprintf(img_fName,"");
			i = save_file_dialog(img_format, img_fName, 1);
			if (i > 0){
				check_file_ext_exist(img_format, img_fName,1);
				image_Dialog_ID = -1;
				set_Export_param(img_fName,img_format,window_2D_ID,window_3D_ID);
				Export_Dialog(main_window);
			}
		}
		break;
			}
//------------------------------------------------------------- Info
	case(4):{
		if (start_VEND == 0) break;
		if (info_Dialog_ID == 0) {
			info_Dialog_ID = -1;
			open_info_dialog(main_window,FileName,Title,NPIX,HMIN,HMAX,dwp[0][3],dwp[1][3]);
		}
		break;
			}
//------------------------------------------------------------- Atom
	case(5):{
		if (start_VEND == 0) break;
		if (model_Dialog_ID == 0) model_Dialog_ID = -1;
		break;
			}
//------------------------------------------------------------- 2D-Projection
	case(6):{
		if (start_VEND == 0) break;
		if (prj_Dialog_ID == 0) {
			prj_Dialog_ID = -1;
			set_prj_dialog_settings(prj_plane,prj_cell,prj_pal,prj_cont_show,prj_show_2D,prj_show_3D,
				                    prj_RGB, prj_width, prj_hkl, prj_grid);
			set_prj_dialog_param(prj_project,prj_seclev_scale,prj_3Dlev_scale,prj_org);
			open_prj_dialog(main_window,dwp[0][8],dwp[1][8]);
		}
		break;
			}
//------------------------------------------------------------- Preferences
	case(7):{
		if (start_VEND == 0) break;
		if (pref_Dialog_ID == 0) {
			pref_Dialog_ID = -1;
			preferency_set_def_param(def_BKCL,def_BKCL2D,def_BKCL3D,def_unit_cell_color,def_unit_cell_width,
				                     def_show_axis,stepw_angle,def_file_format_ID,save_pos_ID,save_dpos_ID);
			preferency_set_cur_param(BKCL,BKCL2D,BKCL3D,unit_cell_color,unit_cell_width,show_axis);
			open_preferences_dialog(main_window,dwp[0][7],dwp[1][7]);
		}
		break;
			}
//------------------------------------------------------------- About
	case(8):{
		if (about_dialog_ID == 0){
			about_dialog_ID = -1;
			open_about_dialog(main_window,dwp[0][0],dwp[1][0]);
		}
		break;
			}
//------------------------------------------------------------- General
	case(10):{
		if (start_VEND == 0) break;
		if (genral_Dialog_ID == 0) {
			genral_Dialog_ID = -1;
			set_general_param(l_ambient,l_diffuse,l_direction,BKCL,BKCL2D,BKCL3D,show_axis,
				              projection_type,perspective);
			open_general_dialog(main_window, dwp[0][2], dwp[1][2]);
		}
		break;
			 }
//------------------------------------------------------------- Isosurface control
	case(11):{
		if (start_VEND == 0) break;
		if (isosurface_Dialog_ID == 0) {
			isosurface_Dialog_ID = -1;
			set_isosurface_param(HMIN,HMAX,ORIGIN,OBJLEV,SECLEV,range_ID,numBox,range,
				                 mat_ambient,mat_diffuse,mat_specular,mat_emission,
								 mat_shininess,iso_pm_parm,DELC_ID);
			open_isosurface_dialog(main_window,dwp[0][4],dwp[1][4]);
		}
		break;
			 }
//------------------------------------------------------------- Slices control
	case(12):{
		if (start_VEND == 0) break;
		if (sli_Dialog_ID == 0) {
			sli_Dialog_ID = -1;
			set_slices_dialog_param(num_of_hkl, opacity_hkl, position_hkl,index_hkl,index_show_hkl,
				                    palette_hkl,objlev_scale, seclev_scale);
			open_slices_dialog(main_window,dwp[0][9],dwp[1][9]);
		}
		break;
			 }
//------------------------------------------------------------- Unit Cell Outline control
	case(13):{
		if (start_VEND == 0) break;
		if (unitcell_Dialog_ID == 0) {
			unitcell_Dialog_ID = -1;
			set_ucell_prop_dialog(unit_cell_show,unit_cell_width,unit_cell_color,unit_cell_type);
			open_ucell_prop_dialog(main_window,dwp[0][11], dwp[1][11]);
		}
		break;
			 }
//------------------------------------------------------------- Unit Cell Outline control
	case(14):{
		if (start_VEND == 0) break;
		if (model_proper_dialog_ID == 0) {
			model_proper_dialog_ID = -1;
			set_model_prop_params(model_type, atoms_stack,atoms_slice, bonds_stack,bonds_slice,  
						   BNCL,bRadii[0],bRadii[1],tAtom,bonds_type);
			for(i=0; i<tAtom; i++){
				set_model_atom_params(i,AtomT[i].symb,AtomT[i].color,AtomT[i].R);
			}
			open_model_prop_dialog(main_window,dwp[0][5],dwp[1][5]);
		}
		break;
			 }
//------------------------------------------------------------- Model
	case(20):{set_display_list();break;}
//------------------------------------------------------------- Orientation
	case(25):{
		if (start_VEND == 0) break;
		if (corient_dialog_ID == 0) {
			corient_dialog_ID = -1;
			getOrientation(rotation);
			init_orient_dialog_param(rotation,cCell);
			open_orient_prop_dialog(main_window,dwp[0][6],dwp[1][6]);
		}
		break;
			 }
//------------------------------------------------------------- Tools Mode
	case(30):
	case(31):
	case(32):{
		tools_mode = control - 30;
		if (tools_mode != 0) rotation_panel->disable();
		                else rotation_panel->enable();
		break;
			 }
//------------------------------------------------------------- Tools Mode
	case(40):{
		if (rotation_type==1) tbAnimate(GL_TRUE);
		else tbAnimate(GL_FALSE);
		break;
			  }
//------------------------------------------------------------- Rotation Mode
	case(53):
	case(52):
	case(51):
	case(50):{rotation_mode = control - 50;break;}
//====================================================================== Stepwise rotation
	case(54):{
		if (start_VEND == 0) break;
		if (stepwise_dialog_ID == 0){
			stepwise_dialog_ID = -1;
			rotation_tipe_radio->disable();
			bFree->disable();
			bX->disable();
			bY->disable();
			bZ->disable();
			updateStatusBar1();
			init_stepwise_dialog(stepw_angle,stepw_mode);
			open_stepwise_dialog(main_window,dwp[0][10],dwp[1][10]);
		}
		break;
			  }

	}
	updateStatusBar();
	glutPostRedisplay();
}




//**************************  init_program_setting()   **********************//
void init_program_setting()
{
	int i;

	fileID = 0;
	sprintf(FileName,"");
	OBJLEV = 1.0;
	run_img_export = 0;
	sprintf(img_fName,"");
	img_format = 0;

//--------------------------------------------------- Initial Windows Positions
	save_pos_ID = 0;
	save_dpos_ID = 0;
	menu_x  =  10; menu_y  = 30;
	tools_x =  10; tools_y = 90;
	graph_x = 210; graph_y = 90;
    graph_w = 500; graph_h = 600;
	for(i=0; i<12; i++) {
		cdwp[0][i] = dwp[0][i] = 180; 
		cdwp[1][i] = dwp[1][i] = 140;
	}
//-----------------------------------------------------
	load_preferency();
	set_initital_parameters();
	set_initital_model_parameters();
	load_palette();

	scale = 1.0;
	obj_pos[0] = obj_pos[1] = obj_pos[2] = 0;

	TOTAL_T = 0;
//--------------------------------------------------- Initial General Parameters
	for (i=0; i<16; i++) l_direction[i] = 0.0f;

	l_direction[0] = l_direction[5] = l_direction[10] = l_direction[15] = 1.0f;

	l_position[0] = -0.2f;
	l_position[1] = -0.2f;
	l_position[2] = 20;
	l_position[3] = 0;

	l_dir[0] = l_dir[1] = l_dir[2] = -1;

	l_ambient[0] = l_ambient[1] = l_ambient[2] = 0.0f; l_ambient[3] = 1.0f;
	l_diffuse[0] = l_diffuse[1] = l_diffuse[2] = 0.7f; l_diffuse[3] = 1.0f;   

//---------------------------------- Initial Atom Material Properties
	atom_ambient[0] = atom_ambient[1] = atom_ambient[2] = atom_ambient[3] = 1.0;
	atom_specular[0] = atom_specular[1] = atom_specular[2] = atom_specular[3] = 1.0;
	atom_emission[0] = atom_emission[1] = atom_emission[2] = 0.0; atom_emission[3] = 1.0;
	atom_shininess = 128;
//--------------------------------------------------- Initial 2D and 3D windows Settings
	window_2D_ID = -1;
	window_3D_ID = -1;
//--------------------------------------------------- Initial Atom Dialog Settings
	atoms_stack = 15;
	atoms_slice = 15;
	bonds_stack = 1;
	bonds_slice = 15;
	BNCL[0] = BNCL[1] = BNCL[2] = 0.8;
	bRadii[0] = 0.5;
	bRadii[1] = 0.5;
//--------------------------------------------------- Initial Settings
	stepwise_active = 0;
	stepw_mode = 0;
	stepw_rot_axis = 0;
//	stepw_angle = 2;
	stepw_rot[0] = stepw_rot[1] = stepw_rot[2] = 0;

//--------------------------------------------------- Initial Dialog Settings
	arg_openfile_ID = 0;
	openfile_dialog_ID = 0;
	savefile_dialog_ID = 0;
	isosurface_Dialog_ID = 0;
	unitcell_Dialog_ID = 0;
	info_Dialog_ID = 0;
	sli_Dialog_ID = 0;
	image_Dialog_ID = 0;
	pref_Dialog_ID = 0;
	genral_Dialog_ID = 0;
	prj_Dialog_ID = 0;
	cont_Dialog_ID = 0;
	model_Dialog_ID = 0;
	corient_dialog_ID = 0;
	about_dialog_ID = 0;
	stepwise_dialog_ID = 0;
	color_dialog_ID = 0;
	model_proper_dialog_ID = 0;
	message_dialog_ID1 = 0;
	chk_image_ID = 0;
}


//****************************************************************//
//                            main()                              //
//****************************************************************//
int main(int argc, char* argv[])
{
	int num[2];
	char cmd;

	if (getenv( "VENUS" ) != NULL) sprintf(progvar,"%s",getenv( "VENUS" ));
	else sprintf(progvar,"");
	init_program_setting();
//-------------------------   Read 3DD file from command line
	if (argc>1) {
		fileID = -1;
		OBJLEV = 1.0;
		arg_fname = argv[1];
		if (argc == 3) {
			if ((strstr(argv[2],"-l") != NULL) || (strstr(argv[2],"-L") != NULL)){
				sscanf(argv[2],"%c%c%f",&cmd,&cmd,&OBJLEV);
			}
			else fileID = get_format(argv[2]);
		}
		if (argc == 4) sscanf(argv[3],"%c%c%f",&cmd,&cmd,&OBJLEV);
		if ((fileID < 0) || (fileID > 6)) fileID = check_format(arg_fname);
		if (fileID >= 0) arg_openfile_ID = -1;
	}
//-------------------------   Initialize GLUT and create window
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition( graph_x, graph_y );
	glutInitWindowSize(graph_w,graph_h );
	main_window = glutCreateWindow("Graphics");
	glutDisplayFunc( VEND_Display );
	GLUI_Master.set_glutReshapeFunc( VEND_Reshape );  
	GLUI_Master.set_glutKeyboardFunc( VEND_Keyboard );
//	GLUI_Master.set_glutSpecialFunc( NULL );
	GLUI_Master.set_glutMouseFunc( VEND_Mouse );
	glutMotionFunc( VEND_Mouse_Motion );
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
//-------------------------------   Track_Ball Initialization
	initTrackBall();
//-------------------------------   Create the statusbar subwindow
    gluisub = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );

	gluisub->set_main_gfx_window( main_window );
	statusbar = gluisub->add_statictext("");
	updateStatusBar();
//-------------------------------   Output General information
	printf("Video configuration: %s\n",glGetString(GL_RENDERER) );
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS,num); 
	printf("Maximum supported width and height of the viewport: %i x %i\n",num[0],num[1]);
//-------------------------------   Initialize OpenGL main parameters
	glEnable(GL_LIGHTING);
	glEnable( GL_NORMALIZE );
	glEnable(GL_LIGHT0);

	set_light_prop();
	set_material_prop();
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
//-------------------------------   Create the menu window
	menu_glui = GLUI_Master.create_glui("VEND: Menu",0,menu_x,menu_y );
	menu_glui->set_main_gfx_window( main_window );
//-------------------------------   Add Main Menu Buttons to GLUI menu window
//--- File Menu
	menu_glui->add_column(true);
	open_btn = menu_glui->add_button("Open", 1, VEND_control);open_btn->set_w(50);
	menu_glui->add_column(false);
	save_btn = menu_glui->add_button("Save", 2, VEND_control);save_btn->set_w(50);
	menu_glui->add_column(false);
	exp_btn  = menu_glui->add_button("Image", 3, VEND_control);exp_btn->set_w(50);
	menu_glui->add_column(true);
//--- Options Menu
	atom_btn = menu_glui->add_button("VICS", 5, VEND_control);atom_btn->set_w(50);
	menu_glui->add_column(false);
	proj_btn = menu_glui->add_button("2D", 6, VEND_control);proj_btn->set_w(50);
	menu_glui->add_column(true);
//--- Help Menu
	info_btn = menu_glui->add_button("Info", 4, VEND_control);info_btn->set_w(50);
	menu_glui->add_column(false);
	pref_btn = menu_glui->add_button("Preferences", 7, VEND_control);pref_btn->set_w(50);
	menu_glui->add_column(true);
	about_btn = menu_glui->add_button("About", 8, VEND_control);about_btn->set_w(50);
	menu_glui->add_column(false);
	exit_btn = menu_glui->add_button("Exit", 0,(GLUI_Update_CB)exit );exit_btn->set_w(50);
	menu_glui->add_column(true);

//-------------------------------   Create the tools window
	glui = GLUI_Master.create_glui("Dialog",0,tools_x,tools_y);
	glui->set_main_gfx_window( main_window );
	main_panel = glui->add_panel("",GLUI_PANEL_RAISED);
//-----------------------------------------------------------------//
//   Properties Rollout                                            //
//-----------------------------------------------------------------//
	rollout_prop = glui->add_rollout_to_panel(main_panel, "Properties", false );
	glui->add_separator_to_panel(rollout_prop);
	glui->add_button_to_panel(rollout_prop, "General", 10, VEND_control);
	glui->add_separator_to_panel(rollout_prop);
	glui->add_button_to_panel(rollout_prop, "Isosurfaces", 11, VEND_control);
	glui->add_button_to_panel(rollout_prop, "Slices", 12, VEND_control);
	glui->add_button_to_panel(rollout_prop, "Unit cell", 13, VEND_control);
	glui->add_separator_to_panel(rollout_prop);
	Mbtn = glui->add_button_to_panel(rollout_prop, "Model", 14, VEND_control);
	Mbtn->disable();
	glui->add_separator_to_panel(rollout_prop);
//-----------------------------------------------------------------//
//   Model Panel                                                   //
//-----------------------------------------------------------------//
	model_panel = glui->add_panel_to_panel(main_panel,"Models",true);
	model_radio = glui->add_radiogroup_to_panel(model_panel,&model_mode,20,VEND_control);
	glui->add_radiobutton_to_group( model_radio, "Surface" );
	glui->add_radiobutton_to_group( model_radio, "Frame" );
	glui->add_radiobutton_to_group( model_radio, "Mesh" );
//-----------------------------------------------------------------//
//   Show Isosurface                                               //
//-----------------------------------------------------------------//
	glui->add_separator_to_panel(main_panel);
	show_iso_check = glui->add_checkbox_to_panel(main_panel,"Show isosurfaces",&show_iso,21,VEND_control);
	show_sls_check = glui->add_checkbox_to_panel(main_panel,"Show sections",&show_sls,21,VEND_control);
	glui->add_separator_to_panel(main_panel);
//-----------------------------------------------------------------//
//   Show Atoms                                                    //
//-----------------------------------------------------------------//
	show_model_check = glui->add_checkbox_to_panel(main_panel,"Show model",&show_model,22,VEND_control);
	show_model_check->disable();
	glui->add_separator_to_panel(main_panel);
//-----------------------------------------------------------------//
//  Crystal Orientation
//-----------------------------------------------------------------//
	bOrient = glui->add_button_to_panel(main_panel,"Orientation",25,VEND_control);
	glui->add_separator_to_panel(main_panel);

//-----------------------------------------------------------------//
//   Tools Panel                                                   //
//-----------------------------------------------------------------//
	tools_panel = glui->add_panel_to_panel(main_panel,"Manipulations",true);
	bR = glui->add_button_to_panel(tools_panel,"Rotate",30,VEND_control);
	bM = glui->add_button_to_panel(tools_panel,"Magnify",31,VEND_control);
	bT = glui->add_button_to_panel(tools_panel,"Translate",32,VEND_control);
//-----------------------------------------------------------------//
//   Rotation Panel                                                //
//-----------------------------------------------------------------//
	rotation_panel = glui->add_panel_to_panel(main_panel,"Rotation modes",true);
	stw_btn = glui->add_button_to_panel(rotation_panel,"Stepwise",54,VEND_control);
	glui->add_separator_to_panel(rotation_panel);
	rotation_tipe_radio = glui->add_radiogroup_to_panel(rotation_panel,&rotation_type,40,VEND_control);
	glui->add_radiobutton_to_group(rotation_tipe_radio,"Drag");
	glui->add_radiobutton_to_group(rotation_tipe_radio,"Push");
	glui->add_radiobutton_to_group(rotation_tipe_radio,"Click");
	rotation_tipe_radio->set_int_val(rotation_type);
	glui->add_separator_to_panel(rotation_panel);
	bFree = glui->add_button_to_panel(rotation_panel,"Free",50,VEND_control);
	bX = glui->add_button_to_panel(rotation_panel,"X axis",51,VEND_control);
	bY = glui->add_button_to_panel(rotation_panel,"Y axis",52,VEND_control);
	bZ = glui->add_button_to_panel(rotation_panel,"Z axis",53,VEND_control);
//-------------------------------   Register the idle callback with GLUI
	GLUI_Master.set_glutIdleFunc( VEND_Idle );
	
	rollout_prop->open();rollout_prop->close();

//-------------------------------   Run 
	clear_display_list();
	glutMainLoop();

	return(0);
}

