#include <math.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include "tools.h"
#include "contour.h"

int winW, winH;
float xy_aspect2D;
int windowID;
int	ParentWindow;
float winrange[] = {0.75f, 0.75f};
int	last_y;
float shiftY = 0.9;
short Palette[100][6];
float outl_cell[4][2];
float maxP;
float Shift[] = {0.5,0.5};
float orig[2];
float DD[MAX_PIX][MAX_PIX];
float transl[2][2];
float label_color;
char dm_max[128];
float maxD, minD;
int MaxPix[2];


float VXY,COST;
float VX,VY,THETA;
float MAX_XY;
int MAX_PX;
float max_PPP;

int Type;

int IMOD(int ix, int iy)
{
	int ires;
	if (iy == 0) return 0;

	ires = ix - (int)((float)ix/(float)iy) * iy;
	return(ires);
}


/**************************************** get_MAX_den() *********************/
float get_MAX_den()
{
	return(maxD);
}

/**************************************** get_MAX_den() *********************/
float get_MIN_den()
{
	return(minD);
}

/**************************************** _update_2Dwindow_title() **********/
void _update_2Dwindow_title()
{
	char *pdest;
	char nTitle[300];

	sprintf(nTitle,"");
	pdest = strrchr(FileName, '\\');
    if (pdest == NULL) pdest = strrchr(FileName, '/');
    if (pdest != NULL){
       pdest++;
       sprintf(nTitle,"2D Map: %s",pdest);
	}
    else {
		if (strlen(FileName) > 0) sprintf(nTitle,"2D Map: %s",FileName);
		else sprintf(nTitle,"2D Map");
	}
	glutSetWindowTitle(nTitle);
}



//**************************  draw_string_bitmap()   ************************//
void draw_string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}

//**************************  set_palette()   *******************************//
void set_palette(short Pl[100][6])
{
	int i,j;
	for(i=0; i<100; i++){
		for(j=0; j<6; j++) Palette[i][j] = Pl[i][j];
	}
}

void  clear_contor_list()
{
	glNewList(4,GL_COMPILE);
	glEndList();
}

void clear_color_bar()
{
	glNewList(2,GL_COMPILE);
	glEndList();
}

void clear_map_cell()
{
	glNewList(3,GL_COMPILE);
	glEndList();
}

void clear_grid_cell()
{
	glNewList(5,GL_COMPILE);
	glEndList();
}


