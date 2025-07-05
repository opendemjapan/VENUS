#include <stdio.h>
#include <string.h>
#include <math.h>

#include "tools.h"



//*******************************************************************************
void check_file_ext_exist(int form, char *fName, int mode)
{
   char name[256];
   char *dest;
   char *ext;

   dest = "";
   ext = "";
   dest = strrchr(fName,'\\');
   if (dest == NULL) {
      dest = strrchr(fName,'/');
      if (dest == NULL) dest = fName;
   }
   sprintf(name,"");
   sprintf(name,"%s",dest);
   dest = strrchr(name, '.');
   if (dest == NULL){
	   switch(mode){
//---------------------------------------- Save VCS File
	   case(0):{ext = ".vnd"; break;}
//---------------------------------------- Export Image
	   case(1):{
		   switch(form){
		   case(0):{ext = ".bmp"; break;}
		   case(1):{ext = ".eps"; break;}
		   case(2):{ext = ".jpg"; break;}
		   case(3):{ext = ".jp2"; break;}
		   case(4):{ext = ".ppm"; break;}
		   case(5):{ext = ".raw"; break;}
		   case(6):{ext = ".rgb"; break;}
		   case(7):{ext = ".tga"; break;}
		   case(8):{ext = ".tif"; break;}
		   }
		   break;
			   }
	   }
           sprintf(name,"");
           sprintf(name,"%s",fName);
	   sprintf(fName,"");
	   sprintf(fName,"%s%s",name,ext);
   }
}





//*******************************************************************************
int get_format(char *argv)
{
	int i;

	i = -1;
	sscanf(argv,"%i",&i);
	return i;
}

//*******************************************************************************
int check_format(char *name)
{
	char *pdest;

	pdest = strrchr( name, '.');
	if (pdest != NULL) pdest++;
	else return -1;
//------------------------------------------------
	if( strstr( pdest, "den") != NULL ) return 0;
	if( strstr( pdest, "DEN") != NULL ) return 0;
//------------------------------------------------
	if( strstr( pdest, "cube") != NULL ) return 1;
	if( strstr( pdest, "CUBE") != NULL ) return 1;
	if( strstr( pdest, "cub") != NULL ) return 1;
	if( strstr( pdest, "CUB") != NULL ) return 1;
//------------------------------------------------
	if( strstr( pdest, "scat") != NULL ) return 2;
	if( strstr( pdest, "SCAT") != NULL ) return 2;
	if( strstr( pdest, "sca") != NULL ) return 2;
	if( strstr( pdest, "SCA") != NULL ) return 2;
//------------------------------------------------
	if( strstr( pdest, "rho") != NULL ) return 3;
	if( strstr( pdest, "RHO") != NULL ) return 3;
//------------------------------------------------
	if( strstr( pdest, "vnd") != NULL ) return 4;
	if( strstr( pdest, "VND") != NULL ) return 4;
//------------------------------------------------
	if( strstr( pdest, "ked") != NULL ) return 5;
	if( strstr( pdest, "ped") != NULL ) return 5;
	if( strstr( pdest, "led") != NULL ) return 5;
	if( strstr( pdest, "ted") != NULL ) return 5;
	if( strstr( pdest, "KED") != NULL ) return 5;
	if( strstr( pdest, "PED") != NULL ) return 5;
	if( strstr( pdest, "LED") != NULL ) return 5;
	if( strstr( pdest, "TED") != NULL ) return 5;
//------------------------------------------------
	if( strstr( pdest, "eb") != NULL ) return 6;
	if( strstr( pdest, "EB") != NULL ) return 6;

   return -1;
}


/********************************************************************/
float maxf(float a, float b, float c)
{
	float d;
	d = (a >= b) ? a : b;
	d = (d >= c) ? d : c;
	return(d);
}

//*****************************  save_preferency()  *************************//
void save_preferency()
{
	FILE *fptr;
	char path[256];
	int i;

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,PREFFILE);
	fptr = fopen(path,"w");

	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_BKCL[0],def_BKCL[1],def_BKCL[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_BKCL2D[0],def_BKCL2D[1],def_BKCL2D[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_BKCL3D[0],def_BKCL3D[1],def_BKCL3D[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_unit_cell_color[0],def_unit_cell_color[1],def_unit_cell_color[2]);
	fprintf(fptr,"%2i %6.3f\n",def_unit_cell_type,def_unit_cell_width);
	fprintf(fptr,"%2i\n",def_show_axis);
	fprintf(fptr,"%6.3f\n",stepw_angle);
	fprintf(fptr,"%2i\n",def_file_format_ID);

	fprintf(fptr,"%i %i\n",menu_x, menu_y);
	fprintf(fptr,"%i %i\n",tools_x, tools_y);
	fprintf(fptr,"%i %i %i %i\n",graph_x, graph_y, graph_w, graph_h);

	for(i=0; i<12; i++){
		fprintf(fptr,"%i %i\n", cdwp[0][i],cdwp[1][i]);
	}


	
	fclose(fptr);
}



//*****************************  load_preferency()  *************************//
void load_preferency()
{
	FILE *fptr;
	char path[256];
	int i;

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,PREFFILE);
	fptr = fopen(path,"r");

	fscanf(fptr,"%f %f %f\n",&def_BKCL[0],&def_BKCL[1],&def_BKCL[2]);
	fscanf(fptr,"%f %f %f\n",&def_BKCL2D[0],&def_BKCL2D[1],&def_BKCL2D[2]);
	fscanf(fptr,"%f %f %f\n",&def_BKCL3D[0],&def_BKCL3D[1],&def_BKCL3D[2]);
	fscanf(fptr,"%f %f %f\n",&def_unit_cell_color[0],&def_unit_cell_color[1],&def_unit_cell_color[2]);
	fscanf(fptr,"%i %f\n",&def_unit_cell_type, &def_unit_cell_width);
	fscanf(fptr,"%i\n",&def_show_axis);
	fscanf(fptr,"%f\n",&def_stepw_angle);
	fscanf(fptr,"%i\n",&def_file_format_ID);

	fscanf(fptr,"%i %i\n",&menu_x, &menu_y);
	fscanf(fptr,"%i %i\n",&tools_x, &tools_y);
	fscanf(fptr,"%i %i %i %i\n",&graph_x, &graph_y, &graph_w, &graph_h);

	for(i=0; i<12; i++){
		fscanf(fptr,"%i %i\n", &cdwp[0][i],&cdwp[1][i]);
		dwp[0][i] = cdwp[0][i];
		dwp[1][i] = cdwp[1][i];
	}
	fclose(fptr);
}


