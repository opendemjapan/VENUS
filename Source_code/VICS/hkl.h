void set_hkl_prop_dialog(int ID, int orig, int inv_hkl, int out_hkl, int HKL[3], float RGB[3], float trans, float oW);
void set_cell_param(float param[6]);
void open_hkl_prop_dialog(int ID, int dpX, int dpY);

int get_hkl_prop_dialog_id();
void set_hkl_prop_dialog_id(int ID);
int get_hkl_prop_preview_id();
void get_hkl_prop_main_param(int &ID, int &orig, int &inv_hkl, int &out_hkl, float &oW);
void get_hkl_prop_colors(float &R, float &G, float &B, float &T);
void get_hkl_prop_index(int &h, int &k, int &l);

void set_hkl_colors(float RGB[3]);
void get_hkl_colors(float RGB[3]);
void get_hkl_dialog_pos(int &x, int &y);