//**************************  map_HKL_Cell()   ******************************//
void map_HKL_Cell(int hkl[3], float orig)
{
	int jh,jv;
	float maxiD;
	float R0[3],R1[3],R2[3];
	float AA,BB,CC,CA,CB,CG;
	float DX,DY,DZ,DHX,DHY,DHZ,DVX,DVY,DVZ;
	float FX,FY,FZ,FMH,FMV;
	float FH,FV;
	float X,Y,Z,SX,SY,SZ,DDX,DDY,DDZ;
	int IX,IY,IZ,KX,KY,KZ,KX1,KY1,KZ1;
	float P1,P2,P3,P4,P5,P6,DDD;
	int NPX,NPY;
	float shift;
	int ID,sign_ID;


	if ((hkl[0] != 0) && (hkl[1] != 0) && (hkl[2] != 0)) ID = 0;
	if ((hkl[0] != 0) && (hkl[1] != 0) && (hkl[2] == 0)) ID = 1;
	if ((hkl[0] != 0) && (hkl[1] == 0) && (hkl[2] != 0)) ID = 2;
	if ((hkl[0] == 0) && (hkl[1] != 0) && (hkl[2] != 0)) ID = 3;
	switch(ID)
	{
	case(0):{   //---------  (hkl)
		sign_ID = 0;
		for (jh=0; jh<3; jh++){if (hkl[jh] < 0) sign_ID++;}
		if (sign_ID > 1) {
			hkl[0] = -1*hkl[0];
			hkl[1] = -1*hkl[1];
			hkl[2] = -1*hkl[2];
		}
		R0[0] = 0.0;           R0[1] = 0.0;           R0[2] = 1.0/(float)hkl[2];
		R1[0] = 1.0/(float)hkl[0]; R1[1] = 0.0;           R1[2] = 0.0;
		R2[0] = 0.0;           R2[1] = 1.0/(float)hkl[1]; R2[2] = 0.0;
		if (hkl[2] < 0) {
			R1[0] = R1[0]; R1[1] = 0.0;   R1[2] = -R0[2];
			R2[0] = 0.0;   R2[1] = R2[1]; R2[2] = -R0[2];
			R0[0] = 0.0;   R0[1] = 0.0;   R0[2] = 0.0;
		}
		DX = R2[0] - R1[0];
		DY = R2[1] - R1[1];
		DZ = R2[2] - R1[2];
		DHX = R2[0] - R0[0];
		DHY = R2[1] - R0[1];
		DHZ = R2[2] - R0[2];
		DVX = R1[0] - R0[0];
		DVY = R1[1] - R0[1];
		DVZ = R1[2] - R0[2];
		break;
			}
	case(1):{   //---------  (hk0)
		if ((hkl[0] < 0) && (hkl[1] < 0)) {
			hkl[0] = -1*hkl[0];
			hkl[1] = -1*hkl[1];
		}
		R0[0] = 1.0/(float)hkl[0]; R0[1] = 0.0;               R0[2] = 0.0;
		R1[0] = 0.0;               R1[1] = 1.0/(float)hkl[1]; R1[2] = 0.0;
		R2[0] = 0.0;               R2[1] = 0.0;               R2[2] = 1.0;
		if ((hkl[0] > 0) && (hkl[1] > 0)) {
			DHX = R0[0] - R1[0];
			DHY = R0[1] - R1[1];
			DHZ = R0[2] - R1[2];
		}
		else {
			DHX = fabs(R0[0]) + fabs(R1[0]);
			DHY = fabs(R0[1]) + fabs(R1[1]);
			DHZ = fabs(R0[2]) + fabs(R1[2]);
		}
		DVX = R2[0]; 
		DVY = R2[1]; 
		DVZ = R2[2];
		DX = R2[0] - DHX;
		DY = R2[1] - DHY;
		DZ = R2[2] - DHZ;
		break;
			}
	case(2):{   //---------  (h0l)
		if ((hkl[0] < 0) && (hkl[2] < 0)) {
			hkl[0] = -1*hkl[0];
			hkl[2] = -1*hkl[2];
		}
		R0[0] = 0.0;               R0[1] = 0;                 R0[2] = 1.0/(float)hkl[2];
		R1[0] = 1.0/(float)hkl[0]; R1[1] = 0.0;               R1[2] = 0.0;
		R2[0] = 0.0;               R2[1] = 1.0;               R2[2] = 0.0;
		if ((hkl[0] > 0) && (hkl[2] > 0)) {
			DHX = R0[0] - R1[0];
			DHY = R0[1] - R1[1];
			DHZ = R0[2] - R1[2];
		}
		else {
			DHX = fabs(R0[0]) + fabs(R1[0]);
			DHY = fabs(R0[1]) + fabs(R1[1]);
			DHZ = fabs(R0[2]) + fabs(R1[2]);
		}
		DVX = R2[0]; 
		DVY = R2[1]; 
		DVZ = R2[2];
		DX = R2[0] - DHX;
		DY = R2[1] - DHY;
		DZ = R2[2] - DHZ;
		break;
			}
	case(3):{   //---------  (0kl)
		if ((hkl[1] < 0) && (hkl[2] < 0)) {
			hkl[1] = -1*hkl[1];
			hkl[2] = -1*hkl[2];
		}
		R0[0] = 0.0;               R0[1] = 0;                 R0[2] = 1.0/(float)hkl[2];
		R1[0] = 0.0;               R1[1] = 1.0/(float)hkl[1]; R1[2] = 0.0;
		R2[0] = 1.0;               R2[1] = 0.0;               R2[2] = 0.0;
		if ((hkl[1] > 0) && (hkl[2] > 0)) {
			DHX = R0[0] - R1[0];
			DHY = R0[1] - R1[1];
			DHZ = R0[2] - R1[2];
		}
		else {
			DHX = fabs(R0[0]) + fabs(R1[0]);
			DHY = fabs(R0[1]) + fabs(R1[1]);
			DHZ = fabs(R0[2]) + fabs(R1[2]);
		}
		DVX = R2[0]; 
		DVY = R2[1]; 
		DVZ = R2[2];
		DX = R2[0] - DHX;
		DY = R2[1] - DHY;
		DZ = R2[2] - DHZ;
		break;
			}
	}
	maxD = -10000000.0f;
	minD =  10000000.0f;

	AA = cCell[0]; BB = cCell[1]; CC = cCell[2];
	CA = (float)cos(PI*cCell[3]/180.0);
	CB = (float)cos(PI*cCell[4]/180.0);
	CG = (float)cos(PI*cCell[5]/180.0);

	shift = 1.0/(abs(hkl[0]) + abs(hkl[1]) + abs(hkl[2]));
	shift = orig - shift;
//	for (jh=0; jh<3; jh++) R0[jh] += shift;

	VX = AA*AA*DHX*DHX + BB*BB*DHY*DHY + CC*CC*DHZ*DHZ + 
		 2.0*BB*CC*CA*DHY*DHZ + 2.0*CC*AA*CB*DHZ*DHX + 2.0*AA*BB*CG*DHX*DHY;
	VX = (float)sqrt(VX);
	VY = AA*AA*DVX*DVX + BB*BB*DVY*DVY + CC*CC*DVZ*DVZ + 
		 2.0*BB*CC*CA*DVY*DVZ + 2.0*CC*AA*CB*DVZ*DVX + 2.0*AA*BB*CG*DVX*DVY;
	VY = (float)sqrt(VY);
	VXY = AA*AA*DX*DX + BB*BB*DY*DY + CC*CC*DZ*DZ + 
	      2.0*BB*CC*CA*DY*DZ + 2.0*CC*AA*CB*DZ*DX + 2.0*AA*BB*CG*DX*DY;
	VXY = (float)sqrt(VXY);
	MAX_XY = (VX > VY) ? VX : VY;
	MAX_PX = (NPIX[0] > NPIX[1]) ? NPIX[0] : NPIX[1];
	MAX_PX = (MAX_PX > NPIX[2]) ? MAX_PX : NPIX[2];
	NPX = (int)((VX/MAX_XY)*(float)MAX_PX);
	NPY = (int)((VY/MAX_XY)*(float)MAX_PX);
	COST = (VX*VX + VY*VY - VXY*VXY)/(2.0*VX*VY);
	THETA = (float)acos(COST);
	FMH = (float)NPX;
	FMV = (float)NPY;
	FX = (float)NPIX[0];
	FY = (float)NPIX[1];
	FZ = (float)NPIX[2];

	for(jh=0; jh<NPX; jh++){
		for(jv=0; jv<NPY; jv++){
			FH = (float)(jh)/FMH;
			FV = (float)(jv)/FMV;
			X = R0[0] + DHX*FH + DVX*FV;
			Y = R0[1] + DHY*FH + DVY*FV;
			Z = R0[2] + DHZ*FH + DVZ*FV;
			SX  = (5.0 + X)*FX;       
			SY  = (5.0 + Y)*FY;       
			SZ  = (5.0 + Z)*FZ;
			IX  = (int)SX;            
			IY  = (int)SY;            
			IZ  = (int)SZ;
			KX = IX - (IX/NPIX[0])*NPIX[0];
			KY = IY - (IY/NPIX[1])*NPIX[1];
			KZ = IZ - (IZ/NPIX[2])*NPIX[2];
			KX1 = (IX+1) - ((IX+1)/NPIX[0])*NPIX[0];
			KY1 = (IY+1) - ((IY+1)/NPIX[1])*NPIX[1];
			KZ1 = (IZ+1) - ((IZ+1)/NPIX[2])*NPIX[2];
			DX = SX - (float)(IX);    
			DY = SY - (float)(IY);   
			DZ = SZ - (float)(IZ);
			DDX = 1.0 - DX;           
			DDY = 1.0 - DY;           
			DDZ = 1.0 - DZ;
			P1 = DDX*DELC[KX ][KY ][KZ ] + DX*DELC[KX1][KY ][KZ ];
			P2 = DDX*DELC[KX ][KY1][KZ ] + DX*DELC[KX1][KY1][KZ ];
			P3 = DDX*DELC[KX ][KY ][KZ1] + DX*DELC[KX1][KY ][KZ1];
			P4 = DDX*DELC[KX ][KY1][KZ1] + DX*DELC[KX1][KY1][KZ1];
			P5 = DDZ*P1 + DZ*P3;
			P6 = DDZ*P2 + DZ*P4;
			DDD = DDY*P5 + DY*P6;
			minD = (minD < DDD) ? minD : DDD;
			maxD = (maxD > DDD) ? maxD : DDD;
			DD[jh][jv] = DDD;
		}
	}
	MaxPix[0] = NPX-1;
	MaxPix[1] = NPY-1;
	max_PPP = MAX_XY;

	maxiD = ((float)fabs(maxD) >= (float)fabs(minD)) ? (float)fabs(maxD) : (float)fabs(minD);
	sprintf(dm_max,"");
	if (fabs(maxD) >= fabs(minD)){
		if (maxiD <= 10000) {
			sprintf(dm_max,"Z(max) =%10.3f",maxiD);
		}
		else sprintf(dm_max,"Z(max) = %f",maxiD);
	}
	else {
		if (maxiD <= 10000) {
			sprintf(dm_max,"|Z(max)| =%10.3f",maxiD);
		}
		else sprintf(dm_max,"|Z(max)| = %f",maxiD);
	}

	if ((hkl[0] != old_hkl[0]) || (hkl[1] != old_hkl[1]) || (hkl[2] != old_hkl[2])){
		printf("\n");
		printf("*************************************\n");
		printf("    2D map output for (%i %i %i) plane\n", hkl[0],hkl[1],hkl[2]);
		printf("-------------------------------------\n");
		printf(" Section:\n");
		printf("       X = %8.4f Angstrom\n",VX);
		printf("       Y = %8.4f Angstrom\n",VY);
		printf("     phi = %8.3f deg\n\n",THETA*180.0/PI);
		printf(" Directions:\n");
		printf("       X: [%6.3f %6.3f %6.3f]\n",DHX,DHY,DHZ);
		printf("       Y: [%6.3f %6.3f %6.3f]\n\n",DVX,DVY,DVZ);
		printf(" Origin:\n");
		printf("          (%6.3f %6.3f %6.3f)\n", R0[0],R0[1],R0[2]);
		printf("*************************************\n");
		printf("\n\n");
		old_hkl[0] = hkl[0];
		old_hkl[1] = hkl[1];
		old_hkl[2] = hkl[2];
	}
}