//*****************************  load_palette()  ****************************//
void load_palette()
{
	FILE *fptr;
	int i,j;
	int p[6];
	char path[256];

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,PALFILE);
	fptr = fopen(path,"r");

	for(i=0; i<100; i++){
		fscanf(fptr,"%i %i %i %i %i %i\n",&p[0],&p[1],&p[2],    //--- Rainbow Palette
			                              &p[3],&p[4],&p[5]);   //--- Gray Palette
		for(j=0; j<6; j++) Pal[i][j] = p[j];
	}
	fclose(fptr);
}


//*****************************  set_initital_parameters()  *****************//
void set_initital_parameters()
{
	int i;
//---------------------------------- File
//	fileID = 0;
//	sprintf(FileName,"");
//---------------------------------- 3D-Data	
	sprintf(Title,"");
	NASYM = 0;
//	OBJLEV = 1.0;
	SECLEV = 0.5;
	ORIGIN[0] = ORIGIN[1] = ORIGIN[2] = 0;
	DELC_ID = 0;
	HMIN = HMAX = 0;
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1;
	range_ID = 0;
	numBox[0] = numBox[1] = numBox[2] = 1;
	show_iso = 1;
	show_sls = 1;
    model_mode = 0;
//--------------------------------------------------- Initial Background Parameters
	for(i=0; i<3; i++){
		BKCL[i] = def_BKCL[i];
		BKCL2D[i] = def_BKCL2D[i];
		BKCL3D[i] = def_BKCL3D[i];
	}
//--------------------------------------------------- Initial Stepwise Parameters
	stepw_angle = def_stepw_angle;
//--------------------------------------------------- Initial Orientation Parameters
	for(i=0; i<16; i++) rotation[i] = 0;
	rotation[0] = rotation[5] = rotation[10] = rotation[15] = 1;


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


//---------------------------------- Isosurface
	mat_ambient[0] = mat_ambient[1] = mat_ambient[2] = mat_ambient[3] = 1.0;
	mat_diffuse[0] = 1.0;mat_diffuse[1] = 1.0;mat_diffuse[2] = 0.0;mat_diffuse[3] = 1.0;
	mat_specular[0] = mat_specular[1] = mat_specular[2] = mat_specular[3] = 0.0;
	mat_emission[0] = mat_emission[1] = mat_emission[2] = 0.0; mat_emission[3] = 1.0;
	mat_shininess = 128;
	iso_pm_parm = 0;
//---------------------------------- Unit Cell
	unit_cell_show = 1;
	unit_cell_width = def_unit_cell_width;
	unit_cell_type = def_unit_cell_type;
	for(i=0; i<3; i++) unit_cell_color[i] = def_unit_cell_color[i];
//--------------------------------------------------- Initial Compass Settings
	show_axis = def_show_axis;
//--------------------------------------------------- Initial Projection Settings
	projection_type = 0;
	perspective = 1.0f;
//---------------------------------- (hkl) Plane
	objlev_scale = 0.0;
	seclev_scale = 1.0;
	num_of_hkl = 0;
	opacity_hkl = 1.0;
	palette_hkl = 0;
	position_hkl[0] = position_hkl[1] = position_hkl[2] = 0.5;
	index_hkl[0][0] = index_hkl[0][1] = index_hkl[0][2] = 0;
	index_hkl[1][0] = index_hkl[1][1] = index_hkl[1][2] = 0;
	index_hkl[2][0] = index_hkl[2][1] = index_hkl[2][2] = 0;
	index_show_hkl[0] = index_show_hkl[1] = index_show_hkl[2] = 0;
	hkl_mode = 0;
//---------------------------------- 2D Output
	prj_plane = 0;
	prj_cell = 0;
	prj_pal = 0;
	prj_cont_show = 0;
	prj_show_2D = 0;
	prj_show_3D = 0;
	prj_project[0] = 0;
	prj_project[1] = 1;
	prj_seclev_scale = 1.0;
	prj_3Dlev_scale = 1.0;
	prj_RGB[0] = prj_RGB[1] = prj_RGB[2] = 1;
	prj_width = 1;
	prj_hkl[0] = 1; prj_hkl[1] = prj_hkl[2] = 0;
	prj_org = 0.5f;
	old_hkl[0] = old_hkl[1] = old_hkl[2];
	prj_grid = 0;
//---------------------------------- 2D Contour map
	cont_RGB[0] = cont_RGB[1] = cont_RGB[2] = 1;
	cont_width = 1;
	cont_stype = 0;
	cont_show_only = 0;
	cont_step = -1;
	cont_NSTP[0] = 10;
	cont_NSTP[1] = -1;
	conMAX[0] = conMAX[1] = conMIN[0] = conMIN[1] = -1;
}

//*****************************  readELENfile()  ***************************//
int readELENfile(char *fname)
{
	int i,j,k;
	FILE *fptr;

	HMAX = -10000.0;
	HMIN = 10000.0;
	printf("\nReading 3D data from file %s\n\n",fname);
	fptr = fopen(fname,"r");
	sprintf(Title,"");
	fgets( Title, 80, fptr); 
	fscanf(fptr,"%f %f %f %f %f %f\n",&cCell[0],&cCell[1],&cCell[2],
		                              &cCell[3],&cCell[4],&cCell[5]);
	fscanf(fptr,"%i %i %i\n",&NPIX[0],&NPIX[1],&NPIX[2]);
	for(i=0; i<NPIX[0]; i++){
		for(j=0; j<NPIX[1]; j++){
			for(k=0; k<NPIX[2]; k++){
				fscanf(fptr,"%E",&DELC[i][j][k]);
				HMIN = (DELC[i][j][k] < HMIN) ? DELC[i][j][k] : HMIN;
				HMAX = (DELC[i][j][k] > HMAX) ? DELC[i][j][k] : HMAX;
			}
		}
	}
	max_P = maxf(cCell[0],cCell[1],cCell[2]);
	HMIN = 0.000001f;
	NPIX[0] = NPIX[0] - 1;
	NPIX[1] = NPIX[1] - 1;
	NPIX[2] = NPIX[2] - 1;

	fclose(fptr);
	return(1);
}

