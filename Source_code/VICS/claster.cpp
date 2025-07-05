#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vics.h"



typedef struct clastertag
{
	int Num;
	int Ind;      //--- Atomic Index
	float P[3];   //--- Position (x,y,z)
	int S;        //--- Symmetry operation number
	int T[3];     //--- Extra translation  (Tx, Ty, Tz)
	int Extra;
	char show;
} CLASTER;

typedef struct neighbtag
{
	CLASTER A;                       //--- Central Atom Index
	int nNA;                         //--- Number of neighbour atoms
	CLASTER NA[MAX_BONDS_FOR_ATOM];  //--- List of neighbour atoms (Index)
} NEIGHB;

CLASTER C[MAX_NUM_OF_ATOMS];
int nC;
CLASTER Q[MAX_NUM_OF_ATOMS];
int nQ;
CLASTER NN[MAX_NUM_OF_ATOMS];
int nNN;
CLASTER cE;

NEIGHB NB[MAX_NUM_OF_ATOMS];
int nNB;

ATOMTYPE _lAtom[MAX_ATOMS_IN_ASYMM];
int _nAtom;

BONDSINI _bini[MAX_BONDS_SPEC];
int _bond_sNum;

SYMMPOS _sPos[MAX_SYMM_POS];
int _nPos;

ATOM BR[MAX_NUM_OF_ATOMS];
int nBR;

double Mat[3][3];                  //--- Orthogonal Matrix
int SGN;                           //--- Space Group Number      
int SGS;                           //--- Space Group Sewtting
float cell_param[6];               //--- Unit Cell parameters
int Box[6];                        //--- Plot Range Box
float brange[6];                   //--- Plot Range
int Trans[3];
int file_format_ID;

//***************************************************************************//
void _get_plot_range_box()
{
	Box[0] = -2; Box[1] = 2;
	Box[2] = -2; Box[3] = 2;
	Box[4] = -2; Box[5] = 2;

	if (brange[0]<=-1)  Box[0] = (int)brange[0] - 1;
	if (brange[0]>  1)  Box[0] = (int)brange[0] + 1;
	if (brange[1]<=-1)  Box[1] = (int)brange[1] + 2;
	if (brange[1]>  1)  Box[1] = (int)brange[1] + 2;

	if (brange[2]<=-1)  Box[2] = (int)brange[2] - 1;
	if (brange[2]>  1)  Box[2] = (int)brange[2] + 1;
	if (brange[3]<=-1)  Box[3] = (int)brange[3] + 2;
	if (brange[3]>  1)  Box[3] = (int)brange[3] + 2;

	if (brange[4]<=-1)  Box[4] = (int)brange[4] - 1;
	if (brange[4]>  1)  Box[4] = (int)brange[4] + 1;
	if (brange[5]<=-1)  Box[5] = (int)brange[5] + 2;
	if (brange[5]>  1)  Box[5] = (int)brange[5] + 2;
}

//***************************************************************************//
void _calc_ortho_matrix()
{
	double a,b,c,al,bt,gm,V;
	double cosA,cosB,cosG,sinA,sinB,sinG;
	double sinAR;
	double cosAR, cosBR, cosGR;

	a = cell_param[0]; al = cell_param[3];
	b = cell_param[1]; bt = cell_param[4];
	c = cell_param[2]; gm = cell_param[5];
	cosA = cos(al*PI/180); sinA = sqrt(1 - cosA*cosA);
	cosB = cos(bt*PI/180); sinB = sqrt(1 - cosB*cosB);
	cosG = cos(gm*PI/180); sinG = sqrt(1 - cosG*cosG);
	V = a*b*c*sqrt(1 - cosA*cosA - cosB*cosB - cosG*cosG + 2*cosA*cosB*cosG);

	cosAR = (cosB*cosG - cosA)/(sinB*sinG);
	cosBR = (cosA*cosG - cosB)/(sinA*sinG);
	cosGR = (cosA*cosB - cosG)/(sinA*sinB);

	sinAR = sqrt(1 - cosAR*cosAR);

	Mat[0][0] = 1; Mat[0][1] = cosG; Mat[0][2] = cosB;
	Mat[1][0] = 0; Mat[1][1] = sinG; Mat[1][2] = -sinB*cosAR;
	Mat[2][0] = 0; Mat[2][1] = 0;    Mat[2][2] = sinB*sinAR;
}

