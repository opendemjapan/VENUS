int open2Dwindow(int ID);
void close2Dwindow(int ID);
void set_palette(short Pl[100][6]);
void draw_H00_Cell(int mode,int type, float RGB[3], float width);
void D2D_Color_Bar(int palette, float range, float bRGB[3], float width);
void map_H00_Cell(int mode, float range[2]);
void map_HKL_Cell(int hkl[3], float orig);
void D2D_fill_H00_Cell(int mode, int type, int palette, float iLevel);
void map_H00_Contour(int mode, int type, int stype, float zMAX[2], float zMIN[2], int NSTP[2],
					 float Step, float cColor[3], float cW);

void D2D_fill_Grid(int mode, int type, float bRGB[3]);


float get_MAX_den();
float get_MIN_den();

void clear_contor_list();
void clear_color_bar();
void clear_map_cell();
void clear_grid_cell();


void D2D_RenderScene( void );
void D2D_reshape(int wid, int ht);





