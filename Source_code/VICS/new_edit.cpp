//***************************************************************************//
//                 OPEN NEW DATA FILE / EDIT DATA FILE                       //
//---------------------------------------------------------------------------//
// File: new_edit.cpp                                                        //
// Date: 2001.12.19                                                          //
//***************************************************************************//

#include <GL/glui.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "vics.h"

extern "C"
{
	#include "matrix.h"
}

char path[256];
float UB_TR[6];


//-----------------------------------------------------
//  Space Group definitions
//-----------------------------------------------------
typedef struct spgrtag
{
	char name[20];                 //--- Space Group Name
	int SGS;                       //--- Space Group Setting
	int SGSO;                      //--- Number of Symmetry Operations 
	GLUI_String full_name[10];     //--- Space Group Full Name
} SPGR;



SPGR space_group[MAX_NUMBER_OF_GROUPS];
int NSG;
GLUI_String symmetry[] = {"Triclinic","Monoclinic","Orthorhombic","Tetragonal",
                          "Trigonal","Hexagonal","Cubic","Rhombohedral"};

//---------------------------------------------------- Matrix for Hexagonan-Rhombohedral Transformation
float H_R[3][3] = {
	{ 1,  0, 1},
    {-1,  1, 1},
	{ 0, -1, 1}
};

//---------------------------------------------------- Matrix for Hexagonan-Rhombohedral Transformation
float R_H[3][3] = {
	{ 2, -1, -1},
    { 1,  1, -2},
	{ 1,  1,  1}
};



//---------------------------------------------------- Transformation Matrix for Orthorhombic Cell
int tranM[18][18] = {
	  {1, 0, 0,  0, 1, 0,  0, 0, 1,  0, 0,-1,  0, 1, 0,  1, 0, 0},
	  {0, 1, 0,  1, 0, 0,  1, 0, 0,  0, 1, 0,  0, 0, 1,  0, 0,-1},
	  {0, 0, 1,  0, 0,-1,  0, 1, 0,  1, 0, 0,  1, 0, 0,  0, 1, 0},
//
	  {0, 1, 0,  1, 0, 0,  0, 0,-1,  0, 0, 1,  1, 0, 0,  0, 1, 0},
	  {1, 0, 0,  0, 1, 0,  0, 1, 0,  1, 0, 0,  0, 0,-1,  0, 0, 1},
	  {0, 0,-1,  0, 0, 1,  1, 0, 0,  0, 1, 0,  0, 1, 0,  1, 0, 0},
//
	  {0, 1, 0,  0, 0, 1,  1, 0, 0, -1, 0, 0,  0, 0, 1,  0, 1, 0},
	  {0, 0, 1,  0, 1, 0,  0, 1, 0,  0, 0, 1,  1, 0, 0, -1, 0, 0},
	  {1, 0, 0, -1, 0, 0,  0, 0, 1,  0, 1, 0,  0, 1, 0,  0, 0, 1},
//
	  {0, 0, 1,  0, 1, 0, -1, 0, 0,  1, 0, 0,  0, 1, 0,  0, 0, 1},
	  {0, 1, 0,  0, 0, 1,  0, 0, 1,  0, 1, 0, -1, 0, 0,  1, 0, 0},
	  {-1,0, 0,  1, 0, 0,  0, 1, 0,  0, 0, 1,  0, 0, 1,  0, 1, 0},
//
	  {0, 0, 1,  1, 0, 0,  0, 1, 0,  0,-1, 0,  1, 0, 0,  0, 0, 1},
	  {1, 0, 0,  0, 0, 1,  0, 0, 1,  1, 0, 0,  0, 1, 0,  0,-1, 0},
	  {0, 1, 0,  0,-1, 0,  1, 0, 0,  0, 0, 1,  0, 0, 1,  1, 0, 0},
//
	  {1, 0, 0,  0, 0, 1,  0,-1, 0,  0, 1, 0,  0, 0, 1,  1, 0, 0},
	  {0, 0, 1,  1, 0, 0,  1, 0, 0,  0, 0, 1,  0,-1, 0,  0, 1, 0},
	  {0,-1, 0,  0, 1, 0,  0, 0, 1,  1, 0, 0,  1, 0, 0,  0, 0, 1}
  };


//--- Objects
//-------------------------------------------- Main Objects
GLUI *newfile;                                //--- GLUI window
GLUI_Panel *newfile_panel;                    //--- Main Dialog Panel
GLUI_EditText *title_txt;                     //--- Title
//-------------------------------------------- 
GLUI_Panel *block1_panel;                    
GLUI_Panel *block2_panel;                    
//-------------------------------------------- Symmetry
GLUI_Panel *symmetry_panel;                   //--- Symmetry Panel
GLUI_Spinner *number_spin;                    //--- Space Group Number
GLUI_Listbox *sp_grp_lst;                     //--- List of Space Groups
GLUI_Spinner *setting_spin;                   //--- Settings
GLUI_EditText *symmetry_txt;                  //--- Symmetry;
GLUI_EditText *fulltext_txt;                  //--- Space Group Name;
GLUI_Checkbox *update_param_check;            //--- Update Cell and Structur with Settings;
GLUI_Button *ortho_orig;                      //--- Origin for Orthorombic groups (48,50,59,68,70)

//-------------------------------------------- Unit Cell Parameters
GLUI_Panel *param_panel;                      //--- Unit Cell Parameters Panel
GLUI_EditText *param_a_txt;                   //--- a
GLUI_EditText *param_b_txt;                   //--- b
GLUI_EditText *param_c_txt;                   //--- c
GLUI_EditText *param_al_txt;                  //--- alpha
GLUI_EditText *param_bt_txt;                  //--- betta
GLUI_EditText *param_gm_txt;                  //--- gamma
GLUI_EditText *param_ea_txt;                  //--- esd(a)
GLUI_EditText *param_eb_txt;                  //--- esd(b)
GLUI_EditText *param_ec_txt;                  //--- esd(c)
GLUI_EditText *param_eal_txt;                 //--- esd(alpha)
GLUI_EditText *param_ebt_txt;                 //--- esd(beta)
GLUI_EditText *param_egm_txt;                 //--- esd(gamma)
//-------------------------------------------- Structure Parameters
GLUI_Panel *struc_main_panel;                 //--- Structure Parameters Main Panel
GLUI_Panel *struc_panel;                     //--- Structure Parameters Panel
GLUI_Spinner *atom_number_spin;               //--- Atomic Position Number
GLUI_EditText *struc_l_txt;                   //--- Label
GLUI_EditText *struc_s_txt;                   //--- Symbol
GLUI_EditText *struc_g_txt;                   //--- g
GLUI_EditText *struc_x_txt;                   //--- x
GLUI_EditText *struc_y_txt;                   //--- y
GLUI_EditText *struc_z_txt;                   //--- z
GLUI_EditText *struc_ex_txt;                  //--- esd(x)
GLUI_EditText *struc_ey_txt;                  //--- esd(y)
GLUI_EditText *struc_ez_txt;                  //--- esd(z)
GLUI_EditText *struc_B_txt;                   //--- Izotropic Thermal PArameter
GLUI_EditText *numAtomLabel;
//-------------------------------------------- Anizotropic Thermal Parameters
GLUI_Checkbox *use_Biso_check;
GLUI_Checkbox *use_thermal_check;             //--- Use or not anisotropic thermal parameters
GLUI_Listbox *thermalMode;                    //--- Select anisotropic thermal parameters type (Uij or betaij)
GLUI_Panel *thermal_panel;                    //--- Anizotropic Thermal Parameters Panel
GLUI_StaticText *thermalLabel;
GLUI_EditText *thermal_B11_txt;
GLUI_EditText *thermal_B22_txt;
GLUI_EditText *thermal_B33_txt;
GLUI_EditText *thermal_B12_txt;
GLUI_EditText *thermal_B13_txt;
GLUI_EditText *thermal_B23_txt;
//-------------------------------------------- Navigator
GLUI_Panel *navigate_panel;                   //--- Navigation Panel
GLUI_Button *l_btn, *n_btn, *p_btn,           //--- Navigation Buttons 
            *f_btn, *e_btn, *d_btn,
			*a_btn;
//-------------------------------------------- OK / Apply / Close
GLUI_Panel *new_btn_panel;


// Control
int new_edit_Dialog_ID;                       //--- New/Edit Dialog Control
int parent_window_ID;                         //--- Parent Window ID
int c_mode_ID;                                //--- Mode ID (New / Edit)
int panel_dis_ID=0;

// Live Variables
int   numberID = 1;   //--- Space Group Number ID
int     nameID = 0;   //--- Space Group Name ID
int  settingID = 1;   //--- Space Group Setting ID
int  originID = 0;    //--- Origin ID for Orthorhombic groups (48,50,59,68,70)
int symmetryID = 0;   //--- Space Group Symmetry ID
int usethermalID = 0; //--- Use or not Anizotropic Thermal Parameters
int thermalID  = 0;   //--- Type of Anizotropic Thermal Parameters (Uij / betaij)
int cthermalID  = 0;
int isoID = 0;        //--- Type of Izotropic Thermal Parameters (Biso / Uiso)

