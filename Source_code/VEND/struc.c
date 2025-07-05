//**********************************************************************************//
//                                LOAD STRUCTURE MODEL                              //
//----------------------------------------------------------------------------------//
// File: struc.c                                                                    //
// Date: 2002.03.13                                                                 //
//**********************************************************************************//
#include <stdio.h>
#include <string.h>

#include "struc.h"

#define PI			3.14159


//**************************  set_initital_model_parameters()   *************//
void set_initital_model_parameters()
{
	nAtom = 0;
	nXYZ = 0;
	bond_sNum = 0;
	tAtom = 0;
	mnPos = 0;
	nBonds = 0;
	radii_type = 0;
	model_type = 0;
	bonds_type = 1;
}

//**************************  readModelfile()   *****************************//
int readModelfile(char *fname)
{
	FILE *mfile;
	int i,j;
	char line[80];
	int mAtom;
	char *pos;
	float ex,ey,ez,ea[6],g;
	float Probability,bonds_line_width;
	int TEOL;
	char cmd[6];
	char *token;
	char param1[80];
	char seps[]   = " \t\n";


	set_initital_model_parameters();
	mAtom = 0;
    mfile=fopen(fname,"r");
    while( !feof( mfile ) )
	{
		sprintf(line,"");
		fgets( line, 80, mfile); 
//----------------------------------------------------------------- GROUP
		pos = strstr( line, "GROUP");
		if( pos != NULL ) {
			sprintf(line,"");
			fgets( line, 80, mfile); 
			sscanf(line,"%i %i",&mcCell.SpGrN,&mcCell.Setting); 
		}
//----------------------------------------------------------------- SYMOP
		pos = strstr( line, "SYMOP");
		if( pos != NULL ){
			mnPos = 0;
			i = -1;
			while(i!=0){
				if(mnPos > MAX_SYMM_POS) {
					printf("======================================================\n");
					printf("Number of Symmetry Operations out of range\n nPos=%i\n MAX_SYMM_POS=%i\n",
						   mnPos,MAX_SYMM_POS);
					printf("======================================================\n");
					fclose(mfile);
					return(-1);
				}
				fscanf( mfile, "%f %f %f %i %i %i %i %i %i %i %i %i\n", 
					   &msPos[mnPos].t[0],&msPos[mnPos].t[1],&msPos[mnPos].t[2],
					   &msPos[mnPos].a[0][0],&msPos[mnPos].a[1][0],&msPos[mnPos].a[2][0],
					   &msPos[mnPos].a[0][1],&msPos[mnPos].a[1][1],&msPos[mnPos].a[2][1],
					   &msPos[mnPos].a[0][2],&msPos[mnPos].a[1][2],&msPos[mnPos].a[2][2]);
				if (msPos[mnPos].t[0]==-1.0) {i = 0;}
				mnPos++;
            } 
			mnPos=mnPos-1;
        }
//----------------------------------------------------------------- CELLP
		pos = strstr( line, "CELLP");
		if( pos != NULL ) {
			fscanf(mfile,"%f %f %f %f %f %f\n",&mcCell.a[0],&mcCell.a[1],&mcCell.a[2],
					                         &mcCell.a[3],&mcCell.a[4],&mcCell.a[5]);
			fscanf(mfile,"%f %f %f %f %f %f\n",&ea[0],&ea[1],&ea[2],
					                           &ea[3],&ea[4],&ea[5]);
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
					                        &lAtom[nAtom].simb,&lAtom[nAtom].name,&g,
										    &lAtom[nAtom].x, &lAtom[nAtom].y, &lAtom[nAtom].z);
				fscanf(mfile, "%f %f %f\n", &ex,&ey,&ez);

				nAtom++;
            } 
			nAtom=nAtom-1;
		}