//***************************************************************************//
VECTOR3D _make_symmetry_operation(int N,VECTOR3D U)
{
	float x,y,z;
	float X0,Y0,Z0;
	VECTOR3D V;
	int ID;

	ID = file_format_ID;
//-------------- settings control for orthorhombic cell
	X0 = U.x; Y0 = U.y; Z0 = U.z;
	if ((SGN >= 16) && (SGN <= 74)){
		switch(SGS)
		{
		case(1):{X0 = U.x; Y0 = U.y; Z0 =  U.z; break;}
		case(2):{X0 = U.y; Y0 = U.x; Z0 = -U.z; break;}
		case(3):{X0 = U.y; Y0 = U.z; Z0 =  U.x; break;}
		case(4):{X0 = U.z; Y0 = U.y; Z0 = -U.x; break;}
		case(5):{X0 = U.z; Y0 = U.x; Z0 =  U.y; break;}
		case(6):{X0 = U.x; Y0 = U.z; Z0 = -U.y; break;}
		case(7):{X0 = U.x; Y0 = U.y; Z0 =  U.z; break;}
		case(8):{X0 = U.y; Y0 = U.x; Z0 = -U.z; break;}
		case(9):{X0 = U.y; Y0 = U.z; Z0 =  U.x; break;}
		case(10):{X0 = U.z; Y0 = U.y; Z0 = -U.x; break;}
		case(11):{X0 = U.z; Y0 = U.x; Z0 =  U.y; break;}
		case(12):{X0 = U.x; Y0 = U.z; Z0 = -U.y; break;}
		}
	}
	x=_sPos[N].t[0] + X0*_sPos[N].a[0][0] + Y0*_sPos[N].a[1][0] + Z0*_sPos[N].a[2][0];
	y=_sPos[N].t[1] + X0*_sPos[N].a[0][1] + Y0*_sPos[N].a[1][1] + Z0*_sPos[N].a[2][1];
	z=_sPos[N].t[2] + X0*_sPos[N].a[0][2] + Y0*_sPos[N].a[1][2] + Z0*_sPos[N].a[2][2];
	X0 = x; Y0 = y; Z0 = z;
	if ((SGN >= 16) && (SGN <= 74)){
		switch(SGS)
		{
		case(1):{X0 =  x; Y0 = y; Z0 =  z; break;}
		case(2):{X0 =  y; Y0 = x; Z0 = -z; break;}
		case(3):{X0 =  z; Y0 = x; Z0 =  y; break;}
		case(4):{X0 = -z; Y0 = y; Z0 =  x; break;}
		case(5):{X0 =  y; Y0 = z; Z0 =  x; break;}
		case(6):{X0 =  x; Y0 =-z; Z0 =  y; break;}
		case(7):{X0 =  x; Y0 = y; Z0 =  z; break;}
		case(8):{X0 =  y; Y0 = x; Z0 = -z; break;}
		case(9):{X0 =  z; Y0 = x; Z0 =  y; break;}
		case(10):{X0 = -z; Y0 = y; Z0 =  x; break;}
		case(11):{X0 =  y; Y0 = z; Z0 =  x; break;}
		case(12):{X0 =  x; Y0 =-z; Z0 =  y; break;}
		}
	}
	V.x = X0; V.y = Y0; V.z = Z0;
	Trans[0] = Trans[1] = Trans[2] = 0;
//	if ((ID == 2) || (ID == 9) || (ID == 10) || (ID == 14)) {}
//	else {
	if (ID != 16)  {
		if (V.x < 0) Trans[0] = 1; if (V.x >= 1) Trans[0] = -1;
		if (V.y < 0) Trans[1] = 1; if (V.y >= 1) Trans[1] = -1;
		if (V.z < 0) Trans[2] = 1; if (V.z >= 1) Trans[2] = -1;
		V.x += Trans[0];
		V.y += Trans[1];
		V.z += Trans[2];
	}
	return V;
}