char eTitle[70];                                          //---  Title
CELLPARAM eCell;                                          //--- Symmetry
float param[] =  {1.0f, 1.0f, 1.0f, 90.0f, 90.0f, 90.0f}; //--- Initial Unit Cell Parameters
float eparam[] = {0.0f, 0.0f, 0.0f,  0.0f,  0.0f,  0.0f}; //--- Initial ESD for Unit Cell Parameters
//--- Crystal Structure
ATOMTYPE eAtom[MAX_ATOMS_IN_ASYMM];                           //--- Structur Parameters
int TotalAtomNum = 0;                                     //--- Number of Atoms
int cAtomNum = 0;                                         //--- Current Number of Selected Atom
float struc[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
float estruc[] = {0.0f, 0.0f, 0.0f};
float etherm[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLUI_String cAtomLabel;
GLUI_String cAtomSymbol;
GLUI_String cNumAtomLabel;
float erange[] = {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};

double U_B_T[6],B_U_T[6];


/**************************************** get_edit_dialog_pos() ************/
void get_edit_dialog_pos(int &x, int &y)
{
	glutSetWindow(newfile->get_glut_window_id());
	x = glutGet(GLUT_WINDOW_X); 
	y = glutGet(GLUT_WINDOW_Y);
}



//***************************************************************************//
void skip_lines(FILE *cfile, int Line)
{
	int i;
	char line[80];
	for (i=0; i<Line; i++) fgets( line, sizeof(line), cfile); 
}


//********************  load_ortho_group()  *********************************//
void load_ortho_group()
{
//---- Load Additional Space Groups Information for Orthorhomic Space Groups
	int i,j,I1,I2;
	FILE *fptr; 
	char n1[10],n2[10],n3[10],n4[10],n5[10],n6[10];
	char *n;
	char ch0, ch1[6], ch2[6], ch3[6];
	char pathSPGRO[256];

	sprintf(pathSPGRO,"");
	sprintf(pathSPGRO,"%s%s",path,SPGRO);

	fptr = fopen(pathSPGRO,"r");
	for(i=0; i<59; i++){
		sprintf(n1,""); sprintf(n2,""); sprintf(n3,"");
		sprintf(n4,""); sprintf(n5,""); sprintf(n6,"");
		fscanf(fptr,"%i %s %s %s %s %s %s\n",&I1,&n1,&n2,&n3,&n4,&n5,&n6);
		for(j=0; j<6; j++){
			n = "";
			sprintf(ch1,"%s","");
			sprintf(ch2,"%s","");
			sprintf(ch3,"%s","");
			if (j == 0) n = n1;
			if (j == 1) n = n2;
			if (j == 2) n = n3;
			if (j == 3) n = n4;
			if (j == 4) n = n5;
			if (j == 5) n = n6;
			I2 = 0;
			ch0 = n[0];n++;
			if (n[1] == '1') {sprintf(ch1,"%c%c",n[0],n[1]); n +=2;}
			else {sprintf(ch1,"%c",n[0]); n++;}
			if (n[1] == '1') {sprintf(ch2,"%c%c",n[0],n[1]); n +=2;}
			else {sprintf(ch2,"%c",n[0]); n++;}
			if (n[1] == '1') {sprintf(ch3,"%c%c",n[0],n[1]); n +=2;}
			else {sprintf(ch3,"%c",n[0]); n++;}
			if (j == 0) sprintf(space_group[I1-1].name,"%c %s %s %s", ch0,ch1,ch2,ch3);
			sprintf(space_group[I1-1].full_name[j],"");
			sprintf(space_group[I1-1].full_name[j],"%c %s %s %s", ch0,ch1,ch2,ch3);
		}
	}
	fclose(fptr);
}

//********************  load_spgra()  ***************************************//
void load_spgra()
{
//---- Load Space Groups Information from SPGRA file
	FILE *fptr; 
	GLUI_String line;
	int tmp0,tmp1,tmp2,tmp3,tmp4;
	char ch0[2],ch1[5],ch2[5],ch3[5];
	int i;
	char pathSPGRA[256];

	sprintf(pathSPGRA,"");
	sprintf(pathSPGRA,"%s%s",path,SPGRA);

	NSG = 0;
	fptr = fopen(pathSPGRA,"r");
    while( !feof( fptr ) ){
		sprintf(line,"");
		sprintf(ch0,"");
		sprintf(ch1,"");
		sprintf(ch2,"");
		sprintf(ch3,"");
		fgets( line, sizeof(GLUI_String), fptr); 
		sscanf(line,"%3i%3i%3i%3i%3i%s %s %s %s",&tmp0,&tmp1,&tmp2,&tmp3,&tmp4,
					                             &ch0,&ch1,&ch2,&ch3);
		skip_lines(fptr, tmp4+1);
		if (tmp1 == 1) {
			NSG++;
			space_group[NSG-1].SGS = 1; 
			space_group[NSG-1].SGSO = tmp4;
			sprintf(space_group[NSG-1].name,"%s %s %s %s", ch0,ch1,ch2,ch3);
			sprintf(space_group[NSG-1].full_name[0],"%s %s %s %s", ch0,ch1,ch2,ch3);
		}
		else {
			sprintf(space_group[NSG-1].full_name[space_group[NSG-1].SGS],"%s %s %s %s", ch0,ch1,ch2,ch3);
			space_group[NSG-1].SGS++;
		}
	}
	NSG--;
	fclose(fptr);
	for(i=16; i<75; i++){
		space_group[i-1].SGS = 6; 
		load_ortho_group();
	}
}

//********************  make_trasformation()  *******************************//
void make_trasformation(int mode, int I1, int I2, float IN1, float IN2, float IN3, 
						                          float &OUT1, float &OUT2, float &OUT3)
{
	float x,y,z;

	x = IN1 * tranM[I1+0][I2+0] + IN2 * tranM[I1+0][I2+1] + IN3 * tranM[I1+0][I2+2];
	y = IN1 * tranM[I1+1][I2+0] + IN2 * tranM[I1+1][I2+1] + IN3 * tranM[I1+1][I2+2];
	z = IN1 * tranM[I1+2][I2+0] + IN2 * tranM[I1+2][I2+1] + IN3 * tranM[I1+2][I2+2];
	if (mode == 1){
		OUT1 = (float)fabs(x);
		OUT2 = (float)fabs(y);
		OUT3 = (float)fabs(z);
	}
	else {
		OUT1 = x;
		OUT2 = y;
		OUT3 = z;
	}
}

//********************  U_Beta_trans()  *************************************//
void calc_U_Beta_trans_coefficients(int ID)
{
//--------------------------------------//
//  ID = 0 from U to Beta               //
//  ID = 1 from Beta to U               //
//--------------------------------------//
	double ar,br,cr,S,Vol;
	double ca,cb,cg;
	double sa,sb,sg;

	ca = cos(param[3]*PI/180.0);
	cb = cos(param[4]*PI/180.0);
	cg = cos(param[5]*PI/180.0);
	sa = sin(param[3]*PI/180.0);
	sb = sin(param[4]*PI/180.0);
	sg = sin(param[5]*PI/180.0);

	S = 1 - ca*ca - cb*cb - cg*cg + 2*ca*cb*cg;
	Vol = param[0]*param[1]*param[2]*sqrt(S);

	ar = (param[1]*param[2]*sa) / Vol;
	br = (param[0]*param[2]*sb) / Vol;
	cr = (param[0]*param[1]*sg) / Vol;

	if (ID == 0){
		U_B_T[0] = 2.0*PI*ar*ar;
		U_B_T[1] = 2.0*PI*br*br;
		U_B_T[2] = 2.0*PI*cr*cr;
		U_B_T[3] = 2.0*PI*ar*br;
		U_B_T[4] = 2.0*PI*ar*cr;
		U_B_T[5] = 2.0*PI*br*cr;
	}
	else {
		B_U_T[0] = 2.0*PI*ar*ar;
		B_U_T[1] = 2.0*PI*br*br;
		B_U_T[2] = 2.0*PI*cr*cr;
		B_U_T[3] = 2.0*PI*ar*br;
		B_U_T[4] = 2.0*PI*ar*cr;
		B_U_T[5] = 2.0*PI*br*cr;
	}
}

//********************  U_Beta_trans()  *************************************//
MATRIX U_Beta_trans(int ID, MATRIX M)
{
//--------------------------------------//
//  ID = 0 from U to Beta               //
//  ID = 1 from Beta to U               //
//--------------------------------------//
	MATRIX W;

	if (ID == 0){
		W.a[0][0] = (float)(U_B_T[0]*M.a[0][0]);
		W.a[1][1] = (float)(U_B_T[1]*M.a[1][1]);
		W.a[2][2] = (float)(U_B_T[2]*M.a[2][2]);
		W.a[1][0] = W.a[0][1] = (float)(U_B_T[3]*M.a[0][1]);
		W.a[2][0] = W.a[0][2] = (float)(U_B_T[4]*M.a[0][2]);
		W.a[2][1] = W.a[1][2] = (float)(U_B_T[5]*M.a[1][2]);
	}
	else {
		W.a[0][0] = (float)(M.a[0][0]/B_U_T[0]);
		W.a[1][1] = (float)(M.a[1][1]/B_U_T[1]);
		W.a[2][2] = (float)(M.a[2][2]/B_U_T[2]);
		W.a[1][0] = W.a[0][1] = (float)(M.a[0][1]/B_U_T[3]);
		W.a[2][0] = W.a[0][2] = (float)(M.a[0][2]/B_U_T[4]);
		W.a[2][1] = W.a[1][2] = (float)(M.a[1][2]/B_U_T[5]);
	}

	return(W);
}


//********************  set_cur_origin()  ***********************************//
void set_cur_origin(int ori)
{
	int I1,I2,i;

	ortho_orig->enable();
	I2 = update_param_check->get_int_val();
	if ((ori != originID) && (I2 == 1)){
		I1 = eCell.SpGrN*10 + ori;
		switch(I1)
		{
		case(480):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x - 0.25;
				eAtom[i].y = eAtom[i].y - 0.25;
				eAtom[i].z = eAtom[i].z - 0.25;
			}
			break;
				  }
		case(481):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x + 0.25;
				eAtom[i].y = eAtom[i].y + 0.25;
				eAtom[i].z = eAtom[i].z + 0.25;
			}
			break;
				  }
		case(500):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x - 0.25;
				eAtom[i].y = eAtom[i].y - 0.25;
				eAtom[i].z = eAtom[i].z;
			}
			break;
				  }
		case(501):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x + 0.25;
				eAtom[i].y = eAtom[i].y + 0.25;
				eAtom[i].z = eAtom[i].z;
			}
			break;
				  }
		case(590):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x - 0.25;
				eAtom[i].y = eAtom[i].y - 0.25;
				eAtom[i].z = eAtom[i].z;
			}
			break;
				  }
		case(591):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x + 0.25;
				eAtom[i].y = eAtom[i].y + 0.25;
				eAtom[i].z = eAtom[i].z;
			}
			break;
				  }
		case(680):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x;
				eAtom[i].y = eAtom[i].y - 0.25;
				eAtom[i].z = eAtom[i].z - 0.25;
			}
			break;
				  }
		case(681):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x;
				eAtom[i].y = eAtom[i].y + 0.25;
				eAtom[i].z = eAtom[i].z + 0.25;
			}
			break;
				  }
		case(700):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x + 0.125;
				eAtom[i].y = eAtom[i].y + 0.125;
				eAtom[i].z = eAtom[i].z + 0.125;
			}
			break;
				  }
		case(701):{
			for(i=0; i<TotalAtomNum; i++){
				eAtom[i].x = eAtom[i].x - 0.125;
				eAtom[i].y = eAtom[i].y - 0.125;
				eAtom[i].z = eAtom[i].z - 0.125;
			}
			break;
				  }
		}
		struc[1] = eAtom[cAtomNum].x;
		struc[2] = eAtom[cAtomNum].y;
		struc[3] = eAtom[cAtomNum].z;
		struc_x_txt->set_float_val(struc[1]);
		struc_y_txt->set_float_val(struc[2]);
		struc_z_txt->set_float_val(struc[3]);
	}
	originID = ori;
	eCell.Setting = eCell.Setting + originID*6;
}


//********************  get_cur_origin()  ***********************************//
void get_cur_origin(int &num, int &ori)
{
	num = eCell.SpGrN;
	ori = originID;
}

//********************  set_new_edit_file_dialog_param()  *******************//
void get_new_edit_file_dialog_param(char *sTitle, int &SpGrNum, int &SpGrSet, char *SpGrName, 
									float CellPar[6], float CellEsd[6], 
									int &numOfAtom, int &thermal_mode, int &use_thermal, 
									float plot_range[6],int &type_iso)
{
	int i;
	SpGrNum = numberID; 
	SpGrSet = settingID + originID*6;
	sprintf(SpGrName,"");
	sprintf(SpGrName,"%s",space_group[nameID].name);
	numOfAtom = TotalAtomNum;
	thermal_mode = thermalID;
	use_thermal = usethermalID;
	sprintf(eTitle,"");sprintf(eTitle,"%s",title_txt->get_text());
	sprintf(sTitle,"");sprintf(sTitle,"%s",eTitle);
	for(i=0; i<6; i++){
		CellPar[i] = param[i];
		CellEsd[i] = eparam[i];
		plot_range[i] = erange[i];
	}
	i = use_Biso_check->get_int_val();
	if (i == 1) isoID = 0;
	else isoID = 1;
	type_iso = isoID;
}

//********************  get_struc_param()  **********************************//
void get_struc_param(int ID, char *name, char *symb, float &g, float &x, float &y, float &z, 
                     float &ex, float &ey, float &ez, float ATP[6], float &BISO)
{
	int i;
	sprintf(name,"");sprintf(name,"%s",eAtom[ID].name);
	sprintf(symb,"");sprintf(symb,"%s",eAtom[ID].simb);
	x = eAtom[ID].x; y = eAtom[ID].y; z = eAtom[ID].z;
	ex = eAtom[ID].ex; ey = eAtom[ID].ey; ez = eAtom[ID].ez;
	g = eAtom[ID].g;
	for(i=0; i<6; i++) ATP[i] = eAtom[ID].B[i];
	BISO = eAtom[ID].Biso;
}


//********************  get_new_edit_dialog_id()  ***************************//
int get_new_edit_dialog_id()
{
	return(new_edit_Dialog_ID);
}

//********************  set_new_edit_dialog_id()  ***************************//
void set_new_edit_dialog_id(int ID)
{
	new_edit_Dialog_ID = ID;
}


