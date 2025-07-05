void preferency_set_def_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float width, 
							  int comp, float stw_step, int format_ID, int pos, int dpos);
void preferency_set_cur_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float width, int comp);

void open_preferences_dialog(int ID, int dpX, int dpY);

int get_preferences_dialog_id();
void set_preferences_dialog_id(int ID);
void preferency_get_def_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float &width, 
							  int &comp, float &stw_step, int &format_ID, int &pos, int &dpos);
void preferency_get_cur_param(float bkg1[3],float bkg2[3],float bkg3[3],float cell[3], float &width, 
							  int &comp, float &stw_step, int &format_ID, int &pos, int &dpos);

void get_pref_dialog_pos(int &x, int &y);

