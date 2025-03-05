#include "Dataplotter.h"

										// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle

LRESULT CALLBACK DataPlotter(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	static HWND hWndListView;

	switch (message)
	{   case WM_INITDIALOG:
		{   SHMENUBARINFO mbi;				// create menubar for loanform dialog.
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_DATAPLOTTER_MENU;
			mbi.hInstRes   = hInst;
			mbi.nBmpId     = 0;
			mbi.cBmpImages = 0;	
        
			if (!SHCreateMenuBar(&mbi))
				MessageBox(hDlg, L"SHCreateMenuBar Failed", L"Error", MB_OK);
			hwndCB = mbi.hwndMB;

		 	SHINITDLGINFO shidi;
			shidi.dwMask = SHIDIM_FLAGS;         // create a DONE button and size it.
			shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hDlg;
			SHInitDialog(&shidi);   //initialzes the dialog based on the dwFlags parameter
		}

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

		    switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_DATAPLOTTER_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;
			}
			break;

		
		default:
			break;
	}
	return FALSE;
}