//********************  update_monoclinic_param()  *************************//
void update_monoclinic_param()
{
	int I1,I2,i;
	float a1,b1,c1,al1,bt1,gm1;
	float a2,b2,c2,al2,bt2,gm2;
	double cf;
	double x,y,z;
	int swichID;
	MATRIX U,W,M,M1;

	I1 = eCell.Setting;
	I2 = settingID;
	a1 = param[0]; 
	b1 = param[1];
	c1 = param[2];
	al1 = param[3];
	bt1 = param[4];
	gm1 = param[5];
	erange[0] = erange[2] = erange[4] = 0;
	erange[1] = erange[3] = erange[5] = 1;
	if (space_group[nameID].SGS == 6){
		if ((I1 == 1) && (I2 == 2)) swichID = 1;if ((I1 == 2) && (I2 == 1)) swichID = 11;
		if ((I1 == 2) && (I2 == 3)) swichID = 2;if ((I1 == 3) && (I2 == 2)) swichID = 21;
		if ((I1 == 3) && (I2 == 4)) swichID = 3;if ((I1 == 4) && (I2 == 3)) swichID = 31;
		if ((I1 == 4) && (I2 == 5)) swichID = 4;if ((I1 == 5) && (I2 == 4)) swichID = 41;
		if ((I1 == 5) && (I2 == 6)) swichID = 5;if ((I1 == 6) && (I2 == 5)) swichID = 51;
		switch(swichID)
		{
		case(2):
		case(1):{
			M.a[0][0] = -1; M.a[0][1] = 0; M.a[0][2] = -1;
			M.a[1][0] =  0; M.a[1][1] = 1; M.a[1][2] =  0;
			M.a[2][0] =  1; M.a[2][1] = 0; M.a[2][2] =  0;
			a2 = (float)sqrt(a1*a1 + c1*c1 + 2*a1*c1*cos(bt1*PI/180));
			b2 = b1;
			c2 = a1;
			cf = (c1*c1 + a2*a2 - a1*a1)/(2*c1*a2);
			al2 = al1;
			bt2 = 180 - bt1 + (float)(acos(cf)*180/PI);
			gm2 = gm1;
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					if (usethermalID == 1){
						U.a[0][0] = eAtom[i].B[0];
						U.a[1][1] = eAtom[i].B[1];
						U.a[2][2] = eAtom[i].B[2];
						U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
						U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
						U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
						if (thermalID == 0) U = U_Beta_trans(0, U);
					}
					x = -eAtom[i].z;
					y =  eAtom[i].y;
					z = eAtom[i].x - eAtom[i].z;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					if (usethermalID == 1){
						W = MM( RM(TM(M)) , MM(U,RM(M)) );
						if (thermalID == 0) W = U_Beta_trans(1, W);
						eAtom[i].B[0] = W.a[0][0];
						eAtom[i].B[1] = W.a[1][1];
						eAtom[i].B[2] = W.a[2][2];
						eAtom[i].B[3] = W.a[0][1];
						eAtom[i].B[4] = W.a[0][2];
						eAtom[i].B[5] = W.a[1][2];
					}
				}
			}
			break;
				}
		case(21):
		case(11):{
			M.a[0][0] =  0; M.a[0][1] = 0; M.a[0][2] =  1;
			M.a[1][0] =  0; M.a[1][1] = 1; M.a[1][2] =  0;
			M.a[2][0] = -1; M.a[2][1] = 0; M.a[2][2] = -1;
			a2 = c1;
			b2 = b1;
			c2 = (float)sqrt(a1*a1 + c1*c1 + 2*a1*c1*cos(bt1*PI/180));
			cf = (a1*a1 + c2*c2 - c1*c1)/(2*a1*c2);
			al2 = al1;
			bt2 = 180 - bt1 + (float)(acos(cf)*180/PI);
			gm2 = gm1;
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					if (usethermalID == 1){
						U.a[0][0] = eAtom[i].B[0];
						U.a[1][1] = eAtom[i].B[1];
						U.a[2][2] = eAtom[i].B[2];
						U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
						U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
						U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
						if (thermalID == 0) U = U_Beta_trans(0, U);
					}
					x = -eAtom[i].x + eAtom[i].z;
					y =  eAtom[i].y;
					z = -eAtom[i].x;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					if (usethermalID == 1){
						W = MM( RM(TM(M)) , MM(U,RM(M)) );
						if (thermalID == 0) W = U_Beta_trans(1, W);
						eAtom[i].B[0] = W.a[0][0];
						eAtom[i].B[1] = W.a[1][1];
						eAtom[i].B[2] = W.a[2][2];
						eAtom[i].B[3] = W.a[0][1];
						eAtom[i].B[4] = W.a[0][2];
						eAtom[i].B[5] = W.a[1][2];
					}
				}
			}
			break;
				 }
		case(5):
		case(4):{
			M.a[0][0] =  0; M.a[0][1] =  1; M.a[0][2] =  0;
			M.a[1][0] = -1; M.a[1][1] = -1; M.a[1][2] =  0;
			M.a[2][0] =  0; M.a[2][1] =  0; M.a[2][2] =  1;
			a2 = b1;
			b2 = (float)sqrt(a1*a1 + b1*b1 + 2*a1*b1*cos(gm1*PI/180));
			c2 = c1;
			cf = (a1*a1 + b2*b2 - b1*b1)/(2*a1*b2);
			al2 = al1;
			bt2 = bt1;
			gm2 = 180 - gm1 + (float)(acos(cf)*180/PI);
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					if (usethermalID == 1){
						U.a[0][0] = eAtom[i].B[0];
						U.a[1][1] = eAtom[i].B[1];
						U.a[2][2] = eAtom[i].B[2];
						U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
						U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
						U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
						if (thermalID == 0) U = U_Beta_trans(0, U);
					}
					x = -eAtom[i].x + eAtom[i].y;
					y = -eAtom[i].x;
					z =  eAtom[i].z;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					if (usethermalID == 1){
						W = MM( RM(TM(M)) , MM(U,RM(M)) );
						if (thermalID == 0) W = U_Beta_trans(1, W);
						eAtom[i].B[0] = W.a[0][0];
						eAtom[i].B[1] = W.a[1][1];
						eAtom[i].B[2] = W.a[2][2];
						eAtom[i].B[3] = W.a[0][1];
						eAtom[i].B[4] = W.a[0][2];
						eAtom[i].B[5] = W.a[1][2];
					}
				}
			}
			break;
				}
		case(51):
		case(41):{
			M.a[0][0] = -1; M.a[0][1] = -1; M.a[0][2] =  0;
			M.a[1][0] =  1; M.a[1][1] =  0; M.a[1][2] =  0;
			M.a[2][0] =  0; M.a[2][1] =  0; M.a[2][2] =  1;
			a2 = (float)sqrt(a1*a1 + b1*b1 + 2*a1*b1*cos(gm1*PI/180));
			b2 = a1;
			c2 = c1;
			cf = (b1*b1 + a2*a2 - a1*a1)/(2*b1*a2);
			al2 = al1;
			bt2 = bt1;
			gm2 = 180 - gm1 + (float)(acos(cf)*180/PI);
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					if (usethermalID == 1){
						U.a[0][0] = eAtom[i].B[0];
						U.a[1][1] = eAtom[i].B[1];
						U.a[2][2] = eAtom[i].B[2];
						U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
						U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
						U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
						if (thermalID == 0) U = U_Beta_trans(0, U);
					}
					x = -eAtom[i].y;
					y =  eAtom[i].x - eAtom[i].y;
					z =  eAtom[i].z;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					if (usethermalID == 1){
						W = MM( RM(TM(M)) , MM(U,RM(M)) );
						if (thermalID == 0) W = U_Beta_trans(1, W);
						eAtom[i].B[0] = W.a[0][0];
						eAtom[i].B[1] = W.a[1][1];
						eAtom[i].B[2] = W.a[2][2];
						eAtom[i].B[3] = W.a[0][1];
						eAtom[i].B[4] = W.a[0][2];
						eAtom[i].B[5] = W.a[1][2];
					}
				}
			}
			break;
				 }
		case(3):{
			M.a[0][0] = -1; M.a[0][1] =  0; M.a[0][2] = -1;
			M.a[1][0] =  0; M.a[1][1] =  1; M.a[1][2] =  0;
			M.a[2][0] =  1; M.a[2][1] =  0; M.a[2][2] =  0;
			M1.a[0][0] =  0; M1.a[0][1] =  0; M1.a[0][2] =  1;
			M1.a[1][0] =  1; M1.a[1][1] =  0; M1.a[1][2] =  0;
			M1.a[2][0] =  0; M1.a[2][1] =  1; M1.a[2][2] =  0;
			a2 = (float)sqrt(a1*a1 + c1*c1 + 2*a1*c1*cos(bt1*PI/180));
			b2 = b1;
			c2 = a1;
			cf = (c1*c1 + a2*a2 - a1*a1)/(2*c1*a2);
			al2 = al1;
			bt2 = 180 - bt1 + (float)(acos(cf)*180/PI);
			gm2 = gm1;
			a1 = a2;
			b1 = b2;
			c1 = c2;
			al1 = al2;
			bt1 = bt2;
			gm1 = gm2;
			a2 = c1;
			b2 = a1;
			c2 = b1;
			al2 = gm1;
			bt2 = al1;
			gm2 = bt1;
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					if (usethermalID == 1){
						U.a[0][0] = eAtom[i].B[0];
						U.a[1][1] = eAtom[i].B[1];
						U.a[2][2] = eAtom[i].B[2];
						U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
						U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
						U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
						if (thermalID == 0) U = U_Beta_trans(0, U);
					}
					x = -eAtom[i].z;
					y =  eAtom[i].y;
					z =  eAtom[i].x - eAtom[i].z;
					eAtom[i].x = x;
					eAtom[i].y = y;
					eAtom[i].z = z;
					x = eAtom[i].z;
					y = eAtom[i].x;
					z = eAtom[i].y;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					if (usethermalID == 1){
						W = MM( RM(TM(M)) , MM(U,RM(M)) );
						U = MM( RM(TM(M1)) , MM(W,RM(M1)) );
						if (thermalID == 0) U = U_Beta_trans(1, U);
						eAtom[i].B[0] = U.a[0][0];
						eAtom[i].B[1] = U.a[1][1];
						eAtom[i].B[2] = U.a[2][2];
						eAtom[i].B[3] = U.a[0][1];
						eAtom[i].B[4] = U.a[0][2];
						eAtom[i].B[5] = U.a[1][2];
					}
				}
			}
			break;
				}
		case(31):{
			M.a[0][0] =  0; M.a[0][1] =  1; M.a[0][2] =  0;
			M.a[1][0] =  0; M.a[1][1] =  0; M.a[1][2] =  1;
			M.a[2][0] =  1; M.a[2][1] =  0; M.a[2][2] =  0;
			M1.a[0][0] = -1; M1.a[0][1] =  0; M1.a[0][2] = -1;
			M1.a[1][0] =  0; M1.a[1][1] =  1; M1.a[1][2] =  0;
			M1.a[2][0] =  1; M1.a[2][1] =  0; M1.a[2][2] =  0;
			a2 = b1;
			b2 = c1;
			c2 = a1;
			al2 = bt1;
			bt2 = gm1;
			gm2 = al1;
			a1 = a2;
			b1 = b2;
			c1 = c2;
			al1 = al2;
			bt1 = bt2;
			gm1 = gm2;
			a2 = c1;
			b2 = b1;
			c2 = (float)sqrt(a1*a1 + c1*c1 + 2*a1*c1*cos(bt1*PI/180));
			cf = (a1*a1 + c2*c2 - c1*c1)/(2*a1*c2);
			al2 = al1;
			bt2 = 180 - bt1 + (float)(acos(cf)*180/PI);
			gm2 = gm1;
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					if (usethermalID == 1){
						U.a[0][0] = eAtom[i].B[0];
						U.a[1][1] = eAtom[i].B[1];
						U.a[2][2] = eAtom[i].B[2];
						U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
						U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
						U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
						if (thermalID == 0) U = U_Beta_trans(0, U);
					}
					x = eAtom[i].y;
					y = eAtom[i].z;
					z = eAtom[i].x;
					eAtom[i].x = x;
					eAtom[i].y = y;
					eAtom[i].z = z;
					x = -eAtom[i].x + eAtom[i].z;
					y = eAtom[i].y;
					z = -eAtom[i].x;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					if (usethermalID == 1){
						W = MM( RM(TM(M)) , MM(U,RM(M)) );
						U = MM( RM(TM(M1)) , MM(W,RM(M1)) );
						if (thermalID == 0) U = U_Beta_trans(1, U);
						eAtom[i].B[0] = U.a[0][0];
						eAtom[i].B[1] = U.a[1][1];
						eAtom[i].B[2] = U.a[2][2];
						eAtom[i].B[3] = U.a[0][1];
						eAtom[i].B[4] = U.a[0][2];
						eAtom[i].B[5] = U.a[1][2];
					}
				}
			}
			break;
				 }
		}
		param[0] = a2;
		param[1] = b2;
		param[2] = c2;
		param[3] = al2;
		param[4] = bt2;
		param[5] = gm2;
		struc[1] = eAtom[cAtomNum].x;
		struc[2] = eAtom[cAtomNum].y;
		struc[3] = eAtom[cAtomNum].z;
		if ((I2 <= 3) && (I1 > 3)) { //---- from unic axis (c) to unic axis (b)
			param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
			param_al_txt->disable();  param_bt_txt->enable();   param_gm_txt->disable();
			param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
			param_eal_txt->disable(); param_ebt_txt->enable();  param_egm_txt->disable();
		}
		if ((I2 > 3) && (I1 <= 3)) { //---- from unic axis (b) to unic axis (c)
			param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
			param_al_txt->disable();  param_bt_txt->disable();  param_gm_txt->enable();
			param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
			param_eal_txt->disable(); param_ebt_txt->disable(); param_egm_txt->enable();
		}
	}
	else {
		if ((I2 == 1) && (I1 == 2)) { //---- from unic axis (c) to unic axis (b)
			M.a[0][0] =  0; M.a[0][1] =  1; M.a[0][2] =  0;
			M.a[1][0] =  0; M.a[1][1] =  0; M.a[1][2] =  1;
			M.a[2][0] =  1; M.a[2][1] =  0; M.a[2][2] =  0;
			param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
			param_al_txt->disable();  param_bt_txt->enable();   param_gm_txt->disable();
			param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
			param_eal_txt->disable(); param_ebt_txt->enable();  param_egm_txt->disable();
			a2 = b1;
			b2 = c1;
			c2 = a1;
			al2 = bt1;
			bt2 = gm1;
			gm2 = al1;
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					U.a[0][0] = eAtom[i].B[0];
					U.a[1][1] = eAtom[i].B[1];
					U.a[2][2] = eAtom[i].B[2];
					U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
					U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
					U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
					if (thermalID == 0) U = U_Beta_trans(0, U);
					x = eAtom[i].z;
					y = eAtom[i].x;
					z = eAtom[i].y;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					W = MM( RM(TM(M)) , MM(U,RM(M)) );
					if (thermalID == 0) W = U_Beta_trans(1, W);
					eAtom[i].B[0] = U.a[0][0];
					eAtom[i].B[1] = U.a[1][1];
					eAtom[i].B[2] = U.a[2][2];
					eAtom[i].B[3] = U.a[0][1];
					eAtom[i].B[4] = U.a[0][2];
					eAtom[i].B[5] = U.a[1][2];
				}
			}
		}
		if ((I2 == 2) && (I1 == 1)) { //---- from unic axis (b) to unic axis (c)
			M.a[0][0] =  0; M.a[0][1] =  0; M.a[0][2] =  1;
			M.a[1][0] =  1; M.a[1][1] =  0; M.a[1][2] =  0;
			M.a[2][0] =  0; M.a[2][1] =  1; M.a[2][2] =  0;
			param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
			param_al_txt->disable();  param_bt_txt->disable();  param_gm_txt->enable();
			param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
			param_eal_txt->disable(); param_ebt_txt->disable(); param_egm_txt->enable();
			a2 = c1;
			b2 = a1;
			c2 = b1;
			al2 = gm1;
			bt2 = al1;
			gm2 = bt1;
			if (update_param_check->get_int_val() == 1) {
				for(i=0; i<TotalAtomNum; i++){
					U.a[0][0] = eAtom[i].B[0];
					U.a[1][1] = eAtom[i].B[1];
					U.a[2][2] = eAtom[i].B[2];
					U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
					U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
					U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
					if (thermalID == 0) U = U_Beta_trans(0, U);
					x = eAtom[i].y;
					y = eAtom[i].z;
					z = eAtom[i].x;
					if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
					if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
					if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
					eAtom[i].x = (float)x;
					eAtom[i].y = (float)y;
					eAtom[i].z = (float)z;
					W = MM( RM(TM(M)) , MM(U,RM(M)) );
					if (thermalID == 0) W = U_Beta_trans(1, W);
					eAtom[i].B[0] = U.a[0][0];
					eAtom[i].B[1] = U.a[1][1];
					eAtom[i].B[2] = U.a[2][2];
					eAtom[i].B[3] = U.a[0][1];
					eAtom[i].B[4] = U.a[0][2];
					eAtom[i].B[5] = U.a[1][2];
				}
			}
		}
		param[0] = a2;
		param[1] = b2;
		param[2] = c2;
		param[3] = al2;
		param[4] = bt2;
		param[5] = gm2;
		struc[1] = eAtom[cAtomNum].x;
		struc[2] = eAtom[cAtomNum].y;
		struc[3] = eAtom[cAtomNum].z;
	}
	param_a_txt->set_float_val(param[0]);
	param_b_txt->set_float_val(param[1]);
	param_c_txt->set_float_val(param[2]);
	param_al_txt->set_float_val(param[3]);
	param_bt_txt->set_float_val(param[4]);
	param_gm_txt->set_float_val(param[5]);
	struc_x_txt->set_float_val(struc[1]);
	struc_y_txt->set_float_val(struc[2]);
	struc_z_txt->set_float_val(struc[3]);
	erange[0] = erange[2] = erange[4] = 0;
	erange[1] = erange[3] = erange[5] = 1;

}