//**************************  map_H00_Cell()   ******************************//
void map_H00_Contour(int mode, int type, int stype, float zMAX[2], float zMIN[2], int NSTP[2],
					 float Step, float cColor[3], float cW)
{
	int i,j,k,m,I1,I2;
	float rv[4][2],v[4][2],r[4][2],ro[4];
	float Z,Z0,ZM;
	int Num;
	float temp1,temp2,dmin,dmax;
	int numC;
	int ID = 1;

	type++;
	Z0 = zMIN[stype];
	ZM = zMAX[stype];
	Num = NSTP[stype];


	glNewList(4,GL_COMPILE);
	glDisable(GL_LIGHTING);
	r[0][0] = 0; r[0][1] = 0;
	r[1][0] = 1; r[1][1] = 0;
	r[2][0] = 1; r[2][1] = 1;
	r[3][0] = 0; r[3][1] = 1;

	glColor3f(cColor[0],cColor[1],cColor[2]);
	glLineWidth(cW);

	for(j=0; j<MaxPix[1]+0; j++){
		for(i=0; i<MaxPix[0]+0; i++){
			for(m=0; m<4; m++){
				I1 = i + r[m][0];
				I2 = j + r[m][1];
				v[m][0] = (float)I1;
				v[m][1] = (float)I2;
				v[m][0] /= ((float)MaxPix[0]+1);
				v[m][1] /= ((float)MaxPix[1]+1);
				ro[m] = DD[I1][I2];
			}
			switch(mode)
			{
//------------------------------------------------------ (100)
			case(0):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*cCell[1] + v[m][1]*cCell[2]*cos(cCell[3]*PI/180);
					rv[m][1] = v[m][1]*cCell[2]*sin(cCell[3]*PI/180);
				}
				max_PPP = max_P;
				break;
					}
//------------------------------------------------------ (010)
			case(1):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*cCell[0] + v[m][1]*cCell[2]*cos(cCell[4]*PI/180);
					rv[m][1] = v[m][1]*cCell[2]*sin(cCell[4]*PI/180);
				}
				max_PPP = max_P;
				break;
					}
