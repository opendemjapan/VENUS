#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#include <direct.h>


int open_file_dialog(int &format, char *inp_file, int mode)
{

	char buffer[_MAX_PATH];
	OPENFILENAME lpofn;
	BOOL ID;
	char dlg_title[20];

	static char *szOpenFilter1 ="MEM (*.den)\0*.den\0"
		                        "Gaussian 98 (*.cube; *.cub)\0*.cube;*.cub\0"
		                        "SCAT (*.scat)\0*.scat\0"
		                        "WIEN2k (*.rho)\0*.rho\0"
		                        "VEND (*.vnd)\0*.vnd\0"
		                        "VEND 3D (*.*ed)\0*.*ed\0"
								"Energy band (*.eb)\0*.eb\0"
                                "\0";
	static char *szOpenFilter2 = "VICS (*.vcs)\0*.vcs\0"
                                "\0";


	switch(mode)
	{
//-------------------------------------------- Open 3D data file
	case(0):{
		sprintf(dlg_title,"");
		sprintf(dlg_title,"Open");
		lpofn.lpstrFilter = szOpenFilter1;
		break;
			}
//-------------------------------------------- Open structure model file (VICS)
	case(1):{
		sprintf(dlg_title,"");
		sprintf(dlg_title,"Import a VICS File");
		lpofn.lpstrFilter = szOpenFilter2;
		break;
			}
	}

	sprintf(inp_file, "");
	format++;

	lpofn.lStructSize = sizeof(OPENFILENAME);
	lpofn.hwndOwner = NULL;
	lpofn.hInstance = NULL;
    lpofn.lpstrCustomFilter = NULL;
	lpofn.nMaxCustFilter = 0;
	lpofn.nFilterIndex = format;  //2;
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

	static char *szSaveFilter1 = "VEND (*.vnd)\0*.vnd\0"
                                 "\0";

	static char *szSaveFilter2 = "BMP (*.bmp)\0*.bmp\0"
		                         "EPS (*.eps)\0*.eps\0"
								 "JPEG (*.jpg)\0*.jpg\0"
								 "JPEG 2000 (*.jp2)\0*.jp2\0"
				                 "PPM (*.ppm)\0*.ppm\0"
		                         "RAW (*.raw)\0*.raw\0"
		                         "RGB (*.rgb)\0*.raw\0"
		                         "TGA (*.tga)\0*.tga\0"
		                         "TIFF (*.tif)\0*.tif\0"
                                 "\0";

	sprintf(inp_file,"%s", " ");
	format++;
	sprintf(dlg_title,"");

	switch(mode)
	{
//-------------------------------------------------- Save VEND file
	case(0):{
		sprintf(dlg_title,"Save");
		lpofn.lpstrFilter = szSaveFilter1;
		break;
			}
//-------------------------------------------------- Export Image
	case(1):{
		sprintf(dlg_title,"Export image");
		lpofn.lpstrFilter = szSaveFilter2;
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