//********************  update_orthorombic_param()  *************************//
void update_orthorombic_param()
{
	int I1,I2,I3,i;
	float t[3][3],x,y,z,rangr[6];

	if (c_mode_ID == 1){
		I3 = eCell.Setting - originID*6;
		I1 = 3*(I3 - 1);
		I2 = 3*(settingID - 1);

		make_trasformation(1, I1, I2, param[0], param[1], param[2],
			                          param[0], param[1], param[2]);
		make_trasformation(1, I1, I2, eparam[0], eparam[1], eparam[2],
			                          eparam[0], eparam[1], eparam[2]);
		param_a_txt->set_float_val(param[0]);
		param_b_txt->set_float_val(param[1]);
		param_c_txt->set_float_val(param[2]);
		param_ea_txt->set_float_val(eparam[0]);
		param_eb_txt->set_float_val(eparam[1]);
		param_ec_txt->set_float_val(eparam[2]);
		for(i=0; i<TotalAtomNum; i++){
			make_trasformation(0,I1,I2,eAtom[i].x, eAtom[i].y, eAtom[i].z,
			                           eAtom[i].x, eAtom[i].y, eAtom[i].z);
			make_trasformation(0,I1,I2,eAtom[i].ex,eAtom[i].ey,eAtom[i].ez,
			                           eAtom[i].ex,eAtom[i].ey,eAtom[i].ez);
			if (usethermalID == 1){
				t[0][0] = eAtom[i].B[0];
				t[1][1] = eAtom[i].B[1];
				t[2][2] = eAtom[i].B[2];
				t[1][0] = t[0][1] = eAtom[i].B[3];
				t[2][0] = t[0][2] = eAtom[i].B[4];
				t[2][1] = t[1][2] = eAtom[i].B[5];
				make_trasformation(1,I1,I2,1,2,3,x,y,z);
				x = x - 1;
				y = y - 1;
				z = z - 1;
				eAtom[i].B[0] = t[(int)x][(int)x];
				eAtom[i].B[1] = t[(int)y][(int)y];
				eAtom[i].B[2] = t[(int)z][(int)z];
				eAtom[i].B[3] = t[(int)x][(int)y];
				eAtom[i].B[4] = t[(int)x][(int)z];
				eAtom[i].B[5] = t[(int)y][(int)z];
			}
		}
		for(i=0; i<6; i++) rangr[i] = erange[i];
		make_trasformation(1,I1,I2,1,3,5,x,y,z);
		erange[0] = rangr[int(x)-1]; erange[1] = rangr[int(x)];
		erange[2] = rangr[int(y)-1]; erange[3] = rangr[int(y)];
		erange[4] = rangr[int(z)-1]; erange[5] = rangr[int(z)];
		struc[1] = eAtom[cAtomNum].x;
		struc[2] = eAtom[cAtomNum].y;
		struc[3] = eAtom[cAtomNum].z;
		estruc[0] = eAtom[cAtomNum].ex;
		estruc[1] = eAtom[cAtomNum].ey;
		estruc[2] = eAtom[cAtomNum].ez;
		for(i=0; i<6; i++) etherm[i] = eAtom[cAtomNum].B[i];
		struc_x_txt->set_float_val(struc[1]);
		struc_y_txt->set_float_val(struc[2]);
		struc_z_txt->set_float_val(struc[3]);
		struc_ex_txt->set_float_val(estruc[0]);
		struc_ey_txt->set_float_val(estruc[1]);
		struc_ez_txt->set_float_val(estruc[2]);
		thermal_B11_txt->set_float_val(etherm[0]);
		thermal_B22_txt->set_float_val(etherm[1]);
		thermal_B33_txt->set_float_val(etherm[2]);
		thermal_B12_txt->set_float_val(etherm[3]);
		thermal_B13_txt->set_float_val(etherm[4]);
		thermal_B23_txt->set_float_val(etherm[5]);
	}
}


