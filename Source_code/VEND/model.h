
void set_model_prop_params(int model, int Stack1, int Slice1, int Stack2, int Slice2,  
						   float bColor[3],float bR1,float bR2,int Num,int tbond);
void set_model_atom_params(int ID, char *symb, float RGB[3], float R);


void open_model_prop_dialog(int ID, int dpX, int dpY);
int get_model_proper_dialog_id();
void set_model_proper_dialog_id(int ID);
void get_model_atom_colors(float RGB[3]);
void set_model_atom_colors(float RGB[3]);
void get_model_bond_colors(float RGB[3]);
void set_model_bond_colors(float RGB[3]);

void get_model_prop_params(int &model, int &Stack1, int &Slice1, int &Stack2, int &Slice2,  
						   float bColor[3],float &bR1,float &bR2,int &tbond);
void get_model_atom_params(int ID, float RGB[3], float &R);

void get_model_dialog_pos(int &x, int &y);







