
void initial_new_edit_file_settings();
void set_new_edit_file_dialog_param(char *sTitle, int SpGrNum, int SpGrSet, 
									float CellPar[6], float CellEsd[6], 
									int numOfAtom, int thermal_mode, int use_thermal,
									float plot_range[6], char *path_to_prog,
									int type_iso);
void set_struc_param(int ID, char *name, char *symb, float g, float x, float y, float z, 
                     float ex, float ey, float ez, float UTP[6], float BISO);
void set_new_edit_dialog_id(int ID);


void open_new_edit_file_dialog(int ID, int Mode, int dpX, int dpY);

int get_new_edit_dialog_id();
void get_new_edit_file_dialog_param(char *sTitle, int &SpGrNum, int &SpGrSet, char *SpGrName, 
									float CellPar[6], float CellEsd[6], 
									int &numOfAtom, int &thermal_mode, int &use_thermal, 
									float plot_range[6],int &type_iso);
void get_struc_param(int ID, char *name, char *symb, float &g, float &x, float &y, float &z, 
                     float &ex, float &ey, float &ez, float ATP[6], float &BISO);


void get_cur_origin(int &num, int &ori);
void set_cur_origin(int ori);
void get_edit_dialog_pos(int &x, int &y);