//------------------------------------------------------ (001)
			case(2):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*cCell[0] + v[m][1]*cCell[1]*cos(cCell[5]*PI/180);
					rv[m][1] = v[m][1]*cCell[1]*sin(cCell[5]*PI/180);
				}
				max_PPP = max_P;
				break;
					}
//------------------------------------------------------ (hkl)
			case(3):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*VX + v[m][1]*VY*cos(THETA);
					rv[m][1] = v[m][1]*VY*sin(THETA);
				}
				max_PPP = MAX_XY;
				break;
					}
			}
			switch(stype){
			case(0):{
				Z = Z0;
				for(m=0; m<Num; m++){
					numC = 0;
					temp1 = (ro[0] < ro[3]) ? ro[0] : ro[3];
					temp2 = (ro[1] < ro[2]) ? ro[1] : ro[2];
					dmin  = (temp1 < temp2) ? temp1 : temp2;
					temp1 = (ro[0] > ro[3]) ? ro[0] : ro[3];
					temp2 = (ro[1] > ro[2]) ? ro[1] : ro[2];
					dmax  = (temp1 > temp2) ? temp1 : temp2;
					if ((Z >= Z0) && (Z <= ZM)) {
						numC = conrec(ro, rv, Z);
					}
					if (numC != 0){
						for(k=0; k<numC; k++){
							if (Z < 0) ID *= -1;
							else ID = 1;
							if (ID == 1) {
								glBegin( GL_LINES );
								glVertex3f(ContLine[k].P1[0]/(type*max_PPP),ContLine[k].P1[1]/(type*max_PPP),0);
								glVertex3f(ContLine[k].P2[0]/(type*max_PPP),ContLine[k].P2[1]/(type*max_PPP),0);
								glEnd();
							}
						}
					}
					Z = Z + Step;
				}
				break;
					}
			case(1):{
				Z = ZM;
				for(m=0; m<Num; m++){
					numC = 0;
					temp1 = (ro[0] < ro[3]) ? ro[0] : ro[3];
					temp2 = (ro[1] < ro[2]) ? ro[1] : ro[2];
					dmin  = (temp1 < temp2) ? temp1 : temp2;
					temp1 = (ro[0] > ro[3]) ? ro[0] : ro[3];
					temp2 = (ro[1] > ro[2]) ? ro[1] : ro[2];
					dmax  = (temp1 > temp2) ? temp1 : temp2;
					if ((Z >= dmin) && (Z <= dmax)) {
						numC = conrec(ro, rv, Z);
					}
					if (numC != 0){
						for(k=0; k<numC; k++){
							if (Z < 0) ID *= -1;
							else ID = 1;
							if (ID == 1) {
								glBegin( GL_LINES );
								glVertex3f(ContLine[k].P1[0]/(type*max_PPP),ContLine[k].P1[1]/(type*max_PPP),0);
								glVertex3f(ContLine[k].P2[0]/(type*max_PPP),ContLine[k].P2[1]/(type*max_PPP),0);
								glEnd();
							}
						}
					}
					Z = Z*Z0;
				}
				break;
					}
			}
		}
	}
	glEnable(GL_LIGHTING);
	glEndList();
}