//*****************************  readDVXfile()  ***************************//
int readDVXfile(char *fname)
{
	FILE *fptr;
	int i,j,k,I1,I2,I3,I4;
	int II1, II2;
	float AU,AU3,D;
	float x0,y0,z0;

	AU = 0.529177f;
	AU3 = AU * AU * AU;
	NASYM = 0;
	HMAX = -10000.0;
	HMIN = 10000.0;
	printf("\nReading 3D data from file %s\n\n",fname);
	fptr = fopen(fname,"r");

	sprintf(Title,"");
	sprintf(Title,"DVXa file");
	fscanf(fptr,"%i %i %i %f %f %f\n",&NPIX[0],&NPIX[1],&NPIX[2],&x0,&y0,&z0);
	if((NPIX[0] > MAX_PIX) || (NPIX[1] > MAX_PIX) || (NPIX[2] > MAX_PIX)) {
			printf("======================================================\n");
			printf("Number of pixels out of range\n MAX_PIX=%i\n",MAX_PIX);
			printf("======================================================\n");
			fclose(fptr);
			return(-1);
	}
	NASYM = (NPIX[0]-1)*(NPIX[1]-1)*(NPIX[2]-1);
	I4 = -1;
	II1 = 0;
	II2 = 0;
	for(k=1; k<=NPIX[2]; k++){
		for(i=1; i<=NPIX[0]; i++){
			for(j=1;j<=NPIX[1]; j++){
				I1 = i - 1;
				I2 = j - 1;
				I3 = k - 1;
				fscanf(fptr,"%f",&D);
				DELC[I1][I2][I3] = D;
//				if (D <= 0.0) DELC_ID = 1;
				if (D >= 0.0) II1 = 1;
				if (D < 0.0) II2 = 1;
				HMIN = (D < HMIN) ? D : HMIN;
				HMAX = (D > HMAX) ? D : HMAX;
				if (D > 0.0) I4 = 1;
			}
		}
	}

	if ((II1 == 1) && (II2 == 1)) {
		iso_pm_parm = 0;
		DELC_ID = 1;
	}
	if ((II1 == 1) && (II2 == 0)) {
		iso_pm_parm = 1;
		DELC_ID = 0;
	}
	if ((II1 == 0) && (II2 == 1)) {
		iso_pm_parm = 2;
		DELC_ID = 0;
	}


	NPIX[0]--;
	NPIX[1]--;
	NPIX[2]--;
	if (HMIN < -1000) HMIN = -1000;
	if ((OBJLEV > HMAX) || (OBJLEV < HMIN)) {
		if (I4 == -1) {OBJLEV = 2*HMAX;}
		else {OBJLEV = (float)fabs(HMAX/2);}
	}
	cCell[0] = x0*AU;
	cCell[1] = y0*AU;
	cCell[2] = z0*AU;
	cCell[3] = cCell[4] = cCell[5] = 90;
	max_P = maxf(cCell[0],cCell[1],cCell[2]);
	fclose(fptr);
	return(1);
}


//*****************************  readCUBEfile()  ***************************//
int readCUBEfile(char *fname)
{
	int i,j,k,l,I1,I2,I4;
	FILE *fptr;
	char line[256];
	float AU,AU3;
	int NumA, NumO, NumI;
	float x0,y0,z0;
	float x[3],y[3],z[3];
	float summ,charg,dum1,dum2,dum3;
	float DM1,DM2,DM3;
	float D,DD;

	summ = 0;
	AU = 0.529177f;
	AU3 = AU * AU * AU;
	NASYM = 0;
	HMAX = -10000.0;
	HMIN = 10000.0;
	printf("\nReading 3D data from file %s\n\n",fname);
	fptr = fopen(fname,"r");

	sprintf(Title,"");
	fgets( Title, 80, fptr); 
	sprintf(line,"");
	fgets( line,80, fptr); 
	fscanf(fptr,"%i %f %f %f\n",&NumA,&x0,&y0,&z0);
	for(i=0; i<3; i++) {
		fscanf(fptr,"%i %f %f %f\n",&NPIX[i],&x[i],&y[i],&z[i]);
		if(NPIX[i] > MAX_PIX) {
			printf("======================================================\n");
			printf("Number of pixels out of range\n MAX_PIX=%i\n",MAX_PIX);
			printf("======================================================\n");
			fclose(fptr);
			return(-1);
		}
	}
	NASYM = NPIX[0]*NPIX[1]*NPIX[2];
	NumI = 0;
	if (NumA < 0) {
		NumA = -1*NumA;
		NumI = -1;
	}
	for(i=0; i<NumA; i++) {
		fscanf(fptr,"%i %f %f %f %f\n",&I1,&charg,&dum1,&dum2,&dum3);
		summ += charg;
	}
	NumO = 1;
	if (NumI < 0){
		sprintf(line,"");
		fgets( line,80, fptr); 
		sscanf(line,"%i",&NumO);
	}
	DM1 = (float)(NPIX[0]-1)*x[0];
	DM2 = (float)(NPIX[1]-1)*y[1];
	DM3 = (float)(NPIX[2]-1)*z[2];

	summ = 1.0f;
	I4 = -1;
	I1 = 0;
	I2 = 0;
	for(i=1; i<NPIX[0]+1; i++){
		for(j=1; j<NPIX[1]+1; j++){
			for(k=1; k<NPIX[2]+1; k++){
				DELC[i][j][k] = 0.0f;
				fscanf(fptr,"%f",&D);
				for(l=1; l<NumO; l++) fscanf(fptr,"%f",&DD);
//				D1 = ((float)(i-1)*x[0]*(float)NPIX[0])/DM1;
//				D2 = ((float)(j-1)*y[1]*(float)NPIX[1])/DM2;
//				D3 = ((float)(k-1)*z[2]*(float)NPIX[2])/DM3;
//				I1 = (int)D1;
//				I2 = (int)D2;
//				I3 = (int)D3;
				if (D >= 0.0) I1 = 1;
				if (D < 0.0) I2 = 1;
				if (D > 0) I4 = 1;
				DELC[i-1][j-1][k-1] = D;
				HMIN = (DELC[i-1][j-1][k-1] <= HMIN) ? DELC[i-1][j-1][k-1] : HMIN;
				HMAX = (DELC[i-1][j-1][k-1] >= HMAX) ? DELC[i-1][j-1][k-1] : HMAX;
			}
		}
	}

	if ((I1 == 1) && (I2 == 1)) {
		iso_pm_parm = 0;
		DELC_ID = 1;
	}
	if ((I1 == 1) && (I2 == 0)) {
		iso_pm_parm = 1;
		DELC_ID = 0;
	}
	if ((I1 == 0) && (I2 == 1)) {
		iso_pm_parm = 2;
		DELC_ID = 0;
	}

	if (HMIN < -1000) HMIN = -1000;
	if ((OBJLEV > HMAX) || (OBJLEV < HMIN)) {
		if (I4 == -1) {OBJLEV = 2*HMAX;}
		else {OBJLEV = (float)fabs(HMAX/2);}
	}
	if ((x0 == 0.0) && (y0 == 0.0) && (z0 == 0.0)){
		cCell[0] = (float)(NPIX[0])*x[0]*AU;
		cCell[1] = (float)(NPIX[1])*y[1]*AU;
		cCell[2] = (float)(NPIX[2])*z[2]*AU;
	}
	else {
		cCell[0] = -2*x0*AU;
		cCell[1] = -2*y0*AU;
		cCell[2] = -2*z0*AU;
	}
	NPIX[0]--;
	NPIX[1]--;
	NPIX[2]--;
	cCell[3] = cCell[4] = cCell[5] = 90;
	max_P = maxf(cCell[0],cCell[1],cCell[2]);

	fclose(fptr);
	return(1);
}

