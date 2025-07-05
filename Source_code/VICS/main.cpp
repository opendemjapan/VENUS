#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>


#include "main_param.h"
//---
#include "about.h"
#include "atoms.h"
#include "bonds.h"
#include "calc.h"
#include "claster.h"
#include "hkl.h"
#include "graph.h"
#include "message.h"
#include "new_edit.h"
//#include "open.h"
#include "orient.h"
#include "ortho_origin.h"
#include "poly.h"
#include "prefer.h"
#include "range.h"
#include "rendering.h"
#include "sbonds.h"
#include "spin.h"
#include "stepw.h"
#include "ucell.h"
#include "winopen.h"

extern "C"
{
#include "bitmaplib.h"
#include "struc_par.h"
#include "trackball.h"
#include "GlutPalette.h"
#include "polygon.h"
}


int motion = 0;
int orientation = 0;
//int start_VICS = 0;
int start_VICS = -1;


/**************************************** m3Datom_TB_init() *****************/
void m3Datom_TB_init(void)
{
	tbInit(GLUT_LEFT_BUTTON, rotation);
	tbAnimate(GL_FALSE);
}


/**************************************** _update__window_title() ***********/
void _update__window_title()
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

/**************************************** calcAtomDist() ********************/
void calcAtomDist(int IN)
{
	int I0,I1,I2,J1,J2;
	double D;
	double esd,mesd;
	int iesd;
	int ID;
	char line[80];
	VECTOR3D V,U;
	VECTOR3D V1,V2,EV1,EV2;

	I1 = 0;
	I2 = 0;
	if (IN == 0) {
		I1 = pick_obj[0] - 1;
		I2 = pick_obj[1] - 1;
	}
	if (IN == 1) {
		I1 = bAtom[pick_obj[0]-nXYZ-1].Atom1;
		I2 = bAtom[pick_obj[0]-nXYZ-1].Atom2;
	}
	if (IN > 1){
		I0 = pick_obj[0]-nXYZ-nBonds-1;
		I1 = poly_list[I0];
		I2 = m[I1].b_list[IN-2];
	}
	J1 = m[I1].Num;
	J2 = m[I2].Num;
	V1.x = m[I1].x; V1.y = m[I1].y; V1.z = m[I1].z;
	V2.x = m[I2].x; V2.y = m[I2].y; V2.z = m[I2].z;

	EV1.x = lAtom[J1].ex; EV1.y = lAtom[J1].ey; EV1.z = lAtom[J1].ez;
	EV2.x = lAtom[J2].ex; EV2.y = lAtom[J2].ey; EV2.z = lAtom[J2].ez;
	calc_D2_esd2(V1,V2,EV1,EV2,cCell.a,cCell.ea,cCell.max_P,D,esd);
	esd = sqrt(esd);
	mesd = esd * 100000;
	iesd = int(mesd);
	D   = sqrt(D);
	ID = 0;
	if (esd >= 1) ID = 5;
	if ((mesd >= 1) && (iesd < 10)) ID = 1;
	if ((mesd >= 10) && (mesd < 100)) {
		if (mesd < 15 ) {ID = 1; iesd = iesd;}
		else {
			ID = 2;
			if ((mesd >= 15) && (mesd <= 20)) iesd = 2;
			else iesd = iesd/10;
		}
	}

	if ((mesd >= 100) && (mesd < 1000)) {
		if (iesd < 150) {ID = 2; iesd = iesd/10;}
		else {
			ID = 3;
			if ((mesd >= 150) && (mesd <= 200)) iesd = 2;
			else iesd = iesd/100;
		}
	}

	if ((iesd >= 1000) && (iesd < 10000)) {
		if (iesd <1500) {ID = 3; iesd = iesd/100;}
		else {
			ID = 4;
			if ((mesd >= 1500) && (mesd <= 2000)) iesd = 2;
			else iesd = iesd/1000;
		}
	}
	if (esd < 0.00001) ID = 0;

	switch(ID)
	{
	case(0):{
		sprintf(infotext,"l(%s-%s) =%8.5f(0) Angstrom",lAtom[J1].name, lAtom[J2].name ,D);
		if (IN == 0) printf("\nl(%s-%s) = %8.5f(0) Angstrom\n",lAtom[J1].name, lAtom[J2].name ,D);
		if (IN == 1) printf("\nBOND: l(%s-%s) = %8.5f(0) Angstrom\n",lAtom[J1].name,lAtom[J2].name,D);
		if (IN >  0) printf("l(%s-%s) = %8.5f(0) Angstrom\n",lAtom[J1].name, lAtom[J2].name ,D);
		break;
			}
	case(1):{
		sprintf(infotext,"l(%s-%s) =%8.5f(%i) Angstrom",lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 0) printf("\nl(%s-%s) = %8.5f(%i) Angstrom\n",lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 1) printf("\nBOND: l(%s-%s) = %8.5f(%i) Angstrom\n",lAtom[J1].name,lAtom[J2].name,D,iesd);
		if (IN > 1)  printf("l(%s-%s) = %8.5f(%i) Angstrom\n",lAtom[J1].name, lAtom[J2].name ,D,iesd);
		break;
			}
	case(2):{
		sprintf(infotext,"l(%s-%s) =%7.4f(%i) Angstrom",lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 0) printf("\nl(%s-%s) = %7.4f(%i) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 1) printf("\nBOND: l(%s-%s) = %7.4f(%i) Angstrom\n",lAtom[J1].name,lAtom[J2].name,D,iesd);
		if (IN > 1)  printf("l(%s-%s) = %7.4f(%i) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D,iesd);
		break;
			}
	case(3):{
		sprintf(infotext,"l(%s-%s) =%6.3f(%i) Angstrom",lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 0) printf("\nl(%s-%s) = %6.3f(%i) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 1) printf("\nBOND: l(%s-%s) = %6.3f(%i) Angstrom\n",lAtom[J1].name,lAtom[J2].name,D,iesd);
		if (IN > 1)  printf("l(%s-%s) = %6.3f(%i) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D,iesd);
		break;
			}
	case(4):{
		sprintf(infotext,"l(%s-%s) =%5.2f(%i) Angstrom",lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 0) printf("\nl(%s-%s) = %5.2f(%i) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D,iesd);
		if (IN == 1) printf("\nBOND: l(%s-%s) = %5.2f(%i) nm\n",lAtom[J1].name,lAtom[J2].name,D,iesd);
		if (IN > 1)  printf("l(%s-%s) = %5.2f(%i) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D,iesd);
		break;
			}
	case(5):{
		sprintf(infotext,"l(%s-%s) =%4.1f(>1.0) Angstrom",lAtom[J1].name, lAtom[J2].name ,D);
		if (IN == 0) printf("\nl(%s-%s) = %4.1f(>1.0) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D);
		if (IN == 1) printf("\nBOND: l(%s-%s) = %4.1f(>1.0) nm\n",lAtom[J1].name,lAtom[J2].name,D);
		if (IN > 1)  printf("l(%s-%s) = %4.1f(>1.0) Angstrom\n", lAtom[J1].name, lAtom[J2].name ,D);
		break;
			}
	}

	if (IN < 2){
		statusbarinfo->set_name(infotext);
		
		sprintf(line,"");
		get_symmetry_string(m[I1].symnum, line);
		V.x = lAtom[J1].x; V.y = lAtom[J1].y; V.z = lAtom[J1].z;
		U = make_symmetry(m[I1].symnum,m[I1].T,V);
		printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J1+1,
		                               lAtom[J1].name,lAtom[J1].simb,U.x,U.y,U.z,
									   m[I1].T[0],m[I1].T[1],m[I1].T[2],line);
		
		sprintf(line,"");
		get_symmetry_string(m[I2].symnum, line);
		V.x = lAtom[J2].x; V.y = lAtom[J2].y; V.z = lAtom[J2].z;
		U = make_symmetry(m[I2].symnum,m[I2].T,V);
		printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J2+1,
		                               lAtom[J2].name,lAtom[J2].simb,U.x,U.y,U.z,
									   m[I2].T[0],m[I2].T[1],m[I2].T[2],line);

		glutPostRedisplay();
	}
}


/**************************************** calcAtomAngle() *******************/
void calcAtomAngle()
{
	int I1,I2,I3;
	int J1,J2,J3;
	VECTOR3D V1,V2,V3,EV1,EV2,EV3;
	double esd,fi;
	int iesd,ID;
	char line[80];
	VECTOR3D V,U;

	I1 = pick_obj[0] - 1;
	I2 = pick_obj[1] - 1;
	I3 = pick_obj[2] - 1;
	J1 = m[I1].Num;
	J2 = m[I2].Num;
	J3 = m[I3].Num;
	V1.x = m[I1].x; V1.y = m[I1].y; V1.z = m[I1].z;
	V2.x = m[I2].x; V2.y = m[I2].y; V2.z = m[I2].z;
	V3.x = m[I3].x; V3.y = m[I3].y; V3.z = m[I3].z;

	EV1.x = lAtom[J1].ex; EV1.y = lAtom[J1].ey; EV1.z = lAtom[J1].ez;
	EV2.x = lAtom[J2].ex; EV2.y = lAtom[J2].ey; EV2.z = lAtom[J2].ez;
	EV3.x = lAtom[J3].ex; EV3.y = lAtom[J3].ey; EV3.z = lAtom[J3].ez;

	calc_angle_esd2(V1,V2,V3,EV1,EV2,EV3,cCell.a,cCell.ea,cCell.max_P,fi,esd);

	if ((esd >= 0.0005) && (esd <= 0.001)) esd = 0.001;

	if (esd < 0.001) ID = 4;
	if ((esd >= 0.001) && (esd < 0.010)) {iesd = int(esd*1000); ID = 0;}

	if ((esd >= 0.010) && (esd < 0.015)) {iesd = int(esd*1000); ID = 0;}
	if ((esd >= 0.015) && (esd < 0.024)) {iesd = 2; ID = 1;}
	if ((esd >= 0.025) && (esd < 0.035)) {iesd = 3; ID = 1;}
	if ((esd >= 0.035) && (esd < 0.045)) {iesd = 4; ID = 1;}
	if ((esd >= 0.045) && (esd < 0.055)) {iesd = 5; ID = 1;}
	if ((esd >= 0.055) && (esd < 0.065)) {iesd = 6; ID = 1;}
	if ((esd >= 0.065) && (esd < 0.075)) {iesd = 7; ID = 1;}
	if ((esd >= 0.075) && (esd < 0.085)) {iesd = 8; ID = 1;}
	if ((esd >= 0.085) && (esd < 0.095)) {iesd = 9; ID = 1;}

	if ((esd >= 0.095) && (esd < 0.100)) {iesd = 10; ID = 21;}
	if ((esd >= 0.100) && (esd < 0.150)) {iesd = int(esd*1000)/10; ID = 21;}


	if ((esd >= 0.150) && (esd < 0.250)) {iesd = 2; ID = 2;}
	if ((esd >= 0.250) && (esd < 0.350)) {iesd = 3; ID = 2;}
	if ((esd >= 0.350) && (esd < 0.450)) {iesd = 4; ID = 2;}
	if ((esd >= 0.450) && (esd < 0.550)) {iesd = 5; ID = 2;}
	if ((esd >= 0.550) && (esd < 0.650)) {iesd = 6; ID = 2;}
	if ((esd >= 0.650) && (esd < 0.750)) {iesd = 7; ID = 2;}
	if ((esd >= 0.750) && (esd < 0.850)) {iesd = 8; ID = 2;}
	if ((esd >= 0.850) && (esd < 0.999)) {iesd = 9; ID = 2;}

	if(esd >= 0.999) ID = 3;

	switch(ID){
	case(0):{
		sprintf(infotext,"phi(%s-%s-%s) =%8.3f(%i) deg",lAtom[J1].name, lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nphi(%s-%s-%s) =%8.3f(%i) deg\n",lAtom[J1].name,lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		break;
			}
	case(1):{
		sprintf(infotext,"phi(%s-%s-%s) =%7.2f(%i) deg",lAtom[J1].name, lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nphi(%s-%s-%s) =%7.2f(%i) deg\n",lAtom[J1].name,lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		break;
			}
	case(21):{
		sprintf(infotext,"phi(%s-%s-%s) =%7.2f(%i) deg",lAtom[J1].name, lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nphi(%s-%s-%s) =%7.2f(%i) deg\n",lAtom[J1].name,lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		break;
			}
	case(2):{
		sprintf(infotext,"phi(%s-%s-%s) =%6.1f(%i) deg",lAtom[J1].name, lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nphi(%s-%s-%s) =%6.1f(%i) deg\n",lAtom[J1].name,lAtom[J2].name, lAtom[J3].name ,fi,iesd);
		break;
			}
	case(3):{
		sprintf(infotext,"phi(%s-%s-%s) =%6.1f(>1.0) deg",lAtom[J1].name, lAtom[J2].name, lAtom[J3].name ,fi);
		statusbarinfo->set_name(infotext);
		printf("\nphi(%s-%s-%s) =%6.1f(>1.0) deg\n",lAtom[J1].name,lAtom[J2].name, lAtom[J3].name ,fi);
		break;
			}
	case(4):{
		sprintf(infotext,"phi(%s-%s-%s) =%8.3f(0) deg",lAtom[J1].name, lAtom[J2].name, lAtom[J3].name ,fi);
		statusbarinfo->set_name(infotext);
		printf("\nphi(%s-%s-%s) =%8.3f(0) deg\n",lAtom[J1].name,lAtom[J2].name, lAtom[J3].name ,fi);
		break;
			}
	}
	sprintf(line,"");
	get_symmetry_string(m[I1].symnum, line);
	V.x = lAtom[J1].x; V.y = lAtom[J1].y; V.z = lAtom[J1].z;
	U = make_symmetry(m[I1].symnum,m[I1].T,V);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J1+1,
	                               lAtom[J1].name,lAtom[J1].simb,U.x,U.y,U.z,
								   m[I1].T[0],m[I1].T[1],m[I1].T[2],line);
	sprintf(line,"");
	V.x = lAtom[J2].x; V.y = lAtom[J2].y; V.z = lAtom[J2].z;
	U = make_symmetry(m[I2].symnum,m[I2].T,V);
	get_symmetry_string(m[I2].symnum, line);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J2+1,
	                               lAtom[J2].name,lAtom[J2].simb,U.x,U.y,U.z,
								   m[I2].T[0],m[I2].T[1],m[I2].T[2],line);
	sprintf(line,"");
	V.x = lAtom[J3].x; V.y = lAtom[J3].y; V.z = lAtom[J3].z;
	U = make_symmetry(m[I3].symnum,m[I3].T,V);
	get_symmetry_string(m[I3].symnum, line);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J3+1,
	                               lAtom[J3].name,lAtom[J3].simb,U.x,U.y,U.z,
								   m[I3].T[0],m[I3].T[1],m[I3].T[2],line);

	glutPostRedisplay();
}

/**************************************** calcAtomTorsAngle() ***************/
void calcAtomTorsAngle()
{
	int I1,I2,I3,I4;
	int J1,J2,J3,J4;
	VECTOR3D V1,V2,V3,V4,EV1,EV2,EV3,EV4;
	VECTOR3D V,U;
	double esd;
	int iesd,ID;
	double fi;
	char line[80];

	I1 = pick_obj[0] - 1;
	I2 = pick_obj[1] - 1;
	I3 = pick_obj[2] - 1;
	I4 = pick_obj[3] - 1;
	J1 = m[I1].Num;
	J2 = m[I2].Num;
	J3 = m[I3].Num;
	J4 = m[I4].Num;
	V1.x = m[I1].x; V1.y = m[I1].y; V1.z = m[I1].z;
	V2.x = m[I2].x; V2.y = m[I2].y; V2.z = m[I2].z;
	V3.x = m[I3].x; V3.y = m[I3].y; V3.z = m[I3].z;
	V4.x = m[I4].x; V4.y = m[I4].y; V4.z = m[I4].z;

	EV1.x = lAtom[J1].ex; EV1.y = lAtom[J1].ey; EV1.z = lAtom[J1].ez;
	EV2.x = lAtom[J2].ex; EV2.y = lAtom[J2].ey; EV2.z = lAtom[J2].ez;
	EV3.x = lAtom[J3].ex; EV3.y = lAtom[J3].ey; EV3.z = lAtom[J3].ez;
	EV4.x = lAtom[J4].ex; EV4.y = lAtom[J4].ey; EV4.z = lAtom[J4].ez;

	calc_tors_esd2(V1,V2,V3,V4,EV1,EV2,EV3,EV4,cCell.a,cCell.ea,cCell.max_P,fi,esd);
	if ((esd >= 0.0005) && (esd <= 0.001)) esd = 0.001;

	if (esd < 0.001) ID = 4;
	if ((esd >= 0.001) && (esd < 0.010)) {iesd = int(esd*1000); ID = 0;}

	if ((esd >= 0.010) && (esd < 0.015)) {iesd = int(esd*1000); ID = 0;}
	if ((esd >= 0.015) && (esd < 0.024)) {iesd = 2; ID = 1;}
	if ((esd >= 0.025) && (esd < 0.035)) {iesd = 3; ID = 1;}
	if ((esd >= 0.035) && (esd < 0.045)) {iesd = 4; ID = 1;}
	if ((esd >= 0.045) && (esd < 0.055)) {iesd = 5; ID = 1;}
	if ((esd >= 0.055) && (esd < 0.065)) {iesd = 6; ID = 1;}
	if ((esd >= 0.065) && (esd < 0.075)) {iesd = 7; ID = 1;}
	if ((esd >= 0.075) && (esd < 0.085)) {iesd = 8; ID = 1;}
	if ((esd >= 0.085) && (esd < 0.095)) {iesd = 9; ID = 1;}

	if ((esd >= 0.095) && (esd < 0.100)) {iesd = 10; ID = 21;}
	if ((esd >= 0.100) && (esd < 0.150)) {iesd = int(esd*1000)/10; ID = 21;}
	if ((esd >= 0.150) && (esd < 0.250)) {iesd = 2; ID = 2;}
	if ((esd >= 0.250) && (esd < 0.350)) {iesd = 3; ID = 2;}
	if ((esd >= 0.350) && (esd < 0.450)) {iesd = 4; ID = 2;}
	if ((esd >= 0.450) && (esd < 0.550)) {iesd = 5; ID = 2;}
	if ((esd >= 0.550) && (esd < 0.650)) {iesd = 6; ID = 2;}
	if ((esd >= 0.650) && (esd < 0.750)) {iesd = 7; ID = 2;}
	if ((esd >= 0.750) && (esd < 0.850)) {iesd = 8; ID = 2;}
	if ((esd >= 0.850) && (esd < 0.999)) {iesd = 9; ID = 2;}

	if(esd >= 0.999) ID = 3;

	switch(ID){
	case(0):{
		sprintf(infotext,"omega(%s-%s-%s-%s) =%8.3f(%i) deg",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nomega(%s-%s-%s-%s) =%8.3f(%i) deg\n",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name ,fi,iesd);
		break;
			}
	case(1):{
		sprintf(infotext,"omega(%s-%s-%s-%s) =%7.2f(%i) deg",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nomega(%s-%s-%s-%s) =%7.2f(%i) deg\n",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name ,fi,iesd);
		break;
			}
	case(21):{
		sprintf(infotext,"omega(%s-%s-%s-%s) =%7.2f(%i) deg",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nomega(%s-%s-%s-%s) =%7.2f(%i) deg\n",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name ,fi,iesd);
		break;
			}
	case(2):{
		sprintf(infotext,"omega(%s-%s-%s-%s) =%6.1f(%i) deg",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name,fi,iesd);
		statusbarinfo->set_name(infotext);
		printf("\nomega(%s-%s-%s-%s) =%6.1f(%i) deg\n",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name ,fi,iesd);
		break;
			}
	case(3):{
		sprintf(infotext,"omega(%s-%s-%s-%s) =%6.1f(>1.0) deg",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name,fi);
		statusbarinfo->set_name(infotext);
		printf("\nomega(%s-%s-%s-%s) =%6.1f(<1.0) deg\n",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name ,fi);
		break;
			}
	case(4):{
		sprintf(infotext,"omega(%s-%s-%s-%s) =%8.3f(0) deg",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name,fi);
		statusbarinfo->set_name(infotext);
		printf("\nomega(%s-%s-%s-%s) =%8.3f(0) deg\n",
			    lAtom[J1].name, lAtom[J2].name, lAtom[J3].name, lAtom[J4].name ,fi);
		break;
			}
	}
	sprintf(line,"");
	get_symmetry_string(m[I1].symnum, line);
	V.x = lAtom[J1].x; V.y = lAtom[J1].y; V.z = lAtom[J1].z;
	U = make_symmetry(m[I1].symnum,m[I1].T,V);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J1+1,
	                     lAtom[J1].name,lAtom[J1].simb,U.x,U.y,U.z,
				         m[I1].T[0],m[I1].T[1],m[I1].T[2],line);
	sprintf(line,"");
	V.x = lAtom[J2].x; V.y = lAtom[J2].y; V.z = lAtom[J2].z;
	U = make_symmetry(m[I2].symnum,m[I2].T,V);
	get_symmetry_string(m[I2].symnum, line);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J2+1,
	                               lAtom[J2].name,lAtom[J2].simb,U.x,U.y,U.z,
								   m[I2].T[0],m[I2].T[1],m[I2].T[2],line);
	sprintf(line,"");

	V.x = lAtom[J3].x; V.y = lAtom[J3].y; V.z = lAtom[J3].z;
	U = make_symmetry(m[I3].symnum,m[I3].T,V);
	get_symmetry_string(m[I3].symnum, line);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J3+1,
	                               lAtom[J3].name,lAtom[J3].simb,U.x,U.y,U.z,
								   m[I3].T[0],m[I3].T[1],m[I3].T[2],line);
	sprintf(line,"");

	V.x = lAtom[J4].x; V.y = lAtom[J4].y; V.z = lAtom[J4].z;
	U = make_symmetry(m[I4].symnum,m[I4].T,V);
	get_symmetry_string(m[I4].symnum, line);
	printf("%4i %7s%3s %8.5f %8.5f %8.5f (%2i,%2i,%2i)+ %s\n",J4+1,
	                               lAtom[J4].name,lAtom[J4].simb,U.x,U.y,U.z,
								   m[I4].T[0],m[I4].T[1],m[I4].T[2],line);


	glutPostRedisplay();
}

/**************************************** draw_string() *********************/
void draw_string(void *font, const char* string) 
{
  while(*string)
    glutStrokeCharacter(font, *string++);
}

/**************************************** draw_string_bitmap() **************/
void draw_string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}

/**************************************** print_out_result() ****************/
void print_out_result()
{
	int i;
	printf("\n");
	printf("==================================================================\n");
	printf("Title               %s\n",Title);
	printf("\n");
	printf("Lattice type        %c\n",cCell.lType);
	printf("Space group name    %s\n",cCell.SpGr);
	printf("Space group number  %i\n",cCell.SpGrN);
	if ((cCell.SpGrN == 48) || (cCell.SpGrN == 50) || (cCell.SpGrN == 59) || 
		(cCell.SpGrN == 68) || (cCell.SpGrN == 70)) {
		if (cCell.Setting <= 6) printf("Setting number      %i (origin 1)\n",cCell.Setting);
		else printf("Setting number      %i (origin 2)\n",cCell.Setting - 6);
	}
	else printf("Setting number      %i\n",cCell.Setting);
	printf("\n");
	printf("Lattice parameters\n");
	printf("\n");
	printf("%4s %8s %8s %11s %7s %9s\n","a","b","c","alpha","beta","gamma");
	printf("%8.5f %8.5f %8.5f %8.4f %8.4f %8.4f\n",cCell.a[0],cCell.a[1],cCell.a[2],
		                                            cCell.a[3],cCell.a[4],cCell.a[5]);
	printf("\n");
	printf("Structure parameters\n");
	printf("\n");
	if (use_iso == 1) printf("%22s %10s %10s %10s %10s\n","x","y","z","g","Uiso");
	else printf("%22s %10s %10s %10s %10s\n","x","y","z","g","Biso");
	for(i=0; i<nAtom; i++){
		printf("%4i %3s %6s %10.5f %10.5f %10.5f %8.3f %8.3f\n",i+1,lAtom[i].simb,lAtom[i].name,
			            lAtom[i].x,lAtom[i].y,lAtom[i].z,lAtom[i].g,lAtom[i].Biso);
	}
	printf("==================================================================\n");
	printf("\n");

}


/**************************************** output_object_info() **************/
void output_object_info()
{
	int i,I1,I2,I3,J1,J2;
	int mode,N;
	char line[80];
	VECTOR3D V,U;
	float PV,pEL,pAV;

	N = pick_obj[0];
	if (N <= nXYZ) mode = 1;
	if ((N > nXYZ) && (N <= (nXYZ+nBonds))) mode = 2;
	if ((N > nXYZ+nBonds) && (N <= (nXYZ+nBonds+nPoly))) mode = 3;

	switch(mode){
	case(1):{      //--- Atoms
		I1 = m[N - 1].Num;
		V.x = lAtom[I1].x; V.y = lAtom[I1].y; V.z = lAtom[I1].z;
		U = make_symmetry(m[N - 1].symnum,m[N - 1].T,V);
		printf("\nATOM: %i %s%3s %8.5f %8.5f %8.5f  ",I1+1,lAtom[I1].name,lAtom[I1].simb,
			     U.x,U.y,U.z);
		sprintf(line,"");
		get_symmetry_string(m[N-1].symnum, line);
		printf("(%2i,%2i,%2i)+ %s\n",m[N-1].T[0],m[N-1].T[1],m[N-1].T[2],line);
		sprintf(infotext,"%s ( %s ) %8.5f %8.5f %8.5f",lAtom[I1].name,lAtom[I1].simb,
		                  U.x,U.y,U.z);
		statusbarinfo->set_name(infotext);
	    glutPostRedisplay();
		break;
			}
	case(2):{      //--- Bonds
		calcAtomDist(1);
		break;
			}
	case(3):{      //--- Polyhedra
		I1 = N - nXYZ - nBonds - 1;
		I2 = poly_list[I1];
		J1 = m[I2].Num;

		V.x = lAtom[J1].x; V.y = lAtom[J1].y; V.z = lAtom[J1].z;
		U = make_symmetry(m[I2].symnum,m[I2].T,V);
		printf("\nPOLYHEDRON:\n");
		printf("%4i %7s%3s %8.5f %8.5f %8.5f  ",J1+1,lAtom[J1].name,lAtom[J1].simb,U.x,U.y,U.z);
		sprintf(line,"");
		get_symmetry_string(m[I2].symnum, line);
		printf("(%2i,%2i,%2i)+ %s\n",m[I2].T[0],m[I2].T[1],m[I2].T[2],line);

	printf("----------------------------------------------------------------------------\n");
		for(i=0; i<m[I2].bondNum; i++){
			I3 = m[I2].b_list[i];
			J2 = m[I3].Num;
			V.x = lAtom[J2].x; V.y = lAtom[J2].y; V.z = lAtom[J2].z;
			U = make_symmetry(m[I3].symnum,m[I3].T,V);
			printf("%4i %7s%3s %8.5f %8.5f %8.5f  ",J2+1,lAtom[J2].name,lAtom[J2].simb,U.x,U.y,U.z);
			sprintf(line,"");
			get_symmetry_string(m[I3].symnum, line);
			printf("(%2i,%2i,%2i)+ %s\n",m[I3].T[0],m[I3].T[1],m[I3].T[2],line);
		}
	printf("----------------------------------------------------------------------------\n");
		for(i=0; i<m[I2].bondNum; i++){
			calcAtomDist(i+2);
		}
		PV = _calc_Poly_Vol(I1);
		if (PV > 0) printf("Volume =%8.4f Angstrom^3\n",PV);
		pEL = _calc_Poly_elong(I1,PV);
		if (pEL > 0) {
			printf("Quadratic elongation =%8.4f\n",pEL);
			pAV = _calc_Poly_var(I1, PV);
			if (pAV > 0) printf("Bond angle variance =%9.4f deg^2\n",pAV);
		}
		break;
			}
	}

}


/**************************************** output_poly_and_BVS_info() ********/
void output_poly_and_BVS_info()
{
	int i,I1,I2,I3,J1,J2;
	double x1,y1,z1,x2,y2,z2;
	double D[MAX_BONDS_FOR_ATOM];
	double BVS;
	float BVP;
	double B;
	int N,result;
	char line[80];
	float Onum;
	float Lav;
	VECTOR3D V,U; 
	float PV,pEL,pAV;

	B = 0.37;

	N = pick_obj[0];
	I1 = N - nXYZ - nBonds - 1;
	I2 = poly_list[I1];
	J1 = m[I2].Num;
	V.x = lAtom[J1].x; V.y = lAtom[J1].y; V.z = lAtom[J1].z;
	U = make_symmetry(m[I2].symnum,m[I2].T,V);
	printf("\nPOLYHEDRON:\n");
	printf("%4i %7s%3s %8.5f %8.5f %8.5f  ",J1+1,lAtom[J1].name,lAtom[J1].simb,U.x,U.y,U.z);
	sprintf(line,"");
	get_symmetry_string(m[I2].symnum, line);
	printf("(%2i,%2i,%2i)+ %s\n",m[I2].T[0],m[I2].T[1],m[I2].T[2],line);
	printf("----------------------------------------------------------------------------\n");
	for(i=0; i<m[I2].bondNum; i++){
		I3 = m[I2].b_list[i];
		J2 = m[I3].Num;
		V.x = lAtom[J2].x; V.y = lAtom[J2].y; V.z = lAtom[J2].z;
		U = make_symmetry(m[I3].symnum,m[I2].T,V);
		printf("%4i %7s%3s %8.5f %8.5f %8.5f  ",J2+1,lAtom[J2].name,lAtom[J2].simb,
			                               U.x,U.y,U.z);
		sprintf(line,"");
		get_symmetry_string(m[I3].symnum, line);
		printf("(%2i,%2i,%2i)+ %s\n",m[I3].T[0],m[I3].T[1],m[I3].T[2],line);
	}
	printf("----------------------------------------------------------------------------\n");
	for(i=0; i<m[I2].bondNum; i++){
		calcAtomDist(i+2);
		I3 = m[I2].b_list[i];
		J2 = m[I3].Num;
		x1 = m[I2].x*cCell.max_P; x2 = m[I3].x*cCell.max_P;
		y1 = m[I2].y*cCell.max_P; y2 = m[I3].y*cCell.max_P;
		z1 = m[I2].z*cCell.max_P; z2 = m[I3].z*cCell.max_P;
		D[i] = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
	}
	PV = _calc_Poly_Vol(I1);
	if (PV > 0) printf("Volume =%8.4f Angstrom^3\n",PV);
	pEL = _calc_Poly_elong(I1,PV);
	if (pEL > 0) {
		printf("Quadratic elongation =%8.4f\n",pEL);
		pAV = _calc_Poly_var(I1, PV);
		if (pAV > 0) printf("Bond angle variance =%9.4f deg^2\n",pAV);
	}
	printf("------------------------------------------\n");
   	BVS = 0;
	printf("Input a bond valence parameter: ");
	result = scanf("%f", &BVP);
	for(i=0; i<m[I2].bondNum; i++){
		BVS = BVS + exp((BVP - D[i])/B);
	}
	printf("Bond valence sum =%6.3f\n",BVS);
	printf("Input an oxidation number [0 for quit]: ");
	result = scanf("%f", &Onum);
	printf("\n");
	if (Onum != 0){
		Lav = BVP - B*log(Onum/m[I2].bondNum);
		printf("Expected bond length =%6.3f Angstrom\n",Lav);
	}


}


/**************************************** create_Cell_List() ****************/
void set_spin_for_atom(int N)
{
	spin_dialog_ID = -1;
	set_spin_dialog_param(N-1, m[N-1].show_spin, spin_H, m[N-1].spin_dir, spin_RGB);
	open_spin_dialog(main_window,dwp[0][11],dwp[1][11]);
}

/**************************************** create_Cell_List() ****************/
void create_Cell_List()
{
	int i;
	glNewList(1,GL_COMPILE);
	if(SHCL == 1) {
		glDisable( GL_LIGHTING );
		glLineWidth(ucell_width);
		glColor3d(ucell_color[0],ucell_color[1],ucell_color[2]);
		switch(ucell_type)
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
		glBegin(GL_LINE_LOOP);
		  for(i=0;i<8;i++) glVertex3d(uCell[i][0],uCell[i][1],uCell[i][2]);
		glEnd();
		glBegin(GL_LINES);
		  glVertex3d(uCell[0][0],uCell[0][1],uCell[0][2]);
		  glVertex3d(uCell[3][0],uCell[3][1],uCell[3][2]);
		  glVertex3d(uCell[2][0],uCell[2][1],uCell[2][2]);
		  glVertex3d(uCell[5][0],uCell[5][1],uCell[5][2]);
		  glVertex3d(uCell[1][0],uCell[1][1],uCell[1][2]);
		  glVertex3d(uCell[6][0],uCell[6][1],uCell[6][2]);
		  glVertex3d(uCell[7][0],uCell[7][1],uCell[7][2]);
		  glVertex3d(uCell[4][0],uCell[4][1],uCell[4][2]);
		glEnd();
		glEnable( GL_LIGHTING );
		if (ucell_type > 0) glDisable (GL_LINE_STIPPLE);
	}
	glEndList();
}

/**************************************** drawEllipse() *********************/
void drawVacancy(float R, int Slice, int Stack, float g)
{
	int Slice2,S0;
	double alpha,fi;
	double x,z,y;
	GLUquadricObj *quadObj;
	int i,j,k;

	Slice2 = Slice / 2;
	S0 = (int)(Slice2 - Slice*g);
	S0 = 0;
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_FLAT);
	gluQuadricOrientation(quadObj, GLU_OUTSIDE);
	gluQuadricOrientation(quadObj, GLU_INSIDE);
	glBegin(GL_QUAD_STRIP);
	for(i=S0; i<Slice2; i++){
		for(j=0; j<=Stack; j++){
			for(k=0; k<2; k++){
				alpha = (float)(i+k)/(float)Slice2;
				fi = (float)j/(float)Stack;
				y = R * sin(PI*alpha/2); 
				x = R * cos(PI*alpha/2) * sin(2*PI*fi);
				z = R * cos(PI*alpha/2) * cos(2*PI*fi);
				glNormal3f(-x,-y,-z);
				glVertex3f(x,y,z);
			}
		}
	}
	glEnd();
	gluDeleteQuadric(quadObj);
	glColor3f(1,1,1);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_FLAT);
	gluQuadricOrientation(quadObj, GLU_OUTSIDE);
	gluQuadricOrientation(quadObj, GLU_INSIDE);
	glBegin(GL_QUAD_STRIP);
	for(i=-Slice2; i<S0; i++){
		for(j=0; j<=Stack; j++){
			for(k=0; k<2; k++){
				alpha = (float)(i+k)/(float)Slice2;
				fi = (float)j/(float)Stack;
				y = R * sin(PI*alpha/2); 
				x = R * cos(PI*alpha/2) * sin(2*PI*fi);
				z = R * cos(PI*alpha/2) * cos(2*PI*fi);
				glNormal3f(-x,-y,-z);
				glVertex3f(x,y,z);
			}
		}
	}
	glEnd();
	gluDeleteQuadric(quadObj);
	glLineWidth(1);
}


/**************************************** drawDotSphere() ********************/
void drawDotSphere(float R, int Slice, int Stack)
{
	GLUquadricObj *quadObj;
	glDisable(GL_LIGHTING);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_POINT);
	gluSphere(quadObj, R, 2*Slice, 2*Stack);
	gluDeleteQuadric(quadObj);
	glEnable(GL_LIGHTING);
}


/**************************************** drawSpinAtom() ********************/
void drawSolidSpin(float R, int Slice, int Stack, float H, float Color[3], float Dir[3])
{
	double cf,fi;
	double V[3];
	GLUquadricObj *quadObj;
	float s_scale;
	float Spin_R;

	if ((Dir[0] == 0) && (Dir[1] == 0) && (Dir[2] == 0)) return;
	if (H == 0) return;

	if ((Dir[0] == 0) && (Dir[1] == 0) && (Dir[2] == 1)) {
		fi = 0; 
		V[0] = 0; V[1] = -1; V[2] = 0;
	}
	else if ((Dir[0] == 0) && (Dir[1] == 0) && (Dir[2] == -1)) {
		fi = 180; 
		V[0] = 0; V[1] = -1; V[2] = 0;
	}
	else {
		cf = Dir[2]/sqrt(Dir[0]*Dir[0] + Dir[1]*Dir[1] + Dir[2]*Dir[2]);
		fi = acos(cf) * 180 / 3.14159;
		V[0] = -Dir[1];
		V[1] = Dir[0];
		V[2] = 0;
	}

	s_scale = 0.4/cCell.max_P;
	Spin_R = 0.15 * R * s_scale;
	H = (H + R) * s_scale;

	glColor3f(Color[0],Color[1],Color[2]);
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	glPushMatrix();
	   glRotatef(fi, V[0],V[1],V[2]);
	   gluCylinder(quadObj,Spin_R, Spin_R, H, Slice, Stack); 
       glTranslatef(0,0,H);
	   glutSolidCone(2.0*Spin_R,0.3*H,Slice, Stack);
	glPopMatrix();
	gluDeleteQuadric(quadObj);

}



/**************************************** drawEllipse() *********************/
void drawEllipse(float a1, float b1, float c1, int IP, int Slice, int Stack)
{
	int i,j,k;
	int Slice2;
	double alpha,fi;
	double x,z,y;
	GLUquadricObj *quadObj;
	float ar_scale;
	float R;
	double a,b,c;
	float Q[] = {0.3389,0.4299,0.4951,0.5479,0.5932,0.6334,0.6699,0.7035,0.7349,0.7644,
		         0.7924,0.8192,0.8447,0.8694,0.8932,0.9162,0.9386,0.9605,0.9818,1.0026,
				 1.0230,1.0430,1.0627,1.0821,1.1012,1.1200,1.1386,1.1570,1.1751,1.1932,
				 1.2110,1.2288,1.2464,1.2638,1.2812,1.2985,1.3158,1.3330,1.3501,1.3672,
				 1.3842,1.4013,1.4183,1.4354,1.4524,1.4695,1.4866,1.5037,1.5209,1.5382,
				 1.5555,1.5729,1.5904,1.6080,1.6257,1.6436,1.6616,1.6797,1.6980,1.7164,
				 1.7351,1.7540,1.7730,1.7924,1.8119,1.8318,1.8519,1.8724,1.8932,1.9144,
				 1.9360,1.9580,1.9804,2.0034,2.0269,2.0510,2.0757,2.1012,2.1274,2.1544,
				 2.1824,2.2114,2.2416,2.2730,2.3059,2.3404,2.3767,2.4153,2.4563,2.5003,
				 2.5478,2.5997,2.6571,2.7216,2.7955,2.8829,2.9912,3.1365,3.3682,5.9503};

	ar_scale = 1.0/cCell.max_P;
	R = Q[IP];
	R = R*ar_scale;
	a = a1*R;
	b = b1*R;
	c = c1*R;

	if(TEOL == 1) {
		glDisable( GL_LIGHTING );
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		for(j=0; j<=Stack; j++){
			fi = (float)j/(float)Stack;
			x = a * sin(2*PI*fi);
			z = c * cos(2*PI*fi);
			glVertex3f(x,0,z);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(j=0; j<=Slice; j++){
			fi = (float)j/(float)Slice;
			y = b * sin(2*PI*fi);
			z = c * cos(2*PI*fi);
			glVertex3f(0,y,z);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(j=0; j<=Slice; j++){
			fi = (float)j/(float)Slice;
			x = a * sin(2*PI*fi);
			y = b * cos(2*PI*fi);
			glVertex3f(x,y,0);
		}
		glEnd();
		glEnable( GL_LIGHTING );
	}
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
//	gluQuadricNormals(quadObj, GLU_FLAT);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluQuadricOrientation(quadObj, GLU_OUTSIDE);
	gluQuadricOrientation(quadObj, GLU_INSIDE);
	Slice2 = Slice / 2;
	glBegin(GL_QUAD_STRIP);
	for(i=-Slice2; i<Slice2; i++){
		for(j=0; j<=Stack; j++){
			for(k=0; k<2; k++){
				alpha = (float)(i+k)/(float)Slice2;
				fi = (float)j/(float)Stack;
				y = b * sin(PI*alpha/2); 
				x = a * cos(PI*alpha/2) * sin(2*PI*fi);
				z = c * cos(PI*alpha/2) * cos(2*PI*fi);
				glNormal3f(-x,-y,-z);
				glVertex3f(x,y,z);
			}
		}
	}
	glEnd();
	gluDeleteQuadric(quadObj);
	glLineWidth(1);
}




/**************************************** CreatAtomList() *******************/
void CreatAtomList()
{
	int i,j,objID,res;
	int I1,iH;
	float R;
	float ar_scale;
	float dot_scale;


	if (radii_type != 2){
		if (model_mode == 1) ar_scale = 1.3/cCell.max_P;
		else ar_scale = 0.4/cCell.max_P;
		dot_scale = 13/4;
	}
	else {
		if (model_mode == 1) ar_scale = 1/cCell.max_P;
		else ar_scale = 0.25/cCell.max_P;
		dot_scale = 10/2.5f;
	}

	glNewList(2,GL_COMPILE);

	for(i=0; i<nXYZ; i++){
		objID = 0;
		for (j=0; j<objNum; j++){if (i+1 == pick_obj[j]) {objID = 1; break;}}
		if (m[i].show=='y'){
			I1 = m[i].Num;
			iH = 0;
			if ((strstr(lAtom[I1].simb, "H") != NULL) && (strlen(lAtom[I1].simb) == 1)) iH = 1;
			if ((strstr(lAtom[I1].simb, "D") != NULL) && (strlen(lAtom[I1].simb) == 1)) iH = 1;
			R = ar_scale*lAtom[I1].R;
			glColor3f(lAtom[I1].color[0],lAtom[I1].color[1],lAtom[I1].color[2]);
			glPushMatrix();
			glTranslatef(m[i].x,m[i].y,m[i].z);
			if (objID == 1) {
				glDisable(GL_LIGHTING);
			}
			switch(model_mode){
//------------------------------------------------------------------ B&S Model
			case(0):{
				glLoadName(i+1);
				if ((use_aniso == 1) && (atom_type == 1)) {
					if (iH == 0) {
						res = make_th_ellips(i);
						glPushMatrix();
						glMultMatrixf(termMatrix);
						drawEllipse(ellAtom[I1].axis[0],ellAtom[I1].axis[1],ellAtom[I1].axis[2], 
					                Probability,atoms_slice,atoms_stack);
						glPopMatrix();
					}
					else {glutSolidSphere(R,atoms_slice,atoms_stack);}
				}
				else {
					if (lAtom[I1].g >= 1) {
						glutSolidSphere(R,atoms_slice,atoms_stack);
					}
					else drawVacancy(R,atoms_slice,atoms_stack,lAtom[I1].g);
				}
				if (m[i].show_spin == 1) drawSolidSpin(lAtom[I1].R,atoms_slice,atoms_stack,spin_H,spin_RGB,m[i].spin_dir);
				if (dot_surf_ID == 1) drawDotSphere(R*dot_scale,atoms_slice,atoms_stack);
				break;
					}
//------------------------------------------------------------------ Space Filling Model
			case(1):{
				glLoadName(i+1);
				glutSolidSphere(R,atoms_slice,atoms_stack);
				break;
					}
//------------------------------------------------------------------ Polyhedra Model
			case(2):{
				if ((m[i].poly_show != 0) && (m[i].poly_show != 3)) {
					glLoadName(i+1);
					if ((use_aniso == 1) && (atom_type == 1)) {
						if (iH == 0) {
							res = make_th_ellips(i);
							glPushMatrix();
							glMultMatrixf(termMatrix);
							drawEllipse(ellAtom[I1].axis[0],ellAtom[I1].axis[1],ellAtom[I1].axis[2], 
					                    Probability,atoms_slice,atoms_stack);
							glPopMatrix();
						}
						else {glutSolidSphere(R,atoms_slice,atoms_stack);}
					}
					else {
						if (lAtom[I1].g >= 1) glutSolidSphere(R,atoms_slice,atoms_stack);
						else drawVacancy(R,atoms_slice,atoms_stack,lAtom[I1].g);
					}
					if (m[i].show_spin == 1) drawSolidSpin(lAtom[I1].R,atoms_slice,atoms_stack,spin_H,spin_RGB,m[i].spin_dir);
				}
				break;
					}
//------------------------------------------------------------------ Wire-frame Model
			case(3):{
				if (dot_surf_ID == 0) {
					if ((m[i].bondNum == 0) && (m[i].bondatom == 0)){
						glLoadName(i+1);
						glutWireSphere(R,atoms_slice,atoms_stack);
					}
				}
				else {
					glLoadName(i+1);
					if ((m[i].bondNum == 0) && (m[i].bondatom == 0)) glutWireSphere(R,atoms_slice,atoms_stack);
					drawDotSphere(R*dot_scale,atoms_slice,atoms_stack);
				}
				break;
					}
//------------------------------------------------------------------ Stick Model
			case(4):{
				if (dot_surf_ID == 0) {
					if ((m[i].bondNum == 0) && (m[i].bondatom == 0)){
						glLoadName(i+1);
						glutSolidSphere(R,atoms_slice,atoms_stack);
					}
				}
				else {
					glLoadName(i+1);
					if ((m[i].bondNum == 0) && (m[i].bondatom == 0)) glutSolidSphere(R,atoms_slice,atoms_stack);
					drawDotSphere(R*dot_scale,atoms_slice,atoms_stack);
				}
				break;
					}
			}
			glEnable(GL_LIGHTING);
			glPopMatrix();
		}
	}
	glEndList();
}

/**************************************** CreatBondList() *******************/
void CreatBondList()
{
	GLUquadricObj *quadObj;
	float dx,dy; 
	int i,j,I1,I2;
	float br_scale;
	float bond_R;
	int bond_enabled = 1;
	int control;
	int objID;
	int J1,J2;
	float ar_scale,R1,R2,dist1,dist2;
	int c_bonds_type;

	br_scale = 0.2/cCell.max_P;
	if (model_mode == 4) bond_R = br_scale*bRadii[1];
	else bond_R = br_scale*bRadii[0];
	ar_scale = 0.4/cCell.max_P;

	glNewList(3,GL_COMPILE);
	if(bond_enabled==1) {
		for(i=0;i<nBonds;i++){
			c_bonds_type = bonds_type;
			objID = 0;
			if (bAtom[i].show=='y'){
				for (j=0; j<objNum; j++){if (i+1+nXYZ == pick_obj[j]) {objID = 1; break;}}
				control = 1;
				if (model_mode == 2){                          //--- Polyhedra Mode
					I1 = bAtom[i].Atom1;
					I2 = m[I1].Num;
					if (lAtom[I2].CN > 2) {
						if ((m[I1].poly_show != 0) && (m[I1].poly_show != 2)) control = 1;
						else control = 0;
					}
				}
				if (control == 1) {
					if (objID == 1) glDisable(GL_LIGHTING);
					glLoadName(i+1+nXYZ);
//------------------------------------------- for Hydrogen bonds
					if ((hydrogen_bonds_id == 1) && (bAtom[i].hydr == 'h')) c_bonds_type = 4;
				switch(c_bonds_type){
//------------------------------------------------------------- 1 Color Cylinder
				case(0):{
					quadObj = gluNewQuadric();
					gluQuadricDrawStyle(quadObj, GLU_FILL);
					I1 = bAtom[i].Atom1;
					I2 = bAtom[i].Atom2;
					dx = m[I2].x - m[I1].x;
					dy = m[I2].y - m[I1].y;
					if (dx==0 && dy==0) {dx = 1; dy = 0;}
					glPushMatrix();
					glTranslated( m[I1].x,m[I1].y,m[I1].z);
					glRotated(bAtom[i].FI, -dy/bAtom[i].dist, dx/bAtom[i].dist, 0);
					glColor3f(BNCL[0],BNCL[1],BNCL[2]);
					gluCylinder(quadObj,bond_R, bond_R, bAtom[i].dist,bonds_slice,bonds_stack); 
					glPopMatrix();
					gluDeleteQuadric(quadObj);
					break;
						}
//------------------------------------------------------------- 2 Color Cylinder
				case(1):{
					quadObj = gluNewQuadric();
					gluQuadricDrawStyle(quadObj, GLU_FILL);
					I1 = bAtom[i].Atom1;
					I2 = bAtom[i].Atom2;
					J1 = m[I1].Num;
					J2 = m[I2].Num;
					R1 = ar_scale*lAtom[J1].R;
					R2 = ar_scale*lAtom[J2].R;
					dist1 = R1 + (bAtom[i].dist - R1 - R2)/2;
					dist2 = bAtom[i].dist - dist1;

					dx = m[I2].x - m[I1].x;
					dy = m[I2].y - m[I1].y;
					if (dx==0 && dy==0) {dx = 1; dy = 0;}
					glPushMatrix();
					glTranslated( m[I1].x,m[I1].y,m[I1].z);
					glRotated(bAtom[i].FI, -dy/bAtom[i].dist, dx/bAtom[i].dist, 0);
					if ((model_mode == 0) || (model_mode == 2)){
						glColor3f(lAtom[m[I1].Num].color[0],lAtom[m[I1].Num].color[1],lAtom[m[I1].Num].color[2]);
						gluCylinder(quadObj,bond_R, bond_R, dist1,bonds_slice,bonds_stack); 
						glTranslated( 0, 0, dist1);
						glColor3f(lAtom[m[I2].Num].color[0],lAtom[m[I2].Num].color[1],lAtom[m[I2].Num].color[2]);
						gluCylinder(quadObj,bond_R, bond_R, dist2,bonds_slice,bonds_stack); 
					}
					else {
						glColor3f(lAtom[m[I1].Num].color[0],lAtom[m[I1].Num].color[1],lAtom[m[I1].Num].color[2]);
						gluCylinder(quadObj,bond_R, bond_R, bAtom[i].dist/2,bonds_slice,bonds_stack); 
						glTranslated( 0, 0, bAtom[i].dist/2 );
						glColor3f(lAtom[m[I2].Num].color[0],lAtom[m[I2].Num].color[1],lAtom[m[I2].Num].color[2]);
						gluCylinder(quadObj,bond_R, bond_R, bAtom[i].dist/2,bonds_slice,bonds_stack); 
					}
					glPopMatrix();
					gluDeleteQuadric(quadObj);
					break;
						}
//------------------------------------------------------------- Color Line
				case(2):{
					glDisable(GL_LIGHTING);
					I1 = bAtom[i].Atom1;
					I2 = bAtom[i].Atom2;
					glLineWidth(bonds_line_width);
					glColor3f(BNCL[0],BNCL[1],BNCL[2]);
					glBegin(GL_LINES);
					  glVertex3f(m[I1].x,m[I1].y,m[I1].z);
					  glVertex3f(m[I2].x,m[I2].y,m[I2].z);
					glEnd();
					glLineWidth(1);
					glEnable(GL_LIGHTING);
					break;
						}
//------------------------------------------------------------- Gradient Color Line
				case(3):{
					glDisable(GL_LIGHTING);
					I1 = bAtom[i].Atom1;
					I2 = bAtom[i].Atom2;
					glLineWidth(bonds_line_width);
					glBegin(GL_LINES);
					  glColor3f(lAtom[m[I1].Num].color[0],lAtom[m[I1].Num].color[1],lAtom[m[I1].Num].color[2]);
					  glVertex3f(m[I1].x,m[I1].y,m[I1].z);
					  glColor3f(lAtom[m[I2].Num].color[0],lAtom[m[I2].Num].color[1],lAtom[m[I2].Num].color[2]);
					  glVertex3f(m[I2].x,m[I2].y,m[I2].z);
					glEnd();
					glLineWidth(1);
					glEnable(GL_LIGHTING);
					break;
						}
//------------------------------------------------------------- Hydrogen Bonds
				case(4):{
					glDisable(GL_LIGHTING);
					switch(hbond_line_type)
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
					I1 = bAtom[i].Atom1;
					I2 = bAtom[i].Atom2;
					glLineWidth(bonds_line_width);
					glColor3f(BNCL[0],BNCL[1],BNCL[2]);
					glBegin(GL_LINES);
					  glVertex3f(m[I1].x,m[I1].y,m[I1].z);
					  glVertex3f(m[I2].x,m[I2].y,m[I2].z);
					glEnd();
					glLineWidth(1);
					glEnable(GL_LIGHTING);
					if (hbond_line_type > 0) glDisable (GL_LINE_STIPPLE);
					break;
						}
				}
				glEnable(GL_LIGHTING);
				}
			}
		}
	}
	glEndList();
}

/**************************************** CreatPolyList() *******************/
void CreatePolyList()
{
	int i,j,k;
	int objID;
	float RGB[3];
	int I1;

	glNewList(4,GL_COMPILE);
//------------------------------- Draw Polyhedra
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (i=0; i<nPoly; i++){
		objID = 0;
		for (j=0; j<objNum; j++){if (i+1+nXYZ+nBonds == pick_obj[j]) {objID = 1; break;}}
		if (tp[i].show=='y'){
			if (objID == 1) {
				glDisable(GL_LIGHTING);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_BLEND);
			}
			I1 = m[poly_list[i]].Num;
			RGB[0] = lAtom[I1].color[0];
			RGB[1] = lAtom[I1].color[1];
			RGB[2] = lAtom[I1].color[2];
			glColor4f(RGB[0],RGB[1],RGB[2],poly_trans);
//			glColor4d(tp[i].color[0],tp[i].color[1],tp[i].color[2],poly_trans);
			glPushMatrix();
			glTranslatef(tp[i].center[0],tp[i].center[1],tp[i].center[2]);
			glLoadName(i+nXYZ+nBonds+1);
			for(j=0;j<tp[i].fN;j++){
				glBegin(GL_POLYGON);
				glNormal3f(tp[i].face[j].fVector[0],tp[i].face[j].fVector[1],tp[i].face[j].fVector[2]);
				for(k=0;k<tp[i].face[j].vN;k++) {
					glVertex3d(tp[i].face[j].v[k][0],tp[i].face[j].v[k][1],tp[i].face[j].v[k][2]);
				}
				glEnd();
			}
			glPopMatrix();
			glEnable(GL_LIGHTING);
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
		}
	}
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
//------------------------------- Draw Polyhedra Outline
	if (poly_outline == 1){
		glLoadName(nXYZ+nBonds+nPoly);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(poly_outline_width);
		for (i=0; i<nPoly; i++){
			if (tp[i].show=='y'){
				glPushMatrix();
				glTranslatef(tp[i].center[0],tp[i].center[1],tp[i].center[2]);
				for(j=0;j<tp[i].fN;j++){
					glColor3f(poly_line_color[0],poly_line_color[1],poly_line_color[2]);
					glBegin(GL_POLYGON);
					for(k=0;k<tp[i].face[j].vN;k++) {
						glVertex3d(tp[i].face[j].v[k][0],tp[i].face[j].v[k][1],tp[i].face[j].v[k][2]);
					}
					glEnd();
				}
				glPopMatrix();
				glEnable(GL_LIGHTING);
			}
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEndList();
}

/**************************************** create_Axis_List() ****************/
void create_Axis_List()
{
	int i;
	float d[3], cf, alpha[3];
	float colors[3][3];
	char Axis_label1[2],Axis_label2[2],Axis_label3[2];
	double bd;
	float label_color;


	colors[0][0] = 1;colors[0][1] = 0;colors[0][2] = 0;
	colors[1][0] = 0;colors[1][1] =	1;colors[1][2] = 0;
	colors[2][0] = 0;colors[2][1] = 0;colors[2][2] = 1;
	if((fileID == 2) || (fileID == 8) || (fileID == 10) || (fileID == 12) || 
	   (fileID == 13) || (fileID == 16) || (fileID == 18)){
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

	glNewList(5,GL_COMPILE);
	glLoadName(nXYZ+nBonds+nPoly+2);
	if (show_axis > 0){
//-------------------------------------- Draw Axis Cone and Labels
		for(i=0; i<3; i++){
			glPushMatrix();
			glRotatef(-alpha[i],uAxis[i][1],-uAxis[i][0],0);
			glTranslatef(0,0,d[i]);
			glColor3f(colors[i][0],colors[i][1],colors[i][2]);
			glutSolidCone(0.015,0.04,11,1);
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
		glColor3f( 0.9, 0.9, 0.9 );
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
}


/**************************************** create_HKL_list() *****************/
void create_HKL_list()
{
	int i;

	glNewList(6,GL_COMPILE);
	glLoadName(nXYZ+nBonds+nPoly+1);
	if (show_hkl==1){
//-------------------------------------------------- (hkl) Plane Outline
		if (show_hkl_out == 1){
			glLineWidth(hkl_out_width);
			glDisable( GL_LIGHTING );
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3d(hkl_rgb[0],hkl_rgb[1],hkl_rgb[2]);
			glBegin(GL_POLYGON);
			for(i=0; i<cPNum; i++)	glVertex3d(cPlane1[i][0],cPlane1[i][1],cPlane1[i][2]);
			glEnd();
			if ((origin_hkl == 0) && (show_hkl_inv==1)){
				glBegin(GL_POLYGON);
				for(i=0; i<cPNum; i++) glVertex3d(cPlane2[i][0],cPlane2[i][1],cPlane2[i][2]);
				glEnd();
			}
			glEnable( GL_LIGHTING );
		}
//-------------------------------------------------- (hkl) Plane
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (hkl_trans < 1) {
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(hkl_rgb[0],hkl_rgb[1],hkl_rgb[2],hkl_trans);
		}
		else glColor3f(hkl_rgb[0],hkl_rgb[1],hkl_rgb[2]);
		glBegin(GL_POLYGON);
		glNormal3f(Normal[0],Normal[1],Normal[2]);
		for(i=0; i<cPNum; i++) glVertex3d(cPlane1[i][0],cPlane1[i][1],cPlane1[i][2]);
		glEnd();
		if((origin_hkl == 0) && (show_hkl_inv==1)){
			glBegin(GL_POLYGON);
			glNormal3f(hkl[0],hkl[1],hkl[2]);
			for(i=0; i<cPNum; i++) glVertex3d(cPlane2[i][0],cPlane2[i][1],cPlane2[i][2]);
			glEnd();
		}
		if (hkl_trans < 1) {
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
	}
	glLineWidth(1);
	glEndList();
}


/**************************************** clear_cell_list() *****************/
void clear_cell_list()
{
	glNewList(1,GL_COMPILE);
	glEndList();
}

/**************************************** clear_atom_list() *****************/
void clear_atom_list()
{
	glNewList(2,GL_COMPILE);
	glEndList();
}

/**************************************** clear_bond_list() *****************/
void clear_bond_list()
{
	glNewList(3,GL_COMPILE);
	glEndList();
}

/**************************************** clear_poly_list() *****************/
void clear_poly_list()
{
	glNewList(4,GL_COMPILE);
	glEndList();
}

/**************************************** clear_axis_list() *****************/
void clear_axis_list()
{
	glNewList(5,GL_COMPILE);
	glEndList();
}

/**************************************** clear_hkl_list() ******************/
void clear_hkl_list()
{
	glNewList(6,GL_COMPILE);
	glEndList();
}

/**************************************** clear_display_list() **************/
void clear_display_list()
{
	clear_cell_list();
	clear_atom_list();
	clear_bond_list();
	clear_poly_list();
	clear_axis_list();
	clear_hkl_list();
}




/**************************************** set_display_List() ****************/
void set_display_List()
{
	int i,j,I1,I2;
	int old_bonds_type;


	total_select = 0;
	old_bonds_type = bonds_type;

	if (nXYZ > 0) {
		create_Cell_List();
		create_Axis_List();
		create_HKL_list();
	}
	switch(model_mode)
	{
//------------------------------------------------ B&S model
	case(0):{
		CreatAtomList();
		CreatBondList();
		clear_poly_list();
		break;
			}
//------------------------------------------------ Space Filling model
	case(1):{
		CreatAtomList();
		clear_bond_list();
		clear_poly_list();
		break;
			}
//------------------------------------------------ Polyhedra model
	case(2):{
//		bonds_type = 0;
//====================================================================//
//   poly_type  Central Atom  Coordination Atoms  Bonds  Polyhedra    //
//....................................................................//
//       0        Yes (1)          Yes (1)         Yes      No        //
//       1        Yes (1)          Yes (1)         Yes      Yes       //
//       2        Yes (2)          Yes (1)         No       Yes       //
//       3        Yes (2)          No  (0)         No       Yes       //
//       4        No  (0)          No  (0)         No       Yes       //
//       5        No  (3)          No  (0)         Yes      No        //
//====================================================================//
		switch(poly_type)
		{
		case(0):{
			for(i=0; i<nPoly; i++){
				I1 = poly_list[i];
				m[I1].poly_show = 1;
				for(j=0; j<m[I1].bondNum; j++) {
					I2 = m[I1].b_list[j];
					m[I2].poly_show = 1;
				}
			}
			CreatAtomList();
			CreatBondList();
			clear_poly_list();
			break;
				}
		case(1):{
			for(i=0; i<nPoly; i++){
				I1 = poly_list[i];
				m[I1].poly_show = 1;
				for(j=0; j<m[I1].bondNum; j++) {
					I2 = m[I1].b_list[j];
					m[I2].poly_show = 1;
				}
			}
			CreatAtomList();
			CreatBondList();
			CreatePolyList();
			break;
				}
		case(2):{
			for(i=0; i<nPoly; i++){
				I1 = poly_list[i];
				m[I1].poly_show = 2;
				for(j=0; j<m[I1].bondNum; j++) {
					I2 = m[I1].b_list[j];
					m[I2].poly_show = 1;
				}
			}
			CreatAtomList();
			CreatBondList();
			CreatePolyList();
			break;
				}
		case(3):{
			for(i=0; i<nPoly; i++){
				I1 = poly_list[i];
				m[I1].poly_show = 2;
				for(j=0; j<m[I1].bondNum; j++) {
					I2 = m[I1].b_list[j];
					m[I2].poly_show = 0;
				}
			}
			CreatAtomList();
			CreatBondList();
			CreatePolyList();
			break;
				}
		case(4):{
			for(i=0; i<nPoly; i++){
				I1 = poly_list[i];
				m[I1].poly_show = 0;
				for(j=0; j<m[I1].bondNum; j++) {
					I2 = m[I1].b_list[j];
					m[I2].poly_show = 0;
				}
			}
			CreatAtomList();
			CreatBondList();
			CreatePolyList();
			break;
				}
		case(5):{
			for(i=0; i<nPoly; i++){
				I1 = poly_list[i];
				m[I1].poly_show = 3;
				for(j=0; j<m[I1].bondNum; j++) {
					I2 = m[I1].b_list[j];
					m[I2].poly_show = 0;
				}
			}
			CreatAtomList();
			CreatBondList();
			clear_poly_list();
			break;
				}
		}
		break;
			}
//------------------------------------------------ Wire-frame model
	case(3):{
		old_bonds_type = bonds_type;
		bonds_type = 3;
		CreatAtomList();
		CreatBondList();
		clear_poly_list();
		break;
			}
//------------------------------------------------ Stick model
	case(4):{
		if (bonds_type > 1) {
			old_bonds_type = bonds_type;
			bonds_type = 0;
		}
		CreatAtomList();
		CreatBondList();
		clear_poly_list();
		break;
			}
//------------------------------------------------ Stick model
	case(5):{
		CreatAtomList();
		CreatBondList();
		clear_poly_list();
		break;
			}
	}
	bonds_type = old_bonds_type;
}


/**************************************** reset_all_deleted_objects() *******/
void reset_all_deleted_objects()
{
	int i,I1;
	if (del_Object.nAtom > 0) {
		for (i=0; i<del_Object.nAtom; i++){
			I1 = del_Object.del_Atom[i];
			m[I1].show = 'y';
		}
		del_Object.nAtom = 0;
	}
	if (del_Object.nBonds > 0) {
		for (i=0; i<del_Object.nBonds; i++){
			I1 = del_Object.del_Bonds[i];
			bAtom[I1].show = 'y';
		}
		del_Object.nBonds = 0;
	}
	if (del_Object.nPoly > 0) {
		for (i=0; i<del_Object.nPoly; i++){
			I1 = del_Object.del_Poly[i];
			tp[I1].show = 'y';
		}
		del_Object.nPoly = 0;
	}
	num_Spin = 0;
	for(i=0; i<nXYZ; i++) m[i].show_spin = 0;
}


/**************************************** load_data() ***********************/
void load_data(int ID)
{
	get_atom_R_and_colors();
	setOrthoMatrix();

	cCell.max_P = maxf(cCell.a[0],cCell.a[1],cCell.a[2]);
	get_bond_search_mode();
	set_ortho_cell();
	switch(fileID)
	{
	case(0):     //--- VICS
	case(1):     //--- AMCSD 
	case(3):     //--- CIF
	case(4):     //--- CM
	case(5):     //--- CRYSTIN
	case(6):     //--- CSSR
	case(7):     //--- FDAT.CSD
	case(9):     //--- ICSD
	case(8):     //--- Gaussian 98
	case(11):    //--- MINCRYST
	case(14):    //--- RIETAN
	case(15):    //--- WIEN2k
	case(16):    //--- XmolXYZ
	case(17):    //--- ASSE
	case(19):{   //--- MXD
		nXYZ = _build_crystal(cCell,sPos, nPos, lAtom, nAtom, bini, bond_sNum, range,fileID);
		_get_XYZ_in_range(m);
		CalcCN();
		setOrthoXYZ();
		setOrigin();
		searchBonds();
		break;
			}
	case(13):   //--- PDB
	case(2):{   //--- Chem3D
		CalcCN();
		setOrthoXYZ();
		setOrigin();
		if (mol_bond != 0) (void)bonds_for_molecule(1);
		else {
			searchBonds();
		}
		break;
			 }
	case(10):    //--- MDL Molfile
	case(18):    //--- SCAT f01
	case(12):{  //--- MOLDA
		CalcCN();
		setOrthoXYZ();
		setOrigin();
		if (mol_bond != 0) (void)bonds_for_molecule(0);
		else {
			searchBonds();
		}
		break;
			 }
	}
	calc_param_for_therm();
	check_iso_uniso_parameters();
	if (hydrogen_bonds_id == 1) (void)search_hydr_bond();
	CalcPolyNum();
    makePoly1();
	check_deleted_atoms();
	set_boundaries_ortho_XYZ();
	if (ID != 1) print_out_result();
	model_mode = MODL - 1;
	model_radio->set_int_val(model_mode);
	if ((model_mode == 1) || (model_mode == 2)) dot_surf_check->disable();
	else dot_surf_check->enable();
	dot_surf_check->set_int_val(dot_surf_ID);
	show_hkl = 0;
	shift_hkl[0] = shift_hkl[1] = 0;
	if ((ID == 2) && (fileID==0)) num_Spin = get_spin_from_file(FileName);
	set_display_List();
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
	case (3):{sprintf(statustext,"%s %s",info1,"Interatomic distance");break;}
	case (4):{sprintf(statustext,"%s %s",info1,"Bond/torsion angle");break;}
	case (5):{sprintf(statustext,"%s %s",info1,"Select object(s)");break;}
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


/***************************************** set_lights() *********************/
void set_lights()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glMultMatrixf(l_direction);

	
	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l_dir);

	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
}

/***************************************** set_atom_material() **************/
void set_material(int ID)
{
	switch(ID)
	{
//---------------- ATOMS
	case(0):{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_atom_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_atom_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_atom_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_atom_shininess);
		break;
			}
//---------------- BONDS
	case(1):{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_bond_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_bond_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_bond_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_bond_shininess);
		break;
			}
//---------------- POLYHEDRA
	case(2):{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_poly_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_poly_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_poly_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_poly_shininess);
		break;
			}
//---------------- HKL PLANE
	case(3):{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_plane_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_plane_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_plane_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_plane_shininess);
		break;
			}
	}
}

/***************************************** m3Datom_Translate() **************/
void m3Datom_Translate(int x, int y)
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

/***************************************** m3Datom_Magnitude() **************/
void m3Datom_Magnitude(int x, int y)
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


/***************************************** m3Datom_Magnitude() **************/
void m3Datom_Rotation(int x, int y)
{
	tbMotion(x,y,rotation_mode);
}

/***************************************** m3Datom_HKL_Shift() **************/
void m3Datom_HKL_Shift(int x, int y)
{
	int x1,y1;
	x1 = x - last_x;
	y1 = last_y - y;
//	float x1,y1;
//	x1 = (float)(x - last_x);
//	y1 = (float)(last_y - y);
	shift_hkl[0] += 0.01*x1;
	shift_hkl[1] += 0.01*y1;
	makeHKLplane(origin_hkl);
	create_HKL_list();
	last_x = x;
	last_y = y;
	glutPostRedisplay(); 
}

/***************************************** m3Datom_HKL_Shift() **************/
void m3Datom_XYZ_Shift(int x, int y)
{
	float x1,y1;
	x1 = (float)(x - last_x);
	y1 = (float)(last_y - y);
	shift_compass[0] += 0.005*x1;
	shift_compass[1] += 0.005*y1;
	last_x = x;
	last_y = y;
	glutPostRedisplay(); 
}

/***************************************** m3Datom_Mouse_Motion() ***********/
void m3Datom_Mouse_Motion(int x, int y )
{
	if ((tools_mode == 0) && (rotation_type == 2)) return;
	if (motion == 1){
		switch(tools_mode)
		{
		case(0):{m3Datom_Rotation(x,y); break;}
		case(1):{m3Datom_Magnitude(x,y); break;}
		case(2):{m3Datom_Translate(x,y); break;}
		case(5):{
			if(move_hkl == 1) m3Datom_HKL_Shift(x,y); 
			else if (move_hkl == 2) m3Datom_XYZ_Shift(x,y);
			break;
				}
		}
	}

}


/**************************************** m3Datom_Idle() ********************/
void m3Datom_Idle( void )
{
	int i,j,k,file_control,ID,I1;
	int preview;
	float RGB[3];

	if ( glutGetWindow() != main_window ) glutSetWindow(main_window);  


//------------------------------------------------------------------------- Export Image Msg Dialog_1
	if (chk_image_ID == -2){
		chk_image_ID = -1;
		Msg_Export_Dialog(main_window);
	}
//------------------------------------------------------------------------- Export Image Msg Dialog
	if (chk_image_ID == -1){
		i = get_msg_export_dialog_id();
		if (i == 1) chk_image_ID = 0;
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Poly Type Dialog
	if (poly_type_dialog_ID != 0){
		i = get_Polygon_ID();
		if (i == 1){
			j = GetPolygonType();
			set_poly_type(j);
			get_poly_prop_main_param(poly_type, poly_trans, poly_outline_width, poly_outline);
			set_display_List();
			poly_type_dialog_ID = 0;
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Color Palette Dialog
	if (color_dialog_ID != 0){
		i = get_palette_ID();
		if (i == 0){
			switch(color_dialog_ID)
			{
//-----------------------------------------------------  Background colors
			case(101):{
				GetPaletteRGB(RGB);
				set_bkg_colors(RGB);
				get_background_rendering(BKCL[0], BKCL[1], BKCL[2]);
				create_Axis_List();
				break;
					  }
//-----------------------------------------------------  Atom colors
			case(102):{
				GetPaletteRGB(RGB);
				set_atom_colors(RGB);
				for(j=0; j<tAtom; j++) get_type_R_RGB(j, AtomT[j].R,AtomT[j].color);
				get_atom_R_and_colors();
				CreatAtomList();
				break;
					  }
//-----------------------------------------------------  Bond colors
			case(103):{
				GetPaletteRGB(BNCL);
				set_bond_colors(BNCL);
				CreatBondList();
				break;
					  }
//-----------------------------------------------------  Poly Outline colors
			case(104):{
				GetPaletteRGB(RGB);
				set_poly_colors(RGB);
				poly_line_color[0] = get_poly_prop_colors_R();
				poly_line_color[1] = get_poly_prop_colors_G();
				poly_line_color[2] = get_poly_prop_colors_B();
				CreatePolyList();
				break;
					  }
//-----------------------------------------------------  Unit Cell colors
			case(105):{
				GetPaletteRGB(ucell_color);
				set_ucell_colors(ucell_color);
				create_Cell_List();
				break;
					  }
//-----------------------------------------------------  Lattice Plane colors
			case(106):{
				GetPaletteRGB(RGB);
				set_hkl_colors(RGB);
				get_hkl_prop_colors(hkl_rgb[0], hkl_rgb[1], hkl_rgb[2], hkl_trans);
				create_HKL_list();
				break;
					  }
//-----------------------------------------------------  Atom Vector colors
			case(107):{
				GetPaletteRGB(RGB);
				set_spin_color(RGB);
				break;
					  }
			}
			color_dialog_ID = 0;
		}

	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Message Dialog 2
	if (message_dialog_ID2 != 0){
		i = get_message_dialog_id2();
		if (i == 1){
			message_dialog_ID2 = 0;
			exit(0);
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Message Dialog 3
	if (message_dialog_ID3 != 0){
		i = get_message_dialog_id3();
		if (i == 1){
			message_dialog_ID3 = 0;
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- About Dialog
	if (about_dialog_ID != 0){
		i = get_about_dialog_id();
		if (i == 1) {
			get_about_dialog_pos(dwp[0][0],dwp[1][0]);
			glutSetWindow(main_window);
			about_dialog_ID = 0;
			new_btn->enable();
			open_btn->enable();
			save_btn->enable();
			img_btn->enable();
			exp_btn->enable();
			edit_btn->enable();
			bonds_btn->enable();
			hkl_btn->enable();
			pref_btn->enable();
			about_btn->enable();
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Read File Name from ARG
	if(arg_openfile_ID != 0) {
		arg_openfile_ID = 0;
		sprintf(FileName,"");
		sprintf(FileName,"%s",arg_fname);
		file_control = readdata(fileID,FileName,0);  
		if (file_control != -1) {
			_update__window_title();
			start_VICS = 1;
			m3Datom_TB_init();
			load_data(2);
			set_lights();
		}
		else {
			message_dialog_ID2 = -1;
			open_message_dialog2(main_window,1);
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Export Image Dialog
	if(image_dialog_ID != 0) {
		i = get_graph_dialog_id();
		if (i == 1) {image_dialog_ID = 0;run_img_export = 0;}
		if (i == 10){
			image_dialog_ID = 0;
			set_graph_dialog_id(-1);
			get_export_param(img_smoothing,img_scale);
			run_img_export = 1;
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Export Dialog
	if(message_dialog_ID1 != 0) {
		i = get_message_dialog_id1();
		if (i > 0){
			message_dialog_ID1 = 0;
			export_mode = i;
			export_data(e_fileID,e_FileName);
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Preferences Dialog
	if (prefernces_dialog_ID != 0){
		i = get_preferences_dialog_id();
		switch(i){
		case(1):{
			prefernces_dialog_ID = 0;
			get_prefer_dialog_pos(dwp[0][7],dwp[1][7]);
			glutSetWindow(main_window);
			get_preferences_cur_param(atoms_stack,atoms_slice,out_atoms_stack,out_atoms_slice,
				                      bonds_stack,bonds_slice,out_bonds_stack,out_bonds_slice,
									  ucell_width,poly_outline_width,hkl_out_width,bonds_line_width,
									  poly_trans,hkl_trans,
									  BKCL[0],BKCL[1],BKCL[2],
									  ucell_color[0],ucell_color[1],ucell_color[2],
									  poly_line_color[0],poly_line_color[1],poly_line_color[2],
									  hkl_rgb[0],hkl_rgb[1],hkl_rgb[2],
									  BNCL[0],BNCL[1],BNCL[2],
									  bRadii[0],bRadii[1],show_axis,stepw_angle,
									  def_mol_bond,def_search_bond,def_file_format_ID,
									  t_FileName,save_pos_ID,save_dpos_ID);
			fog_color[0] = BKCL[0];
			fog_color[1] = BKCL[1];
			fog_color[2] = BKCL[2];
			fog_color[3] = 0;
			save_pos_ID = 0;
			save_dpos_ID = 0;
			set_display_List();
			break;
				}
		case(2):{
			set_preferences_dialog_id(-1);
			get_preferences_cur_param(atoms_stack,atoms_slice,out_atoms_stack,out_atoms_slice,
				                      bonds_stack,bonds_slice,out_bonds_stack,out_bonds_slice,
									  ucell_width,poly_outline_width,hkl_out_width,bonds_line_width,
									  poly_trans,hkl_trans,
									  BKCL[0],BKCL[1],BKCL[2],
									  ucell_color[0],ucell_color[1],ucell_color[2],
									  poly_line_color[0],poly_line_color[1],poly_line_color[2],
									  hkl_rgb[0],hkl_rgb[1],hkl_rgb[2],
									  BNCL[0],BNCL[1],BNCL[2],
									  bRadii[0],bRadii[1],show_axis,stepw_angle,
									  def_mol_bond,def_search_bond,def_file_format_ID,
									  t_FileName,save_pos_ID,save_dpos_ID);
			fog_color[0] = BKCL[0];
			fog_color[1] = BKCL[1];
			fog_color[2] = BKCL[2];
			fog_color[3] = 0;
			get_preferences_def_param(def_atom_s_stick,def_atom_s_slice,def_atom_o_stick,def_atom_o_slice,
			                      def_bond_s_stick,def_bond_s_slice,def_bond_o_stick,def_bond_o_slice,
								  def_ucel_line_width,def_poly_line_width,def_hkl_line_width,def_bond_line_width,
								  def_poly_tran,def_hkl_tran,
								  def_bkgr_color[0],def_bkgr_color[1],def_bkgr_color[2],
								  def_ucel_color[0],def_ucel_color[1],def_ucel_color[2],
								  def_poly_color[0],def_poly_color[1],def_poly_color[2],
								  def_hkl_color[0],def_hkl_color[1],def_hkl_color[2],
								  def_bond_color[0],def_bond_color[1],def_bond_color[2],
								  def_bond_rad[0],def_bond_rad[1],def_show_axis,stepw_angle,
								  def_mol_bond,def_search_bond,def_file_format_ID,
								  t_FileName,save_pos_ID,save_dpos_ID);
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
				for(j=0; j<15; j++) {cdwp[0][j] = dwp[0][j];cdwp[1][j] = dwp[1][j];}
			}
			save_default_pref();
			break;
				}
		}

	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- New/Edit Dialog
	if (new_edit_dialog_ID != 0){
		i = get_new_edit_dialog_id();
		switch(i)
		{
//--- "Close / Cancle" 
		case(0):{
			new_edit_dialog_ID = 0;
			get_edit_dialog_pos(dwp[0][3],dwp[1][3]);
			glutSetWindow(main_window);
			print_out_result();
			break;
				}
//--- "OK"     (New Data Dialog; cMode = 0)
		case(1):{
			reset_all_deleted_objects();
            init_data_settings();
			new_edit_dialog_ID = 0; 
			get_edit_dialog_pos(dwp[0][3],dwp[1][3]);
			glutSetWindow(main_window);
			get_new_edit_file_dialog_param(Title,cCell.SpGrN,cCell.Setting,cCell.SpGr, 
  									       cCell.a, cCell.ea,nAtom,Btype,use_aniso,range,use_iso); 
			for(i=0; i<nAtom; i++){
				get_struc_param(i,lAtom[i].name,lAtom[i].simb,lAtom[i].g,lAtom[i].x,lAtom[i].y,lAtom[i].z, 
                                lAtom[i].ex,lAtom[i].ey,lAtom[i].ez,lAtom[i].B,lAtom[i].Biso);
			}
			ID = get_group_name_and_XYZ();
			XYZ_to_Matrix(ID);
			Check_Lattice_Type();
			update_atom_type();
			load_data(0); 
			start_VICS = 1;
			break;
				}
//--- "Apply"     (Edit Data Dialog; cMode = 1)
		case(2):{
			set_new_edit_dialog_id(-1);
			reset_all_deleted_objects();
            if (fileID != 15) init_data_settings();
			get_new_edit_file_dialog_param(Title,cCell.SpGrN,cCell.Setting,cCell.SpGr, 
  									       cCell.a, cCell.ea,nAtom,Btype,use_aniso,range,use_iso); 
			for(i=0; i<nAtom; i++){
				get_struc_param(i,lAtom[i].name,lAtom[i].simb,lAtom[i].g,lAtom[i].x,lAtom[i].y,lAtom[i].z, 
                                lAtom[i].ex,lAtom[i].ey,lAtom[i].ez,lAtom[i].B,lAtom[i].Biso);
			}

//			nPos = 0;
			if (fileID != 15) {
				ID = get_group_name_and_XYZ();
				XYZ_to_Matrix(ID);
			}
			Check_Lattice_Type();
			update_atom_type();
			load_data(1);
			start_VICS = 1;
			break;
				}
//--- "Origin" 
		case(10):{
			set_new_edit_dialog_id(-1);
			if (orig_dialog_ID == 0){
				orig_dialog_ID = -1;
				get_cur_origin(cCell.SpGrN,ID);
				set_ortho_origin_dialog(cCell.SpGrN,ID);
				open_ortho_origin_dialog(main_window,dwp[0][5],dwp[1][5]);
			}
			break;
				 }
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Origin for Ortho Dialog
	if (orig_dialog_ID != 0){
		i = get_ortho_origin_dialog_id();
		if (i == 1){
			orig_dialog_ID = 0;
			get_origin_dialog_pos(dwp[0][5],dwp[1][5]);
			glutSetWindow(main_window);
			ID = get_ortho_origin_dialog();
			set_cur_origin(ID);
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Bonds Dialog
	if (bonds_dialog_ID != 0){
		i = get_bond_dialog_id();
		switch(i){
		case(1):{
			reset_all_deleted_objects();
			bonds_dialog_ID = 0;
			get_sbonds_dialog_pos(dwp[0][10],dwp[1][10]);
			glutSetWindow(main_window);
			bond_sNum = get_new_number_bond_spec();
			hydrogen_bonds_id = get_h_bond_id();
			for(j=0; j<bond_sNum; j++) {
				get_new_bond_spec(j, bini[j].A1, bini[j].A2, bini[j].d,
				            bini[j].search_mode,bini[j].boundary_mode,bini[j].show_poly_mode);
			}
			load_data(1);
			CreatBondList();
				}
		case(0):{
			bonds_dialog_ID = 0;
			get_sbonds_dialog_pos(dwp[0][10],dwp[1][10]);
			glutSetWindow(main_window);
			break;
				}
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- General Properties
	if (general_prop_dialog_ID != 0){
		i = get_render_control_id();
		switch(i)
		{
		case(1):{
			general_prop_dialog_ID = 0;
			get_general_dialog_pos(dwp[0][9],dwp[1][9]);
			glutSetWindow(main_window);
			break;
				}
		case(100):{
			set_render_control_id(-1);
			get_lights_rendering(l_ambient, l_diffuse, l_direction);
			set_lights();
			break;
				  }
		case(101):{
			set_render_control_id(-1);
			get_atom_rendering(m_atom_ambient,m_atom_diffuse, m_atom_specular, m_atom_shininess);
			set_material(0);
			break;
				  }
		case(102):{
			set_render_control_id(-1);
			get_bond_rendering(m_bond_ambient,m_bond_diffuse, m_bond_specular, m_bond_shininess);
			set_material(1);
			break;
				  }
		case(103):{
			set_render_control_id(-1);
			get_poly_rendering(m_poly_ambient,m_poly_diffuse, m_poly_specular, m_poly_shininess);
			set_material(2);
			break;
				  }
		case(104):{
			set_render_control_id(-1);
			get_plane_rendering(m_plane_ambient,m_plane_diffuse, m_plane_specular, m_plane_shininess);
			set_material(3);
			break;
				  }
		case(105):{
			set_render_control_id(-1);
			get_background_rendering(BKCL[0], BKCL[1], BKCL[2]);
			fog_color[0] = BKCL[0];
			fog_color[1] = BKCL[1];
			fog_color[2] = BKCL[2];
			fog_color[3] = 0;
			create_Axis_List();
			break;
				  }
		case(106):{
			set_render_control_id(-1);
			show_axis = get_compass_rendering();
			create_Axis_List();
			break;
				  }
		case(107):{
			set_render_control_id(-1);
			get_projection_perspective(projection_type, perspective);
			break;
				  }
		case(108):{
			get_bkg_colors(RGB);
			set_render_control_id(-1);
			color_dialog_ID = 101;
			InitPaletteDialog(RGB);
			get_general_dialog_pos(dwp[0][9],dwp[1][9]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][9],dwp[1][9]);
			break;
				  }
		case(109):{
			set_render_control_id(-1);
			radii_type = get_radii_rendering();
			set_atom_R();
			set_display_List();
			break;
				  }
		case(110):{
			set_render_control_id(-1);
			fog = get_fog_rendering();
			break;
				  }

		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Atoms Properties Dialog
	if (atoms_prop_dialog_ID != 0){
		i = get_atom_prop_dialog_id();
		preview = get_atom_prop_preview_id();
		switch(i)
		{
		case(1):
		case(0):{
			atoms_prop_dialog_ID = 0;
			get_atom_dialog_pos(dwp[0][1],dwp[1][1]);
			glutSetWindow(main_window);
			get_atom_prop_main_params(atoms_stack,atoms_slice,atom_type,TEOL,Probability);
			for(j=0; j<tAtom; j++) get_type_R_RGB(j, AtomT[j].R,AtomT[j].color);
			get_atom_R_and_colors();
			set_display_List();
			break;
				}
		case(200):{
			if (preview == 1) {
				set_atom_prop_dialog_id(-1);
				get_atom_prop_main_params(atoms_stack,atoms_slice,atom_type,TEOL,Probability);
				CreatAtomList();
			}
			break;
				  }
		case(201):{
			if (preview == 1) {
				set_atom_prop_dialog_id(-1);
				for(j=0; j<tAtom; j++) get_type_R_RGB(j, AtomT[j].R,AtomT[j].color);
				get_atom_R_and_colors();
				CreatAtomList();
			}
			break;
				  }
		case(202):{
			if (preview == 1) {
				set_atom_prop_dialog_id(-1);
				for(j=0; j<tAtom; j++) get_type_R_RGB(j, AtomT[j].R,AtomT[j].color);
				get_atom_R_and_colors();
				CreatAtomList();
			}
			break;
				  }
		case(203):{
			if (preview == 1) {
				set_atom_prop_dialog_id(-1);
				get_atom_prop_main_params(atoms_stack,atoms_slice,atom_type,TEOL,Probability);
				CreatAtomList();
			}
			break;
				  }
		case(300):{
			get_atom_colors(RGB);
			set_atom_prop_dialog_id(-1);
			color_dialog_ID = 102;
			InitPaletteDialog(RGB);
			get_atom_dialog_pos(dwp[0][1],dwp[1][1]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][1],dwp[1][1]);
			break;
				  }
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Bonds Properties Dialog
	if (bonds_prop_dialog_ID != 0){
		i = get_bond_prop_dialog_id();
		preview = get_bond_prop_preview_id();
		switch(i){
		case(0):
		case(1):{
			bonds_prop_dialog_ID = 0;
			get_bonds_dialog_pos(dwp[0][14],dwp[1][14]);
			glutSetWindow(main_window);
			get_bond_main_prop_params(bonds_type,bonds_stack,bonds_slice,bonds_line_width,
				hbond_line_type);
			bRadii[0] = get_bond_prop_radius_B_S();
			bRadii[1] = get_bond_prop_radius_Stick();
			get_bond_colors(BNCL);
			CreatBondList();
			break;
				}
		case(300):{
			set_bond_prop_dialog_id(-1);
			if (preview == 1){
			    get_bond_main_prop_params(bonds_type,bonds_stack,bonds_slice,bonds_line_width,
				hbond_line_type);
				bRadii[0] = get_bond_prop_radius_B_S();
				bRadii[1] = get_bond_prop_radius_Stick();
				get_bond_colors(BNCL);
				CreatBondList();
			}
			break;
				  }
		case(400):{
			get_bond_colors(RGB);
			set_bond_prop_dialog_id(-1);
			color_dialog_ID = 103;
			InitPaletteDialog(RGB);
			get_bonds_dialog_pos(dwp[0][14],dwp[1][14]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][14],dwp[1][14]);
			break;
				  }
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Polyhedra Properties Dialog
	if (poly_prop_dialog_ID != 0){
		i = get_poly_prop_dialog_id();
		preview = get_poly_prop_preview_id();
		switch(i)
		{
//------------------------------------------ OK / Cancel
		case(0):
		case(1):{
			poly_prop_dialog_ID = 0;
			get_poly_dialog_pos(dwp[0][6],dwp[1][6]);
			glutSetWindow(main_window);
			get_poly_prop_main_param(poly_type, poly_trans, poly_outline_width, poly_outline);
			poly_line_color[0] = get_poly_prop_colors_R();
			poly_line_color[1] = get_poly_prop_colors_G();
			poly_line_color[2] = get_poly_prop_colors_B();
			set_display_List();
			break;
				}
//------------------------------------------ Opacity, Outline Width, Show Outline 
		case(404):
		case(403):
		case(402):{
			if (preview == 1){
				get_poly_prop_main_param(poly_type, poly_trans, poly_outline_width, poly_outline);
				CreatePolyList();
			}
			set_poly_prop_dialog_id(-1);
			break;
				  }
//------------------------------------------ Outline Colors 
		case(405):{
			if (preview == 1){
				poly_line_color[0] = get_poly_prop_colors_R();
				poly_line_color[1] = get_poly_prop_colors_G();
				poly_line_color[2] = get_poly_prop_colors_B();
				CreatePolyList();
			}
			set_poly_prop_dialog_id(-1);
			break;
				  }
//------------------------------------------ Outline Colors (Palette)
		case(500):{
			get_poly_colors(RGB);
			set_poly_prop_dialog_id(-1);
			color_dialog_ID = 104;
			InitPaletteDialog(RGB);
			get_poly_dialog_pos(dwp[0][6],dwp[1][6]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][6],dwp[1][6]);
			break;
				  }
//------------------------------------------ Polygon Type
		case(600):{
			i = get_poly_type();
			set_poly_prop_dialog_id(-1);
			poly_type_dialog_ID = 100;
			InitPolygonDialog(i);
			get_poly_dialog_pos(dwp[0][6],dwp[1][6]);
			glutSetWindow(main_window);
			OpenPolygonWindow(main_window,dwp[0][6],dwp[1][6]);
			break;
				  }

		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Unit Cell Outline Properties Dialog
	if (ucell_prop_dialog_ID != 0){
		i = get_ucell_prop_dialog_id();
		switch(i)
		{
		case(1):{
			ucell_prop_dialog_ID = 0;
			get_ucell_dialog_pos(dwp[0][13],dwp[1][13]);
			glutSetWindow(main_window);
			break;
				}
		case(500):{
			set_ucell_prop_dialog_id(-1);
			get_ucell_prop(SHCL,ucell_width,ucell_color[0],ucell_color[1],ucell_color[2], ucell_type);
			create_Cell_List();
			break;
				  }
		case(600):{
			get_ucell_colors(ucell_color);
			set_ucell_prop_dialog_id(-1);
			color_dialog_ID = 105;
			InitPaletteDialog(ucell_color);
			get_ucell_dialog_pos(dwp[0][13],dwp[1][13]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][13],dwp[1][13]);
			break;
				  }
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Plot Range Dialog
	if (plot_range_dialog_ID != 0){
		i = get_plot_range_dialog_id();
		if (i == 1){
			plot_range_dialog_ID = 0;
			get_range_dialog_pos(dwp[0][8],dwp[1][8]);
			glutSetWindow(main_window);
			get_plot_range_param(range);
			reset_all_deleted_objects();
			load_data(1);
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Crystal Orientation Dialog
	if (corient_dialog_ID != 0){
		i = get_orient_dialog_id();
		if ((i == 0) || (i == 1)) {
			corient_dialog_ID = 0; 
			orientation = 0;
			get_orient_dialog_pos(dwp[0][4],dwp[1][4]);
			glutSetWindow(main_window);
		}
		if (i == -1) orientation = 1;
		switch(i){
		case(0):{
			get_view_point(c_rotation);
			tbUpdate(0,GLUT_LEFT_BUTTON, c_rotation);
			break;
				}
		case(20):{
			get_view_point(c_rotation);
			tbUpdate(0,GLUT_LEFT_BUTTON, c_rotation);
			set_orient_dialog_id(-1);
			getOrientation(c_rotation);
			set_orient_param(c_rotation);
			break;
				 }
		}

	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Stepwise Rotation Dialog
	if (stepwise_dialog_ID != 0){
		i = get_stepwise_dialog_id();
		switch(i){
		case(0):
		case(1):{
			stepwise_dialog_ID = 0;
			get_step_dialog_pos(dwp[0][12],dwp[1][12]);
			glutSetWindow(main_window);
			stepwise_active = 0;
			rotation_tipe_radio->enable();
			bFree->enable();
			bX->enable();
			bY->enable();
			bZ->enable();
			get_stepwise_param(stepw_mode, stepw_rot_axis, stepw_angle);
			updateStatusBar();
			break;
				}
		case(10):{
			set_stepwise_dialog_id(-1);
			stepwise_active = 1;
			get_stepwise_param(stepw_mode, stepw_rot_axis, stepw_angle);
			updateStatusBar1();
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
			break;
				 }
		}
	}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- (hkl) Plane Properties Dialog
	if (hkl_prop_dialog_ID != 0){
		i = get_hkl_prop_dialog_id();
		preview = get_hkl_prop_preview_id();
		switch(i)
		{
		case(0):{
			hkl_prop_dialog_ID = 0;
			get_hkl_dialog_pos(dwp[0][2],dwp[1][2]);
			glutSetWindow(main_window);
			break;
				}
		case(1):{
			hkl_prop_dialog_ID = 0;
			get_hkl_dialog_pos(dwp[0][2],dwp[1][2]);
			glutSetWindow(main_window);
			get_hkl_prop_main_param(show_hkl, origin_hkl, show_hkl_inv, show_hkl_out, hkl_out_width);
			get_hkl_prop_colors(hkl_rgb[0], hkl_rgb[1], hkl_rgb[2], hkl_trans);
			get_hkl_prop_index(hkl[0],hkl[1],hkl[2]);
			makeHKLplane(origin_hkl);
			create_HKL_list();
			break;
				}
		case(600):{
			if (preview == 1){
				get_hkl_prop_main_param(show_hkl, origin_hkl, show_hkl_inv, show_hkl_out, hkl_out_width);
				get_hkl_prop_colors(hkl_rgb[0], hkl_rgb[1], hkl_rgb[2], hkl_trans);
				get_hkl_prop_index(hkl[0],hkl[1],hkl[2]);
				makeHKLplane(origin_hkl);
				create_HKL_list();
			}
			set_hkl_prop_dialog_id(-1);
			break;
				  }
		case(601):{
			if (preview == 1){
				get_hkl_prop_index(hkl[0],hkl[1],hkl[2]);
				makeHKLplane(origin_hkl);
				create_HKL_list();
			}
			set_hkl_prop_dialog_id(-1);
			break;
				  }
		case(602):{
			if (preview == 1){
				get_hkl_prop_colors(hkl_rgb[0], hkl_rgb[1], hkl_rgb[2], hkl_trans);
				create_HKL_list();
			}
			set_hkl_prop_dialog_id(-1);
			break;
				  }
		case(604):{
			if (preview == 1){
				get_hkl_prop_main_param(show_hkl, origin_hkl, show_hkl_inv, show_hkl_out, hkl_out_width);
				get_hkl_prop_colors(hkl_rgb[0], hkl_rgb[1], hkl_rgb[2], hkl_trans);
				get_hkl_prop_index(hkl[0],hkl[1],hkl[2]);
				makeHKLplane(origin_hkl);
				create_HKL_list();
			}
			set_hkl_prop_dialog_id(-1);
			break;
				  }
		case(700):{
			get_hkl_colors(RGB);
			set_hkl_prop_dialog_id(-1);
			color_dialog_ID = 106;
			InitPaletteDialog(RGB);
			get_hkl_dialog_pos(dwp[0][2],dwp[1][2]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][2],dwp[1][2]);
			break;
				  }
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//------------------------------------------------------------------------- Atom Vector Dialog
	if (spin_dialog_ID != 0){
		i = get_spin_dialog_id();
		switch(i)
		{
		case(1):{
			spin_dialog_ID = 0; 
			get_spin_dialog_pos(dwp[0][11],dwp[1][11]);
			glutSetWindow(main_window);
			break;
				}
		case(10):{
			set_spin_dialog_id(-1);
			I1 = get_spin_atom_num();
			j = m[I1].show_spin;
			get_spin_dialog_param(m[I1].show_spin,spin_H,m[I1].spin_dir,spin_RGB);
			switch(j)
			{
			case(0):{if(m[I1].show_spin == 1) num_Spin++; break;}
			case(1):{if(m[I1].show_spin == 0) num_Spin--; break;}
			}
			CreatAtomList();
			break;
				 }
		case(20):{
			get_spin_color(RGB);
			set_spin_dialog_id(-1);
			color_dialog_ID = 107;
			InitPaletteDialog(RGB);
			get_spin_dialog_pos(dwp[0][11],dwp[1][11]);
			glutSetWindow(main_window);
			OpenPaletteWindow(main_window,dwp[0][11],dwp[1][11]);
			break;
				 }
		}
	}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

	glutPostRedisplay();
}

/**************************************** m3Datom_RenderScene ***************/
void m3Datom_RenderScene(void)
{
	int i;

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0f, -5.0 );
//----------------------------------------------------------- XYZ Axis
	glPushMatrix();
      switch(projection_type)
	  {
	  case(0):{glTranslatef( -xy_aspect*0.80, -0.80,0);break;}
	  case(1):{glTranslatef( -xy_aspect*0.64, -0.64,0);break;}
	  }
	  glTranslatef(shift_compass[0],shift_compass[1],0);

	  if (stepwise_active == 1){
		  if (stepw_mode == 1) tbStepW(stepwise_active,stepw_mode,stepw_angle, stepw_rot);
		  else tbMatrix();
	  }
	  else tbMatrix();
	  glCallList(5);
	glPopMatrix();

	glEnable(GL_FOG);
//	glHint (GL_FOG_HINT, GL_NICEST);
	glFogf (GL_FOG_START,5+obj_pos[2]+origin[2]);
	glFogf (GL_FOG_END,  5*fog+obj_pos[2]-origin[2]);
	glFogi (GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fog_color);

	glPushMatrix();
	  glScalef( scale,scale,1 );
	  glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
	  glPushMatrix();
	  if (projection_type == 0) glScalef( 1,1,perspective );
          else glScalef( 1,1,1);
	  if (stepwise_active == 1){
		  if (stepw_mode == 1) tbStepW(stepwise_active,stepw_mode,stepw_angle,stepw_rot);
		  else tbMatrix();
	  }
	  else tbMatrix();
	  glTranslatef( origin[0],origin[1],origin[2]);
//----------------------------------------------------------- Unit Cell Outline
	  glCallList(1);
//----------------------------------------------------------- Atoms
	  set_material(0);
	  glCallList(2);
//----------------------------------------------------------- Bonds
	  set_material(1);
	  glCallList(3);
//----------------------------------------------------------- Polyhedra
	  set_material(2);
	  glCallList(4);
//----------------------------------------------------------- (hkl) Plane;
	  set_material(3);
	  glCallList(6);
//-----------------------------------------------------------
	  if ((tools_mode!=5) && (objNum>1)){
		  glDisable( GL_LIGHTING );
		  glColor3d(0.8f,0.8f,0.8f);
		  glLineWidth(2);
		  glEnable (GL_LINE_STIPPLE);
		  glLineStipple (1, 0x0F0F);
		  glBegin(GL_LINE_STRIP);
		  for (i=0; i<objNum; i++){
			  glVertex3f(m[pick_obj[i] - 1].x, m[pick_obj[i] - 1].y, m[pick_obj[i] - 1].z);
		  }
		  glEnd();
		  glLineWidth(1);
		  glEnable( GL_LIGHTING );
		  glDisable (GL_LINE_STIPPLE);
	  }
	glPopMatrix();
	glPopMatrix();
    glDisable(GL_FOG);
}


/***************************************** PickUpObj() **********************/
void PickUpObj(int x, int y)
{
	int i;
	int pikObjID;

	GLuint selectBuff[512];
	GLint hits;
	GLint viewport[4];
	int	choose;
	int	depth;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, selectBuff);
	(void)glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	total_select = 0;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	   glLoadIdentity();
	   gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3]-y+ty), 1.0f, 1.0f, viewport);
	   switch(projection_type)
	   {
//------------------------------------------------ Perspective projection
	   case(0):{glFrustum( -xy_aspect*0.02, xy_aspect*0.02, -0.02, 0.02, 0.1, 100.0 );break;}
//------------------------------------------------ Parallel projection
	   case(1):{glOrtho( -xy_aspect*0.8, xy_aspect*0.8, -0.8, 0.8, 0.1, 10.0 );break;}
	   }
	   glMatrixMode(GL_MODELVIEW);
	   set_display_List();
	   m3Datom_RenderScene();
	   glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	hits = glRenderMode(GL_RENDER);
	if (hits > 0)  {
		choose = selectBuff[3];
		depth = selectBuff[1];
		for (i = 1; i < hits; i++){
			if (selectBuff[i*4+1] < GLuint(depth)){
				choose = selectBuff[i*4+3];
				depth = selectBuff[i*4+1];
			}       
		}
		model_pick = choose;
		pikObjID = 0;
		for (i=0; i<objNum; i++) {
			if (pick_obj[i]==model_pick) {pikObjID = 1; break;}
		}
		if((tools_mode!=5) && (model_pick>nXYZ)) pikObjID = 1;
		else if (tools_mode == 5) pikObjID = 0;
		if (pikObjID == 0){
			pick_obj[objNum] = model_pick;
			objNum++;
		}

	}
	set_display_List();
	m3Datom_RenderScene();
}

//**************************  m3Datom_window_dump()   **************************//
int m3Datom_WindowDump1(FILE *fptr, int shiftX, int shiftY)
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
	glFinish();
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
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
			putc(image[3*j*tw+3*i+0],fptr);
			putc(image[3*j*tw+3*i+1],fptr);
			putc(image[3*j*tw+3*i+2],fptr);
		}
		fseek( fptr, offset2, SEEK_CUR);
	}
  
	free(image);
	return(0);
}

//**************************  m3Datom_window_dump()   **************************//
int m3Datom_window_dump1()
{
	int i,j;
    FILE *fptr;
	int ast,asl,bst,bsl;
	int num[2],pixels[2],max_pixels, max_win;

	ast = atoms_stack;
	asl = atoms_slice;
	bst = bonds_stack;
	bsl = bonds_slice;

	atoms_stack = out_atoms_stack;
	atoms_slice = out_atoms_slice;
	bonds_stack = out_bonds_stack;
	bonds_slice = out_bonds_slice;

	set_display_List();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	m3Datom_RenderScene();

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
			m3Datom_RenderScene();
			if (m3Datom_WindowDump1(fptr,i,j) < 0) return(-1);
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

	atoms_stack = ast;
	atoms_slice = asl;
	bonds_stack = bst;
	bonds_slice = bsl;

	set_display_List();
	glViewport( tx, ty, tw, th );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	m3Datom_RenderScene();
	run_img_export = 0;
	printf("OK!!!\n");
	return(0);
}

/**************************************** m3Datom_Keyboard() ****************/
void m3Datom_Keyboard(unsigned char Key, int x, int y)
{
	int i,j,I1,I2;

	switch(Key)
	{
	case(127):{
		for (i=0; i<objNum; i++){
			I1 = pick_obj[i];
			if (I1 <= nXYZ) {
				if (del_Object.nAtom < MAX_DELETED_OBJ){
					I2 = I1-1;
					m[I2].show = 'd';
					del_Object.del_Atom[del_Object.nAtom] = I2;
					del_Object.nAtom++;
					for(j=0; j<nBonds; j++){
						if ((I2 == bAtom[j].Atom1) || (I2 == bAtom[j].Atom2)) {
							if (del_Object.nBonds < MAX_DELETED_OBJ){
								bAtom[j].show = 'd';
								del_Object.del_Bonds[del_Object.nBonds] = j;
								del_Object.nBonds++;
							}
						}
					}
				}
			}
			if ((I1 > nXYZ) && (I1 <= (nXYZ+nBonds))) {
				if (del_Object.nBonds < MAX_DELETED_OBJ){
					I2 = I1 - nXYZ;
					bAtom[I2-1].show = 'n';
					del_Object.del_Bonds[del_Object.nBonds] = I2 - 1;
					del_Object.nBonds++;
				}
			}
			if ((I1 > (nXYZ+nBonds)) && (I1 <= (nXYZ+nBonds+nPoly))) {
				if (del_Object.nPoly < MAX_DELETED_OBJ){
					I2 = I1 - nXYZ - nBonds;
					tp[I2-1].show = 'd';
					del_Object.del_Poly[del_Object.nPoly] = I2 - 1;
					del_Object.nPoly++;
				}
			}

		}
		objNum = 0;
		pick_obj[0] = 0;
		set_display_List();
		m3Datom_RenderScene();
		break;
			  }

	case(27):{
		reset_all_deleted_objects();
		set_display_List();
		m3Datom_RenderScene();
		break;
			 }
	}
}


/***************************************** m3Datom_Mouse() ******************/
void m3Datom_Mouse(int button, int button_state, int x, int y )
{
	int objID,i;
	int control_keyID;
	int animateID = 0;

	motion = 0;
	move_hkl = 0;

	if (orientation == 1) return;

	animateID = tbMouse(button, button_state, x, y, rotation_mode,rotation_type);
	if (animateID == 1) glutIdleFunc(m3Datom_Idle);
	control_keyID = glutGetModifiers();
	if ( button == GLUT_LEFT_BUTTON  && button_state == GLUT_DOWN ){
		motion = 1;
		last_x = x;
		last_y = y;
		statusbarinfo->set_name(" ");
		switch(tools_mode)
		{
		case(0):{glutSetCursor(GLUT_CURSOR_CROSSHAIR); break;}
		case(1):{glutSetCursor(GLUT_CURSOR_UP_DOWN); break;}
		case(2):{glutSetCursor(GLUT_CURSOR_INFO); break;}
		case(3):{
			if (objNum==2) objNum = 0; 
			model_pick=0;
			PickUpObj(x,y); 
			break;
				}
		case(4):{
			if((control_keyID != GLUT_ACTIVE_SHIFT) && (objNum==3)) objNum = 0;
			if (objNum ==4) objNum = 0;
			model_pick=0;
			PickUpObj(x,y); 
			break;
				}
		case(5):{
			if (glutGetModifiers() != GLUT_ACTIVE_SHIFT) objNum = 0; 
			model_pick=0; 
			PickUpObj(x,y); 
			objID = 0;
			if (origin_hkl == 1){
				for (i=0; i<objNum; i++){
					if (nXYZ+nBonds+nPoly+1 == pick_obj[i]) {objID = 1; break;}
					else if (nXYZ+nBonds+nPoly+2 == pick_obj[i]) {objID = 2; break;}
				}
			}
			move_hkl = objID;
			break;
				}
		case(6):{glutSetCursor(GLUT_CURSOR_UP_DOWN); break;}
		}
		if ((tools_mode == 0) && (rotation_type == 2)) {
			tbClickMotion(x,y,rotation_mode,fabs(stepw_angle));
		}
	}
	if ((tools_mode == 5) && (objID == 1)) glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		

	if ( button == GLUT_LEFT_BUTTON  && button_state == GLUT_UP ){
		motion = 0;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		if ((tools_mode == 3) && (objNum == 2)) calcAtomDist(0);
		if (tools_mode == 4){
			if ((glutGetModifiers() != GLUT_ACTIVE_SHIFT) && (objNum == 3)) calcAtomAngle();
			if ((glutGetModifiers() == GLUT_ACTIVE_SHIFT) && (objNum == 4)) calcAtomTorsAngle();
		}
		if ((tools_mode == 5) && (objNum == 1)) {
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
				if ((pick_obj[0] > nXYZ+nBonds) && (pick_obj[0] <= (nXYZ+nBonds+nPoly))) {
					output_poly_and_BVS_info();
				}
				else if ((pick_obj[0] <= nXYZ) && (spin_dialog_ID == 0)) set_spin_for_atom(pick_obj[0]);
			}
			else output_object_info();
		}
	}
	if (motion == 1) stw_btn->disable();
	else if ((tools_mode == 0) && (rotation_type == 2)) stw_btn->disable();
	else stw_btn->enable();
}


/**************************************** m3Datom_Reshape() *****************/
void m3Datom_Reshape( int x, int y )
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
	case(0):{glFrustum( -xy_aspect*0.02, xy_aspect*0.02, -0.02, 0.02, 0.1, 100.0 );break;}
	case(1):{glOrtho( -xy_aspect*0.8, xy_aspect*0.8, -0.8, 0.8, 0.1, 10.0 );break;}
	}
	glMatrixMode( GL_MODELVIEW );

	updateStatusBar();

	glutPostRedisplay();
}



/**************************************** m3Datom_Display() *****************/
void m3Datom_Display( void )
{
	int chk = -1;

	glClearColor(BKCL[0],BKCL[1],BKCL[2], 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	switch(projection_type)
	{
	case(0):{glFrustum( -xy_aspect*0.02, xy_aspect*0.02, -0.02, 0.02, 0.1, 100.0 );break;}
	case(1):{glOrtho( -xy_aspect*0.8, xy_aspect*0.8, -0.8, 0.8, 0.1, 10.0 );break;}
	}
	glMatrixMode( GL_MODELVIEW );

	if (start_VICS == -1) {
		glutReshapeWindow(10,10);
		glutReshapeWindow(graph_w,graph_h);
		rollout_prop->open();
		rollout_prop->close();
		menu_glui->hide();
		menu_glui->show();
		start_VICS = 0;
	}

//--- Get Image Settings and Start Export
	if( run_img_export == 1){
		chk = m3Datom_window_dump1();
		if (chk == 0) {
			chk = -1;
			chk_image_ID = -2;
		}
	}

	m3Datom_RenderScene();

	glFlush();
	glutSwapBuffers(); 
}


/**************************************** m3Datom_control() *****************/
void m3Datom_control( int control )
{
	int i;
	int file_control = 0;
	int file_exist;
	FILE *fptr;

	switch(control)
	{
//====================================================================== New Data
	case(1):{
		if (new_edit_dialog_ID == 0){
			new_edit_dialog_ID = -1;
			init_data_settings1();
			clear_display_list();
			initial_new_edit_file_settings();
			open_new_edit_file_dialog(main_window,0,dwp[0][3],dwp[1][3]);
		}
		break;
			}
//====================================================================== Load Data from File
	case(2):{
		file_control = -1;
		sprintf(FileName,"%s"," ");
		fileID = def_file_format_ID;
		file_exist = 0;
		glutSetWindow(main_window);  
//		while(file_exist != 1){
			file_control = open_file_dialog(fileID,FileName);
			if (file_control > 0){
				fptr = fopen(FileName,"r");
				if( fptr != NULL ) {
					fclose(fptr);
					file_exist = 1;
				}
				else open_message_dialog3(main_window,1,FileName);
			}
			else file_exist = 1;
//		}

			if (file_exist == 1){
		glutSetWindow(main_window);  
		if (file_control > 0) {
			rotation_type = 0;
			rotation_tipe_radio->set_int_val(rotation_type);
			updateStatusBar();
			if ((fileID == 3) || (fileID == 14)) file_control = readdata(fileID,FileName,1);
			else file_control = readdata(fileID,FileName,0);  
			if (file_control != -1) {
				if (file_control == -2) {
					message_dialog_ID3 = -1;
					open_message_dialog3(main_window,0,"");
				}
				else if (file_control == -3) {
					message_dialog_ID3 = -1;
					open_message_dialog3(main_window,2,"");
				}
				_update__window_title();
				start_VICS = 1;
				m3Datom_TB_init();
				load_data(2);
				set_lights();
			}
			else {
				message_dialog_ID2 = -1;
				open_message_dialog2(main_window,1);
			}
		}
			}
		break;
			}
//====================================================================== Save Data to File
	case(3):{
		if (start_VICS == 0) break;
		file_control = -1;
		sprintf(FileName,"%s"," ");
		file_control = save_file_dialog(sfileID,FileName,0);
		glutSetWindow(main_window);  
		if (file_control > 0){
			check_file_ext_exist(sfileID,FileName,0);
			getOrientation(c_rotation);
			save_data(FileName);
			file_control = readdata(0,FileName,0);  
			if (file_control != -1) {
				m3Datom_TB_init();
				load_data(2);
				set_lights();
			}
			else {
				message_dialog_ID2 = -1;
				open_message_dialog2(main_window,2);
			}
		}
		break;
			}
//====================================================================== Export Image
	case(301):{
		if (start_VICS == 0) break;
		if (image_dialog_ID == 0) {
			file_control = -1;
			sprintf(img_fName,"");
			file_control = save_file_dialog(img_format, img_fName, 2);
			glutSetWindow(main_window);  
			if (file_control > 0){
				check_file_ext_exist(img_format, img_fName,2);
				image_dialog_ID = -1;
				set_Export_param(img_fName,img_format);
				Export_Dialog(main_window);
			}
		}
		break;
			  }
//====================================================================== Export 
	case(4):{
		if (start_VICS == 0) break;
		file_control = -1;
		sprintf(e_FileName,"%s"," ");
		file_control = save_file_dialog(e_fileID,e_FileName,1);
		if (file_control > 0) {
			check_file_ext_exist(e_fileID,e_FileName,1);
			if ((e_fileID == 0) || (e_fileID == 2)){
				if (use_aniso == 1) {
					if(message_dialog_ID1 == 0){
						message_dialog_ID1 = -1;
						open_message_dialog1(main_window);
					}
				}
				else export_mode = 1;
				export_data(e_fileID,e_FileName);
			}
			else {
				export_mode = 0;
				export_data(e_fileID,e_FileName);
			}
		}
		break;
			}
//====================================================================== Edit Data
	case(5):{
		if (start_VICS == 0) break;
		if (new_edit_dialog_ID == 0){
			new_edit_dialog_ID = -1;
			initial_new_edit_file_settings();
			set_new_edit_file_dialog_param(Title,cCell.SpGrN,cCell.Setting,cCell.a,cCell.ea,nAtom,Btype,
				                           use_aniso,range,progvar,use_iso);
			for(i=0; i<nAtom; i++){
				set_struc_param(i,lAtom[i].name,lAtom[i].simb,lAtom[i].g,lAtom[i].x,lAtom[i].y,lAtom[i].z, 
                                  lAtom[i].ex,lAtom[i].ey,lAtom[i].ez,lAtom[i].B,lAtom[i].Biso);
			}
			open_new_edit_file_dialog(main_window,1,dwp[0][3],dwp[1][3]);
		}
		break;
			}
//====================================================================== Bonds
	case(6):{
		if (start_VICS == 0) break;
		if (bonds_dialog_ID == 0){
			bonds_dialog_ID = -1;
			init_bonds_dialog(def_mol_bond,nAtom,bond_sNum,hydrogen_bonds_id);
			for(i=0; i<nAtom; i++) set_atoms_to_bonds_dialog(i,lAtom[i].simb);
			for(i=0; i<bond_sNum; i++) set_bonds_to_bonds_dialog(i,bini[i].A1,bini[i].A2,bini[i].d,
				                       bini[i].search_mode,bini[i].boundary_mode,bini[i].show_poly_mode);
			open_bonds_dialog(main_window,dwp[0][10],dwp[1][10]);
		}
		break;
			}
//====================================================================== About
	case(7):{
		if (about_dialog_ID == 0){
			about_dialog_ID = -1;
			open_about_dialog(main_window,dwp[0][0],dwp[1][0]);
		}
		break;
			}
//====================================================================== (hkl) Plane Properties
	case(9):{
		if (start_VICS == 0) break;
		if (hkl_prop_dialog_ID == 0){
			hkl_prop_dialog_ID = -1;
			set_hkl_prop_dialog(show_hkl, origin_hkl, show_hkl_inv, show_hkl_out, hkl, hkl_rgb, hkl_trans, hkl_out_width);
			set_cell_param(cCell.a);
			open_hkl_prop_dialog(main_window,dwp[0][2],dwp[1][2]);
		}
		break;
			 }
//====================================================================== Preferences
	case(91):{
		if (start_VICS == 0) break;
		if (prefernces_dialog_ID == 0){
			prefernces_dialog_ID = -1;
			set_preferences_def_param(def_atom_s_stick,def_atom_s_slice,def_atom_o_stick,def_atom_o_slice,
				                      def_bond_s_stick,def_bond_s_slice,def_bond_o_stick,def_bond_o_slice,
									  def_ucel_line_width,def_poly_line_width,def_hkl_line_width,def_bond_line_width,
									  def_poly_tran,def_hkl_tran,
									  def_bkgr_color,def_ucel_color,def_poly_color,def_hkl_color,def_bond_color,
									  def_bond_rad,def_show_axis,stepw_angle,def_mol_bond,def_search_bond,
									  def_file_format_ID,t_FileName,save_pos_ID,save_dpos_ID);
			set_preferences_cur_param(atoms_stack,atoms_slice,out_atoms_stack,out_atoms_slice,
				                      bonds_stack,bonds_slice,out_bonds_stack,out_bonds_slice,
									  ucell_width,poly_outline_width,hkl_out_width,bonds_line_width,
									  poly_trans,hkl_trans,
									  BKCL,ucell_color,poly_line_color,hkl_rgb,BNCL,
									  bRadii,show_axis);
			open_preferences_dialog(main_window,dwp[0][7],dwp[1][7]);
		}
		break;
			 }
//====================================================================== Geneal Properties
	case(10):{
		if (start_VICS == 0) break;
		if (general_prop_dialog_ID == 0){
			general_prop_dialog_ID = -1;
			init_rendering_dialog(m_atom_ambient, m_atom_diffuse, m_atom_specular, m_atom_shininess,
		  	    			      m_bond_ambient, m_bond_diffuse, m_bond_specular, m_bond_shininess,
						          m_poly_ambient, m_poly_diffuse, m_poly_specular, m_poly_shininess,
						          m_plane_ambient, m_plane_diffuse, m_plane_specular, m_plane_shininess,
						          l_ambient, l_diffuse, l_direction, BKCL, show_axis,
								  projection_type,perspective,radii_type,fog);
			open_rendering_dialog(main_window,dwp[0][9],dwp[1][9]);
		}
		break;
			 }
//====================================================================== Atoms Properties
	case(11):{
		if (start_VICS == 0) break;
		if (atoms_prop_dialog_ID == 0){
			atoms_prop_dialog_ID = -1;
			set_atom_prop_main_params(tAtom,atoms_stack,atoms_slice,atom_type,TEOL,Probability,use_aniso);
			for(i=0; i<tAtom; i++){
				set_atom_prop_params(i, AtomT[i].symb,AtomT[i].R,AtomT[i].color);
			}
			open_atoms_prop_dialog(main_window,dwp[0][1],dwp[1][1]);
		}
		break;
			 }
//====================================================================== Bonds Properties
	case(12):{
		if (start_VICS == 0) break;
		if (bonds_prop_dialog_ID == 0){
			bonds_prop_dialog_ID = -1;
			set_bond_prop_params(bonds_type,bonds_stack,bonds_slice,bRadii,bonds_line_width,BNCL,MODL,
				hbond_line_type);
			open_bonds_prop_dialog(main_window,dwp[0][14],dwp[1][14]);
		}
		break;
			 }
//====================================================================== Polyhedra Properties
	case(13):{
		if (start_VICS == 0) break;
		if (poly_prop_dialog_ID == 0){
			poly_prop_dialog_ID = -1;
			set_poly_prop(poly_type, poly_trans, poly_outline_width, poly_line_color, poly_outline);
			open_poly_prop_dialog(main_window,dwp[0][6],dwp[1][6]);
		}
		break;
			 }
//====================================================================== Unit Cell Outline Properties
	case(14):{
		if (start_VICS == 0) break;
		if (ucell_prop_dialog_ID == 0){
			ucell_prop_dialog_ID = -1;
			set_ucell_prop_dialog(SHCL,ucell_width,ucell_color,ucell_type);
			open_ucell_prop_dialog(main_window,dwp[0][13],dwp[1][13]);
		}
		break;
			 }
//====================================================================== Model Mode
	case(20):{
		MODL = model_mode + 1;
		if ((model_mode == 1) || (model_mode == 2)) dot_surf_check->disable();
		else dot_surf_check->enable();
		dot_surf_check->set_int_val(dot_surf_ID);
		set_display_List();
		break;
			 }
//====================================================================== Model Mode
	case(21):{
		dot_surf_ID = dot_surf_check->get_int_val();
		set_display_List();
		break;
			 }
//====================================================================== Plot range
	case(37):{
		if (start_VICS == 0) break;
		if (plot_range_dialog_ID == 0){
			plot_range_dialog_ID = -1;
			set_plot_range_param(range);
			open_plot_range_dialog(main_window,dwp[0][8],dwp[1][8]);
		}
		break;
			 }
//====================================================================== Orientation
	case(36):{
		if (start_VICS == 0) break;
		if (corient_dialog_ID == 0){
			corient_dialog_ID = -1;
			getOrientation(c_rotation);
			init_orient_dialog_param(c_rotation,cCell.a);
			open_orient_prop_dialog(main_window,dwp[0][4],dwp[1][4]);
		}
		break;
			 }
//====================================================================== Tools Mode
	case (30):
	case (31):
	case (32):
	case (33):
	case (34):
	case (35):{
		objNum = 0;
		model_pick = 0;
		set_display_List();
		sprintf(infotext,"%s"," ");
		statusbarinfo->set_name(infotext);
		tools_mode = control - 30;
		if (tools_mode != 0) rotation_panel->disable();
		                else rotation_panel->enable();
		break;
			  }
//====================================================================== Rotation Type
	case (40):{
		if (rotation_type==1) tbAnimate(GL_TRUE);
		else tbAnimate(GL_FALSE);
		break;
			  }
//====================================================================== Stepwise rotation
	case (41):{
		if (start_VICS == 0) break;
		if (stepwise_dialog_ID == 0){
			stepwise_dialog_ID = -1;
			rotation_tipe_radio->disable();
			bFree->disable();
			bX->disable();
			bY->disable();
			bZ->disable();
			updateStatusBar1();
			init_stepwise_dialog(stepw_angle,stepw_mode);
			open_stepwise_dialog(main_window,dwp[0][12],dwp[1][12]);
		}
		break;
			  }
//====================================================================== Rotation Mode
	case (50):
	case (51):
	case (52):
	case (53):
	case (54):
	case (55):
	case (56):{rotation_mode = control - 50;break;}
//====================================================================== Projection Type
		}

	updateStatusBar();
	glutPostRedisplay();
}


/**************************************** init_program_setting() ************/
void init_program_setting()
{
	int i;
//--------------------------------------------------- Initial Windows Positions
	save_pos_ID = 0;
	menu_x  =  10; menu_y  = 30;
	tools_x =  10; tools_y = 90;
	graph_x = 210; graph_y = 90;
    graph_w = 500; graph_h = 600;
//-----------------------------------------------------
	sprintf(t_FileName,"");
	sprintf(t_FileName,"%s%s",progvar,TEMPLATE);
//-----------------------------------------------------
	for(i=0; i<15; i++) {
		cdwp[0][i] = dwp[0][i] = 180; 
		cdwp[1][i] = dwp[1][i] = 140;
	}
	loadDefPar();
	load_default_pref();
	init_data_settings1();
//--------------------------------------------------- Initial Dialog Settings
	arg_openfile_ID = 0;
	openfile_dialog_ID = 0;
	new_edit_dialog_ID = 0;
	plot_range_dialog_ID = 0;
	bonds_dialog_ID = 0;
	general_prop_dialog_ID = 0;
	atoms_prop_dialog_ID = 0;
	bonds_prop_dialog_ID = 0;
	poly_prop_dialog_ID = 0;
	ucell_prop_dialog_ID = 0;
	hkl_prop_dialog_ID = 0;
	prefernces_dialog_ID = 0;
	message_dialog_ID1 = 0;
	message_dialog_ID2 = 0;
	message_dialog_ID3 = 0;
	image_dialog_ID = 0;
	corient_dialog_ID = 0;
	about_dialog_ID = 0;
	stepwise_dialog_ID = 0;
	spin_dialog_ID = 0;
	color_dialog_ID = 0;
	poly_type_dialog_ID = 0;
	chk_image_ID = 0;
	orig_dialog_ID = 0;

//--------------------------------------------------- Initial Settings 1
	origin[0] = origin[1] = origin[2] = 0;
	n_list = 0;
	del_Object.nAtom = del_Object.nBonds = del_Object.nPoly = 0;
//--------------------------------------------------- Initial Settings 2
	stepwise_active = 0;
	stepw_mode = 0;
	stepw_rot_axis = 0;
	stepw_rot[0] = stepw_rot[1] = stepw_rot[2] = 0;
//-----------------------------------------------------
	set_lights();
//-----------------------------------------------------
	shift_compass[0] = shift_compass[1] = 0;
}


//***********************************************************************//
//               main()                                                  //
//***********************************************************************//
int main(int argc, char* argv[])
{
	int num[2];

	if (getenv( "VENUS" ) != NULL) sprintf(progvar,"%s",getenv( "VENUS" ));
	else sprintf(progvar,"");

	init_program_setting();
//-------------------------   Read file from command line
	if (argc>1) {
		fileID = -1;
		arg_fname = argv[1];
		if (argc == 3) fileID = get_format(argv[2]);
		if ((fileID < 0) || (fileID > 19)) fileID = check_format(arg_fname);
		if (fileID >= 0) arg_openfile_ID = -1;
	}
//-------------------------------   Initialize GLUT and create window
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition(graph_x, graph_y);
	glutInitWindowSize(graph_w,graph_h);
	main_window = glutCreateWindow("Graphics");                //--- Creat the Main Window
	glutDisplayFunc( m3Datom_Display );                           //--- Display control
	GLUI_Master.set_glutReshapeFunc( m3Datom_Reshape );           //--- Reshape control
	GLUI_Master.set_glutKeyboardFunc( m3Datom_Keyboard );         //--- Keyboard control
	GLUI_Master.set_glutMouseFunc( m3Datom_Mouse );               //--- Mouse control
	glutMotionFunc( m3Datom_Mouse_Motion );                       //--- Mouse motion control
	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);                       //--- Set Cursor Type
//-------------------------------   Track_Ball Initialization
	m3Datom_TB_init();

//-------------------------------   Create the statusbar subwindow
    gluisub = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );

	gluisub->set_main_gfx_window( main_window );
	statusbar = gluisub->add_statictext("");
	gluisub->add_column(true);
	statusbarinfo = gluisub->add_statictext("");
	updateStatusBar();

//-------------------------------   Output General information
	printf("Video configuration: %s\n",glGetString(GL_RENDERER) );
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS,num); 
	printf("Maximum supported width and height of the viewport: %i x %i\n",num[0],num[1]);

//-------------------------------   Initialize OpenGL main parameters
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable( GL_NORMALIZE );
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glMultMatrixf( l_direction );

//-------------------------------   Create the menu window
	menu_glui = GLUI_Master.create_glui("VICS: Menu",0,menu_x,menu_y);
	menu_glui->set_main_gfx_window( main_window );
//-------------------------------   Add Main Menu Buttons to GLUI window
//--- File Menu
	menu_glui->add_column(true);
	new_btn  = menu_glui->add_button("New", 1, m3Datom_control);new_btn->set_w(50);
	menu_glui->add_column(false);
	open_btn = menu_glui->add_button("Open", 2, m3Datom_control);open_btn->set_w(50);
	menu_glui->add_column(false);
	save_btn = menu_glui->add_button("Save", 3, m3Datom_control);save_btn->set_w(50);
	menu_glui->add_column(false);
	exp_btn  = menu_glui->add_button("Export", 4, m3Datom_control);exp_btn->set_w(50);
	menu_glui->add_column(true);
	img_btn  = menu_glui->add_button("Image", 301, m3Datom_control);img_btn->set_w(50);
	menu_glui->add_column(true);
//--- Edit Menu
	edit_btn = menu_glui->add_button("Edit", 5, m3Datom_control);edit_btn->set_w(50);
	menu_glui->add_column(false);
	bonds_btn = menu_glui->add_button("Bonds", 6, m3Datom_control);bonds_btn->set_w(50);
	menu_glui->add_column(true);
//--- Help Menu
	pref_btn = menu_glui->add_button("Preferences", 91, m3Datom_control);pref_btn->set_w(50);
	menu_glui->add_column(false);
	about_btn = menu_glui->add_button("About", 7, m3Datom_control);about_btn->set_w(50);
//--- Quit
	menu_glui->add_column(true);
	exit_btn = menu_glui->add_button("Exit", 0,(GLUI_Update_CB)exit );exit_btn->set_w(50);

//-------------------------------   Create the tools window
	glui = GLUI_Master.create_glui("Dialog",0,tools_x,tools_y);
	glui->set_main_gfx_window( main_window );
//-------------------------------   Register the idle callback with GLUI
	GLUI_Master.set_glutIdleFunc( m3Datom_Idle );
	main_panel = glui->add_panel("",GLUI_PANEL_RAISED);

//-----------------------------------------------------------------//
//   Properties Rollout                                                   //
//-----------------------------------------------------------------//
	rollout_prop = glui->add_rollout_to_panel(main_panel, "Properties", false );
	glui->add_separator_to_panel(rollout_prop);
	glui->add_button_to_panel(rollout_prop, "General",    10, m3Datom_control );
	glui->add_separator_to_panel(rollout_prop);
	glui->add_button_to_panel(rollout_prop, "Atoms",      11, m3Datom_control );
	glui->add_button_to_panel(rollout_prop, "Bonds",      12, m3Datom_control );
	glui->add_button_to_panel(rollout_prop, "Polyhedra",  13, m3Datom_control );
	glui->add_button_to_panel(rollout_prop, "Unit cell",  14, m3Datom_control );
	glui->add_separator_to_panel(rollout_prop);
//-----------------------------------------------------------------//
//   Model Panel                                                   //
//-----------------------------------------------------------------//
	model_panel = glui->add_panel_to_panel(main_panel,"Models",true);
	model_radio = glui->add_radiogroup_to_panel(model_panel,&model_mode,20,m3Datom_control);
	glui->add_radiobutton_to_group( model_radio, "Ball-and-stick" );
	glui->add_radiobutton_to_group( model_radio, "Space-filling" );
	glui->add_radiobutton_to_group( model_radio, "Polyhedra" );
	glui->add_radiobutton_to_group( model_radio, "Wire-frame" );
	glui->add_radiobutton_to_group( model_radio, "Stick" );
	glui->add_separator_to_panel(model_panel);
	dot_surf_check = glui->add_checkbox_to_panel(model_panel,"Show dot surface",NULL,21,m3Datom_control);
	dot_surf_check->set_int_val(0);

	
//-----------------------------------------------------------------//
//  Plot Range
//-----------------------------------------------------------------//
	glui->add_button_to_panel(main_panel,"Plot range",37,m3Datom_control);
//-----------------------------------------------------------------//
//  Crystal Orientation
//-----------------------------------------------------------------//
	bOrient = glui->add_button_to_panel(main_panel,"Orientation",36,m3Datom_control);
//-----------------------------------------------------------------//
//  (hkl) plane
//-----------------------------------------------------------------//
	hkl_btn = glui->add_button_to_panel(main_panel,"Lattice plane", 9, m3Datom_control);
	glui->add_separator_to_panel(main_panel);
//-----------------------------------------------------------------//
//   Tools Panel                                                   //
//-----------------------------------------------------------------//
	tools_panel = glui->add_panel_to_panel(main_panel,"Manipulations",true);
	bR = glui->add_button_to_panel(tools_panel,"Rotate",30,m3Datom_control);
	bM = glui->add_button_to_panel(tools_panel,"Magnify",31,m3Datom_control);
	bT = glui->add_button_to_panel(tools_panel,"Translate",32,m3Datom_control);
	glui->add_column_to_panel(tools_panel,false);

	bS = glui->add_button_to_panel(tools_panel,"Select",35,m3Datom_control);
	bD = glui->add_button_to_panel(tools_panel,"Distance",33,m3Datom_control);
	bA = glui->add_button_to_panel(tools_panel,"Angle",34,m3Datom_control);

	bR->set_w(65);
	bM->set_w(65);
	bT->set_w(65);
	bD->set_w(65);
	bA->set_w(65);
	bS->set_w(65);
//-----------------------------------------------------------------//
//   Rotation Panel                                                //
//-----------------------------------------------------------------//
	rotation_panel = glui->add_panel_to_panel(main_panel,"Rotation modes",true);
	stw_btn = glui->add_button_to_panel(rotation_panel,"Stepwise",41,m3Datom_control);
	glui->add_separator_to_panel(rotation_panel);
	rotation_tipe_radio = glui->add_radiogroup_to_panel(rotation_panel,&rotation_type,40,m3Datom_control);
	glui->add_radiobutton_to_group(rotation_tipe_radio,"Drag");
	glui->add_radiobutton_to_group(rotation_tipe_radio,"Push");
	glui->add_radiobutton_to_group(rotation_tipe_radio,"Click");
	rotation_tipe_radio->set_int_val(rotation_type);
	glui->add_separator_to_panel(rotation_panel);
	bFree = glui->add_button_to_panel(rotation_panel,"Free" ,50,m3Datom_control);
	bX = glui->add_button_to_panel(rotation_panel,"X axis",51,m3Datom_control);
	bY = glui->add_button_to_panel(rotation_panel,"Y axis",52,m3Datom_control);
	bZ = glui->add_button_to_panel(rotation_panel,"Z axis",53,m3Datom_control);

//-------------------------------   Run 
	glutMainLoop();

	return 0;
}