//********************  update_rhombohedral_param()  ************************//
void update_rhombohedral_param()
{
	int I1,I2,i;
	double d,cf;
	double x,y,z;
	MATRIX U,W;
	MATRIX H_R, R_H, X_H_R, X_R_H;
	double a1,b1,c1,al1,bt1,gm1;
	double a2,b2,c2,al2,bt2,gm2;


	R_H.a[0][0] =  1; R_H.a[0][1] = -1; R_H.a[0][2] =  0;
	R_H.a[1][0] =  0; R_H.a[1][1] =  1; R_H.a[1][2] = -1;
	R_H.a[2][0] =  1; R_H.a[2][1] =  1; R_H.a[2][2] =  1;

	H_R.a[0][0] =  2.0f/3.0f; H_R.a[0][1] =  1.0f/3.0f; H_R.a[0][2] =  1.0f/3.0f;
	H_R.a[1][0] = -1.0f/3.0f; H_R.a[1][1] =  1.0f/3.0f; H_R.a[1][2] =  1.0f/3.0f;
	H_R.a[2][0] = -1.0f/3.0f; H_R.a[2][1] = -2.0f/3.0f; H_R.a[2][2] =  1.0f/3.0f;


	X_R_H.a[0][0] =  2.0f/3.0f; X_R_H.a[0][1] = -1.0f/3.0f; X_R_H.a[0][2] = -1.0f/3.0f;
	X_R_H.a[1][0] =  1.0f/3.0f; X_R_H.a[1][1] =  1.0f/3.0f; X_R_H.a[1][2] = -2.0f/3.0f;
	X_R_H.a[2][0] =  1.0f/3.0f; X_R_H.a[2][1] =  1.0f/3.0f; X_R_H.a[2][2] =  1.0f/3.0f;

	X_H_R.a[0][0] =  1; X_H_R.a[0][1] =  0; X_H_R.a[0][2] =  1;
	X_H_R.a[1][0] = -1; X_H_R.a[1][1] =  1; X_H_R.a[1][2] =  1;
	X_H_R.a[2][0] =  0; X_H_R.a[2][1] = -1; X_H_R.a[2][2] =  1;


	a1 = param[0];
	b1 = param[1];
	c1 = param[2];
	al1 = param[3];
	bt1 = param[4];
	gm1 = param[5];

	if (thermalID == 0) calc_U_Beta_trans_coefficients(0);
//--- Rhombohedral Axes
	if (settingID == 2) {
		param_a_txt->enable();    param_b_txt->disable();    param_c_txt->disable();
		param_al_txt->enable();   param_bt_txt->disable();   param_gm_txt->disable();
		param_ea_txt->enable();   param_eb_txt->disable();   param_ec_txt->disable();
		param_eal_txt->enable();  param_ebt_txt->disable();  param_egm_txt->disable();
	}
//--- Hexagonal Axes
	else {
		param_a_txt->enable();    param_b_txt->disable();    param_c_txt->enable();
		param_al_txt->disable();  param_bt_txt->disable();   param_gm_txt->disable();
		param_ea_txt->enable();   param_eb_txt->disable();   param_ec_txt->enable();
		param_eal_txt->disable(); param_ebt_txt->disable();  param_egm_txt->disable();
	}
	I1 = eCell.Setting;
	I2 = settingID;
	if ((I1 == 1) && (I2 == 2)){   //--- From (H) to (R) 
		d = (c1 / a1) * (c1 / a1);
		cf = (d - (3.0/2.0)) / (d + 3.0);
		b2 = c2 = a2 = a1 * sqrt(3 + d) / 3;
		bt2 = gm2 = al2 = acos(cf)*180/PI;
	}
	if ((I1 == 2) && (I2 == 1)){   //--- From (R) to (H) 
		b2 = a2 = a1 * sqrt(2 - 2 * cos(al1*PI/180));
		     c2 = a1 * sqrt(3 + 6 * cos(al1*PI/180));
		al2 = bt2 = 90;
		gm2 = 120;
	}
	param[0] = a2;
	param[1] = b2; 
	param[2] = c2;
	param[3] = al2;
	param[4] = bt2;
	param[5] = gm2;
	eparam[0] = eparam[1] = eparam[2];
	eparam[3] = eparam[4] = eparam[5];
	param_a_txt->set_float_val(param[0]);
	param_b_txt->set_float_val(param[1]);
	param_c_txt->set_float_val(param[2]);
	param_al_txt->set_float_val(param[3]);
	param_bt_txt->set_float_val(param[4]);
	param_gm_txt->set_float_val(param[5]);

	param_ea_txt->set_float_val(eparam[0]);
	param_eb_txt->set_float_val(eparam[1]);
	param_ec_txt->set_float_val(eparam[2]);
	param_eal_txt->set_float_val(eparam[3]);
	param_ebt_txt->set_float_val(eparam[4]);
	param_egm_txt->set_float_val(eparam[5]);
	erange[0] = erange[2] = erange[4] = 0;
	erange[1] = erange[3] = erange[5] = 1;
	if (thermalID == 0) calc_U_Beta_trans_coefficients(1);

	if (update_param_check->get_int_val() == 1) {
		for(i=0; i<TotalAtomNum; i++){
			if (usethermalID == 1){
				U.a[0][0] = eAtom[i].B[0];
				U.a[1][1] = eAtom[i].B[1];
				U.a[2][2] = eAtom[i].B[2];
				U.a[0][1] = U.a[1][0] = eAtom[i].B[3];
				U.a[0][2] = U.a[2][0] = eAtom[i].B[4];
				U.a[1][2] = U.a[2][1] = eAtom[i].B[5];
				if (thermalID == 0) U = U_Beta_trans(0, U);
			}
			if ((I1 == 1) && (I2 == 2)){   //--- From (H) to (R) 
				x = eAtom[i].x*X_H_R.a[0][0] + eAtom[i].y*X_H_R.a[0][1] + eAtom[i].z*X_H_R.a[0][2];
				y = eAtom[i].x*X_H_R.a[1][0] + eAtom[i].y*X_H_R.a[1][1] + eAtom[i].z*X_H_R.a[1][2];
				z = eAtom[i].x*X_H_R.a[2][0] + eAtom[i].y*X_H_R.a[2][1] + eAtom[i].z*X_H_R.a[2][2];
				if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
				if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
				if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
				eAtom[i].x = (float)x;
				eAtom[i].y = (float)y;
				eAtom[i].z = (float)z;
				if (usethermalID == 1){
					W = MM( RM(TM(H_R)) , MM(U,RM(H_R)) );
					if (thermalID == 0) W = U_Beta_trans(1, W);
					eAtom[i].B[0] = W.a[0][0];
					eAtom[i].B[1] = W.a[1][1];
					eAtom[i].B[2] = W.a[2][2];
					eAtom[i].B[3] = W.a[0][1];
					eAtom[i].B[4] = W.a[0][2];
					eAtom[i].B[5] = W.a[1][2];
				}
			}
			if ((I1 == 2) && (I2 == 1)){   //--- From (R) to (H) 
				x = eAtom[i].x*X_R_H.a[0][0] + eAtom[i].y*X_R_H.a[0][1] + eAtom[i].z*X_R_H.a[0][2];
				y = eAtom[i].x*X_R_H.a[1][0] + eAtom[i].y*X_R_H.a[1][1] + eAtom[i].z*X_R_H.a[1][2];
				z = eAtom[i].x*X_R_H.a[2][0] + eAtom[i].y*X_R_H.a[2][1] + eAtom[i].z*X_R_H.a[2][2];
				if (x < 0) x = x + 1; if (x >= 1) x = x - 1;
				if (y < 0) y = y + 1; if (y >= 1) y = y - 1;
				if (z < 0) z = z + 1; if (z >= 1) z = z - 1;
				eAtom[i].x = (float)x;
				eAtom[i].y = (float)y;
				eAtom[i].z = (float)z;
				if (usethermalID == 1){
					W = MM( RM(TM(R_H)) , MM(U,RM(R_H)) );
					if (thermalID == 0) W = U_Beta_trans(1, W);
					eAtom[i].B[0] = W.a[0][0];
					eAtom[i].B[1] = W.a[1][1];
					eAtom[i].B[2] = W.a[2][2];
					eAtom[i].B[3] = W.a[0][1];
					eAtom[i].B[4] = W.a[0][2];
					eAtom[i].B[5] = W.a[1][2];
				}
			}
		}
		struc[1] = eAtom[cAtomNum].x;
		struc[2] = eAtom[cAtomNum].y;
		struc[3] = eAtom[cAtomNum].z;
		estruc[0] = eAtom[cAtomNum].ex;
		estruc[1] = eAtom[cAtomNum].ey;
		estruc[2] = eAtom[cAtomNum].ez;

		for(i=0; i<6; i++) etherm[i] = eAtom[cAtomNum].B[i];

		struc_x_txt->set_float_val(struc[1]);
		struc_y_txt->set_float_val(struc[2]);
		struc_z_txt->set_float_val(struc[3]);
		struc_ex_txt->set_float_val(estruc[0]);
		struc_ey_txt->set_float_val(estruc[1]);
		struc_ez_txt->set_float_val(estruc[2]);

		thermal_B11_txt->set_float_val(etherm[0]);
		thermal_B22_txt->set_float_val(etherm[1]);
		thermal_B33_txt->set_float_val(etherm[2]);
		thermal_B12_txt->set_float_val(etherm[3]);
		thermal_B13_txt->set_float_val(etherm[4]);
		thermal_B23_txt->set_float_val(etherm[5]);
	}
}

//********************  make_lattice_constrains()  **************************//
void make_lattice_constrains()
{
	switch(symmetryID)
	{
	case(3):
	case(4):
	case(5):{     //--- Hexagonal lattice
		param[1] = param[0];
		eparam[1] = eparam[0];
		break;
			}
	case(6):{     //--- Cubic lattice
		param[2] = param[1] = param[0];
		eparam[2] = eparam[1] = eparam[0];
		break;
			}
	case(7):{     //--- Rhombohedral lattice
		if (settingID == 1){                   //--- Hexagonal Setting
			param[1] = param[0];
			eparam[1] = eparam[0];
		}
		else {                                 //--- Rhombohedral Setting
			param[2] = param[1] = param[0];
			eparam[2] = eparam[1] = eparam[0];
			param[5] = param[4] = param[3];
			eparam[5] = eparam[4] = eparam[3];
		}
		break;
			}
	}
	param_a_txt->set_float_val(param[0]);
	param_b_txt->set_float_val(param[1]);
	param_c_txt->set_float_val(param[2]);
	param_al_txt->set_float_val(param[3]);
	param_bt_txt->set_float_val(param[4]);
	param_gm_txt->set_float_val(param[5]);

	param_ea_txt->set_float_val(eparam[0]);
	param_eb_txt->set_float_val(eparam[1]);
	param_ec_txt->set_float_val(eparam[2]);
	param_eal_txt->set_float_val(eparam[3]);
	param_ebt_txt->set_float_val(eparam[4]);
	param_egm_txt->set_float_val(eparam[5]);
}


//********************  update_dialog_param_2()  ****************************//
void update_dialog_param_2()
{
	switch(symmetryID)
	{
	case(1):{   //--- Monoclinic
		update_monoclinic_param();
		eCell.Setting = settingID;
		break;
			}
	case(2):{   //--- Orthorhombic
		if (update_param_check->get_int_val() == 1) {
			update_orthorombic_param();
			eCell.Setting = settingID + originID*6;
		}
		break;
			}
	case(7):{   //--- Rhombohedral
		update_rhombohedral_param();
		eCell.Setting = settingID;
		break;
			}
	}
	fulltext_txt -> set_text(space_group[nameID].full_name[settingID-1]);
	param_a_txt->set_float_val(param[0]);
	param_b_txt->set_float_val(param[1]);
	param_c_txt->set_float_val(param[2]);
	param_al_txt->set_float_val(param[3]);
	param_bt_txt->set_float_val(param[4]);
	param_gm_txt->set_float_val(param[5]);

	param_ea_txt->set_float_val(eparam[0]);
	param_eb_txt->set_float_val(eparam[1]);
	param_ec_txt->set_float_val(eparam[2]);
	param_eal_txt->set_float_val(eparam[3]);
	param_ebt_txt->set_float_val(eparam[4]);
	param_egm_txt->set_float_val(eparam[5]);
}

//********************  get_symmetry_ID()  **********************************//
void get_symmetry_ID()
{
//---- Get Symmetry from Space Group Number
	if (numberID >= 195) symmetryID = 6;                                   //--- Cubic
	if ((numberID >= 168) && (numberID <= 194)) symmetryID = 5;            //--- Hexagonal
	if ((numberID >= 143) && (numberID <= 167)) {
		if ((numberID == 146) || (numberID == 148) || (numberID == 155) || 
			(numberID == 160) || (numberID == 161) || (numberID == 166) || 
			(numberID == 167)) { symmetryID = 7; }                         //--- Rhombohedral 
		else symmetryID = 4;                                               //--- Trigonal
	}
	if ((numberID >= 75) && (numberID <= 142)) symmetryID = 3;             //--- Tetragonal
	if ((numberID >= 16) && (numberID <= 74)) symmetryID = 2;              //--- Orthorhombic
	if ((numberID >= 3) && (numberID <= 15)) symmetryID = 1;               //--- Monoclinic
	if ((numberID >=  0) && (numberID <= 2)) symmetryID = 0;               //--- Triclinic
	switch(symmetryID)
	{
	case(4):
	case(5):
	case(7):{
		if (eCell.Setting == 1){
			param[3] = param[4] = 90.0f;
			param[5] = 120.0f;
		}
		break;
			}
	}
}


