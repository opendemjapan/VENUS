void set_preferences_def_param(int a_St_s, int a_Sl_s, int a_St_o, int a_Sl_o,
							   int b_St_s, int b_Sl_s, int b_St_o, int b_Sl_o,
							   float u_w, float p_w, float h_w, float b_w,
							   float p_t, float h_t, float bkg_c[3], float u_c[3], 
							   float p_c[3], float h_c[3], float b_c[3],
							   float b_r[2], int comp, float stw_step, float bond, 
							   int startup_S, int format_ID, char *export_fileN, int pos, int dpos);
void set_preferences_cur_param(int a_St_s, int a_Sl_s, int a_St_o, int a_Sl_o,
							   int b_St_s, int b_Sl_s, int b_St_o, int b_Sl_o,
							   float u_w, float p_w, float h_w, float b_w,
							   float p_t, float h_t,
							   float bkg_c[3], float u_c[3], float p_c[3], float h_c[3], float b_c[3],
							   float b_r[2], int comp);

void open_preferences_dialog(int ID, int dpX, int dpY);

int get_preferences_dialog_id();
void set_preferences_dialog_id(int ID);
void get_preferences_cur_param(int &a_St_s, int &a_Sl_s, int &a_St_o, int &a_Sl_o,
							   int &b_St_s, int &b_Sl_s, int &b_St_o, int &b_Sl_o,
							   float &u_w, float &p_w, float &h_w, float &b_w,
							   float &p_t, float &h_t,
							   float &bkg_c_R, float &bkg_c_G, float &bkg_c_B,
							   float &u_c_R, float &u_c_G, float &u_c_B,
							   float &p_c_R, float &p_c_G, float &p_c_B,
							   float &h_c_R, float &h_c_G, float &h_c_B,
							   float &b_c_R, float &b_c_G, float &b_c_B,
							   float &b_r_1, float &b_r_2, int &comp, float &stw_step, 
							   float &bond, int &startup_S,int &format_ID, 
							   char *export_fileN, int &pos, int &dpos);
void get_preferences_def_param(int &a_St_s, int &a_Sl_s, int &a_St_o, int &a_Sl_o,
							   int &b_St_s, int &b_Sl_s, int &b_St_o, int &b_Sl_o,
							   float &u_w, float &p_w, float &h_w, float &b_w,
							   float &p_t, float &h_t,
							   float &bkg_c_R, float &bkg_c_G, float &bkg_c_B,
							   float &u_c_R, float &u_c_G, float &u_c_B,
							   float &p_c_R, float &p_c_G, float &p_c_B,
							   float &h_c_R, float &h_c_G, float &h_c_B,
							   float &b_c_R, float &b_c_G, float &b_c_B,
							   float &b_r_1, float &b_r_2, int &comp, float &stw_step, 
							   float &bond, int &startup_S,int &format_ID, 
							   char *export_fileN, int &pos, int &dpos);
void get_prefer_dialog_pos(int &x, int &y);