//----------------------------------------------------------------- ATOMT
		pos = strstr( line, "ATOMT");
		if( pos != NULL ) {
			i = -1;
			tAtom = 0;
			while(i!=0){
				if(tAtom >  MAX_ATOMS_SPEC) i = 0;
				else {
					fscanf(mfile,"%i %s %f %f %f %f\n",&i,&AtomT[tAtom].symb,&AtomT[tAtom].R,
				                  &AtomT[tAtom].color[0],&AtomT[tAtom].color[1],&AtomT[tAtom].color[2]);
					tAtom = tAtom + 1;
				}
			}
			tAtom = tAtom - 1;
		}
//----------------------------------------------------------------- MODEL
		pos = strstr( line, "MODEL");
		if( pos != NULL ) {sscanf(line,"%s %i",&cmd,&model_type);}

//----------------------------------------------------------------- RADII
		pos = strstr( line, "RADII");
		if( pos != NULL ) {sscanf(line,"%s %i",&cmd,&radii_type);}

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
					}
					token = strtok(NULL, seps);
					j++;
				}
				if (j == 4) bini[bond_sNum].search_mode = -1;
				bond_sNum++;
			}
		}
//----------------------------------------------------------------- BONDS
		pos = strstr( line, "BONDS");
		if( pos != NULL ){sscanf(line,"%s %i",&cmd,&bonds_type);}
//----------------------------------------------------------------- ATOMP
		pos = strstr( line, "ATOMP");
		if( pos != NULL ){
			fscanf( mfile, "%i %i %i %f\n",&atoms_stack,&atoms_slice,&TEOL,&Probability);
		}
//----------------------------------------------------------------- BONDP
		pos = strstr( line, "BONDP");
		if( pos != NULL ){
			fscanf( mfile, "%i %i %f %f %f %f %f %f\n",&bonds_stack,&bonds_slice,
				    &bRadii[0],&bRadii[1],&bonds_line_width,&BNCL[0],&BNCL[1],&BNCL[2]);
		}
//----------------------------------------------------------------- 
	}
	fclose(mfile);

	if (model_type != 4) model_type = 0;
	if (bonds_type != 1) bonds_type = 0;

	for(j=0; j<tAtom; j++){
		for (i=0; i<nAtom; i++){
			if (strcmp(lAtom[i].simb,AtomT[j].symb) == 0){
						lAtom[i].R = AtomT[j].R;
						lAtom[i].color[0] = AtomT[j].color[0];
						lAtom[i].color[1] = AtomT[j].color[1];
						lAtom[i].color[2] = AtomT[j].color[2];
			}
		}
	}

	printf("\n");
	printf("=======================================================\n");
	printf("MODEL\n");
	printf("\n");
	printf("%8.5f %8.5f %8.5f %8.4f %8.4f %8.4f\n",mcCell.a[0],mcCell.a[1],mcCell.a[2],
		                                            mcCell.a[3],mcCell.a[4],mcCell.a[5]);
	printf("\n");
	printf("%22s %10s %10s %10s\n","x","y","z");
	for(i=0; i<nAtom; i++){
		printf("%4i %3s %6s %10.5f %10.5f %10.5f\n",i+1,lAtom[i].simb,lAtom[i].name,
			            lAtom[i].x,lAtom[i].y,lAtom[i].z);
	}
	printf("=======================================================\n");
	printf("\n");

	return 0;
}



//**************************  Inside()   ************************************//
int Inside(float a, float b, float c)
{
	int ID = -1;
	if ((a>=bound[0]) && (a<=bound[1]) &&
		(b>=bound[2]) && (b<=bound[3]) &&
		(c>=bound[4]) && (c<=bound[5])) ID = 0;
	return(ID);
}


//**************************  checkDublicate()   ****************************//
int checkDublicate(float x, float y, float z)
{
	int k;
	int ID = 0;
	for (k=0; k<nXYZ; k++){
		if (x==m[k].x && y==m[k].y && z==m[k].z) ID = -1;
	}
	return(ID);
}


//**************************  CheckRange()   ********************************//
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

