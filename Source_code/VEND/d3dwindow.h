int open3Dwindow(int ID);
void close3Dwindow(int ID);
void set_3Dpalette(short Pl[100][6]);
void map_H00_3D(int mode, float range[2]);
void map_HKL_3D(int hkl[3], float orig);
void D3D_fill_H00_Cell(int mode, int palette, float iLevel, float Level, float fRGB[3], float width);

void D3D_reshape(int wid, int ht);
void D3D_RenderScene( void );



