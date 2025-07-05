#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#include <direct.h>


int open_file_dialog(int &format, char *inp_file)
{

	char buffer[_MAX_PATH];
	OPENFILENAME lpofn;
	BOOL ID;
	char dlg_title[20];

	static char *szOpenFilter = "VICS (*.vcs)\0*.vcs\0"
								"AMCSD (*.amc)\0*.amc\0"
								"Chem3D (*.cc1; *.cc2)\0*.cc1;*.cc2\0"
								"CIF (*.cif)\0*.cif\0"
								"CrystalMaker text file (*.cmt)\0*.cmt\0"
								"CRYSTIN (*.cry)\0*.cry\0"
								"CSSR (*.cssr; *.css)\0*.cssr;*.css\0"
								"FDAT & CSD (*.fdt; *.csd)\0*.fdt;*.csd\0"
								"Gaussian 98 Cube (*.cube; *.cub)\0*.cube;*.cub\0"
								"ICSD (*.ics)\0*.ics\0"
								"MDL molfile (*.mol)\0*.mol\0"
								"MINCRYST (*.min)\0*.min\0"
								"MOLDA (*.mld)\0*.mld\0"
								"PDB (*.pdb)\0*.pdb\0"
		                        "RIETAN (*.ins)\0*.ins\0"
								"WIEN2k (*.struct)\0*.struct\0"
								"XMol xyz (*.xyz)\0*.xyz\0"
								"ASSE (*.asse)\0*.asse\0"
								"SCAT f01\0f01\0"
								"MXDORTO file07.dat\0file07.dat\0"
                                "\0";
	sprintf(inp_file,"%s", " ");
	format++;

	sprintf(dlg_title,"Open");

	lpofn.lStructSize = sizeof(OPENFILENAME);
	lpofn.hwndOwner = NULL;
	lpofn.hInstance = NULL;
    lpofn.lpstrCustomFilter = NULL;
	lpofn.nMaxCustFilter = 0;
	lpofn.lpstrFilter = szOpenFilter;
	lpofn.nFilterIndex = format;
	lpofn.lpstrFile = inp_file;
	lpofn.nMaxFile = 80;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = NULL;
	lpofn.lpstrInitialDir = NULL;
	lpofn.lpstrTitle = dlg_title;
	lpofn.Flags = NULL;
	lpofn.nFileOffset = NULL;
	lpofn.nFileExtension = NULL;
	lpofn.lpstrDefExt = NULL;
	lpofn.lCustData = NULL;
	lpofn.lpfnHook = NULL;
	lpofn.lpTemplateName = NULL;

	_getcwd(buffer,_MAX_PATH);
	ID = GetOpenFileName((LPOPENFILENAME) &lpofn);
	_chdir(buffer);
	format = lpofn.nFilterIndex - 1;
	
	return(ID);
}



int save_file_dialog(int &format, char *inp_file, int mode)
{

	char buffer[_MAX_PATH];
	OPENFILENAME lpofn;
	BOOL ID;
	char dlg_title[20];

	static char *szSaveFilter1 = "VICS (*.vcs)\0*.vcs\0"
                                 "\0";

								
	static char *szSaveFilter2 = "CIF (*.cif)\0*.cif\0"
		                         "PDB (*.pdb)\0*.pdb\0"
				                 "RIETAN (*.ins)\0*.ins\0"
		                         "XMol xyz (*.xyz)\0*.xyz\0"
                                 "\0";

	static char *szSaveFilter3 = "BMP (*.bmp)\0*.bmp\0"
		                         "EPS (*.eps)\0*.eps\0"
								 "JPEG (*.jpg)\0*.jpg\0"
								 "JPEG 2000 (*.jp2)\0*.jp2\0"
				                 "PPM (*.ppm)\0*.ppm\0"
		                         "RAW (*.raw)\0*.raw\0"
		                         "RGB (*.rgb)\0*.rgb\0"
		                         "TGA (*.tga)\0*.tga\0"
		                         "TIFF (*.tif)\0*.tif\0"
                                 "\0";

	static char *szSaveFilter4 = "RIETAN (*.ins)\0*.ins\0"
                                 "\0";
		                        

	sprintf(inp_file,"%s", " ");
	format++;
	sprintf(dlg_title,"");

	switch(mode)
	{
//-------------------------------------------------- Save VICS file
	case(0):{
		sprintf(dlg_title,"Save");
		lpofn.lpstrFilter = szSaveFilter1;
		break;
			}
//-------------------------------------------------- Export to MDL, RIETAN, XMol...
	case(1):{
		sprintf(dlg_title,"Export");
		lpofn.lpstrFilter = szSaveFilter2;
		break;
			}
//-------------------------------------------------- Export Image
	case(2):{
		sprintf(dlg_title,"Export image");
		lpofn.lpstrFilter = szSaveFilter3;
		break;
			}
//-------------------------------------------------- Template for RIETAN
	case(3):{
		sprintf(dlg_title,"Open");
		lpofn.lpstrFilter = szSaveFilter4;
		break;
			}
	}

	lpofn.lStructSize = sizeof(OPENFILENAME);
	lpofn.hwndOwner = NULL;
	lpofn.hInstance = NULL;
    lpofn.lpstrCustomFilter = NULL;
	lpofn.nMaxCustFilter = 0;
	lpofn.nFilterIndex = format;
	lpofn.lpstrFile = inp_file;
	lpofn.nMaxFile = 80;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = NULL;
	lpofn.lpstrInitialDir = NULL;
	lpofn.lpstrTitle = dlg_title;
	lpofn.Flags = NULL;
	lpofn.nFileOffset = NULL;
	lpofn.nFileExtension = NULL;
	lpofn.lpstrDefExt = NULL;
	lpofn.lCustData = NULL;
	lpofn.lpfnHook = NULL;
	lpofn.lpTemplateName = NULL;

	_getcwd(buffer,_MAX_PATH);
	ID = GetSaveFileName((LPOPENFILENAME) &lpofn);
	_chdir(buffer);
	format = lpofn.nFilterIndex - 1;
	
	return(ID);
}




int open_templ_dialog(int mode, char *inp_file)
{

	char buffer[_MAX_PATH];
	OPENFILENAME lpofn;
	BOOL ID;
	char dlg_title[20];

	static char *szOpenFilter = "RIETAN (*.ins)\0*.ins\0"
                                "\0";
	sprintf(inp_file,"%s", " ");
	sprintf(dlg_title,"Open");

	lpofn.lStructSize = sizeof(OPENFILENAME);
	lpofn.hwndOwner = NULL;
	lpofn.hInstance = NULL;
    lpofn.lpstrCustomFilter = NULL;
	lpofn.nMaxCustFilter = 0;
	lpofn.lpstrFilter = szOpenFilter;
	lpofn.nFilterIndex = 1;
	lpofn.lpstrFile = inp_file;
	lpofn.nMaxFile = 80;
	lpofn.lpstrFileTitle = NULL;
	lpofn.nMaxFileTitle = NULL;
	lpofn.lpstrInitialDir = NULL;
	lpofn.lpstrTitle = dlg_title;
	lpofn.Flags = NULL;
	lpofn.nFileOffset = NULL;
	lpofn.nFileExtension = NULL;
	lpofn.lpstrDefExt = NULL;
	lpofn.lCustData = NULL;
	lpofn.lpfnHook = NULL;
	lpofn.lpTemplateName = NULL;

	_getcwd(buffer,_MAX_PATH);
	ID = GetOpenFileName((LPOPENFILENAME) &lpofn);
	_chdir(buffer);
	
	return(ID);
}