//***************************************************************************//
int _check_atom_dublicat(VECTOR3D V, int Num)
{
	int i;
	double d1,d2,d3;
	for (i=0; i<Num; i++){
		d1 = fabs(V.x - C[i].P[0]);
		d2 = fabs(V.y - C[i].P[1]);
		d3 = fabs(V.z - C[i].P[2]);
		if ((d1 <= 0.001) && (d2 <= 0.001) && (d3 <= 0.001)) return(0);
	}
	return (-1);
}

//***************************************************************************//
int _check_atom_inside_box(VECTOR3D V)
{
	if ((V.x >= brange[0]) && (V.x <= brange[1]) && 
		(V.y >= brange[2]) && (V.y <= brange[3]) &&
		(V.z >= brange[4]) && (V.z <= brange[5])) return 1;
	return -1;
}

//***************************************************************************//
int _collect_sell_info()
{
	int i,j;
	int l,k,m;
	VECTOR3D V1,V2,V3;

	nC = 0;
	for(i=0; i<_nAtom; i++){
		for(j=0; j<_nPos; j++){
			V1.x = _lAtom[i].x;
			V1.y = _lAtom[i].y;
			V1.z = _lAtom[i].z;
			V2 = _make_symmetry_operation(j,V1);
			for(l=Box[4]; l<Box[5]; l++){
				for(k=Box[2]; k<Box[3]; k++){
					for(m=Box[0]; m<Box[1]; m++){
						V3.x = V2.x + (float)m;
						V3.y = V2.y + (float)k;
						V3.z = V2.z + (float)l;
						if (_check_atom_inside_box(V3) > 0){
							if (_check_atom_dublicat(V3,nC) < 0){
								if (nC >= MAX_NUM_OF_ATOMS) return(-1);
								C[nC].Num  = nC;
								C[nC].Ind  = i;
								C[nC].P[0] = V3.x;
								C[nC].P[1] = V3.y;
								C[nC].P[2] = V3.z;
								C[nC].S    = j;
								C[nC].T[0] = Trans[0] + m;
								C[nC].T[1] = Trans[1] + k;
								C[nC].T[2] = Trans[2] + l;
								C[nC].Extra = 0;
								nC++;
							}
						}
					}
				}
			}
		}
	}
	return(0);
}

//***************************************************************************//
int _check_dublicat_in_claster(VECTOR3D V,int Num)
{
	int i;
	float x,y,z;

	for(i=0; i<Num; i++){
		x = C[i].P[0];
		y = C[i].P[1];
		z = C[i].P[2];
		if ((x == V.x) && (y == V.y) && (z == V.z)) {
			C[i].Extra = 1;
			return i;
		}
	}
	return (-1);
}


//***************************************************************************//
int _check_neighbors_atoms_in_claster(int sNum)
{
	int i;
	int Add_C, Add_Q;
	VECTOR3D V;
	int check;

	Add_C = nC;
	Add_Q = nQ;
	for(i=0; i<nNN; i++){
		V.x = NN[i].P[0]; 
		V.y = NN[i].P[1]; 
		V.z = NN[i].P[2]; 
		check = _check_dublicat_in_claster(V,Add_C);
		if (check < 0){
			if (Add_C >= MAX_NUM_OF_ATOMS) return -1;
			C[Add_C] = NN[i];
			C[Add_C].Num = Add_C;
			C[Add_C].Extra = 1;
			NN[i].Num = Add_C;
			Add_C++;
			if (_bini[sNum].search_mode == 2) {
				Q[Add_Q] = NN[i];
				Add_Q++;
			}
		}
		else NN[i].Num = check;
	}
	nC = Add_C;
	nQ = Add_Q;
	return 0;
}

