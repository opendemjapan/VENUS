#include <math.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include "tools.h"

int tw3D, th3D;
float xy_aspect3D;
float obj_pos3D[] = {0,0,0};
float origin3D[] = {0,0,0};
int	last_y3D,last_x3D;
float rotationX = 10;
float rotationY = 15;
int WindowID;
int	parentWindow;
int contol_key, controlID=0;
short Pal3D[100][6];
float DD[MAX_PIX][MAX_PIX];
float max3D, min3D;
int Max3Pix[2];

float VXY,COST;
float VX,VY,THETA;
float MAX_XY;
int MAX_PX;

float max_PPP;



/**************************************** _update_3Dwindow_title() **********/
void _update_3Dwindow_title()
{
	char *pdest;
	char nTitle[300];

	pdest = strrchr(FileName, '\\');
    if (pdest == NULL) pdest = strrchr(FileName, '/');
    if (pdest != NULL){
       pdest++;
       sprintf(nTitle,"Bird's-Eye View: %s",pdest);
	}
    else {
		if (strlen(FileName) > 0) sprintf(nTitle,"Bird's-Eye View: %s",FileName);
		else sprintf(nTitle,"Bird's-Eye View");
	}
	glutSetWindowTitle(nTitle);
}



//**************************  set_3Dpalette()   *******************************//
void set_3Dpalette(short Pl[100][6])
{
	int i,j;
	for(i=0; i<100; i++){
		for(j=0; j<6; j++) Pal3D[i][j] = Pl[i][j];
	}
}

//**************************  map_HKL_Cell()   ******************************//
void map_HKL_3D(int hkl[3], float orig)
{
	int jh,jv;
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
	max3D = -10000000.0f;
	min3D =  10000000.0f;

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
			min3D = (min3D < DDD) ? min3D : DDD;
			max3D = (max3D > DDD) ? max3D : DDD;
			DD[jh][jv] = DDD;
		}
	}
	Max3Pix[0] = NPX-1; Max3Pix[1] = NPY-1;
//	Max3Pix[0] = NPX; Max3Pix[1] = NPY;
}




//**************************  map_H00_3D()   ******************************//
void map_H00_3D(int mode, float range[2])
{
	int i,j,k;
	int p_range[2];
	int signID;

	max3D = -1000000;
	min3D =  1000000;
	signID = 1;

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
					if (DELC[i][j][k] < 0) signID = -1;
					DD[j][k] += DELC[i][j][k];
				}
			}
		}
		for(k=0; k<NPIX[2]+1; k++){
			for(j=0; j<NPIX[1]+1; j++){
				max3D = (max3D > DD[j][k]) ? max3D : DD[j][k];
				min3D = (min3D < DD[j][k]) ? min3D : DD[j][k];
			}
		}
		origin3D[0] = (float)NPIX[1]/(2*(float)NPIX[1]+1);
		origin3D[2] = (float)NPIX[2]/(2*(float)NPIX[2]+1);
		Max3Pix[0] = NPIX[1]; Max3Pix[1] = NPIX[2];
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
				max3D = (max3D > DD[i][k]) ? max3D : DD[i][k];
				min3D = (min3D < DD[i][k]) ? min3D : DD[i][k];
			}
		}
		Max3Pix[0] = NPIX[0]; Max3Pix[1] = NPIX[2];
		origin3D[0] = (float)NPIX[0]/(2*(float)NPIX[0]+1);
		origin3D[2] = (float)NPIX[2]/(2*(float)NPIX[2]+1);
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
				max3D = (max3D > DD[i][j]) ? max3D : DD[i][j];
				min3D = (min3D < DD[i][j]) ? min3D : DD[i][j];
			}
		}
		Max3Pix[0] = NPIX[0]; Max3Pix[1] = NPIX[1];
		origin3D[0] = (float)NPIX[0]/(2*(float)NPIX[0]+1);
		origin3D[1] = (float)NPIX[1]/(2*(float)NPIX[1]+1);
		break;
			}
	}
}

