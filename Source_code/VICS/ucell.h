void set_ucell_prop_dialog(int show_ID, float width, float color[3], int type);

void open_ucell_prop_dialog(int ID, int dpX, int dpY);

int get_ucell_prop_dialog_id();
void get_ucell_prop(int &show_ID, float &width, float &R, float &G, float &B, int &type);

void set_ucell_prop_dialog_id(int ID);
void set_ucell_colors(float RGB[3]);
void get_ucell_colors(float RGB[3]);
void get_ucell_dialog_pos(int &x, int &y);