//**************************  addAtom()   ***********************************//
int addAtom(int Ind, float x, float y, float z)
{
	int i,j,k;
	int dub_ID;
	float dx,dy,dz;

	int iMin,iMax,jMin,jMax,kMin,kMax;

	iMin = -1; iMax = 2;
	jMin = -1; jMax = 2;
	kMin = -1; kMax = 2;

	if (bound[0]>= 1)  iMin = (int)bound[0] + 1;
	if (bound[1]>= 1)  iMax = (int)bound[1] + 2;

	if (bound[2]>= 1)  jMin = (int)bound[2] + 1;
	if (bound[3]>= 1)  jMax = (int)bound[3] + 2;

	if (bound[4]>= 1)  kMin = (int)bound[4] + 1;
	if (bound[5]>= 1)  kMax = (int)bound[5] + 2;

	for(i=iMin; i<iMax; i++){
		for(j=jMin; j<jMax; j++){
			for(k=kMin; k<kMax; k++){
				dx = x + i;
				dy = y + j;
				dz = z + k;
				if (Inside(dx,dy,dz)==0){
					dub_ID = checkDublicate(dx,dy,dz);
					if(dub_ID==0){
						nXYZ += 1;
						if (nXYZ <= MAX_NUM_OF_ATOMS){
						    m[nXYZ-1].x=dx; m[nXYZ-1].y=dy; m[nXYZ-1].z=dz;
							m[nXYZ-1].Num = Ind;
							m[nXYZ-1].bondNum = 0;
							m[nXYZ-1].bondatom = 0;
							m[nXYZ-1].show = 'y';
						}
						else {
							printf("======================================================\n");
							printf("Total Number of Atoms out of range\n nXYZ=%i\n MAX_NUM_OF_ATOMS=%i\n",
								   nXYZ,MAX_NUM_OF_ATOMS);
							printf("======================================================\n");
							return(-1);
						}
					}
				}
			}
		}
	}
	return(0);
}


//**************************  make_model_atom_pos()   ***********************//
int make_model_atom_pos()
{
	int i,j;
	int dub_ID,check_ID;
	float x,y,z,x0,y0,z0;
	float trans[3];

	nXYZ = 0;
	for(i=0;i<nAtom;i++){
		for(j=0;j<mnPos;j++){
			x0 = lAtom[i].x; y0 = lAtom[i].y; z0 =  lAtom[i].z;
			if ((mcCell.SpGrN >= 16) && (mcCell.SpGrN <= 74)){
				switch(mcCell.Setting)
				{
				case(1):{x0 = lAtom[i].x; y0 = lAtom[i].y; z0 =  lAtom[i].z;break;}
				case(2):{x0 = lAtom[i].y; y0 = lAtom[i].x; z0 = -lAtom[i].z;break;}
				case(3):{x0 = lAtom[i].y; y0 = lAtom[i].z; z0 =  lAtom[i].x;break;}
				case(4):{x0 = lAtom[i].z; y0 = lAtom[i].y; z0 = -lAtom[i].x;break;}
				case(5):{x0 = lAtom[i].z; y0 = lAtom[i].x; z0 =  lAtom[i].y;break;}
				case(6):{x0 = lAtom[i].x; y0 = lAtom[i].z; z0 = -lAtom[i].y;break;}
				}
			}
			x=msPos[j].t[0]+
			     x0*msPos[j].a[0][0]+y0*msPos[j].a[1][0]+z0*msPos[j].a[2][0];
			y=msPos[j].t[1]+
			     x0*msPos[j].a[0][1]+y0*msPos[j].a[1][1]+z0*msPos[j].a[2][1];
			z=msPos[j].t[2]+
			     x0*msPos[j].a[0][2]+y0*msPos[j].a[1][2]+z0*msPos[j].a[2][2];
			x0 = x; y0 = y; z0 = z;
			if ((mcCell.SpGrN >= 16) && (mcCell.SpGrN <= 74)){
				switch(mcCell.Setting)
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
					nXYZ += 1;
					if (nXYZ <= MAX_NUM_OF_ATOMS){
						m[nXYZ-1].x=x; m[nXYZ-1].y=y; m[nXYZ-1].z=z;
						m[nXYZ-1].Num = i;
						m[nXYZ-1].Num;
						m[nXYZ-1].bondNum = 0;
						m[nXYZ-1].bondatom = 0;
						m[nXYZ-1].show = 'y';
					}
					else {
						printf("======================================================\n");
						printf("Total Number of Atoms out of range\n nXYZ=%i\n MAX_NUM_OF_ATOMS=%i\n",
							   nXYZ,MAX_NUM_OF_ATOMS);
						printf("======================================================\n");
						return(-1);
					}
					if (addAtom(i,x,y,z) < 0) return(-1);
				}
			}
			if (addAtom(i,x,y,z) < 0) return(-1);
		}
	}
	return(0);
}