//*****************************  readVIENfile()  ***************************//
int readVIENfile(char *fname)
{
	int i,j,k,I1,I2;
	FILE *fptr;
	char line[80];
	char *pos;
	int mode = 0;
	float AU, AU3;

	AU = 0.529177f;
	AU3 = AU * AU * AU;
	NASYM = 0;
	HMAX = -10000.0;
	HMIN = 10000.0;
	sprintf(Title,"");
	sprintf(Title,"WIEN2K file");
	printf("\nReading 3D data from file %s\n\n",fname);
	fptr = fopen(fname,"r");

	fscanf(fptr,"%s\n",&line);
	pos = strstr( line, "cell");
	if( pos != NULL ) mode = 0;
	pos = strstr( line, "free");
	if( pos != NULL ) mode = 1;
	printf("Mode = %i\n",mode);

	fscanf(fptr,"%f %f %f\n",&cCell[0],&cCell[1],&cCell[2]);
	fscanf(fptr,"%f %f %f\n",&cCell[3],&cCell[4],&cCell[5]);
	fscanf(fptr,"%i %i %i %f %f %f\n",
		         &NPIX[0],&NPIX[1],&NPIX[2],
				 &cCell[0],&cCell[1],&cCell[2]);
    if((NPIX[0] > MAX_PIX) || (NPIX[1] > MAX_PIX) || (NPIX[2] > MAX_PIX)) {
		printf("======================================================\n");
		printf("Number of pixels out of range\n MAX_PIX=%i\n",MAX_PIX);
		printf("======================================================\n");
		fclose(fptr);
		return(-1);
	}

	for(i=0; i<NPIX[0]; i++){
		for(j=0; j<NPIX[1]; j++){
			for(k=0; k<NPIX[2]; k++){
				NASYM ++;
				fscanf(fptr,"%f",&DELC[i][j][k]);
				if (DELC[i][j][k] >= 0.0) I1 = 1;
				if (DELC[i][j][k] < 0.0) I2 = 1;
				HMIN = (DELC[i][j][k] <= HMIN) ? DELC[i][j][k] : HMIN;
				HMAX = (DELC[i][j][k] >= HMAX) ? DELC[i][j][k] : HMAX;
			}
		}
	}
	fclose(fptr);
	if ((I1 == 1) && (I2 == 1)) {
		iso_pm_parm = 0;
		DELC_ID = 1;
	}
	if ((I1 == 1) && (I2 == 0)) {
		iso_pm_parm = 1;
		DELC_ID = 0;
	}
	if ((I1 == 0) && (I2 == 1)) {
		iso_pm_parm = 2;
		DELC_ID = 0;
	}
	if ((OBJLEV > HMAX) || (OBJLEV < HMIN)) {
		OBJLEV = (float)fabs(HMAX/2);
	}
	if (HMIN < -1000) HMIN = -1000;
//	if (HMIN == 0) HMIN = 0.000001f;
	NPIX[0] = NPIX[0] - 1;
	NPIX[1] = NPIX[1] - 1;
	NPIX[2] = NPIX[2] - 1;
	cCell[0] *= AU;
	cCell[1] *= AU;
	cCell[2] *= AU;
	if (mode == 1) cCell[3] = cCell[4] = cCell[5] = 90;
	max_P = maxf(cCell[0],cCell[1],cCell[2]);

	return(1);
}



//*****************************  readMEEDfile()  ***************************//
int readMEED3file(char *fname)
{
	FILE *mfile;
	char line[256];
	int comentID,comentN;
	int i,j,k,l;
	float density;
	int x,y,z;
	int ISX,ISY,ISZ;
	int NX,NY,NZ;
	int KX,KY,KZ;
	float DD;


	HMAX = -10000.0;
	HMIN = 10000.0;
	sprintf(line,"");

//====================================================================================
    mfile=fopen(fname,"r");
	comentID = 0;
	comentN = 0;
	while (comentID != 1){
		fgets( line, 256, mfile); 
		if (line[0] == '#') {
			comentID = 0;
			comentN++;
		}
		else comentID = 1;
	}
	rewind(mfile);
	for(i=0; i<comentN; i++) fgets( line, 256, mfile); 
//--------------------------------------------------- Title
	sprintf(Title,"%s","");
	fgets( Title, 80, mfile); 
	comentN++;
//--------------------------------------------------- Number of pixel in asym. unit.
        fscanf( mfile, "%i\n", &NASYM);
	comentN++;
//--------------------------------------------------- Read 3D-Data
	printf("\nReading 3D data from file %s\n\n",fname);
	for(i=0; i<NASYM; i++){
		 fscanf( mfile, "%i %i %i %E",&x,&y,&z,&density);
	}
//--------------------------------------------------- Read Unit Cell Parameters
	fscanf( mfile, "%f %f %f %f %f %f\n", &cCell[0],&cCell[1],&cCell[2],&cCell[3],&cCell[4],&cCell[5]);
//--------------------------------------------------- Read Dimensions and Symmetry settings
	fscanf( mfile, "%i %i %i %i %i %i\n", &NPIX[0],&NPIX[1],&NPIX[2],&NSYM,&NCEN,&NSUB);
    if((NPIX[0] > MAX_PIX) || (NPIX[1] > MAX_PIX) || (NPIX[2] > MAX_PIX)) {
		printf("======================================================\n");
		printf("Number of pixels out of range\n MAX_PIX=%i\n",MAX_PIX);
		printf("======================================================\n");
		fclose(mfile);
		return(-1);
	}
//--------------------------------------------------- Read Symmetry Matrix
	for(i=0;i<NSYM;i++){
		fscanf( mfile,"%i %i %i %i %i %i %i %i %i %i %i %i\n",
		              &sPos[i].a[0][0],&sPos[i].a[1][0],&sPos[i].a[2][0],
		              &sPos[i].a[0][1],&sPos[i].a[1][1],&sPos[i].a[2][1],
		              &sPos[i].a[0][2],&sPos[i].a[1][2],&sPos[i].a[2][2],
		              &sPos[i].t[0],&sPos[i].t[1],&sPos[i].t[2]);
	}
//--------------------------------------------------- Read Brave Translations
	for(i=0;i<NSUB;i++){
		fscanf( mfile,"%i %i %i\n",&ISUB[0][i],&ISUB[1][i],&ISUB[2][i]);
	}
//====================================================================================
	rewind(mfile);
	for(i=0; i<comentN; i++) fgets( line, 256, mfile); 
	for(i=0; i<NASYM; i++){
		fscanf( mfile, "%i %i %i %E",&x,&y,&z,&density);
		KX = x;
		KY = y;
		KZ = z;
		DD = (float)fabs(density);
		HMIN = (DD <= HMIN) ? DD : HMIN;
		HMAX = (DD >= HMAX) ? DD : HMAX;
		for(j=0;j<NCEN+1;j++){
			for(l=0;l<NSUB;l++){
				ISX = ISUB[0][l]; ISY = ISUB[1][l]; ISZ = ISUB[2][l];
				for(k=0;k<NSYM;k++){
					NX = KX*sPos[k].a[0][0] + KY*sPos[k].a[0][1] + KZ*sPos[k].a[0][2] + sPos[k].t[0];
					NY = KX*sPos[k].a[1][0] + KY*sPos[k].a[1][1] + KZ*sPos[k].a[1][2] + sPos[k].t[1];
					NZ = KX*sPos[k].a[2][0] + KY*sPos[k].a[2][1] + KZ*sPos[k].a[2][2] + sPos[k].t[2];
					if (j==1){NX = -NX; NY = -NY;	NZ = -NZ;}
					NX = NX + 2*NPIX[0] + ISX;
					NY = NY + 2*NPIX[1] + ISY;
					NZ = NZ + 2*NPIX[2] + ISZ;
					NX = NX - (NX/NPIX[0])*NPIX[0];
					NY = NY - (NY/NPIX[1])*NPIX[1];
					NZ = NZ - (NZ/NPIX[2])*NPIX[2];
					DELC[NX][NY][NZ] = DD;
					if ((NX == 0) && (NY == 0) && (NZ == 0)){
						DELC[NPIX[0]][0][0] = DD;
						DELC[0][NPIX[1]][0] = DD;
						DELC[0][0][NPIX[2]] = DD;
						DELC[NPIX[0]][NPIX[1]][0] = DD;
						DELC[NPIX[0]][0][NPIX[2]] = DD;
						DELC[0][NPIX[1]][NPIX[2]] = DD;
						DELC[NPIX[0]][NPIX[1]][NPIX[2]] = DD;
					}
					if ((NX == 0) && (NY == 0) && (NZ != 0)){
						DELC[NPIX[0]][0][NZ] = DD;
						DELC[0][NPIX[1]][NZ] = DD;
						DELC[NPIX[0]][NPIX[1]][NZ] = DD;
					}
					if ((NX == 0) && (NY != 0) && (NZ == 0)){
						DELC[NPIX[0]][NY][0] = DD;
						DELC[0][NY][NPIX[2]] = DD;
						DELC[NPIX[0]][NY][NPIX[2]] = DD;
					}
					if ((NX != 0) && (NY == 0) && (NZ == 0)){
						DELC[NX][NPIX[1]][0] = DD;
						DELC[NX][0][NPIX[2]] = DD;
						DELC[NX][NPIX[1]][NPIX[2]] = DD;
					}
					if ((NX == 0) && (NY != 0) && (NZ != 0)) DELC[NPIX[0]][NY][NZ] = DD;
					if ((NX != 0) && (NY == 0) && (NZ != 0)) DELC[NX][NPIX[1]][NZ] = DD;
					if ((NX != 0) && (NY != 0) && (NZ == 0)) DELC[NX][NY][NPIX[2]] = DD;
				} //---------- NSYM
			} //---------- NSUB
		} //---------- NCEN
	}
	fclose(mfile);
	if ((OBJLEV > HMAX) || (OBJLEV < HMIN)) {
		OBJLEV = (float)fabs(HMAX/2);
	}
	if (HMIN == 0) HMIN = 0.000001f;
	iso_pm_parm = 1;
	DELC_ID = 0;

	return(1);
}

