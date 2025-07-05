void set_contour_dialog(int stype, float cMAX[2], float cMIN[2], int cNUM[2], float cSTP,
						float RGB[3], float width, int show_only);

void open_contour_dialog(int ID, int dpX, int dpY);
void get_contour_dialog(int &stype, float cMAX[2], float cMIN[2], int cNUM[2], float &cSTP, 
						float RGB[3], float &width, int &show_only);

int get_contour_dialog_id();
void set_contour_dialog_id(int ID);

void get_contour_colors(float RGB[3]);
void set_contour_colors(float RGB[3]);

void get_contour_dialog_pos(int &x, int &y);