//********************  update_dialog_param_1()  ****************************//
void update_dialog_param_1()
{
//---- Update Symmetry Information
	symmetry_txt -> set_text(symmetry[symmetryID]);
	fulltext_txt -> set_text(space_group[nameID].full_name[settingID-1]);
	setting_spin->set_int_limits(1,space_group[nameID].SGS,GLUI_LIMIT_CLAMP);
	setting_spin->set_int_val(settingID);
//---- Update Unit Cell Information
	switch(symmetryID)
	{
	case(0):{   //--- Triclinic
		param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
		param_al_txt->enable();   param_bt_txt->enable();   param_gm_txt->enable();
		param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
		param_eal_txt->enable();  param_ebt_txt->enable();  param_egm_txt->enable();
		break;
			}
	case(1):{   //--- Monoclinic
		param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
		param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
		if (settingID < 3) {
			param_al_txt->disable();  param_bt_txt->enable();   param_gm_txt->disable();
			param_eal_txt->disable(); param_ebt_txt->enable();  param_egm_txt->disable();
		}
		else {
			param_al_txt->disable();  param_bt_txt->disable();   param_gm_txt->enable();
			param_eal_txt->disable(); param_ebt_txt->disable();  param_egm_txt->enable();
		}
		break;
			}
	case(2):{   //--- Orthorhombic
		param_a_txt->enable();    param_b_txt->enable();    param_c_txt->enable();
		param_al_txt->disable();  param_bt_txt->disable();  param_gm_txt->disable();
		param_ea_txt->enable();   param_eb_txt->enable();   param_ec_txt->enable();
		param_eal_txt->disable(); param_ebt_txt->disable(); param_egm_txt->disable();
		break;
			}
	case(3):{   //--- Tetragonal
		param_a_txt->enable();    param_b_txt->disable();    param_c_txt->enable();
		param_al_txt->disable();  param_bt_txt->disable();   param_gm_txt->disable();
		param_ea_txt->enable();   param_eb_txt->disable();   param_ec_txt->enable();
		param_eal_txt->disable(); param_ebt_txt->disable();  param_egm_txt->disable();
		break;
			}
	case(4):   //--- Trigonal (not Rhombohedral)
	case(5):{  //--- Hexagonal
		param_a_txt->enable();    param_b_txt->disable();    param_c_txt->enable();
		param_al_txt->disable();  param_bt_txt->disable();   param_gm_txt->disable();
		param_ea_txt->enable();   param_eb_txt->disable();   param_ec_txt->enable();
		param_eal_txt->disable(); param_ebt_txt->disable();  param_egm_txt->disable();
		break;
			}
	case(6):{   //--- Cubic
		param_a_txt->enable();    param_b_txt->disable();    param_c_txt->disable();
		param_al_txt->disable();  param_bt_txt->disable();   param_gm_txt->disable();
		param_ea_txt->enable();   param_eb_txt->disable();   param_ec_txt->disable();
		param_eal_txt->disable(); param_ebt_txt->disable();  param_egm_txt->disable();
		break;
			}
	case(7):{  //--- Rhombohedral
		if (settingID == 1) {	//--- Hexagonal Setting
			param_a_txt->enable();    param_b_txt->disable();    param_c_txt->enable();
			param_al_txt->disable();  param_bt_txt->disable();   param_gm_txt->disable();
			param_ea_txt->enable();   param_eb_txt->disable();   param_ec_txt->enable();
			param_eal_txt->disable(); param_ebt_txt->disable();  param_egm_txt->disable();
		}
		else {	                //--- Rhombohedral Setting
			param_a_txt->enable();   param_b_txt->disable();    param_c_txt->disable();
			param_al_txt->enable();  param_bt_txt->disable();   param_gm_txt->disable();
			param_ea_txt->enable();  param_eb_txt->disable();   param_ec_txt->disable();
			param_eal_txt->enable(); param_ebt_txt->disable();  param_egm_txt->disable();
		}
		break;
			}
	}
	param_a_txt->set_float_val(param[0]);
	param_b_txt->set_float_val(param[1]);
	param_c_txt->set_float_val(param[2]);
	param_al_txt->set_float_val(param[3]);
	param_bt_txt->set_float_val(param[4]);
	param_gm_txt->set_float_val(param[5]);

	param_ea_txt->set_float_val(eparam[0]);
	param_eb_txt->set_float_val(eparam[1]);
	param_ec_txt->set_float_val(eparam[2]);
	param_eal_txt->set_float_val(eparam[3]);
	param_ebt_txt->set_float_val(eparam[4]);
	param_egm_txt->set_float_val(eparam[5]);
}

//********************  update_thermal_mode()  ******************************//
void update_thermal_mode()
{
	int i,j;
	switch(thermalID)
	{
	case(0):{
		thermal_B11_txt->set_name("U11 = ");
		thermal_B22_txt->set_name("U22 = ");
		thermal_B33_txt->set_name("U33 = ");
		thermal_B12_txt->set_name("U12 = ");
		thermal_B13_txt->set_name("U13 = ");
		thermal_B23_txt->set_name("U23 = ");
		break;
			}
	case(1):{
		thermal_B11_txt->set_name("beta11 = ");
		thermal_B22_txt->set_name("beta22 = ");
		thermal_B33_txt->set_name("beta33 = ");
		thermal_B12_txt->set_name("beta12 = ");
		thermal_B13_txt->set_name("beta13 = ");
		thermal_B23_txt->set_name("beta23 = ");
		break;
			}
	}
	if (thermalID != cthermalID){
		switch(thermalID){
		case(0):{
			for(i=0; i<TotalAtomNum; i++) {
				for(j=0; j<6; j++) eAtom[i].B[j] /= (float)UB_TR[j];
			}
			break;
				}
		case(1):{
			for(i=0; i<TotalAtomNum; i++) {
				for(j=0; j<6; j++) eAtom[i].B[j] *= (float)UB_TR[j];
			}
			break;
				}
		}
		for(j=0; j<6; j++) etherm[j] = eAtom[cAtomNum].B[j];
		thermal_B11_txt->set_float_val(etherm[0]);
		thermal_B22_txt->set_float_val(etherm[1]);
		thermal_B33_txt->set_float_val(etherm[2]);
		thermal_B12_txt->set_float_val(etherm[3]);
		thermal_B13_txt->set_float_val(etherm[4]);
		thermal_B23_txt->set_float_val(etherm[5]);
	}
	cthermalID = thermalID;
}


//********************  update_iso_param()  *********************************//
void update_iso_param()
{
	int i;
	float dif;

	dif = (float)(8.0*PI*PI);
	switch(isoID){
	case(0):{   //------ from Uiso to Biso
		for(i=0; i<TotalAtomNum; i++) eAtom[i].Biso *= dif;
		break;
			}
	case(1):{   //------ from Biso to Uiso 
		for(i=0; i<TotalAtomNum; i++) eAtom[i].Biso /= dif;
		break;
			}
	}
	struc_B_txt->set_float_val(eAtom[cAtomNum].Biso);

}

//********************  update_structure_param()  ***************************//
void update_structure_param()
{
	int i;
	if (TotalAtomNum == 0) atom_number_spin->disable();
	else {
		atom_number_spin->enable();
		atom_number_spin->set_int_limits(1,TotalAtomNum,GLUI_LIMIT_CLAMP);
		sprintf(cAtomLabel,""); sprintf(cAtomLabel,"%s",eAtom[cAtomNum].name);
		sprintf(cAtomSymbol,"");sprintf(cAtomSymbol,"%s",eAtom[cAtomNum].simb);
		struc[0] = eAtom[cAtomNum].g;
		struc[1] = eAtom[cAtomNum].x;
		struc[2] = eAtom[cAtomNum].y;
		struc[3] = eAtom[cAtomNum].z;
		estruc[0] = eAtom[cAtomNum].ex;
		estruc[1] = eAtom[cAtomNum].ey;
		estruc[2] = eAtom[cAtomNum].ez;
		for(i=0; i<6; i++) etherm[i] = eAtom[cAtomNum].B[i];
		struc[4] = eAtom[cAtomNum].Biso;
	}
	struc_l_txt->set_text(cAtomLabel);
	struc_s_txt->set_text(cAtomSymbol);
	struc_g_txt->set_float_val(struc[0]);
	struc_B_txt->set_float_val(struc[4]);
	struc_x_txt->set_float_val(struc[1]);
	struc_y_txt->set_float_val(struc[2]);
	struc_z_txt->set_float_val(struc[3]);
	struc_ex_txt->set_float_val(estruc[0]);
	struc_ey_txt->set_float_val(estruc[1]);
	struc_ez_txt->set_float_val(estruc[2]);
	numAtomLabel->set_int_val(TotalAtomNum);
	thermal_B11_txt->set_float_val(etherm[0]);
	thermal_B22_txt->set_float_val(etherm[1]);
	thermal_B33_txt->set_float_val(etherm[2]);
	thermal_B12_txt->set_float_val(etherm[3]);
	thermal_B13_txt->set_float_val(etherm[4]);
	thermal_B23_txt->set_float_val(etherm[5]);
}

//********************  update_navigator_panel()  ***************************//
void update_navigator_panel()
{
	if (TotalAtomNum == 0){
		a_btn->enable();
		l_btn->disable();
		n_btn->disable();
		p_btn->disable();
		f_btn->disable();
		e_btn->disable();
		d_btn->disable();
	}
	else {
		a_btn->enable();
		e_btn->enable();
		d_btn->enable();
		l_btn->enable();
		n_btn->enable();
		p_btn->enable();
		f_btn->enable();
		if (TotalAtomNum == 1){
			l_btn->disable();
			n_btn->disable();
			p_btn->disable();
			f_btn->disable();
		}
		else {
			if (cAtomNum == 0){
				l_btn->enable();
				n_btn->enable();
				p_btn->disable();
				f_btn->disable();
			}
			if ((cAtomNum != 0) && (cAtomNum < (TotalAtomNum - 1))){
				l_btn->enable();
				n_btn->enable();
				p_btn->enable();
				f_btn->enable();
			}
			if (cAtomNum == (TotalAtomNum - 1)){
				l_btn->disable();
				n_btn->disable();
				p_btn->enable();
				f_btn->enable();
			}
		}
	}
}


//********************  get_navigator_selection()  **************************//
void get_navigator_selection(int ID)
{
	int i,j;
	switch(ID)
	{
	case(0):{
		TotalAtomNum++;
		cAtomNum = TotalAtomNum - 1;
		eAtom[cAtomNum].g = struc[0];
		eAtom[cAtomNum].x = struc[1];
		eAtom[cAtomNum].y = struc[2];
		eAtom[cAtomNum].z = struc[3];
		eAtom[cAtomNum].Biso = struc[4];
		eAtom[cAtomNum].ex = estruc[0];
		eAtom[cAtomNum].ey = estruc[1];
		eAtom[cAtomNum].ez = estruc[2];
		for(i=0; i<6; i++) eAtom[cAtomNum].B[i] = etherm[i];
		sprintf(eAtom[cAtomNum].name,""); sprintf(eAtom[cAtomNum].name,"%s",struc_l_txt->get_text());
		sprintf(eAtom[cAtomNum].simb,""); sprintf(eAtom[cAtomNum].simb,"%s",struc_s_txt->get_text());
		printf("Add atom: %7s %2s %5.f %10.5f %10.5f %10.5f\n",eAtom[cAtomNum].name,eAtom[cAtomNum].simb,
			   eAtom[cAtomNum].g,eAtom[cAtomNum].x,eAtom[cAtomNum].y,eAtom[cAtomNum].z);
		printf("%37.5f %10.5f %10.5f\n",eAtom[cAtomNum].ex,eAtom[cAtomNum].ey,eAtom[cAtomNum].ez);
		atom_number_spin->set_int_limits(1,TotalAtomNum,GLUI_LIMIT_CLAMP);
		atom_number_spin->set_int_val(cAtomNum+1);
		break;
			}
	case(1):{cAtomNum = TotalAtomNum - 1;atom_number_spin->set_int_val(cAtomNum+1);break;}
	case(2):{cAtomNum++;atom_number_spin->set_int_val(cAtomNum+1);break;}
	case(3):{cAtomNum--;atom_number_spin->set_int_val(cAtomNum+1);break;}
	case(4):{cAtomNum = 0;atom_number_spin->set_int_val(cAtomNum+1);break;}
	case(5):{
		eAtom[cAtomNum].g = struc[0];
		eAtom[cAtomNum].Biso = struc[4];
		eAtom[cAtomNum].x = struc[1];
		eAtom[cAtomNum].y = struc[2];
		eAtom[cAtomNum].z = struc[3];
		eAtom[cAtomNum].ex = estruc[0];
		eAtom[cAtomNum].ey = estruc[1];
		eAtom[cAtomNum].ez = estruc[2];
		for(i=0; i<6; i++) eAtom[cAtomNum].B[i] = etherm[i];
		sprintf(eAtom[cAtomNum].name,""); sprintf(eAtom[cAtomNum].name,"%s",struc_l_txt->get_text());
		sprintf(eAtom[cAtomNum].simb,""); sprintf(eAtom[cAtomNum].simb,"%s",struc_s_txt->get_text());
		break;
			}
	case(6):{
		if (cAtomNum < (TotalAtomNum - 1)){
			for(i=cAtomNum; i<TotalAtomNum; i++){
				sprintf(eAtom[i].name,""); sprintf(eAtom[i].name,"%s",eAtom[i+1].name);
				sprintf(eAtom[i].simb,""); sprintf(eAtom[i].simb,"%s",eAtom[i+1].simb);
				eAtom[i].g = eAtom[i+1].g;
				eAtom[i].x = eAtom[i+1].x;
				eAtom[i].y = eAtom[i+1].y;
				eAtom[i].z = eAtom[i+1].z;
				eAtom[i].ex = eAtom[i+1].ex;
				eAtom[i].ey = eAtom[i+1].ey;
				eAtom[i].ez = eAtom[i+1].ez;
				for(j=0; j<6; j++) eAtom[i].B[j] = eAtom[i+1].B[j];
			}
		}
		TotalAtomNum--;
		cAtomNum = TotalAtomNum - 1;
		atom_number_spin->set_int_limits(1,TotalAtomNum,GLUI_LIMIT_CLAMP);
		atom_number_spin->set_int_val(cAtomNum+1);
		break;
			}
	}
	update_navigator_panel();
	update_structure_param();
}

