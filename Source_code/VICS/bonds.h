void set_bond_prop_params(int Mode, int Stack, int Slice, float Rad[2], float lWidth, float RGB[3],
						  int aModel, int hMode);
void open_bonds_prop_dialog(int ID, int dpX, int dpY);

int get_bond_prop_dialog_id();
void set_bond_prop_dialog_id(int ID);
int get_bond_prop_preview_id();
void get_bond_main_prop_params(int &Mode, int &Stack, int &Slice, float &lWidth, int &hMode);
float get_bond_prop_radius_B_S();
float get_bond_prop_radius_Stick();
float get_bond_prop_colors_R();
float get_bond_prop_colors_G();
float get_bond_prop_colors_B();

void set_bond_colors(float RGB[3]);
void get_bond_colors(float RGB[3]);
void get_bonds_dialog_pos(int &x, int &y);