//**************************  make_orto_model_atom()   **********************//
void make_orto_matrix()
{
	int i,j;
	float cos_angles[3],sin_angles[3];
	float volume, cos_a1, sin_a1;

// Zero matrices
	for(i=0; i<3; i++){
		for(j=0; j<3; j++) matrix[i][j] = 0;
	}
// Set up sines and cosines
	for(i=0; i<3; i++){
		cos_angles[i] = (float)cos(PI*mcCell.a[i+3]/180);
		sin_angles[i] = (float)sin(PI*mcCell.a[i+3]/180);
	}
// Compute cell volume
	volume = (float) (mcCell.a[0] * mcCell.a[1] * mcCell.a[2] * 
		     sqrt(1 + 2 * cos_angles[0] * cos_angles[1] * cos_angles[2] - cos_angles[0]*cos_angles[0] 
			 - cos_angles[1]*cos_angles[1] - cos_angles[2]*cos_angles[2]));
// Compute some terms
    cos_a1 =(float) (cos_angles[1] * cos_angles[2] - cos_angles[0]) / (sin_angles[1] * sin_angles[2]);
	sin_a1 = (float) sqrt(1 - cos_a1*cos_a1);

	matrix[0][0] =   mcCell.a[0];
	matrix[0][1] =   cos_angles[2] * mcCell.a[1];
	matrix[0][2] =   cos_angles[1] * mcCell.a[2];

	matrix[1][0] =   0;
	matrix[1][1] =   sin_angles[2] * mcCell.a[1];
	matrix[1][2] = - sin_angles[1] * cos_a1 * mcCell.a[2];

	matrix[2][0] =   0;
	matrix[2][1] =   0;
	matrix[2][2] =   sin_angles[1] * sin_a1 * mcCell.a[2];
}


//**************************  make_orto_model_atom()   **********************//
void make_orto_model_atom(int NPX[3])
{
	int i;
	float x,y,z;
	float mmx,mmy,mmz;

	for(i=0; i<nXYZ; i++){
		if (m[i].show == 'y'){
			x = m[i].x * (float)NPX[0];
			y = m[i].y * (float)NPX[1];
			z = m[i].z * (float)NPX[2];
			mmx = (x - 1) / (float)(NPX[0] + 1);
			mmy = (y - 1) / (float)(NPX[1] + 1);
			mmz = (z - 1) / (float)(NPX[2] + 1);
			m[i].xo = matrix[0][0]*mmx + matrix[0][1]*mmy + matrix[0][2]*mmz;
			m[i].yo = matrix[1][0]*mmx + matrix[1][1]*mmy + matrix[1][2]*mmz;
			m[i].zo = matrix[2][0]*mmx + matrix[2][1]*mmy + matrix[2][2]*mmz;

			x = m[i].x;
			y = m[i].y;
			z = m[i].z;
			m[i].x = matrix[0][0]*x + matrix[0][1]*y + matrix[0][2]*z;
			m[i].y = matrix[1][0]*x + matrix[1][1]*y + matrix[1][2]*z;
			m[i].z = matrix[2][0]*x + matrix[2][1]*y + matrix[2][2]*z;
		}
	}
}