//*****************************  readMEED_Tfile()  ***************************//
int readMEED4file(int ID,char *fname)
{
	FILE *mfile;
	char line[256];
	int comentID,comentN;

	int i,j,l,k,I1,I2;
	float density1,density2,density3;
	int x,y,z;
	float t1,t2,t3;
	int ISX,ISY,ISZ;
	int NX,NY,NZ;
	int KX,KY,KZ;
	float DD;

	HMAX = -10000.0;
	HMIN = 10000.0;

	sprintf(line,"");

//====================================================================================
    mfile=fopen(fname,"r");
	comentID = 0;
	comentN = 0;
	while (comentID != 1){
		fgets( line, 256, mfile); 
		if ((line[0] == '#') || (line[0] == '*')) {
			comentID = 0;
			comentN++;
		}
		else comentID = 1;
	}
	rewind(mfile);
	for(i=0; i<comentN; i++) fgets( line, 256, mfile); 
//--------------------------------------------------- Title
	sprintf(line,"%s","");
	fgets( line, 256, mfile); 
	comentN++;
	sscanf(line,"%79c",&Title);
//--------------------------------------------------- Number of pixel in asym. unit.
    fscanf( mfile, "%i\n", &NASYM);
	comentN++;
//--------------------------------------------------- Read 3D-Data
	printf("\nReading 3D data from file %s\n\n",fname);
	
	for(i=0; i<NASYM; i++){
		switch(ID){
		case(4):{
			fscanf( mfile, "%i %i %i %E\n",&x,&y,&z,&density1);
			break;
				}
		case(5):{
			fscanf( mfile, "%i %i %i %E %E\n",&x,&y,&z,&density1,&density2);
			break;
				}
		case(6):{
			fscanf( mfile, "%i %i %i %E %E %E\n",&x,&y,&z,&density1,&density2,&density3);
			break;
				}
		}
	}
//--------------------------------------------------- Read Unit Cell Parameters
	fscanf( mfile, "%f %f %f %f %f %f\n", &cCell[0],&cCell[1],&cCell[2],&cCell[3],&cCell[4],&cCell[5]);
//--------------------------------------------------- Read Dimensions and Symmetry settings
	fscanf( mfile, "%i %i %i %i %i %i\n", &NPIX[0],&NPIX[1],&NPIX[2],&NSYM,&NCEN,&NSUB);
    if((NPIX[0] > MAX_PIX) || (NPIX[1] > MAX_PIX) || (NPIX[2] > MAX_PIX)) {
		printf("======================================================\n");
		printf("Number of pixels out of range\n MAX_PIX=%i\n",MAX_PIX);
		printf("======================================================\n");
		fclose(mfile);
		return(-1);
	}
//--------------------------------------------------- Read Symmetry Matrix
	for(i=0;i<NSYM;i++){
		if (ID == 5) {
			fscanf( mfile,"%i %i %i %i %i %i %i %i %i %i %i %i\n",
				           &sPos[i].a[0][0],&sPos[i].a[1][0],&sPos[i].a[2][0],
						   &sPos[i].a[0][1],&sPos[i].a[1][1],&sPos[i].a[2][1],
						   &sPos[i].a[0][2],&sPos[i].a[1][2],&sPos[i].a[2][2],
						   &sPos[i].t[0],&sPos[i].t[1],&sPos[i].t[2]);
		}
		else {
			fscanf( mfile,"%i %i %i %i %i %i %i %i %i %f %f %f\n",
				           &sPos[i].a[0][0],&sPos[i].a[0][1],&sPos[i].a[0][2],
						   &sPos[i].a[1][0],&sPos[i].a[1][1],&sPos[i].a[1][2],
						   &sPos[i].a[2][0],&sPos[i].a[2][1],&sPos[i].a[2][2],
						   &t1,&t2,&t3);
			sPos[i].t[0] = (int)(t1*NPIX[0]);
			sPos[i].t[1] = (int)(t2*NPIX[1]);
			sPos[i].t[2] = (int)(t3*NPIX[2]);
		}
	}
//--------------------------------------------------- Read Brave Translations
	for(i=0;i<NSUB;i++){
		if (ID == 5) {
			fscanf( mfile,"%i %i %i\n",&ISUB[0][i],&ISUB[1][i],&ISUB[2][i]);
		}
		else {
			fscanf( mfile,"%f %f %f\n",&t1,&t2,&t3);
			ISUB[0][i] = (int)(t1*NPIX[0]);
			ISUB[1][i] = (int)(t2*NPIX[1]);
			ISUB[2][i] = (int)(t3*NPIX[2]);
		}
	}
//====================================================================================
	rewind(mfile);
	I1 = 0;
	I2 = 0;
	for(i=0; i<comentN; i++) fgets( line, 256, mfile); 
	for(i=0; i<NASYM; i++){
		switch(ID){
		case(4):{
			fscanf( mfile, "%i %i %i %E\n",&x,&y,&z,&density1);
			break;
				}
		case(5):{
			fscanf( mfile, "%i %i %i %E %E\n",&x,&y,&z,&density1,&density2);
			density1 = density1 + density2;
			break;
				}
		case(6):{
			fscanf( mfile, "%i %i %i %E %E %E\n",&x,&y,&z,&density1,&density2,&density3);
			break;
				}
		}
		KX = x;
		KY = y;
		KZ = z;
		DD = density1;
		if (DD >= 0.0) I1 = 1;
		if (DD < 0.0) I2 = 1;
		HMIN = (DD <= HMIN) ? DD : HMIN;
		HMAX = (DD >= HMAX) ? DD : HMAX;

		for(j=0;j<NCEN+1;j++){
			for(l=0;l<NSUB;l++){
				ISX = ISUB[0][l]; ISY = ISUB[1][l]; ISZ = ISUB[2][l];
				for(k=0;k<NSYM;k++){
					NX = KX*sPos[k].a[0][0] + KY*sPos[k].a[0][1] + KZ*sPos[k].a[0][2] + sPos[k].t[0];
					NY = KX*sPos[k].a[1][0] + KY*sPos[k].a[1][1] + KZ*sPos[k].a[1][2] + sPos[k].t[1];
					NZ = KX*sPos[k].a[2][0] + KY*sPos[k].a[2][1] + KZ*sPos[k].a[2][2] + sPos[k].t[2];
					if (j==1){NX = -NX; NY = -NY;	NZ = -NZ;}
					NX = NX + 2*NPIX[0] + ISX;
					NY = NY + 2*NPIX[1] + ISY;
					NZ = NZ + 2*NPIX[2] + ISZ;
					NX = NX - (NX/NPIX[0])*NPIX[0];
					NY = NY - (NY/NPIX[1])*NPIX[1];
					NZ = NZ - (NZ/NPIX[2])*NPIX[2];
					DELC[NX][NY][NZ] = DD;
					if ((NX == 0) && (NY == 0) && (NZ == 0)){
						DELC[NPIX[0]][0][0] = DD;
						DELC[0][NPIX[1]][0] = DD;
						DELC[0][0][NPIX[2]] = DD;
						DELC[NPIX[0]][NPIX[1]][0] = DD;
						DELC[NPIX[0]][0][NPIX[2]] = DD;
						DELC[0][NPIX[1]][NPIX[2]] = DD;
						DELC[NPIX[0]][NPIX[1]][NPIX[2]] = DD;
					}
					if ((NX == 0) && (NY == 0) && (NZ != 0)){
						DELC[NPIX[0]][0][NZ] = DD;
						DELC[0][NPIX[1]][NZ] = DD;
						DELC[NPIX[0]][NPIX[1]][NZ] = DD;
					}
					if ((NX == 0) && (NY != 0) && (NZ == 0)){
						DELC[NPIX[0]][NY][0] = DD;
						DELC[0][NY][NPIX[2]] = DD;
						DELC[NPIX[0]][NY][NPIX[2]] = DD;
					}
					if ((NX != 0) && (NY == 0) && (NZ == 0)){
						DELC[NX][NPIX[1]][0] = DD;
						DELC[NX][0][NPIX[2]] = DD;
						DELC[NX][NPIX[1]][NPIX[2]] = DD;
					}
					if ((NX == 0) && (NY != 0) && (NZ != 0)) DELC[NPIX[0]][NY][NZ] = DD;
					if ((NX != 0) && (NY == 0) && (NZ != 0)) DELC[NX][NPIX[1]][NZ] = DD;
					if ((NX != 0) && (NY != 0) && (NZ == 0)) DELC[NX][NY][NPIX[2]] = DD;
				} //---------- NSYM
			} //---------- NSUB
		} //---------- NCEN
	}
//====================================================================================
	fclose(mfile);

	if ((I1 == 1) && (I2 == 1)) {
		iso_pm_parm = 0;
		DELC_ID = 1;
	}
	if ((I1 == 1) && (I2 == 0)) {
		iso_pm_parm = 1;
		DELC_ID = 0;
	}
	if ((I1 == 0) && (I2 == 1)) {
		iso_pm_parm = 2;
		DELC_ID = 2;
	}

	if ((OBJLEV > HMAX) || (OBJLEV < HMIN)) {
		OBJLEV = (float)fabs(HMAX/2);
	}
	if (HMIN == 0) HMIN = 0.000001f;
//	HMIN = 0.0;
	return(1);
}




