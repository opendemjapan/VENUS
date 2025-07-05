#include <stdio.h>
#include <math.h>
#include "vics.h"

/****************************************************************************/
void calc_D2_esd2(VECTOR3D V1, VECTOR3D V2, VECTOR3D EV1, VECTOR3D EV2,
				float cell[6], float ecell[6], float max_par,
				double &D, double &esd)
{
	double A1,A2,A3,B1,B2,B3,D1,D2,D3;
	double sa,sb,sg,ca,cb,cg;
	double S1,S2,S3,S4,S5,S6,S7,S8,S9;

	sa = sin(cell[3]*PI/180);
	sb = sin(cell[4]*PI/180);
	sg = sin(cell[5]*PI/180);
	ca = cos(cell[3]*PI/180);
	cb = cos(cell[4]*PI/180);
	cg = cos(cell[5]*PI/180);
	A1 = ecell[0] / cell[0];
	A2 = ecell[1] / cell[1];
	A3 = ecell[2] / cell[2];
	B1 = sa*(ca - cb * cg) * ecell[3];
	B2 = sb*(cb - ca * cg) * ecell[4];
	B3 = sg*(cg - cb * ca) * ecell[5];
	D1 = max_par * (V1.x - V2.x);
	D2 = max_par * (V1.y - V2.y);
	D3 = max_par * (V1.z - V2.z);

	S1 = D1 + D2*cg + D3*cb;
	S2 = D1*D1*A1*A1 + cell[0]*cell[0]*(EV1.x*EV1.x + EV2.x*EV2.x);

	S3 = D1*cg + D2 + D3*ca;
	S4 = D2*D2*A2*A2 + cell[1]*cell[1]*(EV1.y*EV1.y + EV2.y*EV2.y);

	S5 = D1*cb + D2*ca + D3;
	S6 = D3*D3*A3*A3 + cell[2]*cell[2]*(EV1.z*EV1.z + EV2.z*EV2.z);

	S7 = D1*D2*ecell[5]*cell[5];
	S8 = D1*D3*ecell[4]*cell[4];
	S9 = D2*D3*ecell[3]*cell[3];

	D  = D1*D1 + D2*D2 + D3*D3;
	esd = (S1*S1*S2 + S3*S3*S4 + S5*S5*S6 + S7*S7 + S8*S8 + S9*S9)/D;
	if ((A1 == 0) && (A2 == 0) && (A3 == 0) && (B1 == 0) && (B2 == 0) && (B3 == 0)) esd = 0.0;
//	if ((EV1.x == 0) && (EV1.y == 0) && (EV1.z == 0)) esd = 0.0;
//	if ((EV2.x == 0) && (EV2.y == 0) && (EV2.z == 0)) esd = 0.0;
}