//**************************  fill_H00_Cell()   ******************************//
void D3D_fill_H00_Cell(int mode, int palette, float iLevel, float Level, float fRGB[3], float width)
{
	int i,j,m;
	int I1,I2;
	float rv[4][2],v[4][2];
	int r[4][2];
	float den[4];
	int Index;
	float x,y;
	float RGB[4][3];
	float maxiD;

	if (Level == 0) Level = 0.0000001;

	r[0][0] = 0; r[0][1] = 0;
	r[1][0] = 0; r[1][1] = 1;
	r[2][0] = 1; r[2][1] = 1;
	r[3][0] = 1; r[3][1] = 0;

	if (min3D < 0) origin3D[1] = 0;
	else origin3D[1] = iLevel/2;
	switch(mode)
	{
//------------------------------------------------------ (100)
	case(0):{
		origin3D[0] = (float)NPIX[1]/((float)NPIX[1]+1);
		origin3D[2] = (float)NPIX[2]/((float)NPIX[2]+1);
		x = origin3D[0]/2;
		y = origin3D[2]/2;
		origin3D[0] = x*cCell[1] + y*cCell[2]*cos(cCell[3]*PI/180);
		origin3D[2] = y*cCell[2]*sin(cCell[3]*PI/180);
		max_PPP = max_P;
		break;
			}
//------------------------------------------------------ (010)
	case(1):{
		origin3D[0] = (float)NPIX[0]/((float)NPIX[0]+1);
		origin3D[2] = (float)NPIX[2]/((float)NPIX[2]+1);
		x = origin3D[0]/2;
		y = origin3D[2]/2;
		origin3D[0] = x*cCell[0] + y*cCell[2]*cos(cCell[4]*PI/180);
		origin3D[2] = y*cCell[2]*sin(cCell[4]*PI/180);
		max_PPP = max_P;
		break;
			}
//------------------------------------------------------ (001)
	case(2):{
		origin3D[0] = (float)NPIX[0]/((float)NPIX[0]+1);
		origin3D[2] = (float)NPIX[1]/((float)NPIX[1]+1);
		x = origin3D[0]/2;
		y = origin3D[2]/2;
		origin3D[0] = x*cCell[0] + y*cCell[1]*cos(cCell[5]*PI/180);
		origin3D[2] = y*cCell[1]*sin(cCell[5]*PI/180);
		max_PPP = max_P;
		break;
			}
//------------------------------------------------------ (hkl)
	case(3):{
		origin3D[0] = (float)Max3Pix[0]/((float)Max3Pix[0]+1);
		origin3D[2] = (float)Max3Pix[1]/((float)Max3Pix[1]+1);
		x = origin3D[0]/2;
		y = origin3D[2]/2;
		origin3D[0] = x*VX + y*VY*cos(THETA);
		origin3D[2] = y*VY*sin(THETA);
		max_PPP = MAX_XY;
		break;
			}
	}
	maxiD = ((float)fabs(max3D) >= (float)fabs(min3D)) ? (float)fabs(max3D) : (float)fabs(min3D);

	glNewList(1,GL_COMPILE);
	for(j=0; j<Max3Pix[1]; j++){
		for(i=0; i<Max3Pix[0]; i++){
			for(m=0; m<4; m++){
				I1 = i + r[m][0];
				I2 = j + r[m][1];
				v[m][0] = (float)I1;
				v[m][1] = (float)I2;
				v[m][0] /= (Max3Pix[0]+1);
				v[m][1] /= (Max3Pix[1]+1);
				den[m] = DD[I1][I2] / (Level*maxiD);
				if (den[m] > 1) den[m] = 1;
				if (den[m] < -1) den[m] = -1;
				Index = (int)(den[m]*99);
				if (Index < 0) Index = 99 + Index;
				switch(palette){
//------------------------------------------------------ Rainbow
				case(0):{
					RGB[m][0] = (float)Pal3D[Index][0]/255;
					RGB[m][1] = (float)Pal3D[Index][1]/255;
					RGB[m][2] = (float)Pal3D[Index][2]/255;
					break;
						}
//------------------------------------------------------ Invers Rainbow
				case(1):{
					RGB[m][0] = 1 - (float)Pal3D[Index][0]/255;
					RGB[m][1] = 1 - (float)Pal3D[Index][1]/255;
					RGB[m][2] = 1 - (float)Pal3D[Index][2]/255;
					break;
						}
//------------------------------------------------------ Gray
				case(2):{
					RGB[m][0] = 1 - (float)Pal3D[Index][3]/255;
					RGB[m][1] = 1 - (float)Pal3D[Index][4]/255;
					RGB[m][2] = 1 - (float)Pal3D[Index][5]/255;
					break;
						}
				}
			}
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
			
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glShadeModel(GL_SMOOTH);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (mode != 3) {
				glBegin(GL_POLYGON);
				for(m=0; m<4; m++){
					glColor3f(RGB[m][0],RGB[m][1],RGB[m][2]);
					glVertex3f(rv[m][0]/max_P,iLevel*den[m],rv[m][1]/max_P);
				}
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				for(m=0; m<4; m++){
					glColor3f(RGB[m][0],RGB[m][1],RGB[m][2]);
//					glVertex3f(rv[m][0]/MAX_XY,iLevel*den[m],rv[m][1]/MAX_XY);
					glVertex3f(rv[m][1]/MAX_XY,iLevel*den[m],rv[m][0]/MAX_XY);
				}
				glEnd();
			}
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glColor3f(fRGB[0],fRGB[1],fRGB[2]);
			glLineWidth(width);
			if (mode != 3) {
				glBegin(GL_POLYGON);
				for(m=0; m<4; m++){
					glVertex3f(rv[m][0]/max_P,iLevel*den[m],rv[m][1]/max_P);
				}
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				for(m=0; m<4; m++){
//					glVertex3f(rv[m][0]/MAX_XY,iLevel*den[m],rv[m][1]/MAX_XY);
					glVertex3f(rv[m][1]/MAX_XY,iLevel*den[m],rv[m][0]/MAX_XY);
				}
				glEnd();
			}
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEndList();
	glLineWidth(1);
}

//**************************  D3D_render()    *******************************//
void D3D_RenderScene( void )
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0f, -4.6f );
	glPushMatrix();
	  glTranslatef( obj_pos3D[0], obj_pos3D[1], -obj_pos3D[2] ); 
      glRotatef(rotationY, 1,0,0);
      glRotatef(rotationX, 0,1,0);
	  glTranslatef(-origin3D[0]/max_PPP,-origin3D[1],-origin3D[2]/max_PPP);
	   glCallList(1);
	glPopMatrix();
}