//******************************************** Check file format
int readMEEDfile(char *fname)
{
	int i,j;
	FILE *mfile;
	char line[256];
	int comentID,comentN;
	char seps[]   = " ,\t\n";
	char *token;


//====================================================================================
    mfile = fopen(fname,"r");
	if (mfile == NULL) {
		return -1;
	}
	comentID = 0;
	comentN = 0;
	while (comentID != 1){
		fgets( line, 256, mfile); 
		if ((line[0] == '#') || (line[0] == '*')) {
			comentID = 0;
			comentN++;
		}
		else comentID = 1;
	}
	rewind(mfile);
	for(i=0; i<comentN; i++) fgets( line, 256, mfile); 
//--------------------------------------------------- 
	for(i=0; i<2; i++) fgets( line, 256, mfile); 
	sprintf(line,"");
	fgets( line, 256, mfile); 
	j = 0;
	token = strtok( line, seps );
	while( token != NULL ) {
		token = strtok( NULL, seps );
		j++;
	}
	fclose(mfile);
	if (j == 8) {
		i = readMEED3file(fname);den_file_ID = 0;return i;
	}
	else if ((j == 4) || (j == 5) || (j == 6)) {
		i = readMEED4file(j,fname); 
		den_file_ID = 1;
		return i;
	}
	return(0);
}