/****************************************************************************/
void calc_angle_esd2(VECTOR3D V1, VECTOR3D V2, VECTOR3D V3,
					 VECTOR3D EV1, VECTOR3D EV2, VECTOR3D EV3,
				     float cell[6], float ecell[6], float max_par,
					 double &angle, double &esd)
{
	double D1,D2,D3;
	double DD1,DD2,DD3;
	XYZ U1,U2;
	double cf,Z;
	double x1,y1,z1,x2,y2,z2,x3,y3,z3;
	double l1,l2,l3,ca1,ca3,cb1,cb3,cg1,cg3,A1,A3,B1,B3,C1,C3;
	double sx1,sy1,sz1, sx2,sy2,sz2, sx3,sy3,sz3;
	double S1,S2,S3;
	double esd1,esd2,esd3;
	double K;
	double cosA,cosB,cosG,sinB,sinG;
	double cosAR,sinAR,M[3][3];
	double xx,yy,zz;

	cosA = cos(cell[3]*PI/180);
	cosB = cos(cell[4]*PI/180); sinB = sin(cell[4]*PI/180);
	cosG = cos(cell[5]*PI/180); sinG = sin(cell[5]*PI/180);
	cosAR = (cosB*cosG - cosA)/(sinB*sinG);
	sinAR = sqrt(1 - cosAR*cosAR);

	M[0][0] = 1; M[0][1] = cosG; M[0][2] = cosB;
	M[1][0] = 0; M[1][1] = sinG; M[1][2] = -sinB*cosAR;
	M[2][0] = 0; M[2][1] = 0;    M[2][2] = sinB*sinAR;

	x1 = max_par*V1.x; y1 = max_par*V1.y; z1 = max_par*V1.z;
	x2 = max_par*V2.x; y2 = max_par*V2.y; z2 = max_par*V2.z;
	x3 = max_par*V3.x; y3 = max_par*V3.y; z3 = max_par*V3.z;

	sx1 = cell[0]*cell[0]*EV1.x*EV1.x;
	sy1 = cell[1]*cell[1]*EV1.y*EV1.y;
	sz1 = cell[2]*cell[2]*EV1.z*EV1.z;

	sx2 = cell[0]*cell[0]*EV2.x*EV2.x;
	sy2 = cell[1]*cell[1]*EV2.y*EV2.y;
	sz2 = cell[2]*cell[2]*EV2.z*EV2.z;

	sx3 = cell[0]*cell[0]*EV3.x*EV3.x;
	sy3 = cell[1]*cell[1]*EV3.y*EV3.y;
	sz3 = cell[2]*cell[2]*EV3.z*EV3.z;

	xx = sx1*M[0][0] + sy1*M[0][1] + sz1*M[0][2];
	yy = sx1*M[1][0] + sy1*M[1][1] + sz1*M[1][2];
	zz = sx1*M[2][0] + sy1*M[2][1] + sz1*M[2][2];
	sx1 = xx; sy1 = yy = sz1 = zz;

	xx = sx2*M[0][0] + sy2*M[0][1] + sz2*M[0][2];
	yy = sx2*M[1][0] + sy2*M[1][1] + sz2*M[1][2];
	zz = sx2*M[2][0] + sy2*M[2][1] + sz2*M[2][2];
	sx2 = xx; sy2 = yy = sz2 = zz;

	xx = sx3*M[0][0] + sy3*M[0][1] + sz3*M[0][2];
	yy = sx3*M[1][0] + sy3*M[1][1] + sz3*M[1][2];
	zz = sx3*M[2][0] + sy3*M[2][1] + sz3*M[2][2];
	sx3 = xx; sy3 = yy = sz3 = zz;

	esd1 = sqrt(sx1 + sy1 + sz1);
	esd2 = sqrt(sx2 + sy2 + sz2);
	esd3 = sqrt(sx3 + sy3 + sz3);

	sx1 /= esd1; sy1 /= esd1; sz1 /= esd1;
	sx2 /= esd2; sy2 /= esd2; sz2 /= esd2;
	sx3 /= esd3; sy3 /= esd3; sz3 /= esd3;

	D1 = x2 - x1; D2 = y2 - y1; D3 = z2 - z1;
	l1 = D1*D1 + D2*D2 + D3*D3; l1 = sqrt(l1);
	ca1 = D1/l1; cb1 = D2/l1; cg1 = D3/l1;

	D1 = x2 - x3; D2 = y2 - y3; D3 = z2 - z3;
	l3 = D1*D1 + D2*D2 + D3*D3; l3 = sqrt(l3);
	ca3 = D1/l3; cb3 = D2/l3; cg3 = D3/l3;

	D1 = x1 - x3; D2 = y1 - y3; D3 = z1 - z3;
	l2 = D1*D1 + D2*D2 + D3*D3; l2 = sqrt(l2);

	D1 = max_par * (V1.x - V2.x);
	D2 = max_par * (V1.y - V2.y);
	D3 = max_par * (V1.z - V2.z);
	DD1 = D1*D1 + D2*D2 + D3*D3;
	U1.x = D1; U1.y = D2; U1.z = D3;

	D1 = max_par * (V1.x - V3.x);
	D2 = max_par * (V1.y - V3.y);
	D3 = max_par * (V1.z - V3.z);
	DD2 = D1*D1 + D2*D2 + D3*D3;

	D1 = max_par * (V2.x - V3.x);
	D2 = max_par * (V2.y - V3.y);
	D3 = max_par * (V2.z - V3.z);
	DD3 = D1*D1 + D2*D2 + D3*D3;
	U2.x = -D1; U2.y = -D2; U2.z = -D3;

	cf = (U1.x*U2.x + U1.y*U2.y + U1.z*U2.z) / (sqrt(DD1) * sqrt(DD3));
	angle = acos(cf)*180/PI;

	Z = 1.0/(l1*l1*l3*l3*(1.0-cf*cf));

	A1 = l1*(ca1 - cf*ca3); A3 = l3*(ca3 - cf*ca1);
	B1 = l1*(cb1 - cf*cb3); B3 = l3*(cb3 - cf*cb1);
	C1 = l1*(cg1 - cf*cg3); C3 = l3*(cg3 - cf*cg1);

	S1 = A3*A3*sx1 + (A1+A3)*(A1+A3)*sx2 + A1*A1*sx3;
	S2 = B3*B3*sy1 + (B1+B3)*(B1+B3)*sy2 + B1*B1*sy3;
	S3 = C3*C3*sz1 + (C1+C3)*(C1+C3)*sz2 + C1*C1*sz3;

	K = 4.0; K *= K;
	esd = Z*(S1 + S2 + S3);
	esd = esd*K;
	esd = sqrt(esd);
	printf("%f\n",esd);
}




