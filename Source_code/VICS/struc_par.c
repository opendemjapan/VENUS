#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "struc_par.h"
#include "matrix.h"


FILE *mfile;
int result;
int controlID = 0;
int line_shift[MAX_ATOMS_IN_ASYMM];
int num_shift;
int  origID = 0;    //--- Origin ID for Orthorhombic groups (48,50,59,68,70)


double G[6],TR[6];
MATRIX RVS,RTS;



//*******************************************************************************
void check_file_ext_exist(int form, char *fName, int mode)
{
   char name[256];
   char *dest;
   char *ext;

   dest = "";
   ext = "";
   dest = strrchr(fName, '\\');
   if (dest == NULL) {
         dest = strrchr(fName, '/');
         if (dest == NULL) dest = fName;
   }
   sprintf(name,"");
   sprintf(name,"%s",dest);
   dest = strrchr(name, '.');
   if (dest == NULL){
	   switch(mode){
//---------------------------------------- Save VCS File
	   case(0):{ext = ".vcs"; break;}
//---------------------------------------- Export File
	   case(1):{
		   switch(form){
		   case(0):{ext = ".cif"; break;}
		   case(1):{ext = ".pdb"; break;}
		   case(2):{ext = ".ins"; break;}
		   case(3):{ext = ".xyz"; break;}
		   }
		   break;
			   }
//---------------------------------------- Export Image
	   case(2):{
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
int check_format(char *pdest)
{
   char *name;

   name = "";
   name = strrchr( pdest, '.');
   if (name != NULL) name++;
   else {
	   if(strstr( name, "f01") != NULL) {
		   return 16;
	   }
	   else {
		   if(strstr( name, "F01") != NULL) return 16;
		   else return -1;
	   }
   }
//------------------------------------------------
   if(strstr( name, "vcs") != NULL ) return 0;
   if(strstr( name, "VCS") != NULL ) return 0;
//------------------------------------------------
   if(strstr( name, "amc") != NULL ) return 1;
   if(strstr( name, "AMC") != NULL ) return 1;
//------------------------------------------------
   if(strstr( name, "CC1") != NULL ) return 2;
   if(strstr( name, "cc1") != NULL ) return 2;
//------------------------------------------------
   if(strstr( name, "cif") != NULL ) return 3;
   if(strstr( name, "CIF") != NULL ) return 3;
//------------------------------------------------
   if(strstr( name, "cmt") != NULL ) return 4;
   if(strstr( name, "CMT") != NULL ) return 4;
//------------------------------------------------
   if(strstr( name, "cry") != NULL ) return 5;
   if(strstr( name, "CRY") != NULL ) return 5;
//------------------------------------------------
   if(strstr( name, "CSSR") != NULL ) return 6;
   if(strstr( name, "cssr") != NULL ) return 6;
   if(strstr( name, "css") != NULL )  return 6;
   if(strstr( name, "CSS") != NULL )  return 6;
//------------------------------------------------
   if(strstr( name, "FDT") != NULL ) return 7;
   if(strstr( name, "fdt") != NULL ) return 7;
//------------------------------------------------
   if(strstr( name, "CSD") != NULL ) return 7;
   if(strstr( name, "csd") != NULL ) return 7;
//------------------------------------------------
   if(strstr( name, "cube") != NULL ) return 8;
   if(strstr( name, "CUBE") != NULL ) return 8;
   if(strstr( name, "cub")  != NULL ) return 8;
   if(strstr( name, "CUB")  != NULL ) return 8;
//------------------------------------------------
   if(strstr( name, "ics") != NULL ) return 9;
   if(strstr( name, "ICS") != NULL ) return 9;
//------------------------------------------------
   if(strstr( name, "mol") != NULL ) return 10;
   if(strstr( name, "MOL") != NULL ) return 10;
//------------------------------------------------
   if(strstr( name, "min") != NULL ) return 11;
   if(strstr( name, "MIN") != NULL ) return 11;
//------------------------------------------------
   if(strstr( name, "mld") != NULL ) return 12;
   if(strstr( name, "MLD") != NULL ) return 12;
//------------------------------------------------
   if(strstr( name, "pdb") != NULL ) return 13;
   if(strstr( name, "PDB") != NULL ) return 13;
//------------------------------------------------
   if(strstr( name, "ins") != NULL ) return 14;
   if(strstr( name, "INS") != NULL ) return 14;
//------------------------------------------------
   if(strstr( name, "struct") != NULL ) return 15;
   if(strstr( name, "STRUCT") != NULL ) return 15;
   if(strstr( name, "str") != NULL ) return 15;
   if(strstr( name, "STR") != NULL ) return 15;
//------------------------------------------------
   if(strstr( name, "XYZ") != NULL ) return 16;
   if(strstr( name, "xyz") != NULL ) return 16;
//------------------------------------------------
   if(strstr( name, "ASSE") != NULL ) return 17;
   if(strstr( name, "asse") != NULL ) return 17;
   if(strstr( name, "ASS") != NULL ) return 17;
   if(strstr( name, "ass") != NULL ) return 17;
//------------------------------------------------
   if(strstr( name, "DAT") != NULL ) return 19;
   if(strstr( name, "dat") != NULL ) return 19;
//------------------------------------------------

   return -1;
}

//****************************************************************//
//         Convert string from uppercase to lowercase             //
//****************************************************************//
void Up2Lo(char *param)
{
	int i;
	for(i=0; i<(int)strlen(param); i++){
		if (param[i] == 'A') param[i] = 'a';
		if (param[i] == 'B') param[i] = 'b';
		if (param[i] == 'C') param[i] = 'c';
		if (param[i] == 'D') param[i] = 'd';
		if (param[i] == 'N') param[i] = 'n';
		if (param[i] == 'M') param[i] = 'm';
	}
}


//*******************************************************************************
float maxf(float a, float b, float c)
{
	float d;
	d = (a >= b) ? a : b;
	d = (d >= c) ? d : c;
	return(d);
}

//*******************************************************************************
float minf(float a, float b, float c)
{
	float d;
	d = (a <= b) ? a : b;
	d = (d <= c) ? d : c;
	return(d);
}

//*******************************************************************************
void save_default_pref()
{
	FILE *fptr;
	char path[256];
	int i;

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,DEFPREF);

	fptr = fopen(path,"w");
	fprintf(fptr,"%2i %2i %2i %2i\n",def_atom_s_stick,def_atom_s_slice,def_atom_o_stick,def_atom_o_slice);
	fprintf(fptr,"%2i %2i %2i %2i\n",def_bond_s_stick,def_bond_s_slice,def_bond_o_stick,def_bond_o_slice);
	fprintf(fptr,"%6.3f %6.3f %6.3f %6.3f\n",def_ucel_line_width,def_poly_line_width,def_hkl_line_width,def_bond_line_width);
	fprintf(fptr,"%2i\n",def_ucel_type);
	fprintf(fptr,"%6.3f %6.3f\n",def_poly_tran,def_hkl_tran);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_bkgr_color[0],def_bkgr_color[1],def_bkgr_color[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_ucel_color[0],def_ucel_color[1],def_ucel_color[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_poly_color[0],def_poly_color[1],def_poly_color[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_hkl_color[0],def_hkl_color[1],def_hkl_color[2]);
	fprintf(fptr,"%6.3f %6.3f %6.3f\n",def_bond_color[0],def_bond_color[1],def_bond_color[2]);
	fprintf(fptr,"%6.3f %6.3f\n",def_bond_rad[0],def_bond_rad[1]);
	fprintf(fptr,"%2i\n",def_show_axis);
	fprintf(fptr,"%6.3f\n",stepw_angle);
	fprintf(fptr,"%6.3f %i\n",def_mol_bond,def_search_bond);
	fprintf(fptr,"%2i\n",def_file_format_ID);
	fprintf(fptr,"%s\n",t_FileName);

//	fprintf(fptr,"%i\n",save_pos_ID);
	fprintf(fptr,"%i %i\n",menu_x, menu_y);
	fprintf(fptr,"%i %i\n",tools_x, tools_y);
	fprintf(fptr,"%i %i %i %i\n",graph_x, graph_y, graph_w, graph_h);
	for(i=0; i<15; i++){
		fprintf(fptr,"%i %i\n",cdwp[0][i],cdwp[1][i]);
	}

	fclose(fptr);
}


//*******************************************************************************
void load_default_pref()
{
	FILE *fptr;
	char path[256],line [256];
	int i,l;

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,DEFPREF);

	fptr = fopen(path,"r");

	fscanf(fptr,"%i %i %i %i",&def_atom_s_stick,&def_atom_s_slice,&def_atom_o_stick,&def_atom_o_slice);
	fscanf(fptr,"%i %i %i %i",&def_bond_s_stick,&def_bond_s_slice,&def_bond_o_stick,&def_bond_o_slice);
	fscanf(fptr,"%f %f %f %f\n",&def_ucel_line_width,&def_poly_line_width,&def_hkl_line_width,&def_bond_line_width);
	fscanf(fptr,"%i\n",&def_ucel_type);
	fscanf(fptr,"%f %f\n",&def_poly_tran,&def_hkl_tran);
	fscanf(fptr,"%f %f %f\n",&def_bkgr_color[0],&def_bkgr_color[1],&def_bkgr_color[2]);
	fscanf(fptr,"%f %f %f\n",&def_ucel_color[0],&def_ucel_color[1],&def_ucel_color[2]);
	fscanf(fptr,"%f %f %f\n",&def_poly_color[0],&def_poly_color[1],&def_poly_color[2]);
	fscanf(fptr,"%f %f %f\n",&def_hkl_color[0],&def_hkl_color[1],&def_hkl_color[2]);
	fscanf(fptr,"%f %f %f\n",&def_bond_color[0],&def_bond_color[1],&def_bond_color[2]);
	fscanf(fptr,"%f %f\n",&def_bond_rad[0],&def_bond_rad[1]);
	fscanf(fptr,"%i\n",&def_show_axis);
	fscanf(fptr,"%f\n",&stepw_angle);
	fscanf(fptr,"%f %i\n",&def_mol_bond,&def_search_bond);
	fscanf(fptr,"%i\n",&def_file_format_ID);
	sprintf(line,"");
	fgets( line, 256, fptr);
	l = strlen(line);

	fscanf(fptr,"%i %i\n",&menu_x, &menu_y);
	fscanf(fptr,"%i %i\n",&tools_x, &tools_y);
	fscanf(fptr,"%i %i %i %i\n",&graph_x, &graph_y, &graph_w, &graph_h);

	for(i=0; i<15; i++){
		fscanf(fptr,"%i %i\n",&cdwp[0][i],&cdwp[1][i]);
		dwp[0][i] = cdwp[0][i];
		dwp[1][i] = cdwp[1][i];
	}
	rewind(fptr);
	for(i=0; i<15; i++) fgets( line, 256, fptr);
	fgets(t_FileName, l, fptr);
	fclose(fptr);

	fptr = fopen(t_FileName,"r");
	if( fptr == NULL ) {
		sprintf(t_FileName,"");
		sprintf(t_FileName,"%s%s",progvar,TEMPLATE);
	}
	else fclose(fptr);
}

//*******************************************************************************
void loadDefPar()
{
	FILE *fptr;
	int i; 
	char path[256];

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,INIFILE);

	fptr = fopen(path,"r");
	if( fptr == NULL ) {
		printf( "The file (%s) was not opened\n",INIFILE );getchar();exit(0);}
	else fclose(fptr);
	fptr = fopen(path,"r");
	for (i=0;i<96;i++){
		fscanf(fptr,"%i %s %f %f %f %f %f %f\n",&dPar[i].atomN, &dPar[i].atsymb, 
			         &dPar[i].Ra,&dPar[i].Rv,&dPar[i].Ri,
				     &dPar[i].RGB[0], &dPar[i].RGB[1], &dPar[i].RGB[2]);
    }
	fclose(fptr);
}


//*******************************************************************************
void set_atom_R_and_RGB()
{
	int i,j;
	int control = 0;
	char *pos;

	for(i=0; i<nAtom; i++){
		for (j=0; j<96;j++){
			lAtom[i].R=dPar[95].Ra;
			pos = strstr(dPar[j].atsymb,lAtom[i].simb);
			if ((pos != NULL) && (strlen(dPar[j].atsymb) == strlen(lAtom[i].simb))){
				lAtom[i].color[0] = dPar[j].RGB[0];
				lAtom[i].color[1] = dPar[j].RGB[1];
				lAtom[i].color[2] = dPar[j].RGB[2];
				switch(radii_type)
				{
				case(0):{lAtom[i].R=dPar[j].Ra;break;}   //--- Atomic Radii
				case(1):{lAtom[i].R=dPar[j].Ri;break;}   //--- Ionic Radii
				case(2):{lAtom[i].R=dPar[j].Rv;break;}   //--- Van der Vaals Radii
				}
				break;
			}
		}
	}
	if (tAtom > 0) {
		for(i=0; i<tAtom; i++){
			for (j=0; j<96;j++){
				pos = strstr(dPar[j].atsymb,AtomT[i].symb);
				if ((pos != NULL) && (strlen(dPar[j].atsymb) == strlen(AtomT[i].symb))){
					AtomT[i].color[0] = dPar[j].RGB[0];
					AtomT[i].color[1] = dPar[j].RGB[1];
					AtomT[i].color[2] = dPar[j].RGB[2];
					switch(radii_type)
					{
					case(0):{AtomT[i].R=dPar[j].Ra;break;}   //--- Atomic Radii
					case(1):{AtomT[i].R=dPar[j].Ri;break;}   //--- Ionic Radii
					case(2):{AtomT[i].R=dPar[j].Rv;break;}   //--- Van der Vaals Radii
					}
					break;
				}
			}
		}
	}
}



//*******************************************************************************
void set_atom_R()
{
	int i,j;
	int control = 0;
	char *pos;

	for(i=0; i<nAtom; i++){
		for (j=0; j<96;j++){
			lAtom[i].R=dPar[95].Ra;
			pos = strstr(dPar[j].atsymb,lAtom[i].simb);
			if ((pos != NULL) && (strlen(dPar[j].atsymb) == strlen(lAtom[i].simb))){
				switch(radii_type)
				{
				case(0):{lAtom[i].R=dPar[j].Ra;break;}   //--- Atomic Radii
				case(1):{lAtom[i].R=dPar[j].Ri;break;}   //--- Ionic Radii
				case(2):{lAtom[i].R=dPar[j].Rv;break;}   //--- Van der Vaals Radii
				}
				break;
			}
		}
	}
	if (tAtom > 0) {
		for(i=0; i<tAtom; i++){
			for (j=0; j<96;j++){
				pos = strstr(dPar[j].atsymb,AtomT[i].symb);
				if ((pos != NULL) && (strlen(dPar[j].atsymb) == strlen(AtomT[i].symb))){
					switch(radii_type)
					{
					case(0):{AtomT[i].R=dPar[j].Ra;break;}   //--- Atomic Radii
					case(1):{AtomT[i].R=dPar[j].Ri;break;}   //--- Ionic Radii
					case(2):{AtomT[i].R=dPar[j].Rv;break;}   //--- Van der Vaals Radii
					}
					break;
				}
			}
		}
	}
}

//*******************************************************************************
void setAtomT()
{
	int i,j,control;
	char *pos;

	tAtom = 0;
	for(i=0; i<nAtom; i++){
		control = 0;
		for(j=0; j<tAtom; j++){
			pos = strstr(AtomT[j].symb,lAtom[i].simb);
			if ((pos != NULL) && (strlen(AtomT[j].symb) == strlen(lAtom[i].simb))) {
				control = 1;
				break;
			}
		}
		if (control == 0){
			sprintf(AtomT[tAtom].symb,"");
			sprintf(AtomT[tAtom].symb,"%s",lAtom[i].simb);
			AtomT[tAtom].R = lAtom[i].R;
			AtomT[tAtom].color[0] = lAtom[i].color[0];
			AtomT[tAtom].color[1] = lAtom[i].color[1];
			AtomT[tAtom].color[2] = lAtom[i].color[2];
			tAtom++;
		}
	}

}


//*******************************************************************************
void setDefPar1(int ID)
{
	int j;
	char *pos;

	for (j=0; j<96;j++){
		lAtom[ID].R=dPar[95].Ra;
		lAtom[ID].color[0]=dPar[95].RGB[0];
		lAtom[ID].color[1]=dPar[95].RGB[1];
		lAtom[ID].color[2]=dPar[95].RGB[2];
		pos = strstr(dPar[j].atsymb,lAtom[ID].simb);
		if ((pos != NULL) && (strlen(dPar[j].atsymb) == strlen(lAtom[ID].simb))){
			switch(radii_type)
			{
			case(0):{lAtom[ID].R=dPar[j].Ra;break;}   //--- Atomic Radii
			case(1):{lAtom[ID].R=dPar[j].Ri;break;}   //--- Ionic Radii
			case(2):{lAtom[ID].R=dPar[j].Rv;break;}   //--- Van der Vaals Radii
			}
			lAtom[ID].color[0]=dPar[j].RGB[0];
			lAtom[ID].color[1]=dPar[j].RGB[1];
			lAtom[ID].color[2]=dPar[j].RGB[2];
			break;
		}
	}
}

//*******************************************************************************
void setDefPar()
{
	int i,j,k;
	char *pos;
	int control = 0;

	for (i=0; i<nAtom; i++){
		for (j=0; j<96;j++){
			lAtom[i].R=dPar[95].Ra;
			lAtom[i].color[0]=dPar[95].RGB[0];
			lAtom[i].color[1]=dPar[95].RGB[1];
			lAtom[i].color[2]=dPar[95].RGB[2];
			pos = strstr(dPar[j].atsymb,lAtom[i].simb);
			if ((pos != NULL) && (strlen(dPar[j].atsymb) == strlen(lAtom[i].simb))){
				switch(radii_type)
				{
				case(0):{lAtom[i].R=dPar[j].Ra;break;}   //--- Atomic Radii
				case(1):{lAtom[i].R=dPar[j].Ri;break;}   //--- Ionic Radii
				case(2):{lAtom[i].R=dPar[j].Rv;break;}   //--- Van der Vaals Radii
				}
				lAtom[i].color[0]=dPar[j].RGB[0];
				lAtom[i].color[1]=dPar[j].RGB[1];
				lAtom[i].color[2]=dPar[j].RGB[2];
				break;
			}
		}
		control = 0;
		for (k=0; k<tAtom; k++) {
			pos = strstr(lAtom[i].simb,AtomT[k].symb);
			if ((pos != NULL) && (strlen(lAtom[i].simb) == strlen(AtomT[k].symb))) {
				control = 1;
			}
		}
		if (control == 0){
			sprintf(AtomT[tAtom].symb,"");
			sprintf(AtomT[tAtom].symb,"%s",lAtom[i].simb);
			AtomT[tAtom].R = lAtom[i].R;
			AtomT[tAtom].color[0] = lAtom[i].color[0];
			AtomT[tAtom].color[1] = lAtom[i].color[1];
			AtomT[tAtom].color[2] = lAtom[i].color[2];
			tAtom++;
		}
	}
}

//*******************************************************************************
void update_atom_type1()
{
	int i,k,control;
	char *pos;

	tAtom = 0;
	for(i=0; i<nAtom; i++){
		control = 0;
		for (k=0; k<tAtom; k++) {
			pos = strstr(lAtom[i].simb,AtomT[k].symb);
			if (pos != NULL) {
				sprintf(AtomT[tAtom].symb,"");
				sprintf(AtomT[tAtom].symb,"%s",lAtom[i].simb);
				AtomT[tAtom].R = lAtom[i].R;
				AtomT[tAtom].color[0] = lAtom[i].color[0];
				AtomT[tAtom].color[1] = lAtom[i].color[1];
				AtomT[tAtom].color[2] = lAtom[i].color[2];
				tAtom++;
			}
			else setDefPar();
		}
	}
}


void update_atom_type()
{
	int i,k,control;
	char *pos;

	for(i=0; i<nAtom; i++) setDefPar1(i);
	if (tAtom == 0) {}
	else {
		for(i=0; i<nAtom; i++){
			control = 0;
			for (k=0; k<tAtom; k++) {
				pos = strstr(lAtom[i].simb,AtomT[k].symb);
				if ((pos != NULL) && (strlen(lAtom[i].simb) == strlen(AtomT[k].symb))) {
					control = 1;
					lAtom[i].R = AtomT[k].R;
					lAtom[i].color[0] = AtomT[k].color[0]; 
					lAtom[i].color[1] = AtomT[k].color[1];
					lAtom[i].color[2] = AtomT[k].color[2];
					break;
				}
			}
			if (control == 0) setDefPar1(i);
		}
	}
	setAtomT();
}


//***********************************************************************************
int checkEmptyLine(char *param)
{
	char *result;
	result = strpbrk( param, "0123456789qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM" );
	if (result == NULL) return 1;
	if (param[0] == '#') return 1;
	return 0;
}

//***********************************************************************************
void skipLines(FILE *cfile, int Line)
{
	int i;
	char line[80];
	for (i=0; i<Line; i++) fgets( line, sizeof(line), cfile); 
}

//***********************************************************************************
void skipLinesB(FILE *cfile, int Line)
{
	int i;
	char line[90];
	for (i=0; i<Line; i++) fgets( line, sizeof(line), cfile); 
}



//***********************************************************************************
void checkFileFormat(char *name)
{
	FILE *stream;
	char line[256],line1[256];
	char ch;

	mfile = fopen(name,"r");
	stream = tmpfile();
	sprintf(line,"");
	sprintf(line1,"");
	while(!feof(mfile)){
		ch = fgetc(mfile);
		if ((ch == '\r') || (ch == '\n')){
			fprintf(stream,"%s\n",line);
			sprintf(line,"");
		}
		else {
			sprintf(line1,"");
			sprintf(line1,"%s",line);
			sprintf(line,"");
			sprintf(line,"%s%c",line1,ch); 
		}
	}
	fclose(mfile);
	mfile = fopen(name,"w");
	rewind(stream);
	while(!feof(stream)){
		sprintf(line,"");
		fgets( line, sizeof(line), stream); 
		fprintf(mfile,"%s",line);
	}
	fclose(mfile);
	fclose(stream);
}


//****************************************************************//
//                 Get Space Group Name and XYZ from Number       //
//****************************************************************//
int get_group_name_and_XYZ()
{
	FILE *fptr; 
	char line[40];
	int tmp0,tmp1,tmp2,tmp3,tmp4;
	char ch0[3],ch1[5],ch2[5],ch3[5];
	char par1[10];
	char par2[10];
	char par3[10];
	int i,j;
	int Out;
	char path[256];
	int setID;
	char seps[]   = " ,\t\n";
	char *token;


	if ((cCell.SpGrN >=16) && (cCell.SpGrN <=74)){
		if (cCell.Setting > 6) setID = 2;
		else setID = 1;
	}
	else setID = cCell.Setting;

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,SPGRA);

	Out = 0;
	fptr = fopen(path,"r");
	if( fptr == NULL ) {printf( "The file (%s) was not opened\n",SPGRA );getchar();exit(0);}
	else fclose(fptr);
	fptr = fopen(path,"r");
//	nPos = 0;
    while( !feof( fptr ) ){
		sprintf(line,"");
		sprintf(ch0,"");
		sprintf(ch1,"");
		sprintf(ch2,"");
		sprintf(ch3,"");
		sprintf(par1,"");
		sprintf(par2,"");
		sprintf(par3,"");
		fgets( line, sizeof(line), fptr); 
		sscanf(line,"%3i%3i%3i%3i%3i%s %s %s %s",&tmp0,&tmp1,&tmp2,&tmp3,&tmp4,
					                             &ch0,&ch1,&ch2,&ch3);
		skipLines(fptr, 1);
		if ((tmp0 == cCell.SpGrN) && (tmp1 == setID)){
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
			cCell.lType = ch0[0];
			Out = tmp3;
			if (nPos == 0){
				for(i=0; i<tmp4; i++){
					fgets( line, 80, fptr); 
					sprintf(sOperation[i].symm1,"");
					sprintf(sOperation[i].symm2,"");
					sprintf(sOperation[i].symm3,"");
					token = strtok( line, seps );
					j = 0;
					while( token != NULL ) {
						switch(j){
						case(0):{sprintf(sOperation[i].symm1,"%s",token);break;} 
						case(1):{sprintf(sOperation[i].symm2,"%s",token);break;}
						case(2):{sprintf(sOperation[i].symm3,"%s",token);break;} 
						}
						token = strtok( NULL, seps );
						j++;
					}
				}
				nPos = tmp4;
			}
			break;
		}
		skipLines(fptr, tmp4);
	}
	fclose(fptr);
	return(Out);
}


//****************************************************************//
//   Get Space Group Number for non-standart Orthorhombic Group   //
//****************************************************************//
int get_non_standart_ortho()
{
	FILE *fptr;
	char *param;
	int i,I1,I2;
	char n1[10],n2[10],n3[10],n4[10],n5[10],n6[10],name[10];
	char path[256];

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,SPGRO);

	param = "";
	sprintf(name,"");
	param = cCell.SpGr;
	I2 = strlen(param);
	strncpy(name," ",I2);
	I1 = 0;
	for(i=0; i<I2; i++){
		if (param[0] == ' ') param++; 
		else {
			strncpy(name + I1,param,1);
			param++;
			I1++;
		}
	}
//---- Load Additional Space Groups Information for Orthorhomic Space Groups
	fptr = fopen(path,"r");
	for(i=0; i<59; i++){
		sprintf(n1,""); sprintf(n2,""); sprintf(n3,"");
		sprintf(n4,""); sprintf(n5,""); sprintf(n6,"");
		fscanf(fptr,"%i %s %s %s %s %s %s\n",&I1,&n1,&n2,&n3,&n4,&n5,&n6);
		if ((strstr(n1,name) != NULL) && (strlen(n1) == strlen(name))){
			cCell.lType = n1[0]; cCell.SpGrN = I1; 
			cCell.Setting = 1 + origID*6;
			fclose(fptr);return(0);
		}
		if ((strstr(n2,name) != NULL) && (strlen(n2) == strlen(name))){
			cCell.lType = n1[0]; cCell.SpGrN = I1; 
			cCell.Setting = 2 + origID*6;
			fclose(fptr);return(0);
		}
		if ((strstr(n3,name) != NULL) && (strlen(n3) == strlen(name))){
			cCell.lType = n1[0]; cCell.SpGrN = I1; 
			cCell.Setting = 3 + origID*6;
			fclose(fptr);return(0);
		}
		if ((strstr(n4,name) != NULL) && (strlen(n4) == strlen(name))){
			cCell.lType = n1[0]; cCell.SpGrN = I1; 
			cCell.Setting = 4 + origID*6;
			fclose(fptr);return(0);
		}
		if ((strstr(n5,name) != NULL) && (strlen(n5) == strlen(name))){
			cCell.lType = n1[0]; cCell.SpGrN = I1; 
			cCell.Setting = 5 + origID*6;
			fclose(fptr);return(0);
		}
		if ((strstr(n6,name) != NULL) && (strlen(n6) == strlen(name))){
			cCell.lType = n1[0]; cCell.SpGrN = I1; 
			cCell.Setting = 6 + origID*6;
			fclose(fptr);return(0);
		}
	}
	fclose(fptr);
	return(-1);
}

//****************************************************************//
//                 Get Space Group Number and XYZ from Name       //
//****************************************************************//
int get_group_number_and_XYZ()
{
	FILE *fptr; 
	char line[40];
	int tmp0,tmp1,tmp2,tmp3,tmp4;
	char ch0[3],ch1[5],ch2[5],ch3[5],ch4[20];
	char par1[10];
	char par2[10];
	char par3[10];
	int i,j;
	int lineID;
	char path[256];
	char seps[]   = " ,\t\n";
	char *token;

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,SPGRA);

	fptr = fopen(path,"r");
    while( !feof( fptr ) ){
		sprintf(line,"");
		sprintf(ch0,"");
		sprintf(ch1,"");
		sprintf(ch2,"");
		sprintf(ch3,"");
		sprintf(par1,"");
		sprintf(par2,"");
		sprintf(par3,"");
		fgets( line, sizeof(line), fptr); 
		sscanf(line,"%3i%3i%3i%3i%3i%s %s %s %s",&tmp0,&tmp1,&tmp2,&tmp3,&tmp4,
					                             &ch0,&ch1,&ch2,&ch3);
		skipLines(fptr, 1);
		sprintf(ch4,"%s %s %s %s",ch0,ch1,ch2,ch3);
		if ((strstr(ch4,cCell.SpGr) != NULL) && (tmp1 == cCell.Setting)){
			lineID = 1;
			if ( (tmp0 == 5) || (tmp0 == 7) || (tmp0 == 8) || (tmp0 == 9) || (tmp0 == 12) ||
				(tmp0 == 13) || (tmp0 == 14) || (tmp0 == 15)) {
				if ((cCell.a[3] == 90) && (cCell.a[4] != 90) && (cCell.a[5] == 90)) lineID = 1;
				if ((cCell.a[3] == 90) && (cCell.a[4] == 90) && (cCell.a[5] != 90)) lineID = 4;
			}
			if (tmp1 >= lineID) {
				cCell.SpGrN = tmp0;
				cCell.Setting = tmp1;
				if (nPos == 0){
					for(i=0; i<tmp4; i++){
						fgets( line, 80, mfile); 
						sprintf(sOperation[i].symm1,"");
						sprintf(sOperation[i].symm2,"");
						sprintf(sOperation[i].symm3,"");
						token = strtok( line, seps );
						j = 0;
						while( token != NULL ) {
							switch(j){
							case(0):{sprintf(sOperation[i].symm1,"%s",token);break;} 
							case(1):{sprintf(sOperation[i].symm2,"%s",token);break;}
							case(2):{sprintf(sOperation[i].symm3,"%s",token);break;} 
							}
							token = strtok( NULL, seps );
							j++;
						}
					}
					nPos = tmp4;
				}
				break;
			}
		}
		skipLines(fptr, tmp4);
	}
	fclose(fptr);
	return(tmp3);
}



//****************************************************************//
//                 Add Lattice Translations to Matrix             //
//****************************************************************//
void Check_Lattice_Type()
{
	int i,j,k;
	switch(cCell.lType)
	{
	case('I'):{
		for(i=0; i<nPos; i++){
			for(j=0; j<3; j++){
				for(k=0; k<3; k++) sPos[i+nPos].a[j][k] = sPos[i].a[j][k];
				sPos[i+nPos].t[j] = sPos[i].t[j] + 0.5f;
			}
		}
		nPos *= 2;
		break;
			  }
	case('F'):{
		for(i=0; i<nPos; i++){
			for(j=0; j<3; j++){
				for(k=0; k<3; k++) {
					sPos[i+1*nPos].a[j][k] = sPos[i].a[j][k];
					sPos[i+2*nPos].a[j][k] = sPos[i].a[j][k];
					sPos[i+3*nPos].a[j][k] = sPos[i].a[j][k];
				}
			}
			sPos[i+1*nPos].t[0] = sPos[i].t[0];
			sPos[i+1*nPos].t[1] = sPos[i].t[1] + 0.5f;
			sPos[i+1*nPos].t[2] = sPos[i].t[2] + 0.5f;
			sPos[i+2*nPos].t[0] = sPos[i].t[0] + 0.5f;
			sPos[i+2*nPos].t[1] = sPos[i].t[1];
			sPos[i+2*nPos].t[2] = sPos[i].t[2] + 0.5f;
			sPos[i+3*nPos].t[0] = sPos[i].t[0] + 0.5f;
			sPos[i+3*nPos].t[1] = sPos[i].t[1] + 0.5f;
			sPos[i+3*nPos].t[2] = sPos[i].t[2];
		}
		nPos *= 4;
		break;
			  }
	case('C'):{
		for(i=0; i<nPos; i++){
			for(j=0; j<3; j++){
				for(k=0; k<3; k++) sPos[i+nPos].a[j][k] = sPos[i].a[j][k];
			}
		sPos[i+1*nPos].t[0] = sPos[i].t[0] + 0.5f;
		sPos[i+1*nPos].t[1] = sPos[i].t[1] + 0.5f;
		sPos[i+1*nPos].t[2] = sPos[i].t[2];
		}
		nPos *= 2;
		break;
			  }
	case('B'):{
		for(i=0; i<nPos; i++){
			for(j=0; j<3; j++){
				for(k=0; k<3; k++) sPos[i+nPos].a[j][k] = sPos[i].a[j][k];
			}
		sPos[i+1*nPos].t[0] = sPos[i].t[0] + 0.5f;
		sPos[i+1*nPos].t[1] = sPos[i].t[1];
		sPos[i+1*nPos].t[2] = sPos[i].t[2] + 0.5f;
		}
		nPos *= 2;
		break;
			  }
	case('A'):{
		for(i=0; i<nPos; i++){
			for(j=0; j<3; j++){
				for(k=0; k<3; k++) sPos[i+nPos].a[j][k] = sPos[i].a[j][k];
			}
		sPos[i+1*nPos].t[0] = sPos[i].t[0];
		sPos[i+1*nPos].t[1] = sPos[i].t[1] + 0.5f;
		sPos[i+1*nPos].t[2] = sPos[i].t[2] + 0.5f;
		}
		nPos *= 2;
		break;
			  }
	case('R'):{
		if (cCell.a[5] == 120){
			for(i=0; i<nPos; i++){
				for(j=0; j<3; j++){
					for(k=0; k<3; k++) {
						sPos[i+1*nPos].a[j][k] = sPos[i].a[j][k];
						sPos[i+2*nPos].a[j][k] = sPos[i].a[j][k];
					}
				}
				sPos[i+1*nPos].t[0] = sPos[i].t[0] + 2.0f/3.0f;
				sPos[i+1*nPos].t[1] = sPos[i].t[1] + 1.0f/3.0f;
				sPos[i+1*nPos].t[2] = sPos[i].t[2] + 1.0f/3.0f;
				sPos[i+2*nPos].t[0] = sPos[i].t[0] + 1.0f/3.0f;
				sPos[i+2*nPos].t[1] = sPos[i].t[1] + 2.0f/3.0f;
				sPos[i+2*nPos].t[2] = sPos[i].t[2] + 2.0f/3.0f;
			}
			nPos *= 3;
		}
		break;
			  }
	}
}

//****************************************************************//
//                 Convert (xyz) symmetry operations to Matrix    //
//****************************************************************//
void XYZ_to_Matrix(int ID)
{
	int i,j,k;
	char par1[10];
	char par2[10];
	char par3[10];
	int start;
	for(i=0;i<nPos; i++) {
//------------------------------------ Clear Matric
		for (j=0; j<3; j++){
			for (k=0;k<3;k++) sPos[i].a[j][k] = 0;
			sPos[i].t[j] = 0;
		}
		sprintf(par1,"%s",sOperation[i].symm1);
		sprintf(par2,"%s",sOperation[i].symm2);
		sprintf(par3,"%s",sOperation[i].symm3);
//------------------------------------ (X)
		start = 0;
		if (par1[0]=='-') start = 1;
		if (par1[1]=='/') start = 3;
		if (start == 3){
			if ((par1[start] == '+') && (par1[start+1] == 'x'))   sPos[i].a[0][0] =  1;
			if ((par1[start] == '+') && (par1[start+1] == 'y'))   sPos[i].a[1][0] =  1;
			if ((par1[start] == '+') && (par1[start+1] == 'z'))   sPos[i].a[2][0] =  1;
			if ((par1[start] == '-') && (par1[start+1] == 'x'))   sPos[i].a[0][0] = -1;
			if ((par1[start] == '-') && (par1[start+1] == 'y'))   sPos[i].a[1][0] = -1;
			if ((par1[start] == '-') && (par1[start+1] == 'z'))   sPos[i].a[2][0] = -1;
			if ((par1[start+2] == '+') && (par1[start+3] == 'x')) sPos[i].a[0][0] =  1;
			if ((par1[start+2] == '+') && (par1[start+3] == 'y')) sPos[i].a[1][0] =  1;
			if ((par1[start+2] == '-') && (par1[start+3] == 'x')) sPos[i].a[0][0] = -1;
			if ((par1[start+2] == '-') && (par1[start+3] == 'y')) sPos[i].a[1][0] = -1;
		}
		else {
			if (start == 0){
				if (par1[start] == 'x') sPos[i].a[0][0] =  1;
				if (par1[start] == 'y') sPos[i].a[1][0] =  1;
				if (par1[start] == 'z') sPos[i].a[2][0] =  1;
			}
			else {
				if (par1[start] == 'x') sPos[i].a[0][0] = -1;
				if (par1[start] == 'y') sPos[i].a[1][0] = -1;
				if (par1[start] == 'z') sPos[i].a[2][0] = -1;
			}
			if ((par1[start+1] == '+') && (par1[start+2] == 'x')) sPos[i].a[0][0] =  1;
			if ((par1[start+1] == '+') && (par1[start+2] == 'y')) sPos[i].a[1][0] =  1;
			if ((par1[start+1] == '-') && (par1[start+2] == 'x')) sPos[i].a[0][0] = -1;
			if ((par1[start+1] == '-') && (par1[start+2] == 'y')) sPos[i].a[1][0] = -1;

		}
		if (strstr( par1, "1/2") != NULL) sPos[i].t[0] = 0.5f;
		if (strstr( par1, "1/3") != NULL) sPos[i].t[0] = 1.0f/3.0f;
		if (strstr( par1, "2/3") != NULL) sPos[i].t[0] = 2.0f/3.0f;
		if (strstr( par1, "1/4") != NULL) sPos[i].t[0] = 0.25f;
		if (strstr( par1, "3/4") != NULL) sPos[i].t[0] = 0.75f;
		if (strstr( par1, "1/6") != NULL) sPos[i].t[0] = 1.0f/6.0f;
		if (strstr( par1, "5/6") != NULL) sPos[i].t[0] = 5.0f/6.0f;
//------------------------------------ (Y)
		start = 0;
		if (par2[0]=='-') start = 1;
		if (par2[1]=='/') start = 3;
		if (start == 3){
			if ((par2[start] == '+') && (par2[start+1] == 'x'))   sPos[i].a[0][1] =  1;
			if ((par2[start] == '+') && (par2[start+1] == 'y'))   sPos[i].a[1][1] =  1;
			if ((par2[start] == '+') && (par2[start+1] == 'z'))   sPos[i].a[2][1] =  1;
			if ((par2[start] == '-') && (par2[start+1] == 'x'))   sPos[i].a[0][1] = -1;
			if ((par2[start] == '-') && (par2[start+1] == 'y'))   sPos[i].a[1][1] = -1;
			if ((par2[start] == '-') && (par2[start+1] == 'z'))   sPos[i].a[2][1] = -1;
			if ((par2[start+2] == '+') && (par2[start+3] == 'x')) sPos[i].a[0][1] =  1;
			if ((par2[start+2] == '+') && (par2[start+3] == 'y')) sPos[i].a[1][1] =  1;
			if ((par2[start+2] == '-') && (par2[start+3] == 'x')) sPos[i].a[0][1] = -1;
			if ((par2[start+2] == '-') && (par2[start+3] == 'y')) sPos[i].a[1][1] = -1;
		}
		else {
			if (start == 0){
				if (par2[start] == 'x') sPos[i].a[0][1] =  1;
				if (par2[start] == 'y') sPos[i].a[1][1] =  1;
				if (par2[start] == 'z') sPos[i].a[2][1] =  1;
			}
			else {
				if (par2[start] == 'x') sPos[i].a[0][1] = -1;
				if (par2[start] == 'y') sPos[i].a[1][1] = -1;
				if (par2[start] == 'z') sPos[i].a[2][1] = -1;
			}
			if ((par2[start+1] == '+') && (par2[start+2] == 'x')) sPos[i].a[0][1] =  1;
			if ((par2[start+1] == '+') && (par2[start+2] == 'y')) sPos[i].a[1][1] =  1;
			if ((par2[start+1] == '-') && (par2[start+2] == 'x')) sPos[i].a[0][1] = -1;
			if ((par2[start+1] == '-') && (par2[start+2] == 'y')) sPos[i].a[1][1] = -1;
		}
		if (strstr( par2, "1/2") != NULL) sPos[i].t[1] = 0.5f;
		if (strstr( par2, "1/3") != NULL) sPos[i].t[1] = 1.0f/3.0f;
		if (strstr( par2, "2/3") != NULL) sPos[i].t[1] = 2.0f/3.0f;
		if (strstr( par2, "1/4") != NULL) sPos[i].t[1] = 0.25f;
		if (strstr( par2, "3/4") != NULL) sPos[i].t[1] = 0.75f;
		if (strstr( par2, "1/6") != NULL) sPos[i].t[1] = 1.0f/6.0f;
		if (strstr( par2, "5/6") != NULL) sPos[i].t[1] = 5.0f/6.0f;
//------------------------------------ (Z)
		start = 0;
		if (par3[0]=='-') start = 1;
		if (par3[1]=='/') start = 3;
		if (start == 3){
			if ((par3[start] == '+') && (par3[start+1] == 'x'))   sPos[i].a[0][2] =  1;
			if ((par3[start] == '+') && (par3[start+1] == 'y'))   sPos[i].a[1][2] =  1;
			if ((par3[start] == '+') && (par3[start+1] == 'z'))   sPos[i].a[2][2] =  1;
			if ((par3[start] == '-') && (par3[start+1] == 'x'))   sPos[i].a[0][2] = -1;
			if ((par3[start] == '-') && (par3[start+1] == 'y'))   sPos[i].a[1][2] = -1;
			if ((par3[start] == '-') && (par3[start+1] == 'z'))   sPos[i].a[2][2] = -1;
			if ((par3[start+2] == '+') && (par3[start+3] == 'x')) sPos[i].a[0][2] =  1;
			if ((par3[start+2] == '+') && (par3[start+3] == 'y')) sPos[i].a[1][2] =  1;
			if ((par3[start+2] == '-') && (par3[start+3] == 'x')) sPos[i].a[0][2] = -1;
			if ((par3[start+2] == '-') && (par3[start+3] == 'y')) sPos[i].a[1][2] = -1;
		}
		else {
			if (start == 0){
				if (par3[start] == 'x') sPos[i].a[0][2] =  1;
				if (par3[start] == 'y') sPos[i].a[1][2] =  1;
				if (par3[start] == 'z') sPos[i].a[2][2] =  1;
			}
			else {
				if (par3[start] == 'x') sPos[i].a[0][2] = -1;
				if (par3[start] == 'y') sPos[i].a[1][2] = -1;
				if (par3[start] == 'z') sPos[i].a[2][2] = -1;
			}
			if ((par3[start+1] == '+') && (par3[start+2] == 'x')) sPos[i].a[0][2] =  1;
			if ((par3[start+1] == '+') && (par3[start+2] == 'y')) sPos[i].a[1][2] =  1;
			if ((par3[start+1] == '-') && (par3[start+2] == 'x')) sPos[i].a[0][2] = -1;
			if ((par3[start+1] == '-') && (par3[start+2] == 'y')) sPos[i].a[1][2] = -1;
		}
		if (strstr( par3, "1/2") != NULL) sPos[i].t[2] = 0.5f;
		if (strstr( par3, "1/3") != NULL) sPos[i].t[2] = 1.0f/3.0f;
		if (strstr( par3, "2/3") != NULL) sPos[i].t[2] = 2.0f/3.0f;
		if (strstr( par3, "1/4") != NULL) sPos[i].t[2] = 0.25f;
		if (strstr( par3, "3/4") != NULL) sPos[i].t[2] = 0.75f;
		if (strstr( par3, "1/6") != NULL) sPos[i].t[2] = 1.0f/6.0f;
		if (strstr( par3, "5/6") != NULL) sPos[i].t[2] = 5.0f/6.0f;
	}
	if (ID == 1){
		for(i=0; i<nPos; i++){
			for(j=0; j<3; j++){
				for(k=0; k<3; k++){
					sPos[i+nPos].a[j][k] = -sPos[i].a[j][k];
				}
				if (sPos[i].t[j] == 0) sPos[i+nPos].t[j] = sPos[i].t[j];
				else sPos[i+nPos].t[j] = 1 - sPos[i].t[j];
			}
		}
		nPos *= 2;
	}
}


//****************************************************************//
//                 Update Symmetry                                //
//****************************************************************//
void update_symmetry()
{
	int ID;

	nPos = 0;
	ID = get_group_name_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();
}


//****************************************************************//
//                 Read CrystalMaker ASCII File                   //
//****************************************************************//
int readCMfile(char *fname)
{
	char cmd[5];
	char line[80];
	char *pos;
	int lineID = 0;
	int eAtom = 0;
	char ch0[2],ch1[5],ch2[5],ch3[5];
	int ID;
	int i,j;

    mfile=fopen(fname,"r");
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
//----------------------------------------------------------------- read Structure Data
		pos = strstr( line, "ATOM");
		if( pos != NULL ) {
			controlID = 1;
			fgets( line, 80, mfile); 
			lineID = checkEmptyLine(line);
		}
//----------------------------------------------------------------- read Background Colour
		pos = strstr( line, "BKCL");
		if( pos != NULL ) {controlID = 0; sscanf(line,"%s %f %f %f", &cmd,&BKCL[0],&BKCL[1],&BKCL[2]);}
//----------------------------------------------------------------- read max. distance bond specification
		pos = strstr( line, "BMAX");
		if(bond_sNum > MAX_BONDS_SPEC) {
			printf("======================================================\n");
			printf("Number of Bond Specifications out of range\n bond_sNum=%i\n MAX_BONDS_SPEC=%i\n",
				   bond_sNum,MAX_BONDS_SPEC);
			printf("======================================================\n");
			fclose(mfile);
			return(-1);
		}
		if( pos != NULL ) {
			controlID = 0; 
			sscanf(line,"%s %2s %2s %f", &cmd,&bini[bond_sNum].A1,&bini[bond_sNum].A2,&bini[bond_sNum].d);
			bini[bond_sNum].search_mode = 0;
			bini[bond_sNum].show_poly_mode = 1;
			bini[bond_sNum].boundary_mode = 1;
			bond_sNum++;
		}
//----------------------------------------------------------------- Bond Colour
		pos = strstr( line, "BNCL");
		if( pos != NULL ) {controlID = 0;  sscanf(line,"%s %f %f %f", &cmd,&BNCL[0],&BNCL[1],&BNCL[2]);}
//----------------------------------------------------------------- Bond Radii
		pos = strstr( line, "BRAD");
		if( pos != NULL ) {controlID = 0;  sscanf(line,"%s %f %f", &cmd,&bRadii[0],&bRadii[1]);}
//----------------------------------------------------------------- Unit Cell Constants
		pos = strstr( line, "CELL");
		if( pos != NULL ) {
			controlID = 0;  
			sscanf(line,"%s %f %f %f %f %f %f", &cmd,&cCell.a[0],
			             &cCell.a[1],&cCell.a[2],&cCell.a[3],&cCell.a[4],&cCell.a[5]);
		}
//----------------------------------------------------------------- Errors in Cell Constants
		pos = strstr( line, "ERRC");
		if( pos != NULL ) {controlID = 0;  sscanf(line,"%s %f %f %f %f %f %f", &cmd,&cCell.ea[0],
			                          &cCell.ea[1],&cCell.ea[2],&cCell.ea[3],&cCell.ea[4],&cCell.ea[5]);}
//----------------------------------------------------------------- Errors in Structure Data
		pos = strstr( line, "ERRS");
		if( pos != NULL ) {
			controlID = 2;
			fgets( line, 80, mfile); 
			lineID = checkEmptyLine(line);
		}
//----------------------------------------------------------------- Lattice Type
		pos = strstr( line, "LATC");
		if( pos != NULL ) {controlID = 0; sscanf(line,"%6s%s",&cmd,&cCell.lType);}
//----------------------------------------------------------------- Model_Type
		pos = strstr( line, "MODL");
		if( pos != NULL ) {controlID = 0; sscanf(line,"%s %i",&cmd,&MODL);}
//---------------------------------------------------------------- Show Unit Cell Outline
		pos = strstr( line, "SHCL");
		if( pos != NULL ) {controlID = 0; sscanf(line,"%s %i",&cmd,&SHCL);}
//----------------------------------------------------------------- Space Group Symbol
		pos = strstr( line, "SPGR");
		if( pos != NULL ) {
			sprintf(ch1,"");
			sprintf(ch2,"");
			sprintf(ch3,"");
			controlID = 0; sscanf(line,"%s %s %s %s %s",&cmd,&ch0,&ch1,&ch2,&ch3); 
			cCell.lType = ch0[0];
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
		}
//----------------------------------------------------------------- Symmetry Operators
		pos = strstr( line, "SYMM");
		if( pos != NULL ) {
			if(nPos > MAX_SYMM_POS) {
				printf("======================================================\n");
				printf("Number of Symmetry Operation out of range\n nPos=%i\n MAX_SYMM_POS=%i\n",
					   nPos,MAX_SYMM_POS);
				printf("======================================================\n");
				fclose(mfile);
				return(-1);
			}
			controlID = 0; sscanf(line,"%6s %s %s %s",&cmd,&sOperation[nPos].symm1,
				                          &sOperation[nPos].symm2,&sOperation[nPos].symm3); 
			nPos++;
		}
//----------------------------------------------------------------- Title Line
		pos = strstr( line, "TITL");
		if( pos != NULL ){controlID = 0; sscanf(line,"%s%75c", cmd, Title);}
//----------------------------------------------------------------- Atom Types
		pos = strstr( line, "TYPE");
		if( pos != NULL ) {
			controlID = 3;
			fgets( line, 80, mfile); 
			lineID = checkEmptyLine(line);
		}
//----------------------------------------------------------------- x,y,z Range
		pos = strstr( line, "XYZR");
		if( pos != NULL ){controlID = 0; sscanf(line,"%6s %f %f %f %f %f %f", &cmd, &range[0],
			                             &range[1],&range[2],&range[3],&range[4],&range[5]);}
//----------------------------------------------------------------- 
//----------------------------------------------------------------- ControlID
		if (lineID != 1){
			switch(controlID)
			{
			case(1):{                                        //---- for Structure Data (Symbol, Label, x,y,z)
				if (nAtom == MAX_ATOMS_IN_ASYMM) {
					printf("======================================================\n");
					printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
						    nAtom,MAX_ATOMS_IN_ASYMM);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sprintf(lAtom[nAtom].simb,"");
				sprintf(lAtom[nAtom].name,"");
				sscanf(line,"%2s %6s %f %f %f",&lAtom[nAtom].simb,&lAtom[nAtom].name,
					           &lAtom[nAtom].x,&lAtom[nAtom].y,&lAtom[nAtom].z);
				lAtom[nAtom].ex = lAtom[nAtom].ey = lAtom[nAtom].ez = 0;
				lAtom[nAtom].R = 0;
				lAtom[nAtom].g = 1;
				lAtom[nAtom].color[0] = lAtom[nAtom].color[1] = lAtom[nAtom].color[2] = 0;
				nAtom++;
				break;
					}
			case(2):{                                        //---- for Errors in Structure Data (Label, ex,ey,ez)
				sscanf(line,"%7s %f %f %f",&lAtom[eAtom].name,
					           &lAtom[eAtom].ex,&lAtom[eAtom].ey,&lAtom[eAtom].ez);
				eAtom++;
				break;
					}
			case(3):{                                        //---- for Atom Types (Symbol, Radius,R,G,B)
				sscanf(line,"%3s %f %f %f %f",&AtomT[tAtom].symb,&AtomT[tAtom].R,
					           &AtomT[tAtom].color[0],&AtomT[tAtom].color[1],&AtomT[tAtom].color[2]);
				tAtom++;
				break;
					}
			}
		}
//----------------------------------------------------------------- 
	}
	fclose(mfile);
	for(i=0; i<nAtom; i++){
		lAtom[i].B[0] = lAtom[i].B[1] = lAtom[i].B[2] = 0;
		lAtom[i].B[3] = lAtom[i].B[4] = lAtom[i].B[5] = 0;
		lAtom[i].Biso = 0.0;
		for(j=0; j<tAtom; j++){
			if (strstr( lAtom[i].simb, AtomT[j].symb) != NULL){
				lAtom[i].R = AtomT[j].R;
				lAtom[i].color[0] = AtomT[j].color[0];
				lAtom[i].color[1] = AtomT[j].color[1];
				lAtom[i].color[2] = AtomT[j].color[2];
				break;
			}
		}
	}
	ID = 0;
	if (nPos == 0) ID = get_group_number_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();
	return 0;
}


//****************************************************************//
//                 Read ICSD ASCII File                           //
//****************************************************************//

void check_extra_symbols()
{
	char *pos;
	char S;
	char SpGr[20];
	int code,I1;

	pos = "";
	sprintf(SpGr,"");
	pos = cCell.SpGr;
	S = pos[0];
	pos++;
	sprintf(SpGr,"%s",pos);
	if (strstr(pos,"H") != NULL) code = 1;
	if (strstr(pos,"HR") != NULL) code = 1;
	if (strstr(pos,"R") != NULL) code = 2;
	if ((pos[1] == '1') && (pos[3] == '1')) code = 3;
	if ((pos[1] == '1') && (pos[3] != '1')) code = 4;
	if (strstr(pos,"Z") != NULL) code = 5;
	switch(code){
	case(2):
	case(1):{
		cCell.Setting = code;
		I1 = strcspn(pos,"H");
		sprintf(cCell.SpGr,"");
		sprintf(cCell.SpGr,"%c",S);
		strncat(cCell.SpGr,SpGr, I1-1);
		break;
			}
	case(3):{
		pos = pos + 4;
		sprintf(cCell.SpGr,"");
		sprintf(cCell.SpGr,"%c%s",S,pos);
		break;
			}
	case(4):{
		pos = pos + 2;
		sprintf(cCell.SpGr,"");
		sprintf(cCell.SpGr,"%c%s",S,pos);
		break;
			}
	case(5):{
		I1 = strcspn(pos,"H");
		sprintf(cCell.SpGr,"");
		sprintf(cCell.SpGr,"%c",S);
		strncat(cCell.SpGr,SpGr, I1-1);
		if ((cCell.SpGrN >= 16) && (cCell.SpGrN <= 74)) origID = 1;
		if ((cCell.SpGrN>= 75) && (cCell.SpGrN<=142)) cCell.Setting = 2;
		if ((cCell.SpGrN>= 195) && (cCell.SpGrN<=230)) cCell.Setting = 2;
		break;
			}
	}
}

int readICSDfile_old(char *fname)
{
	char cmd[5];
	int lineID = 0;
	int i,j,k;
	char *pos;
	char line[80];
	int I1,I2,I3;
	char ch;
	char cell_param0[15],cell_param1[15],cell_param2[15],
		 cell_param3[15],cell_param4[15],cell_param5[15];
	char ch0[2],ch1[5],ch2[5],ch3[5];
	float oxid;
	int mAtom = 0;
	int izAtom = 0;

    mfile=fopen(fname,"r");
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
		if (line[1] != ' ') controlID = 0;
//----------------------------------------------------------------- Title Line
		pos = strstr( line, "FORM");
		if( pos != NULL ){
			pos = pos + 5;
			sprintf(Title,"");
			strncat(Title,pos, strlen(pos)-1);
			controlID = 0; 
		}
//----------------------------------------------------------------- Unit Cell Constants end Errors
		pos = strstr( line, "CELL");
		if( pos != NULL ){
			controlID = 0; 
			sscanf(line,"%s %s %s %s %s %s %s", cmd,&cell_param0,&cell_param1,&cell_param2,
				                                    &cell_param3,&cell_param4,&cell_param5);
//----- a, esd(a)
			I1 = strcspn(cell_param0,"=") + 1;
			I2 = strcspn(cell_param0,"(");
			I3 = strcspn(cell_param0,".") + 1;
			if (I2>0) sscanf(cell_param0+I1,"%f%c%f%c",&cCell.a[0],&ch,&cCell.ea[0],&ch);
			else {sscanf(cell_param0+I1,"%f",&cCell.a[0]); cCell.ea[0] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[0] /= 10;}
//----- b, esd(b)
			I1 = strcspn(cell_param1,"=") + 1;
			I2 = strcspn(cell_param1,"(");
			I3 = strcspn(cell_param1,".") + 1;
			if (I2>0) sscanf(cell_param1+I1,"%f%c%f%c",&cCell.a[1],&ch,&cCell.ea[1],&ch);
			else {sscanf(cell_param1+I1,"%f",&cCell.a[1]); cCell.ea[1] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[1] /= 10;}
//----- c, esd(c)
			I1 = strcspn(cell_param2,"=") + 1;
			I2 = strcspn(cell_param2,"(");
			I3 = strcspn(cell_param2,".") + 1;
			if (I2>0) sscanf(cell_param2+I1,"%f%c%f%c",&cCell.a[2],&ch,&cCell.ea[2],&ch);
			else {sscanf(cell_param2+I1,"%f",&cCell.a[2]); cCell.ea[2] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[2] /= 10;}
//----- alpha, esd(alpha)
			I1 = strcspn(cell_param3,"=") + 1;
			I2 = strcspn(cell_param3,"(");
			I3 = strcspn(cell_param3,".") + 1;
			if (I2>0) sscanf(cell_param3+I1,"%f%c%f%c",&cCell.a[3],&ch,&cCell.ea[3],&ch);
			else {sscanf(cell_param3+I1,"%f",&cCell.a[3]); cCell.ea[3] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[3] /= 10;}
//----- beta, esd(beta)
			I1 = strcspn(cell_param4,"=") + 1;
			I2 = strcspn(cell_param4,"(");
			I3 = strcspn(cell_param4,".") + 1;
			if (I2>0) sscanf(cell_param4+I1,"%f%c%f%c",&cCell.a[4],&ch,&cCell.ea[4],&ch);
			else {sscanf(cell_param4+I1,"%f",&cCell.a[4]); cCell.ea[4] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[4] /= 10;}
//----- gamma, esd(gamma)
			I1 = strcspn(cell_param5,"=") + 1;
			I2 = strcspn(cell_param5,"(");
			I3 = strcspn(cell_param5,".") + 1;
			if (I2>0) sscanf(cell_param5+I1,"%f%c%f%c",&cCell.a[5],&ch,&cCell.ea[5],&ch);
			else {sscanf(cell_param4+I1,"%f",&cCell.a[5]); cCell.ea[5] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[5] /= 10;}
  		}
//----------------------------------------------------------------- read Structure Data and Errors
		pos = strstr( line, "PARM");
		if( pos != NULL ) {
			controlID = 1;
			k = 1;
			fgets( line, 80, mfile); 
			lineID = checkEmptyLine(line);
		}
//----------------------------------------------------------------- Thermal Parameters
		if (strstr( line, "ITF") == NULL){
			pos = strstr( line, "TF");
			if( pos != NULL ) {
				pos = strstr( line, "U(");
				if (pos != NULL) Btype = 0;
				else Btype = 1;
				controlID = 2;
				fgets( line, 80, mfile); 
				lineID = checkEmptyLine(line);
			}
		}
		else {
			pos = strstr( line, "B=");
			if (pos != NULL) {
				use_iso = 0;
				pos += 2;
				sscanf(pos,"%f",&lAtom[izAtom].Biso);
				izAtom++;
			}
			else {
				pos = strstr( line, "U=");
				if (pos != NULL) {
					use_iso = 1;
					pos += 2;
					sscanf(pos,"%f",&lAtom[izAtom].Biso);
					izAtom++;
				}
				else {
					use_iso = 0;
//					lAtom[izAtom].Biso = 1.0;
					lAtom[izAtom].Biso = 0.0;
					izAtom++;
				}
			}
		}
//----------------------------------------------------------------- Space Group Symbol
		pos = strstr( line, "SGR");
		if( pos != NULL ) {
			sprintf(ch1,"");
			sprintf(ch2,"");
			sprintf(ch3,"");
			controlID = 0; sscanf(line,"%s %s %s %s %s",&cmd,&ch0,&ch1,&ch2,&ch3); 
			cCell.lType = ch0[0];
			if (strstr( ch2, "(") != NULL){
				sprintf(ch2,"");
				sprintf(ch3,"");
			}
			if ((strlen(ch3) == 1) && (ch3[0] == '1')) sprintf(ch3,"");
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
			check_extra_symbols();
		}
//----------------------------------------------------------------- 
//----------------------------------------------------------------- ControlID
		if (lineID != 1){
			switch(controlID)
			{
			case(1):{                                        //---- for Structure Data (Symbol, Label, x,y,z)
				if (nAtom == MAX_ATOMS_IN_ASYMM) {
					printf("======================================================\n");
					printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
						    nAtom,MAX_ATOMS_IN_ASYMM);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sprintf(cell_param0,"");
				sprintf(cell_param1,"");
				sprintf(cell_param2,"");
				sprintf(cell_param3,"");
				sprintf(lAtom[nAtom].simb,"");
				sprintf(lAtom[nAtom].name,"");
				k = 1;
				lAtom[nAtom].ex = lAtom[nAtom].ey = lAtom[nAtom].ez = 0;
   			    lAtom[nAtom].R = 0;
				lAtom[nAtom].g = 1;
				lAtom[nAtom].color[0] = lAtom[nAtom].color[1] = lAtom[nAtom].color[2] = 0;

				sscanf(line,"%2s %i %f %4s %s %s %s %s", &lAtom[nAtom].simb, &j, &oxid, &ch1,
							   &cell_param0,&cell_param1,&cell_param2,&cell_param3);
//----- Name and Symbol
				sprintf(lAtom[nAtom].name,"%s%i",lAtom[nAtom].simb,j);
//----- X and esd(X)
				I1 = strcspn(cell_param0,"/");
				if (I1 == 1){
					sscanf(cell_param0,"%i%c%i",&i,&ch,&j);
					lAtom[nAtom].x = (float)i/(float)j;
					lAtom[nAtom].ex = 0;
				}
				else {
					I2 = strcspn(cell_param0,"(");
					I3 = strcspn(cell_param0,".") + 1;
					if (I2 > 0) sscanf(cell_param0,"%f%c%f%c",&lAtom[nAtom].x,&ch,&lAtom[nAtom].ex,&ch);
					else {sscanf(cell_param0,"%f",&lAtom[nAtom].x); lAtom[nAtom].ex = 0;}
					if (I2 > 0){for (j=0; j<(I2-I3); j++) lAtom[nAtom].ex /= 10;}
				}
//----- Y and esd(Y)
				I1 = strcspn(cell_param1,"/");
				if (I1 == 1){
					sscanf(cell_param1,"%i%c%i",&i,&ch,&j);
					lAtom[nAtom].y = (float)i/(float)j;
					lAtom[nAtom].ey = 0;
				}
				else {
					I2 = strcspn(cell_param1,"(");
					I3 = strcspn(cell_param1,".") + 1;
					if (I2 > 0) sscanf(cell_param1,"%f%c%f%c",&lAtom[nAtom].y,&ch,&lAtom[nAtom].ey,&ch);
					else {sscanf(cell_param1,"%f",&lAtom[nAtom].y); lAtom[nAtom].ey = 0;}
					if (I2 > 0){for (j=0; j<(I2-I3); j++) lAtom[nAtom].ey /= 10;}
				}
//----- Z and esd(Z)
				I1 = strcspn(cell_param2,"/");
				if (I1 == 1){
					sscanf(cell_param2,"%i%c%i",&i,&ch,&j);
					lAtom[nAtom].z = (float)i/(float)j;
					lAtom[nAtom].ez = 0;
				}
				else {
					I2 = strcspn(cell_param2,"("); 
					I3 = strcspn(cell_param2,".") + 1;
					if (I2>0) sscanf(cell_param2,"%f%c%f%c",&lAtom[nAtom].z,&ch,&lAtom[nAtom].ez,&ch);
					else {sscanf(cell_param2,"%f",&lAtom[nAtom].z); lAtom[nAtom].ez = 0;}
					if (I2 > 0){for (j=0; j<(I2-I3); j++) lAtom[nAtom].ez /= 10;}
				}
//----- g and esd(g)
				if (cell_param3 != NULL){
					I1 = strcspn(cell_param3,"/");
					if (I1 == 1){
						sscanf(cell_param3,"%i%c%i",&i,&ch,&j);
						lAtom[nAtom].g = (float)i/(float)j;
					}
					else sscanf(cell_param3,"%f",&lAtom[nAtom].g);
				}
//				lAtom[nAtom].Biso = 1.0;
				lAtom[nAtom].Biso = 0.0;
				nAtom++;
				break;
					}
			case(2):{
				use_aniso = 1;
				if (mAtom == MAX_ATOMS_IN_ASYMM) {
					printf("======================================================\n");
					printf("Number of Thermal Parameters out of range\n mAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
						    mAtom,MAX_ATOMS_IN_ASYMM);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sscanf(line,"%2s %i %f %f %f %f %f %f", &ch1, &j, 
					&lAtom[mAtom].B[0],&lAtom[mAtom].B[1],&lAtom[mAtom].B[2],
					&lAtom[mAtom].B[3],&lAtom[mAtom].B[4],&lAtom[mAtom].B[5]);
//				lAtom[mAtom].Biso = 1;
				lAtom[mAtom].Biso = 0.0;
				mAtom++;
				j = 1;
				while (j == 1){
					fgets( line, 80, mfile); 
					j = checkEmptyLine(line);
				}
				break;
					}
			}
		}
//----------------------------------------------------------------- 
	}
	fclose(mfile);
	if (mAtom == 0) use_aniso = 0;
	return 0;
}

int readICSDfile_new(char *fname)
{
	int i,j,k;
	int mAtom = 0;
	char line[100];
	char cmd[15],cmd1[15];
	char *pos;
	char cell_param0[15],cell_param1[15],cell_param2[15],
		 cell_param3[15],cell_param4[15],cell_param5[15];
	int I1,I2,I3;
	char ch;
	char ch0[2],ch1[5],ch2[5],ch3[5];
	int lineID = 0;
	int iso_ID;

	nAtom = 0;
	controlID = 0;
	sprintf(Title,"ICSD file");
	mfile=fopen(fname,"r");
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 100, mfile); 
		if (strstr( line, "*end") != NULL) break;
		lineID = checkEmptyLine(line);
		if ((line[0] == ' ') && (line[1] != ' ')) {
			controlID = 1;
		}
		else {
			if (controlID != 2) controlID = 0;
		}
//----------------------------------------------------------------- Title Line
		pos = strstr( line, "Structured");
		if( pos != NULL ) {
			pos = pos + 12;
			sprintf(Title,"");
			strncat(Title,pos, strlen(pos)-1);
			controlID = 0; 
//			sscanf(line,"%s%75c", &cmd, &Title);
		}

//----------------------------------------------------------------- Unit Cell Constants end Errors
		pos = strstr( line, "Unit Cell");
		if( pos != NULL ){
			controlID = 0; 
			pos = pos + 10;
			sscanf(pos,"%s %s %s %s %s %s", &cell_param0,&cell_param1,&cell_param2,
				                                    &cell_param3,&cell_param4,&cell_param5);
//----- a, esd(a)
			I2 = strcspn(cell_param0,"(");
			I3 = strcspn(cell_param0,".") + 1;
			if (I2>0) sscanf(cell_param0,"%f%c%f%c",&cCell.a[0],&ch,&cCell.ea[0],&ch);
			else {sscanf(cell_param0,"%f",&cCell.a[0]); cCell.ea[0] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[0] /= 10;}
//----- b, esd(b)
			I2 = strcspn(cell_param1,"(");
			I3 = strcspn(cell_param1,".") + 1;
			if (I2>0) sscanf(cell_param1,"%f%c%f%c",&cCell.a[1],&ch,&cCell.ea[1],&ch);
			else {sscanf(cell_param1,"%f",&cCell.a[1]); cCell.ea[1] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[1] /= 10;}
//----- c, esd(c)
			I2 = strcspn(cell_param2,"(");
			I3 = strcspn(cell_param2,".") + 1;
			if (I2>0) sscanf(cell_param2,"%f%c%f%c",&cCell.a[2],&ch,&cCell.ea[2],&ch);
			else {sscanf(cell_param2,"%f",&cCell.a[2]); cCell.ea[2] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[2] /= 10;}
//----- alpha, esd(alpha)
			I2 = strcspn(cell_param3,"(");
			I3 = strcspn(cell_param3,".") + 1;
			if (I2>0) sscanf(cell_param3,"%f%c%f%c",&cCell.a[3],&ch,&cCell.ea[3],&ch);
			else {sscanf(cell_param3,"%f",&cCell.a[3]); cCell.ea[3] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[3] /= 10;}
//----- beta, esd(beta)
			I2 = strcspn(cell_param4,"(");
			I3 = strcspn(cell_param4,".") + 1;
			if (I2>0) sscanf(cell_param4,"%f%c%f%c",&cCell.a[4],&ch,&cCell.ea[4],&ch);
			else {sscanf(cell_param4,"%f",&cCell.a[4]); cCell.ea[4] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[4] /= 10;}
//----- gamma, esd(gamma)
			I2 = strcspn(cell_param5,"(");
			I3 = strcspn(cell_param5,".") + 1;
			if (I2>0) sscanf(cell_param5,"%f%c%f%c",&cCell.a[5],&ch,&cCell.ea[5],&ch);
			else {sscanf(cell_param4,"%f",&cCell.a[5]); cCell.ea[5] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[5] /= 10;}
  		}
//----------------------------------------------------------------- Space Group Number
		pos = strstr( line, "SG Number");
		if( pos != NULL ) {
			controlID = 0; 
			pos = pos + 10;
			sscanf(pos,"%i",&cCell.SpGrN);
		}
//----------------------------------------------------------------- Space Group Symbol
		pos = strstr( line, "Space Group");
		if( pos != NULL ) {
			controlID = 0; 
			pos = pos + 12;
			sprintf(ch1,"");
			sprintf(ch2,"");
			sprintf(ch3,"");
			sscanf(pos,"%s %s %s %s",&ch0,&ch1,&ch2,&ch3); 
			cCell.lType = ch0[0];
			if (strstr( ch2, "(") != NULL){
				sprintf(ch2,"");
				sprintf(ch3,"");
			}
			if ((strlen(ch3) == 1) && (ch3[0] == '1')) sprintf(ch3,"");
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
			check_extra_symbols();
		}
//----------------------------------------------------------------- read Structure Data and Errors
		pos = strstr( line, "Atom");
		if( pos != NULL ) {
			Btype = -1;
			iso_ID = 0;
			if (strstr(line,"ITF(U)") != NULL) {Btype = 0;iso_ID = 1;use_iso = 1;}
			if (strstr(line,"ITF(B)") != NULL) {Btype = 1;iso_ID = 1;use_iso = 0;}
			controlID = 1;
			fgets( line, 100, mfile); 
			lineID = checkEmptyLine(line);
		}
//----------------------------------------------------------------- Thermal Parameters
		if (strstr( line, "Lbl") != NULL){
			if (strstr(line,"U11") !=NULL) Btype = 0;
			else Btype = 1;
			controlID = 2;
			fgets( line, 100, mfile); 
			lineID = checkEmptyLine(line);
		}
//----------------------------------------------------------------- ControlID
		if (lineID != 1){
			switch(controlID)
			{
			case(1):{                                        //---- for Structure Data (Symbol, Label, x,y,z)
				if (nAtom == MAX_ATOMS_IN_ASYMM) {
					printf("======================================================\n");
					printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
						    nAtom,MAX_ATOMS_IN_ASYMM);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sprintf(cell_param0,"");
				sprintf(cell_param1,"");
				sprintf(cell_param2,"");
				sprintf(cell_param3,"");
				sprintf(lAtom[nAtom].simb,"");
				sprintf(lAtom[nAtom].name,"");
				lAtom[nAtom].ex = lAtom[nAtom].ey = lAtom[nAtom].ez = 0;
   			    lAtom[nAtom].R = 0;
				lAtom[nAtom].g = 1;
				lAtom[nAtom].color[0] = lAtom[nAtom].color[1] = lAtom[nAtom].color[2] = 0;
				if (iso_ID == 1) {
					sscanf(line,"%2s %i %s %i %c %s %s %s %s %i %s", &lAtom[nAtom].simb, &j,&cmd,&i,&ch,
					       &cell_param0,&cell_param1,&cell_param2,&cell_param3,&k,&cell_param4);
				}
				else {
					sscanf(line,"%2s %i %s %i %c %s %s %s %s %i", &lAtom[nAtom].simb, &j,&cmd,&i,&ch,
					       &cell_param0,&cell_param1,&cell_param2,&cell_param3,&k);
				}
//----- Name and Symbol
				sprintf(lAtom[nAtom].name,"%s%i",lAtom[nAtom].simb,j);
//----- X and esd(X)
				I1 = strcspn(cell_param0,"/");
				if (I1 == 1){
					sscanf(cell_param0,"%i%c%i",&i,&ch,&j);
					lAtom[nAtom].x = (float)i/(float)j;
					lAtom[nAtom].ex = 0;
				}
				else {
					I2 = strcspn(cell_param0,"(");
					I3 = strcspn(cell_param0,".") + 1;
					if (I2 > 0) sscanf(cell_param0,"%f%c%f%c",&lAtom[nAtom].x,&ch,&lAtom[nAtom].ex,&ch);
					else {sscanf(cell_param0,"%f",&lAtom[nAtom].x); lAtom[nAtom].ex = 0;}
					if (I2 > 0){for (j=0; j<(I2-I3); j++) lAtom[nAtom].ex /= 10;}
				}
//----- Y and esd(Y)
				I1 = strcspn(cell_param1,"/");
				if (I1 == 1){
					sscanf(cell_param1,"%i%c%i",&i,&ch,&j);
					lAtom[nAtom].y = (float)i/(float)j;
					lAtom[nAtom].ey = 0;
				}
				else {
					I2 = strcspn(cell_param1,"(");
					I3 = strcspn(cell_param1,".") + 1;
					if (I2 > 0) sscanf(cell_param1,"%f%c%f%c",&lAtom[nAtom].y,&ch,&lAtom[nAtom].ey,&ch);
					else {sscanf(cell_param1,"%f",&lAtom[nAtom].y); lAtom[nAtom].ey = 0;}
					if (I2 > 0){for (j=0; j<(I2-I3); j++) lAtom[nAtom].ey /= 10;}
				}
//----- Z and esd(Z)
				I1 = strcspn(cell_param2,"/");
				if (I1 == 1){
					sscanf(cell_param2,"%i%c%i",&i,&ch,&j);
					lAtom[nAtom].z = (float)i/(float)j;
					lAtom[nAtom].ez = 0;
				}
				else {
					I2 = strcspn(cell_param2,"("); 
					I3 = strcspn(cell_param2,".") + 1;
					if (I2>0) sscanf(cell_param2,"%f%c%f%c",&lAtom[nAtom].z,&ch,&lAtom[nAtom].ez,&ch);
					else {sscanf(cell_param2,"%f",&lAtom[nAtom].z); lAtom[nAtom].ez = 0;}
					if (I2 > 0){for (j=0; j<(I2-I3); j++) lAtom[nAtom].ez /= 10;}
				}
//----- g and esd(g)
				if (cell_param3 != NULL){
					I1 = strcspn(cell_param3,"/");
					if (I1 == 1){
						sscanf(cell_param3,"%i%c%i",&i,&ch,&j);
						lAtom[nAtom].g = (float)i/(float)j;
					}
					else sscanf(cell_param3,"%f",&lAtom[nAtom].g);
				}
//----- B and esd(B) (or U and esd(U))
				if (iso_ID == 1) sscanf(cell_param4,"%f",&lAtom[nAtom].Biso);
				else lAtom[nAtom].Biso = 0.0f;
				nAtom++;
				break;
					}
			case(2):{
				use_aniso = 1;
				if (mAtom == MAX_ATOMS_IN_ASYMM) {
					printf("======================================================\n");
					printf("Number of Thermal Parameters out of range\n mAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
						    mAtom,MAX_ATOMS_IN_ASYMM);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sprintf(cell_param0,"");
				sprintf(cell_param1,"");
				sprintf(cell_param2,"");
				sprintf(cell_param3,"");
				sprintf(cell_param4,"");
				sprintf(cell_param5,"");
				sscanf(line,"%s %s %s %s %s %s %s %s", &cmd,&cmd1,&cell_param0,&cell_param1,
					         &cell_param2,&cell_param3,&cell_param4,&cell_param5);
				controlID = 0;
				for(i=0; i<nAtom; i++){
					pos = strstr(cmd,lAtom[i].name);
//					pos = strstr(cmd,lAtom[i].simb);
					if ((pos != NULL) && (strlen(cmd) == strlen(lAtom[i].name))){
						controlID = 2;
						sscanf(cell_param0,"%f",&lAtom[i].B[0]);
						sscanf(cell_param1,"%f",&lAtom[i].B[1]);
						sscanf(cell_param2,"%f",&lAtom[i].B[2]);
						sscanf(cell_param3,"%f",&lAtom[i].B[3]);
						sscanf(cell_param4,"%f",&lAtom[i].B[4]);
						sscanf(cell_param5,"%f",&lAtom[i].B[5]);
						lAtom[i].Biso = 0.0;
						break;
					}
				}
				mAtom++;
				break;
					}
			}
		}
	}
	fclose(mfile);
	if (mAtom == 0) use_aniso = 0;
	return 0;
}

int readICSDfile(char *fname)
{
	int i,ID,fID,rID;
	char line[80];
	char *pos;

	rID = 0;
	mfile=fopen(fname,"r");
	sprintf(line,"");
	fgets( line, 80, mfile); 
	pos = strstr( line, "*data for");
	if (pos != NULL) rID = 1;
	fclose(mfile);

	cCell.SpGrN = 0;
    cCell.Setting = 1;
	origID = 0;
	switch(rID)
	{
	case(0):{fID = readICSDfile_old(fname); break;}
	case(1):{fID = readICSDfile_new(fname); break;}
	}
	ID = 0;
	ID = get_group_number_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();
	if (nPos != 0) {
		if ((cCell.SpGrN >= 16) && (cCell.SpGrN <= 74)) {
			cCell.Setting = cCell.Setting + origID*6;
		}
	}
	if (nPos == 0){
		i = get_non_standart_ortho();
		if (i == 0) update_symmetry();
	}
    if ((nPos == 0) && (cCell.SpGrN > 0)) {
		cCell.Setting = 1;
		origID = 0;
		update_symmetry();
		return(-2);
	}
	if (nPos == 0) return -1;
	return 0;
}



//****************************************************************//
//                 Check Number of Phases (CIF File)              //
//****************************************************************//
int check_num_phase(char *fname)
{
	char line[80];
	char keyword[15];
	char *pos;

	nPhase = 0;
    mfile=fopen(fname,"r");
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 80, mfile); 
		pos = strstr( line, "_pd_phase_name");
		if( pos != NULL ) {
			sscanf(line,"%s %s",keyword,phases[nPhase].name);
			nPhase++;
		}
	}
	fclose(mfile);
	return(nPhase);
}

//****************************************************************//
//                 Read CIF ASCII File                            //
//****************************************************************//
int check_standart_group_name()
{
	int i,checkID;
	FILE *fptr; 
	char line[80];
	int tmp0,tmp1,tmp2,tmp3,tmp4;
	char ch0[15],ch1[15],ch2[15],ch3[15],ch4[20];

	char path[256];
	sprintf(path,"");
	sprintf(path,"%s%s",progvar,SPGRA);
	fptr = fopen(path,"r");

	checkID = -1;
//---------------------------------- check for standart Space Groups
//	fptr = fopen("spgra.dat","r");
    while( !feof( fptr ) ){
		sprintf(line,"");
		sprintf(ch0,"");
		sprintf(ch1,"");
		sprintf(ch2,"");
		sprintf(ch3,"");
		sprintf(ch4,"");
		fgets( line, sizeof(line), fptr); 
		sscanf(line,"%3i%3i%3i%3i%3i%s %s %s %s",&tmp0,&tmp1,&tmp2,&tmp3,&tmp4,
					                             &ch0,&ch1,&ch2,&ch3);
		fgets( line, sizeof(line), fptr); 
		sprintf(ch4,"%s%s%s%s",ch0,ch1,ch2,ch3);
		if ((strlen(ch4) == strlen(cCell.SpGr)) && (strstr(ch4,cCell.SpGr) != NULL)){
			cCell.SpGrN = tmp0;
			cCell.Setting = tmp1;
			sprintf(cCell.SpGr,"");
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
			checkID = 1;
			break;
		}
		for(i=0; i<tmp4; i++) fgets( line, sizeof(line), fptr); 
	}
	fclose(fptr);
	return(checkID);
}

int check_non_standart_group_name()
{
	int i,I1,checkID;
	FILE *fptr; 
	char n1[10],n2[10],n3[10],n4[10],n5[10],n6[10];

	char path[256];

	sprintf(path,"");
	sprintf(path,"%s%s",progvar,SPGRO);
	fptr = fopen(path,"r");

	checkID = -1;
//	fptr = fopen("spgro.dat","r");
	for(i=0; i<59; i++){
		sprintf(n1,""); sprintf(n2,""); sprintf(n3,"");
		sprintf(n4,""); sprintf(n5,""); sprintf(n6,"");
		fscanf(fptr,"%i %s %s %s %s %s %s\n",&I1,&n1,&n2,&n3,&n4,&n5,&n6);
//-----------------------------------------------------------------------------------
		if ((strstr(n1,cCell.SpGr) != NULL) && (strlen(n1) == strlen(cCell.SpGr))){
			cCell.SpGrN = I1;
			cCell.Setting = 1;
			checkID = 1;
			break;
		}
//-----------------------------------------------------------------------------------
		if ((strstr(n2,cCell.SpGr) != NULL) && (strlen(n2) == strlen(cCell.SpGr))){
			cCell.SpGrN = I1;
			cCell.Setting = 2;
			checkID = 1;
			break;
		}
//-----------------------------------------------------------------------------------
		if ((strstr(n3,cCell.SpGr) != NULL) && (strlen(n3) == strlen(cCell.SpGr))){
			cCell.SpGrN = I1;
			cCell.Setting = 3;
			checkID = 1;
			break;
		}
//-----------------------------------------------------------------------------------
		if ((strstr(n4,cCell.SpGr) != NULL) && (strlen(n4) == strlen(cCell.SpGr))){
			cCell.SpGrN = I1;
			cCell.Setting = 4;
			checkID = 1;
			break;
		}
//-----------------------------------------------------------------------------------
		if ((strstr(n5,cCell.SpGr) != NULL) && (strlen(n5) == strlen(cCell.SpGr))){
			cCell.SpGrN = I1;
			cCell.Setting = 5;
			checkID = 1;
			break;
		}
//-----------------------------------------------------------------------------------
		if ((strstr(n6,cCell.SpGr) != NULL) && (strlen(n6) == strlen(cCell.SpGr))){
			cCell.SpGrN = I1;
			cCell.Setting = 6;
			checkID = 1;
			break;
		}
	}
	fclose(fptr);
	return(checkID);
}

void check_standart_group_name_number()
{
	int i,checkID;
	FILE *fptr; 
	char line[80];
	int tmp0,tmp1,tmp2,tmp3,tmp4;
	char ch0[15],ch1[15],ch2[15],ch3[15];

	char path[256];
	sprintf(path,"");
	sprintf(path,"%s%s",progvar,SPGRA);
	fptr = fopen(path,"r");

	checkID = -1;
//---------------------------------- check for standart Space Groups
//	fptr = fopen("spgra.dat","r");
    while( !feof( fptr ) ){
		sprintf(line,"");
		sprintf(ch0,"");
		sprintf(ch1,"");
		sprintf(ch2,"");
		sprintf(ch3,"");
		fgets( line, sizeof(line), fptr); 
		sscanf(line,"%3i%3i%3i%3i%3i%s %s %s %s",&tmp0,&tmp1,&tmp2,&tmp3,&tmp4,
					                             &ch0,&ch1,&ch2,&ch3);
		fgets( line, sizeof(line), fptr); 
		if (cCell.SpGrN == tmp0){
			if (cCell.Setting == 0) cCell.Setting = tmp1;
			sprintf(cCell.SpGr,"");
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
			break;
		}
		for(i=0; i<tmp4; i++) fgets( line, sizeof(line), fptr); 
	}
	fclose(fptr);
}



int readCIFfile(char *fname, int nF)
{
	int i,j,k,l,ID;
	int line_start,line_end;
	int I1,I2,I3,I0,CIF_ID,SPGR_ID;
	char line[90],cell_param[20],keyword[35];
	char ch;
	char *pos;
	char ch0[15],ch1[15],ch2[15],ch3[15];
	int lineID = 0;
	int keykode[] = {0,0,0,0,0,0,0,0};
	int Bkeykode[] = {0,0,0,0,0,0};
	char seps[]   = " ,\t\n";
	char *token;
	int mAtom = 0;
	float B[6];
	int title_ID = 0;
	char cTitle[70];
	int lst2sifID = 0;

//--------------------------------------------- search fearst phase
    mfile=fopen(fname,"r");
	line_start = -1;
	line_end   = -1;
	I1 = 0;
	sprintf(cTitle,"");
	sprintf(Title,"");
	sprintf(Title,"CIF file");
    while( !feof( mfile ) )
	{
		I1++;
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		pos = line;
		if (pos[0] == 'd') {
			pos = "";
			pos = strstr( line, "data_");
			if (pos != NULL){
				pos = strstr( line, "_global");
				if (pos == NULL ) pos = strstr( line, "_publ");
				if (pos == NULL ) pos = strstr( line, "_default");
				if (pos == NULL){
					if (line_start > 0) line_end = I1;
					else line_start = I1;
					if ((line_start > 0) && (line_end > 0)) break;
				}
			}
		}
	}
	if (line_start == -1) line_start = 1;
	if (line_end == -1) line_end = I1;
	fclose(mfile);
//--------------------------------------------- end search fearst phase
	cCell.SpGrN = 0;
	cCell.Setting = 0;
	sprintf(cCell.SpGr,"");

    mfile=fopen(fname,"r");
	for(i=0; i<line_start; i++) {
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	I1 = line_start;
	lst2sifID = 0;
    while( !feof( mfile ) ){
		I1++;
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if (strstr(line,"_phase") != NULL) {
			if (lst2sifID == 1) break;
			else lst2sifID = 1;
		}
		CIF_ID = -1;
//------------- check line for unit cell parameters
		if (strstr( line, "_cell_length_a") != NULL)    CIF_ID = 10;
		if (strstr( line, "_cell_length_b") != NULL)    CIF_ID = 11;
		if (strstr( line, "_cell_length_c") != NULL)    CIF_ID = 12;
		if (strstr( line, "_cell_angle_alpha") != NULL) CIF_ID = 13;
		if (strstr( line, "_cell_angle_beta") != NULL)  CIF_ID = 14;
		if (strstr( line, "_cell_angle_gamma") != NULL) CIF_ID = 15;
//------------- check line for symmetry
		if (strstr( line, "_symmetry_Int_Tables_number") != NULL)    CIF_ID = 20;
		if (strstr( line, "_symmetry_space_group_name_H-M") != NULL) CIF_ID = 21;
//------------- check line for Title
		if (strstr( line, "_chemical_name_common") != NULL) CIF_ID = 30;
		if (strstr( line, "_chemical_formula_moiety") != NULL) CIF_ID = 31;
		if (strstr( line, "_chemical_formula_sum") != NULL) CIF_ID = 32;
		if (strstr( line, "_pd_phase_name") != NULL) CIF_ID = 33;

//------------- read data from file
		switch(CIF_ID)
		{
//--------------------------------------- Unit Cell parameters
		case(15):
		case(14):
		case(13):
		case(12):
		case(11):
		case(10):{
			I0 = CIF_ID - 10;
			sprintf(cell_param,"");
			sprintf(keyword,"");
			sscanf(line,"%s %s", &keyword,&cell_param);
			I2 = strcspn(cell_param,"(");
			if (I2 == (int)strlen(cell_param)) I2 = -1;
			I3 = strcspn(cell_param,".") + 1;
			if (I2 > 0) sscanf(cell_param,"%f%c%f%c",&cCell.a[I0],&ch,&cCell.ea[I0],&ch);
			else {sscanf(cell_param,"%f",&cCell.a[I0]); cCell.ea[I0] = 0;}
			if (I2 > 0){for (j=0; j<(I2-I3); j++) cCell.ea[I0] /= 10;}
			break;
				 }
//--------------------------------------- Symmetry
		case(20):{
			sscanf(line,"%s %i",&keyword,&cCell.SpGrN); 
			break;
				 }
		case(21):{
			sprintf(cCell.SpGr,"");
			sprintf(cell_param,"");
			sprintf(ch0,"");
			sprintf(ch1,"");
			sprintf(ch2,"");
			sprintf(ch3,"");
			sscanf(line,"%s %s %s %s %s",&keyword,&ch0,&ch1,&ch2,&ch3); 
			pos = "";
			pos = strstr(ch0,"'");
			if (pos != NULL) {
				pos++;
				sprintf(ch0,"");
				sprintf(ch0,"%s",pos);
			}
			cCell.lType = ch0[0];
			sprintf(cell_param,"%s%s%s%s",ch0,ch1,ch2,ch3);
			I2 = strcspn(cell_param,"'");
			if (I2 != 0) strncat(cCell.SpGr,cell_param, I2);
			SPGR_ID = check_standart_group_name();
			if (SPGR_ID < 0) {
				SPGR_ID = check_non_standart_group_name();
				if (SPGR_ID >= 0) check_standart_group_name_number();
			}
			if (SPGR_ID < 0) return(-1);
			break;
				 }
//--------------------------------------- Title
		case(33):
		case(30):{
			if (strstr(line,"?") != NULL) {
				title_ID = 0;
				break;
			}
			sprintf(Title,"");
//			sscanf(line,"%s %80c",&keyword,&Title); 
			pos  = strstr(line,"'");
			if (pos != NULL) {
				pos++;
			}
			else {
				pos = line;
				if (CIF_ID==30){
					pos = pos + 21;
				}
				else {
					pos = pos + 14;
				}
			}
			while(pos[0]==' ') pos++;
			strncat(cTitle,pos,strlen(pos));
			I2 = strcspn(cTitle,"'");
			if (I2 != 0) strncat(Title,cTitle,I2);
			else strncat(Title,cTitle,strlen(cTitle));
			title_ID = 1;
			break;
				 }
		case(32):
		case(31):{
			if (strstr(line,"?") != NULL) break;
			if (title_ID == 0){
				sprintf(Title,"");
				sprintf(cTitle,"");
				pos  = strstr(line,"'");
				if (pos != NULL) {
					pos++;
				}
				else {
					pos = line;
					if (CIF_ID==31){
						pos = pos + 24;
					}
					else {
						pos = pos + 21;
					}
				}
				while(pos[0]==' ') pos++;
				strncat(cTitle,pos,strlen(pos));
				I2 = strcspn(cTitle,"'");
				if (I2 != 0) strncat(Title,cTitle,I2);
				else strncat(Title,cTitle,strlen(cTitle));
				title_ID = 1;
			}
			break;
				 }
		}
		if (I1 == line_end) break;
	}
	fclose(mfile);
//--------------------------------------- Loops
    mfile=fopen(fname,"r");
	for(i=0; i<line_start; i++) {
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	I1 = line_start;
	lst2sifID = 0;
    while( !feof( mfile ) ){
		I1++;
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if (strstr(line,"_phase") != NULL) {
			if (lst2sifID == 1) break;
			else lst2sifID = 1;
		}
//----------------------------------------------------------------- Structure Parameters
		pos = strstr( line, "_atom_site_label");
		if (pos != NULL){
			I2 = pos - line + 1;
			if (I2 > 5) pos = NULL;
		}
		if( pos != NULL ) {
			i = 0;
			lineID = 0;
			keykode[0] = 0;   //--- Label OK!
			while(lineID == 0){
				sprintf(line,"");
				fgets( line, sizeof(line), mfile); 
				lineID = checkEmptyLine(line);
				if (lineID == 0) {
					pos = strstr(line, "_atom_site_");
					if (pos != NULL) {
						i++;
						if(strstr(line, "_atom_site_occupancy") != NULL )             keykode[1] = i; //--- g OK!
						if(strstr(line, "_atom_site_fract_x") != NULL )               keykode[2] = i; //--- x OK!
						if(strstr(line, "_atom_site_fract_y") != NULL )               keykode[3] = i; //--- y OK!
						if(strstr(line, "_atom_site_fract_z") != NULL )               keykode[4] = i; //--- z OK!
						if(strstr(line, "_type_symbol") != NULL )                     keykode[5] = i; //--- Symbol OK!
						if(strstr(line, "_atom_site_thermal_displace_type") != NULL ) keykode[6] = i; // Aniz. Thermal Parameters Type (U / beta) OK!
						if(strstr(line, "_atom_site_B_iso_or_equiv") != NULL ) {keykode[7] = i; use_iso = 0;} //--- Biso!
						if(strstr(line, "_atom_site_U_iso_or_equiv") != NULL ) {keykode[7] = i; use_iso = 1;} //--- Uiso!
    				}
					else {
						pos = "";
						pos = line;
						if (strstr(line,"loop_") != NULL) lineID = 1; 
						if (strstr(line,"data_") != NULL) lineID = 1; 
						if ((pos[0] == '_') || (pos[0] == ';')) lineID = 1;
						if (lineID == 0)  {	//---------------------------------------------------- get Label, g, x,y,z, Symbol
							nAtom++;
							if (nAtom == MAX_ATOMS_IN_ASYMM) {
								printf("======================================================\n");
								printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
									   nAtom,MAX_ATOMS_IN_ASYMM);
								printf("======================================================\n");
								fclose(mfile);
								return(-1);
							}
							j = 0;
							token = strtok( line, seps );
							lAtom[nAtom-1].Biso = 0.0;
							while( token != NULL ) {
								if (keykode[0] == j) {
									sprintf(cell_param,"");
									sprintf(lAtom[nAtom-1].name,"");
									sprintf(lAtom[nAtom-1].simb,"");
									sprintf(cell_param,"%s",token); 
									sprintf(lAtom[nAtom-1].name,"%s",cell_param);
									pos = strpbrk(cell_param,"0123456789+-");
									if (pos != NULL) {
										I2 = strcspn(cell_param,pos);
										strncat(lAtom[nAtom-1].simb,cell_param,I2);
									}
									else sprintf(lAtom[nAtom-1].simb,"%s",cell_param);
								}
								if (keykode[1] == j) sscanf(token,"%f",&lAtom[nAtom-1].g); 
								if (keykode[2] == j) {   //------------------------------------ X and esd(X)
									sprintf(cell_param,"");
									sprintf(cell_param,"%s",token); 
									if (strstr(cell_param,"/") != NULL) {
										sscanf(cell_param,"%i%c%i",&k,&ch,&l);
										lAtom[nAtom-1].x = (float)k/(float)l;
										lAtom[nAtom-1].ex = 0;
									}
									else {
										I2 = strcspn(cell_param,"(");
										I3 = strcspn(cell_param,".") + 1;
										if (I2 > 0) sscanf(cell_param,"%f%c%f%c",&lAtom[nAtom-1].x,&ch,&lAtom[nAtom-1].ex,&ch);
										else {sscanf(cell_param,"%f",&lAtom[nAtom-1].x); lAtom[nAtom-1].ex = 0;}
										if (I2 > 0){for (i=0; i<(I2-I3); i++) lAtom[nAtom-1].ex /= 10;}
									}
								}
								if (keykode[3] == j) {   //------------------------------------ Y and esd(Y)
									sprintf(cell_param,"");
									sprintf(cell_param,"%s",token); 
									if (strstr(cell_param,"/") != NULL) {
										sscanf(cell_param,"%i%c%i",&k,&ch,&l);
										lAtom[nAtom-1].y = (float)k/(float)l;
										lAtom[nAtom-1].ey = 0;
									}
									else {
										I2 = strcspn(cell_param,"(");
										I3 = strcspn(cell_param,".") + 1;
										if (I2 > 0) sscanf(cell_param,"%f%c%f%c",&lAtom[nAtom-1].y,&ch,&lAtom[nAtom-1].ey,&ch);
										else {sscanf(cell_param,"%f",&lAtom[nAtom-1].y); lAtom[nAtom-1].ey = 0;}
										if (I2 > 0){for (i=0; i<(I2-I3); i++) lAtom[nAtom-1].ey /= 10;}
									}
								}
								if (keykode[4] == j) {   //------------------------------------ Z and esd(Z)
									sprintf(cell_param,"");
									sprintf(cell_param,"%s",token); 
									if (strstr(cell_param,"/") != NULL) {
										sscanf(cell_param,"%i%c%i",&k,&ch,&l);
										lAtom[nAtom-1].z = (float)k/(float)l;
										lAtom[nAtom-1].ez = 0;
									}
									else {
										I2 = strcspn(cell_param,"(");
										I3 = strcspn(cell_param,".") + 1;
										if (I2 > 0) sscanf(cell_param,"%f%c%f%c",&lAtom[nAtom-1].z,&ch,&lAtom[nAtom-1].ez,&ch);
										else {sscanf(cell_param,"%f",&lAtom[nAtom-1].z); lAtom[nAtom-1].ez = 0;}
										if (I2 > 0){for (i=0; i<(I2-I3); i++) lAtom[nAtom-1].ez /= 10;}
									}
								}
								if (keykode[5] == j) {   //------------------------------------ Symbol  
									sprintf(cell_param,"");
									sprintf(lAtom[nAtom-1].simb,"");
									sprintf(cell_param,"%s",token); 
									pos = strpbrk(cell_param,"0123456789+-");
									if (pos != NULL) {
										I1 = strcspn(cell_param,pos);
										strncat(lAtom[nAtom-1].simb,cell_param, I1);
									}
									else sprintf(lAtom[nAtom-1].simb,"%s",cell_param);
								}
								if (keykode[6] == j) {   //------------------------------------ Uani / Bani
									sprintf(cell_param,"%s",token); 
									pos = strstr(cell_param,"Uani");
									if (pos != NULL) Btype = 0;
									else {
										pos = strstr(cell_param,"Bani");
										if (pos != NULL) Btype = 1;
									}
								}
								if (keykode[7] == j) {   //------------------------------------ Biso
									sprintf(cell_param,"");
									sprintf(cell_param,"%s",token); 
									I2 = strcspn(cell_param,"(");
									if (I2 > 0) sscanf(cell_param,"%f%s",&lAtom[nAtom-1].Biso,&ch0);
									else sscanf(cell_param,"%f",&lAtom[nAtom-1].Biso);
								}
								token = strtok( NULL, seps );
								j++;
							} //--- while(token)

						} //--- if(lineID == 0)
					} //--- pos = _atom_site
				} //--- if(lineID == 0)
			} //--- while(lineID == 0)
		} //--- if (pos != NULL)

//----------------------------------------------------------------- Anisotropic Thermal Parameters
		pos = strstr( line, "_atom_site_aniso_");
		if( pos != NULL ) 
		{
			use_aniso = 1;
			for(j=0; j<nAtom; j++){
				lAtom[j].B[0] = lAtom[j].B[1] = lAtom[j].B[2] = 0;
				lAtom[j].B[3] = lAtom[j].B[4] = lAtom[j].B[5] = 0;
//				lAtom[j].Biso = 1;
			}
			j = 0;
			l = 0;
			lineID = 0;
			while(j <= 6){
				sprintf(line,"");
				fgets( line, sizeof(line), mfile); 
				if (strstr(line, "_atom_site_aniso") == NULL) break;
				switch(Btype)
				{
				case(0):{
					if (strstr( line, "_atom_site_aniso_U_11") != NULL) {Bkeykode[j] = 0;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_U_22") != NULL) {Bkeykode[j] = 1;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_U_33") != NULL) {Bkeykode[j] = 2;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_U_12") != NULL) {Bkeykode[j] = 3;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_U_13") != NULL) {Bkeykode[j] = 4;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_U_23") != NULL) {Bkeykode[j] = 5;j++;lineID = 1;break;}
					break;
						}
				case(1):{
					if (strstr( line, "_atom_site_aniso_B_11") != NULL) {Bkeykode[j] = 0;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_B_22") != NULL) {Bkeykode[j] = 1;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_B_33") != NULL) {Bkeykode[j] = 2;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_B_12") != NULL) {Bkeykode[j] = 3;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_B_13") != NULL) {Bkeykode[j] = 4;j++;lineID = 1;break;}
					if (strstr( line, "_atom_site_aniso_B_23") != NULL) {Bkeykode[j] = 5;j++;lineID = 1;break;}
					break;
						}
				}
				if (lineID == 0) l++;
			}
			lineID = 0;
			while(lineID == 0)
			{
				lineID = checkEmptyLine(line);
				if (lineID == 0){
					pos = "";
					pos = line;
					if (strstr(line,"loop_") != NULL) lineID = 1; 
					if (strstr(line,"data_") != NULL) lineID = 1; 
					if ((pos[0] == '_') || (pos[0] == ';')) lineID = 1;
				}
				if (lineID == 0) {
					j = 0;
					token = strtok( line, seps );
					sprintf(cell_param,"%s",token);
					while( token != NULL )
					{
						if (j > l) sscanf(token,"%f",&B[j-l-1]);
						token = strtok( NULL, seps );
						j++;
					}
					if (j < 7) {
						sprintf(line,"");
						fgets( line, sizeof(line), mfile); 
						token = strtok( line, seps );
						while( token != NULL ){
							if (j > l) sscanf(token,"%f",&B[j-l-1]);
							token = strtok( NULL, seps );
							j++;
						}
					}
					for(j=0; j<nAtom; j++){
						if ((strstr(cell_param,lAtom[j].name) != NULL) && (strlen(cell_param) == strlen(lAtom[j].name))){
							for(i=0; i<6; i++) lAtom[j].B[Bkeykode[i]] = B[i];
							break;
						}
					}
					mAtom++;
				}
				sprintf(line,"");
				fgets( line, sizeof(line), mfile); 
			}
		}
		if (I1 == line_end) break;
	}

	fclose(mfile);

	ID = 0;
	ID = get_group_name_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();
	if (cCell.Setting == 0) cCell.Setting = 1;

	return 0;
}





//****************************************************************//
//                 Read RIETAN *.INS File                         //
//****************************************************************//
int checkINSphase1(char *fname, int lN[5])
{
	int i,j;
	char line[90];

	mfile=fopen(fname,"r");

//-------------------------------------------------- Title
	while(!feof(mfile)){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if ((strstr(line,"#") == NULL) && (strstr(line,PHNAME) != NULL)) break;
		lN[0]++;
	}
//-------------------------------------------------- Space Group Number and Setting
	rewind(mfile);
	while(!feof(mfile)){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if ((strstr(line,"#") == NULL) && (strstr(line,VNS) != NULL)) break;
		lN[1]++;
	}
//-------------------------------------------------- Unit Cell Parameters
	rewind(mfile);
	while(!feof(mfile)){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if (strstr(line,"#") == NULL) {
		   if ((strstr(line, CELQ1) != NULL) || (strstr( line, CELQ2) != NULL)) break;
		}
		lN[2]++;
	}
//-------------------------------------------------- End of Phase1
	lN[3] = lN[2]+1;
	rewind(mfile);
	for(i=0; i<lN[2]+1; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	while(!feof(mfile)){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if (strstr(line,"#") == NULL) {
			if ((strstr(line, CELQ1) != NULL) || (strstr( line, CELQ2) != NULL)) break;
			if ((lN[3] > lN[2]) && (strstr(line, "}") != NULL)) break;
		}
		lN[3]++;
	}
//-------------------------------------------------- Structure Parameters
	rewind(mfile);
	for(i=0; i<lN[2]+1; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	nAtom = 0;
	j = lN[2];
	while(!feof(mfile)){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		j++;
		if (j >= lN[3]) break;
		if (strstr(line,"#") == NULL) {
			if (strstr(line,"/") != NULL) {
				if (nAtom == 0) lN[4] = j;
				nAtom++;
			}
		}
	}

	fclose(mfile);
	return 0;
}


int readINSfile(char *fname, int nF)
{
	int lN[5];
	char line[90];
	char *pos;
	char cell_param[20];
	int lineID = 0;
	int cPhase = 0;
	int cNumber = 0;
	int cUCell = 0;
	int I1,I2;
	char ch0;
	int i,j,k;
	int ID;
	char seps[]   = " /,\t\n";
	char *token;
	float param[12];

	checkFileFormat(fname);

	lN[0] = lN[1] = lN[2] = lN[3] = lN[4] = 0;
	checkINSphase1(fname,lN);
	if (nAtom == 0) {
		printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("Warning!\n");
		printf(" Structure parameters should be given for the first phase.\n");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		return -1;
	}

    mfile=fopen(fname,"r");
//-------------------------------------------------- Title
	for(i=0; i<lN[0]; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	pos = strstr( line,"'");
	if (pos != NULL) pos = pos + 1;
	else {
		pos = strstr( line,"=");
		pos = pos + 2;
	}
	I1 = strcspn(pos,":");
	sprintf(Title,"");
	strncat( Title, pos, I1-1 );
//-------------------------------------------------- Space Group Number and Setting
    rewind(mfile);
	for(i=0; i<lN[1]; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	pos  = strstr( line, "'A-");
	if (pos != NULL){
		pos = pos + 3;
		sprintf(cell_param,"");
		sprintf(cell_param,"%s",pos);
		I1 = strcspn(cell_param,"-");
		if (I1 >= 0) sscanf(cell_param,"%i%c%i",&cCell.SpGrN,&ch0,&cCell.Setting);
		else {sscanf(cell_param,"%i",&cCell.SpGrN); cCell.Setting = 1;}
	}
	else {
		printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("Warning!\n");
		printf("Change the space group volume name from <I> to <A>.\n");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fclose(mfile);
		return(-1);
	}
//-------------------------------------------------- Unit Cell Parameters
    rewind(mfile);
	for(i=0; i<lN[2]; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	token = strtok( line, seps );
	I1 = 0;
	i=lN[2];
	token = strtok( NULL, seps );
	while (I1 != 6){
		if (strstr(line,"#") == NULL) {
			while( token != NULL ) {
				sscanf(token,"%f",&cCell.a[I1]);
				I1++;
				if (I1 == 6) break;
				token = strtok( NULL, seps );
			}
		}
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		token = strtok( line, seps );
		i++;
		if (i > lN[3]) {
			printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("Warning!\n");
			printf("Check unit cell parameters.\n");
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			fclose(mfile);
			return -1;
		}
	}
//-------------------------------------------------- Structure Parameters
    rewind(mfile);
	for(i=0; i<lN[4]; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	i = -1;
	I1 = lN[4]-1;
	j = 0;
	while (i < nAtom) {
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		I1++;
		if (I1 >= lN[3]) break;
		if ((strstr(line,"#") == NULL) && (checkEmptyLine(line) == 0)) {
			if (strstr(line,"/") != NULL) {
				I2 = j;
				i++;
				j = 0;
				for(k=0; k<6; k++) lAtom[i].B[k] = 0.0f;
				lAtom[i].Biso = 0.0;
				sprintf(lAtom[i].name,"");
				sprintf(lAtom[i].simb,"");
				token = strtok( line, seps );
				sprintf(lAtom[i].name,"%s",token);
				token = strtok( NULL, seps );
				sprintf(lAtom[i].simb,"%s",token);
				token = strtok( NULL, seps );
				if (i > 0) {
					if (I2 < 5) {
						printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
						printf("Warning!\n");
						printf("Check structure parameters.\n");
						printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
						fclose(mfile);
						return -1;
					}
					lAtom[i-1].g = param[0];
					lAtom[i-1].x = param[1];
					lAtom[i-1].y = param[2];
					lAtom[i-1].z = param[3];
					if (I2 == 5) lAtom[i-1].Biso = param[4];
					else {
						lAtom[i-1].B[0] = param[4];
						lAtom[i-1].B[1] = param[5];
						lAtom[i-1].B[2] = param[6];
						lAtom[i-1].B[3] = param[7];
						lAtom[i-1].B[4] = param[8];
						lAtom[i-1].B[5] = param[9];
						lAtom[i-1].Biso = 0.0;
					}
				}
			}
			else {token = strtok( line, seps );}
			while( token != NULL ) {
				pos = token;
				if (strstr(pos,".") != NULL)  {
					sscanf(token,"%f",&param[j]);
					j++;
					token = strtok( NULL, seps );
				}
				else token = NULL;
			}
		}
	}
	if (j < 5) {
		printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("Warning!\n");
		printf("Check structure parameters.\n");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		fclose(mfile);
		return -1;
	}
	lAtom[i].g = param[0];
	lAtom[i].x = param[1];
	lAtom[i].y = param[2];
	lAtom[i].z = param[3];
	if (j == 5) lAtom[i].Biso = param[4];
	else {
		lAtom[i].B[0] = param[4];
		lAtom[i].B[1] = param[5];
		lAtom[i].B[2] = param[6];
		lAtom[i].B[3] = param[7];
		lAtom[i].B[4] = param[8];
		lAtom[i].B[5] = param[9];
		lAtom[i-1].Biso = 0.0;
	}
	fclose(mfile);
	for(i=0; i<nAtom; i++){
		if ((lAtom[i].B[0] != 0) && (lAtom[i].B[1] != 0) && (lAtom[i].B[2] != 0)) use_aniso = 1;
	}
	if ((cCell.SpGrN == 48) || (cCell.SpGrN == 50) || (cCell.SpGrN == 59) ||
		(cCell.SpGrN == 68) || (cCell.SpGrN == 70)) {
		if (cCell.Setting == 2) cCell.Setting = 7;
	}
	Btype = 1;
	ID = 0;
	ID = get_group_name_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();
	for(i=0; i<nAtom; i++){
		I1 = strcspn(lAtom[i].simb,"-+0123456789");
		if (I1 >= 0) {
			sprintf(cell_param,"");
			strncat(cell_param,lAtom[i].simb,I1);
			sprintf(lAtom[i].simb,"");
			sprintf(lAtom[i].simb,"%s",cell_param);
		}
	}
	return(0);
}


//****************************************************************//
//                 Read XMol *.XYZ File                           //
//****************************************************************//
int readXYZfile(char *fname)
{
	int i,j,k;
	char line[90];
	char cell_param[20];
	char *result;
	float maxPar  = -100000;
	float maxParX  = -100000;
	float maxParY  = -100000;
	float maxParZ  = -100000;
	float minX = 100000;
	float minY = 100000;
	float minZ = 100000;
	float maxX = -100000;
	float maxY = -100000;
	float maxZ = -100000;
	float orig[3];

    mfile=fopen(fname,"r");
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	sscanf(line,"%i",&nAtom);
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	if (checkEmptyLine(line) == 0) sscanf(line,"%s",&Title);
	else sprintf(Title,"XYZ file");
	j = 1;
	for(i=0; i<nAtom; i++){
		sprintf(line,"");
		sprintf(lAtom[nAtom].simb,"");
		sprintf(lAtom[nAtom].name,"");
		fgets( line, sizeof(line), mfile); 
		sscanf(line,"%s %f %f %f",&lAtom[i].simb,&lAtom[i].x,&lAtom[i].y,&lAtom[i].z);
		lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0;
		lAtom[i].g = 1;
		lAtom[i].B[0] = lAtom[i].B[1] = lAtom[i].B[2] = 0;
		lAtom[i].B[3] = lAtom[i].B[4] = lAtom[i].B[5] = 0;
		lAtom[i].Biso = 0;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
//		maxParX = (lAtom[i].x >= maxParX) ? lAtom[i].x : maxParX;
//		maxParY = (lAtom[i].y >= maxParY) ? lAtom[i].y : maxParY;
//		maxParZ = (lAtom[i].z >= maxParZ) ? lAtom[i].z : maxParZ;
		if (i == 0){
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,j);
		}
		else {
			j = 0;
			for(k=i-1; k>=0; k--){
				if (strstr(lAtom[i].simb,lAtom[k].simb) != NULL) {
					sprintf(cell_param,"%s",lAtom[k].name);
					result = strpbrk(cell_param,"123456789");
					if (result != NULL){
						sscanf(result,"%i",&j);
						break;
					}
				}
			}
			j++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,j);
		}
	}
	fclose(mfile);
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (float)fabs(minX);
		lAtom[i].y += (float)fabs(minY);
		lAtom[i].z += (float)fabs(minZ);
		maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
		maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
		maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
	}
	maxPar = (maxParX >= maxParY) ? maxParX : maxParY;
	maxPar = (maxPar  >= maxParZ) ? maxPar  : maxParZ;
	maxPar *= 1.5001f;
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	cCell.a[0] = maxPar;
	cCell.a[1] = maxPar;
	cCell.a[2] = maxPar;
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	Btype = 0;
	minX = 10000;
	minY = 10000;
	minZ = 10000;
	maxX = -10000;
	maxY = -10000;
	maxZ = -10000;
	for(i=0; i<nAtom; i++){
		lAtom[i].x /= maxPar;
		lAtom[i].y /= maxPar;
		lAtom[i].z /= maxPar;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
	}
	orig[0] = (minX + maxX) / 2;
	orig[1] = (minY + maxY) / 2;
	orig[2] = (minZ + maxZ) / 2;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
	}
	
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 0.9f;
	if (def_search_bond == 1){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}

	return(0);
}

//****************************************************************//
//                 Read Chem3D File                               //
//****************************************************************//
int readChem3Dfile(char *fname)
{
	int i,j,I1;
	char line[90];
	float maxPar  = -100000;
	float maxParX  = -100000;
	float maxParY  = -100000;
	float maxParZ  = -100000;
	float minX = 100000;
	float minY = 100000;
	float minZ = 100000;
	float maxZ = -100000;
	float maxX = -100000;
	float maxY = -100000;
	float orig[3];
	char cell_param[20];
	char *result;
	char *token;
	char seps[]   = ", \t\n";

	nAtom = 0;
	mol_bond = 0;
    mfile=fopen(fname,"r");
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	sscanf(line,"%i",&nAtom);
	for(i=0; i<nAtom; i++){
		sprintf(line,"");
		sprintf(lAtom[i].simb,"");
		sprintf(lAtom[i].name,"");
		fgets( line, sizeof(line), mfile); 
		j = 0;
		token = strtok(line, seps);
		while( token != NULL ){
			if (j == 0) {
				sscanf(token,"%s",&lAtom[i].simb);
				m[i].bondNum = 0;
			}
			if (j == 2) sscanf(token,"%f",&lAtom[i].x);
			if (j == 3) sscanf(token,"%f",&lAtom[i].y);
			if (j == 4) sscanf(token,"%f",&lAtom[i].z);
			if (j > 5){
				sscanf(token,"%i",&I1);
				I1--;
				m[i].b_list[m[i].bondNum] = I1;
				m[i].bondNum++;
				m[I1].bondatom = 1;
				mol_bond++;
			}
			token = strtok(NULL, seps);
			j++;
		}
		printf("\n");
		lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0;
		lAtom[i].g = 1;
		lAtom[i].B[0] = lAtom[i].B[1] = lAtom[i].B[2] = 0;
		lAtom[i].B[3] = lAtom[i].B[4] = lAtom[i].B[5] = 0;
		lAtom[i].Biso = 0.0;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
	}
	fclose(mfile);
	I1 = 1;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (float)fabs(minX);
		lAtom[i].y += (float)fabs(minY);
		lAtom[i].z += (float)fabs(minZ);
		maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
		maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
		maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
		sprintf(lAtom[i].name,"");
		if (i == 0) sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		else {
			I1 = 0;
			for(j=i-1; j>=0; j--){
				if (strstr(lAtom[i].simb,lAtom[j].simb) != NULL) {
					sprintf(cell_param,"%s",lAtom[j].name);
					result = strpbrk(cell_param,"123456789");
					if (result != NULL){
						sscanf(result,"%i",&I1);
						break;
					}
				}
			}
			I1++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
	}
	maxPar = (maxParX >= maxParY) ? maxParX : maxParY;
	maxPar = (maxPar  >= maxParZ) ? maxPar  : maxParZ;
//	maxPar *= 1.2f;

	sprintf(Title,"Chem3D_File");
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	cCell.a[0] = cCell.a[1] = cCell.a[2] = maxPar;
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	Btype = 0;
	minX = 10000;
	minY = 10000;
	minZ = 10000;
	maxX = -10000;
	maxY = -10000;
	maxZ = -10000;
	for(i=0; i<nAtom; i++){
		lAtom[i].x /= maxPar;
		lAtom[i].y /= maxPar;
		lAtom[i].z /= maxPar;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
	}
	orig[0] = (minX + maxX) / 2;
	orig[1] = (minY + maxY) / 2;
	orig[2] = (minZ + maxZ) / 2;

	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
		m[i].show        = 'y';
		m[i].Num         = i;
		m[i].Index       = i;
		m[i].symnum      = 0;
		m[i].T[0]        = 0;
		m[i].T[1]        = 0;
		m[i].T[2]        = 0;
		m[i].x           = lAtom[i].x;
		m[i].y           = lAtom[i].y;
		m[i].z           = lAtom[i].z;
		if (m[i].bondatom != 1) m[i].bondatom = 0;
		m[i].show_spin   = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1.0f;
//	range[1] = range[3] = range[5] = 0.9f;
	if ((mol_bond == 0) && (def_search_bond == 1)){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}

	return(0);
}

//****************************************************************//
//                 Read CSSR File                                 //
//****************************************************************//
int readCSSRfile(char *fname)
{
	int i,j;
	char line[90];
	char *pos;
	int I1,I2;
	int Sp_ID;
	float minX = 10000;
	float minY = 10000;
	float minZ = 10000;
	float maxParX = 0;
	float maxParY = 0;
	float maxParZ = 0;
	float maxPar  = 0;
	float maxX = -10000;
	float maxY = -10000;
	float maxZ = -10000;
	float orig[3];
	int ID;

	CSSR_ID = 0;
	mol_bond = 0;
	Sp_ID = 0;
    mfile=fopen(fname,"r");

//------------------------------------------------ Unit Cell Parameters
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	pos = "";
	pos = line;
	pos += 40;
	sscanf(pos,"%f %f %f",&cCell.a[0],&cCell.a[1],&cCell.a[2]);
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	pos = "";
	pos = line;
	pos += 22;
	sscanf(pos,"%f %f %f",&cCell.a[3],&cCell.a[4],&cCell.a[5]);
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	if ((cCell.a[0] == 1.0) && (cCell.a[1] == 1.0) && (cCell.a[2] == 1.0)) CSSR_ID = 1;
//------------------------------------------------ Space Group Number, Name, Setting
	if (CSSR_ID == 0){
		I1 = -1;
		pos = ""; pos = line; pos += 55;
		for(i=0; i<3; i++){
			if (pos[i] == ' ') pos++;
			else {
				sscanf(pos,"%i",&I1);
				break;
			}
		}
		if (I1 != -1) {
			cCell.SpGrN = I1;
			cCell.Setting = 1;
			Sp_ID = 0;
		}
		else {
			Sp_ID = 1;
			cCell.SpGrN = 0;
			cCell.Setting = 1;
			pos = ""; pos = line; pos += 59;
			sprintf(cCell.SpGr,"");
			if (strlen(line) <= 76) sscanf(pos,"%s",&cCell.SpGr);
			else {
				j = 0;
				for(i=0; i<(int)strlen(pos); i++) {
					if ((pos[i] == ' ') && (pos[i+1] == ' ')) break; 
					cCell.SpGr[j] = pos[i];
					j++;
				}
			}
		}
		if (strlen(line) > 76) {
			pos = ""; pos = line; pos += 76;
			sscanf(pos,"%i",&cCell.Setting);
		}
		if (strstr(cCell.SpGr,"P1") != NULL) sprintf(cCell.SpGr,"P 1");
	}
	else {
		sprintf(cCell.SpGr,""); sprintf(cCell.SpGr,"P 1");
		cCell.SpGrN = 1; cCell.Setting = 1;
	}
//------------------------------------------------ Number of Atoms
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	sscanf(line,"%i", &nAtom);
	if(nAtom > MAX_ATOMS_IN_ASYMM) {
		printf("======================================================\n");
		printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
			    nAtom,MAX_ATOMS_IN_ASYMM);
		printf("======================================================\n");
		fclose(mfile);
		return(-1);
	}
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
//------------------------------------------------ Structure Parameters
	for(i=0; i<nAtom; i++){
		sprintf(lAtom[i].simb,"");
		sprintf(lAtom[i].name,"");
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		sscanf(line,"%i %s %f %f %f",&I1,&lAtom[i].name,&lAtom[i].x,&lAtom[i].y,&lAtom[i].z);
		lAtom[i].Biso = 0.0f;
		lAtom[i].g = 1.0f;
		lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0.0f;
		for (j=0; j<6; j++) lAtom[i].B[j] = 0.0f;
		pos = strpbrk(lAtom[i].name,"1234567890");
		if (pos != NULL){
			I1 = strlen(lAtom[i].name);
			I2 = strlen(pos);
			strncat(lAtom[i].simb, lAtom[i].name, I1-I2);
		}
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
	}
	fclose(mfile);
	sprintf(Title,"CSSR File");
	Btype = 0;
	if (CSSR_ID == 0){
		ID = 0;
		if (Sp_ID == 0) ID = get_group_name_and_XYZ();
		else ID = get_group_number_and_XYZ();
		XYZ_to_Matrix(ID);
		Check_Lattice_Type();
		if (cCell.Setting == 0) cCell.Setting = 1;
	}
	else {
		for(i=0; i<nAtom; i++){
			lAtom[i].x += (float)fabs(minX);
			lAtom[i].y += (float)fabs(minY);
			lAtom[i].z += (float)fabs(minZ);
			maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
			maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
			maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
		}
		maxPar = (maxParX >= maxParY) ? maxParX : maxParY;
		maxPar = (maxPar  >= maxParZ) ? maxPar  : maxParZ;
		cCell.a[0] = cCell.a[1] = cCell.a[2] = maxPar;
		cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
		for(i=0; i<6; i++) cCell.ea[i] = 0;
		cCell.lType = 'P';
		cCell.Setting = 1;
		sprintf(cCell.SpGr,"P 1");
		cCell.SpGrN = 1;
		nPos = 1;
		for(i=0; i<3; i++){
			for(j=0; j<3; j++) {
				if (j == i) sPos[0].a[i][j] = 1;
				else sPos[0].a[i][j] = 0;
			}
			sPos[0].t[i] = 0;
		}
		minX = 10000;
		minY = 10000;
		minZ = 10000;
		maxX = -10000;
		maxY = -10000;
		maxZ = -10000;
		for(i=0; i<nAtom; i++){
			lAtom[i].x /= maxPar;
			lAtom[i].y /= maxPar;
			lAtom[i].z /= maxPar;
			minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
			minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
			minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
			maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
			maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
			maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
		}
		orig[0] = (minX + maxX) / 2;
		orig[1] = (minY + maxY) / 2;
		orig[2] = (minZ + maxZ) / 2;
		for(i=0; i<nAtom; i++){
			lAtom[i].x += (0.5f - orig[0]);
			lAtom[i].y += (0.5f - orig[1]);
			lAtom[i].z += (0.5f - orig[2]);
			m[i].show        = 'y';
			m[i].Num         = i;
			m[i].Index       = i;
			m[i].symnum      = 0;
			m[i].T[0]        = 0;
			m[i].T[1]        = 0;
			m[i].T[2]        = 0;
			m[i].x           = lAtom[i].x;
			m[i].y           = lAtom[i].y;
			m[i].z           = lAtom[i].z;
			if (m[i].bondatom != 1) m[i].bondatom = 0;
			m[i].show_spin   = 0;
			m[i].spin_dir[0] = 0;
			m[i].spin_dir[1] = 0;
			m[i].spin_dir[2] = 1;
		}
		nXYZ = nAtom;
		range[0] = range[2] = range[4] = 0;
		range[1] = range[3] = range[5] = 1.0f;

		if ((mol_bond == 0) && (def_search_bond == 1)){
			bond_sNum = 1;
			sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
			sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
			bini[0].d = def_mol_bond;
			bini[0].search_mode = 2;
		}
	}


	return(0);
}

//****************************************************************//
//                 MDL mol File                                   //
//****************************************************************//
int readMDLfile(char *fname)
{
	int i,j,k;
	char line[90];
	char *pos;
	char par1[10],par2[10],par3[10];
	float par;
	float maxParX = 0;
	float maxParY = 0;
	float maxParZ = 0;
	float maxPar  = 0;
	float minX = 10000;
	float minY = 10000;
	float minZ = 10000;
	float maxX = -10000;
	float maxY = -10000;
	float maxZ = -10000;
	float orig[3];
	int I1,I2;
	int bondIndex[2][MAX_NUM_OF_BONDS];

	nAtom = 0;
	mol_bond = 0;
	sprintf(Title,"MDL molfile");
    mfile=fopen(fname,"r");
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	if (checkEmptyLine(line) == 0){
		sprintf(Title,"");
		sscanf(line,"%s",&Title);
	}
	for(i=0; i<2; i++) fgets( line, sizeof(line), mfile); 
	while(!feof(mfile)){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if (checkEmptyLine(line) == 0){
			sprintf(par1,"");
			sprintf(par2,"");
			sscanf(line,"%3s%3s",&par1,&par2);
			sscanf(par1,"%f",&par);nAtom = (int)par;
			sscanf(par2,"%f",&par);mol_bond = (int)par;
			j = 1;
			if ( nAtom > MAX_ATOMS_IN_ASYMM){
				printf("======================================================\n");
				printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
					   nAtom,MAX_ATOMS_IN_ASYMM);
				printf("======================================================\n");
				fclose(mfile);
				return(-1);
			}
			for(i=0; i<nAtom;i++) {
				sprintf(lAtom[i].simb,"");
				sprintf(lAtom[i].name,"");
				sprintf(line,"");
				fgets( line, sizeof(line), mfile); 
				sprintf(par1,"");
				sprintf(par2,"");
				sprintf(par3,"");
				sscanf(line,"%10s%10s%10s %s",&par1,&par2,&par3,&lAtom[i].simb);
				sscanf(par1,"%f",&lAtom[i].x);
				sscanf(par2,"%f",&lAtom[i].y);
				sscanf(par3,"%f",&lAtom[i].z);
				lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0;
				lAtom[i].g = 1;
				for(j=0; j<6; j++) lAtom[i].B[j] = 0;
				lAtom[i].Biso = 0.0;
				minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
				minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
				minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
				if (i == 0)	sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,j);
				else {
					j = 0;
					for(k=i-1; k>=0; k--){
						if (strstr(lAtom[i].simb,lAtom[k].simb) != NULL) {
							sprintf(par1,"");
							sprintf(par1,"%s",lAtom[k].name);
							pos = strpbrk(par1,"123456789");
							if (pos != NULL){
								sscanf(pos,"%i",&j);
								break;
							}
						}
					}
					j++;
					sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,j);
				}
			}
			for(i=0; i<mol_bond; i++){
				sprintf(line,"");
				fgets( line, sizeof(line), mfile); 
				sscanf(line,"%3s%3s",&par1,&par2);
				sscanf(par1,"%f",&par);I1 = (int)par;
				sscanf(par2,"%f",&par);I2 = (int)par;
				bondIndex[0][i] = I1-1;
				bondIndex[1][i] = I2-1;
			}
			break;
		}
	}
	fclose(mfile);
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (float)fabs(minX);
		lAtom[i].y += (float)fabs(minY);
		lAtom[i].z += (float)fabs(minZ);
		maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
		maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
		maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
	}
	maxPar = (maxParX >= maxParY) ? maxParX : maxParY;
	maxPar = (maxPar  >= maxParZ) ? maxPar  : maxParZ;
//	maxPar *= 1.2f;

	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	cCell.a[0] = cCell.a[1] = cCell.a[2] = maxPar;
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	Btype = 0;
	minX = 10000;
	minY = 10000;
	minZ = 10000;
	maxX = -10000;
	maxY = -10000;
	maxZ = -10000;
	for(i=0; i<nAtom; i++){
		lAtom[i].x /= maxPar;
		lAtom[i].y /= maxPar;
		lAtom[i].z /= maxPar;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
	}
	orig[0] = (minX + maxX) / 2;
	orig[1] = (minY + maxY) / 2;
	orig[2] = (minZ + maxZ) / 2;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
		m[i].show        = 'y';
		m[i].Num         = i;
		m[i].Index       = i;
		m[i].symnum      = 0;
		m[i].T[0]        = 0;
		m[i].T[1]        = 0;
		m[i].T[2]        = 0;
		m[i].x           = lAtom[i].x;
		m[i].y           = lAtom[i].y;
		m[i].z           = lAtom[i].z;
		m[i].bondatom    = 0;
		m[i].bondNum     = 0;
		m[i].show_spin   = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
	for(i=0; i<mol_bond; i++){
		I1 = bondIndex[0][i];
		I2 = bondIndex[1][i];
		m[I1].b_list[m[I1].bondNum] = I2;
		m[I2].bondatom = 1;
		m[I1].bondNum++;
	}
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1.0f;
//	range[1] = range[3] = range[5] = 0.9f;
	if ((mol_bond == 0) && (def_search_bond == 1)){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}
	return(0);
}


//****************************************************************//
//                 MXD File                                       //
//****************************************************************//
int readMXDfile(char *fname)
{
	int i,j,k,l;
	char line[90],line1[90];
	char *pos;
	char *token;
	char seps[]   = ", \t\n";
	int Nums[MAX_ATOMS_SPEC];
	float dumm;

	nAtom = 0;
	mol_bond = 0;
	num_shift = 0;
    mfile=fopen(fname,"r");

	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	pos = strchr(line, ',' );
	i = pos - line;
	sprintf(Title,"");
	strncpy(Title, line, i);
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	sscanf(line,"%i %i",&nAtom,&tAtom);
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	j = 0;
	token = strtok(line, seps);
	while( token != NULL ){
		sscanf(token,"%s",&AtomT[j].symb);
		i = strlen(AtomT[j].symb);
		if (i == 2){
			pos = AtomT[j].symb;
			if (AtomT[j].symb[1] == 'A') AtomT[j].symb[1] = 'a';
			if (AtomT[j].symb[1] == 'B') AtomT[j].symb[1] = 'b';
			if (AtomT[j].symb[1] == 'C') AtomT[j].symb[1] = 'c';
			if (AtomT[j].symb[1] == 'D') AtomT[j].symb[1] = 'd';
			if (AtomT[j].symb[1] == 'E') AtomT[j].symb[1] = 'e';
			if (AtomT[j].symb[1] == 'F') AtomT[j].symb[1] = 'f';
			if (AtomT[j].symb[1] == 'G') AtomT[j].symb[1] = 'g';
			if (AtomT[j].symb[1] == 'H') AtomT[j].symb[1] = 'h';
			if (AtomT[j].symb[1] == 'I') AtomT[j].symb[1] = 'i';
			if (AtomT[j].symb[1] == 'K') AtomT[j].symb[1] = 'k';
			if (AtomT[j].symb[1] == 'L') AtomT[j].symb[1] = 'l';
			if (AtomT[j].symb[1] == 'M') AtomT[j].symb[1] = 'm';
			if (AtomT[j].symb[1] == 'N') AtomT[j].symb[1] = 'n';
			if (AtomT[j].symb[1] == 'O') AtomT[j].symb[1] = 'o';
			if (AtomT[j].symb[1] == 'P') AtomT[j].symb[1] = 'p';
			if (AtomT[j].symb[1] == 'R') AtomT[j].symb[1] = 'r';
			if (AtomT[j].symb[1] == 'S') AtomT[j].symb[1] = 's';
			if (AtomT[j].symb[1] == 'T') AtomT[j].symb[1] = 't';
			if (AtomT[j].symb[1] == 'U') AtomT[j].symb[1] = 'u';
			if (AtomT[j].symb[1] == 'Y') AtomT[j].symb[1] = 'y';
		}
		token = strtok(NULL, seps);
		j++;
	}
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	j = 0;
	token = strtok(line, seps);
	while( token != NULL ){
		sscanf(token,"%i",&Nums[j]);
		token = strtok(NULL, seps);
		j++;
	}
	for(i=0; i<3; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
	}
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	sscanf(line,"%f %f %f %f %f %f %f",&dumm,&cCell.a[0],&cCell.a[1],&cCell.a[2],
		                                     &cCell.a[3],&cCell.a[4],&cCell.a[5]);
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	k = 0;
	for(i=0; i<tAtom; i++){
		for(j=0; j<Nums[i]; j++){
			sprintf(line,"");
			fgets( line, sizeof(line), mfile); 
			sprintf(line1,"");
			strncpy(line1, line, 27);
			sscanf(line1,"%f %f %f\n",&lAtom[k].x,&lAtom[k].y,&lAtom[k].z);
			lAtom[k].ex = lAtom[k].ey = lAtom[k].ez = 0;
			lAtom[k].g = 1;
			for(l=0; l<6; l++) lAtom[k].B[l] = 0;
			lAtom[k].Biso = 0;
			sprintf(lAtom[k].simb,"");
			sprintf(lAtom[k].simb,"%s",AtomT[i].symb);
			sprintf(lAtom[k].name,"");
			sprintf(lAtom[k].name,"%s%i",AtomT[i].symb,j+1);
			k++;
		}
	}
	fclose(mfile);
	Btype = 0;
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	set_atom_R_and_RGB();
	return(0);
}


//****************************************************************//
//                 PDB File                                       //
//****************************************************************//
int _get_shift_num(int I1)
{
	int i;
	if (num_shift == 1) {
		if (I1 > line_shift[0]) {I1--; return(I1);}
	}
	if (num_shift > 1){
		for(i=num_shift-1; i>=0; i++){
			if (I1 > line_shift[i]) {
				I1 = I1 - (i+1);
				return(I1);
			}
		}
	}
	return(I1);
}


int readPDBfile(char *fname)
{
	int i,j;
	char line[90];
	char *pos;
	char *result;
	char par1[10];
	int lineID;
	float maxParX = -100000;
	float maxParY = -100000;
	float maxParZ = -100000;
	float maxPar  = -100000;
	float minX = 10000;
	float minY = 10000;
	float minZ = 10000;
	float maxX = -10000;
	float maxY = -10000;
	float maxZ = -10000;
	float orig[3];
	char cell_param[20];
	int I1,I2;
	char *token;
	char seps[]   = ", \t\n";

	nAtom = 0;
	mol_bond = 0;
	num_shift = 0;
    mfile=fopen(fname,"r");
	while(!feof(mfile)){
		if(nAtom > MAX_ATOMS_IN_ASYMM) {
			printf("======================================================\n");
			printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
				   nAtom,MAX_ATOMS_IN_ASYMM);
			printf("======================================================\n");
			fclose(mfile);
			return(-1);
		}
		pos = "";
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		if (checkEmptyLine(line) == 0){
			lineID = 0;
			pos = strstr( line, "ATOM"); 
			if (pos != NULL) lineID = 1;
			else {
				pos = strstr( line, "HETATM"); 
				if (pos != NULL) lineID = 1;
				else {
					pos = strstr( line, "CONECT"); 
					if (pos != NULL) lineID = 3;
					else {
						pos = strstr( line, "TER"); 
						if (pos != NULL) lineID = 2;
					}
				}
			}
			switch(lineID)
			{
			case(1):{
//-------------------------------------------------------------- Atom Name
				pos = pos + 12;
				sprintf(par1,"");
				sprintf(lAtom[nAtom].name,"");
				sscanf(pos,"%s",&par1);
				sprintf(lAtom[nAtom].name,"%s",par1);
				if (strlen(lAtom[nAtom].name) == 0) sprintf(lAtom[nAtom].name,"A");
//-------------------------------------------------------------- Structure parameters 
				pos = pos + 18;
				sscanf(pos,"%f %f %f %f",&lAtom[nAtom].x,&lAtom[nAtom].y,&lAtom[nAtom].z,&lAtom[nAtom].g);
				if (lAtom[nAtom].g == 0) lAtom[nAtom].g = 1;
				lAtom[nAtom].ex = lAtom[nAtom].ey = lAtom[nAtom].ez = 0;
				for(j=0; j<6; j++) lAtom[nAtom].B[j] = 0;
				lAtom[nAtom].Biso = 0;
				minX = (lAtom[nAtom].x <= minX) ? lAtom[nAtom].x : minX;
				minY = (lAtom[nAtom].y <= minY) ? lAtom[nAtom].y : minY;
				minZ = (lAtom[nAtom].z <= minZ) ? lAtom[nAtom].z : minZ;
//-------------------------------------------------------------- Atom Symbol 
				sprintf(lAtom[nAtom].simb,"");
				if (strlen(line) < 77) {
					result = strpbrk(lAtom[nAtom].name,"QWERTYUIOPASDFGHJKLZXCVBNM");
					if (result != NULL){
						sprintf(lAtom[nAtom].simb,"%s",result);
						I2 = strlen(lAtom[nAtom].simb);
						result = strpbrk(lAtom[nAtom].simb,"0123456789");
						if (result != NULL) I1 = strlen(result);
						else I1 = 0;
						sprintf(par1,"");
						sprintf(par1,"%s",lAtom[nAtom].simb);
						sprintf(lAtom[nAtom].simb,"");
						strncat(lAtom[nAtom].simb, par1, I2-I1);
					}
				}
				else {
					I1 = 2;
					pos = pos + 46;
					if (pos[0] == ' '){
						pos++;
						I1 = 1;
					}
					if (pos[0] == ' '){
						pos++;
						I1 = 0;
					}
					if (I1 > 0) strncat(lAtom[nAtom].simb, pos, I1);
					else {
						result = strpbrk(lAtom[nAtom].name,"QWERTYUIOPASDFGHJKLZXCVBNM");
						if (result != NULL){
							sprintf(lAtom[nAtom].simb,"%s",result);
							I2 = strlen(lAtom[nAtom].simb);
							result = strpbrk(lAtom[nAtom].simb,"0123456789");
							if (result != NULL) I1 = strlen(result);
							else I1 = 0;
							sprintf(par1,"");
							sprintf(par1,"%s",lAtom[nAtom].simb);
							sprintf(lAtom[nAtom].simb,"");
							strncat(lAtom[nAtom].simb, par1, I2-I1);
						}
					}
				}
//-------------------------------------------------------------- 
				nAtom++;
				break;
					}
			case(2):{
				pos = pos + 6;
				sscanf(pos,"%i",&line_shift[num_shift]);
				num_shift++;
				break;
					}
			case(3):{
				j = 0;
				token = strtok(line, seps);
				while( token != NULL ){
					if (j == 1) {
						sscanf(token,"%i",&I1);
						I1 = _get_shift_num(I1);
						I1--;
						m[I1].bondNum = 0;
					}
					if (j > 1){
						sscanf(token,"%i",&I2);
						I2 = _get_shift_num(I2);
						I2--;
						m[I1].b_list[m[I1].bondNum] = I2;
						m[I1].bondNum++;
						m[I2].bondatom = 1;
					}
					token = strtok(NULL, seps);
					j++;
				}
				mol_bond++;
				break;
					}
			}
		}
	}
	fclose(mfile);
	I1 = 1;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (float)fabs(minX);
		lAtom[i].y += (float)fabs(minY);
		lAtom[i].z += (float)fabs(minZ);
		maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
		maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
		maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
		sprintf(lAtom[i].name,"");
		if (i == 0) sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		else {
			I1 = 0;
			for(j=i-1; j>=0; j--){
				if (strstr(lAtom[i].simb,lAtom[j].simb) != NULL) {
					sprintf(cell_param,"%s",lAtom[j].name);
					result = strpbrk(cell_param,"123456789");
					if (result != NULL){
						sscanf(result,"%i",&I1);
						break;
					}
				}
			}
			I1++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
	}
	maxPar = (maxParX >= maxParY) ? maxParX : maxParY;
	maxPar = (maxPar  >= maxParZ) ? maxPar  : maxParZ;
//	maxPar *= 1.5f;

	sprintf(Title,"PDB File");
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	cCell.a[0] = cCell.a[1] = cCell.a[2] = maxPar;
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	Btype = 0;
	minX = 10000;
	minY = 10000;
	minZ = 10000;
	maxX = -10000;
	maxY = -10000;
	maxZ = -10000;
	for(i=0; i<nAtom; i++){
		lAtom[i].x /= maxPar;
		lAtom[i].y /= maxPar;
		lAtom[i].z /= maxPar;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
	}
	orig[0] = (minX + maxX) / 2;
	orig[1] = (minY + maxY) / 2;
	orig[2] = (minZ + maxZ) / 2;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
		m[i].show        = 'y';
		m[i].Num         = i;
		m[i].Index       = i;
		m[i].symnum      = 0;
		m[i].T[0]        = 0;
		m[i].T[1]        = 0;
		m[i].T[2]        = 0;
		m[i].x           = lAtom[i].x;
		m[i].y           = lAtom[i].y;
		m[i].z           = lAtom[i].z;
		if (m[i].bondatom != 1) m[i].bondatom = 0;
		m[i].show_spin   = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1.0f;
//	range[1] = range[3] = range[5] = 0.9f;

	if ((mol_bond == 0) && (def_search_bond == 1)){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}
	return(0);
}


//****************************************************************//
//                 MOLDA File                                     //
//****************************************************************//
int readMOLDAfile(char *fname)
{
	int i,j,k,I1,I2;
	char line[90];
	float maxParX = -100000;
	float maxParY = -100000;
	float maxParZ = -100000;
	float maxPar  = -100000;
	float minX = 10000;
	float minY = 10000;
	float minZ = 10000;
	float maxX = -10000;
	float maxY = -10000;
	float maxZ = -10000;
	float orig[3];
	char cell_param[20];
	char *result;
	char *token;
	char seps[]   = ", \t\n";
	int atomID;

	I1 = 1;
    mfile=fopen(fname,"r");
	sprintf(line,"");
	fgets( Title, sizeof(line), mfile); 
	fscanf(mfile,"%i\n", &nAtom);
	if(nAtom > MAX_ATOMS_IN_ASYMM) {
		printf("======================================================\n");
		printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
			   nAtom,MAX_ATOMS_IN_ASYMM);
		printf("======================================================\n");
		fclose(mfile);
		return(-1);
	}
	for(i=0; i<nAtom; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		j = 0;
		token = strtok(line, seps);
		while( token != NULL ){
			switch(j)
			{
			case(0):{sscanf(token,"%f",&lAtom[i].x);break;}
			case(1):{sscanf(token,"%f",&lAtom[i].y);break;}
			case(2):{sscanf(token,"%f",&lAtom[i].z);break;}
			case(3):{sscanf(token,"%i",&atomID);break;}
			}
			token = strtok(NULL, seps);
			j++;
		}
		if (atomID == 0) atomID = 6;
		lAtom[i].g = 1;
		lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0;
		for(j=0; j<6; j++) lAtom[i].B[j] = 0;
		lAtom[i].Biso = 0.0;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		sprintf(lAtom[i].simb,"");
		sprintf(lAtom[i].name,"");
		sprintf(lAtom[i].simb,"%s",dPar[atomID-1].atsymb);
		sprintf(lAtom[i].name,"%s",dPar[atomID-1].atsymb);
		if (i == 0){
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
		else {
			I1 = 0;
			for(k=i-1; k>=0; k--){
				if (strstr(lAtom[i].simb,lAtom[k].simb) != NULL) {
					sprintf(cell_param,"%s",lAtom[k].name);
					result = strpbrk(cell_param,"123456789");
					if (result != NULL){
						sscanf(result,"%i",&I1);
						break;
					}
				}
			}
			I1++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
		m[i].bondNum = 0;

	}
	fscanf(mfile,"%i\n", &mol_bond);
	for(i=0; i<mol_bond; i++){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		j = 0;
		token = strtok(line, seps);
		while( token != NULL ){
			if (j == 0) sscanf(token,"%i",&I1);
			if (j == 1) sscanf(token,"%i",&I2);
			token = strtok(NULL, seps);
			j++;
		}
		I1--;
		I2--;
		m[I1].b_list[m[I1].bondNum] = I2;
		m[I1].bondNum++;
		m[I2].bondatom = 1;
	}
	fclose(mfile);
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (float)fabs(minX);
		lAtom[i].y += (float)fabs(minY);
		lAtom[i].z += (float)fabs(minZ);
		maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
		maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
		maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
	}
	maxPar = (maxParX >= maxParY) ? maxParX : maxParY;
	maxPar = (maxPar  >= maxParZ) ? maxPar  : maxParZ;
//	maxPar *= 1.2f;
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	cCell.a[0] = cCell.a[1] = cCell.a[2] = maxPar;
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	Btype = 0;

	minX = 10000;
	minY = 10000;
	minZ = 10000;
	maxX = -10000;
	maxY = -10000;
	maxZ = -10000;
	for(i=0; i<nAtom; i++){
		lAtom[i].x /= maxPar;
		lAtom[i].y /= maxPar;
		lAtom[i].z /= maxPar;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
	}
	orig[0] = (minX + maxX) / 2;
	orig[1] = (minY + maxY) / 2;
	orig[2] = (minZ + maxZ) / 2;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
		m[i].show        = 'y';
		m[i].Num         = i;
		m[i].Index       = i;
		m[i].symnum      = 0;
		m[i].T[0]        = 0;
		m[i].T[1]        = 0;
		m[i].T[2]        = 0;
		m[i].x           = lAtom[i].x;
		m[i].y           = lAtom[i].y;
		m[i].z           = lAtom[i].z;
		if (m[i].bondatom != 1) m[i].bondatom = 0;
		m[i].show_spin   = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
	
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1.0f;
//	range[1] = range[3] = range[5] = 0.9f;
	if ((mol_bond == 0) && (def_search_bond == 1)){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}

	return(0);
}

//****************************************************************//
//                 Gaussian CUBE File                             //
//****************************************************************//
int readCUBEfile(char *fname)
{
	int i,j,k,I1,atomID;
	char line[90];
	float shift[3];
	float AU = 0.529177f;
	float charg;
	char *result;
	float maxParX = -100000;
	float maxParY = -100000;
	float maxParZ = -100000;
	float maxPar  = -100000;
	float minX = 10000;
	float minY = 10000;
	float minZ = 10000;
	float maxX = -10000;
	float maxY = -10000;
	float maxZ = -10000;
	float orig[3];
	char cell_param[20];
	float x[3],y[3],z[3];
	int PXL[3];

	nAtom = 0;
    mfile=fopen(fname,"r");
	sprintf(line,"");
	fgets( Title, sizeof(line), mfile); 
	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	fscanf(mfile,"%i %f %f %f\n",&nAtom,&shift[0],&shift[1],&shift[2]);
	shift[0] *= AU;
	shift[1] *= AU;
	shift[2] *= AU;
	for (i=0; i<3; i++) fscanf(mfile,"%i %f %f %f\n",&PXL[i],&x[i],&y[i],&z[i]);
	if (nAtom < 0) nAtom = -1*nAtom;
	I1 = 1;
	for(i=0; i<nAtom; i++){
		fscanf(mfile,"%i %f %f %f %f\n",&atomID,&charg,&lAtom[i].x,&lAtom[i].y,&lAtom[i].z);
		lAtom[i].x = lAtom[i].x*AU;
		lAtom[i].y = lAtom[i].y*AU;
		lAtom[i].z = lAtom[i].z*AU;
		lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0;
		lAtom[i].g = 1;
		for(j=0; j<6; j++) lAtom[i].B[j] = 0;
		lAtom[i].Biso = 1;
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x > maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y > maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z > maxZ) ? lAtom[i].z : maxZ;
		sprintf(lAtom[i].simb,"");
		sprintf(lAtom[i].name,"");
		sprintf(lAtom[i].simb,"%s",dPar[atomID-1].atsymb);
		sprintf(lAtom[i].name,"%s",dPar[atomID-1].atsymb);
		if (i == 0){
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
		else {
			I1 = 0;
			for(k=i-1; k>=0; k--){
				if (strstr(lAtom[i].simb,lAtom[k].simb) != NULL) {
					sprintf(cell_param,"%s",lAtom[k].name);
					result = strpbrk(cell_param,"123456789");
					if (result != NULL){
						sscanf(result,"%i",&I1);
						break;
					}
				}
			}
			I1++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
		m[i].bondNum = 0;
	}
	fclose(mfile);
	if ((shift[0] == 0.0) && (shift[1] == 0.0) && (shift[2] == 0.0)){
		cCell.a[0] = (float)PXL[0]*x[0]*AU;
		cCell.a[1] = (float)PXL[1]*y[1]*AU;
		cCell.a[2] = (float)PXL[2]*z[2]*AU;
	}
	else {
		for(i=0; i<nAtom; i++){
			lAtom[i].x += (float)fabs(minX);
			lAtom[i].y += (float)fabs(minY);
			lAtom[i].z += (float)fabs(minZ);
		}
		cCell.a[0] = (float)fabs(shift[0])*2;
		cCell.a[1] = (float)fabs(shift[1])*2;
		cCell.a[2] = (float)fabs(shift[2])*2;
	}
	
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	Btype = 0;

	minX = 10000;
	minY = 10000;
	minZ = 10000;
	maxX = -10000;
	maxY = -10000;
	maxZ = -10000;
	for(i=0; i<nAtom; i++){
		lAtom[i].Biso = 0.0;
		for(j=0;j<6;j++) lAtom[i].B[j] = 0.0;
		lAtom[i].x /= cCell.a[0];
		lAtom[i].y /= cCell.a[1];
		lAtom[i].z /= cCell.a[2];
		minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
		minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
		minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
		maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
		maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
		maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;
	}
	orig[0] = (minX + maxX) / 2;
	orig[1] = (minY + maxY) / 2;
	orig[2] = (minZ + maxZ) / 2;
	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
		m[i].show        = 'y';
		m[i].Num         = i;
		m[i].Index       = i;
		m[i].symnum      = 0;
		m[i].T[0]        = 0;
		m[i].T[1]        = 0;
		m[i].T[2]        = 0;
		m[i].x           = lAtom[i].x;
		m[i].y           = lAtom[i].y;
		m[i].z           = lAtom[i].z;
		if (m[i].bondatom != 1) m[i].bondatom = 0;
		m[i].show_spin   = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
	
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1.0f;
//	range[1] = range[3] = range[5] = 0.9f;
	if ((mol_bond == 0) && (def_search_bond == 1)){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}

	return(0);
}


//****************************************************************//
//                 SCAT f01 File                                  //
//****************************************************************//

int readSCATfile(char *fname)
{
	int i,j,k,I1,atomID,Dum;
	int lineID;
	char pathn[256];
	char line[80];
	char *result,*pos;

	float AU = 0.529177f;
	float maxParX = -100000;
	float maxParY = -100000;
	float maxParZ = -100000;
	float maxPar  = -100000;
	float minX = 10000;
	float minY = 10000;
	float minZ = 10000;
	float maxX = -10000;
	float maxY = -10000;
	float maxZ = -10000;
	float orig[3];
	char cell_param[20];
	int co4d_id;
	int i_return;

	i_return = 0;
	nAtom = 0;
	sprintf(pathn,"");
	strncat(pathn, fname, strlen(fname)-3);
	strcat(pathn, "C04D");
	if ((mfile=fopen(pathn,"r")) != NULL) co4d_id = 1;
	else co4d_id = 0;
	if (co4d_id == 1){
		fscanf(mfile,"%i %i %i\n",&i,&j,&k);
		fscanf(mfile,"%f %f %f\n",&cCell.a[0],&cCell.a[1],&cCell.a[2]);
		fscanf(mfile,"%f %f %f\n",&orig[0],&orig[1],&orig[2]);
		cCell.a[0] *= AU;
		cCell.a[1] *= AU;
		cCell.a[2] *= AU;
		for(i=0; i<3; i++) {
			if (orig[i] != 0) orig[i] *= AU;
		}
		fclose(mfile);
	}
    mfile=fopen(fname,"r");
	sprintf(Title,"");
	sprintf(Title,"SCAT f01 file");

	sprintf(line,"");
	fgets( line, sizeof(line), mfile); 
	lineID = 0;
	i = 0;
	I1 = 1;
	nAtom = 0;
	while (lineID == 0)
	{
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		pos = "";
		pos = line;
		if (pos[0] == '-') lineID = 1;
		else {
			sprintf(lAtom[i].simb,"");
			sprintf(lAtom[i].name,"");
			sscanf(line,"%i %i %f %f %f",&atomID,&Dum,&lAtom[i].y,&lAtom[i].x,&lAtom[i].z);
			lAtom[i].ex = lAtom[i].ey = lAtom[i].ez = 0;
			lAtom[i].g = 1;
			for(j=0; j<6; j++) lAtom[i].B[j] = 0;
			lAtom[i].Biso = 0;
			minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
			minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
			minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
			sprintf(lAtom[i].simb,"%s",dPar[atomID-1].atsymb);
			sprintf(lAtom[i].name,"%s",dPar[atomID-1].atsymb);
			if (i == 0){
				sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
			}
			else {
				I1 = 0;
				for(k=i-1; k>=0; k--){
					if (strstr(lAtom[i].simb,lAtom[k].simb) != NULL) {
						sprintf(cell_param,"%s",lAtom[k].name);
						result = strpbrk(cell_param,"123456789");
						if (result != NULL){
							sscanf(result,"%i",&I1);
							break;
						}
					}
				}
				I1++;
				sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
			}
			m[i].bondNum = 0;
			i++;
		}
	}
	nAtom = i;
	lineID = 0;
	while (lineID == 0){
		sprintf(line,"");
		fgets( line, sizeof(line), mfile); 
		pos = "";
		pos = line;
		if (strstr(line, "Unit") != NULL){
			sscanf(pos,"%i\n",&I1);
			lineID = 1;
		}
	}
	fclose(mfile);
	if (I1 == 1) {
		for(i=0; i<nAtom; i++) {
			lAtom[i].x *= AU;
			lAtom[i].y *= AU;
			lAtom[i].z *= AU;
		}
	}

	if (co4d_id == 1) {
		for(i=0; i<nAtom; i++){
			lAtom[i].x -= orig[0];
			lAtom[i].y -= orig[1];
			lAtom[i].z -= orig[2];
			lAtom[i].x /= cCell.a[0];
			lAtom[i].y /= cCell.a[1];
			lAtom[i].z /= cCell.a[2];
		}
	}
	else {
		for(i=0; i<nAtom; i++){
			lAtom[i].x += (float)fabs(minX);
			lAtom[i].y += (float)fabs(minY);
			lAtom[i].z += (float)fabs(minZ);
			maxParX = ((float)fabs(lAtom[i].x) >= maxParX) ? (float)fabs(lAtom[i].x) : maxParX;
			maxParY = ((float)fabs(lAtom[i].y) >= maxParY) ? (float)fabs(lAtom[i].y) : maxParY;
			maxParZ = ((float)fabs(lAtom[i].z) >= maxParZ) ? (float)fabs(lAtom[i].z) : maxParZ;
		}
		cCell.a[0] = 1.2f*maxParX;
		cCell.a[1] = 1.2f*maxParY;
		cCell.a[2] = 1.2f*maxParZ;
		minX = 10000;
		minY = 10000;
		minZ = 10000;
		maxX = -10000;
		maxY = -10000;
		maxZ = -10000;
		for(i=0; i<nAtom; i++){
			lAtom[i].x /= cCell.a[0];
			lAtom[i].y /= cCell.a[1];
			lAtom[i].z /= cCell.a[2];
			minX = (lAtom[i].x <= minX) ? lAtom[i].x : minX;
			minY = (lAtom[i].y <= minY) ? lAtom[i].y : minY;
			minZ = (lAtom[i].z <= minZ) ? lAtom[i].z : minZ;
			maxX = (lAtom[i].x >= maxX) ? lAtom[i].x : maxX;
			maxY = (lAtom[i].y >= maxY) ? lAtom[i].y : maxY;
			maxZ = (lAtom[i].z >= maxZ) ? lAtom[i].z : maxZ;

		}
		orig[0] = (minX + maxX) / 2;
		orig[1] = (minY + maxY) / 2;
		orig[2] = (minZ + maxZ) / 2;
	}

	
	cCell.lType = 'P';
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.SpGrN = 1;
	nPos = 1;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) {
			if (j == i) sPos[0].a[i][j] = 1;
			else sPos[0].a[i][j] = 0;
		}
		sPos[0].t[i] = 0;
	}
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
	for(i=0; i<6; i++) cCell.ea[i] = 0;
	Btype = 0;

	for(i=0; i<nAtom; i++){
		lAtom[i].x += (0.5f - orig[0]);
		lAtom[i].y += (0.5f - orig[1]);
		lAtom[i].z += (0.5f - orig[2]);
		m[i].show        = 'y';
		m[i].Num         = i;
		m[i].Index       = i;
		m[i].symnum      = 0;
		m[i].T[0]        = 0;
		m[i].T[1]        = 0;
		m[i].T[2]        = 0;
		m[i].x           = lAtom[i].x;
		m[i].y           = lAtom[i].y;
		m[i].z           = lAtom[i].z;
		if (m[i].bondatom != 1) m[i].bondatom = 0;
		m[i].show_spin   = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
	
	range[0] = range[2] = range[4] = 0;
	range[1] = range[3] = range[5] = 1.0f;
	if ((mol_bond == 0) && (def_search_bond == 1)){
		bond_sNum = 1;
		sprintf(bini[0].A1,""); sprintf(bini[0].A1,"XX");
		sprintf(bini[0].A2,""); sprintf(bini[0].A2,"XX");
		bini[0].d = def_mol_bond;
		bini[0].search_mode = 2;
	}

	return(i_return);
}





//****************************************************************//
//                 Get Space Group Name for CRY File              //
//****************************************************************//
void get_spgr_name(char *name)
{
	char *pos;
	char name1[20];
	int I1,I2,code;
	char S;

	pos = "";
	pos = name;
	S = pos[0];
	pos++;
	if ((pos[1] == '1') && (pos[3] == '1')) code = 1;
	if ((pos[1] == '1') && (pos[3] != '1')) code = 2;
	switch(code){
	case(1):{
		pos = pos + 4;
		sprintf(name,"");
		sprintf(name,"%c%s",S,pos);
		break;
			}
	case(2):{
		pos = pos + 2;
		sprintf(name,"");
		sprintf(name,"%c%s",S,pos);
		break;
			}
	}
	sprintf(name1,"%s",name);
	I2 = strlen(name1);
	I1 = 0;
	while(I1 != 1){
		if (name1[I2-1] == ' ') I2--;
		else I1 = 1;
	}
	if (name1[I2-1] == '1') {
		if ((name1[I2-2] == 'm') || (name1[I2-2] == 'a') || (name1[I2-2] == 'c') ||
			(name1[I2-2] == 'n') || (name1[I2-2] == 'd') || (name1[I2-2] == '1')) 
		{
			sprintf(name,"");
			strncat(name, name1, I2-1);
		}
	}
	name++;
	I1 = 0;
	while (I1 != 1) {
	  if (name[0] == ' ') name++;
	  else I1 = 1;
	}
	sprintf(cCell.SpGr,"");
	sprintf(cCell.SpGr,"%c %s",S,name);
	cCell.Setting = 1;
}


//****************************************************************//
//                 Get a, b, c and Alpha                          //
//****************************************************************//
void get_a_b_c_alpha(char *line)
{
	line = line + 9;
	sscanf(line,"%f",&cCell.a[0]);
	line = line + 8;
	sscanf(line,"%f",&cCell.ea[0]);
	line = line + 9;
	sscanf(line,"%f",&cCell.a[1]);
	line = line + 8;
	sscanf(line,"%f",&cCell.ea[1]);
	line = line + 9;
	sscanf(line,"%f",&cCell.a[2]);
	line = line + 8;
	sscanf(line,"%f",&cCell.ea[2]);
	line = line + 8;
	sscanf(line,"%f",&cCell.a[3]);
	line = line + 8;
	sscanf(line,"%f",&cCell.ea[3]);
}


//****************************************************************//
//                 Get Beta and Gamma                             //
//****************************************************************//
void get_Beta_Gamma(char *line)
{
	line = line + 9;
	sscanf(line,"%f",&cCell.a[4]);
	line = line + 8;
	sscanf(line,"%f",&cCell.ea[4]);
	line = line + 6;
	sscanf(line,"%f",&cCell.a[5]);
	line = line + 9;
	sscanf(line,"%f",&cCell.ea[5]);
}

//****************************************************************//
//                 CRYSTIN File                                   //
//****************************************************************//
int readCRYfile(char *fname)
{
	char line[90],line1[90];
	int lineID;
	int I1;
	float par;
	char ch[3];
	char param[20],param0[20];
//	float B,eB;
	float eB;
	char ch0;
	int i,j;
	int mAtom;
	char par1[10],par2[10],par3[10];
	int ID;

	mAtom = 0;
    mfile=fopen(fname,"r");
	while(!feof(mfile))
	{
		sprintf(line,"");
		sprintf(line1,"");
		fgets( line, sizeof(line), mfile); 
		sscanf(line,"%i %s",&lineID,&line1);
		switch(lineID)
		{
//--------------------------------------------------------------- Title
		case(300):{
			sscanf(line,"%i %s",&i,&Title);
			break;
				  }
//--------------------------------------------------------------- Unit Cell Parameters (a,b,c,alpha)
		case(800):{
			get_a_b_c_alpha(line);
			if (cCell.ea[0] < 0) cCell.ea[0] = 0;
			if (cCell.ea[1] < 0) cCell.ea[1] = 0;
			if (cCell.ea[2] < 0) cCell.ea[2] = 0;
			if (cCell.ea[3] < 0) cCell.ea[3] = 0;
			break;
				  }
//--------------------------------------------------------------- Unit Cell Parameters (beta,gamma)
		case(810):{
			get_Beta_Gamma(line);
			if (cCell.ea[4] < 0) cCell.ea[4] = 0;
			if (cCell.ea[5] < 0) cCell.ea[5] = 0;
			break;
				  }
//--------------------------------------------------------------- Space Group Name
		case(900):{
			sprintf(par1,"");
			sprintf(par2,"");
			sprintf(par3,"");
			sprintf(param,"");
			sprintf(param0,"");
			sscanf(line,"%i %c%s %s %s",&i,&ch0,&par1,&par2,&par3);
			cCell.lType = ch0;
			sprintf(param0,"%s %s %s",par1,par2,par3);
			Up2Lo(param0);
			sprintf(param,"%c %s",ch0,param0);
			get_spgr_name(param);
			break;
				  }
//--------------------------------------------------------------- Structure Parameters (Name,Symbol, x,y,z)
		case(1200):{
			if(nAtom > MAX_ATOMS_IN_ASYMM) {
				printf("======================================================\n");
				printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
					   nAtom,MAX_ATOMS_IN_ASYMM);
				printf("======================================================\n");
				fclose(mfile);
				return(-1);
			}
			sprintf(ch,"");
			sprintf(lAtom[nAtom].simb,"");
			sprintf(lAtom[nAtom].name,"");
			sscanf(line,"%i %s %i %f %s %f %f %f %f %f %f",&i,&lAtom[nAtom].simb,&I1,&par,&ch,
				          &lAtom[nAtom].x,&lAtom[nAtom].ex,&lAtom[nAtom].y,&lAtom[nAtom].ey,
						  &lAtom[nAtom].z,&lAtom[nAtom].ez);
			sprintf(lAtom[nAtom].name,"%s%i",lAtom[nAtom].simb,I1);
			if (lAtom[nAtom].ex < 0) lAtom[nAtom].ex = 0;
			if (lAtom[nAtom].ey < 0) lAtom[nAtom].ey = 0;
			if (lAtom[nAtom].ez < 0) lAtom[nAtom].ez = 0;
			for(j=0; j<6; j++) lAtom[nAtom].B[j] = 0;
			lAtom[i].Biso = 0.0;
			nAtom++;
			break;
				   }
//--------------------------------------------------------------- Unit Cell Parameters (g)
		case(1210):{
			sprintf(ch,"");
			if ((strstr(line, "B") == NULL) && (strstr(line, "A") == NULL)){
//				sscanf(line,"%i %f %f %f",&i,&B,&eB,&lAtom[nAtom-1].g);
				sscanf(line,"%i %f %f %f",&i,&lAtom[nAtom-1].Biso,&eB,&lAtom[nAtom-1].g);
			}
			else{
//				sscanf(line,"%i %c %f %f %f",&i,&ch0, &B,&eB,&lAtom[nAtom-1].g);
				sscanf(line,"%i %c %f %f %f",&i,&ch0, &lAtom[nAtom-1].Biso,&eB,&lAtom[nAtom-1].g);
			}
			break;
				   }
//--------------------------------------------------------------- Anizotropic Thermal Parameters (beta11...)
		case(1300):{
			Btype = 1;
//			Btype = 2;
			use_aniso = 1;
			sscanf(line,"%i %s %i %f %f %f %f %f %f",&i,&param,
				    &I1,&lAtom[mAtom].B[0],&eB,&lAtom[mAtom].B[1],&eB,&lAtom[mAtom].B[2],&eB);
			lAtom[mAtom].Biso = 0.0;
			mAtom++;
			break;
				   }
//--------------------------------------------------------------- Anizotropic Thermal Parameters (beta12...)
		case(1310):{
			sscanf(line,"%i %f %f %f %f %f %f",&i,&lAtom[mAtom-1].B[3],&eB,
				         &lAtom[mAtom-1].B[4],&eB,&lAtom[mAtom-1].B[5],&eB);
			break;
				   }
//--------------------------------------------------------------- Anizotropic Thermal Parameters (U11,U22,U33)
		case(1500):{
			Btype = 0;
			use_aniso = 1;
			sscanf(line,"%i %s %i %f %f %f %f %f %f",&i,&param,
				    &I1,&lAtom[mAtom].B[0],&eB,&lAtom[mAtom].B[1],&eB,&lAtom[mAtom].B[2],&eB);
			lAtom[mAtom].Biso = 0.0;
			mAtom++;
			break;
				   }
//--------------------------------------------------------------- Anizotropic Thermal Parameters (U12,U13,U23)
		case(1510):{
			sscanf(line,"%i %f %f %f %f %f %f",&i,&lAtom[mAtom-1].B[3],&eB,
				         &lAtom[mAtom-1].B[4],&eB,&lAtom[mAtom-1].B[5],&eB);
			break;
				   }
		}
	}
	fclose(mfile);
	origID = 0;
//	Btype = 0;
	ID = 0;
	ID = get_group_number_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();
	if (nPos == 0){
		i = get_non_standart_ortho();
		if (i == 0) update_symmetry();
	}


	return(0);
}


//****************************************************************//
//                 FDAT/CSD File                                  //
//****************************************************************//
int readFDATfile(char *fname)
{
	int i,j,k,I1,I2;
	char line[90];
	char *line1;
	char par1[3], par0[1], par2[6];
	int CELLI, ATFOR, NAT, NSAT;
	int param[6],eparam[6],P[6];
	int xyz[3];
	int ID;
	int lineID;
	char Symb1[20],Symb2[20];
	char *pos;
	int modeID;   //--- modeID = 0 - FDAT format; modeID = 1 - CSD format;
	
	sprintf(Symb1,"EAGLIRCNOUSBHDFTMYPK");
	sprintf(Symb2,"eaglircnousbhdftmypk");

    mfile=fopen(fname,"r");
//------------------------------------------------ Read Record No 1
	sprintf(line,"");
	fgets( line, sizeof(line), mfile);
//------------------------------------------------ Get modeID
	pos = strpbrk( line, "QWERTYUIOPLKJHGFDSAZXCVBNM" );
	if (pos != NULL) modeID = 0;
	else modeID = 1;
//------------------------------------------------ Get Number of Atoms
	line1  = line;
	line1  = line1 + 44;
	sprintf(par1,"");sscanf(line1,"%3s",&par1);sscanf(par1,"%i",&NAT);
	line1  = line1 + 3;
	sprintf(par1,"");sscanf(line1,"%3s",&par1);sscanf(par1,"%i",&NSAT);
	nAtom = NAT + NSAT;
	if(nAtom > MAX_ATOMS_IN_ASYMM) {
		printf("======================================================\n");
		printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
			   nAtom,MAX_ATOMS_IN_ASYMM);
		printf("======================================================\n");
		fclose(mfile);
		return(-1);
	}
	line1  = line1 + 9;
	sscanf(line1,"%c",&par0);sscanf(par0,"%1i",&CELLI);
	line1  = line1 + 2;
	sscanf(line1,"%c",&par0);sscanf(par0,"%1i",&ATFOR);
//------------------------------------------------ Read Record No 1
	if (CELLI == 0) skipLines(mfile, 1);
	else {
		sprintf(line,"");
		fgets( line, sizeof(line), mfile);
		line1  = line;
		for(i=0; i<6; i++){
			sprintf(par2,"");strncat(par2, line1, 6);sscanf(par2,"%i",&param[i]);
			line1 = line1 + 6;
		}
		for(i=0; i<6; i++){
			sprintf(par2,"");strncat(par2, line1, 1);sscanf(par2,"%i",&P[i]);
			line1 = line1 + 1;
		}
		for(i=0; i<6; i++){
			sprintf(par2,"");strncat(par2, line1, 2);sscanf(par2,"%i",&eparam[i]);
			line1 = line1 + 2;
		}
//---------------------------------------------- Unit Cell Parameters
		for(i=0; i<6; i++){
			cCell.a[i] = (float)param[i];
			cCell.ea[i] = (float)eparam[i];
			if (P[i] != 0){
				for(j=0; j<P[i]; j++){
					cCell.a[i] /= 10;
					cCell.ea[i] /= 10;
				}
			}
		}
	}
//---------------------------------------------- Space Group Number
	line1 = line1 + 6;
	sprintf(par2,"");strncat(par2, line1, 3);sscanf(par2,"%i",&cCell.SpGrN);
//---------------------------------------------- Structure Parameters
	if (modeID == 1){
		lineID = 0;
		while(lineID == 0){
			sprintf(line,"");fgets( line, sizeof(line), mfile);
			pos = strpbrk(line, "QWERTYUIOPLKJHGFDSAZXCVBNM" );
			if (pos != NULL) {
				lineID = 1;
			}
		}
	}
	else {
		for(i=0; i<3; i++){sprintf(line,"");fgets( line, sizeof(line), mfile);}
	}
	if (ATFOR != 0){
		I1 = (int)(nAtom / 3);
		I2 = nAtom - I1;
		for(i=0; i<I1; i++){
			sprintf(line,"");
			fgets( line, sizeof(line), mfile);
			line1  = line;
//------------------------------------------------------ Label and Symbol
			for(j=0; j<3;j++){
				sprintf(lAtom[i+j*I1].name,"");
				sprintf(lAtom[i+j*I1].simb,"");
				sprintf(par2,"");strncat(par2, line1, 5);sscanf(par2,"%s",&lAtom[i+j*I1].name);
				line1 = line1 + 5;
				lineID = 0;
				for(k=0; k<(int)strlen(lAtom[i+j*I1].name); k++){
					if ((lAtom[i+j*I1].name[k] == '0') || (lAtom[i+j*I1].name[k] == '1') ||
						(lAtom[i+j*I1].name[k] == '2') || (lAtom[i+j*I1].name[k] == '3') ||
						(lAtom[i+j*I1].name[k] == '4') || (lAtom[i+j*I1].name[k] == '5') ||
						(lAtom[i+j*I1].name[k] == '6') || (lAtom[i+j*I1].name[k] == '7') ||
						(lAtom[i+j*I1].name[k] == '8') || (lAtom[i+j*I1].name[k] == '9')) break;
				}
				if (modeID == 1) strncat(lAtom[i+j*I1].simb, lAtom[i+j*I1].name, k);
				else {
					if (k == 1) sprintf(lAtom[i+j*I1].simb,"%1c",lAtom[i+j*I1].name[0]);
					if (k == 2){
						strncat(lAtom[i+j*I1].simb, lAtom[i+j*I1].name, 2);
						for(k=0;k<20;k++) if(lAtom[i+j*I1].simb[1] == Symb1[k]) lAtom[i+j*I1].simb[1] = Symb2[k];
					}
				}
//------------------------------------------------------ x,y,z
				sprintf(par2,"");strncat(par2, line1, 7);sscanf(par2,"%i",&xyz[0]);
				line1 = line1 + 7;
				sprintf(par2,"");strncat(par2, line1, 7);sscanf(par2,"%i",&xyz[1]);
				line1 = line1 + 7;
				sprintf(par2,"");strncat(par2, line1, 7);sscanf(par2,"%i",&xyz[2]);
				line1 = line1 + 7;
				lAtom[i+j*I1].x = (float)xyz[0];
				lAtom[i+j*I1].y = (float)xyz[1];
				lAtom[i+j*I1].z = (float)xyz[2];
				lAtom[i+j*I1].ex = 0;
				lAtom[i+j*I1].ey = 0;
				lAtom[i+j*I1].ez = 0;
				for(k=0; k<5; k++) {
					lAtom[i+j*I1].x /= 10;
					lAtom[i+j*I1].y /= 10;
					lAtom[i+j*I1].z /= 10;
				}
				lAtom[i+j*I1].g = 1;
				lAtom[i+j*I1].Biso = 1;
				for(k=0; k<6; k++) lAtom[i+j*I1].B[k] = 0;
				line1 = line1 + 1;
			}
		}
		if (I2 > 0){
			sprintf(line,"");
			fgets( line, sizeof(line), mfile);
			line1  = line;
//------------------------------------------------------ Label and Symbol
			for(j=0; j<I2;j++){
				sprintf(lAtom[j+3*I1].name,"");
				sprintf(lAtom[j+3*I1].simb,"");
				sprintf(par2,"");strncat(par2, line1, 5);sscanf(par2,"%s",&lAtom[j+3*I1].name);
				line1 = line1 + 5;
				lineID = 0;
				for(k=0; k<(int)strlen(lAtom[j+3*I1].name); k++){
					if ((lAtom[j+3*I1].name[k] == '0') || (lAtom[j+3*I1].name[k] == '1') ||
						(lAtom[j+3*I1].name[k] == '2') || (lAtom[j+3*I1].name[k] == '3') ||
						(lAtom[j+3*I1].name[k] == '4') || (lAtom[j+3*I1].name[k] == '5') ||
						(lAtom[j+3*I1].name[k] == '6') || (lAtom[j+3*I1].name[k] == '7') ||
						(lAtom[j+3*I1].name[k] == '8') || (lAtom[j+3*I1].name[k] == '9')) break;
				}
				if (modeID == 1) strncat(lAtom[j+3*I1].simb, lAtom[j+3*I1].name, k);
				else {
					if (k == 1) sprintf(lAtom[j+3*I1].simb,"%1c",lAtom[j+3*I1].name[0]);
					if (k == 2){
						strncat(lAtom[j+3*I1].simb, lAtom[j+3*I1].name, 2);
						for(k=0;k<20;k++) if(lAtom[j+3*I1].simb[1] == Symb1[k]) lAtom[j+3*I1].simb[1] = Symb2[k];
					}
				}
//------------------------------------------------------ x,y,z
				sprintf(par2,"");strncat(par2, line1, 7);sscanf(par2,"%i",&xyz[0]);
				line1 = line1 + 7;
				sprintf(par2,"");strncat(par2, line1, 7);sscanf(par2,"%i",&xyz[1]);
				line1 = line1 + 7;
				sprintf(par2,"");strncat(par2, line1, 7);sscanf(par2,"%i",&xyz[2]);
				line1 = line1 + 7;
				lAtom[j+3*I1].x = (float)xyz[0];
				lAtom[j+3*I1].y = (float)xyz[1];
				lAtom[j+3*I1].z = (float)xyz[2];
				lAtom[j+3*I1].ex = 0;
				lAtom[j+3*I1].ey = 0;
				lAtom[j+3*I1].ez = 0;
				for(k=0; k<5; k++) {
					lAtom[j+3*I1].x /= 10;
					lAtom[j+3*I1].y /= 10;
					lAtom[j+3*I1].z /= 10;
				}
				lAtom[j+3*I1].g = 1;
				lAtom[j+3*I1].B[0] = lAtom[j+3*I1].B[1] = lAtom[j+3*I1].B[2] = 0;
				lAtom[j+3*I1].B[3] = lAtom[j+3*I1].B[4] = lAtom[j+3*I1].B[5] = 0;
				lAtom[j+3*I1].Biso = 0;

				line1 = line1 + 1;
			}
		}
	}
	fclose(mfile);

	if (modeID == 0) sprintf(Title,"FDAT File");
	else sprintf(Title,"CSD File");
	Btype = 0;
	ID = 0;
	ID = get_group_name_and_XYZ();
	XYZ_to_Matrix(ID);
	Check_Lattice_Type();

	return(0);
}


//****************************************************************//
int get_space_group_for_AMC(char *param)
{
	char ch0, ch1[6], ch2[6], ch3[6];
	char ch;
	char ch_s[1] = "";

	sprintf(ch1,"");
	sprintf(ch2,"");
	sprintf(ch3,"");

	if (param[0] == '*') return(-1);
	ch0 = param[0];
	cCell.lType = ch0;
	param++;
//.................................................................................
	if ((param[0] == 'a') || (param[0] == 'b') || (param[0] == 'c') ||
		(param[0] == 'm') || (param[0] == 'n') || (param[0] == 'd')) {
		sprintf(ch1,"%c",param[0]);
		param++;
	}
	else {
		sprintf(ch_s,"");
		if (param[0] == '-') {sprintf(ch_s,"%c",param[0]); param++;}
		if ((param[0] == '1') ||(param[0] == '2') || (param[0] == '3') || (param[0] == '4') || (param[0] == '6')){
			ch = param[0];
			param++;
			if (param[0] == '_') {
				param++;
				if (param[1] == '/') {sprintf(ch1,"%s%c%c%c%c",ch_s,ch,param[0],param[1],param[2]); param += 3;}
				else {sprintf(ch1,"%s%c%c",ch_s,ch,param[0]); param++;}
			}
			else {
				if (param[0] == '/') {sprintf(ch1,"%s%c%c%c",ch_s,ch,param[0],param[1]);param += 2;}
				else sprintf(ch1,"%s%c",ch_s,ch);
			}
		}
	}
//.................................................................................
	if ((param[0] == 'a') || (param[0] == 'b') || (param[0] == 'c') ||
		(param[0] == 'm') || (param[0] == 'n') || (param[0] == 'd')) {
		sprintf(ch2,"%c",param[0]);
		param++;
	}
	else {
		sprintf(ch_s,"");
		if (param[0] == '-') {sprintf(ch_s,"%c",param[0]); param++;}
		if ((param[0] == '1') ||(param[0] == '2') || (param[0] == '3') || (param[0] == '4') || (param[0] == '6')){
			ch = param[0];
			param++;
			if (param[0] == '_') {
				param++;
				if (param[1] == '/') {sprintf(ch2,"%s%c%c%c%c",ch_s,ch,param[0],param[1],param[2]); param += 3;}
				else {sprintf(ch2,"%s%c%c",ch_s,ch,param[0]); param++;}
			}
			else {
				if (param[0] == '/') {sprintf(ch2,"%s%c%c%c",ch_s,ch,param[0],param[1]);param += 2;}
				else sprintf(ch2,"%s%c",ch_s,ch);
			}
		}
	}
//.................................................................................
	if ((param[0] == 'a') || (param[0] == 'b') || (param[0] == 'c') ||
		(param[0] == 'm') || (param[0] == 'n') || (param[0] == 'd')) {
		sprintf(ch3,"%c",param[0]);
	}
	else {
		sprintf(ch_s,"");
		if (param[0] == '-') {sprintf(ch_s,"%c",param[0]); param++;}
		if ((param[0] == '1') ||(param[0] == '2') || (param[0] == '3') || (param[0] == '4') || (param[0] == '6')){
			ch = param[0];
			param++;
			if (param[0] == '_') {
				param++;
				if (param[1] == '/') {sprintf(ch3,"%s%c%c%c%c",ch_s,ch,param[0],param[1],param[2]);}
				else {sprintf(ch3,"%s%c%c",ch_s,ch,param[0]);}
			}
			else {
				if (param[0] == '/') {sprintf(ch3,"%s%c%c%c",ch_s,ch,param[0],param[1]);}
				else sprintf(ch3,"%s%c",ch_s,ch);
			}
		}
	}
	sprintf(cCell.SpGr,"%c %s %s %s",ch0,ch1,ch2,ch3);
	return(0);
}

//****************************************************************//
int check_is_cell_for_AMC(char *param)
{
	int j,I1;
	char seps[]   = " ,\t\n";
	char *token;
	char param1[80];
	char cell_param[20];
	int ID;

	ID = 0;
	sprintf(param1,"%s",param);
	j = 0;
	token = strtok(param1, seps);
	while( token != NULL ){token = strtok( NULL, seps );j++;}
	if (j != 7) return(1);
	j = 0;
	token = strtok(param, seps);
	while( token != NULL ){
		if (j < 6) {
			I1 = strcspn(token,"0123456789qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM");
			if (I1 > 0) return(1);
			else sscanf(token,"%f",&cCell.a[j]);
		}
		if (j == 6) {
			sscanf(token,"%s",&cell_param);
			ID = get_space_group_for_AMC(cell_param);
		}
		token = strtok(NULL, seps);
		j++;
	}
	return(ID);
}

//****************************************************************//
int check_atom_format_for_AMC(char *param,short index[2][19])
{
	int j;
	char seps[]   = " \t\n";
	char *token;
	char param1[80];
	char *pos;

	sprintf(param1,"%s",param);
	j = 0;
	token = strtok(param, seps);
	while( token != NULL ){
		if( strstr( token, "atom") != NULL )   index[0][ 0] = 1;
		if( strstr( token, "x") != NULL )      index[0][ 1] = 1;
		if( strstr( token, "y") != NULL )      index[0][ 2] = 1;
		if( strstr( token, "z") != NULL )      index[0][ 3] = 1;
		if( strstr( token, "occ") != NULL )    index[0][ 4] = 1;
		if( strstr( token, "Biso") != NULL )   index[0][ 5] = 1;
		if( strstr( token, "Uiso") != NULL )   index[0][ 6] = 1;
		if( strstr( token, "B(1,1)") != NULL ) index[0][ 7] = 1;
		if( strstr( token, "B(2,2)") != NULL ) index[0][ 8] = 1;
		if( strstr( token, "B(3,3)") != NULL ) index[0][ 9] = 1;
		if( strstr( token, "B(1,2)") != NULL ) index[0][10] = 1;
		if( strstr( token, "B(1,3)") != NULL ) index[0][11] = 1;
		if( strstr( token, "B(2,3)") != NULL ) index[0][12] = 1;
		if( strstr( token, "U(1,1)") != NULL ) index[0][13] = 1;
		if( strstr( token, "U(2,2)") != NULL ) index[0][14] = 1;
		if( strstr( token, "U(3,3)") != NULL ) index[0][15] = 1;
		if( strstr( token, "U(1,2)") != NULL ) index[0][16] = 1;
		if( strstr( token, "U(1,3)") != NULL ) index[0][17] = 1;
		if( strstr( token, "U(2,3)") != NULL ) index[0][18] = 1;
		token = strtok(NULL, seps);
		j++;
	}
	pos = strstr( param1, "atom");   if (pos != NULL) index[1][ 0] = pos - param1 + 3;
	pos = strstr( param1, "x");      if (pos != NULL) index[1][ 1] = pos - param1;
	pos = strstr( param1, "y");      if (pos != NULL) index[1][ 2] = pos - param1;
	pos = strstr( param1, "z");      if (pos != NULL) index[1][ 3] = pos - param1;
	pos = strstr( param1, "occ");	 if (pos != NULL) index[1][ 4] = pos - param1 + 2;
	pos = strstr( param1, "Biso");	 if (pos != NULL) {index[1][ 5] = pos - param1 + 3; use_iso = 0;}
	pos = strstr( param1, "Uiso");	 if (pos != NULL) {index[1][ 6] = pos - param1 + 3; use_iso = 1;}
	pos = strstr( param1, "B(1,1)"); if (pos != NULL) index[1][ 7] = pos - param1 + 5;
	pos = strstr( param1, "B(2,2)"); if (pos != NULL) index[1][ 8] = pos - param1 + 5;
	pos = strstr( param1, "B(3,3)"); if (pos != NULL) index[1][ 9] = pos - param1 + 5;
	pos = strstr( param1, "B(1,2)"); if (pos != NULL) index[1][10] = pos - param1 + 5;
	pos = strstr( param1, "B(1,3)"); if (pos != NULL) index[1][11] = pos - param1 + 5;
	pos = strstr( param1, "B(2,3)"); if (pos != NULL) index[1][12] = pos - param1 + 5;
	pos = strstr( param1, "U(1,1)"); if (pos != NULL) index[1][13] = pos - param1 + 5;
	pos = strstr( param1, "U(2,2)"); if (pos != NULL) index[1][14] = pos - param1 + 5;
	pos = strstr( param1, "U(3,3)"); if (pos != NULL) index[1][15] = pos - param1 + 5;
	pos = strstr( param1, "U(1,2)"); if (pos != NULL) index[1][16] = pos - param1 + 5;
	pos = strstr( param1, "U(1,3)"); if (pos != NULL) index[1][17] = pos - param1 + 5;
	pos = strstr( param1, "U(2,3)"); if (pos != NULL) index[1][18] = pos - param1 + 5;

	return(j);
}

//****************************************************************//
void get_atom_symbol(int Num, char *name)
{
	int I1,I2;

	I1 = 0;
	while (I1 == 0){
		I2 = strcspn(name,"0123456789qwertyuioplkjhgfdsazxcvbnm");
		if (I2 == 0)  name++;
		else I1 = 1;
	}
	lAtom[Num].simb[0] = name[0];
	if (strlen(name) > 1){
		name++;
		if (name[0] == '(') name++;
		if (strrchr("0123456789QWERTYUIOPLKJHGFDSAZXCVBNM", name[0] ) == NULL) lAtom[Num].simb[1] = name[0];
	}
}

//****************************************************************//
void get_struc_param_for_AMC(char *param, short index[2][19], int ID)
{
	int I1,I2,k,l;
	char seps[]   = " \t\n";
	char *token;
	char param1[80];
	char cell_par[20];
	char ch;
	int cNum;

	sprintf(param1,"");
	sprintf(param1,"%s",param);
	cNum = 0;
	token = strtok(param, seps);
	while( token != NULL ){
		I1 = 0;
		while(I1 == 0){
			I1 = index[0][cNum];
			if (I1 == 0) cNum++;
			else {
				if (cNum > 3){
					I2 = index[1][cNum];
					if (param[I2] == ' ') {I1 = 0; cNum++;}
				}
			}
		}
		switch(cNum)
		{
		case( 0):{                                                     //--- Label
			sscanf(token,"%s",&lAtom[ID].name);
			get_atom_symbol(ID,lAtom[ID].name);
			break;
				 }
		case( 1):{                                                     //--- x
			sprintf(cell_par,"");
			sprintf(cell_par,"%s",token);
			if (strstr( cell_par, "/") != NULL){
				sscanf(token,"%i%c%i",&k,&ch,&l);
				lAtom[ID].x = (float)k/(float)l;
			}
			else {sscanf(token,"%f",&lAtom[ID].x);}
			lAtom[ID].ex = 0;
			break;
				 }
		case( 2):{                                                     //--- y
			sprintf(cell_par,"");
			sprintf(cell_par,"%s",token);
			if (strstr( cell_par, "/") != NULL){
				sscanf(token,"%i%c%i",&k,&ch,&l);
				lAtom[ID].y = (float)k/(float)l;
			}
			else {sscanf(token,"%f",&lAtom[ID].y);}
			lAtom[ID].ey = 0;
			break;
				 }
		case( 3):{                                                     //--- z
			sprintf(cell_par,"");
			sprintf(cell_par,"%s",token);
			if (strstr( cell_par, "/") != NULL){
				sscanf(token,"%i%c%i",&k,&ch,&l);
				lAtom[ID].z = (float)k/(float)l;
			}
			else {sscanf(token,"%f",&lAtom[ID].z);}
			lAtom[ID].ez = 0;
			break;
				 }
		case( 4):{sscanf(token,"%f",&lAtom[ID].g);break;}               //--- g
		case( 6):
		case( 5):{sscanf(token,"%f",&lAtom[ID].Biso);break;}            //--- Biso or Uiso
		case(13):
		case( 7):{sscanf(token,"%f",&lAtom[ID].B[0]);break;}            //--- beta(1,1) or U(1,1)
		case(14):
		case( 8):{sscanf(token,"%f",&lAtom[ID].B[1]);break;}            //--- beta(2,2) or U(2,2)
		case(15):
		case( 9):{sscanf(token,"%f",&lAtom[ID].B[2]);break;}            //--- beta(3,3) or U(3,3)
		case(16):
		case(10):{sscanf(token,"%f",&lAtom[ID].B[3]);break;}            //--- beta(1,2) or U(1,2)
		case(17):
		case(11):{sscanf(token,"%f",&lAtom[ID].B[4]);break;}            //--- beta(1,3) or U(1,3)
		case(18):
		case(12):{sscanf(token,"%f",&lAtom[ID].B[5]);break;}            //--- beta(2,3) or U(2,3)
		}
		token = strtok(NULL, seps);
		cNum++;
	}
}


//****************************************************************//
//                        Read AMC File                           //
//****************************************************************//
int readAMCfile(char *fname)
{
	int ID0;
	int i,j,lineID;
	char line[80];
	short Index[2][19],numIndex;

    mfile=fopen(fname,"r");
	sprintf(line,"");
//--------------------------------------------  mineral name
	lineID = 1;
	while(lineID != 0){
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
	}
	sprintf(Title,"");sprintf(Title,"%s",line);
//--------------------------------------------  unit cell and space group
	lineID = 1;
	while(lineID != 0){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
		if (lineID == 0){
			cCell.a[0] = cCell.a[1] = cCell.a[2] = 1;
			cCell.a[3] = cCell.a[4] = cCell.a[5] = 90;
			cCell.ea[0] = cCell.ea[1] = cCell.ea[2] = 0;
			cCell.ea[3] = cCell.ea[4] = cCell.ea[5] = 0;
			sprintf(cCell.SpGr,"");
			lineID = 1;
			lineID = check_is_cell_for_AMC(line);
			if (lineID == -1) {fclose(mfile);return(-1);}
		}
	}
//--------------------------------------------  examine atom parameter header (atom  x  y  z ....)
	lineID = 1;
	for(i=0; i<19; i++) {Index[0][i] = 0;Index[1][i] = 0;}
	while(lineID != 0){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
		if (lineID == 0) numIndex = check_atom_format_for_AMC(line, Index);
	}
	if ((Index[0][7] == 0) && (Index[0][14] == 0)) Btype = 0;
	if ((Index[0][7] == 0) && (Index[0][14] != 0)) Btype = 0;
	if (Index[0][7] != 0) Btype = 1;
//--------------------------------------------  structure parameters
	nAtom = 0;
	while( !feof( mfile ) ){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
		if (lineID == 0) {
			lAtom[nAtom].g = 1;
			lAtom[nAtom].Biso = 0.0;
			for(j=0; j<6; j++) lAtom[nAtom].B[j] = 0;
			sprintf(lAtom[nAtom].name,"");
			sprintf(lAtom[nAtom].simb,"");
			get_struc_param_for_AMC(line,Index,nAtom);
			nAtom++;
		}
	}
	fclose(mfile);
	use_aniso = 0;
	for(i=0; i<nAtom; i++){
		if ((lAtom[i].B[0] != 0) && (lAtom[i].B[1] != 0) && (lAtom[i].B[2] != 0)) use_aniso = 1;
	}
	ID0 = 0;
	ID0 = get_group_number_and_XYZ();
	XYZ_to_Matrix(ID0);
	Check_Lattice_Type();

	return(0);
}


//***********************************************************************************
int check_is_symmetry(char *param)
{
	char *pos;
	pos = strstr( param, "Triclinic"); if (pos != NULL) return(1);
	pos = strstr( param, "Monoclinic"); if (pos != NULL) return(1);
	pos = strstr( param, "Orthorhombic"); if (pos != NULL) return(1);
	pos = strstr( param, "Tetragonal"); if (pos != NULL) return(1);
	pos = strstr( param, "Hexagonal"); if (pos != NULL) return(1);
	pos = strstr( param, "Rhomohedral"); if (pos != NULL) return(1);
	pos = strstr( param, "Cubic"); if (pos != NULL) return(1);
	return(0);
}

//***********************************************************************************
void get_space_group_for_MINCR(char *param)
{
	char ch0, ch1[6], ch2[6], ch3[6];
	char ch;
	char ch_s[1] = "";

	sprintf(ch1,"");
	sprintf(ch2,"");
	sprintf(ch3,"");

	ch0 = cCell.lType;
//.................................................................................
	if ((param[0] == 'a') || (param[0] == 'b') || (param[0] == 'c') ||
		(param[0] == 'm') || (param[0] == 'n') || (param[0] == 'd')) {
		sprintf(ch1,"%c",param[0]);
		param++;
	}
	else {
		sprintf(ch_s,"");
		if (param[0] == '-') {sprintf(ch_s,"%c",param[0]); param++;}
		if ((param[0] == '1') ||(param[0] == '2') || (param[0] == '3') || (param[0] == '4') || (param[0] == '6')){
			ch = param[0];
			param++;
			if (param[0] == '(') {
				param++;
				if (param[2] == '/') {sprintf(ch1,"%s%c%c%c%c",ch_s,ch,param[0],param[2],param[3]); param += 4;}
				else {sprintf(ch1,"%s%c%c",ch_s,ch,param[0]); param += 2;}
			}
			else {
				if (param[0] == '/') {sprintf(ch1,"%s%c%c%c",ch_s,ch,param[0],param[1]);param += 2;}
				else sprintf(ch1,"%s%c",ch_s,ch);
			}
		}
	}
//.................................................................................
	if ((param[0] == 'a') || (param[0] == 'b') || (param[0] == 'c') ||
		(param[0] == 'm') || (param[0] == 'n') || (param[0] == 'd')) {
		sprintf(ch2,"%c",param[0]);
		param++;
	}
	else {
		sprintf(ch_s,"");
		if (param[0] == '-') {sprintf(ch_s,"%c",param[0]); param++;}
		if ((param[0] == '1') ||(param[0] == '2') || (param[0] == '3') || (param[0] == '4') || (param[0] == '6')){
			ch = param[0];
			param++;
			if (param[0] == '(') {
				param++;
				if (param[2] == '/') {sprintf(ch2,"%s%c%c%c%c",ch_s,ch,param[0],param[2],param[3]); param += 4;}
				else {sprintf(ch2,"%s%c%c",ch_s,ch,param[0]); param += 2;}
			}
			else {
				if (param[0] == '/') {sprintf(ch2,"%s%c%c%c",ch_s,ch,param[0],param[1]);param += 2;}
				else sprintf(ch2,"%s%c",ch_s,ch);
			}
		}
	}
//.................................................................................
	if ((param[0] == 'a') || (param[0] == 'b') || (param[0] == 'c') ||
		(param[0] == 'm') || (param[0] == 'n') || (param[0] == 'd')) {
		sprintf(ch3,"%c",param[0]);
	}
	else {
		sprintf(ch_s,"");
		if (param[0] == '-') {sprintf(ch_s,"%c",param[0]); param++;}
		if ((param[0] == '1') ||(param[0] == '2') || (param[0] == '3') || (param[0] == '4') || (param[0] == '6')){
			ch = param[0];
			param++;
			if (param[0] == '(') {
				param++;
				if (param[2] == '/') {sprintf(ch3,"%s%c%c%c%c",ch_s,ch,param[0],param[2],param[3]);}
				else {sprintf(ch3,"%s%c%c",ch_s,ch,param[0]);}
			}
			else {
				if (param[0] == '/') {sprintf(ch3,"%s%c%c%c",ch_s,ch,param[0],param[1]);}
				else sprintf(ch3,"%s%c",ch_s,ch);
			}
		}
	}
	sprintf(cCell.SpGr,"%c %s %s %s",ch0,ch1,ch2,ch3);
}

//***********************************************************************************
void set_atoms_name()
{
	int i,k;
	int Num = 1;
	char param[20];
	char *result;

	for(i=0; i<nAtom; i++){
		if (i == 0){
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,Num);
		}
		else {
			Num = 0;
			for(k=i-1; k>=0; k--){
				if (strstr(lAtom[i].simb,lAtom[k].simb) != NULL) {
					sprintf(param,"%s",lAtom[k].name);
					result = strpbrk(param,"123456789");
					if (result != NULL){
						sscanf(result,"%i",&Num);
						break;
					}
				}
			}
			Num++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,Num);
		}
	}
}


//****************************************************************//
//                        Read MINCRYST File                      //
//****************************************************************//
int readMINCRfile(char *fname)
{
	int i,lineID,ID0;
	char line[80];
	char *pos;
	int control;
	char cell_param[15];
	char ch0, ch[15];

    mfile=fopen(fname,"r");
	sprintf(line,"");
//--------------------------------------------  mineral name
	skipLines(mfile,11);
	sprintf(line,"");
	fgets( line, 80, mfile); 
	sprintf(Title,"");
	sprintf(Title,"%s",line);
	skipLines(mfile,6);
//--------------------------------------------  space group
	sprintf(line,"");
	fgets( line, 80, mfile); 
	lineID = check_is_symmetry(line);
	if (lineID == 0) return(-1);
	sscanf(line,"%s %c %s",&cell_param,&ch0,&ch);
	cCell.lType = ch0;
	get_space_group_for_MINCR(ch);
//--------------------------------------------  Lattice parameters
	control = 0;
	while(control == 0){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		pos = strstr( line, "Lattice parameters");
		if (pos != NULL) {
			skipLines(mfile,1);
			control = 1;
		}
	}
	for(i=0; i<3; i++){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		sscanf(line,"%c %c %c %f %s %c %f",&ch0,&ch0,&ch0,&cCell.a[i],&ch,&ch0,&cCell.a[i+3]);
	}
//--------------------------------------------  structure parameters
	control = 0;
	while(control == 0){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		pos = strstr( line, "Co-ordinates,");
		if (pos != NULL) {
			skipLines(mfile,3);
			control = 1;
		}
	}
	nAtom = 0;
	lineID = 0;
	while(lineID == 0){
		sprintf(line,"");
		fgets( line, 80, mfile); 
		lineID = checkEmptyLine(line);
		if (lineID == 0){
			if (nAtom > MAX_ATOMS_IN_ASYMM) {
				printf("======================================================\n");
				printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
					    nAtom,MAX_ATOMS_IN_ASYMM);
				printf("======================================================\n");
				fclose(mfile);
				return(-1);
			}
			sscanf(line,"%i %f %f %f %f %s",&i,&lAtom[nAtom].x,&lAtom[nAtom].y,&lAtom[nAtom].z,
				                            &lAtom[nAtom].Biso,&lAtom[nAtom].simb);
			lAtom[nAtom].ex = lAtom[nAtom].ey = lAtom[nAtom].ez = 0;
			lAtom[nAtom].g = 1;
			for(i=0; i<6; i++) lAtom[nAtom].B[i] = 0;
			nAtom++;
		}
	}
	set_atoms_name();
    fclose(mfile);
	Btype = 0;
	ID0 = 0;
	ID0 = get_group_number_and_XYZ();
	XYZ_to_Matrix(ID0);
	Check_Lattice_Type();

	return(0);
}

//****************************************************************//
//                 Read WIEN2K File                               //
//****************************************************************//
int readWIENfile(char *fname)
{
	char line[85];
	char *pos;
	float au;
	int i,j,I1,I2,ID;
	float X_R_H[3][3];
	double x,y,z;

	nAtom = 0;
	tAtom = 0;
	cCell.lType = 'P';
	au = 0.529177f;
    mfile=fopen(fname,"r");
	sprintf(line,"");
//----------------------------------------------------------------- TITLE
	sprintf(Title,"");
	sprintf(line,"");
	fgets( line, 85, mfile);
	I1 = strlen(line) - 2;
	pos = "";
	pos = line;
	for(i=I1;i>0; i--){
		if (pos[i] != ' ') break;
	}
	I1 = i;
	strncat(Title,pos,I1);
//----------------------------------------------------------------- LATTICE TYPE
	sprintf(line,"");
	fgets( line, 85, mfile);
	pos = "";
	pos = line;
	if (pos[0] == 'P') cCell.lType = 'P';
	if (pos[0] == 'F') cCell.lType = 'F';
	if (pos[0] == 'B') cCell.lType = 'I';
	if (pos[0] == 'R') cCell.lType = 'R';
	if ((pos[0] == 'C') && (pos[1] == 'X') && (pos[2] == 'Y')) cCell.lType = 'C';
	if ((pos[0] == 'C') && (pos[1] == 'Y') && (pos[2] == 'Z')) cCell.lType = 'A';
	if ((pos[0] == 'C') && (pos[1] == 'X') && (pos[2] == 'Z')) cCell.lType = 'B';
//----------------------------------------------------------------- UNIT CELL PARAMETERS
	sprintf(line,"");
	fgets( line, 85, mfile);
	sprintf(line,"");
	fgets( line, 85, mfile);
	pos = line;
	I1 = 0;
	for(i=0; i<6; i++){
		sscanf(pos,"%f",&cCell.a[i]);
		if (i < 3) cCell.a[i] *= au;
		cCell.ea[i] = 0.0;
		pos += 10;
	}
//----------------------------------------------------------------- STRUCTURE AND SYMMETRY
    while( !feof( mfile ) ) {
		sprintf(line,"");
		fgets( line, 85, mfile);
		if (strstr( line, "ATOM") != NULL) ID = 0;  //--- Structure parameters;
		else if (strstr( line, "SYMMETRY") != NULL) ID = 1; //--- Symmetry operations
		if (ID == 0) {
			if (nAtom > MAX_ATOMS_IN_ASYMM) {
				printf("======================================================\n");
				printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
					    nAtom,MAX_ATOMS_IN_ASYMM);
				printf("======================================================\n");
				fclose(mfile);
				return(-1);
			}
			pos = line;
			pos += 13; sscanf(pos,"%f",&lAtom[nAtom].x);
			pos += 13; sscanf(pos,"%f",&lAtom[nAtom].y);
			pos += 13; sscanf(pos,"%f",&lAtom[nAtom].z);
			lAtom[nAtom].ex = 0;
			lAtom[nAtom].ey = 0;
			lAtom[nAtom].ez = 0;
			lAtom[nAtom].Biso = 1;
			for(i=0; i<6; i++) lAtom[nAtom].B[i] = 0;
			lAtom[nAtom].g = 1;
			sprintf(line,"");
			fgets( line, 85, mfile);
			pos = strstr( line, "MULT=");
			pos += 5;
			sscanf(pos,"%i",&I1);
			if (I1 > 1) {
				for(i=1; i<I1; i++) {
					sprintf(line,"");
					fgets( line, 85, mfile);
					pos = line;
					pos += 13; sscanf(pos,"%f",&lAtom[nAtom+i].x);
					pos += 13; sscanf(pos,"%f",&lAtom[nAtom+i].y);
					pos += 13; sscanf(pos,"%f",&lAtom[nAtom+i].z);
					lAtom[nAtom+i].ex = 0;
					lAtom[nAtom+i].ey = 0;
					lAtom[nAtom+i].ez = 0;
					lAtom[nAtom+i].Biso = 1;
					for(j=0; j<6; j++) lAtom[nAtom+i].B[j] = 0;
					lAtom[nAtom+i].g = 1;
				}
			}
			sprintf(line,"");
			fgets( line, 85, mfile);
			sprintf(lAtom[nAtom].name,"");
			pos = line;
			sscanf(pos,"%s1",&lAtom[nAtom].name);
			pos += 56;
			sscanf(pos,"%i",&I2);
			sprintf(lAtom[nAtom].simb,"");
			sprintf(lAtom[nAtom].simb,"%s",dPar[I2-1].atsymb);
			for(i=1; i<I1; i++){
				sprintf(lAtom[nAtom+i].simb,"");
				sprintf(lAtom[nAtom+i].name,"");
				sprintf(lAtom[nAtom+i].name,"%s%i",lAtom[nAtom].name,i+1);
				sprintf(lAtom[nAtom+i].simb,"%s",lAtom[nAtom].simb);

			}
			for(i=0; i<3;i++){
				sprintf(line,"");
				fgets( line, 85, mfile);
			}
			nAtom = nAtom + I1;
		}
		else break;
	}
    fclose(mfile);

	if (cCell.lType == 'R') {
		X_R_H[0][0] =  2.0f/3.0f; X_R_H[0][1] = -1.0f/3.0f; X_R_H[0][2] = -1.0f/3.0f;
		X_R_H[1][0] =  1.0f/3.0f; X_R_H[1][1] =  1.0f/3.0f; X_R_H[1][2] = -2.0f/3.0f;
		X_R_H[2][0] =  1.0f/3.0f; X_R_H[2][1] =  1.0f/3.0f; X_R_H[2][2] =  1.0f/3.0f;
		for(i=0; i<nAtom; i++) {
			x = lAtom[i].x*X_R_H[0][0] + lAtom[i].y*X_R_H[0][1] + lAtom[i].z*X_R_H[0][2];
			y = lAtom[i].x*X_R_H[1][0] + lAtom[i].y*X_R_H[1][1] + lAtom[i].z*X_R_H[1][2];
			z = lAtom[i].x*X_R_H[2][0] + lAtom[i].y*X_R_H[2][1] + lAtom[i].z*X_R_H[2][2];
			if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
			if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
			if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
			lAtom[i].x = (float)x;
			lAtom[i].y = (float)y;
			lAtom[i].z = (float)z;
		}
	}
	Btype = 0;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) sPos[0].a[i][j] = 0;
		sPos[0].t[i] = 0.0;
	}
	sPos[0].a[0][0] = sPos[0].a[1][1] = sPos[0].a[2][2] = 1;
	nPos = 1;
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"");
	sprintf(cCell.SpGr,"P1");
	cCell.SpGrN = 1;
	Check_Lattice_Type();
	cCell.lType = 'P';
	update_atom_type();

	return(0);
}


//****************************************************************//
//                 Read WIEN2K (asse) File                        //
//****************************************************************//
int readWIEN_ASSEfile(char *fname)
{
	char line[85];
	int ID,code,I1,j,i;
	char *pos,cmd[5];
	char chU[27],chL[27],cell_param[20];
	float au;


    sprintf(chU,"QWERTYUIOPLKJHGFDSAZXCVBNM");
    sprintf(chL,"qwertyuioplkjhgfdsazxcvbnm");
	nAtom = 0;
	tAtom = 0;
	cCell.lType = 'P';
	au = 0.529177f;
    mfile=fopen(fname,"r");
	cCell.a[3] = cCell.a[4] = cCell.a[5] = 90.0;

//----------------------------------------------------------------- TITLE
	sprintf(Title,"");
	sprintf(Title,"ASSE file");
   	while( !feof( mfile ) ) {
		sprintf(line,"");
		fgets( line, 256, mfile);
		ID = -1;
		if (strstr( line, "lattice") != NULL) ID = 0;  //--- Lattice block
		if (strstr( line, "LATTICE") != NULL) ID = 0;  //--- Lattice block
		if (strstr( line, "atoms") != NULL) ID = 1;    //--- Structure block
		if (strstr( line, "ATOMS") != NULL) ID = 1;    //--- Structure block
		switch(ID){
		case(0):{         //--- Lattice block
			code = 0;
			while (code != 1) {
				sprintf(line,"");
				fgets( line, 256, mfile);
				if (strstr( line, "type") != NULL) I1 = 0;   //--- Lattice type
				if (strstr( line, "TYPE") != NULL) I1 = 0;   //--- Lattice type
				if (strstr( line, "alat") != NULL) I1 = 1;   //--- a,b,c
				if (strstr( line, "ALAT") != NULL) I1 = 1;   //--- a,b,c
				if (strstr( line, "angle") != NULL) I1 = 2;  //--- alpha,beta,gamma
				if (strstr( line, "ANGLE") != NULL) I1 = 2;  //--- alpha,beta,gamma
				if (strstr( line, "end") != NULL) I1 = 3;  //--- end Lattice block
				if (strstr( line, "END") != NULL) I1 = 3;  //--- end Lattice block
				switch(I1){
				case(0):{ 
					pos = strstr( line, "type");
					if (pos == NULL) pos = strstr( line, "TYPE");
					pos += 5;
					if (pos[0] == 'P') cCell.lType = 'P';
					if (pos[0] == 'F') cCell.lType = 'F';
					if (pos[0] == 'B') cCell.lType = 'I';
					if (pos[0] == 'I') cCell.lType = 'I';
					if (pos[0] == 'R') {cCell.lType = 'R'; cCell.a[5] = 120.0;}
					if (pos[0] == 'H') {cCell.lType = 'P'; cCell.a[5] = 120.0;}
					if ((pos[0] == 'C') && (pos[1] == 'X') && (pos[2] == 'Y')) cCell.lType = 'C';
					if ((pos[0] == 'C') && (pos[1] == 'Y') && (pos[2] == 'Z')) cCell.lType = 'A';
					if ((pos[0] == 'C') && (pos[1] == 'X') && (pos[2] == 'Z')) cCell.lType = 'B';
					cCell.lType = pos[0];
					break;
						}
				case(1):{
					pos = strstr( line, "alat");
					if (pos == NULL) pos = strstr( line, "ALAT");
					pos += 4;
					sscanf(pos,"%f %f %f",&cCell.a[0],&cCell.a[1],&cCell.a[2]);
					cCell.ea[0] = 0.0; cCell.ea[0] = 0.0; cCell.ea[0] = 0.0;
					break;
						}
				case(2):{
					pos = strstr( line, "angle");
					if (pos == NULL) pos = strstr( line, "ANGLE");
					pos += 5;
					sscanf(pos,"%f %f %f",&cCell.a[3],&cCell.a[4],&cCell.a[5]);
					cCell.ea[3] = 0.0; cCell.ea[4] = 0.0; cCell.ea[5] = 0.0;
					break;
						}
				case(3):{
					code = 1;
					break;
						}
				}
			}
			break;
				}
		case(1):{         //--- Structure block
			nAtom = 0;
			code = 0;
			while (code != 1) {
				sprintf(line,"");
				fgets( line, 256, mfile);
				if (checkEmptyLine(line) == 0) {
					if ((strstr(line,"end") != NULL) || (strstr(line,"END") != NULL)) code = 1;
					if (code != 1){
						sscanf(line,"%s %s %f %f %f", &lAtom[nAtom].name,&cmd,&lAtom[nAtom].x,
							                          &lAtom[nAtom].y,&lAtom[nAtom].z);
						if ((strstr(cmd,"xyz") != NULL) || (strstr(cmd,"xyz") != NULL)) {
							lAtom[nAtom].x /= cCell.a[0];
							lAtom[nAtom].y /= cCell.a[1];
							lAtom[nAtom].z /= cCell.a[2];
						}
						lAtom[nAtom].ex = 0;
						lAtom[nAtom].ey = 0;
						lAtom[nAtom].ez = 0;
						lAtom[nAtom].Biso = 1;
						for(j=0; j<6; j++) lAtom[nAtom].B[j] = 0;
						lAtom[nAtom].g = 1;
						nAtom++;
					}
				}
			}
			break;
				}
		}

	}
    fclose(mfile);
	cCell.a[0] *= au; cCell.a[1] *= au; cCell.a[2] *= au;
	for(i=0; i<nAtom; i++){
		sprintf(lAtom[i].simb,"");
		strncat(lAtom[i].simb,lAtom[i].name,2);
		pos = "";
		pos = lAtom[i].simb;
		code = 0;
		for(j=0; j<26; j++){
			if (pos[0] == chU[j]) {
				code = 1;
				break;
			}
		}
		if (code == 0){
			for(j=0; j<26; j++){
				if (pos[0] == chL[j]) {
					code = 1;
					break;
				}
			}
		}
		pos[0] = chU[j];
		lAtom[i].simb[0] = pos[0];
		lAtom[i].simb[1] = pos[1];
	}

	I1 = 1;
	for(i=0; i<nAtom; i++){
		sprintf(lAtom[i].name,"");
		if (i == 0) sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		else {
			I1 = 0;
			for(j=i-1; j>=0; j--){
				if (strstr(lAtom[i].simb,lAtom[j].simb) != NULL) {
					sprintf(cell_param,"%s",lAtom[j].name);
					pos = strpbrk(cell_param,"123456789");
					if (pos != NULL){
						sscanf(pos,"%i",&I1);
						break;
					}
				}
			}
			I1++;
			sprintf(lAtom[i].name,"%s%i",lAtom[i].simb,I1);
		}
	}

	Btype = 0;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) sPos[0].a[i][j] = 0;
		sPos[0].t[i] = 0.0;
	}
	sPos[0].a[0][0] = sPos[0].a[1][1] = sPos[0].a[2][2] = 1;
	nPos = 1;
	cCell.Setting = 1;
	sprintf(cCell.SpGr,"");
	sprintf(cCell.SpGr,"P1");
	cCell.SpGrN = 1;
	Check_Lattice_Type();
	cCell.lType = 'P';
	update_atom_type();

	return(0);
}



//****************************************************************//
//                 Read VICS File                                 //
//****************************************************************//
int readVICSfile(char *fname)
{
	char cmd[6];
	int i,j;
	char line[80];
	int mAtom;
	char *pos;
	int ID;
	char ch0[2],ch1[5],ch2[5],ch3[5];
	float ttt;

	char *token;
	char param1[80];
	char seps[]   = " \t\n";

	mAtom = 0;
    mfile=fopen(fname,"r");
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 80, mfile); 
//----------------------------------------------------------------- TITLE
		pos = strstr( line, "TITLE");
		if( pos != NULL ){
			sprintf(Title,"");
			sprintf(line,"");
			fgets( Title, 80, mfile); 
//			sscanf(line,"%s",&Title);
		}
//----------------------------------------------------------------- GROUP
		pos = strstr( line, "GROUP");
		if( pos != NULL ) {
			sprintf(line,"");
			fgets( line, 80, mfile); 
			sprintf(ch1,"");
			sprintf(ch2,"");
			sprintf(ch3,"");
			controlID = 0; sscanf(line,"%i %i %s %s %s %s",&cCell.SpGrN,&cCell.Setting,&ch0,&ch1,&ch2,&ch3); 
			cCell.lType = ch0[0];
			sprintf(cCell.SpGr,"%s %s %s %s",ch0,ch1,ch2,ch3);
			origID = 0;
			if ((cCell.SpGrN == 48) || (cCell.SpGrN == 50) || (cCell.SpGrN == 59) || 
				(cCell.SpGrN == 68) || (cCell.SpGrN == 70)) {
				if (cCell.Setting > 6) origID = 1;
			}
		}
//----------------------------------------------------------------- SYMOP
		pos = strstr( line, "SYMOP");
		if( pos != NULL ){
			nPos = 0;
			i = -1;
			while(i!=0){
				if(nPos > MAX_SYMM_POS) {
					printf("======================================================\n");
					printf("Number of Symmetry Operations out of range\n nPos=%i\n MAX_SYMM_POS=%i\n",
						   nAtom,MAX_SYMM_POS);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				fscanf( mfile, "%f %f %f %i %i %i %i %i %i %i %i %i\n", 
					   &sPos[nPos].t[0],&sPos[nPos].t[1],&sPos[nPos].t[2],
					   &sPos[nPos].a[0][0],&sPos[nPos].a[1][0],&sPos[nPos].a[2][0],
					   &sPos[nPos].a[0][1],&sPos[nPos].a[1][1],&sPos[nPos].a[2][1],
					   &sPos[nPos].a[0][2],&sPos[nPos].a[1][2],&sPos[nPos].a[2][2]);
				if (sPos[nPos].t[0]==-1.0) {i = 0;}
				else nPos++;
            } 
//			nPos=nPos-1;
        }
//----------------------------------------------------------------- CELLP
		pos = strstr( line, "CELLP");
		if( pos != NULL ) {
			fscanf(mfile,"%f %f %f %f %f %f\n",&cCell.a[0],&cCell.a[1],&cCell.a[2],
					                         &cCell.a[3],&cCell.a[4],&cCell.a[5]);
			fscanf(mfile,"%f %f %f %f %f %f\n",&cCell.ea[0],&cCell.ea[1],&cCell.ea[2],
					                           &cCell.ea[3],&cCell.ea[4],&cCell.ea[5]);
		}

//----------------------------------------------------------------- STRUC
		pos = strstr( line, "STRUC");
		if( pos != NULL ) {
			nAtom = 0;
			i = -1;
			while(i!=0){
				if(nAtom > MAX_ATOMS_IN_ASYMM) {
					printf("======================================================\n");
					printf("Number of Atoms out of range\n nAtom=%i\n MAX_ATOMS_IN_ASYMM=%i\n",
						   nAtom,MAX_ATOMS_IN_ASYMM);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sprintf(lAtom[nAtom].simb,"");
				sprintf(lAtom[nAtom].name,"");
				fscanf(mfile, "%i %s %s %f %f %f %f\n", &i,
					                        &lAtom[nAtom].simb,&lAtom[nAtom].name,&lAtom[nAtom].g,
										    &lAtom[nAtom].x, &lAtom[nAtom].y, &lAtom[nAtom].z);
				fscanf(mfile, "%f %f %f\n", &lAtom[nAtom].ex,&lAtom[nAtom].ey,&lAtom[nAtom].ez);
				lAtom[nAtom].Biso = 0.0;
				for(j=0; j<6; j++) lAtom[nAtom].B[j] = 0;
				nAtom++;
            } 
			nAtom=nAtom-1;
		}
//----------------------------------------------------------------- THERI
		pos = strstr( line, "THERI");
		if( pos != NULL ) {
			mAtom = 0;
			i = -1;
			mAtom = 0;
			while(i!=0){
				if (mAtom > MAX_ATOMS_IN_ASYMM) i = 0;
				else {
					fscanf(mfile,"%i %s %f\n",&i, &lAtom[mAtom].name,&lAtom[mAtom].Biso);
					mAtom++;
				}
			}
		}
//----------------------------------------------------------------- THERT
		pos = strstr( line, "THERT");
		if( pos != NULL ) {
			sscanf(line,"%s %i",&cmd,&Btype);
		}
//----------------------------------------------------------------- THERM
		pos = strstr( line, "THERM");
		if( pos != NULL ) {
			i = -1;
			mAtom = 0;
			while(i!=0){
				if (mAtom > MAX_ATOMS_IN_ASYMM) i = 0;
				else {
					fscanf(mfile,"%i %s %f %f %f %f %f %f\n",&i, &lAtom[mAtom].name,
						        &lAtom[mAtom].B[0],&lAtom[mAtom].B[1],&lAtom[mAtom].B[2],
								&lAtom[mAtom].B[3],&lAtom[mAtom].B[4],&lAtom[mAtom].B[5]);
/**
					if((lAtom[mAtom].B[0] == 0.001) && (lAtom[mAtom].B[1] == 0.001) && (lAtom[mAtom].B[2] == 0.001) &&
						(lAtom[mAtom].B[3] == 0) && (lAtom[mAtom].B[4] == 0) && (lAtom[mAtom].B[5] == 0)) {
						lAtom[mAtom].Biso = 1;
					}
					else if((lAtom[mAtom].B[0] == 1) && (lAtom[mAtom].B[1] == 1) && (lAtom[mAtom].B[2] == 1) &&
						(lAtom[mAtom].B[3] == 0) && (lAtom[mAtom].B[4] == 0) && (lAtom[mAtom].B[5] == 0)) {
						lAtom[mAtom].Biso = 1;
					}
//					else lAtom[mAtom].Biso = 0;
**/
					mAtom = mAtom + 1;
				}
			}
			mAtom = mAtom - 1;
			if (mAtom > 0) use_aniso = 1;
			else use_aniso = 0;
		}
//----------------------------------------------------------------- ATOMT
		pos = strstr( line, "ATOMT");
		if( pos != NULL ) {
			i = -1;
			tAtom = 0;
			while(i!=0){
				if(tAtom >  MAX_ATOMS_SPEC) i = 0;
				else {
					sprintf(AtomT[tAtom].symb,"");
					fscanf(mfile,"%i %s %f %f %f %f\n",&i,&AtomT[tAtom].symb,&AtomT[tAtom].R,
				                  &AtomT[tAtom].color[0],&AtomT[tAtom].color[1],&AtomT[tAtom].color[2]);
					if (i != 0) tAtom++;
				}
			}
		}
//----------------------------------------------------------------- MODEL
		pos = strstr( line, "MODEL");
		if( pos != NULL ) {sscanf(line,"%s %i",&cmd,&MODL);}
//----------------------------------------------------------------- RADII
		pos = strstr( line, "RADII");
		if( pos != NULL ) {sscanf(line,"%s %i",&cmd,&radii_type);}
//----------------------------------------------------------------- BOUND
		pos = strstr( line, "BOUND");
		if( pos != NULL ){
			fscanf(mfile,"%f %f %f %f %f %f", &range[0],&range[1],&range[2],
						                      &range[3],&range[4],&range[5]);
		}
//----------------------------------------------------------------- SBOND
		pos = strstr( line, "SBOND");
		if( pos != NULL ) {
			bond_sNum = 0;
			i = -1;
			while(i!=0){
				if(bond_sNum > MAX_BONDS_SPEC) {
					printf("======================================================\n");
					printf("Number of Bond Specifications out of range\n bond_sNum=%i\n MAX_BONDS_SPEC=%i\n",
						   bond_sNum,MAX_BONDS_SPEC);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				sprintf(line,"");
				sprintf(param1,"");
				fgets( line, 80, mfile); 
				sprintf(param1,line);
				pos = line;
				j = 0;
				token = strtok(param1, seps);
				while( token != NULL ){
					switch(j)
					{
					case(0):{sscanf(token,"%i",&i);break;}
					case(1):{sscanf(token,"%s",&bini[bond_sNum].A1);break;}
					case(2):{sscanf(token,"%s",&bini[bond_sNum].A2);break;}
					case(3):{sscanf(token,"%f",&bini[bond_sNum].d);break;}
					case(4):{sscanf(token,"%i",&bini[bond_sNum].search_mode);break;}
					case(5):{sscanf(token,"%i",&bini[bond_sNum].boundary_mode);break;}
					case(6):{sscanf(token,"%i",&bini[bond_sNum].show_poly_mode);break;}
					}
					token = strtok(NULL, seps);
					j++;
				}
				if (j == 4) bini[bond_sNum].search_mode = -1;
				bond_sNum++;
			}
/**			
			bond_sNum = 0;
			i = -1;
			while(i!=0){
				if(bond_sNum > MAX_BONDS_SPEC) {
					printf("======================================================\n");
					printf("Number of Bond Specifications out of range\n bond_sNum=%i\n MAX_BONDS_SPEC=%i\n",
						   bond_sNum,MAX_BONDS_SPEC);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				fscanf( mfile, "%i %2s %2s %f\n",&i,&bini[bond_sNum].A1,&bini[bond_sNum].A2,
					                                &bini[bond_sNum].d);
				bond_sNum++;
			}
**/
			bond_sNum = bond_sNum - 1;
		}
//----------------------------------------------------------------- HBOND
		pos = strstr( line, "HBOND");
		if( pos != NULL ) {
			sscanf(line,"%s %i %i",&cmd,&hydrogen_bonds_id,&hbond_line_type);
		}
//----------------------------------------------------------------- PLANE
		pos = strstr( line, "PLANE");
		if( pos != NULL ){
			fscanf( mfile, "%i %i %i\n",&hkl[0],&hkl[1],&hkl[2]);
		}
//----------------------------------------------------------------- ATOMS
		pos = strstr( line, "ATOMS");
		if( pos != NULL ){sscanf(line,"%s %i",&cmd,&atom_type);}
//----------------------------------------------------------------- BONDS
		pos = strstr( line, "BONDS");
		if( pos != NULL ){sscanf(line,"%s %i",&cmd,&bonds_type);}
//----------------------------------------------------------------- POLYS
		pos = strstr( line, "POLYS");
		if( pos != NULL ){sscanf(line,"%s %i",&cmd,&poly_type);}
//----------------------------------------------------------------- ATOMP
		pos = strstr( line, "ATOMP");
		if( pos != NULL ){
			fscanf( mfile, "%i %i %i %f\n",&atoms_stack,&atoms_slice,&TEOL,&ttt);
			if (ttt<=1.0) {Probability = (int)(ttt*100.0001);}
			else {Probability = (int)ttt;}
		}
//----------------------------------------------------------------- BONDP
		pos = strstr( line, "BONDP");
		if( pos != NULL ){
			fscanf( mfile, "%i %i %f %f %f %f %f %f\n",&bonds_stack,&bonds_slice,
				    &bRadii[0],&bRadii[1],&bonds_line_width,&BNCL[0],&BNCL[1],&BNCL[2]);
		}
//----------------------------------------------------------------- POLYP
		pos = strstr( line, "POLYP");
		if( pos != NULL ){
			fscanf( mfile, "%f %i %f %f %f %f\n",
				    &poly_trans,&poly_outline,&poly_outline_width,
					&poly_line_color[0],&poly_line_color[1],&poly_line_color[2]);
		}
//----------------------------------------------------------------- HKLPP
		pos = strstr( line, "HKLPP");
		if( pos != NULL ){
			fscanf( mfile, "%f %i %f %f %f %f\n",
   		            &hkl_trans,&show_hkl_out,&hkl_out_width,
				    &hkl_rgb[0],&hkl_rgb[1],&hkl_rgb[2]);
		}
//----------------------------------------------------------------- UCOLP
		pos = strstr( line, "UCOLP");
		if( pos != NULL ){
			fscanf( mfile, "%i %f %f %f %f\n",&SHCL,&ucell_width,
		            &ucell_color[0],&ucell_color[1],&ucell_color[2]);
		}
//----------------------------------------------------------------- BKGRC
		pos = strstr( line, "BKGRC");
		if( pos != NULL ){
			fscanf( mfile, "%f %f %f\n",&BKCL[0],&BKCL[1],&BKCL[2]);
		}
//----------------------------------------------------------------- PROJT
		pos = strstr( line, "PROJT");
		if( pos != NULL ){sscanf(line,"%s %i",&cmd,&projection_type);}
//----------------------------------------------------------------- COMPS
		pos = strstr( line, "COMPS");
		if( pos != NULL ){sscanf(line,"%s %i",&cmd,&show_axis);}
//------------------------------------------------------ DEPTH-CUEING
		pos = strstr( line, "DEPTH");
		if( pos != NULL ){sscanf(line,"%s %f",&cmd,&fog);}
//----------------------------------------------------------------- SCENE
		pos = strstr( line, "SCENE");
		if( pos != NULL ){
			for(i=0; i<4; i++) {
				fscanf( mfile,"%f %f %f %f\n",&rotation[i],&rotation[i+4],&rotation[i+8],&rotation[i+12]);
			}
			fscanf( mfile,"%f %f\n",&obj_pos[0],&obj_pos[1]);
			fscanf( mfile,"%f\n",&obj_pos[2]);
			fscanf( mfile,"%f\n",&scale);
		}
//----------------------------------------------------------------- LIGHT
		pos = strstr( line, "LIGHT");
		if( pos != NULL ){
			for(i=0; i<4; i++) {
				fscanf(mfile,"%f %f %f %f\n",&l_direction[i],&l_direction[i+4],&l_direction[i+8],&l_direction[i+12]);
			}
			fscanf(mfile,"%f %f %f %f\n",&l_position[0],&l_position[1],&l_position[2],&l_position[3]);
			fscanf(mfile,"%f %f %f\n",&l_dir[0],&l_dir[1],&l_dir[2]);
			fscanf(mfile,"%f %f %f %f\n",&l_ambient[0],&l_ambient[1],&l_ambient[2],&l_ambient[3]);
			fscanf(mfile,"%f %f %f %f\n",&l_diffuse[0],&l_diffuse[1],&l_diffuse[2],&l_diffuse[3]);
		}
//----------------------------------------------------------------- ATOMM
		pos = strstr( line, "ATOMM");
		if( pos != NULL ){
			fscanf(mfile,"%f %f %f %f\n",&m_atom_ambient[0],&m_atom_ambient[1],&m_atom_ambient[2],&m_atom_ambient[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_atom_diffuse[0],&m_atom_diffuse[1],&m_atom_diffuse[2],&m_atom_diffuse[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_atom_specular[0],&m_atom_specular[1],&m_atom_specular[2],&m_atom_specular[3]);
			fscanf(mfile,"%f\n",&m_atom_shininess);
		}
//----------------------------------------------------------------- BONDM
		pos = strstr( line, "BONDM");
		if( pos != NULL ){
			fscanf(mfile,"%f %f %f %f\n",&m_bond_ambient[0],&m_bond_ambient[1],&m_bond_ambient[2],&m_bond_ambient[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_bond_diffuse[0],&m_bond_diffuse[1],&m_bond_diffuse[2],&m_bond_diffuse[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_bond_specular[0],&m_bond_specular[1],&m_bond_specular[2],&m_bond_specular[3]);
			fscanf(mfile,"%f\n",&m_bond_shininess);
		}
//----------------------------------------------------------------- POLYM
		pos = strstr( line, "POLYM");
		if( pos != NULL ){
			fscanf(mfile,"%f %f %f %f\n",&m_poly_ambient[0],&m_poly_ambient[1],&m_poly_ambient[2],&m_poly_ambient[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_poly_diffuse[0],&m_poly_diffuse[1],&m_poly_diffuse[2],&m_poly_diffuse[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_poly_specular[0],&m_poly_specular[1],&m_poly_specular[2],&m_poly_specular[3]);
			fscanf(mfile,"%f\n",&m_poly_shininess);
		}
//----------------------------------------------------------------- HKLPM
		pos = strstr( line, "HKLPM");
		if( pos != NULL ){
			fscanf(mfile,"%f %f %f %f\n",&m_plane_ambient[0],&m_plane_ambient[1],&m_plane_ambient[2],&m_plane_ambient[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_plane_diffuse[0],&m_plane_diffuse[1],&m_plane_diffuse[2],&m_plane_diffuse[3]);
			fscanf(mfile,"%f %f %f %f\n",&m_plane_specular[0],&m_plane_specular[1],&m_plane_specular[2],&m_plane_specular[3]);
			fscanf(mfile,"%f\n",&m_poly_shininess);
		}
//----------------------------------------------------------------- DELOB
		pos = strstr( line, "DELOB");
		if( pos != NULL ){
			fscanf(mfile,"%i\n",&del_Object.nAtom);
			for (i=0; i<del_Object.nAtom; i++) fscanf(mfile,"%i",&del_Object.del_Atom[i]);
			fscanf(mfile,"%i\n",&del_Object.nBonds);
			for (i=0; i<del_Object.nBonds; i++) fscanf(mfile,"%i",&del_Object.del_Bonds[i]);
			fscanf(mfile,"%i\n",&del_Object.nPoly);
			for (i=0; i<del_Object.nPoly; i++) fscanf(mfile,"%i",&del_Object.del_Poly[i]);
		}

//----------------------------------------------------------------- 
//----------------------------------------------------------------- 
	}
	fclose(mfile);
	ID = 0;
	if (nPos == 0) {
		ID = get_group_name_and_XYZ();
		XYZ_to_Matrix(ID);
		Check_Lattice_Type();
	}
	return 0;
}


//****************************************************************//
int get_spin_from_file(char *fname)
{
	int i,I1, nSpin;
	float dS[3];
	char line[80];
	char *pos;

	nSpin = 0;
    mfile=fopen(fname,"r");
	if( mfile == NULL ) {
		printf( "The file (%s) was not opened\n",fname );
		return(0);
	}
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 80, mfile); 
//----------------------------------------------------------------- VECTA
		pos = strstr( line, "VECTA");
		if( pos != NULL ){
			fscanf(mfile,"%i\n",&nSpin);
			if (nSpin != 0) {
				fscanf(mfile,"%f\n",&spin_H);
				fscanf(mfile,"%f %f %f\n",&spin_RGB[0],&spin_RGB[1],&spin_RGB[2]);
				for(i=0; i<nSpin; i++){
					fscanf(mfile,"%i %f %f %f",&I1, &dS[0],&dS[1],&dS[2]);
					m[I1].show_spin = 1;
					m[I1].spin_dir[0] = dS[0];
					m[I1].spin_dir[1] = dS[1];
					m[I1].spin_dir[2] = dS[2];
				}
			}
		}
	}
	fclose(mfile);
	return nSpin;
}



//****************************************************************//
void init_data_settings()
{
	sprintf(Title,"%s","");
	Setting = 1;
	NumSpGr = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.Setting = 1;
	cCell.SpGrN = 1;
	cCell.lType = 'P';
	nAtom = 0;
	Btype = 0;
	nPos  = 0;
}

//****************************************************************//
void init_data_settings1()
{
	int i;
//--------------------------------------------------- Initial Structure Settings 
	sprintf(Title,"%s","");
	Setting = 1;
	NumSpGr = 1;
	sprintf(cCell.SpGr,"P 1");
	cCell.Setting = 1;
	cCell.SpGrN = 1;
	cCell.lType = 'P';
	origID = 0;

	radii_type = 0;
	nAtom = 0;
	nBonds = 0;
	nPoly = 0;
	num_Spin = 0;
	nPos  = 0;
	Btype = 0;
	use_aniso = 0;
	use_iso = 0;
	tAtom = 0;
	bond_sNum = 0;
//--------------------------------------------------- Initial Boundaries Settings
	range[0] = range[2] = range[4] = 0; 
	range[1] = range[3] = range[5] = 1; 
//--------------------------------------------------- Initial Background Settings
	BKCL[0] = def_bkgr_color[0];
	BKCL[1] = def_bkgr_color[1];
	BKCL[2] = def_bkgr_color[2];
//--------------------------------------------------- Initial Model Settings
	MODL = 1;
	dot_surf_ID = 0;
//--------------------------------------------------- Initial Atoms Settings
	atom_type = 0;
	atoms_stack = def_atom_s_stick;
	atoms_slice = def_atom_s_slice;
	out_atoms_stack = def_atom_o_stick;
	out_atoms_slice = def_atom_o_slice;
	Probability = 50;
	TEOL = 0;
//--------------------------------------------------- Initial Bonds Settings
	bonds_type = 0;
	hydrogen_bonds_id = 0;
	hbond_line_type = 2;
	bonds_stack = def_bond_s_stick;
	bonds_slice = def_bond_s_slice;
	out_bonds_stack = def_bond_o_stick;
	out_bonds_slice = def_bond_o_slice;
	bonds_line_width = def_bond_line_width;
	for(i=0; i<2; i++) bRadii[i] = def_bond_rad[i];
	for(i=0; i<3; i++) BNCL[i] = def_bond_color[i];
//--------------------------------------------------- Initial Polyhedra Settings
	poly_type = 1;
	poly_outline = 1;
	poly_outline_width = def_poly_line_width;
	for(i=0; i<3; i++) poly_line_color[i] = def_poly_color[i];
	poly_trans = def_poly_tran;
//--------------------------------------------------- Initial Unit Cell Outline Settings
	ucell_width = def_ucel_line_width;
	ucell_type = def_ucel_type;
	for(i=0; i<3; i++) ucell_color[i] = def_ucel_color[i];
//--------------------------------------------------- Initial (hkl) Plane Settings
	show_hkl = 0;
	origin_hkl = 1;
	show_hkl_inv = 0;
	show_hkl_out = 1;
	hkl_out_width = def_hkl_line_width;
	hkl_trans = def_hkl_tran;
	hkl[0] = hkl[1] = hkl[2] = 1;
	for(i=0; i<3; i++) hkl_rgb[i] = def_hkl_color[i];
	shift_hkl[0] = shift_hkl[1] = shift_hkl[2] = 0.0f;
	cPNum = 0;
	move_hkl = 0;

	HKLplane[0] = HKLplane[1] = HKLplane[2] = 1; 
	SHCL = 0;
//--------------------------------------------------- Initial Compass Settings
	show_axis = 1;
	show_axis_label = 1;
//--------------------------------------------------- Initial Projection Settings
	projection_type = 0;
	perspective = 1.0f;
//--------------------------------------------------- Initial Scene Settings
	scale = 1.0f;
	obj_pos[0] = obj_pos[1] = obj_pos[2] = 0;
	for (i=0; i<16; i++) rotation[i] = 0.0f;
	rotation[0] = rotation[5] = rotation[10] = rotation[15] = 1.0f;
	for (i=0; i<16; i++) c_rotation[i] = rotation[i];
//--------------------------------------------------- Initial Materials Settings
	m_atom_ambient[0] = m_atom_ambient[1] = m_atom_ambient[2] = 0.25f; m_atom_ambient[3] = 1.0f;
	m_atom_diffuse[0] = m_atom_diffuse[1] = m_atom_diffuse[2] = 0.75f;m_atom_diffuse[3] = 1.0;
	m_atom_specular[0] = m_atom_specular[1] = m_atom_specular[2] = m_atom_specular[3] = 1.0f;
	m_atom_shininess  = 128.0f;

	m_bond_ambient[0] = m_bond_ambient[1] = m_bond_ambient[2] = 0.25f; m_bond_ambient[3] = 1.0f;
	m_bond_diffuse[0] = m_bond_diffuse[1] = m_bond_diffuse[2] = 0.75f; m_bond_diffuse[3] = 1.0;
	m_bond_specular[0] = m_bond_specular[1] = m_bond_specular[2] = m_bond_specular[3] = 1.0f;
	m_bond_shininess  = 128.0f;

	m_poly_ambient[0] = m_poly_ambient[1] = m_poly_ambient[2] = 0.25f; m_poly_ambient[3] = 1.0f;
	m_poly_diffuse[0] = m_poly_diffuse[1] = m_poly_diffuse[2] = 0.75f; m_poly_diffuse[3] = 1.0;
	m_poly_specular[0] = m_poly_specular[1] = m_poly_specular[2] = m_poly_specular[3] = 1.0f;
	m_poly_shininess  = 128.0f;

	m_plane_ambient[0] = m_plane_ambient[1] = m_plane_ambient[2] = 0.25f; m_plane_ambient[3] = 1.0f;
	m_plane_diffuse[0] = m_plane_diffuse[1] = m_plane_diffuse[2] = 0.75f; m_plane_diffuse[3] = 1.0;
	m_plane_specular[0] = m_plane_specular[1] = m_plane_specular[2] = m_plane_specular[3] = 1.0f;
	m_plane_shininess  = 128.0f;
//--------------------------------------------------- Initial Lights Settings
	for (i=0; i<16; i++) l_direction[i] = 0.0f;
	l_direction[0] = l_direction[5] = l_direction[10] = l_direction[15] = 1.0f;
	l_position[0] = -0.2f;l_position[1] = -0.2f;l_position[2] = 20;l_position[3] = 0;
//	l_position[0] = 10.0f;l_position[1] = 10.0f;l_position[2] = -10;l_position[3] = 1;
	l_dir[0] = l_dir[1] = l_dir[2] = -1;
	l_ambient[0] = l_ambient[1] = l_ambient[2] = 0.0f; l_ambient[3] = 1.0f;
	l_diffuse[0] = l_diffuse[1] = l_diffuse[2] = 0.7f; l_diffuse[3] = 1.0f;   
//--------------------------------------------------- Initial View Direction
	VA[0] = 0; VA[1] = 0; VA[2] = 1;
	VF = 0;
//--------------------------------------------------- Initial Eye distance
	eye_dist = 0;
//--------------------------------------------------- Rietan Export mode (0 - Izotropic; 1 - Anizotropic)
	export_mode = 0;
//--------------------------------------------------- Atom Spin Initial Settings
	spin_H = 1.0f;
	spin_RGB[0] = spin_RGB[1] = spin_RGB[2] = 0.8f;
//--------------------------------------------------- Depth-cueting Setting
	fog = 1.5f;
	fog_color[0] = BKCL[0];
	fog_color[1] = BKCL[1];
	fog_color[2] = BKCL[2];
	fog_color[3] = 1;

	CSSR_ID = 0;
}



//****************************************************************//
//                 Read Data From File                            //
//----------------------------------------------------------------//
// control ID = 0 -- VICS            File                         //
//              1 -- AMCSD           File                         //
//              2 -- Chem3D          File                         //
//              3 -- CIF             File                         //
//              4 -- Crystal Maker   File                         //
//              5 -- CRYSTIN         File                         //
//              6 -- CSSR            File                         //
//              7 -- FDAT/CSD        File                         //
//              8 -- Gaussian 98     File                         //
//              9 -- ICSD            File                         //
//             10 -- MDL mol         File                         //
//             11 -- MINCRYST        File                         //
//             12 -- MOLDA           File                         //
//             13 -- PDB             File                         //
//             14 -- Rietan *.ins    File                         //
//             15 -- Wien2k          File                         //
//             16 -- XMol XYZ        File                         //
//             17 -- SCAT            File                         //
//             18 -- MXDORTO         File                         //
//****************************************************************//
int readdata(int ID, char *fname, int PhN)
{
//	int i;
	FILE *mfile;
	int FileControl = -1;
//--- Initial Settings for Symmetry

	mfile=fopen(fname,"r");
	if( mfile == NULL ) {
		printf( "The file (%s) was not opened\n",fname );
		return(-1);
	}
	else {
		fclose(mfile);
		init_data_settings1();

//---------------------------------------------------
		switch(ID)
		{
//----------------------------------------------- VICS file
		case( 0):{bonds_type = 0; radii_type = 0; FileControl = readVICSfile(fname); break;}
//----------------------------------------------- AMC
		case( 1):{bonds_type = 0; radii_type = 1; FileControl = readAMCfile(fname); break;}
//----------------------------------------------- Chem3D
		case( 2):{bonds_type = 1; radii_type = 2; FileControl = readChem3Dfile(fname); break;}
//----------------------------------------------- CIF
		case( 3):{bonds_type = 0; radii_type = 0; FileControl = readCIFfile(fname, PhN); break;}
//----------------------------------------------- Crystal Maker
		case( 4):{bonds_type = 0; radii_type = 0; FileControl = readCMfile(fname); break;}
//----------------------------------------------- CRYSTIN
		case( 5):{bonds_type = 0; radii_type = 1; FileControl = readCRYfile(fname); break;}
//----------------------------------------------- CSSR
		case( 6):{bonds_type = 0; radii_type = 0; FileControl = readCSSRfile(fname); break;}
//----------------------------------------------- Cambridge Structural Data (FDAT/CSD)
		case( 7):{bonds_type = 0; radii_type = 0; FileControl = readFDATfile(fname); break;}
//----------------------------------------------- Gaussian 98
		case( 8):{bonds_type = 1; radii_type = 2; FileControl = readCUBEfile(fname); break;}
//----------------------------------------------- ICSD
		case( 9):{bonds_type = 0; radii_type = 1; FileControl = readICSDfile(fname); break;}
//----------------------------------------------- MDL mol
		case(10):{bonds_type = 1; radii_type = 2; FileControl = readMDLfile(fname); break;}
//----------------------------------------------- MINCRYST
		case(11):{bonds_type = 0; radii_type = 1; FileControl = readMINCRfile(fname); break;}
//----------------------------------------------- MOLDAXMol (mld)
		case(12):{bonds_type = 1; radii_type = 2; FileControl = readMOLDAfile(fname); break;}
//---------------------------------------------- PDB
		case(13):{bonds_type = 1; radii_type = 2; FileControl = readPDBfile(fname); break;}
//----------------------------------------------- RIETAN *.INS FILE
		case(14):{bonds_type = 0; radii_type = 0; FileControl = readINSfile(fname, PhN); break;}
//----------------------------------------------- WIEN2k
		case(15):{bonds_type = 0; radii_type = 0; FileControl = readWIENfile(fname); break;}
//----------------------------------------------- XMol (XYZ)
		case(16):{bonds_type = 0; radii_type = 0; FileControl = readXYZfile(fname); break;}
//----------------------------------------------- ASSE
		case(17):{bonds_type = 0; radii_type = 0; FileControl = readWIEN_ASSEfile(fname); break;}
//----------------------------------------------- SCAT
		case(18):{bonds_type = 0; radii_type = 0; FileControl = readSCATfile(fname); break;}
//----------------------------------------------- MXD
		case(19):{bonds_type = 0; radii_type = 0; FileControl = readMXDfile(fname); break;}
		}
		if (FileControl == 0)  {
			fog_color[0] = BKCL[0];
			fog_color[1] = BKCL[1];
			fog_color[2] = BKCL[2];
			fog_color[3] = 1;
			if (ID != 0) atom_type = 0;
			if (nPos == 0) {
				printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				printf("Warning!\n");
				printf("Change the space group name.\n");
				printf("Correct space group names are given in `Std. symbol`\n");
				printf("in the New or Edit Data Dialog Box.\n\n");
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				return(-1);
			}
		}
		if (CSSR_ID == 1) fileID = 2;
	}
	return(FileControl);
}


//****************************************************************//
//                 Get Atoms Radius and Colors                    //
//****************************************************************//
void get_atom_R_and_colors()
{
	int i,j;
	int control = 0;
	char *pos;

	for(i=0; i<nAtom; i++){
		control = 0;
		for(j=0; j<tAtom; j++){
			pos = strstr(AtomT[j].symb,lAtom[i].simb);
			if((pos != NULL) && (strlen(AtomT[j].symb) == strlen(lAtom[i].simb) ) ){
				lAtom[i].R = AtomT[j].R;
				lAtom[i].color[0] = AtomT[j].color[0];
				lAtom[i].color[1] = AtomT[j].color[1];
				lAtom[i].color[2] = AtomT[j].color[2];
				control = 1;
				break;
			}
		}
	}
	if ((tAtom == 0) || (control == 0)) setDefPar();
}

//*******************************************************************************
void calc_param_for_therm()
{
	double ar,br,cr,ca,cb,cg,sa,sb,sg,car,cbr,cgr,Vol,SS;

	SS = 1 - cos(cCell.a[3]*RAD)*cos(cCell.a[3]*RAD)
		    - cos(cCell.a[4]*RAD)*cos(cCell.a[4]*RAD)
		    - cos(cCell.a[5]*RAD)*cos(cCell.a[5]*RAD)
			+ 2*cos(cCell.a[3]*RAD)*cos(cCell.a[4]*RAD)*cos(cCell.a[5]*RAD);
	Vol = cCell.a[0] * cCell.a[1] * cCell.a[2] * sqrt(SS);

	ca = cos(cCell.a[3]*RAD);
	cb = cos(cCell.a[4]*RAD);
	cg = cos(cCell.a[5]*RAD);
	sa = sin(cCell.a[3]*RAD);
	sb = sin(cCell.a[4]*RAD);
	sg = sin(cCell.a[5]*RAD);
	ar = cCell.a[1]*cCell.a[2]*sa/Vol;
	br = cCell.a[0]*cCell.a[2]*sb/Vol;
	cr = cCell.a[0]*cCell.a[1]*sg/Vol;
	car = (cb*cg - ca)/(sb*sg);
	cbr = (ca*cg - cb)/(sa*sg);
	cgr = (ca*cb - cg)/(sa*sb);

	G[0] = ar*ar; 
	G[1] = br*br;
	G[2] = cr*cr;
	G[3] = ar*br*cgr; 
	G[4] = ar*cr*cbr;
	G[5] = br*cr*car;

	TR[0] = SPI2*ar*ar;
	TR[1] = SPI2*br*br;
	TR[2] = SPI2*cr*cr;
	TR[3] = SPI2*ar*br;
	TR[4] = SPI2*ar*cr;
	TR[5] = SPI2*br*cr;

//	TR[0] = ar*ar;
//	TR[1] = br*br;
//	TR[2] = cr*cr;
//	TR[3] = ar*br;
//	TR[4] = ar*cr;
//	TR[5] = br*cr;

	RVS.a[0][0] = 1.0f;           
	RVS.a[0][1] =  0.0f;                 
	RVS.a[0][2] = 0.0f;

	RVS.a[1][0] = (float)cg; 
	RVS.a[1][1] = (float)sg; 
	RVS.a[1][2] = 0.0f;
	
	RVS.a[2][0] = (float)cb; 
	RVS.a[2][1] = -(float)(sb*car);    
	RVS.a[2][2] = (float)(1.0/(cCell.a[2]*cr));

	RTS = TM(RVS);
}


//*******************************************************************************
void check_iso_uniso_parameters()
{
	int i,j;
	float B[6], BB;
	double D,Vol,cad,cbd,cgd,sad,sbd,sgd,car,cbr,cgr,ar,br,cr,ad,bd,cd;

	ad = cCell.a[0];
	bd = cCell.a[1];
	cd = cCell.a[2];
	cad = cos(cCell.a[3]*RAD); sad = sin(cCell.a[3]*RAD); 
	cbd = cos(cCell.a[4]*RAD); sbd = sin(cCell.a[4]*RAD);
	cgd = cos(cCell.a[5]*RAD); sgd = sin(cCell.a[5]*RAD);

	Vol = ad*bd*cd*sqrt(1.0 - cad*cad - cbd*cbd - cgd*cgd + 2.0*cad*cbd*cgd);
		
	car = (cbd*cgd - cad)/(sbd*sgd);
	cbr = (cad*cgd - cbd)/(sad*sgd);
	cgr = (cad*cbd - cgd)/(sad*sbd);

	ar = bd*cd*sad/Vol;
	br = ad*cd*sbd/Vol;
	cr = ad*bd*sgd/Vol;

	for(i=0; i<nAtom; i++){
		switch(use_aniso){
		case(0):{if (lAtom[i].Biso == 0) lAtom[i].Biso = 1.0;break;}
		case(1):{
			BB = lAtom[i].Biso;
			for(j=0; j<6; j++) B[j] = lAtom[i].B[j];
			if (BB == 0) {
				if ((B[0] != 0) && (B[1] != 0) && (B[2] != 0)){
					if (Btype == 0) {
						B[0] *= (float)(ar*ar);
						B[1] *= (float)(br*br);
						B[2] *= (float)(cr*cr);
						B[3] *= (float)(ar*br);
						B[4] *= (float)(ar*cr);
						B[5] *= (float)(br*cr);
					}
					D = B[0]*ad*ad + B[1]*bd*bd + B[2]*cd*cd +
						2.0*(B[3]*ad*bd*cgd + B[4]*ad*cd*cbd + B[5]*bd*cd*cad);
					BB = (float)(D/(3.0*SPI2));
					if (use_iso == 0) BB *= (float)SPI8;
//					BB = (float)(4.0*D/3.0);
//					if (use_iso == 1) BB /= (float)SPI8;
				}
				else BB = 1.0;
				lAtom[i].Biso = BB;
			}
			else {
				if ((B[0] == 0) || (B[1] == 0) || (B[2] == 0)){
//					B[0] = (float)(0.25*BB*ar*ar/SPI2);
//					B[1] = (float)(0.25*BB*br*br/SPI2);
//					B[2] = (float)(0.25*BB*cr*cr/SPI2);
//					B[3] = (float)(0.25*BB*ar*br*cgr/SPI2);
//					B[4] = (float)(0.25*BB*ar*cr*cbr/SPI2);
//					B[5] = (float)(0.25*BB*br*cr*car/SPI2);
					B[0] = (float)(0.25*BB*ar*ar);
					B[1] = (float)(0.25*BB*br*br);
					B[2] = (float)(0.25*BB*cr*cr);
					B[3] = (float)(0.25*BB*ar*br*cgr);
					B[4] = (float)(0.25*BB*ar*cr*cbr);
					B[5] = (float)(0.25*BB*br*cr*car);
					if (use_iso == 1) BB *= (float)SPI8;
					if (Btype == 0) {
						B[0] /= (float)(ar*ar);
						B[1] /= (float)(br*br);
						B[2] /= (float)(cr*cr);
						B[3] /= (float)(ar*br);
						B[4] /= (float)(ar*cr);
						B[5] /= (float)(br*cr);
					}
					for(j=0; j<6; j++) lAtom[i].B[j] = B[j];
				}
			}
			break;
				}
		}
	}
}



void check_iso_uniso_parameters1()
{
	int i,j;
	double dif;
	float R;

	dif = 0.25;
	if (use_iso == 1) dif = SPI8/4.0;
	for (i=0; i<nAtom; i++){
		if ((lAtom[i].B[0] == 0.001f) && (lAtom[i].B[1] == 0.001f) && (lAtom[i].B[2] == 0.001f) &&
			(lAtom[i].B[3] == 0.0f) && (lAtom[i].B[4] == 0.0f) && (lAtom[i].B[5] == 0.0f) &&
			(lAtom[i].Biso != 0.0f)){
			for(j=0; j<6; j++) {
				switch(Btype)
				{
				case(0):{ //----- Uij
					R = (float)(G[j]*dif/TR[j]);
					lAtom[i].B[j] = lAtom[i].Biso*R;
					break;
						}
				case(1):{ //----- beta(ij)
					R = (float)(G[j]*dif);
					lAtom[i].B[j] = lAtom[i].Biso*R;
					break;
						}
				}
			}

		}
	}
}


//*******************************************************************************
int checkDublicate(float x, float y, float z)
{
	int k;
	int ID = 0;
	for (k=0; k<nXYZ; k++){
		if (x==m[k].x && y==m[k].y && z==m[k].z) ID = -1;
	}
	return(ID);
}

//****************************************************************//
int Inside(float a, float b, float c)
{
	int ID = -1;
	if ((a>=range[0]) && (a<=range[1]) &&
		(b>=range[2]) && (b<=range[3]) &&
		(c>=range[4]) && (c<=range[5])) ID = 0;
	return(ID);
}


//****************************************************************//
int CheckRange(float x, float y, float z, float v[3])
{
	int i,j,k;
	int ID;
	float nx,ny,nz;

	v[0] = 0.0; v[1] = 0.0; v[2] = 0.0;
	i = 0; j = 0; k = 0;
	ID = 0;
	if (Inside(x,y,z)!=0){
		ID = -1;
		for (i=-1;i<2;i++) {
			for(j=-1;j<2;j++){
				for(k=-1;k<2;k++){
					nx = x + i;
					ny = y + j;
					nz = z + k;
					if (Inside(nx,ny,nz)==0) {ID = 0;}
					if (ID==0) break;
				}
				if (ID==0) break;
			}
			if (ID==0) break;
		}
	}
	if (ID==0) {v[0] = (float)i; v[1] = (float)j; v[2] = (float)k;}
	return(ID);
}


//*******************************************************************************
void addAtom(int Ind, float x, float y, float z, int Sym)
{
	int i,j,k;
	int dub_ID;
	float dx,dy,dz;
	int iMin,iMax,jMin,jMax,kMin,kMax;

	iMin = -1; iMax = 2;
	jMin = -1; jMax = 2;
	kMin = -1; kMax = 2;

	if (range[0]<=-1)  iMin = (int)range[0] - 1;
	if (range[0]>= 1)  iMin = (int)range[0] + 1;
	if (range[1]<=-1)  iMax = (int)range[1] + 2;
	if (range[1]>= 1)  iMax = (int)range[1] + 2;

	if (range[2]<=-1)  jMin = (int)range[2] - 1;
	if (range[2]>= 1)  jMin = (int)range[2] + 1;
	if (range[3]<=-1)  jMax = (int)range[3] + 2;
	if (range[3]>= 1)  jMax = (int)range[3] + 2;

	if (range[4]<=-1)  kMin = (int)range[4] - 1;
	if (range[4]>= 1)  kMin = (int)range[4] + 1;
	if (range[5]<=-1)  kMax = (int)range[5] + 2;
	if (range[5]>= 1)  kMax = (int)range[5] + 2;


	for(i=iMin;i<iMax;i++){
		for(j=jMin;j<jMax;j++){
			for(k=kMin;k<kMax;k++){
				dx = x + i;
				dy = y + j;
				dz = z + k;
				if (Inside(dx,dy,dz)==0){
					dub_ID = checkDublicate(dx,dy,dz);
					if(dub_ID==0){
						nXYZ += 1;
						if (nXYZ <= MAX_NUM_OF_ATOMS){
							m[nXYZ-1].x=dx; m[nXYZ-1].y=dy; m[nXYZ-1].z=dz;
							m[nXYZ-1].Index = Ind;
							m[nXYZ-1].Num = m[nXYZ-1].Index;
							m[nXYZ-1].show = 'y';
							m[nXYZ-1].bondNum = 0;
							m[nXYZ-1].bondatom = 0;
							m[nXYZ-1].polyNum = 0;
							m[nXYZ-1].symnum = Sym;
						}
						else {
							printf("======================================================\n");
							printf("Total Number of Atoms out of range\n nXYZ=%i\n MAX_NUM_OF_ATOMS=%i\n",
							       nXYZ,MAX_NUM_OF_ATOMS);
							printf("======================================================\n");
							getchar();
							exit(0);
						}
					}
				}
			}
		}
	}
}

//****************************************************************//
//                 Get Atomic Positions                           //
//****************************************************************//
void multiXYZ()
{
	int i,j;
	int dub_ID,check_ID;
	float x,y,z;
	float x0,y0,z0;

	float trans[3];

	nXYZ = 0;
	for(i=0;i<nAtom;i++){
		for(j=0;j<nPos;j++){
			x0 = lAtom[i].x; y0 = lAtom[i].y; z0 =  lAtom[i].z;
			if ((cCell.SpGrN >= 16) && (cCell.SpGrN <= 74)){
				switch(cCell.Setting)
				{
				case(1):{x0 = lAtom[i].x; y0 = lAtom[i].y; z0 =  lAtom[i].z;break;}
				case(2):{x0 = lAtom[i].y; y0 = lAtom[i].x; z0 = -lAtom[i].z;break;}
				case(3):{x0 = lAtom[i].y; y0 = lAtom[i].z; z0 =  lAtom[i].x;break;}
				case(4):{x0 = lAtom[i].z; y0 = lAtom[i].y; z0 = -lAtom[i].x;break;}
				case(5):{x0 = lAtom[i].z; y0 = lAtom[i].x; z0 =  lAtom[i].y;break;}
				case(6):{x0 = lAtom[i].x; y0 = lAtom[i].z; z0 = -lAtom[i].y;break;}
				}
			}
			x=sPos[j].t[0]+
			     x0*sPos[j].a[0][0]+y0*sPos[j].a[1][0]+z0*sPos[j].a[2][0];
			y=sPos[j].t[1]+
			     x0*sPos[j].a[0][1]+y0*sPos[j].a[1][1]+z0*sPos[j].a[2][1];
			z=sPos[j].t[2]+
			     x0*sPos[j].a[0][2]+y0*sPos[j].a[1][2]+z0*sPos[j].a[2][2];
			x0 = x; y0 = y; z0 = z;
			if ((cCell.SpGrN >= 16) && (cCell.SpGrN <= 74)){
				switch(cCell.Setting)
				{
				case(1):{x0 =  x; y0 = y; z0 =  z; break;}
				case(2):{x0 =  y; y0 = x; z0 = -z; break;}
				case(3):{x0 =  z; y0 = x; z0 =  y; break;}
				case(4):{x0 = -z; y0 = y; z0 =  x; break;}
				case(5):{x0 =  y; y0 = z; z0 =  x; break;}
				case(6):{x0 =  x; y0 =-z; z0 =  y; break;}
				}
			}
			x = x0; y = y0; z = z0;
			check_ID = CheckRange(x, y, z, trans);
			if (check_ID==0){
				x += trans[0];
				y += trans[1];
				z += trans[2];
				dub_ID = checkDublicate(x,y,z);
				if(dub_ID==0){
					nXYZ++;
					if (nXYZ <= MAX_NUM_OF_ATOMS){
						m[nXYZ-1].x=x; m[nXYZ-1].y=y; m[nXYZ-1].z=z;
						m[nXYZ-1].Index = i;
						m[nXYZ-1].Num = m[nXYZ-1].Index;
						m[nXYZ-1].show = 'y';
						m[nXYZ-1].bondNum = 0;
						m[nXYZ-1].bondatom = 0;
						m[nXYZ-1].polyNum = 0;
						m[nXYZ-1].symnum = j;
					}
					else {
						printf("======================================================\n");
						printf("Total Number of Atoms out of range\n nXYZ=%i\n MAX_NUM_OF_ATOMS=%i\n",
							   nXYZ,MAX_NUM_OF_ATOMS);
						printf("======================================================\n");
						getchar();
						exit(0);
					}
					addAtom(i,x,y,z,j);
				}
			}
			addAtom(i,x,y,z,j);
		}
	}
}


//****************************************************************//
//                 Set Orthogonal Matrix                          //
//****************************************************************//
void setOrthoMatrix()
{
	double a,b,c;
	double cosA,cosB,cosG,sinA,sinB,sinG;
	double cosAR,sinAR;
	double V;
	a = cCell.a[0];
	b = cCell.a[1];
	c = cCell.a[2];
	cosA = cos(cCell.a[3]*PI/180); sinA = sqrt(1 - cosA*cosA);
	cosB = cos(cCell.a[4]*PI/180); sinB = sqrt(1 - cosB*cosB);
	cosG = cos(cCell.a[5]*PI/180); sinG = sqrt(1 - cosG*cosG);
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


//****************************************************************//
//                 Convert to Orthogonal XYZ                      //
//****************************************************************//
void setOrthoXYZ()
{
	int i;
	double x,y,z,x1,y1,z1;

	for(i=0; i<nXYZ; i++) {
		x = m[i].x*cCell.a[0];
		y = m[i].y*cCell.a[1];
		z = m[i].z*cCell.a[2];
		x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
		y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
		z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
		m[i].x = (float)x1/cCell.max_P;
		m[i].y = (float)y1/cCell.max_P;
		m[i].z = (float)z1/cCell.max_P;
	}
}

//****************************************************************//
//                 Set Origing                                    //
//****************************************************************//
void setOrigin()
{
	float dr1,dr2,dr3;
	dr1 = range[1]-range[0];
	dr2 = range[3]-range[2];
	dr3 = range[5]-range[4];
	origin[0] = (-range[0] - dr1/2)*(cCell.ortho_a[0]/cCell.max_P);
	origin[1] = (-range[2] - dr2/2)*(cCell.ortho_a[1]/cCell.max_P);
	origin[2] = (-range[4] - dr3/2)*(cCell.ortho_a[2]/cCell.max_P);
}

//****************************************************************//
void get_ortho_XYZ()
{
	int i;

	for(i=0; i<nAtom; i++){
		m[i].Index = m[i].Num = i;
		m[i].x = lAtom[i].x;
		m[i].y = lAtom[i].y;
		m[i].z = lAtom[i].z;
		m[i].show = 'y';
		m[i].show_spin = 0;
		m[i].spin_dir[0] = 0;
		m[i].spin_dir[1] = 0;
		m[i].spin_dir[2] = 1;
	}
	nXYZ = nAtom;
}

void get_ortho_origin()
{
	int i;
	float maxX, maxY, maxZ;
	float minX, minY, minZ;

	maxX = maxY = maxZ = -100000;
	minX = minY = minZ =  100000;

	for(i=0; i<nAtom; i++){
		maxX = (maxX >= lAtom[i].x) ? maxX : lAtom[i].x;
		maxY = (maxY >= lAtom[i].y) ? maxY : lAtom[i].y;
		maxZ = (maxZ >= lAtom[i].z) ? maxZ : lAtom[i].z;
		
		minX = (minX <= lAtom[i].x) ? minX : lAtom[i].x;
		minY = (minY <= lAtom[i].y) ? minY : lAtom[i].y;
		minZ = (minZ <= lAtom[i].z) ? minZ : lAtom[i].z;
	}
	origin[0] = -(maxX + minX) / 2;
	origin[1] = -(maxY + minY) / 2;
	origin[2] = -(maxZ + minZ) / 2;
}


//*******************************************************************************
void setOrthoUCell_and_UAxis()
{
	int i;
	double x,y,z,x1,y1,z1;

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

	for(i=0; i<8; i++) {
		x = uCell[i][0]*cCell.a[0];
		y = uCell[i][1]*cCell.a[1];
		z = uCell[i][2]*cCell.a[2];
		x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
		y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
		z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
		uCell[i][0] = (float)x1/cCell.max_P;
		uCell[i][1] = (float)y1/cCell.max_P;
		uCell[i][2] = (float)z1/cCell.max_P;
	}
}

//*******************************************************************************
void setOrthoABC()
{
	double x1,y1,z1;

	x1 = cCell.a[0]*ortho_M[0][0] + cCell.a[1]*ortho_M[0][1] + cCell.a[2]*ortho_M[0][2];
	y1 = cCell.a[0]*ortho_M[1][0] + cCell.a[1]*ortho_M[1][1] + cCell.a[2]*ortho_M[1][2];
	z1 = cCell.a[0]*ortho_M[2][0] + cCell.a[1]*ortho_M[2][1] + cCell.a[2]*ortho_M[2][2];
	cCell.ortho_a[0] = (float)x1;
	cCell.ortho_a[1] = (float)y1;
	cCell.ortho_a[2] = (float)z1;
}

//****************************************************************//
//                 Set Orthogonal Unit Cell                       //
//****************************************************************//
void set_ortho_cell()
{
	uCell[0][0] = 0; uCell[0][1] = 0; uCell[0][2] = 0;
	uCell[1][0] = 1; uCell[1][1] = 0; uCell[1][2] = 0;
	uCell[2][0] = 1; uCell[2][1] = 0; uCell[2][2] = 1;
	uCell[3][0] = 0; uCell[3][1] = 0; uCell[3][2] = 1;
	uCell[4][0] = 0; uCell[4][1] = 1; uCell[4][2] = 1;
	uCell[5][0] = 1; uCell[5][1] = 1; uCell[5][2] = 1;
	uCell[6][0] = 1; uCell[6][1] = 1; uCell[6][2] = 0;
	uCell[7][0] = 0; uCell[7][1] = 1; uCell[7][2] = 0;

	uAxis[0][0] = 0.08f; uAxis[0][1] = 0;     uAxis[0][2] = 0;
	uAxis[1][0] = 0;     uAxis[1][1] = 0.08f; uAxis[1][2] = 0;
	uAxis[2][0] = 0;     uAxis[2][1] = 0;     uAxis[2][2] = 0.08f;

	setOrthoABC();
	cCell.max_P = maxf(cCell.ortho_a[0],cCell.ortho_a[1],cCell.ortho_a[2]);
	setOrthoUCell_and_UAxis();
}


//*******************************************************************************
int checkBondsDub(int I1, int I2)
{
	int i;
	for (i=0; i<nBonds; i++){
		if ((I1 == bAtom[i].Atom1) && (I2 == bAtom[i].Atom2)) return 1;
		if ((I1 == bAtom[i].Atom2) && (I2 == bAtom[i].Atom1)) return 1;
	}
	return 0;
}


//****************************************************************//
//                 Search Bonds                                   //
//****************************************************************//
void get_bond_search_mode()
{
	int i;
	char *par1, *par2;

	if (bond_sNum > 0){
		for(i=0; i<bond_sNum; i++){
			if (bini[i].search_mode == -1){
				bini[i].search_mode = 0;
				bini[i].boundary_mode = 1;
				bini[i].show_poly_mode = 1;
				par1 = "";
				par2 = "";
				par1 = bini[i].A1;
				par2 = bini[i].A2;
				if (par1[0] == ' ') par1++;
				if (par2[0] == ' ') par2++;
				if ((strstr(par1, "XX") != NULL) && (strstr(par2, "XX") != NULL)) {
					bini[i].search_mode = 2;
					bini[i].show_poly_mode = 0;
				}
				else {
					par1 = "";
					par2 = "";
					par1 = bini[i].A1;
					par2 = bini[i].A2;
					if (par1[0] == ' ') par1++;
					if (par2[0] == ' ') par2++;
					if (strstr(par2, "XX") != NULL) bini[i].search_mode = 1;
				}
			}
		}
	}
}

//****************************************************************//
//                 Search Bonds                                   //
//****************************************************************//
void searchBonds()
{
	int i,j,k;
	float dx,dy,dz;
	double dist,fi;
	char *pos;
	char *par1,*par2;
	int I1, I2;
	int Num;
	int check;
	int h_bond_check;

//---------------------------------------------------------------------
	for(i=0; i<nXYZ; i++){
		m[i].bondNum = 0;
//		m[i].bondatom = 0;
	}
//---------------------------------------------------------------------
	nBonds = 0;
	for(i=0; i<bond_sNum; i++){
		par1 = bini[i].A1;
		par2 = bini[i].A2;
		if (par1[0] == ' ') par1++;
		if (par2[0] == ' ') par2++;
		for(j=0; j<nXYZ; j++){
			Num = 0;
			I1 = m[j].Num;
			check = 1;
			if (bini[i].search_mode != 2) {
				pos = strstr(lAtom[I1].simb, par1);
				if ((pos != NULL) && (strlen(par1) == strlen(lAtom[I1].simb))) check = 1;
				else check = -1;
			}
			if (bini[i].search_mode == 2) {
				if ((strstr(lAtom[I1].simb, "H") != NULL) && (strlen(lAtom[I1].simb) == 1)) check = -1;
				if ((strstr(lAtom[I1].simb, "D") != NULL) && (strlen(lAtom[I1].simb) == 1)) check = -1;
			}
			if (check > 0) {
				h_bond_check = 0;
				if ((strstr(lAtom[I1].simb, "H") != NULL) && (strlen(lAtom[I1].simb) == 1)) h_bond_check = 1;
				if ((strstr(lAtom[I1].simb, "D") != NULL) && (strlen(lAtom[I1].simb) == 1)) h_bond_check = 1;
				for(k=0; k<nXYZ; k++){
					I2 = m[k].Num;
					check = 1;
					if (bini[i].search_mode == 0) {
						pos = strstr(lAtom[I2].simb, par2);
						if((pos != NULL) && (strlen(par2) == strlen(lAtom[I2].simb))) check = 1;
						else check = -1;
						if (m[k].show == 'n') check = -1;
					}
					if (h_bond_check == 1){
						if ((strstr(lAtom[I2].simb, "H") != NULL) && (strlen(lAtom[I2].simb) == 1)) check = -1;
						if ((strstr(lAtom[I2].simb, "D") != NULL) && (strlen(lAtom[I2].simb) == 1)) check = -1;
					}
					if (check > 0){
						dx = (m[j].x - m[k].x)*cCell.max_P;
						dy = (m[j].y - m[k].y)*cCell.max_P;
						dz = (m[j].z - m[k].z)*cCell.max_P;
						dist = sqrt(dx*dx + dy*dy + dz*dz);
						if ((dist<=bini[i].d) && (dist > 0)){
							if (checkBondsDub(j,k) == 0){
								if (nBonds == MAX_NUM_OF_BONDS){
									printf("======================================================\n");
									printf("Number of Bonds out of Range\n nBonds=%i\n MAX_NUM_OF_BONDS=%i\n",
										   nBonds+1,MAX_NUM_OF_BONDS);
									printf("======================================================\n");
									getchar();
									exit(0);
								}
								dx = (m[k].x - m[j].x); 
								dy = (m[k].y - m[j].y);
								dz = (m[k].z - m[j].z);
								dist = sqrt(dx*dx + dy*dy + dz*dz);
								fi = acos(dz/dist)*180.0/PI;
								if (dx==0 && dy==0) { 
									if (dz>0) {fi = 0;} 
									else {fi = 180;}
								}
								bAtom[nBonds].Atom1 = j;
								bAtom[nBonds].Atom2 = k;
								bAtom[nBonds].dist = dist;
								bAtom[nBonds].FI = fi;
								bAtom[nBonds].show = 'y';
								bAtom[nBonds].hydr = 'n';
								if (bini[i].search_mode == 2) {
									m[j].bondNum = 0;
									m[k].bondNum = 0;
								}
								if (bini[i].search_mode != 2) {
									m[j].b_list[Num] = k;
									m[j].bondNum++;
									Num++;
								}
								m[k].bondatom = 1;
								nBonds += 1;
							}
							else {
								m[k].bondatom = 1;
							}
						}
					}
				}
			}
		}
	}

//---- Check not bonded atoms
	if (nBonds > 0) {
		for (j=0; j<bond_sNum; j++){
			par1 = bini[j].A2;
			if (par1[0] == ' ') par1++;
			for(i=0; i<nXYZ; i++){
				I1 = m[i].Num;
				pos = strstr(lAtom[I1].simb, par1);
				if ((pos != NULL) && (strlen(lAtom[I1].simb) == strlen(par1))){
					if ((m[i].bondNum == 0) && (m[i].bondatom == 0)) {
						m[i].show = 'n';
						for(k=0; k<m[i].bondNum; k++) m[k].show = 'n';
					}
				}
			}
		}
//--- Check bonds
		for(i=0; i<nBonds; i++){
			I1 = bAtom[i].Atom1;
			I2 = bAtom[i].Atom2;
			par1 = lAtom[m[I1].Num].simb;
			par2 = lAtom[m[I2].Num].simb;
			if (par1[0] == ' ') par1++;
			if (par2[0] == ' ') par2++;
			if (par1 != par2) {
				if (m[I1].show == 'n'){
					for(j=0; j<m[I1].bondNum; j++) {m[j].show = 'n';m[j].bondatom = 0;}
				}
			}
			if ((m[I1].show == 'n') || (m[I2].show == 'n')) bAtom[i].show = 'n';
		}
	}
}

//*************************************************************************************
int search_hydr_bond()
{
	int i,I1,I2,I3,I4,Num;
	float d;
	int h_check;

	Num = 0;
	for(i=0; i<nBonds; i++){
		I1 = bAtom[i].Atom1;
		I2 = m[I1].Num;
		I3 = bAtom[i].Atom2;
		I4 = m[I3].Num;
		h_check = 0;
		if ((strstr(lAtom[I2].simb, "H") != NULL) && (strlen(lAtom[I2].simb) == 1)) h_check++;
		if ((strstr(lAtom[I4].simb, "H") != NULL) && (strlen(lAtom[I4].simb) == 1)) h_check++;
		if ((strstr(lAtom[I2].simb, "D") != NULL) && (strlen(lAtom[I2].simb) == 1)) h_check++;
		if ((strstr(lAtom[I4].simb, "D") != NULL) && (strlen(lAtom[I4].simb) == 1)) h_check++;
		if (h_check > 0){
			bAtom[i].hydr = 'b';
			d = (float)(bAtom[i].dist * cCell.max_P);
			if (d > 1.1f) {
				bAtom[i].hydr = 'h'; 
				Num++;
			}
		}
	}
	return(Num);
}

//*************************************************************************************
int jacobi(float W[3][3], int n)
{
	int i,ip,iq,j;
	float b[3],z[3];
	int nrot;
	float sm,tresh,g,h,theta,t,c,s;
	float tau;
	float d[3];

	for(ip=0; ip<n; ip++){
		for(iq=0; iq<n; iq++) VV[ip][iq] = 0;
		VV[ip][ip] = 1.0f;
	}
	for(ip=0; ip<n; ip++){
		b[ip] = W[ip][ip];
		d[ip] = b[ip];
		z[ip] = 0.0f;
	}
	nrot = 0;
	for(i=1; i<=50; ++i){
		sm = 0.0;
		for(ip=0; ip<n-1; ip++){
			for(iq=ip+1; iq<n; iq++) sm = sm + (float)fabs(W[ip][iq]);
		}
		if (sm == 0) {
			V[0] = d[0];
			V[1] = d[1];
			V[2] = d[2];
			return(0);
		}
		if (i < 4 ) tresh = 0.2f * sm / (n*n);
		else tresh = 0.0f;
		for(ip=0; ip<n-1; ip++){
			for(iq=ip+1; iq<n; iq++){
				g = (float)(100.0*fabs(W[ip][iq]));
				if ( (i > 4) && ((fabs(d[ip])+g) == fabs(d[ip])) && ((fabs(d[iq])+g) == fabs(d[iq]))) {
					W[ip][iq] = 0.0;
				}
				else if(fabs(W[ip][iq]) > tresh) {
					h = d[iq] - d[ip];
					if((fabs(h)+g) == fabs(h)) {
						t = W[ip][iq]/h;
					}
					else {
						theta = 0.5f * h / W[ip][iq];
						t = 1.0f/(float)((fabs(theta) + sqrt(1.0 + theta*theta)));
						if (theta < 0) t = -t;
					}
					c = 1.0f/(float)(sqrt(1.0 + t*t));
					s = t * c;
					tau = s/(1.0f + c);
					h = t * W[ip][iq];
					z[ip] = z[ip] - h;
					z[iq] = z[iq] + h;
					d[ip] = d[ip] - h;
					d[iq] = d[iq] + h;
					W[ip][iq] = 0.0f;
					if(ip > 0) {
//						for(j=0; j<ip-1; j++) {
						for(j=0; j<ip; j++) {
							g = W[j][ip];
							h = W[j][iq];
							W[j][ip] = g - s * (h + g * tau);
							W[j][iq] = h + s * (g - h * tau);
						}
					}
					if ((ip == 0) && (iq == 2)) {
						for(j=ip+1; j<iq; j++){
							g = W[ip][j];
							h = W[j][iq];
							W[ip][j] = g - s * (h + g * tau);
							W[j][iq] = h + s * (g - h * tau);
						}
					}
					if (iq < 2) {
						for(j=iq+1; j<3; j++){
							g = W[ip][j];
							h = W[iq][j];
							W[ip][j] = g - s * (h + g * tau);
							W[iq][j] = h + s * (g - h * tau);
						}
					}
					for(j=0; j<3; j++){
						g = VV[j][ip];
						h = VV[j][iq];
						VV[j][ip] = g - s * (h + g * tau);
						VV[j][iq] = h + s * (g - h * tau);
					}
					nrot = nrot + 1;
				}
			}
		}
		for(ip=0; ip<3; ip++){
			b[ip] = b[ip] + z[ip];
			d[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	return(-1);
}



//****************************************************************//
//                 Make Thermal Ellipsoids                        //
//****************************************************************//
int make_th_ellips(int N)
{
	int I1,I2,i,j;
	MATRIX S,RS,T,ST;
	float W[3][3];
	int ID;

	ID = -1;
	I1 = m[N].symnum;
	I2 = m[N].Num;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) S.a[i][j] = (float)sPos[I1].a[j][i];
	}
	RS = RM(S);
	for(i=0; i<16; i++) termMatrix[i] = 0;
	termMatrix[0] = termMatrix[5] = termMatrix[10] = termMatrix[15] = 1.0f;

	if (Btype == 0) { //-------------------- Convert Uij to betaij
		T.a[0][0] = (float)(lAtom[I2].B[0]*TR[0]);
		T.a[1][1] = (float)(lAtom[I2].B[1]*TR[1]);
		T.a[2][2] = (float)(lAtom[I2].B[2]*TR[2]);
		T.a[0][1] = T.a[1][0] = (float)(lAtom[I2].B[3]*TR[3]);
		T.a[0][2] = T.a[2][0] = (float)(lAtom[I2].B[4]*TR[4]);
		T.a[1][2] = T.a[2][1] = (float)(lAtom[I2].B[5]*TR[5]);
	}
	else {
		T.a[0][0] = lAtom[I2].B[0];
		T.a[1][1] = lAtom[I2].B[1];
		T.a[2][2] = lAtom[I2].B[2];
		T.a[0][1] = T.a[1][0] = lAtom[I2].B[3];
		T.a[0][2] = T.a[2][0] = lAtom[I2].B[4];
		T.a[1][2] = T.a[2][1] = lAtom[I2].B[5];
	}
//---------------------------------- Convert T matrix to orthogonal system
	ST = MM(T,RVS);  
	ST = MM(RTS,ST);
//---------------------------------- Make symmetry operation
	ST = MM(ST,RS);
	ST = MM(S,ST);

	W[0][0] = ST.a[0][0]/(float)TR[0]; W[0][1] = ST.a[0][1]/(float)TR[3]; W[0][2] = ST.a[0][2]/(float)TR[4];
	W[1][0] = ST.a[1][0]/(float)TR[3]; W[1][1] = ST.a[1][1]/(float)TR[1]; W[1][2] = ST.a[1][2]/(float)TR[5];
	W[2][0] = ST.a[2][0]/(float)TR[4]; W[2][1] = ST.a[2][1]/(float)TR[5]; W[2][2] = ST.a[2][2]/(float)TR[2];

	ID = jacobi(W, 3);
	if (ID == 0) {
		for(j=0; j<3; j++) ellAtom[I2].axis[j] = (float)sqrt(V[j]);
		termMatrix[0]  = VV[0][0]; termMatrix[1]  = VV[1][0]; termMatrix[2]  = VV[2][0];
		termMatrix[4]  = VV[0][1]; termMatrix[5]  = VV[1][1]; termMatrix[6]  = VV[2][1];
		termMatrix[8]  = VV[0][2]; termMatrix[9]  = VV[1][2]; termMatrix[10] = VV[2][2];
	}
	return(ID);
}

//****************************************************************//
//             Get Number of Coordination Polyhedras              //
//****************************************************************//
void CalcPolyNum()
{
	int i,j,k,I2,I1;
	char *par1,*pos;
	int check;

	nPoly = 0;
	for(j=0; j<nXYZ; j++) m[j].poly_show = -1;
	for(j=0; j<nXYZ; j++){
		I1 = m[j].Num;
		check = 1;
		for(i=0; i<bond_sNum; i++){
			par1 = bini[i].A1;
			if (par1[0] == ' ') par1++;
			pos = strstr(lAtom[I1].simb, par1);
			if ((pos != NULL) && (strlen(par1) == strlen(lAtom[I1].simb))) {
				if (bini[i].show_poly_mode == 0) check = 0;
				break;
			}
		}
		if ((check == 1) && (m[j].bondNum == lAtom[I1].CN) && (m[j].bondNum > 2)){
			if (nPoly == MAX_POLYHEDRA){
				printf("======================================================\n");
				printf("Number of Polyhedra out of Range\n nPoly=%i\n MAX_POLYHEDRA=%i\n",
					   nPoly+1,MAX_POLYHEDRA);
				printf("======================================================\n");
				getchar();
				exit(0);
			}
			m[j].poly_show = 1;
			poly_list[nPoly] = j;
			for(k=0; k<m[j].bondNum; k++) {
				I2 = m[j].b_list[k];
				m[I2].poly_show = 1;
			}
			nPoly++;
		}
	}
}


//****************************************************************//
//                 Get Coordination Number                        //
//****************************************************************//
void CalcCN()
{
	int i,j;

	for(j=0; j<nAtom; j++){
		lAtom[j].CN = 0;
		for(i=0; i<nXYZ; i++) {
			if ((m[i].Num == j) && (m[i].bondNum > 0)) {
				lAtom[j].CN = (m[i].bondNum >= lAtom[j].CN) ? m[i].bondNum : lAtom[j].CN;
				break;
			}
		}
	}
}



//****************************************************************//
//                 Make Polyhedra                                 //
//****************************************************************//


//***************************************************************************//
int check_dubl_poly_face(int N[MAX_PFV], int N_list)
{
	int i,j,k,ID;

    ID = 0;
	for(i=0;i<p.fN;i++){
		if (N_list==p.face[i].vN){
			ID = 0;
			for(j=0;j<p.face[i].vN;j++){
				for(k=0;k<N_list;k++){
					if(N[k]==face_list[i][j]) ID += 1;
				}
			}
		}
		if (ID == N_list) break;
		else ID = 0;
	}
	return(ID);
}


//***************************************************************************//
int find_next_v1(float s1[3], float s[MAX_PFV][3], int numS)
{
	int i;
	float min_dist = 1000000;
	float r,d1,d2,d3;
	int ID = 0;
	for(i=0;i<numS;i++){
		d1 = s1[0]-s[i][0];
		d2 = s1[1]-s[i][1];
		d3 = s1[2]-s[i][2];
		r = (float)sqrt(d1*d1 + d2*d2 + d3*d3);
		if (r<min_dist) {
			min_dist = r;
			ID = i;
		}
	}
	return(ID);
}

//***************************************************************************//
void sort_poly_face(float v[3][MAX_BONDS_FOR_ATOM])
{
	int tmp_n[MAX_PFV];
	int tmp_n_list;
	float s1[3],s[MAX_V][3];
	int i,j,m,k;
	int ID, Index;
	int I1,I2,I3;
	float dd1[3],dd2[3];
	float a[3];
	float vol;
	
	I1 = 0;
	face_list[p.fN][I1] = n[0];
	I1 += 1;
    p.face[p.fN].vN = I1;
    tmp_n_list = n_list - 1;

	for(k=1;k<n_list;k++){
		m = 0;
		for(i=0;i<n_list;i++){
			ID = 0;
			for(j=0;j<I1;j++){
				if(n[i]==face_list[p.fN][j]) ID = 1;
			}
			if (ID==0){
				tmp_n[m] = n[i];
				m += 1;
			}
		}
		for(i=0;i<tmp_n_list;i++){
			s[i][0] = v[0][tmp_n[i]];
			s[i][1] = v[1][tmp_n[i]];
			s[i][2] = v[2][tmp_n[i]];
		}
		s1[0] = v[0][face_list[p.fN][I1-1]];
		s1[1] = v[1][face_list[p.fN][I1-1]];
		s1[2] = v[2][face_list[p.fN][I1-1]];
		Index = find_next_v1(s1, s, tmp_n_list);
		face_list[p.fN][I1] = tmp_n[Index];
		I1 += 1;
		p.face[p.fN].vN = I1;
		tmp_n_list -= 1;
	}


	I1 = face_list[p.fN][0];
	I2 = face_list[p.fN][1];
	I3 = face_list[p.fN][2];

	dd1[0] = v[0][I2] - v[0][I1];dd1[1] = v[1][I2] - v[1][I1];dd1[2] = v[2][I2] - v[2][I1];
	dd2[0] = v[0][I3] - v[0][I1];dd2[1] = v[1][I3] - v[1][I1];dd2[2] = v[2][I3] - v[2][I1];
	a[0] = dd1[1]*dd2[2] - dd1[2]*dd2[1];
	a[1] = dd1[2]*dd2[0] - dd1[0]*dd2[2];
	a[2] = dd1[0]*dd2[1] - dd1[1]*dd2[0];
	vol = v[0][I1]*a[0] + v[1][I1]*a[1] + v[2][I1]*a[2];
	if (vol < 0){
		for(i=0; i<p.face[p.fN].vN; i++){
			I1 = p.face[p.fN].vN - i - 1;
			tmp_n[i] = face_list[p.fN][I1];
		}
		for(i=0; i<p.face[p.fN].vN; i++){
			face_list[p.fN][i] = tmp_n[i];
		}
		a[0] = -a[0];
		a[1] = -a[1];
		a[2] = -a[2];
	}
	p.face[p.fN].fVector[0] = a[0];
	p.face[p.fN].fVector[1] = a[1];
	p.face[p.fN].fVector[2] = a[2];
}


//*******************************************************************************
void search_poly_face(float v0[3], float v[3][MAX_BONDS_FOR_ATOM], int Num)
{
	int i,j,k,l;
	float d1[3],d2[3],d3[3];
	float a[3];
	float vol;
	int check_face;
	int sign;
	int summ, abs_summ;

	p.fN = 0;
	for (i=0; i<Num-2; i++){
		for(j=i+1;j<Num-1; j++){
			for(k=j+1;k<Num;k++){
				d1[0] = v[0][j] - v[0][i]; d1[1] = v[1][j] - v[1][i]; d1[2] = v[2][j] - v[2][i];
				d2[0] = v[0][k] - v[0][i]; d2[1] = v[1][k] - v[1][i]; d2[2] = v[2][k] - v[2][i];
				a[0] = d1[1]*d2[2] - d1[2]*d2[1];
				a[1] = d1[2]*d2[0] - d1[0]*d2[2];
				a[2] = d1[0]*d2[1] - d1[1]*d2[0];
				n[0] = i; n[1] = j; n[2] = k;
				n_list = 3;
				summ = 0;
				abs_summ = 0;
				for(l=0; l<Num; l++){
					if((l != i) && (l != j) && (l != k)){
						d3[0] = v[0][l] - v[0][i]; d3[1] = v[1][l] - v[1][i]; d3[2] = v[2][l] - v[2][i];
						vol = d3[0]*a[0] + d3[1]*a[1] + d3[2]*a[2];
						if (fabs(vol) < 0.00001) {
							n[n_list] = l;
							n_list += 1;
						}
						else {
							if (vol<0) sign = -1;
							else sign = 1;
							summ += sign;
							abs_summ += 1;
						}
					}
				}
				if (abs(summ)==abs_summ) {
					check_face = 0;
					if (p.fN!=0) check_face = check_dubl_poly_face(n,n_list);
					if (check_face==0){
						sort_poly_face(v);
						p.fN++;
					}
				}
			}
		}
	}

	for(i=0;i<p.fN;i++){
		for(j=0;j<p.face[i].vN;j++){
			p.face[i].v[j][0] = v[0][face_list[i][j]];
			p.face[i].v[j][1] = v[1][face_list[i][j]];
			p.face[i].v[j][2] = v[2][face_list[i][j]];
		}
	}
}


//*******************************************************************************
void sort_by_XYZ1(float v[3][MAX_BONDS_FOR_ATOM], int Num)
{
	int i,j,M;
	float v0[3];
//----------------------------- Sort by X
	for(i=1; i<Num; i++){
		M = i;
		for(j=i-1; j>=0; j--){
			if (v[0][M] > v[0][j]){
				v0[0] = v[0][j]; v0[1] = v[1][j]; v0[2] = v[2][j];
				v[0][j] = v[0][M]; v[1][j] = v[1][M]; v[2][j] = v[2][M];
				v[0][M] = v0[0]; v[1][M] = v0[1]; v[2][M] = v0[2];
				M = j;
			}
		}
	}
//----------------------------- Sort by Y
	for(i=1; i<Num; i++){
		M = i;
		for(j=i-1; j>=0; j--){
			if (v[1][M] > v[1][j]){
				v0[0] = v[0][j]; v0[1] = v[1][j]; v0[2] = v[2][j];
				v[0][j] = v[0][M]; v[1][j] = v[1][M]; v[2][j] = v[2][M];
				v[0][M] = v0[0]; v[1][M] = v0[1]; v[2][M] = v0[2];
				M = j;
			}
		}
	}
//----------------------------- Sort by Z
	for(i=1; i<Num; i++){
		M = i;
		for(j=i-1; j>=0; j--){
			if (v[2][M] > v[2][j]){
				v0[0] = v[0][j]; v0[1] = v[1][j]; v0[2] = v[2][j];
				v[0][j] = v[0][M]; v[1][j] = v[1][M]; v[2][j] = v[2][M];
				v[0][M] = v0[0]; v[1][M] = v0[1]; v[2][M] = v0[2];
				M = j;
			}
		}
	}
}

//*******************************************************************************
void makePoly1()
{
	int i,j;
	int I1,I2,I3,I4;
	float v[3][MAX_BONDS_FOR_ATOM];
	float v0[3];
	float R1,G1,B1,R2,G2,B2,SR1,SR2;

	for(i=0; i<nPoly; i++){
		I1 = poly_list[i];
		I4 = m[I1].Num;
		v0[0] = m[I1].x; v0[1] = m[I1].x;v0[2] = m[I1].x;
//----------------------------------------- Shift Origin to the Central Atom
		for(j=0; j<m[I1].bondNum; j++){
			I2 = m[I1].b_list[j];
			v[0][j] = (m[I2].x - m[I1].x);
			v[1][j] = (m[I2].y - m[I1].y);
			v[2][j] = (m[I2].z - m[I1].z);
		}
//----------------------------------------- Sort Atoms by XYZ
		sort_by_XYZ1(v,m[I1].bondNum);
		search_poly_face(v0,v,m[I1].bondNum);

		R2 = 0; G2 = 0; B2 = 0; 
		for (j=0; j<m[I1].bondNum; j++){
			I2 = m[I1].b_list[j];
			I3 = m[I2].Num;
			R2 += lAtom[I3].color[0]; G2 += lAtom[I3].color[1]; B2 += lAtom[I3].color[2];
		}
		SR2 = (R2 + G2 + B2)/m[I1].bondNum;
		R2 /= m[I1].bondNum;
		G2 /= m[I1].bondNum;
		B2 /= m[I1].bondNum;
		R1 = lAtom[I4].color[0]; G1 = lAtom[I4].color[1]; B1 = lAtom[I4].color[2];
        SR1 = R1 + G1 + B1;
		tp[i] = p;
		tp[i].show = 'y'; 
		tp[i].color[0] = R1;
		tp[i].color[1] = G1;
		tp[i].color[2] = B1;
		tp[i].center[0] = m[I1].x;
		tp[i].center[1] = m[I1].y;
		tp[i].center[2] = m[I1].z;
	}
}

//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
//H                   Calcilate Polyhedra Parameters                               H//
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
float _calc_Poly_Vol(int ID)
{
	int i,j;
	float PVol;
	VECTOR3D a,b,c;
	float ca,cb,cg,d1,d2,d3,Vol;

	PVol = 0.0f;
	for(i=0; i<tp[ID].fN; i++){
		a.x = tp[ID].face[i].v[0][0]*cCell.max_P;
		a.y = tp[ID].face[i].v[0][1]*cCell.max_P;
		a.z = tp[ID].face[i].v[0][2]*cCell.max_P;
		d1 = (float)sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
		for(j=1; j<(tp[ID].face[i].vN-1); j++){
			b.x = tp[ID].face[i].v[j][0]*cCell.max_P;
			b.y = tp[ID].face[i].v[j][1]*cCell.max_P;
			b.z = tp[ID].face[i].v[j][2]*cCell.max_P;
			c.x = tp[ID].face[i].v[j+1][0]*cCell.max_P;
			c.y = tp[ID].face[i].v[j+1][1]*cCell.max_P;
			c.z = tp[ID].face[i].v[j+1][2]*cCell.max_P;
			d2 = (float)sqrt(b.x*b.x + b.y*b.y + b.z*b.z);
			d3 = (float)sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
			ca = (b.x*c.x + b.y*c.y + b.z*c.z)/(d2*d3);
			cb = (a.x*c.x + a.y*c.y + a.z*c.z)/(d1*d3);
			cg = (b.x*a.x + b.y*a.y + b.z*a.z)/(d2*d1);
			Vol = (float)(d1*d2*d3*sqrt(1.0f - ca*ca - cb*cb - cg*cg + 2*ca*cb*cg));
			PVol += Vol/2;
		}
	}
	PVol /= 3;
	return PVol;
}

float _calc_Poly_elong(int ID, float Vol)
{

	int i,I1,I2;
	VECTOR3D a;
	int Num,ID_check;
	float Par,L,L0,Sum,EL;

	I1 = poly_list[ID];
	Num = m[I1].bondNum;
	ID_check = -1;

	switch(Num){
//------------------------------------- for Terahedron
	case(4):{
		if (tp[ID].fN != 4) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 3) return -1;
		}
		Par = (float)exp(log(12*Vol/sqrt(2))/3);
		L0 = Par*Par*6/16;
		ID_check = 1;
		break;
			}
//------------------------------------- for Oktahedron
	case(6):{
		if (tp[ID].fN != 8) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 3) return -1;
		}
		Par = (float)exp(log(3*Vol/sqrt(2))/3);
		L0 = Par*Par*2/4;
		ID_check = 1;
		break;
			}
//------------------------------------- for Cube
	case(8):{
		if (tp[ID].fN != 6) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 4) return -1;
		}
		Par = (float)exp(log(Vol)/3);
		L0 = Par*Par*3/4;
		ID_check = 1;
		break;
			}
//------------------------------------- for Ikosahedron
	case(12):{
		if (tp[ID].fN != 20) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 3) return -1;
		}
		Par = (float)exp(log(12*Vol/(5*(3+sqrt(5))))/3);
		L0 = (float)(Par*Par*(  2*(5+sqrt(5)) )/16);
		ID_check = -1;
		break;
			}
//------------------------------------- for Dodeahedron
	case(20):{
		if (tp[ID].fN != 12) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 5) return -1;
		}
		Par = (float)exp(log(4*Vol/(15+7*sqrt(5)))/3);
		L0 = (float)(Par*Par*3*(1+sqrt(5))*(1+sqrt(5))/16);
		ID_check = -1;
		break;
			}
	}
	if (ID_check == 1){
		Sum = 0;
		for(i=0; i<m[I1].bondNum; i++){
			I2 = m[I1].b_list[i];
			a.x = (m[I2].x - m[I1].x)*cCell.max_P;
			a.y = (m[I2].y - m[I1].y)*cCell.max_P;
			a.z = (m[I2].z - m[I1].z)*cCell.max_P;
			L = a.x*a.x + a.y*a.y + a.z*a.z;
			Sum = Sum + (L/L0);
		}
		EL = Sum/(float)Num;
		return EL;
	}

	return -1;

}


float _calc_Poly_var(int ID, float Vol)
{

	int i,j,I1;
	VECTOR3D a,b,c;
	int Num,ID_check,bNum;
	float Par,L0,Sum,Sum2,TH0,pVar;
	float ac1,ac2,ac3,d1,d2,d3,ca,cb,cg;

	I1 = poly_list[ID];
	Num = m[I1].bondNum;
	ID_check = -1;
	bNum = (tp[ID].fN*3)/2;

	switch(Num){
//------------------------------------- for Terahedron
	case(4):{
		if (tp[ID].fN != 4) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 3) return -1;
		}
		Par = (float)exp(log(12*Vol/sqrt(2))/3);
		L0 = Par*Par*6/16;
		TH0 = 109.471f;
		ID_check = 1;
		break;
			}
//------------------------------------- for Oktahedron
	case(6):{
		if (tp[ID].fN != 8) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 3) return -1;
		}
		Par = (float)exp(log(3*Vol/sqrt(2))/3);
		L0 = Par*Par*2/4;
		TH0 = 90.0f;
		ID_check = 1;
		break;
			}
//------------------------------------- for Cube
	case(8):{
		if (tp[ID].fN != 6) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 4) return -1;
		}
		Par = (float)exp(log(Vol)/3);
		L0 = Par*Par*3/4;
		TH0 = 70.529f;
		ID_check = 1;
		break;
			}
//------------------------------------- for Ikosahedron
	case(12):{
		if (tp[ID].fN != 20) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 3) return -1;
		}
		Par = (float)exp(log(12*Vol/(5*(3+sqrt(5))))/3);
		L0 = (float)(Par*Par*(  2*(5+sqrt(5)) )/16);
		Par = 4*L0/(  (float)sqrt( 2*(5+(float)sqrt(5)) )  );
		TH0 = (float)(acos(1 - Par*Par/(2*L0*L0))*180/PI);
		ID_check = -1;
		break;
			}
//------------------------------------- for Dodeahedron
	case(20):{
		if (tp[ID].fN != 12) return -1;
		for(i=0; i<tp[ID].fN; i++){
			if (tp[ID].face[i].vN != 5) return -1;
		}
		Par = (float)exp(log(4*Vol/(15+7*sqrt(5)))/3);
		L0 = (float)(Par*Par*3*(1+sqrt(5))*(1+sqrt(5))/16);
		Par = 4*L0/((float)sqrt(3)*(1+(float)sqrt(5)));
		TH0 = (float)(acos(1 - Par*Par/(2*L0*L0))*180/PI);
		ID_check = -1;
		break;
			}
	}
	if (ID_check == 1){
		Sum = 0;
		Sum2 = 0;
		for(i=0; i<tp[ID].fN; i++){
			a.x = tp[ID].face[i].v[0][0]*cCell.max_P;
			a.y = tp[ID].face[i].v[0][1]*cCell.max_P;
			a.z = tp[ID].face[i].v[0][2]*cCell.max_P;
			d1 = (float)sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
			for(j=1; j<(tp[ID].face[i].vN-1); j++){
				b.x = tp[ID].face[i].v[j][0]*cCell.max_P;
				b.y = tp[ID].face[i].v[j][1]*cCell.max_P;
				b.z = tp[ID].face[i].v[j][2]*cCell.max_P;
				c.x = tp[ID].face[i].v[j+1][0]*cCell.max_P;
				c.y = tp[ID].face[i].v[j+1][1]*cCell.max_P;
				c.z = tp[ID].face[i].v[j+1][2]*cCell.max_P;
				d2 = (float)sqrt(b.x*b.x + b.y*b.y + b.z*b.z);
				d3 = (float)sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
				ca = (b.x*c.x + b.y*c.y + b.z*c.z)/(d2*d3);
				cb = (a.x*c.x + a.y*c.y + a.z*c.z)/(d1*d3);
				cg = (b.x*a.x + b.y*a.y + b.z*a.z)/(d2*d1);
				ac1 = (float)(acos(ca)*180/PI);
				ac2 = (float)(acos(cb)*180/PI);
				ac3 = (float)(acos(cg)*180/PI);
				Sum += (ac1 - TH0)*(ac1 - TH0);
				Sum += (ac2 - TH0)*(ac2 - TH0);
				Sum += (ac3 - TH0)*(ac3 - TH0);
			}
		}
		Sum /= 2;
		pVar = Sum/(bNum-1);
		return(pVar);
	}
	return -1;
}




//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
//H                          (hkl) PLANE                                           H//
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//

//*******************************************************************************
void clippPlane(int h, int k, int l,float cPlane[9][3])
{
	int ID,i;
	double x,y,z,x1,y1,z1;
	if ((h!=0) && (k==0) && (l==0)) ID = 0;     //--- (h00)
	if ((h==0) && (k!=0) && (l==0)) ID = 1;     //--- (0k0)
	if ((h==0) && (k==0) && (l!=0)) ID = 2;     //--- (00l)
	if ((h!=0) && (k!=0) && (l==0)) ID = 3;     //--- (hk0)
	if ((h!=0) && (k==0) && (l!=0)) ID = 4;     //--- (h0l)
	if ((h==0) && (k!=0) && (l!=0)) ID = 5;     //--- (0kl)
	if ((h!=0) && (k!=0) && (l!=0)) ID = 6;     //--- (hkl)
	switch (ID) {
	case (0) : {
		cPNum = 4;
		cPlane[0][0] = 1/(float)h; cPlane[0][1] = 0; cPlane[0][2] = 0;
		cPlane[1][0] = 1/(float)h; cPlane[1][1] = 1; cPlane[1][2] = 0;
		cPlane[2][0] = 1/(float)h; cPlane[2][1] = 1; cPlane[2][2] = 1;
		cPlane[3][0] = 1/(float)h; cPlane[3][1] = 0; cPlane[3][2] = 1;
		break;
			   }
	case (1) : {
		cPNum = 4;
		cPlane[0][0] = 0; cPlane[0][1] = 1/(float)k; cPlane[0][2] = 0;
		cPlane[1][0] = 0; cPlane[1][1] = 1/(float)k; cPlane[1][2] = 1;
		cPlane[2][0] = 1; cPlane[2][1] = 1/(float)k; cPlane[2][2] = 1;
		cPlane[3][0] = 1; cPlane[3][1] = 1/(float)k; cPlane[3][2] = 0;
		break;
			   }
	case (2) : {
		cPNum = 4;
		cPlane[0][0] = 0; cPlane[0][1] = 0; cPlane[0][2] = 1/(float)l;
		cPlane[1][0] = 1; cPlane[1][1] = 0; cPlane[1][2] = 1/(float)l;
		cPlane[2][0] = 1; cPlane[2][1] = 1; cPlane[2][2] = 1/(float)l;
		cPlane[3][0] = 0; cPlane[3][1] = 1; cPlane[3][2] = 1/(float)l;
		break;
			   }
	case (3) : {
		cPNum = 4;
		cPlane[0][0] = 1/(float)h; cPlane[0][1] =   0; cPlane[0][2] = 0;
		cPlane[1][0] =   0; cPlane[1][1] = 1/(float)k; cPlane[1][2] = 0;
		cPlane[2][0] =   0; cPlane[2][1] = 1/(float)k; cPlane[2][2] = 1;
		cPlane[3][0] = 1/(float)h; cPlane[3][1] =   0; cPlane[3][2] = 1;
		break;
			   }
	case (4) : {
		cPNum = 4;
		cPlane[0][0] = 1/(float)h; cPlane[0][1] = 0; cPlane[0][2] =   0;
		cPlane[1][0] = 1/(float)h; cPlane[1][1] = 1; cPlane[1][2] =   0;
		cPlane[2][0] =   0; cPlane[2][1] = 1; cPlane[2][2] = 1/(float)l;
		cPlane[3][0] =   0; cPlane[3][1] = 0; cPlane[3][2] = 1/(float)l;
		break;
			   }
	case (5) : {
		cPNum = 4;
		cPlane[0][0] = 1; cPlane[0][1] = 1/(float)k; cPlane[0][2] =   0;
		cPlane[1][0] = 0; cPlane[1][1] = 1/(float)k; cPlane[1][2] =   0;
		cPlane[2][0] = 0; cPlane[2][1] =   0; cPlane[2][2] = 1/(float)l;
		cPlane[3][0] = 1; cPlane[3][1] =   0; cPlane[3][2] = 1/(float)l;
		break;
			   }
	case (6) : {
		cPNum = 3;
		cPlane[0][0] = 1/(float)h; cPlane[0][1] =   0; cPlane[0][2] =   0;
		cPlane[1][0] =   0; cPlane[1][1] = 1/(float)k; cPlane[1][2] =   0;
		cPlane[2][0] =   0; cPlane[2][1] =   0; cPlane[2][2] = 1/(float)l;
		break;
			   }
	}
	if (h<0) for(i=0; i<cPNum;i++){cPlane[i][0] += 1;}
	if (k<0) for(i=0; i<cPNum;i++){cPlane[i][1] += 1;}
	if (l<0) for(i=0; i<cPNum;i++){cPlane[i][2] += 1;}
	for(i=0; i<cPNum; i++){
		x = cPlane[i][0]*cCell.a[0];
		y = cPlane[i][1]*cCell.a[1];
		z = cPlane[i][2]*cCell.a[2];
		x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
		y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
		z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
		cPlane[i][0] = (float)x1/cCell.max_P;
		cPlane[i][1] = (float)y1/cCell.max_P;
		cPlane[i][2] = (float)z1/cCell.max_P;
	}
}


//***************************************************************************//
void set_plane_normal(int h, int k, int l)
{
	VECTOR3D OA,OB,OC,AB,AC,BC;
	float normal_l;

	OA.x = hkl_o_axis[0][0];OA.y = hkl_o_axis[0][1];OA.z = hkl_o_axis[0][2];
	OB.x = hkl_o_axis[1][0];OB.y = hkl_o_axis[1][1];OB.z = hkl_o_axis[1][2];
	OC.x = hkl_o_axis[2][0];OC.y = hkl_o_axis[2][1];OC.z = hkl_o_axis[2][2];

	AB.x = OB.x - OA.x;
	AB.y = OB.y - OA.y;
	AB.z = OB.z - OA.z;

	AC.x = OC.x - OA.x;
	AC.y = OC.y - OA.y;
	AC.z = OC.z - OA.z;

	BC.x = OC.x - OB.x;
	BC.y = OC.y - OB.y;
	BC.z = OC.z - OB.z;

	if ((h != 0) && (k != 0) && (l != 0)){
		Normal[0] = AB.y * AC.z - AB.z * AC.y;
		Normal[1] = AB.z * AC.x - AB.x * AC.z;
		Normal[2] = AB.x * AC.y - AB.y * AC.x;
	}
	if ((h != 0) && (k != 0) && (l == 0)){
		Normal[0] = AB.y * OC.z - AB.z * OC.y;
		Normal[1] = AB.z * OC.x - AB.x * OC.z;
		Normal[2] = AB.x * OC.y - AB.y * OC.x;
	}
	if ((h != 0) && (k == 0) && (l != 0)){
		Normal[0] = OB.y * AC.z - OB.z * AC.y;
		Normal[1] = OB.z * AC.x - OB.x * AC.z;
		Normal[2] = OB.x * AC.y - OB.y * AC.x;
	}
	if ((h == 0) && (k != 0) && (l != 0)){
		Normal[0] = BC.y * OA.z - BC.z * OA.y;
		Normal[1] = BC.z * OA.x - BC.x * OA.z;
		Normal[2] = BC.x * OA.y - BC.y * OA.x;
	}
	if ((h != 0) && (k == 0) && (l == 0)){
		Normal[0] = OB.y * OC.z - OB.z * OC.y;
		Normal[1] = OB.z * OC.x - OB.x * OC.z;
		Normal[2] = OB.x * OC.y - OB.y * OC.x;
	}
	if ((h == 0) && (k != 0) && (l == 0)){
		Normal[0] = OC.y * OA.z - OC.z * OA.y;
		Normal[1] = OC.z * OA.x - OC.x * OA.z;
		Normal[2] = OC.x * OA.y - OC.y * OA.x;
	}
	if ((h == 0) && (k == 0) && (l != 0)){
		Normal[0] = OA.y * OB.z - OA.z * OB.y;
		Normal[1] = OA.z * OB.x - OA.x * OB.z;
		Normal[2] = OA.x * OB.y - OA.y * OB.x;
	}

	normal_l = (float)sqrt(Normal[0]*Normal[0] + Normal[1]*Normal[1] + Normal[2]*Normal[2]);
	Normal[0] /=  normal_l;
	Normal[1] /=  normal_l;
	Normal[2] /=  normal_l;
}


//****************************************************************//
//                 Get Orthogonal (hkl) Plane                     //
//****************************************************************//
void get_ortho_hkl_plane()
{
	double x,y,z,x1,y1,z1;
	float A,B,C;

	if (hkl[0] == 0) A = cCell.a[0]; else A = cCell.a[0] / (float)hkl[0];
	if (hkl[1] == 0) B = cCell.a[1]; else B = cCell.a[1] / (float)hkl[1];
	if (hkl[2] == 0) C = cCell.a[2]; else C = cCell.a[2] / (float)hkl[2];
//-------------------------------------------------------------- (a / h)
	x = A; y = 0; z = 0;
	x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
	y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
	z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
	hkl_o_axis[0][0] = (float)x1;
	hkl_o_axis[0][1] = (float)y1;
	hkl_o_axis[0][2] = (float)z1;
//-------------------------------------------------------------- (b / k)
	x = 0; y = B; z = 0;
	x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
	y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
	z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
	hkl_o_axis[1][0] = (float)x1;
	hkl_o_axis[1][1] = (float)y1;
	hkl_o_axis[1][2] = (float)z1;
//-------------------------------------------------------------- (c / l)
	x = 0; y = 0; z = C;
	x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
	y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
	z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
	hkl_o_axis[2][0] = (float)x1;
	hkl_o_axis[2][1] = (float)y1;
	hkl_o_axis[2][2] = (float)z1;
}

//***************************************************************************//
void HKLordering()
{
	int i,j,I1;
	int nVector;
	VECTOR3D A[9],C;
	int Sign, sum;
	float vol;
	int oIndex[9],Index[9],numIndex, ind;

	numIndex = 1;
	Index[0] = 0;
	ind = 0;
	while (numIndex < cPNum){
//-------------------------------------------------------------
		j = 0;
		for(i=0; i<cPNum; i++){
			if(i != ind){
				A[j].x = cPlane1[i][0] - cPlane1[ind][0];
				A[j].y = cPlane1[i][1] - cPlane1[ind][1];
				A[j].z = cPlane1[i][2] - cPlane1[ind][2];
				oIndex[j] = i;
				j++;
			}
		}
		nVector = cPNum - 1;
//-------------------------------------------------------------
		for(i=0; i<nVector; i++){
			I1 = i;
			sum = 0;
			for(j=0; j<nVector; j++){
				if (j != I1){
					C.x = A[I1].y*A[j].z - A[I1].z*A[j].y;
					C.y = A[I1].z*A[j].x - A[I1].x*A[j].z;
					C.z = A[I1].x*A[j].y - A[I1].y*A[j].x;
					vol = C.x*Normal[0] + C.y*Normal[1] + C.z*Normal[2];
					if (vol >= 0) Sign = 1;
					else Sign = -1;
					sum += Sign;
				}
			}
			if (sum == (nVector-1)){
				Index[numIndex] = oIndex[I1];
				numIndex++;
				ind = oIndex[I1];
			}
		}
//-------------------------------------------------------------
	}

	for(i=0; i<cPNum; i++){
		A[i].x = cPlane1[Index[i]][0];
		A[i].y = cPlane1[Index[i]][1];
		A[i].z = cPlane1[Index[i]][2];
	}

	for(i=0; i<cPNum; i++){
		cPlane1[i][0] = A[i].x;
		cPlane1[i][1] = A[i].y;
		cPlane1[i][2] = A[i].z;
	}
}


//***************************************************************************//
void make_bound_hkl_plane(float cPlane[9][3])
{
	VECTOR3D C,V,A,X,n,CA,CV,R;
	short v1[] = {0,1,3,0,4,5,7,4,0,1,2,3};
	short v2[] = {1,2,2,3,5,6,6,7,4,5,6,7};
	int i,j,skip;
	float CN,CM,K;

	n.x = Normal[0];
	n.y = Normal[1];
	n.z = Normal[2];

//	A.x = o_center[0] + shift_hkl[0];
//	A.y = o_center[1] + shift_hkl[1];
//	A.z = o_center[2];

	A.x = o_center[0] + shift_hkl[1]*Normal[0];
	A.y = o_center[1] + shift_hkl[1]*Normal[1];
	A.z = o_center[2] + shift_hkl[1]*Normal[2];

	cPNum = 0;
	for(i=0; i<12; i++){
		C.x = o_range[v1[i]][0];
		C.y = o_range[v1[i]][1];
		C.z = o_range[v1[i]][2];

		V.x = o_range[v2[i]][0];
		V.y = o_range[v2[i]][1];
		V.z = o_range[v2[i]][2];

		CA.x = A.x - C.x;
		CA.y = A.y - C.y;
		CA.z = A.z - C.z;

		CV.x = V.x - C.x;
		CV.y = V.y - C.y;
		CV.z = V.z - C.z;

		CN = CA.x * n.x + CA.y * n.y + CA.z * n.z;
		CM = CV.x * n.x + CV.y * n.y + CV.z * n.z;
		if (CM != 0) K = CN / CM;
		else K = 0;

		X.x = CV.x * K;
		X.y = CV.y * K;
		X.z = CV.z * K;

		if ((K > 0) && (K < 1)){
			R.x = C.x + X.x;
			R.y = C.y + X.y;
			R.z = C.z + X.z;
			skip = 0;
			for(j=0; j<cPNum; j++){
				if ((R.x == cPlane[j][0]) && (R.y == cPlane[j][1]) && (R.z == cPlane[j][2])){
					skip = 1;
					break;
				}
			}
			if (skip == 0){
				cPlane[cPNum][0] = R.x/cCell.max_P; 
				cPlane[cPNum][1] = R.y/cCell.max_P; 
				cPlane[cPNum][2] = R.z/cCell.max_P; 
				cPNum++;
			}
		}
	}
	HKLordering();
}


//*******************************************************************************
void makeHKLplane(int ID)
{
	int HKL_ID;

	if ((hkl[0]==0) && (hkl[1]==0) && (hkl[2]==0)) HKL_ID=0;
	else HKL_ID = 1;
	if (HKL_ID==1) {
		get_ortho_hkl_plane();
//		calc_Dhkl(hkl[0],hkl[1],hkl[2]);
		set_plane_normal(hkl[0],hkl[1],hkl[2]);
		switch(ID)
		{
//--------------------------------------------------------- Origin: Unit Cell
		case(0):{
			clippPlane(hkl[0],hkl[1],hkl[2],cPlane1);
			clippPlane(-hkl[0],-hkl[1],-hkl[2],cPlane2);
			break;
				}
		case(1):{
			make_bound_hkl_plane(cPlane1);
			break;
				}
		}
	}
}


//****************************************************************//
//     Get Orthogonal Boundaries XYZ and Center of Boundaries     //
//****************************************************************//
void set_boundaries_ortho_XYZ()
{
	int i;
	short indX[] = {0,1,1,0,0,1,1,0};
	short indY[] = {2,2,2,2,3,3,3,3};
	short indZ[] = {4,4,5,5,4,4,5,5};
	double x,y,z,x1,y1,z1;

	for (i=0; i<8; i++){
		x = range[indX[i]] * cCell.a[0];
		y = range[indY[i]] * cCell.a[1];
		z = range[indZ[i]] * cCell.a[2];
		x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
		y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
		z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
		o_range[i][0] = (float)x1;
		o_range[i][1] = (float)y1;
		o_range[i][2] = (float)z1;
	}

	x = (range[0] + range[1]) * cCell.a[0] / 2;
	y = (range[2] + range[3]) * cCell.a[1] / 2;
	z = (range[4] + range[5]) * cCell.a[2] / 2;
	x1 = x*ortho_M[0][0] + y*ortho_M[0][1] + z*ortho_M[0][2];
	y1 = x*ortho_M[1][0] + y*ortho_M[1][1] + z*ortho_M[1][2];
	z1 = x*ortho_M[2][0] + y*ortho_M[2][1] + z*ortho_M[2][2];
	o_center[0] = (float)x1;
	o_center[1] = (float)y1;
	o_center[2] = (float)z1;
}






//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//
//H                          Save / Export Data                                    H//
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH//

//*******************************************************************************
void save_data(char *fname)
{
	int i,j;
	FILE *fptr;
	float dif;

	dif = (float)(8.0*PI*PI);
	if (use_iso == 0) dif = 1.0f;

	fptr = fopen(fname,"w");

//------------------------------------------------------ TITLE
	fprintf(fptr,"TITLE\n");
	fprintf(fptr,"%s\n",Title);
//------------------------------------------------------ GROUP
	fprintf(fptr,"GROUP\n");
	fprintf(fptr,"%i %i %s\n",cCell.SpGrN,cCell.Setting,cCell.SpGr);
//------------------------------------------------------ SYMOP
	fprintf(fptr,"SYMOP\n");
	for(i=0; i<nPos; i++){
		fprintf(fptr,"%7.4f %7.4f %7.4f  %2i %2i %2i  %2i %2i %2i  %2i %2i %2i\n",
			         sPos[i].t[0],sPos[i].t[1],sPos[i].t[2],
					 sPos[i].a[0][0],sPos[i].a[1][0],sPos[i].a[2][0],
					 sPos[i].a[0][1],sPos[i].a[1][1],sPos[i].a[2][1],
					 sPos[i].a[0][2],sPos[i].a[1][2],sPos[i].a[2][2]);
	}
	fprintf(fptr," -1.0 -1.0 -1.0  0 0 0  0 0 0  0 0 0\n");
//------------------------------------------------------ CELLP
	fprintf(fptr,"CELLP\n");
	fprintf(fptr,"%10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n", 
		          cCell.a[0],cCell.a[1],cCell.a[2],cCell.a[3],cCell.a[4],cCell.a[5]);
	fprintf(fptr,"%10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n",
		          cCell.ea[0],cCell.ea[1],cCell.ea[2],cCell.ea[3],cCell.ea[4],cCell.ea[5]);

	fprintf(fptr,"STRUC\n");
	for(i=0; i<nAtom; i++){
			fprintf(fptr,"%3i %2s %7s %7.4f %10.5f %10.5f %10.5f\n",
		           i+1,lAtom[i].simb,lAtom[i].name,lAtom[i].g,lAtom[i].x,lAtom[i].y,lAtom[i].z);
		fprintf(fptr,"%33.5f %10.5f %10.5f\n",lAtom[i].ex,lAtom[i].ey,lAtom[i].ez);
	}
	fprintf(fptr,"  0 0 0 0 0 0 0\n");
//----------------------------------------------------------------- THERI
	fprintf(fptr,"THERI\n");
	for(i=0; i<nAtom; i++){
		fprintf(fptr,"%3i %7s %8.3f\n",i+1,lAtom[i].name,lAtom[i].Biso*dif);
	}
	fprintf(fptr,"  0 0 0\n");
//------------------------------------------------------ THERM
	if (use_aniso > 0){
		fprintf(fptr,"THERT %i\n",Btype);

		fprintf(fptr,"THERM\n");
		for(i=0; i<nAtom; i++){
			fprintf(fptr,"%3i %7s %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n",i+1, lAtom[i].name,
			          lAtom[i].B[0],lAtom[i].B[1],lAtom[i].B[2],
					  lAtom[i].B[3],lAtom[i].B[4],lAtom[i].B[5]);
		}
		fprintf(fptr,"  0 0 0 0 0 0 0 0\n");
	}
//------------------------------------------------------ ATOMT
	fprintf(fptr,"ATOMT\n");
	for(i=0; i<tAtom; i++){
		fprintf(fptr,"%3i %2s %7.4f %7.4f %7.4f %7.4f\n",
			          i+1,AtomT[i].symb, AtomT[i].R,AtomT[i].color[0],AtomT[i].color[1],AtomT[i].color[2]);
	}
	fprintf(fptr,"  0 0 0 0 0 0\n");
//------------------------------------------------------ MODEL
	fprintf(fptr,"MODEL %i\n",MODL);
//------------------------------------------------------ RADII
	fprintf(fptr,"RADII %i\n",radii_type);
//------------------------------------------------------ BOUND
	fprintf(fptr,"BOUND\n");
	fprintf(fptr,"%8.3f %8.3f  %8.3f %8.3f  %8.3f %8.3f\n",
		          range[0],range[1],range[2],range[3],range[4],range[5]);
//------------------------------------------------------ SBOND
	fprintf(fptr,"SBOND\n");
	for(i=0; i<bond_sNum; i++){
		fprintf(fptr,"%3i %2s %2s %10.5f %2i %2i %2i\n",i+1,bini[i].A1,bini[i].A2,bini[i].d,
			     bini[i].search_mode,bini[i].boundary_mode,bini[i].show_poly_mode);
	}
	fprintf(fptr,"  0 0 0 0\n");
//------------------------------------------------------ HBOND
	fprintf(fptr,"HBOND %i %i\n",hydrogen_bonds_id,hbond_line_type);
//------------------------------------------------------ PLANE
	fprintf(fptr,"PLANE\n");
	fprintf(fptr,"%3i %3i %3i\n",hkl[0],hkl[1],hkl[2]);
//------------------------------------------------------ ATOMS
	fprintf(fptr,"ATOMS %i\n",atom_type);
//------------------------------------------------------ BONDS
	fprintf(fptr,"BONDS %i\n",bonds_type);
//------------------------------------------------------ POLYS
	fprintf(fptr,"POLYS %i\n",poly_type);
//------------------------------------------------------ ATOMP
	fprintf(fptr,"ATOMP\n");
	fprintf(fptr,"%3i %3i %3i %3i\n",atoms_stack,atoms_slice,TEOL,Probability);
//------------------------------------------------------ BONDP
	fprintf(fptr,"BONDP\n");
	fprintf(fptr,"%3i %3i %6.3f %6.3f %6.3f %6.4f %6.4f %6.4f\n",
		          bonds_stack,bonds_slice,bRadii[0],bRadii[1],bonds_line_width,BNCL[0],BNCL[1],BNCL[2]);
//------------------------------------------------------ POLYP
	fprintf(fptr,"POLYP\n");
	fprintf(fptr,"%7.3f %i %6.3f %6.4f %6.4f %6.4f\n",
		          poly_trans,poly_outline,poly_outline_width,
				  poly_line_color[0],poly_line_color[1],poly_line_color[2]);
//------------------------------------------------------ HKLPP
	fprintf(fptr,"HKLPP\n");
	fprintf(fptr,"%7.3f %i %6.3f %6.4f %6.4f %6.4f\n",
		          hkl_trans,show_hkl_out,hkl_out_width,
				  hkl_rgb[0],hkl_rgb[1],hkl_rgb[2]);
//------------------------------------------------------ UCOLP
	fprintf(fptr,"UCOLP\n");
	fprintf(fptr,"%3i %6.3f %6.4f %6.4f %6.4f\n",SHCL,ucell_width,
		         ucell_color[0],ucell_color[1],ucell_color[2]);
//------------------------------------------------------ BKGRC
	fprintf(fptr,"BKGRC\n");
	fprintf(fptr,"%9.4f %6.4f %6.4f\n",BKCL[0],BKCL[1],BKCL[2]);
//------------------------------------------------------ PROJT
	fprintf(fptr,"PROJT %i\n",projection_type);
//------------------------------------------------------ COMPS
	fprintf(fptr,"COMPS %i\n",show_axis);
//------------------------------------------------------ DEPTH-CUEING
	fprintf(fptr,"DEPTH %6.3f\n",fog);
//------------------------------------------------------ SCENE
	fprintf(fptr,"SCENE\n");
	for(i=0; i<4; i++) {
		fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",c_rotation[i],c_rotation[i+4],c_rotation[i+8],c_rotation[i+12]);
	}
	fprintf(fptr,"%7.3f %7.3f\n",obj_pos[0],obj_pos[1]);
	fprintf(fptr,"%7.3f\n",obj_pos[2]);
	fprintf(fptr,"%7.3f\n",scale);
//------------------------------------------------------ LIGHT
	fprintf(fptr,"LIGHT\n");
	for(i=0; i<4; i++) {
		fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",l_direction[i],l_direction[i+4],l_direction[i+8],l_direction[i+12]);
	}
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",l_position[0],l_position[1],l_position[2],l_position[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f\n",l_dir[0],l_dir[1],l_dir[2]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",l_ambient[0],l_ambient[1],l_ambient[2],l_ambient[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",l_diffuse[0],l_diffuse[1],l_diffuse[2],l_diffuse[3]);
//------------------------------------------------------ ATOMM
	fprintf(fptr,"ATOMM\n");
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_atom_ambient[0],m_atom_ambient[1],m_atom_ambient[2],m_atom_ambient[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_atom_diffuse[0],m_atom_diffuse[1],m_atom_diffuse[2],m_atom_diffuse[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_atom_specular[0],m_atom_specular[1],m_atom_specular[2],m_atom_specular[3]);
	fprintf(fptr,"%7.3f\n",m_atom_shininess);
//------------------------------------------------------ BONDM
	fprintf(fptr,"BONDM\n");
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_bond_ambient[0],m_bond_ambient[1],m_bond_ambient[2],m_bond_ambient[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_bond_diffuse[0],m_bond_diffuse[1],m_bond_diffuse[2],m_bond_diffuse[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_bond_specular[0],m_bond_specular[1],m_bond_specular[2],m_bond_specular[3]);
	fprintf(fptr,"%7.3f\n",m_bond_shininess);
//------------------------------------------------------ POLYM
	fprintf(fptr,"POLYM\n");
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_poly_ambient[0],m_poly_ambient[1],m_poly_ambient[2],m_poly_ambient[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_poly_diffuse[0],m_poly_diffuse[1],m_poly_diffuse[2],m_poly_diffuse[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_poly_specular[0],m_poly_specular[1],m_poly_specular[2],m_poly_specular[3]);
	fprintf(fptr,"%7.3f\n",m_poly_shininess);
//------------------------------------------------------ HKLPM
	fprintf(fptr,"HKLPM\n");
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_plane_ambient[0],m_plane_ambient[1],m_plane_ambient[2],m_plane_ambient[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_plane_diffuse[0],m_plane_diffuse[1],m_plane_diffuse[2],m_plane_diffuse[3]);
	fprintf(fptr,"%7.3f %7.3f %7.3f %7.3f\n",m_plane_specular[0],m_plane_specular[1],m_plane_specular[2],m_plane_specular[3]);
	fprintf(fptr,"%7.3f\n",m_plane_shininess);
//------------------------------------------------------ DELOB
	fprintf(fptr,"DELOB\n");
	fprintf(fptr,"%3i\n",del_Object.nAtom);
	for (i=0; i<del_Object.nAtom; i++) fprintf(fptr,"%i ",del_Object.del_Atom[i]);
	fprintf(fptr,"\n");
	fprintf(fptr,"%3i\n",del_Object.nBonds);
	for (i=0; i<del_Object.nBonds; i++) fprintf(fptr,"%i ",del_Object.del_Bonds[i]);
	fprintf(fptr,"\n");
	fprintf(fptr,"%3i\n",del_Object.nPoly);
	for (i=0; i<del_Object.nPoly; i++) fprintf(fptr,"%i ",del_Object.del_Poly[i]);
	fprintf(fptr,"\n");
//------------------------------------------------------ VECTA
	fprintf(fptr,"VECTA\n");
	fprintf(fptr,"%3i\n",num_Spin);
	if (num_Spin > 0){
		fprintf(fptr,"%7.3f\n",spin_H);
		fprintf(fptr,"%7.3f %7.3f %7.3f\n",spin_RGB[0],spin_RGB[1],spin_RGB[2]);
		j = 0;
		for(i=0; i<nXYZ; i++) {
			if (m[i].show_spin == 1) {
				fprintf(fptr,"%3i %6.2f %6.2f %6.2f ",i,m[i].spin_dir[0],m[i].spin_dir[1],m[i].spin_dir[2]);
				j++;
				if (j == 3) {fprintf(fptr,"\n"); j = 0;}
			}
		}
	}
	fclose(fptr);
}


//*******************************************************************************
void exportCIFfile(char *fname)
{
	char Term[10];
	int i,j,k,I1,I2;
	FILE *fptr;
	char symm1[10],symm2[10],symm3[10];
	char *param, param1[20];
	char esd;
	char aparam[10];
	char line[80];
	float x,ex;
	double a,b,c,ar,br,cr,Vol,ca,cb,cg,sa,sb,sg;
	float b11,b22,b33,b12,b13,b23;
       
	fptr = fopen(fname,"w");
	fptr = fopen(fname,"w");
	fprintf(fptr,"#======================================================================\n\n");
	fprintf(fptr,"# CRYSTAL DATA\n\n");
	fprintf(fptr,"#----------------------------------------------------------------------\n\n");
	fprintf(fptr,"data_VICS_phase_1\n\n\n");
//--------------------------------------------------------------------------- Title
        param = Title; j=0;
	while (j != 1){if (param[0] == ' ') param++;else j = 1;}
	fprintf(fptr,"_pd_phase_name%26c%s\n",' ',param);
//---------------------------------------------------------------------------- Unit Cell
	for(k=0; k<6; k++){
		param = "";
		if (cCell.ea[k] != 0){
			param = "";
			sprintf(param1,"%8.5f",cCell.ea[k]);
                        param = param1;
			j = 0;
			I1 = 0; 
			while(j != 1) {
				if ((param[0] != ' ') && (param[0] != '.') && (param[0] != '0')) j = 1;
				else {param++;I1++;}
			}
			esd = param[0];
			sprintf(param1,"%8.5f",cCell.a[k]);
                        param = param1;
			sprintf(aparam,"%s"," ");
			strncat(aparam,param,I1+1);
		}
                else {
			param = "";
			sprintf(param1,"%8.5f",cCell.a[k]);
                        param = param1;
                        if (k > 2){
			if (cCell.a[k] == 90) param = "90";
			if (cCell.a[k] == 120) param = "120";
			}
		}
            	j = 0;
		while (j != 1){
			if (param[0] == ' ') param++;
			else j = 1;
		}
		switch(k)
		{
		case(0):{fprintf(fptr,"_cell_length_a%25c%s\n",' ',param);break;}
		case(1):{fprintf(fptr,"_cell_length_b%25c%s\n",' ',param);break;}
		case(2):{fprintf(fptr,"_cell_length_c%25c%s\n",' ',param);break;}
		case(3):{fprintf(fptr,"_cell_angle_alpha%22c%s\n",' ',param);break;}
		case(4):{fprintf(fptr,"_cell_angle_beta%23c%s\n",' ',param);break;}
		case(5):{fprintf(fptr,"_cell_angle_gamma%22c%s\n",' ',param);break;}
		}
	}
//---------------------------------------------------------------------------- Space Group Name
	param = "";
        sprintf(param1,"%s",cCell.SpGr);param = param1;j=0;
	while (j != 1){if (param[0] == ' ') param++;else j = 1;}
	fprintf(fptr,"_symmetry_space_group_name_H-M%9c'%s'\n",' ',param);
//---------------------------------------------------------------------------- Space Group Number
	param = "";
        sprintf(param1,"%i",cCell.SpGrN);param = param1;j=0;
	while (j != 1){if (param[0] == ' ') param++;else j = 1;}
	fprintf(fptr,"_symmetry_Int_Tables_number%12c%s\n",' ',param);
//--------------------------------------------------------------------------- Symmetry Operations
	fprintf(fptr,"\nloop_\n");
	fprintf(fptr,"_symmetry_equiv_pos_site_id\n");
	fprintf(fptr,"_symmetry_equiv_pos_as_xyz\n");
	for(i=0; i<nPos; i++){
//.......................................................... (X)
		sprintf(symm1," ");
		if (sPos[i].a[0][0] == 1)  strcat(symm1,"+x");
		if (sPos[i].a[0][0] == -1) strcat(symm1,"-x");
		if (sPos[i].a[1][0] == 1)  strcat(symm1,"+y");
		if (sPos[i].a[1][0] == -1) strcat(symm1,"-y");
		if (sPos[i].a[2][0] == 1)  strcat(symm1,"+z");
		if (sPos[i].a[2][0] == -1) strcat(symm1,"-z");
		param = "";
		sprintf(param1,"%s",symm1);param = param1;
		if (param[0] == ' ') param++;
		if(param[0] == '+') param++;
		sprintf(symm1,"%s",param);
//.......................................................... (Y)
		sprintf(symm2," ");
		if (sPos[i].a[0][1] == 1)  strcat(symm2,"+x");
		if (sPos[i].a[0][1] == -1) strcat(symm2,"-x");
		if (sPos[i].a[1][1] == 1)  strcat(symm2,"+y");
		if (sPos[i].a[1][1] == -1) strcat(symm2,"-y");
		if (sPos[i].a[2][1] == 1)  strcat(symm2,"+z");
		if (sPos[i].a[2][1] == -1) strcat(symm2,"-z");
		param = "";
		sprintf(param1,"%s",symm2);param = param1;
		if (param[0] == ' ') param++;
		if(param[0] == '+') param++;
		sprintf(symm2,"%s",param);
//.......................................................... (Z)
		sprintf(symm3," ");
		if (sPos[i].a[0][2] == 1)  strcat(symm3,"+x");
		if (sPos[i].a[0][2] == -1) strcat(symm3,"-x");
		if (sPos[i].a[1][2] == 1)  strcat(symm3,"+y");
		if (sPos[i].a[1][2] == -1) strcat(symm3,"-y");
		if (sPos[i].a[2][2] == 1)  strcat(symm3,"+z");
		if (sPos[i].a[2][2] == -1) strcat(symm3,"-z");
		param = "";
		sprintf(param1,"%s",symm3);param = param1;
		if (param[0] == ' ') param++;
		if(param[0] == '+') param++;
		sprintf(symm3,"%s",param);
//.......................................................... (Transl X)
		if(sPos[i].t[0] > 0){
			strcat(symm1,"+");
			if (sPos[i].t[0] == (1.0/4.0)) strcat(symm1,"1/4");
			if (sPos[i].t[0] == (3.0/4.0)) strcat(symm1,"3/4");
			if (sPos[i].t[0] == (1.0/2.0)) strcat(symm1,"1/2");
			if (sPos[i].t[0] == (1.0/3.0)) strcat(symm1,"1/3");
			if (sPos[i].t[0] == (2.0/3.0)) strcat(symm1,"2/3");
			if (sPos[i].t[0] == (1.0/6.0)) strcat(symm1,"1/6");
			if (sPos[i].t[0] == (5.0/6.0)) strcat(symm1,"5/6");
			if (sPos[i].t[0] == (1.0/12.0)) strcat(symm1,"1/12");
			if (sPos[i].t[0] == (5.0/12.0)) strcat(symm1,"5/12");
			if (sPos[i].t[0] == (7.0/12.0)) strcat(symm1,"7/12");
			if (sPos[i].t[0] == (11.0/12.0)) strcat(symm1,"11/12");
		}
//.......................................................... (Transl Y)
		if(sPos[i].t[1] > 0){
			strcat(symm2,"+");
			if (sPos[i].t[1] == (1.0/4.0)) strcat(symm2,"1/4");
			if (sPos[i].t[1] == (3.0/4.0)) strcat(symm2,"3/4");
			if (sPos[i].t[1] == (1.0/2.0)) strcat(symm2,"1/2");
			if (sPos[i].t[1] == (1.0/3.0)) strcat(symm2,"1/3");
			if (sPos[i].t[1] == (2.0/3.0)) strcat(symm2,"2/3");
			if (sPos[i].t[1] == (1.0/6.0)) strcat(symm2,"1/6");
			if (sPos[i].t[1] == (5.0/6.0)) strcat(symm2,"5/6");
			if (sPos[i].t[1] == (1.0/12.0)) strcat(symm2,"1/12");
			if (sPos[i].t[1] == (5.0/12.0)) strcat(symm2,"5/12");
			if (sPos[i].t[1] == (7.0/12.0)) strcat(symm2,"7/12");
			if (sPos[i].t[1] == (11.0/12.0)) strcat(symm2,"11/12");
		}
//.......................................................... (Transl Z)
		if(sPos[i].t[2] > 0){
			strcat(symm3,"+");
			if (sPos[i].t[2] == (1.0/4.0)) strcat(symm3,"1/4");
			if (sPos[i].t[2] == (3.0/4.0)) strcat(symm3,"3/4");
			if (sPos[i].t[2] == (1.0/2.0)) strcat(symm3,"1/2");
			if (sPos[i].t[2] == (1.0/3.0)) strcat(symm3,"1/3");
			if (sPos[i].t[2] == (2.0/3.0)) strcat(symm3,"2/3");
			if (sPos[i].t[2] == (1.0/6.0)) strcat(symm3,"1/6");
			if (sPos[i].t[2] == (5.0/6.0)) strcat(symm3,"5/6");
			if (sPos[i].t[2] == (1.0/12.0)) strcat(symm3,"1/12");
			if (sPos[i].t[2] == (5.0/12.0)) strcat(symm3,"5/12");
			if (sPos[i].t[2] == (7.0/12.0)) strcat(symm3,"7/12");
			if (sPos[i].t[2] == (11.0/12.0)) strcat(symm3,"11/12");
		}
//---------------------------------------------------------------------
		fprintf(fptr,"%6i   %s,%s,%s\n",i+1,symm1,symm2,symm3);
	}
//--------------------------------------------------------------------------- Structure Parameters
	fprintf(fptr,"\nloop_\n");
	fprintf(fptr,"   _atom_site_label\n");
	fprintf(fptr,"   _atom_site_occupancy\n");
	fprintf(fptr,"   _atom_site_fract_x\n");
	fprintf(fptr,"   _atom_site_fract_y\n");
	fprintf(fptr,"   _atom_site_fract_z\n");
	fprintf(fptr,"   _atom_site_thermal_displace_type\n");
	if (use_iso == 1) fprintf(fptr,"   _atom_site_U_iso_or_equiv\n");
	else fprintf(fptr,"   _atom_site_B_iso_or_equiv\n");
	fprintf(fptr,"   _atom_site_type_symbol\n");
	for(i=0; i<nAtom; i++){
		switch(export_mode)
		{
		case(1):{
			sprintf(Term,"");
			if (lAtom[i].Biso > 0)	{
				if (use_iso == 1) sprintf(Term,"Uiso %6.3f",lAtom[i].Biso);
				else sprintf(Term,"Biso %6.3f",lAtom[i].Biso);
			}
			else sprintf(Term,"Biso  ?");
			break;
				}
		case(2):{
			sprintf(Term,"");
			if (lAtom[i].Biso == 0){
				if (Btype == 0) sprintf(Term,"Uani  ?");
				else sprintf(Term,"Bani  ?");
			}
			else {
				if (use_iso == 1) sprintf(Term,"Uiso %6.3f",lAtom[i].Biso);
				else sprintf(Term,"Biso %6.3f",lAtom[i].Biso);
			}
			break;
				}
		}
		sprintf(line,"%s","                                                                                  ");
		strncpy(line,lAtom[i].name,strlen(lAtom[i].name));
		param = "";
		if (lAtom[i].g == 1) param = "1.0";
		else {sprintf(param1,"%5.3f",lAtom[i].g);param = param1;}
		strncpy(line + 9,param,strlen(param));
		for(k=0; k<3; k++){
			if (k == 0) {x = lAtom[i].x; ex = lAtom[i].ex;}
			if (k == 1) {x = lAtom[i].y; ex = lAtom[i].ey;}
			if (k == 2) {x = lAtom[i].z; ex = lAtom[i].ez;}
			if (ex != 0){
				param = "";
                                sprintf(param1,"%8.5f",ex);param = param1;
				j = 0;I1 = 0; 
				while(j != 1) {
					if ((param[0] != ' ') && (param[0] != '.') && (param[0] != '0')) j = 1;
					else {param++;I1++;}
				}
				esd = param[0];
				param = "";
                                sprintf(param1,"%8.5f",x);param = param1;
				sprintf(aparam,"%s"," ");strncat(aparam,param,I1+1);
				param = "";
                                sprintf(param1,"%s(%c)",aparam,esd);param = param1;
				I1 = I1+1;I2 = 4;
			}
			else {
				I1 = 8; I2 = 0;
				param = "";
                                sprintf(param1,"%8.5f",x);param = param1;
				if (x == 0)                      { param = ""; param = "0";   I1 = 1;}
				if (x == 0.5)                    { param = ""; param = "1/2"; I1 = 3;}
				if (x == 0.25)                   { param = ""; param = "1/4"; I1 = 3;}
				if (x == 0.75)                   { param = ""; param = "3/4"; I1 = 3;}
				if (x == 0.125)                  { param = ""; param = "1/8"; I1 = 3;}
			}
			j = 0;
			while (j != 1){
				if (param[0] == ' ') {param++; I1--;}
				else j = 1;
			}
			strncpy(line + 15 + k*12,param,I1+I2);
		}
		strncpy(line + 51,Term,strlen(Term));
		strncpy(line + 63,lAtom[i].simb,strlen(lAtom[i].simb));
		fprintf(fptr,"   %s\n",line);
	}
//--------------------------------------------------------------------------- Anizotropic Thermal Parameters
	if (export_mode == 2){
		a = cCell.a[0];
		b = cCell.a[1];
		c = cCell.a[2];
		ca = cos(cCell.a[3]*RAD); sa = sin(cCell.a[3]*RAD); 
		cb = cos(cCell.a[4]*RAD); sb = sin(cCell.a[4]*RAD);
		cg = cos(cCell.a[5]*RAD); sg = sin(cCell.a[5]*RAD);
		Vol = a*b*c*sqrt(1.0 - ca*ca - cb*cb - cg*cg + 2.0*ca*cb*cg);
		ar = b*c*sa/Vol;
		br = a*c*sb/Vol;
		cr = a*b*sg/Vol;

		fprintf(fptr,"\nloop_\n");
		fprintf(fptr,"   _atom_site_aniso_label\n");
		fprintf(fptr,"   _atom_site_aniso_U_11\n");
		fprintf(fptr,"   _atom_site_aniso_U_22\n");
		fprintf(fptr,"   _atom_site_aniso_U_33\n");
		fprintf(fptr,"   _atom_site_aniso_U_12\n");
		fprintf(fptr,"   _atom_site_aniso_U_13\n");
		fprintf(fptr,"   _atom_site_aniso_U_23\n");
		for(i=0; i<nAtom; i++){
			b11 = lAtom[i].B[0];
			b22 = lAtom[i].B[1];
			b33 = lAtom[i].B[2];
			b12 = lAtom[i].B[3];
			b13 = lAtom[i].B[4];
			b23 = lAtom[i].B[5];
			if (Btype == 1) {
				b11 /= (float)(ar*ar);
				b22 /= (float)(br*br);
				b33 /= (float)(cr*cr);
				b12 /= (float)(ar*br);
				b13 /= (float)(ar*cr);
				b23 /= (float)(br*cr);
			}
			fprintf(fptr,"   %s\t%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f\n",lAtom[i].name,
				         b11,b22,b33,b12,b13,b23); 
		}
	}
	fclose(fptr);
}


//*******************************************************************************
void exportRIETANfile(char *fname)
{
	int i;
	FILE *fptr,*tfptr;
	char line[90];
	int code_line,cell_line_ID,species_line_ID,iTitleID;
	float par = 0.0f;
	float b11,b22,b33,b12,b13,b23;
	double a,b,c,ar,br,cr,Vol,ca,cb,cg,sa,sb,sg;
	char string[80];
	float dif;


	dif = 1.0;
	if (use_iso == 1) {dif = (float)(dif*SPI8);}
//	sprintf(t_FileName,"");
//	sprintf(t_FileName,"%s",TEMPLATE);
//------------------------------------------ Reade lines from template file
    fptr = fopen(t_FileName,"r");
	tfptr = tmpfile();
	while(!feof(fptr)){
		sprintf(line,"");
		fgets( line, sizeof(line), fptr); 
		fprintf(tfptr,"%s",line);
	}
	fclose(fptr);
	rewind(tfptr);
	fptr = fopen(fname,"w");
	cell_line_ID = 0;
	species_line_ID = 0;
	iTitleID = 0;
	while(!feof(tfptr)){
		code_line = 0;
		sprintf(line,"");
		fgets( line, sizeof(line), tfptr); 
		if (strstr(line,"Title") != NULL) {
			if (strstr(line,"#") != NULL) code_line = 1; //--- Title
			else code_line = 11; //--- Title (MEM)
		}
		if ((strstr(line,PHNAME)  != NULL) && (line[0] != '#')) code_line = 2;          //--- PHANME
		if ((strstr(line,VNS)     != NULL) && (line[0] != '#')) code_line = 3;          //--- SPGR
		if ((strstr(line,CELQ1)   != NULL) && (line[0] != '#') && (cell_line_ID == 0)){ //--- CELL
			cell_line_ID = 1;
			code_line = 4;
		}
		if (code_line == 1) {
			if (iTitleID == 1) code_line = 0;
		}
		if ((strstr(line,CELQ2)   != NULL) && (line[0] != '#') && (cell_line_ID == 0)){ //--- CELL
			cell_line_ID = 1;
			code_line = 4;
		}
		if ((line[0] == ' ') && (line[3] == '\'')){
			if (species_line_ID < 2) {
				code_line = 5;
				species_line_ID++;
			}
			else code_line = 0;
		}
		if (code_line == 0) fputs(line, fptr);
		else {
			switch(code_line)
			{
			case(1):{
				i = strlen(Title)-1;
				iTitleID = 1;
				fputs(line, fptr);
				fgets( line, sizeof(line), tfptr); 
				sprintf(line,"");
				strncat(line,Title,i);
				strcat(line,"\n");
				fputs(line, fptr);
				break;
					}
			case(11):{
				i = strlen(Title)-1;
				sprintf(line,"");
				sprintf(line,"   TITLE = '");
				strncat(line,Title,i);
				strncat(line,"'",1);
				fputs(line, fptr);
				break;
					 }
			case(2):{
				i = strlen(Title)-1;
				sprintf(line,"");
				sprintf(line,"PHNAME1 = '");
				strncat(line,Title,i);
				strcat(line,"': Phase name (CHARACTER*25).\n");
				fputs(line, fptr);
				break;
					}
			case(3):{
				fprintf(fptr,"VNS1 = 'A-%i-%i': (Vol.No. of Int.Tables: A or I)-(Space group No)-(Setting No).\n",
  			      cCell.SpGrN,cCell.Setting);
				break;
					}
			case(5):{
				if (species_line_ID  == 1) {
					strcpy(string, "  ");
					for(i=0; i<tAtom; i++){
						strcat(string," '");
						strcat(string,AtomT[i].symb);
						strcat(string,"'");
						strcat(string," 0.0 ");
					}
					strcat(string,"/\n");
					fputs(string, fptr);
				}
				if (species_line_ID  == 2) {
					strcpy(string, "  ");
					for(i=0; i<tAtom; i++){
						strcat(string,"  '");
						strcat(string,AtomT[i].symb);
						strcat(string,"' ");
					}
					strcat(string,"/\n");
					fputs(string, fptr);
				}
				break;
					}
			case(4):{
				fprintf(fptr,"CELLQ%9.5f%9.5f%9.5f%9.3f%9.3f%9.3f%5.1f  0000000\n",
		             cCell.a[0],cCell.a[1],cCell.a[2],cCell.a[3],cCell.a[4],cCell.a[5],par);
				while(code_line != 0){
					sprintf(line,"");
					fgets( line, sizeof(line), tfptr); 
					if ((line[0] == ' ') || (line[0] == '#')) fputs(line, fptr);
					if (line[0] == '}') {
						switch(export_mode)
						{
						case(1):{
							for(i=0; i<nAtom; i++){
								fprintf(fptr,"%s/%s  %5.3f %8.5f %8.5f %8.5f %6.3f  00000\n",
									    lAtom[i].name,lAtom[i].simb,lAtom[i].g,
										lAtom[i].x,lAtom[i].y,lAtom[i].z,lAtom[i].Biso*dif);
							}
							break;
								}
						case(2):{
							a = cCell.a[0];
							b = cCell.a[1];
							c = cCell.a[2];
							ca = cos(cCell.a[3]*RAD); sa = sin(cCell.a[3]*RAD); 
							cb = cos(cCell.a[4]*RAD); sb = sin(cCell.a[4]*RAD);
							cg = cos(cCell.a[5]*RAD); sg = sin(cCell.a[5]*RAD);
							Vol = a*b*c*sqrt(1.0 - ca*ca - cb*cb - cg*cg + 2.0*ca*cb*cg);
							ar = b*c*sa/Vol;
							br = a*c*sb/Vol;
							cr = a*b*sg/Vol;
							for(i=0; i<nAtom; i++){
								b11 = lAtom[i].B[0];
								b22 = lAtom[i].B[1];
								b33 = lAtom[i].B[2];
								b12 = lAtom[i].B[3];
								b13 = lAtom[i].B[4];
								b23 = lAtom[i].B[5];
								if (Btype == 0) {
									b11 *= (float)TR[0];
									b22 *= (float)TR[1];
									b33 *= (float)TR[2];
									b12 *= (float)TR[3];
									b13 *= (float)TR[4];
									b23 *= (float)TR[5];
								}
								fprintf(fptr,"%s/%s  %5.3f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f\n",
									lAtom[i].name,lAtom[i].simb,lAtom[i].g,
									lAtom[i].x,lAtom[i].y,lAtom[i].z,b11,b22,b33);
								fprintf(fptr,"%20.5f %8.5f %8.5f  0000000000\n",b12,b13,b23);
							}
							break;
								}
						}
						fputs(line, fptr);
						code_line = 0;
					}
				}
				code_line = 0;
				break;
					}
			}
		}
	}
	fclose(fptr);
	fclose(tfptr);
}

//*******************************************************************************
void exportXYZfile(char *fname)
{
	FILE *fptr;
	int i,I1;
	float x,y,z;

	fptr = fopen(fname,"w");
	fprintf(fptr,"%i\n", nXYZ);
	fprintf(fptr,"%s\n", Title);
	for(i=0; i<nXYZ; i++){
		I1 = m[i].Num;
		x = m[i].x*cCell.max_P;
		y = m[i].y*cCell.max_P;
		z = m[i].z*cCell.max_P;
		fprintf(fptr,"%2s %10.5f %10.5f %10.5f\n",lAtom[I1].simb,x,y,z);
	}
	fclose(fptr);
}

//*******************************************************************************
void exportCHEM3Dfile(char *fname)
{
	FILE *fptr;
	int i,j,I1,I2;
	float x,y,z;

	fptr = fopen(fname,"w");
	fprintf(fptr,"%i\n", nXYZ);
	for(i=0; i<nXYZ; i++){
		I1 = m[i].Num;
		x = m[i].x*cCell.max_P;
		y = m[i].y*cCell.max_P;
		z = m[i].z*cCell.max_P;
		fprintf(fptr,"%s\t%i\t%8.5f%10.5f%10.5f",lAtom[I1].simb,i+1,x,y,z);
		if (m[i].bondNum>0){
			for(j=0; j<m[i].bondNum; j++){
				I2 = m[i].b_list[j] + 1;
				fprintf(fptr,"%5i",I2);
			}
		}
		fprintf(fptr,"\n");
	}
	fclose(fptr);
}

//*******************************************************************************
void exportPDBfile(char *fname)
{
	FILE *fptr;
	int i,j,I1;
	float x,y,z;
	char line[80],param[80];


	fptr = fopen(fname,"w");
	sprintf(line,"");
	sprintf(line,"COMPND    ");
	strncat(line, Title, strlen(Title));
	fprintf(fptr,"%s\n",line);
	sprintf(line,"");
	sprintf(line,"COMPND   1");
	strcat(line, "Created by VICS");
	fprintf(fptr,"%s\n",line);
	for(i=0; i<nXYZ; i++){
		I1 = m[i].Num; 
		x = m[i].x*cCell.max_P;
		y = m[i].y*cCell.max_P;
		z = m[i].z*cCell.max_P;
		if (strlen(lAtom[I1].simb) == 2) {
			fprintf(fptr,"%6s%5i %s%16c%8.3f%8.3f%8.3f%6.2f%6.2f%10c%2s\n",
		   "HETATM",i+1,lAtom[I1].simb,' ',x,y,z,lAtom[I1].g,lAtom[I1].Biso,' ',lAtom[I1].simb);
		}
		else {
			fprintf(fptr,"%6s%5i %s%17c%8.3f%8.3f%8.3f%6.2f%6.2f%10c%2s\n",
		   "HETATM",i+1,lAtom[I1].simb,' ',x,y,z,lAtom[I1].g,lAtom[I1].Biso,' ',lAtom[I1].simb);
		}
	}
	for(i=0; i<nXYZ; i++){
		if (m[i].bondNum > 0){
			sprintf(line,"");
			sprintf(line,"%6s%5i","CONECT",i+1);
			for(j=0; j<m[i].bondNum; j++){
				sprintf(param,"");
				sprintf(param,"%5i",m[i].b_list[j]+1);
				strncat(line,param,strlen(param));
			}
			fprintf(fptr,"%s\n",line);
		}
	}
	fprintf(fptr,"%s\n","END");
   	fclose(fptr);
}


//****************************************************************//
//                 Export Data to Format                          //
//----------------------------------------------------------------//
// control ID = 0 -- CIF             File                         //
//              1 -- PDB             File                         //
//              2 -- Rietan *.ins    File                         //
//              3 -- XMol XYZ        File                         //
//****************************************************************//

//*******************************************************************************
void export_data(int ID, char *fname)
{
	switch(ID){
//--------------------------------------- exportCIFfile
	case(0):{exportCIFfile(fname);break;}
//--------------------------------------- exportPDBfile
	case(1):{exportPDBfile(fname);break;}
//--------------------------------------- exportRIETANfile
	case(2):{exportRIETANfile(fname);break;}
//--------------------------------------- exportXYZfile
	case(3):{exportXYZfile(fname);break;}
	}

}


//*******************************************************************************
void get_symmetry_string(int N, char *line)
{
	int i,j,m;
	char par[6];
	int check;
	double delta;
	double D[11] = {0.250000, 0.500000, 0.750000, 
		            0.333333, 0.666666, 0.166666, 0.833333,
					0.083333, 0.416666, 0.583333, 0.916666};

	for(i=0; i<3; i++){
		check = 0;
		if (sPos[N].a[0][i] == -1) {strcat(line,"-x"); check = 1;}
		if (sPos[N].a[0][i] ==  1) {strcat(line,"x");  check = 1;}

		if (sPos[N].a[1][i] == -1) {strcat(line,"-y"); check = 1;}
		if (sPos[N].a[1][i] ==  1) {
			if (check == 0) {strcat(line,"y"); check = 1;}
			else            {strcat(line,"+y"); check = 1;}
		}
		if (sPos[N].a[2][i] == -1) {strcat(line,"-z"); check = 1;}
		if (sPos[N].a[2][i] ==  1) {
			if (check == 0) {strcat(line,"z");  check = 1;}
			else            {strcat(line,"+z"); check = 1;}
		}
		if (sPos[N].t[i] != 0) {
			strcat(line,"+");
			sprintf(par,"");
			sprintf(par,"%5.3f",sPos[N].t[i]);
			m = 0;
			for(j=0; j<11; j++){
				delta = fabs(sPos[N].t[i] - D[j]);
				if (delta <= 0.0001) {m = j+1; break;}
			}
			if (sPos[N].t[i] == 1) m = 12;

			switch(m)
			{
			case( 0):{strcat(line,par);     break;}
			case( 1):{strcat(line,"1/4");   break;}
			case( 2):{strcat(line,"1/2");   break;}
			case( 3):{strcat(line,"3/4");   break;}
			case( 4):{strcat(line,"1/3");   break;}
			case( 5):{strcat(line,"2/3");   break;}
			case( 6):{strcat(line,"1/6");   break;}
			case( 7):{strcat(line,"5/6");   break;}
			case( 8):{strcat(line,"1/12");  break;}
			case( 9):{strcat(line,"5/12");  break;}
			case(10):{strcat(line,"7/12");  break;}
			case(11):{strcat(line,"11/12"); break;}
			case(12):{strcat(line,"1");     break;}
			}
		}
		if (i < 2) strcat(line,",");
	}
}

//***************************************************************************//
VECTOR3D make_symmetry(int N, int Trans[3],VECTOR3D U)
{
	float x,y,z;
	float X0,Y0,Z0;
	VECTOR3D V;

//-------------- settings control for orthorhombic cell
	X0 = U.x; Y0 = U.y; Z0 = U.z;
	if ((cCell.SpGrN >= 16) && (cCell.SpGrN <= 74)){
		switch(cCell.Setting)
		{
		case(1):{X0 = U.x; Y0 = U.y; Z0 =  U.z; break;}
		case(2):{X0 = U.y; Y0 = U.x; Z0 = -U.z; break;}
		case(3):{X0 = U.y; Y0 = U.z; Z0 =  U.x; break;}
		case(4):{X0 = U.z; Y0 = U.y; Z0 = -U.x; break;}
		case(5):{X0 = U.z; Y0 = U.x; Z0 =  U.y; break;}
		case(6):{X0 = U.x; Y0 = U.z; Z0 = -U.y; break;}
		}
	}
	x=sPos[N].t[0] + X0*sPos[N].a[0][0] + Y0*sPos[N].a[1][0] + Z0*sPos[N].a[2][0];
	y=sPos[N].t[1] + X0*sPos[N].a[0][1] + Y0*sPos[N].a[1][1] + Z0*sPos[N].a[2][1];
	z=sPos[N].t[2] + X0*sPos[N].a[0][2] + Y0*sPos[N].a[1][2] + Z0*sPos[N].a[2][2];
	X0 = x; Y0 = y; Z0 = z;
	if ((cCell.SpGrN >= 16) && (cCell.SpGrN <= 74)){
		switch(cCell.Setting)
		{
		case(1):{X0 =  x; Y0 = y; Z0 =  z; break;}
		case(2):{X0 =  y; Y0 = x; Z0 = -z; break;}
		case(3):{X0 =  z; Y0 = x; Z0 =  y; break;}
		case(4):{X0 = -z; Y0 = y; Z0 =  x; break;}
		case(5):{X0 =  y; Y0 = z; Z0 =  x; break;}
		case(6):{X0 =  x; Y0 =-z; Z0 =  y; break;}
		}
	}
	V.x = X0; V.y = Y0; V.z = Z0;
/**
	Trans[0] = Trans[1] = Trans[2] = 0;
	if (V.x < 0) Trans[0] = 1; if (V.x >= 1) Trans[0] = -1;
	if (V.y < 0) Trans[1] = 1; if (V.y >= 1) Trans[1] = -1;
	if (V.z < 0) Trans[2] = 1; if (V.z >= 1) Trans[2] = -1;
**/
	V.x += Trans[0];
	V.y += Trans[1];
	V.z += Trans[2];
	return V;
}


//***************************************************************************//
void check_deleted_atoms()
{
	int i,I1;

	for(i=0; i<del_Object.nAtom; i++) {
		I1 = del_Object.del_Atom[i];
		m[I1].show = 'd';
	}
	for(i=0; i<del_Object.nBonds; i++) {
		I1 = del_Object.del_Bonds[i];
		bAtom[I1].show = 'd';
	}
	for(i=0; i<del_Object.nPoly; i++) {
		I1 = del_Object.del_Poly[i];
		tp[I1].show = 'd';
	}
}


//***************************************************************************//
int bonds_for_molecule(int ID)
{
	int i,j,k,I1,I2,I3;
	float dx,dy,dz;
	double dist,fi;
	char *par1,*par2,*par3,*par4;
	int code;

	bond_sNum = 0;
	nBonds = 0;

	for(i=0; i<nXYZ; i++){
		if (m[i].bondNum > 0){
			I1 = m[i].Num;
			for(j=0; j<m[i].bondNum; j++){
				I3 = m[i].b_list[j];
				I2 = m[I3].Num;
				dx = (m[I3].x - m[i].x); 
				dy = (m[I3].y - m[i].y);
				dz = (m[I3].z - m[i].z);
				dist = sqrt(dx*dx + dy*dy + dz*dz);
				fi = acos(dz/dist)*180.0/PI;
				if (dx==0 && dy==0) { 
					if (dz>0) {fi = 0;} 
					else {fi = 180;}
				}
				code = 0;
				if (ID == 1) code = checkBondsDub(i,I3);
				if (code == 0){
					if (nBonds == MAX_NUM_OF_BONDS){
						printf("======================================================\n");
						printf("Number of Bonds out of Range\n nBonds=%i\n MAX_NUM_OF_BONDS=%i\n",
							nBonds+1,MAX_NUM_OF_BONDS);
						printf("======================================================\n");
						getchar();
						exit(0);
					}
					bAtom[nBonds].Atom1 = i;
					bAtom[nBonds].Atom2 = I3;
					bAtom[nBonds].dist = dist;
					bAtom[nBonds].FI = fi;
					bAtom[nBonds].show = 'y';
					nBonds++;
				}
				sprintf(bini[bond_sNum].A1,"");
				sprintf(bini[bond_sNum].A1,"%s",lAtom[I1].simb);
				sprintf(bini[bond_sNum].A2,"");
				sprintf(bini[bond_sNum].A2,"%s",lAtom[I2].simb);
				par3 = "";
				par4 = "";
				par3 = bini[bond_sNum].A1;
				par4 = bini[bond_sNum].A2;
				bini[bond_sNum].d = (float)dist*cCell.max_P;
				if (par3[0] == ' ') par3++;
				if (par4[0] == ' ') par4++;
				code = 1;
				for(k=0; k<bond_sNum; k++){
					par1 = "";
					par2 = "";
					par1 = bini[k].A1;
					par2 = bini[k].A2;
					if (par1[0] == ' ') par1++;
					if (par2[0] == ' ') par2++;
					if((strstr(par1,par3) != NULL ) && (strstr(par2,par4) != NULL )) {
						bini[k].d  = (bini[k].d >= bini[bond_sNum].d) ? bini[k].d : bini[bond_sNum].d;
						code = -1;
						break;
					}
					if((strstr(par1,par4) != NULL ) && (strstr(par2,par3) != NULL )) {
						bini[k].d  = (bini[k].d >= bini[bond_sNum].d) ? bini[k].d : bini[bond_sNum].d;
						code = -1;
						break;
					}
				}
				if (code == 1) {
					bini[bond_sNum].boundary_mode = 0;
					bini[bond_sNum].show_poly_mode = 0;
					bini[bond_sNum].search_mode = 0;
					bond_sNum++;
				}
			}
		}
	}
	for(k=0; k<bond_sNum; k++){
		bini[k].d += 0.001f;
	}

	return(0);
}