//***************************************************************************//
int _check_in_list(int I1, int I2, int sNum)
{
	char *par1,*par2;
	char *pos1,*pos2;
	int L1,L2,LP1,LP2;

	if (_bini[sNum].search_mode == 2) return(1);
	par1 = _bini[sNum].A1;
	par2 = _bini[sNum].A2;
	if (par1[0] == ' ') par1++;
	if (par2[0] == ' ') par2++;
	L1 = strlen(_lAtom[I1].simb);
	L2 = strlen(_lAtom[I2].simb);
	LP1 = strlen(par1);
	LP2 = strlen(par2);
	pos1 = strstr(_lAtom[I1].simb, par1);
	pos2 = strstr(_lAtom[I2].simb, par2);

	switch(_bini[sNum].search_mode)
	{
	case(0):{
		if ((L1 == LP1) && (L2 == LP2) && (pos1 != NULL) && (pos2 != NULL)) return(1);
		break;
			}
	case(1):{
		if ((L1 == LP1) && (pos1 != NULL)) return(1);
		break;
			}
	}

	return(0);
}

//***************************************************************************//
double _calc_b_distance(double a1x, double a1y, double a1z, double a2x, double a2y, double a2z)
{
	double x,y,z;
	double x1,y1,z1;
	double x2,y2,z2;
	double dx,dy,dz,d;
	x = a1x * cell_param[0];
	y = a1y * cell_param[1];
	z = a1z * cell_param[2];
	x1 = x*Mat[0][0] + y*Mat[0][1] + z*Mat[0][2];
	y1 = x*Mat[1][0] + y*Mat[1][1] + z*Mat[1][2];
	z1 = x*Mat[2][0] + y*Mat[2][1] + z*Mat[2][2];
	x = a2x * cell_param[0];
	y = a2y * cell_param[1];
	z = a2z * cell_param[2];
	x2 = x*Mat[0][0] + y*Mat[0][1] + z*Mat[0][2];
	y2 = x*Mat[1][0] + y*Mat[1][1] + z*Mat[1][2];
	z2 = x*Mat[2][0] + y*Mat[2][1] + z*Mat[2][2];
	dx = x2 - x1;
	dy = y2 - y1;
	dz = z2 - z1;
	d = sqrt(dx * dx + dy * dy + dz * dz);
	return(d);
}

//***************************************************************************//
double _check_is_it_neighbour(VECTOR3D V1,VECTOR3D V2, double dist)
{
	double x1,y1,z1;
	double x2,y2,z2;
	double d;


	x1 = fabs(V1.x - V2.x);
	y1 = fabs(V1.y - V2.y);
	z1 = fabs(V1.z - V2.z);
	if ((x1 <= 0.001) && (y1 <= 0.001) && (z1 <= 0.001)) return(0);

//	if ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z)) return(0);
	x1 = V1.x;	y1 = V1.y;	z1 = V1.z;
	x2 = V2.x;	y2 = V2.y;	z2 = V2.z;
	d = _calc_b_distance(x1,y1,z1,x2,y2,z2);
	if (d <= dist) return d;

	return 0;
}

//***************************************************************************//
int _check_dublicat_in_neighbour_list(VECTOR3D V, int Num)
{
	int i;
	double d1,d2,d3;

	for(i=0; i<Num; i++){
		d1 = fabs(NN[i].P[0] - V.x);
		d2 = fabs(NN[i].P[1] - V.y);
		d3 = fabs(NN[i].P[2] - V.z);
		if ((d1 <=0.001) && (d2 <=0.001) && (d3 <=0.001)) return(-1);
	}
	return (1);
}


