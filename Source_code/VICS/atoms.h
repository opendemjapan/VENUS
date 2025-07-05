void set_atom_prop_main_params(int Num, int Stack, int Slice,int Type, int Outline, int Prob,int check_aniso);
void set_atom_prop_params(int ID, char Symb[3], float Rad, float RGB[3]);

void open_atoms_prop_dialog(int ID, int dpX, int dpY);

void set_atom_prop_dialog_id(int ID);
int get_atom_prop_dialog_id();
int get_atom_prop_preview_id();
void get_atom_prop_main_params(int &Stack, int &Slice, int &Type, int &Outline, int &Prob);
void get_type_R_RGB(int ID, float &R, float color[3]);

void get_atom_colors(float RGB[3]);
void set_atom_colors(float RGB[3]);
void get_atom_dialog_pos(int &x, int &y);
