void calc_D2_esd2(VECTOR3D V1, VECTOR3D V2, VECTOR3D EV1, VECTOR3D EV2,
				  float cell[6], float ecell[6], float max_par,
			 	  double &D, double &esd);

void calc_angle_esd2(VECTOR3D V1, VECTOR3D V2, VECTOR3D V3,
					 VECTOR3D EV1, VECTOR3D EV2, VECTOR3D EV3,
				     float cell[6], float ecell[6], float max_par,
					 double &angle, double &esd);


void calc_tors_esd2(VECTOR3D V1, VECTOR3D V2, VECTOR3D V3, VECTOR3D V4,
					 VECTOR3D EV1, VECTOR3D EV2, VECTOR3D EV3, VECTOR3D EV4,
				     float cell[6], float ecell[6], float max_par,
					 double &tors, double &esd);