//********************  new_edit_file_control_cb()  *************************//
void new_edit_file_control_cb( int control )
{
	int I1;
	switch(control)
	{
//--- "Apply" / "OK" Button
	case(1):{
		if (c_mode_ID == 0) {
			new_edit_Dialog_ID = 1;
			newfile->close();
		}
		else new_edit_Dialog_ID = 2;
		break;
			}
//--- "Close" / "Cancel" Button
	case(2):{ 
		new_edit_Dialog_ID = 0;
		newfile->close();
		break;
			}
//--- Select Space Group Number
	case(5):{
		settingID = 1;
		originID = 0;
		nameID = numberID - 1;
		sp_grp_lst->set_int_val(nameID);
		if ((numberID == 48) || (numberID == 50) || (numberID == 59) ||
			(numberID == 68) || (numberID == 70)) {
			if (c_mode_ID != 0) {
				ortho_orig->enable();
				update_param_check->set_int_val(0);
			}
		}
		else {
			ortho_orig->disable();
			update_param_check->set_int_val(1);
		}
		get_symmetry_ID();
		eparam[0] = eparam[1] = eparam[2] = eparam[3] = eparam[4] = eparam[5] = 0.0f;
		erange[0] = erange[2] = erange[4] = 0;
		erange[1] = erange[3] = erange[5] = 1;
		make_lattice_constrains();
		update_dialog_param_1();
		break;
			}
//--- Select Space Group Name
	case(10):{
		settingID = 1;
		originID = 0;
		numberID = nameID + 1; 
		number_spin->set_int_val(numberID);
		if ((numberID == 48) || (numberID == 50) || (numberID == 59) ||
			(numberID == 68) || (numberID == 70)) {
			if (c_mode_ID != 0) {
				ortho_orig->enable();
				update_param_check->set_int_val(0);
			}
		}
		else {
			ortho_orig->disable();
			update_param_check->set_int_val(1);
		}
		get_symmetry_ID();
		eparam[0] = eparam[1] = eparam[2] = eparam[3] = eparam[4] = eparam[5] = 0.0f;
		erange[0] = erange[2] = erange[4] = 0;
		erange[1] = erange[3] = erange[5] = 1;
		make_lattice_constrains();
		update_dialog_param_1();
		break;
			 }
//--- Select Space Group Setting
	case(15):{
		get_symmetry_ID();
		update_dialog_param_2();
		break;
			 }
//--- Unit Cell Constains
	case(20):{
		make_lattice_constrains();
		break;
			 }
//--- Atom Number Spin
	case(40):{
		cAtomNum = atom_number_spin->get_int_val() - 1;
		update_navigator_panel();
		update_structure_param();
		break;
			 }
//--- Navigator
	case(50):
	case(51):
	case(52):
	case(53):
	case(54):
	case(55):
	case(56):{get_navigator_selection(control - 50);break;}
//--- Type of Anizotropic Thermal Parameters
	case(60):{
		update_thermal_mode();
		break;
			 }
	case(61):{
		if (usethermalID == 0) {
			thermalMode->disable();
			thermal_panel->disable();
		}
		else {
			thermalMode->enable();
			thermal_panel->enable();
		}
		break;
			 }
	case(62):{
		I1 = use_Biso_check->get_int_val();
		if (I1 == 1) {
			isoID = 0;
			struc_B_txt->set_name("B = ");
		}
		else {
			isoID = 1;
			struc_B_txt->set_name("U = ");
		}
		update_iso_param();
		break;
			 }
	case(70):{
		new_edit_Dialog_ID = 10;
		ortho_orig->disable();
		break;
			 }
	}
}


//********************  set_new_edit_file_dialog_param()  *******************//
void set_new_edit_file_dialog_param(char *sTitle, int SpGrNum, int SpGrSet, 
									float CellPar[6], float CellEsd[6], 
									int numOfAtom, int thermal_mode, int use_thermal,
									float plot_range[6], char *path_to_prog,
									int type_iso)
{
	int i;
	double ar,br,cr,ca,cb,cg,sa,sb,sg,Vol,SS;

	sprintf(eTitle,"");sprintf(eTitle,"%s",sTitle);

	numberID = SpGrNum; 
	nameID = SpGrNum - 1;
	if (SpGrSet > 6) {settingID = SpGrSet - 6; originID = 1;}
	else {settingID = SpGrSet;originID = 0;}
	get_symmetry_ID();

	TotalAtomNum = numOfAtom;
	for(i=0; i<6; i++){
		eCell.a[i] = param[i] = CellPar[i];
		eparam[i] = CellEsd[i];
		erange[i] = plot_range[i];
	}

	thermalID = thermal_mode;
	cthermalID = thermal_mode;
	usethermalID = use_thermal;

	sprintf(path,"");
	sprintf(path,"%s",path_to_prog);

	isoID = type_iso;

	ca = cos(eCell.a[3]*RAD);
	cb = cos(eCell.a[4]*RAD);
	cg = cos(eCell.a[5]*RAD);
	sa = sin(eCell.a[3]*RAD);
	sb = sin(eCell.a[4]*RAD);
	sg = sin(eCell.a[5]*RAD);
	SS = 1 - ca*ca - cb*cb - cg*cg +2.0*ca*cb*cg;
	Vol = eCell.a[0] * eCell.a[1] * eCell.a[2] * sqrt(SS);
	ar = eCell.a[1]*eCell.a[2]*sa/Vol;
	br = eCell.a[0]*eCell.a[2]*sb/Vol;
	cr = eCell.a[0]*eCell.a[1]*sg/Vol;
	UB_TR[0] = SPI2*ar*ar;
	UB_TR[1] = SPI2*br*br;
	UB_TR[2] = SPI2*cr*cr;
	UB_TR[3] = SPI2*ar*br;
	UB_TR[4] = SPI2*ar*cr;
	UB_TR[5] = SPI2*br*cr;
}

//********************  set_struc_param()  **********************************//
void set_struc_param(int ID, char *name, char *symb, float g, float x, float y, float z, 
                     float ex, float ey, float ez, float ATP[6], float BISO)
{
	int i;
	sprintf(eAtom[ID].name,"");sprintf(eAtom[ID].name,"%s",name);
	sprintf(eAtom[ID].simb,"");sprintf(eAtom[ID].simb,"%s",symb);
	eAtom[ID].x  =  x; eAtom[ID].y  =  y; eAtom[ID].z  =  z;
	eAtom[ID].ex = ex; eAtom[ID].ey = ey; eAtom[ID].ez = ez;
	eAtom[ID].g = g;
	eAtom[ID].Biso = BISO;
	if (usethermalID == 1) {
		for(i=0; i<6; i++) eAtom[ID].B[i] = ATP[i];
	}
	else {
		for(i=0; i<6; i++) eAtom[ID].B[i] = 0;
	}
}

//********************  init_new_edit_file_dialog()  ************************//
void init_new_edit_file_dialog()
{
	int i;

	title_txt->set_text(eTitle);

	load_spgra();
	for(i=0; i<NSG; i++) sp_grp_lst->add_item(i,space_group[i].name);
	setting_spin->set_int_limits(1,space_group[nameID].SGS,GLUI_LIMIT_CLAMP);

	eCell.SpGrN = numberID; 
	eCell.Setting = settingID + originID*6; 
	sprintf(eCell.SpGr,"");
	sprintf(eCell.SpGr,"%s",space_group[nameID].name);
	eCell.lType = eCell.SpGr[0];
	get_symmetry_ID();

	number_spin->set_int_val(numberID);
	sp_grp_lst->set_int_val(nameID);
	setting_spin->set_int_val(settingID);
	fulltext_txt -> set_text(space_group[nameID].full_name[settingID-1]);
	symmetry_txt->set_text(symmetry[symmetryID]);
	if ((numberID == 48) || (numberID == 50) || (numberID == 59) ||
		(numberID == 68) || (numberID == 70)) {
		ortho_orig->enable();
		update_param_check->set_int_val(0);
	}
	else {
		ortho_orig->disable();
		update_param_check->set_int_val(1);
	}
	use_thermal_check->set_int_val(usethermalID);
	if (usethermalID == 0) {
		thermalMode->disable();
		thermal_panel->disable();
	}
	else {
		thermalMode->enable();
		thermal_panel->enable();
	}
	thermalMode->set_int_val(thermalID);

	update_thermal_mode();
	update_dialog_param_1();
	update_structure_param();
	update_navigator_panel();
	if (isoID == 1) {
		struc_B_txt->set_name("U = ");
		use_Biso_check->set_int_val(0);
	}
	else {
		struc_B_txt->set_name("B = ");
		use_Biso_check->set_int_val(1);
	}

}

//********************  initial_new_edit_file_settings()  *******************//
void initial_new_edit_file_settings()
{
	sprintf(eTitle,"");

	numberID = 1;
	nameID = 0;
	settingID = 1;
	originID = 0;
	symmetryID = 0;
	param[0] = param[1] = param[2] = 1.0f;
	param[3] = param[4] = param[5] = 90.0f;
	eparam[0] = eparam[1] = eparam[2] = eparam[3] = eparam[4] = eparam[5] = 0.0f;

	usethermalID = 0;
	thermalID = 0;

	TotalAtomNum = 0;
	cAtomNum = 0;
	sprintf(cAtomLabel,"");
	sprintf(cAtomSymbol,"");
	struc[0] = 1.0;
	struc[1] = struc[2] =  struc[3] = 0.0f;
	struc[4] = 1.0;
	estruc[0] = estruc[1] = estruc[2] = 0.0f;
	etherm[0] = etherm[1] = etherm[2] = etherm[3] = etherm[4] = etherm[5] = 0.0f;

	erange[0] = erange[2] = erange[4] = 0;
	erange[1] = erange[3] = erange[5] = 1;
}


