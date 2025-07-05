void set_slices_dialog_param(int Num, float opacity, float position[3],short hkl[3][3],short index_show_hkl[3],
							 int palette_hkl,float scale1, float scale2);
void set_sli_dialog_id(int ID);

void open_slices_dialog(int ID, int dpX, int dpY);
int get_sli_dialog_id();
int get_sli_preview_id();
int get_sli_mode_id();
void get_sli_cur_param(float &position, short hkl[3], float &opacity,int &palette_hkl, float &scal1, float &scale2);
void get_sli_param(int &Num, float position[3], short hkl[3][3], float &opacity,int &palette_hkl, float &scale1, float &scale2);
void get_sli_show_param(short show_hkl[3]);

void get_sli_dialog_pos(int &x, int &y);