//***************************************************************************//
int _get_neighbors_atoms_list()
{
	int i,j,b;
	int I1,I2;
	int k,l,m;
	double bond_dist;
	VECTOR3D V1,V2,V3,V4;
	double D;
	int check;

	I1 = cE.Ind;
	V1.x = cE.P[0];
	V1.y = cE.P[1];
	V1.z = cE.P[2];
	nNN = 0;
	for(b=0; b<_bond_sNum; b++) {
		bond_dist = _bini[b].d;
		for(i=0; i<_nAtom; i++){
			I2 = i;
			if (_check_in_list(I1,I2,b) > 0){
				V2.x = _lAtom[i].x;
				V2.y = _lAtom[i].y;
				V2.z = _lAtom[i].z;
				for(j=0; j<_nPos; j++){
					V3 = _make_symmetry_operation(j,V2);
					for(l=Box[4]; l<=Box[5]; l++){
						for(k=Box[2]; k<=Box[3]; k++){
							for(m=Box[0]; m<=Box[1]; m++){
								V4.x = V3.x + (float)m;
								V4.y = V3.y + (float)k;
								V4.z = V3.z + (float)l;
								check = 1;
								D = 0;
								if (_bini[b].boundary_mode == 0) {
									check = _check_atom_inside_box(V4);
								}
								D = _check_is_it_neighbour(V1,V4,bond_dist);
								if (D > 0.1){
									if (_check_dublicat_in_neighbour_list(V4,nNN) > 0){
										NN[nNN].Ind  = i;
										NN[nNN].P[0] = V4.x;
										NN[nNN].P[1] = V4.y;
										NN[nNN].P[2] = V4.z;
										NN[nNN].S    = j;
										NN[nNN].T[0] = Trans[0] + m;
										NN[nNN].T[1] = Trans[1] + k;
										NN[nNN].T[2] = Trans[2] + l;
										if (check < 0) NN[nNN].show = 'n';
										else NN[nNN].show = 'y';
										nNN++;
									}
								}
							}
						}
					}
				}
			}
		}
		if (nNN > 0){
			if (_check_neighbors_atoms_in_claster(b) < 0) return -1;
		}
	}
	return nNN;
}



//***************************************************************************//
void _init_claster_queue_list()
{
	int i;

	nQ = nC;
	for(i=0; i<nC; i++){
		Q[i] = C[i];
		Q[i].Extra = 0;
	}
}

//***************************************************************************//
void _update_queue_list()
{
	int i;
	for(i=1; i<nQ; i++)	Q[i-1] = Q[i];
}

//***************************************************************************//
int _build_claster() 
{
	int i;

	_init_claster_queue_list();
	while (nQ > 0){
		cE = Q[0];
		NB[nNB].A   = cE;
		NB[nNB].nNA = 0;
		NB[nNB].A.show = 'y';
		if (nQ > 1) _update_queue_list();
		nQ--;
		nNN = 0;
		nNN = _get_neighbors_atoms_list();
		if (nNN < 0) return -1;
		if (nNN > 0){
			for(i=0; i<nNN; i++) NB[nNB].NA[i] = NN[i];
		}
		NB[nNB].nNA = nNN;
		nNB++;
	}
	return 0;
}

//***************************************************************************//
int _check_diblicat(VECTOR3D V, int Num)
{
	int i;
	for (i=0; i<Num; i++){
		if ((V.x == BR[i].x) && (V.y == BR[i].y) && (V.z == BR[i].z)) return i;
	}
	return -1;
}