//*****************************  readSCRIPT()  ***************************//
int readSCRIPT(char *name)
{
	FILE *fptr;
	char line[80],cmd[256];
	int i,l;

	fptr = fopen(name,"r");
	fgets( line, 80, fptr); 
	sprintf(cmd,"");
	fgets( cmd, 256, fptr); 
	l = strlen(cmd);
//	sprintf(FileName,"");
//	strncat(FileName,cmd,strlen(cmd));
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i\n",&fileID);
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fscanf(fptr,"%f %f\n",&OBJLEV,&SECLEV);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i\n",&model_mode);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i %i %i\n",&numBox[0],&numBox[1],&numBox[2]);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%f %f %f\n",&ORIGIN[0],&ORIGIN[1],&ORIGIN[2]);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%f %f  %f %f  %f %f\n",&range[0],&range[1],&range[2],&range[3],&range[4],&range[5]);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%f %f %f %f\n",&mat_ambient[0],&mat_ambient[1],&mat_ambient[2],&mat_ambient[3]);
	fscanf(fptr,"%f %f %f %f\n",&mat_diffuse[0],&mat_diffuse[1],&mat_diffuse[2],&mat_diffuse[3]);
	fscanf(fptr,"%f %f %f %f\n",&mat_specular[0],&mat_specular[1],&mat_specular[2],&mat_specular[3]);
	fscanf(fptr,"%f %f %f %f\n",&mat_emission[0],&mat_emission[1],&mat_emission[2],&mat_emission[3]);
	fscanf(fptr,"%f\n",&mat_shininess);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i %i\n",&show_iso,&show_sls);
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i\n",&palette_hkl);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i %f %f %f\n",&palette_hkl, &objlev_scale, &seclev_scale, &opacity_hkl);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%i\n",&num_of_hkl);
	if (num_of_hkl > 0){
		for (i=0; i<num_of_hkl; i++){
			fscanf(fptr,"%i %i %i %f\n",&index_hkl[i][0],&index_hkl[i][1],&index_hkl[i][2],
				                               &position_hkl[i]);
			index_show_hkl[i] = 0;
		}
	}
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fgets( line, 80, fptr); 
	fscanf(fptr,"%f %f %f\n",&BKCL[0],&BKCL[1],&BKCL[2]);
	fgets( line, 80, fptr); 
	fscanf(fptr,"%f %f %f\n",&unit_cell_color[0],&unit_cell_color[1],&unit_cell_color[2]);
	fscanf(fptr,"%i %i %f\n",&unit_cell_show,&unit_cell_type, &unit_cell_width);
	fgets( line, 80, fptr); 
	for(i=0; i<4; i++) {
		fscanf( fptr,"%f %f %f %f\n",&rotation[i],&rotation[i+4],&rotation[i+8],&rotation[i+12]);
	}
	fscanf( fptr,"%f %f\n",&obj_pos[0],&obj_pos[1]);
	fscanf( fptr,"%f\n",&obj_pos[2]);
	fscanf( fptr,"%f\n",&scale);

	if ((ORIGIN[0] != 0) || (ORIGIN[1] != 0) || (ORIGIN[2] != 0)) {
		range[0] = range[2] = range[4] = 0;
		range[1] = range[3] = range[5] = 1;
	}

	rewind(fptr);
	fgets( line, 80, fptr); 
	sprintf(FileName,"");
	fgets(FileName, l, fptr); 

	fclose(fptr);
	return(0);
}






//****************************************************************//
//                 Read Data From File                            //
//----------------------------------------------------------------//
// control ID = 0 -- MEED            File                         //
//              1 -- VIEN2k          File                         //
//****************************************************************//
//*****************************  read_data_from_file()  *********************//
int read_data_from_file(int ID, char *fname)
{
	FILE *mfile;
	int FileControl = -1;

	mfile=fopen(fname,"r");
	if( mfile == NULL ) {
		printf( "The file (%s) was not opened\n",fname );
		return(-1);
	}
	else {
		fclose(mfile);
		set_initital_parameters();
//---------------------------------------------------
		switch(ID)
		{
//----------------------------------------------- VICS file
		case(0):{FileControl = readMEEDfile(fname); break;}
		case(1):{FileControl = readCUBEfile(fname); break;}
		case(2):{FileControl = readDVXfile(fname);  break;}
		case(3):{FileControl = readVIENfile(fname); break;}
		case(4):{
			readSCRIPT(fname); 
			fname = FileName;
			printf("www [%s] [%i]\n",fname,fileID);

			switch(fileID)
			{
			case(0):{FileControl = readMEEDfile(fname); break;}
			case(1):{FileControl = readCUBEfile(fname); break;}
		    case(2):{FileControl = readDVXfile(fname);  break;}
		    case(3):{FileControl = readVIENfile(fname); break;}
		    case(5):{FileControl = readELENfile(fname); break;}
		    case(6):{FileControl = readVIENfile(fname); break;}
			}
			break;
				}
		case(5):{FileControl = readELENfile(fname); break;}
		case(6):{FileControl = readVIENfile(fname); break;}
		}
	}
	return(FileControl);
}




/********************************************************************/
/**                      Set Orthogonal Matrix                     **/
/********************************************************************/
void set_orthoMatrix()
{
	double a,b,c;
	double cosA,cosB,cosG,sinA,sinB,sinG;
	double cosAR,sinAR;
	double V;
	a = cCell[0];
	b = cCell[1];
	c = cCell[2];
	cosA = cos(cCell[3]*PI/180); sinA = sqrt(1 - cosA*cosA);
	cosB = cos(cCell[4]*PI/180); sinB = sqrt(1 - cosB*cosB);
	cosG = cos(cCell[5]*PI/180); sinG = sqrt(1 - cosG*cosG);
	V = a*b*c*sqrt(1 - cosA*cosA - cosB*cosB - cosG*cosG + 2*cosA*cosB*cosG);
    sinB = sqrt(1 - cosB*cosB);
	sinG = sqrt(1 - cosG*cosG);
	cosAR = (cosB*cosG - cosA)/(sinB*sinG);
	sinAR = sqrt(1 - cosAR*cosAR);

	ortho_M[0][0] = 1;
	ortho_M[0][1] = cosG;
	ortho_M[0][2] = cosB;

	ortho_M[1][0] = 0;
	ortho_M[1][1] = sinG;
	ortho_M[1][2] = -sinB*cosAR;

	ortho_M[2][0] = 0;
	ortho_M[2][1] = 0;
	ortho_M[2][2] = sinB*sinAR;
}