//**************************  map_H00_Cell()   ******************************//
void map_H00_Cell(int mode, float range[2])
{
	int i,j,k;
	int p_range[2];
	float maxiD;

	maxD = -1000000;
	minD =  1000000;

	switch(mode)
	{
//-------------------------------------------------------- (100)
	case(0):{
		p_range[0] = (int)(range[0]*NPIX[0]);
		p_range[1] = (int)(range[1]*NPIX[0]);
		for(k=0; k<NPIX[2]+1; k++){
			for(j=0; j<NPIX[1]+1; j++){
				DD[j][k] = 0;
				for(i=p_range[0]; i<=p_range[1]; i++){
					DD[j][k] += DELC[i][j][k];
				}
			}
		}
		for(k=0; k<NPIX[2]+1; k++){
			for(j=0; j<NPIX[1]+1; j++){
				maxD = (maxD > DD[j][k]) ? maxD : DD[j][k];
				minD = (minD < DD[j][k]) ? minD : DD[j][k];
			}
		}
		MaxPix[0] = NPIX[1]; MaxPix[1] = NPIX[2];
		break;
			}
//-------------------------------------------------------- (010)
	case(1):{
		p_range[0] = (int)(range[0]*NPIX[1]);
		p_range[1] = (int)(range[1]*NPIX[1]);
		for(k=0; k<NPIX[2]+1; k++){
			for(i=0; i<NPIX[0]+1; i++){
				DD[i][k] = 0;
				for(j=p_range[0]; j<=p_range[1]; j++){
					DD[i][k] += DELC[i][j][k];
				}
			}
		}
		for(k=0; k<NPIX[2]+1; k++){
			for(i=0; i<NPIX[0]+1; i++){
				maxD = (maxD > DD[i][k]) ? maxD : DD[i][k];
				minD = (minD < DD[i][k]) ? minD : DD[i][k];
			}
		}
		MaxPix[0] = NPIX[0]; MaxPix[1] = NPIX[2];
		break;
			}
//-------------------------------------------------------- (001)
	case(2):{
		p_range[0] = (int)(range[0]*NPIX[2]);
		p_range[1] = (int)(range[1]*NPIX[2]);
		for(j=0; j<NPIX[1]+1; j++){
			for(i=0; i<NPIX[0]+1; i++){
				DD[i][j] = 0;
				for(k=p_range[0]; k<=p_range[1]; k++){
					DD[i][j] += DELC[i][j][k];
				}
			}
		}
		for(j=0; j<NPIX[1]+1; j++){
			for(i=0; i<NPIX[0]+1; i++){
				maxD = (maxD > DD[i][j]) ? maxD : DD[i][j];
				minD = (minD < DD[i][j]) ? minD : DD[i][j];
			}
		}
		MaxPix[0] = NPIX[0]; MaxPix[1] = NPIX[1];
		break;
			}
	}
	max_PPP = max_P;
	maxiD = ((float)fabs(maxD) >= (float)fabs(minD)) ? (float)fabs(maxD) : (float)fabs(minD);
	sprintf(dm_max,"");
	if (fabs(maxD) >= fabs(minD)){
		if (maxiD <= 10000) {
			sprintf(dm_max,"Z(max) =%10.3f",maxiD);
		}
		else sprintf(dm_max,"Z(max) = %f",maxiD);
	}
	else {
		if (maxiD <= 10000) {
			sprintf(dm_max,"|Z(max)| =%10.3f",maxiD);
		}
		else sprintf(dm_max,"|Z(max)| = %f",maxiD);
	}
}


//**************************  D2D_fill_Grid()   ******************************//
void D2D_fill_Grid(int mode, int type, float bRGB[3])
{
	int i,j,m;
	float rv[2][2],v[2][2];
	float p,A,B,C,R;


	switch(mode){
	case(0):{
//------------------------------------------------------ (100)
		A = cCell[1];
		B = cCell[2]*cos(cCell[3]*PI/180);
		C = cCell[2]*sin(cCell[3]*PI/180);
		break;
			}
//------------------------------------------------------ (010)
	case(1):{
		A = cCell[0];
		B = cCell[2]*cos(cCell[4]*PI/180);
		C = cCell[2]*sin(cCell[4]*PI/180);
		break;
			}
//------------------------------------------------------ (001)
	case(2):{
		A = cCell[0];
		B = cCell[1]*cos(cCell[5]*PI/180);
		C = cCell[1]*sin(cCell[5]*PI/180);
		break;
			}
//------------------------------------------------------ (hkl)
	case(3):{
		A = VX;
		B = VY*cos(THETA);
		C = VY*sin(THETA);
		break;
			}
	}

	R = (float)(type+1)*max_PPP;
	glNewList(5,GL_COMPILE);
	glColor3f(bRGB[0],bRGB[1],bRGB[2]);
	glDisable(GL_LIGHTING);
	for(i=0; i<2; i++){
		p = 0.0;
		for(j=0; j<10; j++){
			p += (float)MaxPix[i]/10.0f;
			switch(i){
			case(0):{
				v[0][0] = p; v[0][1] = 0.0;
				v[1][0] = p; v[1][1] = MaxPix[1];
				break;
					}
			case(1):{
				v[0][1] = p; v[0][0] = 0.0;
				v[1][1] = p; v[1][0] = MaxPix[0];
				break;
					}
			}
			for(m=0; m<2; m++){
				v[m][0] /= (MaxPix[0]+1);
				v[m][1] /= (MaxPix[1]+1);
				rv[m][0] = v[m][0]*A + v[m][1]*B;
				rv[m][1] = v[m][1]*C;
			}
			glBegin(GL_LINES);
		      glVertex3f(rv[0][0]/R,rv[0][1]/R,0);
		      glVertex3f(rv[1][0]/R,rv[1][1]/R,0);
		    glEnd();
		}
	}
	glEnable(GL_LIGHTING);
	glEndList();
}





