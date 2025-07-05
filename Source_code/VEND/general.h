void set_general_param(float l_a[4],   float l_d[4], float l_dir[16], 
					   float RGB1[3], float RGB2[3], float RGB3[3], int comp,
					   int proj_type, float persp);
void open_general_dialog(int ID, int dpX, int dpY);

int get_genral_control_id();
void set_genral_control_id(int ID);

void get_lights_rendering(float l_a[4], float l_d[4], float l_dir[16]);
void get_background_rendering(float RGB1[3],float RGB2[3],float RGB3[3]);
int get_compas_setting();
void get_projection_perspective(int &proj_type, float &persp);

void get_genral_colors(float RGB[3]);
void set_genral_colors(float RGB[3]);
void get_general_dialog_pos(int &x, int &y);