/********************************************************************/
/**                  Convert a, b, c to OrthoABC                   **/
/********************************************************************/
void set_Ortho_ABC()
{
	int i;
	double x,y,z,x1,y1,z1;

	x = cCell[0];
	y = cCell[1];
	z = cCell[2];
	x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
	y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
	z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
	ortho_cCell[0] = (float)x1;
	ortho_cCell[1] = (float)y1;
	ortho_cCell[2] = (float)z1;
	max_P = maxf(ortho_cCell[0],ortho_cCell[1],ortho_cCell[2]);

	uCell[0][0] = 0; uCell[0][1] = 0; uCell[0][2] = 0;
	uCell[1][0] = 1; uCell[1][1] = 0; uCell[1][2] = 0;
	uCell[2][0] = 1; uCell[2][1] = 0; uCell[2][2] = 1;
	uCell[3][0] = 0; uCell[3][1] = 0; uCell[3][2] = 1;
	uCell[4][0] = 0; uCell[4][1] = 1; uCell[4][2] = 1;
	uCell[5][0] = 1; uCell[5][1] = 1; uCell[5][2] = 1;
	uCell[6][0] = 1; uCell[6][1] = 1; uCell[6][2] = 0;
	uCell[7][0] = 0; uCell[7][1] = 1; uCell[7][2] = 0;
	for(i=0; i<8; i++) {
		x = uCell[i][0]*cCell[0];
		y = uCell[i][1]*cCell[1];
		z = uCell[i][2]*cCell[2];
		x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
		y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
		z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
		uCell[i][0] = (float)x1/max_P;
		uCell[i][1] = (float)y1/max_P;
		uCell[i][2] = (float)z1/max_P;
	}

	origin[0] = -ortho_cCell[0]/(2*max_P);
	origin[1] = -ortho_cCell[1]/(2*max_P);
	origin[2] = -ortho_cCell[2]/(2*max_P);

	uAxis[0][0] = 0.08f; uAxis[0][1] = 0;     uAxis[0][2] = 0;
	uAxis[1][0] = 0;     uAxis[1][1] = 0.08f; uAxis[1][2] = 0;
	uAxis[2][0] = 0;     uAxis[2][1] = 0;     uAxis[2][2] = 0.08f;
	for(i=0; i<3; i++){
		x = uAxis[i][0];
		y = uAxis[i][1];
		z = uAxis[i][2];
		x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
		y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
		z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
		uAxis[i][0] = (float)x1;
		uAxis[i][1] = (float)y1;
		uAxis[i][2] = (float)z1;
	}
}
					

void saveSCRIPT(char *name)
{
	FILE *fptr;
	int i;

	fptr = fopen(name,"w");
	fprintf(fptr,"# Data file name\n");
	fprintf(fptr,"%s\n",FileName);
	fprintf(fptr,"# Data file format\n");
	fprintf(fptr,"%i\n",fileID);
	fprintf(fptr,"#\n");
	fprintf(fptr,"#-- ISOSURFACE CONTROL ---\n");
	fprintf(fptr,"#\n");
	fprintf(fptr,"# Isosurface level and f(section)\n");
	fprintf(fptr,"%f %f\n",OBJLEV,SECLEV);
	fprintf(fptr,"# Isosurface mode (Surface, Frame, Mesh)\n");
	fprintf(fptr,"%i\n",model_mode);
	fprintf(fptr,"# Number of cells\n");
	fprintf(fptr,"%i %i %i\n",numBox[0],numBox[1],numBox[2]);
	fprintf(fptr,"# Origin\n");
	fprintf(fptr,"%5.3f %5.3f %5.3f\n",ORIGIN[0],ORIGIN[1],ORIGIN[2]);
	fprintf(fptr,"# Inside boundaries\n");
	fprintf(fptr,"%5.3f %5.3f  %5.3f %5.3f  %5.3f %5.3f\n",range[0],range[1],range[2],range[3],range[4],range[5]);
	fprintf(fptr,"# Colors (ambient, diffuse, specular, emission, shininess)\n");
	fprintf(fptr,"%5.3f %5.3f %5.3f %5.3f\n",mat_ambient[0],mat_ambient[1],mat_ambient[2],mat_ambient[3]);
	fprintf(fptr,"%5.3f %5.3f %5.3f %5.3f\n",mat_diffuse[0],mat_diffuse[1],mat_diffuse[2],mat_diffuse[3]);
	fprintf(fptr,"%5.3f %5.3f %5.3f %5.3f\n",mat_specular[0],mat_specular[1],mat_specular[2],mat_specular[3]);
	fprintf(fptr,"%5.3f %5.3f %5.3f %5.3f\n",mat_emission[0],mat_emission[1],mat_emission[2],mat_emission[3]);
	fprintf(fptr,"%7.3f\n",mat_shininess);
	fprintf(fptr,"# Show isosurface and show slices (boundaries)\n");
	fprintf(fptr,"%i %i\n",show_iso,show_sls);
	fprintf(fptr,"#\n");
	fprintf(fptr,"#-- SLICES CONTROL ---\n");
	fprintf(fptr,"#\n");
	fprintf(fptr,"# Palette\n");
	fprintf(fptr,"%i\n",palette_hkl);
	fprintf(fptr,"# Scale factors for isosurface, f(section); Opacity\n");
	fprintf(fptr,"%i %5.3f %5.3f %5.3f\n",palette_hkl, objlev_scale, seclev_scale, opacity_hkl);
	fprintf(fptr,"# (hkl) planes\n");
	fprintf(fptr,"%i\n",num_of_hkl);
	if (num_of_hkl > 0){
		for (i=0; i<num_of_hkl; i++){
			fprintf(fptr,"%i %i %i %6.3f\n",index_hkl[i][0],index_hkl[i][1],index_hkl[i][2],
				                               position_hkl[i]);
		}
	}
	fprintf(fptr,"#\n");
	fprintf(fptr,"#-- GENERAL CONTROL ---\n");
	fprintf(fptr,"#\n");
	fprintf(fptr,"# Background color\n");
	fprintf(fptr,"%5.3f %5.3f %5.3f\n",BKCL[0],BKCL[1],BKCL[2]);
	fprintf(fptr,"# Unit cell\n");
	fprintf(fptr,"%5.3f %5.3f %5.3f\n",unit_cell_color[0],unit_cell_color[1],unit_cell_color[2]);
	fprintf(fptr,"%3i %3i %5.3f\n",unit_cell_show,unit_cell_type, unit_cell_width);

//------------------------------------------------------ SCENE
	fprintf(fptr,"# Scene\n");
	for(i=0; i<4; i++) {
		fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",rotation[i],rotation[i+4],rotation[i+8],rotation[i+12]);
	}
	fprintf(fptr,"%7.3f %7.3f\n",obj_pos[0],obj_pos[1]);
	fprintf(fptr,"%7.3f\n",obj_pos[2]);
	fprintf(fptr,"%7.3f\n",scale);
	fclose(fptr);
}