//***************************************************************************//
int build_in_plot_range_box()
{
	int i,j;
	int I1,I2;
	VECTOR3D V;
	int check;


	for(i=0; i<nNB; i++) {
		BR[i].show        = NB[i].A.show;
		BR[i].Num         = NB[i].A.Ind;
		BR[i].Index       = NB[i].A.Ind;
		BR[i].symnum      = NB[i].A.S;
		BR[i].T[0]        = NB[i].A.T[0];
		BR[i].T[1]        = NB[i].A.T[1];
		BR[i].T[2]        = NB[i].A.T[2];
		BR[i].x           = NB[i].A.P[0];
		BR[i].y           = NB[i].A.P[1];
		BR[i].z           = NB[i].A.P[2];
		BR[i].bondatom    = 0;
		BR[i].bondNum     = NB[i].nNA;
		BR[i].show_spin   = 0;
		BR[i].spin_dir[0] = 0;
		BR[i].spin_dir[1] = 0;
		BR[i].spin_dir[2] = 1;
	}
	nBR = nNB;
	for(i=0; i<nNB; i++){
		I1 = i;
		I2 = 0;
		for(j=0; j<NB[i].nNA; j++){
			V.x = NB[i].NA[j].P[0];
			V.y = NB[i].NA[j].P[1];
			V.z = NB[i].NA[j].P[2];
			check = _check_diblicat(V,nBR);
			if (check == -1){
				if (nBR >= MAX_NUM_OF_ATOMS) return -1;
//				BR[nBR].show        = 'y';
				BR[nBR].show        = NB[i].NA[j].show;
				BR[nBR].Num         = NB[i].NA[j].Ind;
				BR[nBR].Index       = NB[i].NA[j].Ind;
				BR[nBR].symnum      = NB[i].NA[j].S;
				BR[nBR].T[0]        = NB[i].NA[j].T[0];
				BR[nBR].T[1]        = NB[i].NA[j].T[1];
				BR[nBR].T[2]        = NB[i].NA[j].T[2];
				BR[nBR].x           = NB[i].NA[j].P[0];
				BR[nBR].y           = NB[i].NA[j].P[1];
				BR[nBR].z           = NB[i].NA[j].P[2];
				BR[nBR].bondatom    = 1;
				BR[nBR].bondNum     = 0;
				BR[nBR].show_spin   = 0;
				BR[nBR].spin_dir[0] = 0;
				BR[nBR].spin_dir[1] = 0;
				BR[nBR].spin_dir[2] = 1;
				BR[I1].b_list[I2]   = nBR;
				I2++;
				nBR++;
			}
			else {
				BR[I1].b_list[I2]   = check;
				BR[check].bondatom = 1;
				I2++;
			}
		}
	}
	return(0);
}


//***************************************************************************//
int _build_crystal(CELLPARAM cCell,
				 SYMMPOS sPos[MAX_SYMM_POS], int nPos,
				 ATOMTYPE lAtom[MAX_ATOMS_IN_ASYMM], int nAtom,
				 BONDSINI bini[MAX_BONDS_SPEC], int bond_sNum,
				 float range[6],int formatID) 
{
	int i,k,l;
	int check;

	file_format_ID = formatID;
	SGN = cCell.SpGrN;
	SGS = cCell.Setting;
	for(i=0; i<6; i++) cell_param[i] = cCell.a[i];
	_nPos = nPos;
	for(i=0; i<_nPos; i++){
		for(k=0; k<3; k++){
			for(l=0; l<3; l++) _sPos[i].a[k][l] = sPos[i].a[k][l];
			_sPos[i].t[k] = sPos[i].t[k];
		}
	}
	for(i=0; i<6; i++) brange[i] = range[i];
	for(i=0; i<nAtom; i++) _lAtom[i] = lAtom[i];
	_nAtom = nAtom;
	for(i=0; i<bond_sNum; i++) _bini[i] = bini[i];
	_bond_sNum = bond_sNum;

	nBR = 0;
	nC = 0;
	nQ = 0;
	nNN = 0;
	nNB = 0;

	_get_plot_range_box();
	_calc_ortho_matrix();
	if (_collect_sell_info() < 0) return -1;
	if (_bond_sNum > 0) {
			check = _build_claster();
   		if (check < 0) return -1;
	}
	else {
		for(i=0; i<nC; i++){
			NB[i].A = C[i];
			NB[i].A.show = 'y';
			NB[i].nNA = 0;
		}
		nNB = nC;
	}
	if (build_in_plot_range_box() < 0) return -1;

	return (nBR);
}



//***************************************************************************//
void _get_XYZ_in_range(ATOM BM[MAX_NUM_OF_ATOMS])
{
	int i;
	for(i=0; i<nBR; i++) BM[i] = BR[i];
}