//**************************  fill_H00_Cell()   ******************************//
void D2D_fill_H00_Cell(int mode, int type, int palette, float iLevel)
{
	int i,j,m;
	float rv[4][2],v[4][2],r[4][2];
	float den;
	int Index,I1,I2;
	float RGB[4][3],RGB0[3];
	float maxiD;


//	MAX_XY = 10.0;

	maxiD = ((float)fabs(maxD) >= (float)fabs(minD)) ? (float)fabs(maxD) : (float)fabs(minD);
	if (iLevel == 0) iLevel = 0.0000001;
	type++;

	r[0][0] = 0; r[0][1] = 0;
	r[1][0] = 1; r[1][1] = 0;
	r[2][0] = 1; r[2][1] = 1;
	r[3][0] = 0; r[3][1] = 1;

	glNewList(3,GL_COMPILE);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	for(j=0; j<MaxPix[1]+0; j++){
		for(i=0; i<MaxPix[0]+0; i++){
			RGB0[0] = RGB0[1] = RGB0[2] = 0;
			for(m=0; m<4; m++){
				I1 = i + r[m][0];
				I2 = j + r[m][1];
				v[m][0] = I1;
				v[m][1] = I2;
				v[m][0] /= (MaxPix[0]+1);
				v[m][1] /= (MaxPix[1]+1);
				den = (float)fabs(DD[I1][I2]) / (iLevel*maxiD);
				if (den > 1.0) den = 1.0;
				Index = (int)(den*99);
				switch(palette){
//------------------------------------------------------ Rainbow
				case(0):{
					RGB[m][0] = (float)Palette[Index][0]/255;
					RGB[m][1] = (float)Palette[Index][1]/255;
					RGB[m][2] = (float)Palette[Index][2]/255;
					break;
						}
//------------------------------------------------------ Invers Rainbow
				case(1):{
					RGB[m][0] = 1 - (float)Palette[Index][0]/255;
					RGB[m][1] = 1 - (float)Palette[Index][1]/255;
					RGB[m][2] = 1 - (float)Palette[Index][2]/255;
					break;
						}
//------------------------------------------------------ Gray
				case(2):{
					RGB[m][0] = 1 - (float)Palette[Index][3]/255;
					RGB[m][1] = 1 - (float)Palette[Index][4]/255;
					RGB[m][2] = 1 - (float)Palette[Index][5]/255;
					break;
						}
				}
				RGB0[0] += RGB[m][0];
				RGB0[1] += RGB[m][1];
				RGB0[2] += RGB[m][2];
			}
			RGB0[0] /= 4;
			RGB0[1] /= 4;
			RGB0[2] /= 4;
			switch(mode)
			{
//------------------------------------------------------ (100)
			case(0):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*cCell[1] + v[m][1]*cCell[2]*cos(cCell[3]*PI/180);
					rv[m][1] = v[m][1]*cCell[2]*sin(cCell[3]*PI/180);
				}
				break;
					}
//------------------------------------------------------ (010)
			case(1):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*cCell[0] + v[m][1]*cCell[2]*cos(cCell[4]*PI/180);
					rv[m][1] = v[m][1]*cCell[2]*sin(cCell[4]*PI/180);
				}
				break;
					}
//------------------------------------------------------ (001)
			case(2):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*cCell[0] + v[m][1]*cCell[1]*cos(cCell[5]*PI/180);
					rv[m][1] = v[m][1]*cCell[1]*sin(cCell[5]*PI/180);
				}
				break;
					}
//------------------------------------------------------ (hkl)
			case(3):{
				for(m=0; m<4; m++){
					rv[m][0] = v[m][0]*VX + v[m][1]*VY*cos(THETA);
					rv[m][1] = v[m][1]*VY*sin(THETA);
				}
				break;
					}
			}
			glBegin(GL_POLYGON);
			for(m=0; m<4; m++){
				glColor3f(RGB[m][0],RGB[m][1],RGB[m][2]);
				glVertex3f(rv[m][0]/(type*max_PPP),rv[m][1]/(type*max_PPP),0);
			}
			glEnd();
		}
	}
	glEnable(GL_LIGHTING);
	glEndList();
}

//**************************  D2D_Color_Bar()   ****************************//
void D2D_Color_Bar(int palette, float range, float bRGB[3], float width)
{
	int i,j;
	float w,h,sh;
	char bar[6];
	int Index;
	float RGB[3];
	int Ticks[10] = {0,10,20,30,40,50,60,70,80,90};


	Index = (int)(range*100) + 1;
	if (range == 0) Index = 0;
	if (range == 1) Index = 100;

	sprintf(bar,"%i %%\n",Index);
//	w = (float)winrange[0]*0.07;
//	h = (float)winrange[1]*1.2;
	w = (float)winrange[0]*0.05;
	h = (float)winrange[1]*0.8;
	sh = h / 99;

	glNewList(2,GL_COMPILE);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	for(i=0; i<99; i++){
		switch(palette)
		{
		case(0):{
			RGB[0] = (float)Palette[i][0]/255;
			RGB[1] = (float)Palette[i][1]/255;
			RGB[2] = (float)Palette[i][2]/255;
			break;
				}
		case(1):{
			RGB[0] = 1 - (float)Palette[i][0]/255;
			RGB[1] = 1 - (float)Palette[i][1]/255;
			RGB[2] = 1 - (float)Palette[i][2]/255;
			break;
				}
		case(2):{
			RGB[0] = 1 - (float)Palette[i][3]/255;
			RGB[1] = 1 - (float)Palette[i][4]/255;
			RGB[2] = 1 - (float)Palette[i][5]/255;
			break;
				}
		}
		glColor3f(RGB[0],RGB[1],RGB[2]);
		glBegin(GL_POLYGON);
		glVertex3f(0,(i+5)*sh,0);
		glVertex3f(0,(i+6)*sh,0);
		glVertex3f(w,(i+6)*sh,0);
		glVertex3f(w,(i+5)*sh,0);
		glEnd();
		for(j=0; j<10; j++){
			if (i == Ticks[j]){
				glColor3f(bRGB[0],bRGB[1],bRGB[2]);
				glLineWidth(width);
				glBegin(GL_LINES);
				  glVertex3f(w,(i+5)*sh,0);
				  glVertex3f(w+2*sh,(i+5)*sh,0);
				glEnd();
				break;
			}
		}
	}
	glColor3f(bRGB[0],bRGB[1],bRGB[2]);
	glLineWidth(width);
	glBegin(GL_LINES);
	  glVertex3f(w,h+5*sh,0);
	  glVertex3f(w+2*sh,h+5*sh,0);
	glEnd();

	glColor3f(bRGB[0],bRGB[1],bRGB[2]);
	glLineWidth(width);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_POLYGON);
	glVertex3f(0,5*sh,0);
	glVertex3f(0,h+5*sh,0);
	glVertex3f(w,h+5*sh,0);
	glVertex3f(w,5*sh,0);
	glEnd();

	glColor3f(bRGB[0],bRGB[1],bRGB[2]);