//**************************  D3D_reshape()   *******************************//
void D3D_reshape(int wid, int ht)
{
	tw3D = wid;
	th3D = ht;
	glViewport( 0, 0, tw3D, th3D );
	glutReshapeWindow( tw3D, th3D);
	xy_aspect3D = (float)tw3D / (float)th3D;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect3D*.02, xy_aspect3D*.02, -.02, .02, .1, 15.0 );
	glMatrixMode( GL_MODELVIEW );
	glutPostRedisplay();
}

//**************************  D3D_display()   *******************************//
void D3D_display(void)
{
	glClearColor(BKCL3D[0],BKCL3D[1],BKCL3D[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect3D*.02, xy_aspect3D*.02, -.02, .02, .1, 15.0 );

	D3D_RenderScene();
//........................................................
	glFinish();
	glutSwapBuffers();	
}

//**************************  D3DMouse()   **********************************//
void D3DMouse(int button, int button_state, int x, int y )
{
	controlID = 0;
	if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
		last_x3D = x;
		last_y3D = y;
	}
	if ( button == GLUT_RIGHT_BUTTON && button_state == GLUT_DOWN ) {
		last_y3D = y;
		controlID = 1;
	}
}

//**************************  D3DMotion()   *********************************//
void D3DMotion(int x, int y )
{
	float delta_x;
	float delta_y;

	delta_x = (float) (x - last_x3D);
	delta_y = (float) (y - last_y3D);

	switch(controlID)
	{
	case(0):{
		rotationX += delta_x;
		rotationY += delta_y;
		break;
			}
	case(1):{obj_pos3D[2] +=0.01*delta_y;break;}
	}
	last_x3D = x;
    last_y3D = y;
    glutPostRedisplay(); 
}

//**************************  open3Dwindow()   ******************************//
int open3Dwindow(int ID)
{
	float pos[4] = {0,50,0,0};
	float dir[3] = {0,-1,0};
	GLfloat mat_specular[] = {1,1,1,1};
	GLfloat light_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};

	tw3D = 400;
	th3D = 400;
	parentWindow = ID;
	glutInitWindowPosition( 400, 250 );
	glutInitWindowSize(tw3D, th3D);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	WindowID = glutCreateWindow("Bird's-Eye View");
	_update_3Dwindow_title();
	glutReshapeFunc(D3D_reshape);
	glutDisplayFunc(D3D_display);
	glutMouseFunc( D3DMouse );
	glutMotionFunc( D3DMotion );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
	return WindowID;
}

//**************************  close3Dwindow()   *****************************//
void close3Dwindow(int ID)
{
	glutDestroyWindow(ID);
}
