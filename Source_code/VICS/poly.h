void set_poly_prop(int Model, float Opacity, float lineW, float lineRGB[3], int ShowLine);
void open_poly_prop_dialog(int ID, int dpX, int dpY);

int get_poly_prop_dialog_id();
void set_poly_prop_dialog_id(int ID);
int get_poly_prop_preview_id();

float get_poly_prop_colors_R();
float get_poly_prop_colors_G();
float get_poly_prop_colors_B();
void get_poly_prop_main_param(int &Model, float &Opacity, float &lineW, int &ShowLine);

void set_poly_colors(float RGB[3]);
void get_poly_colors(float RGB[3]);

void set_poly_type(int type);
int get_poly_type();
void get_poly_dialog_pos(int &x, int &y);