//	glRasterPos2f(0,0);
//		draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "0 %");
//	glRasterPos2f(0,106*sh);
//		draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, bar);
	glRasterPos2f(0,0.85);
		draw_string_bitmap(GLUT_BITMAP_HELVETICA_12,dm_max);

	glEnable(GL_LIGHTING);
	glEndList();
	glLineWidth(1);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

//**************************  drawCell()   **********************************//
void draw_H00_Cell(int mode,int type, float RGB[3], float width)
{
	int i;
	int NPX[3];

	if (mode != 3) {
		NPX[0] = NPIX[0] + 2;
		NPX[1] = NPIX[1] + 2;
		NPX[2] = NPIX[2] + 2;
	}
	else {
		NPX[0] = MaxPix[0] + 2;
		NPX[1] = MaxPix[1] + 2;
		NPX[2] = 0;
	}
	type++;
	Type = type; 

	glNewList(1,GL_COMPILE);
	glColor3f(RGB[0],RGB[1],RGB[2]);
	glLineWidth(width);
	glDisable(GL_LIGHTING);
	switch(mode)
	{
//----------------------------------------------- (100)
	case(0):{
		outl_cell[0][0] = 0;         outl_cell[0][1] = 0;
		outl_cell[1][0] = NPIX[1]+1; outl_cell[1][1] = 0;
		outl_cell[2][0] = NPIX[1]+1; outl_cell[2][1] = NPIX[2]+1;
		outl_cell[3][0] = 0;         outl_cell[3][1] = NPIX[2]+1;
		for(i=0; i<4; i++){
			outl_cell[i][0] /= (float)NPX[1];
			outl_cell[i][1] /= (float)NPX[2];
			outl_cell[i][0] = outl_cell[i][0]*cCell[1] + outl_cell[i][1]*cCell[2]*cos(cCell[3]*PI/180);
			outl_cell[i][1] = outl_cell[i][1]*cCell[2]*sin(cCell[3]*PI/180);
		}
		orig[0] = Shift[0]*cCell[1] + Shift[1]*cCell[2]*cos(cCell[3]*PI/180);
		orig[1] = Shift[1]*cCell[2]*sin(cCell[3]*PI/180);
		max_PPP = max_P;
		break;
			}
//----------------------------------------------- (010)
	case(1):{
		outl_cell[0][0] = 0;       outl_cell[0][1] = 0;
		outl_cell[1][0] = NPIX[0]+1; outl_cell[1][1] = 0;
		outl_cell[2][0] = NPIX[0]+1; outl_cell[2][1] = NPIX[2]+1;
		outl_cell[3][0] = 0;       outl_cell[3][1] = NPIX[2]+1;
		for(i=0; i<4; i++){
			outl_cell[i][0] /= (float)NPX[0];
			outl_cell[i][1] /= (float)NPX[2];
			outl_cell[i][0] = outl_cell[i][0]*cCell[0] + outl_cell[i][1]*cCell[2]*cos(cCell[4]*PI/180);
			outl_cell[i][1] = outl_cell[i][1]*cCell[2]*sin(cCell[4]*PI/180);
		}
		orig[0] = Shift[0]*cCell[0] + Shift[1]*cCell[2]*cos(cCell[4]*PI/180);
		orig[1] = Shift[1]*cCell[2]*sin(cCell[4]*PI/180);
		max_PPP = max_P;
		break;
			}
//----------------------------------------------- (001)
	case(2):{
		outl_cell[0][0] = 0;         outl_cell[0][1] = 0;
		outl_cell[1][0] = NPIX[0]+1; outl_cell[1][1] = 0;
		outl_cell[2][0] = NPIX[0]+1; outl_cell[2][1] = NPIX[1]+1;
		outl_cell[3][0] = 0;         outl_cell[3][1] = NPIX[1]+1;
		for(i=0; i<4; i++){
			outl_cell[i][0] /= (float)NPX[0];
			outl_cell[i][1] /= (float)NPX[1];
			outl_cell[i][0] = outl_cell[i][0]*cCell[0] + outl_cell[i][1]*cCell[1]*cos(cCell[5]*PI/180);
			outl_cell[i][1] = outl_cell[i][1]*cCell[1]*sin(cCell[5]*PI/180);
		}
		orig[0] = Shift[0]*cCell[0] + Shift[1]*cCell[1]*cos(cCell[5]*PI/180);
		orig[1] = Shift[1]*cCell[1]*sin(cCell[5]*PI/180);
		max_PPP = max_P;
		break;
			}
//----------------------------------------------- (hkl)
	case(3):{
		outl_cell[0][0] = 0;           outl_cell[0][1] = 0;
		outl_cell[1][0] = MaxPix[0]+1; outl_cell[1][1] = 0;
		outl_cell[2][0] = MaxPix[0]+1; outl_cell[2][1] = MaxPix[1]+1;
		outl_cell[3][0] = 0;           outl_cell[3][1] = MaxPix[1]+1;
		for(i=0; i<4; i++){
			outl_cell[i][0] /= (float)NPX[0];
			outl_cell[i][1] /= (float)NPX[1];

			outl_cell[i][0] = outl_cell[i][0]*VX + outl_cell[i][1]*VY*cos(THETA);
			outl_cell[i][1] = outl_cell[i][1]*VY*sin(THETA);
		}
		orig[0] = Shift[0]*VX + Shift[1]*VY*cos(THETA);
		orig[1] = Shift[1]*VY*sin(THETA);
		max_PPP = MAX_XY;
		break;
			}
	}
	transl[0][0] = outl_cell[1][0] - outl_cell[0][0];
	transl[0][1] = outl_cell[1][1] - outl_cell[0][1];
	transl[1][0] = outl_cell[3][0] - outl_cell[0][0];
	transl[1][1] = outl_cell[3][1] - outl_cell[0][1];
	orig[0] /= max_PPP;
	orig[1] /= max_PPP;
	glBegin( GL_LINE_LOOP );
	for(i=0; i<4; i++) glVertex3f(outl_cell[i][0]/(type*max_PPP),outl_cell[i][1]/(type*max_PPP),0);
	glEnd();


	glEnable(GL_LIGHTING);
	glEndList();
	glLineWidth(1);
}

