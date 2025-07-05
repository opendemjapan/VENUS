
void open_rendering_dialog(int ID, int dpX, int dpY);
void init_rendering_dialog(float m_a_a[4], float m_a_d[4], float m_a_s[4], float m_a_sh,
						   float m_b_a[4], float m_b_d[4], float m_b_s[4], float m_b_sh,
						   float m_p_a[4], float m_p_d[4], float m_p_s[4], float m_p_sh,
						   float m_h_a[4], float m_h_d[4], float m_h_s[4], float m_h_sh,
						   float l_a[4],   float l_d[4], float l_dir[16], float RGB[3], int comp,
						   int proj_type, float persp, int r_type, float fogi);
int get_render_control_id();
void set_render_control_id(int ID);
void get_atom_rendering(float m_a_a[4], float m_a_d[4], float m_a_s[4], float &m_a_sh);
void get_bond_rendering(float m_b_a[4], float m_b_d[4], float m_b_s[4], float &m_b_sh);
void get_poly_rendering(float m_p_a[4], float m_p_d[4], float m_p_s[4], float &m_p_sh);
void get_plane_rendering(float m_h_a[4], float m_h_d[4], float m_h_s[4], float &m_h_sh);
void get_lights_rendering(float l_a[4], float l_d[4], float l_dir[16]);
void get_background_rendering(float &R, float &G, float &B);
int get_compass_rendering();
int get_radii_rendering();
float get_fog_rendering();
void get_projection_perspective(int &proj_type, float &persp);

void set_bkg_colors(float RGB[3]);
void get_bkg_colors(float RGB[3]);
void get_general_dialog_pos(int &x, int &y);