//**************************  checkBondsDub()   *****************************//
int checkBondsDub(int I1, int I2)
{
	int i;
	for (i=0; i<nBonds; i++){
		if ((I1 == bAtom[i].Atom1) && (I2 == bAtom[i].Atom2)) return 1;
		if ((I1 == bAtom[i].Atom2) && (I2 == bAtom[i].Atom1)) return 1;
	}
	return 0;
}


//**************************  searchBonds()   *******************************//
void get_bond_search_mode()
{
	int i;
	char *par1, *par2;

	if (bond_sNum > 0){
		for(i=0; i<bond_sNum; i++){
			if (bini[i].search_mode == -1){
				par1 = "";
				par2 = "";
				par1 = bini[i].A1;
				par2 = bini[i].A2;
				if (par1[0] == ' ') par1++;
				if (par2[0] == ' ') par2++;
				if ((strstr(par1, "XX") != NULL) && (strstr(par2, "XX") != NULL)) bini[i].search_mode = 2;
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

int searchBonds(float maxPar)
{
	int i,j,k;
	double dx,dy,dz;
	double dist,fi;
	char *pos;
	char *par1,*par2;
	int I1, I2;
	int check,Num;

	get_bond_search_mode();
	for(i=0; i<nXYZ; i++) {
		m[i].bondNum = 0;
//		m[i].show = 'y';
	}
	nBonds = 0;
	for(i=0; i<bond_sNum; i++){
		par1 = bini[i].A1;
		par2 = bini[i].A2;
		if (par1[0] == ' ') par1++;
		if (par2[0] == ' ') par2++;
		for(j=0; j<nXYZ; j++){
		if (m[j].show == 'y'){
			Num = 0;
			check = 1;
			I1 = m[j].Num;
			if (bini[i].search_mode != 2) {
				pos = strstr(lAtom[I1].simb, par1);
				if ((pos != NULL) && (strlen(par1) == strlen(lAtom[I1].simb))) check = 1;
				else check = -1;
			}
			if (check > 0) {
				for(k=0; k<nXYZ; k++){
				if (m[k].show == 'y'){
					I2 = m[k].Num;
					check = 1;
					if (bini[i].search_mode == 0) {
						pos = strstr(lAtom[I2].simb, par2);
						if((pos != NULL) && (strlen(par2) == strlen(lAtom[I2].simb))) check = 1;
						else check = -1;
					}
					if (check > 0){
						dx = (m[j].x - m[k].x);
						dy = (m[j].y - m[k].y);
						dz = (m[j].z - m[k].z);
						dist = sqrt(dx*dx + dy*dy + dz*dz);
						if ((dist<=bini[i].d) && (dist > 0)){
							if (checkBondsDub(j,k) == 0){
								if (nBonds == MAX_NUM_OF_BONDS){
									printf("======================================================\n");
									printf("Number of Bonds out of Range\n nBonds=%i\n MAX_NUM_OF_BONDS=%i\n",
										   nBonds+1,MAX_NUM_OF_BONDS);
									printf("======================================================\n");
									return(-1);
								}
								dx = (m[k].x - m[j].x)/maxPar; 
								dy = (m[k].y - m[j].y)/maxPar;
								dz = (m[k].z - m[j].z)/maxPar;
								dist = sqrt(dx*dx + dy*dy + dz*dz);
								fi = acos(dz/dist)*180.0/PI;
								if (dx==0.0 && dy==0.0) { 
									if (dz >= 0.0) {fi = 0.0f;} 
									else {fi = 180.0f;}
								}
								bAtom[nBonds].Atom1 = j;
								bAtom[nBonds].Atom2 = k;
								bAtom[nBonds].dist = dist;
								bAtom[nBonds].FI = fi;
								m[k].bondatom = 1;
								nBonds += 1;
							}
							else m[k].bondatom = 1;
						}
					}
				} 
				} //--- if (m[k].show == 'y')
			}
		}  //--- if (m[i].show == 'y')
		}
	}

	return(0);
}

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
}