/****************************************************************************/
void calc_angle_esd21(VECTOR3D V1, VECTOR3D V2, VECTOR3D V3,
					 VECTOR3D EV1, VECTOR3D EV2, VECTOR3D EV3,
					 float max_par, double &angle, double &esd)
{
	double r;
	double esd1,esd2,esd3;
	double D1,D2,D3;
	double DD1,DD2,DD3;
	XYZ U1,U2;
	double cf;


	r = V1.x*V1.x + V1.y*V1.y + V1.z*V1.z;
	esd1 = (V1.x*V1.x*EV1.x*EV1.x + V1.y*V1.y*EV1.y*EV1.y + V1.z*V1.z*EV1.z*EV1.z)/r;

	r = V2.x*V2.x + V2.y*V2.y + V2.z*V2.z;
	esd2 = (V2.x*V2.x*EV2.x*EV2.x + V2.y*V2.y*EV2.y*EV2.y + V2.z*V2.z*EV2.z*EV2.z)/r;

	r = V3.x*V3.x + V3.y*V3.y + V3.z*V3.z;
	esd3 = (V3.x*V3.x*EV3.x*EV3.x + V3.y*V3.y*EV3.y*EV3.y + V3.z*V3.z*EV3.z*EV3.z)/r;


	D1 = max_par * (V1.x - V2.x);
	D2 = max_par * (V1.y - V2.y);
	D3 = max_par * (V1.z - V2.z);
	DD1 = D1*D1 + D2*D2 + D3*D3;
	U1.x = D1; U1.y = D2; U1.z = D3;

	D1 = max_par * (V1.x - V3.x);
	D2 = max_par * (V1.y - V3.y);
	D3 = max_par * (V1.z - V3.z);
	DD2 = D1*D1 + D2*D2 + D3*D3;

	D1 = max_par * (V2.x - V3.x);
	D2 = max_par * (V2.y - V3.y);
	D3 = max_par * (V2.z - V3.z);
	DD3 = D1*D1 + D2*D2 + D3*D3;
	U2.x = -D1; U2.y = -D2; U2.z = -D3;

	cf = (U1.x*U2.x + U1.y*U2.y + U1.z*U2.z) / (sqrt(DD1) * sqrt(DD3));
	angle = acos(cf)*180/PI;

	esd = esd1/DD1 + (esd2*DD2)/(DD1*DD3) + esd3/DD3;
	printf("%f %f\n", esd,sqrt(esd));
	esd = sqrt(esd);
}


