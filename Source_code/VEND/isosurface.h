void set_isosurface_param(float dmin, float dmax, float org[3], float level,float scale, 
						  int range_ID, int boxes[3], float range[6], 
						  float amb[4], float dif[4], float spec[4], float emis[4], 
						  float shin, int isot, int iso_ID);
void open_isosurface_dialog(int ID, int dpX, int dpY);

void set_isosurface_dialog_id(int ID);
int get_isosurface_dialog_id();
int get_isosurface_preview_id();
void get_isosurface_material_param(float amb[4], float dif[4], float spec[4], float emis[4], float &shin);
void get_isosurface_plot_range(float org[3], int &range_ID, int boxes[3], float range[6],float &level,float &scale);

void get_isosurface_colors(float RGB[3]);
void set_isosurface_colors(float RGB[3]);
int get_isosurface_pm_type();
void get_iso_dialog_pos(int &x, int &y);



