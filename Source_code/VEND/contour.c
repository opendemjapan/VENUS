#include <stdio.h>
#include <math.h>
#include "contour.h"

int _get_sh(double h)
{
	if (h < 0.0) return -1;
	if (h == 0.0) return 0;
	if (h > 0.0) return 1;

	return 0;
}


int conrec(float d[4], float v[4][2], float Z)
{

int m,m1,m2,m3,case_value;
	double h[5],xh[5],yh[5];
	int sh[5];
	double x1,y1,x2,y2;
	int ID;

int castab[3][3][3] = 
{
	{
		{0,0,8},{0,2,5},{7,6,9}
	},
	{
		{0,3,4},{1,0,1},{4,3,0}
	},
	{
		{9,6,7},{5,2,0},{8,0,0}
	}
};


#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])

	nContLine = 0;
	for (m=1; m<5; m++) {
		h[m]  = d[m-1] - Z;
		xh[m] = v[m-1][0];
		yh[m] = v[m-1][1];
		sh[m] = _get_sh(h[m]);
	}
	h[0] = 0.25*(h[1]+h[2]+h[3]+h[4]);
	xh[0]=0.5*(v[0][0] + v[1][0]);
	yh[0]=0.5*(v[0][1] + v[3][1]);
	sh[0] = _get_sh(h[0]);
	for (m=1;m<=4;m++) {
		ID = 0;
		m1 = m;
		m2 = 0;
		if (m != 4) m3 = m + 1;
		else m3 = 1;
		case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1];
		switch (case_value) {
		case 1:{ //     Case 1 - Line between vertices 1 and 2
			x1 = xh[m1]; y1 = yh[m1];
			x2 = xh[m2]; y2 = yh[m2];
			ID = 1;
			break;
			   }
		case 2:{//     Case 2 - Line between vertices 2 and 3
			x1 = xh[m2]; y1 = yh[m2];
			x2 = xh[m3]; y2 = yh[m3];
			ID = 1;
			break;
			   }
		case 3:{//     Case 3 - Line between vertices 3 and 1
			x1 = xh[m3]; y1 = yh[m3];
			x2 = xh[m1]; y2 = yh[m1];
			ID = 1;
			break;
			   }
		case 4:{//     Case 4 - Line between vertex 1 and side 2-3
			x1 = xh[m1]; y1 = yh[m1];
			x2 = xsect(m2,m3); y2 = ysect(m2,m3);
			ID = 1;
			break;
			   }
		case 5:{//     Case 5 - Line between vertex 2 and side 3-1
			x1 = xh[m2]; y1 = yh[m2];
			x2 = xsect(m3,m1); y2 = ysect(m3,m1);
			ID = 1;
			break;
			   }
		case 6:{//     Case 6 - Line between vertex 3 and side 1-2
			x1 = xh[m3]; y1 = yh[m3];
			x2 = xsect(m1,m2); y2 = ysect(m1,m2);
			ID = 1;
			break;
			   }
		case 7:{//     Case 7 - Line between sides 1-2 and 2-3
			x1 = xsect(m1,m2); y1 = ysect(m1,m2);
			x2 = xsect(m2,m3); y2 = ysect(m2,m3);
			ID = 1;
			break;
			   }
		case 8:{//     Case 8 - Line between sides 2-3 and 3-1
			x1 = xsect(m2,m3); y1 = ysect(m2,m3);
			x2 = xsect(m3,m1); y2 = ysect(m3,m1);
			ID = 1;
			break;
			   }
		case 9:{//     Case 9 - Line between sides 3-1 and 1-2
			x1 = xsect(m3,m1); y1 = ysect(m3,m1);
			x2 = xsect(m1,m2); y2 = ysect(m1,m2);
			ID = 1;
			break;
			   }
		default:{break;}
		}
		if (ID == 1){
			ContLine[nContLine].P1[0] = (float)x1;
			ContLine[nContLine].P1[1] = (float)y1;
			ContLine[nContLine].P2[0] = (float)x2;
			ContLine[nContLine].P2[1] = (float)y2;
			nContLine++;
		}
	}

	return(nContLine);
}