/****************************************************************************/
void calc_tors_esd2(VECTOR3D V1, VECTOR3D V2, VECTOR3D V3, VECTOR3D V4,
					 VECTOR3D EV1, VECTOR3D EV2, VECTOR3D EV3, VECTOR3D EV4,
				     float cell[6], float ecell[6], float max_par,
					 double &tors, double &esd)
{
	double D1,D2,D3;
	double dAB,dCD,dBC;
	double eA,eB,eC,eD;
	double cABC,sABC,cBCD,sBCD;
	XYZ U1,U2,W1,W2;
	double cT;
	double S1,S2,S3,S4,C1,C2;
	double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
	double sx1,sy1,sz1, sx2,sy2,sz2, sx3,sy3,sz3, sx4,sy4,sz4;
	double cosA,cosB,cosG,sinB,sinG;
	double cosAR,sinAR,M[3][3];
	double xx,yy,zz;

	cosA = cos(cell[3]*PI/180);
	cosB = cos(cell[4]*PI/180); sinB = sin(cell[4]*PI/180);
	cosG = cos(cell[5]*PI/180); sinG = sin(cell[5]*PI/180);
	cosAR = (cosB*cosG - cosA)/(sinB*sinG);
	sinAR = sqrt(1 - cosAR*cosAR);

	M[0][0] = 1; M[0][1] = cosG; M[0][2] = cosB;
	M[1][0] = 0; M[1][1] = sinG; M[1][2] = -sinB*cosAR;
	M[2][0] = 0; M[2][1] = 0;    M[2][2] = sinB*sinAR;

	x1 = max_par*V1.x; y1 = max_par*V1.y; z1 = max_par*V1.z;
	x2 = max_par*V2.x; y2 = max_par*V2.y; z2 = max_par*V2.z;
	x3 = max_par*V3.x; y3 = max_par*V3.y; z3 = max_par*V3.z;
	x4 = max_par*V4.x; y4 = max_par*V4.y; z4 = max_par*V4.z;

	sx1 = cell[0]*cell[0]*EV1.x*EV1.x;
	sy1 = cell[1]*cell[1]*EV1.y*EV1.y;
	sz1 = cell[2]*cell[2]*EV1.z*EV1.z;

	sx2 = cell[0]*cell[0]*EV2.x*EV2.x;
	sy2 = cell[1]*cell[1]*EV2.y*EV2.y;
	sz2 = cell[2]*cell[2]*EV2.z*EV2.z;

	sx3 = cell[0]*cell[0]*EV3.x*EV3.x;
	sy3 = cell[1]*cell[1]*EV3.y*EV3.y;
	sz3 = cell[2]*cell[2]*EV3.z*EV3.z;

	sx4 = cell[0]*cell[0]*EV4.x*EV4.x;
	sy4 = cell[1]*cell[1]*EV4.y*EV4.y;
	sz4 = cell[2]*cell[2]*EV4.z*EV4.z;

	xx = sx1*M[0][0] + sy1*M[0][1] + sz1*M[0][2];
	yy = sx1*M[1][0] + sy1*M[1][1] + sz1*M[1][2];
	zz = sx1*M[2][0] + sy1*M[2][1] + sz1*M[2][2];
	sx1 = xx; sy1 = yy = sz1 = zz;

	xx = sx2*M[0][0] + sy2*M[0][1] + sz2*M[0][2];
	yy = sx2*M[1][0] + sy2*M[1][1] + sz2*M[1][2];
	zz = sx2*M[2][0] + sy2*M[2][1] + sz2*M[2][2];
	sx2 = xx; sy2 = yy = sz2 = zz;

	xx = sx3*M[0][0] + sy3*M[0][1] + sz3*M[0][2];
	yy = sx3*M[1][0] + sy3*M[1][1] + sz3*M[1][2];
	zz = sx3*M[2][0] + sy3*M[2][1] + sz3*M[2][2];
	sx3 = xx; sy3 = yy = sz3 = zz;

	xx = sx4*M[0][0] + sy4*M[0][1] + sz4*M[0][2];
	yy = sx4*M[1][0] + sy4*M[1][1] + sz4*M[1][2];
	zz = sx4*M[2][0] + sy4*M[2][1] + sz4*M[2][2];
	sx4 = xx; sy4 = yy = sz4 = zz;

	eA = sqrt(sx1 + sy1 + sz1);
	eB = sqrt(sx2 + sy2 + sz2);
	eC = sqrt(sx3 + sy3 + sz3);
	eD = sqrt(sx4 + sy4 + sz4);

	sx1 /= eA; sy1 /= eA; sz1 /= eA;
	sx2 /= eB; sy2 /= eB; sz2 /= eB;
	sx3 /= eC; sy3 /= eC; sz3 /= eC;
	sx4 /= eD; sy4 /= eD; sz4 /= eD;

	eA = (sx1 + sy1 + sz1)/3;
	eB = (sx2 + sy2 + sz2)/3;
	eC = (sx3 + sy3 + sz3)/3;
	eD = (sx4 + sy4 + sz4)/3;

	D1 = max_par * (V1.x - V2.x);
	D2 = max_par * (V1.y - V2.y);
	D3 = max_par * (V1.z - V2.z);
	dAB = D1*D1 + D2*D2 + D3*D3;

	D1 = max_par * (V2.x - V3.x);
	D2 = max_par * (V2.y - V3.y);
	D3 = max_par * (V2.z - V3.z);
	dBC = D1*D1 + D2*D2 + D3*D3;

	D1 = max_par * (V3.x - V4.x);
	D2 = max_par * (V3.y - V4.y);
	D3 = max_par * (V3.z - V4.z);
	dCD = D1*D1 + D2*D2 + D3*D3;

	U1.x = max_par * (V1.x - V2.x);
	U1.y = max_par * (V1.y - V2.y);
	U1.z = max_par * (V1.z - V2.z);
	U2.x = max_par * (V3.x - V2.x);
	U2.y = max_par * (V3.y - V2.y);
	U2.z = max_par * (V3.z - V2.z);
	W1.x = U1.y*U2.z - U1.z*U2.y;
	W1.y = U1.z*U2.x - U1.x*U2.z;
	W1.z = U1.x*U2.y - U1.y*U2.x;
	cABC = (U1.x*U2.x + U1.y*U2.y + U1.z*U2.z)/
		   (sqrt(U1.x*U1.x + U1.y*U1.y + U1.z*U1.z)*sqrt(U2.x*U2.x + U2.y*U2.y + U2.z*U2.z));
	sABC = sqrt(1 - cABC);


	U1.x = max_par * (V2.x - V3.x);
	U1.y = max_par * (V2.y - V3.y);
	U1.z = max_par * (V2.z - V3.z);
	U2.x = max_par * (V4.x - V3.x);
	U2.y = max_par * (V4.y - V3.y);
	U2.z = max_par * (V4.z - V3.z);
	W2.x = U1.y*U2.z - U1.z*U2.y;
	W2.y = U1.z*U2.x - U1.x*U2.z;
	W2.z = U1.x*U2.y - U1.y*U2.x;
	cBCD = (U1.x*U2.x + U1.y*U2.y + U1.z*U2.z)/
		   (sqrt(U1.x*U1.x + U1.y*U1.y + U1.z*U1.z)*sqrt(U2.x*U2.x + U2.y*U2.y + U2.z*U2.z));
	sBCD = sqrt(1 - cBCD);

	cT = (W1.x*W2.x + W1.y*W2.y + W1.z*W2.z) / 
         (sqrt(W1.x*W1.x + W1.y*W1.y + W1.z*W1.z)*sqrt(W2.x*W2.x + W2.y*W2.y + W2.z*W2.z));
	tors = acos(cT)*180/PI;

	C1 = (dBC - dAB*cABC)/(dAB*sABC);
	C2 = (dBC - dCD*cBCD)/(dCD*sBCD);

	S1 = eA/(dAB*sABC*sABC);
	S2 = eD/(dCD*sBCD*sBCD);
	S3 = eB*(cBCD*cBCD + C1*C1 - 2*cT*cBCD*C1)/dBC;
	S4 = eC*(cABC*cABC + C2*C2 - 2*cT*cABC*C2)/dBC;

	esd = S1 + S2 + S3 + S4;
	esd = sqrt(esd);
}



