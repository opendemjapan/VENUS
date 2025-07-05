
//-----------------------------------------------------
//  Matrix definitions
//-----------------------------------------------------
typedef struct matrtag
{
	float a[3][3];
} MATRIX;


float DM(MATRIX M);
MATRIX RM(MATRIX M);
MATRIX TM(MATRIX M);
MATRIX MM(MATRIX M1, MATRIX M2);
