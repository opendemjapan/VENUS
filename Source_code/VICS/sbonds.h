
void init_bonds_dialog(float mol,int numAtom,int numBond,int hb_id);
void set_bonds_to_bonds_dialog(int ID, char Symb1[2],char Symb2[2], float d, int search_mode_ID,
							   int bound_mode_ID, int poly_mode_ID);
void set_atoms_to_bonds_dialog(int ID, char Symb[2]);
void open_bonds_dialog(int ID, int dpX, int dpY);

int get_bond_dialog_id();
int get_new_number_bond_spec();
void get_new_bond_spec(int ID, char Symb1[2], char Symb2[2], float &d, int &smode, int &bmode, int &pmode);
int get_h_bond_id();
void get_sbonds_dialog_pos(int &x, int &y);