//**************************  D2D_render()    *******************************//
void D2D_RenderScene( void )
{
	int l,n;

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glOrtho(-winrange[0], winrange[0], -winrange[1], winrange[1],-winrange[0], winrange[0]);
//........................................................
	glPushMatrix();
	  glTranslatef(-0.7*winrange[0],-0.7*winrange[1],0);
	  glTranslatef(0, 0.25*winrange[1],0);
      glCallList(2);
	glPopMatrix();

    glTranslatef((float)winrange[0]*0.07,0,0);
	glScalef( shiftY,shiftY,1 ); 
	glPushMatrix();
	  glTranslatef(-orig[0],-orig[1],0);
	  for(l=0; l<Type; l++){
		  for(n=0; n<Type; n++){
			  glPushMatrix();
			  glTranslatef(l*transl[1][0]/(Type*max_PPP),l*transl[1][1]/(Type*max_PPP),0);
			  glTranslatef(n*transl[0][0]/(Type*max_PPP),n*transl[0][1]/(Type*max_PPP),0);
			  glCallList(3);
			  glCallList(4);
			  glCallList(5);
			  glCallList(1);
			  glPopMatrix();
		  }
	  }
	  glPopMatrix();
	glPopMatrix();
}

//**************************  D2D_reshape()   *******************************//
void D2D_reshape(int wid, int ht)
{
	winW = wid;
	winH = wid;
	glutReshapeWindow( winW, winH );
	glViewport(0,0,winW,winH);
	glutPostRedisplay();
}

//**************************  D2D_display()   *******************************//
void D2D_display(void)
{
	glClearColor(BKCL2D[0],BKCL2D[1],BKCL2D[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	D2D_RenderScene();
//........................................................
	glFinish();
	glutSwapBuffers();	
}

//**************************  D2DMouse()   **********************************//
void D2DMouse(int button, int button_state, int x, int y )
{
	if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
		last_y = y;
	}
}

//**************************  D2DMotion()   *********************************//
void D2DMotion(int x, int y )
{
	shiftY += 0.005*(float)(-y + last_y);
    last_y = y;
    glutPostRedisplay(); 
}

//**************************  open2Dwindow()   ******************************//
int open2Dwindow(int ID)
{
	winW = 400;
	winH = 400;
	ParentWindow = ID;
	glutInitWindowPosition( 400, 50 );
	glutInitWindowSize(winW, winH);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	windowID = glutCreateWindow("2D Map");
	_update_2Dwindow_title();
	glutReshapeFunc(D2D_reshape);
	glutDisplayFunc(D2D_display);
	glutMouseFunc( D2DMouse );
	glutMotionFunc( D2DMotion );
	glViewport(0, 0, winW, winH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.75, 0.75, -0.75, 0.75, -1, 12);
	glMatrixMode(GL_MODELVIEW);
	return windowID;
}

//**************************  close2Dwindow()   *****************************//
void close2Dwindow(int ID)
{
	glutDestroyWindow(ID);
}