//********************  open_new_edit_file_dialog()  ************************//
void open_new_edit_file_dialog(int ID, int Mode, int dpX, int dpY)
{
	new_edit_Dialog_ID = -1;
	parent_window_ID = ID;
	c_mode_ID = Mode;

	if (Mode == 0) newfile = GLUI_Master.create_glui("New Data",0,dpX,dpY );
	          else newfile = GLUI_Master.create_glui("Edit Data",0,dpX,dpY );
	newfile->set_main_gfx_window( ID );
//----------------------------------------------------------------------------------------------------
	title_txt = newfile->add_edittext("Title: ",GLUI_EDITTEXT_TEXT);
	title_txt->set_alignment(GLUI_ALIGN_LEFT);
	title_txt->set_w(350);
//----------------------------------------------------------------------------------------------------
	newfile_panel = newfile->add_panel("",GLUI_PANEL_RAISED);
//----------------------------------------------------------------------------------------------------
	block1_panel = newfile->add_panel_to_panel(newfile_panel,"",GLUI_PANEL_NONE);
	block1_panel->set_alignment(GLUI_ALIGN_LEFT);
//----------------------------------------------------------------------------------------------------
	symmetry_panel = newfile->add_panel_to_panel(block1_panel,"Space group symmetry",GLUI_PANEL_EMBOSSED);
	symmetry_panel->set_alignment(GLUI_ALIGN_LEFT);
	number_spin = newfile->add_spinner_to_panel(symmetry_panel,"Number: ",GLUI_SPINNER_INT,&numberID,
		                                                                          5,new_edit_file_control_cb);
	number_spin->set_int_limits(1,230,GLUI_LIMIT_CLAMP);
	number_spin->set_speed(0.01);
	number_spin->set_w(200);
	number_spin->set_alignment(GLUI_ALIGN_RIGHT);
	sp_grp_lst = newfile->add_listbox_to_panel(symmetry_panel,"Std. symbol: ",&nameID,10,new_edit_file_control_cb);
	sp_grp_lst->set_w(200);
	sp_grp_lst->set_alignment(GLUI_ALIGN_RIGHT);
	setting_spin = newfile->add_spinner_to_panel(symmetry_panel,"Setting: ",GLUI_SPINNER_INT,&settingID,
 	                                                                         15,new_edit_file_control_cb);
	setting_spin->set_int_limits(1,6,GLUI_LIMIT_CLAMP);
	setting_spin->set_alignment(GLUI_ALIGN_RIGHT);
	setting_spin->set_w(200);
	newfile->add_separator_to_panel(symmetry_panel);
	symmetry_txt = newfile->add_edittext_to_panel(symmetry_panel,"System: ",GLUI_EDITTEXT_TEXT);
	symmetry_txt->set_text(symmetry[0]);
	symmetry_txt -> disable();
	symmetry_txt->set_w(180);
	symmetry_txt->set_alignment(GLUI_ALIGN_RIGHT);
	fulltext_txt = newfile->add_edittext_to_panel(symmetry_panel,"Symbol: ",GLUI_EDITTEXT_TEXT);
	fulltext_txt -> disable();
	fulltext_txt -> set_w(180);
	fulltext_txt->set_alignment(GLUI_ALIGN_RIGHT);
	newfile->add_separator_to_panel(symmetry_panel);
	ortho_orig = newfile->add_button_to_panel(symmetry_panel,"Origin choice",70,new_edit_file_control_cb);
//----------------------------------------------------------------------------------------------------
	newfile->add_column_to_panel(block1_panel);
//----------------------------------------------------------------------------------------------------
	param_panel = newfile->add_panel_to_panel(block1_panel,"Lattice parameters",GLUI_PANEL_EMBOSSED);
	param_panel->set_alignment(GLUI_ALIGN_LEFT);

	param_a_txt = newfile->add_edittext_to_panel(param_panel,"a = ",GLUI_EDITTEXT_FLOAT,&param[0],
                                                                         20,new_edit_file_control_cb);
	param_b_txt = newfile->add_edittext_to_panel(param_panel,"b = ",GLUI_EDITTEXT_FLOAT,&param[1]);
	param_c_txt = newfile->add_edittext_to_panel(param_panel,"c = ",GLUI_EDITTEXT_FLOAT,&param[2]);
	newfile->add_separator_to_panel(param_panel);
	param_al_txt = newfile->add_edittext_to_panel(param_panel,"alpha = ",GLUI_EDITTEXT_FLOAT,&param[3],
                                                                         20,new_edit_file_control_cb);
	param_bt_txt = newfile->add_edittext_to_panel(param_panel,"beta = ",GLUI_EDITTEXT_FLOAT,&param[4]);
	param_gm_txt = newfile->add_edittext_to_panel(param_panel,"gamma = ",GLUI_EDITTEXT_FLOAT,&param[5]);
	newfile->add_column_to_panel(param_panel,false);
	param_ea_txt = newfile->add_edittext_to_panel(param_panel,"esd(a) = ",GLUI_EDITTEXT_FLOAT,&eparam[0],
                                                                         20,new_edit_file_control_cb);
	param_eb_txt = newfile->add_edittext_to_panel(param_panel,"esd(b) = ",GLUI_EDITTEXT_FLOAT,&eparam[1]);
	param_ec_txt = newfile->add_edittext_to_panel(param_panel,"esd(c) = ",GLUI_EDITTEXT_FLOAT,&eparam[2]);
	newfile->add_separator_to_panel(param_panel);
	param_eal_txt = newfile->add_edittext_to_panel(param_panel,"esd(alpha) = ",GLUI_EDITTEXT_FLOAT,&eparam[3],
                                                                         20,new_edit_file_control_cb);
	param_ebt_txt = newfile->add_edittext_to_panel(param_panel,"esd(beta) = ",GLUI_EDITTEXT_FLOAT,&eparam[4]);
	param_egm_txt = newfile->add_edittext_to_panel(param_panel,"esd(gamma) =",GLUI_EDITTEXT_FLOAT,&eparam[5]);
	param_a_txt->set_int_limits(1,100,GLUI_LIMIT_CLAMP);
	param_al_txt->set_int_limits(1,180,GLUI_LIMIT_CLAMP);
	param_b_txt->set_int_limits(1,100,GLUI_LIMIT_CLAMP);
	param_bt_txt->set_int_limits(1,180,GLUI_LIMIT_CLAMP);
	param_c_txt->set_int_limits(1,100,GLUI_LIMIT_CLAMP);
	param_gm_txt->set_int_limits(1,180,GLUI_LIMIT_CLAMP);
	param_a_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_b_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_c_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_al_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_bt_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_gm_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_ea_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_eb_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_ec_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_eal_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_ebt_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_egm_txt->set_alignment(GLUI_ALIGN_RIGHT);
	param_a_txt->set_w(140);
	param_b_txt->set_w(140);
	param_c_txt->set_w(140);
	param_al_txt->set_w(140);
	param_bt_txt->set_w(140);
	param_gm_txt->set_w(140);
	param_ea_txt->set_w(140);
	param_eb_txt->set_w(140);
	param_ec_txt->set_w(140);
	param_eal_txt->set_w(140);
	param_ebt_txt->set_w(140);
	param_egm_txt->set_w(140);

	update_param_check = newfile->add_checkbox_to_panel(block1_panel,"Update parameters with current settings");
	update_param_check->set_int_val(1);
//----------------------------------------------------------------------------------------------------
	block2_panel = newfile->add_panel_to_panel(newfile_panel,"",GLUI_PANEL_RAISED);
	block2_panel->set_alignment(GLUI_ALIGN_LEFT);

	use_thermal_check = newfile->add_checkbox_to_panel(block2_panel,"Use anisotropic atomic displacement parameters",
		                                               &usethermalID,61,new_edit_file_control_cb);
	newfile->add_column_to_panel(block2_panel,false);
	thermalMode = newfile->add_listbox_to_panel(block2_panel,"Type: ",&thermalID,60,new_edit_file_control_cb);
	thermalMode->add_item(0,"U");
	thermalMode->add_item(1,"beta");
	newfile->add_column_to_panel(block2_panel,true);
	use_Biso_check = newfile->add_checkbox_to_panel(block2_panel,"Use B",NULL,62,new_edit_file_control_cb);
//----------------------------------------------------------------------------------------------------
	struc_main_panel = newfile->add_panel_to_panel(newfile_panel,"Structure parameters",GLUI_PANEL_EMBOSSED);
	struc_main_panel->set_alignment(GLUI_ALIGN_LEFT);

//----------------------------------------------------------------------------------------------------
	struc_panel = newfile->add_panel_to_panel(struc_main_panel,"",GLUI_PANEL_NONE);
	struc_panel->set_alignment(GLUI_ALIGN_LEFT);

	numAtomLabel= newfile->add_edittext_to_panel(struc_panel,"No. of atoms: ",GLUI_EDITTEXT_INT);
	numAtomLabel->disable();
	atom_number_spin = newfile->add_spinner_to_panel(struc_panel,"Atom No: ",GLUI_SPINNER_INT,NULL,40,new_edit_file_control_cb);
	atom_number_spin->set_int_limits(0,0,GLUI_LIMIT_CLAMP);
	number_spin->set_speed(0.01);
	atom_number_spin->set_int_val(0);
	struc_l_txt = newfile->add_edittext_to_panel(struc_panel,"Label: ",GLUI_EDITTEXT_TEXT,cAtomLabel);
	struc_s_txt = newfile->add_edittext_to_panel(struc_panel,"Symbol: ",GLUI_EDITTEXT_TEXT,cAtomSymbol);
	newfile->add_column_to_panel(struc_panel,false);
	struc_x_txt  = newfile->add_edittext_to_panel(struc_panel,"x = ",GLUI_EDITTEXT_FLOAT,&struc[1]);
	struc_y_txt  = newfile->add_edittext_to_panel(struc_panel,"y = ",GLUI_EDITTEXT_FLOAT,&struc[2]);
	struc_z_txt  = newfile->add_edittext_to_panel(struc_panel,"z = ",GLUI_EDITTEXT_FLOAT,&struc[3]);
	struc_g_txt = newfile->add_edittext_to_panel(struc_panel,"g = ",GLUI_EDITTEXT_FLOAT,&struc[0]);
	newfile->add_column_to_panel(struc_panel,false);
	struc_ex_txt = newfile->add_edittext_to_panel(struc_panel,"esd(x) = ",GLUI_EDITTEXT_FLOAT,&estruc[0]);
	struc_ey_txt = newfile->add_edittext_to_panel(struc_panel,"esd(y) = ",GLUI_EDITTEXT_FLOAT,&estruc[1]);
	struc_ez_txt = newfile->add_edittext_to_panel(struc_panel,"esd(z) = ",GLUI_EDITTEXT_FLOAT,&estruc[2]);
	struc_B_txt  = newfile->add_edittext_to_panel(struc_panel,"B = ",GLUI_EDITTEXT_FLOAT,&struc[4]);
//	if (Mode != 0)struc_B_txt->disable();

//----------------------------------------------------------------------------------------------------
	thermal_panel = newfile->add_panel_to_panel(struc_main_panel,"",GLUI_PANEL_RAISED);
	thermal_panel->set_alignment(GLUI_ALIGN_LEFT);

	thermal_B11_txt = newfile->add_edittext_to_panel(thermal_panel,"U11 = ",GLUI_EDITTEXT_FLOAT,&etherm[0]);
	thermal_B12_txt = newfile->add_edittext_to_panel(thermal_panel,"U12 = ",GLUI_EDITTEXT_FLOAT,&etherm[3]);
	newfile->add_column_to_panel(thermal_panel,false);
	thermal_B22_txt = newfile->add_edittext_to_panel(thermal_panel,"U22 = ",GLUI_EDITTEXT_FLOAT,&etherm[1]);
	thermal_B13_txt = newfile->add_edittext_to_panel(thermal_panel,"U13 = ",GLUI_EDITTEXT_FLOAT,&etherm[4]);
	newfile->add_column_to_panel(thermal_panel,false);
	thermal_B33_txt = newfile->add_edittext_to_panel(thermal_panel,"U33 = ",GLUI_EDITTEXT_FLOAT,&etherm[2]);
	thermal_B23_txt = newfile->add_edittext_to_panel(thermal_panel,"U23 = ",GLUI_EDITTEXT_FLOAT,&etherm[5]);
//----------------------------------------------------------------------------------------------------
	newfile->add_column_to_panel(struc_main_panel);
//----------------------------------------------------------------------------------------------------
	navigate_panel = newfile->add_panel_to_panel(struc_main_panel,"",GLUI_PANEL_NONE);

	a_btn = newfile->add_button_to_panel(navigate_panel,"Add", 50, new_edit_file_control_cb);
	l_btn = newfile->add_button_to_panel(navigate_panel,">>", 51, new_edit_file_control_cb);
	n_btn = newfile->add_button_to_panel(navigate_panel,">", 52, new_edit_file_control_cb);
	p_btn = newfile->add_button_to_panel(navigate_panel,"<", 53, new_edit_file_control_cb);
	f_btn = newfile->add_button_to_panel(navigate_panel,"<<", 54, new_edit_file_control_cb);
	e_btn = newfile->add_button_to_panel(navigate_panel,"Update", 55, new_edit_file_control_cb);
	d_btn = newfile->add_button_to_panel(navigate_panel,"Delete", 56, new_edit_file_control_cb);
	a_btn->set_w(60); 
	l_btn->set_w(60); 
	n_btn->set_w(60);
	p_btn->set_w(60);
	f_btn->set_w(60);
	d_btn->set_w(60);
	e_btn->set_w(60);
//----------------------------------------------------------------------------------------------------
	new_btn_panel = newfile->add_panel("",GLUI_PANEL_NONE);
	if (c_mode_ID == 0) {
		newfile->add_button_to_panel(new_btn_panel,"OK",1,new_edit_file_control_cb);
		newfile->add_column_to_panel(new_btn_panel,false);
		newfile->add_button_to_panel(new_btn_panel,"Cancel",2,new_edit_file_control_cb);
	}
	else {
		newfile->add_button_to_panel(new_btn_panel,"Apply",1,new_edit_file_control_cb);
		newfile->add_column_to_panel(new_btn_panel,false);
		newfile->add_button_to_panel(new_btn_panel,"Close",2,new_edit_file_control_cb);
	}

	init_new_edit_file_dialog();
}


