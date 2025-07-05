//void set_prj_dialog_settings(int type, int dim, int pal, int cont, int show2D, int show3D,
//							 float RGB[3], float width);
void set_prj_dialog_settings(int type, int dim, int pal, int cont, int show2D, int show3D,
							 float RGB[3], float width, int hkl[3], int showGrid);


void set_prj_dialog_param(float projection[2],float scale1,float scale2, float orig);

void open_prj_dialog(int ID, int dpX, int dpY);
int get_prj_dialog_id();
void set_prj_dialog_id(int ID);

//void get_prj_dialog_settings(int &type, int &dim, int &pal, int &cont, int &show2D, int &show3D,
//							 float RGB[3], float &width);
void get_prj_dialog_settings(int &type, int &dim, int &pal, int &cont, int &show2D, int &show3D,
							 float RGB[3], float &width, int hkl[3], int &showGrid);

void get_prj_dialog_param(float projection[2], float &scale1, float &scale2, float &orig);

void get_prj_colors(float RGB[3]);
void set_prj_colors(float RGB[3]);
void get_prj_dialog_pos(int &x, int &y);






