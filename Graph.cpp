// Graph.cpp : Defines the entry point for the application.
//db

#include "Graph.h"

#define MAX_LOADSTRING	100
#define GRAYWIDTH		30
#define PALETTEINDEX2BPP(i)  (((i)==0)?0x00000000:(((i)==1)?0x00808080:(((i)==2)?0x00C0C0C0:0x00FFFFFF)))

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle
HWND                myDialog;
int					nCmdBarHeight; // Height of the commandbar

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE hInstance, LPTSTR szWindowClass);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	BasicDlgProc    (HWND, UINT, WPARAM, LPARAM);


void drawMainWindow(HWND hWnd);


int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GRAPH);
	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;
	
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPH));
	wc.hCursor			= 0;
	wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= szWindowClass;
	
	return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name
	
	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_GRAPH, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);
	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	
	////////// Setting default main window size
	// This technique allows for you to create the main
	// window to allow for the postion of a menubar and/or
	// the SIP button at the bottom of the screen
	
	SIPINFO si = {0};
	//Set default window creation sizd info
	int iDelta, x = CW_USEDEFAULT, y = CW_USEDEFAULT, cx, cy;
	
	si.cbSize = sizeof(si);
	SHSipInfo(SPI_GETSIPINFO, 0, &si, 0);
		
	//Consider the menu at the bottom, please.
	iDelta = (si.fdwFlags & SIPF_ON) ? 0 : MENU_HEIGHT;
	cx = si.rcVisibleDesktop.right - si.rcVisibleDesktop.left;
	cy = si.rcVisibleDesktop.bottom - si.rcVisibleDesktop.top - iDelta;
		

	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE | WS_BORDER,
		x, y, cx, cy, NULL, NULL, hInstance, NULL);
	
	if (!hWnd)
	{	
		return FALSE;
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	drawMainWindow(hWnd);
	return TRUE;
}


void drawMainWindow(HWND hWnd)
{	ui_clearWindow(hWnd);
	ui_drawBitmap(hWnd, IDB_CALCICON, 35, 20);
	ui_drawBitmap(hWnd, IDB_CONVERSION, 35, 80);
	ui_drawBitmap(hWnd, IDB_LOANICON, 35, 140);
	ui_drawBitmap(hWnd, IDB_WAVESHAPER, 35, 200);
	UpdateWindow(hWnd);
}



//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) 
	{   
	    case WM_SETTINGCHANGE:                      //Handle the SIP being put up
			SHACTIVATEINFO sai;
			memset(&sai, 0, sizeof(SHACTIVATEINFO));
			SHHandleWMSettingChange(hWnd, -1, 0, &sai);//This will force a HWND resize depending on the SIP condition
			drawMainWindow(hWnd);	// repaint our custom UI.
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			// Parse the menu selections:
			switch (wmId)
			{
			    case IDM_HELP_ABOUT:
				    DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC) About);
				    break;

				case IDM_CALCULATOR:
					DialogBox(hInst, (LPCTSTR)IDD_CALCULATOR, hWnd, (DLGPROC) CalcView);
					break;

				case IDM_CONVERSION:
					DialogBox(hInst, (LPCTSTR)IDD_CONVERSION, hWnd, (DLGPROC) ConversionView);
					break;

				case IDM_DATAPLOTTER:
					DialogBox(hInst, (LPCTSTR)IDD_DATAPLOTTER, hWnd, (DLGPROC) DataPlotter);
					break;

			    case IDM_LOAN_FORM:
				    DialogBox(hInst, (LPCTSTR)IDD_LOANFORM, hWnd, (DLGPROC) LoanForm);
			    	break;

				case IDM_EFFECTS:	// mirror.
					DialogBox(hInst, (LPCTSTR)IDD_EFFECTS, hWnd, (DLGPROC) Effects);
					break;

				case IDM_WAVESHAPER:
				    DialogBox(hInst, (LPCTSTR)IDD_WAVESHAPER, hWnd, (DLGPROC) Waveshape);
					break;

				case IDM_TABLE_ASCII:
					DialogBox(hInst, (LPCTSTR)IDD_TEXTWINDOW, hWnd, (DLGPROC) Table_Ascii);
					break;

				case IDM_TABLE_EQUIVALENTS:
					DialogBox(hInst, (LPCTSTR)IDD_TEXTWINDOW, hWnd, (DLGPROC) Table_Equivalents);
					break;

				case IDM_TABLE_FINANCIALS:
					DialogBox(hInst, (LPCTSTR)IDD_TEXTWINDOW, hWnd, (DLGPROC) Table_Financials);
					break;

		    	case IDM_MAIN_EXIT:
		    		DestroyWindow(hWnd);
		    		break;

		    	default:
		    		return DefWindowProc(hWnd, message, wParam, lParam);
    		    	break;
			}

			drawMainWindow(hWnd);
			break;


        case WM_CREATE:
			SHMENUBARINFO mbi;
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hWnd;
			mbi.nToolBarId = IDM_MAIN_MENU;
			mbi.hInstRes   = hInst;
			mbi.nBmpId     = 0;
			mbi.cBmpImages = 0;	
        
			if (!SHCreateMenuBar(&mbi))
				MessageBox(hWnd, L"SHCreateMenuBar Failed", L"Error", MB_OK);
			hwndCB = mbi.hwndMB;

			amort_init(0, 2001, 206150.0, 0.0, 6.875, 360);  // set default loan values.
			break;

		case WM_KEYDOWN:
		    DialogBox(hInst, (LPCTSTR)IDD_LOANFORM, hWnd, (DLGPROC)LoanForm);
//            DialogBox(hInst, (LPCTSTR)IDD_LOANINFO, hWnd, (DLGPROC)LoanView);
			drawMainWindow(hWnd);
			break;

//		case WM_ACTIVATE:			// if application or window becomes active...
		case WM_SETFOCUS:
			drawMainWindow(hWnd);	// repaint our custom UI.
			break;

/*		case WM_KILLFOCUS:
			SHSipPreference(hWnd, SIP_DOWN);
			break;
*/
		case WM_MOUSEMOVE:
		{   int x,y;
			x = LOWORD(lParam);
			y = HIWORD(lParam);

			if ( x > 35 && y > 20 && x < 35+163 && y < 20+42)	// Calc Icon
			{	DialogBox(hInst, (LPCTSTR)IDD_CALCULATOR, hWnd, (DLGPROC) CalcView);
			}
			else
			if ( x > 35 && y > 80 && x < 35+163 && y < 80+42)   // Conversion Icon
			{	DialogBox(hInst, (LPCTSTR)IDD_CONVERSION, hWnd, (DLGPROC) ConversionView);
			}
			else
			if (x > 35 && y > 140 && x < 35+163 && y < 140+42)  // Loan Icon
			{
				DialogBox(hInst, (LPCTSTR)IDD_LOANFORM, hWnd, (DLGPROC)LoanForm);
//		        DialogBox(hInst, (LPCTSTR)IDD_LOANINFO, hWnd, (DLGPROC)LoanView);
			}
			else
			if (x > 35 && y > 200 && x < 35+163 && y < 200+42)  // Waveshaper Icon
			{	DialogBox(hInst, (LPCTSTR)IDD_WAVESHAPER, hWnd, (DLGPROC) Waveshape);
			}			
			drawMainWindow(hWnd);
		}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:                // Tell the command bar to resize 
			                         //itself to fill the top of window
		    SendMessage(hwndCB, TB_AUTOSIZE, 0L, 0L);
//			CommandBar_AlignAdornments(hwndCB);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
            break;

	}
	return 0;
}

// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hWndListView;

	switch (message)
	{
		case WM_INITDIALOG:
      //On Rapier devices you normally create all Dialog's as fullscreen dialog's
      // with an OK button in the upper corner. 
      SHINITDLGINFO shidi;
      // Create a Done button and size it.
      shidi.dwMask = SHIDIM_FLAGS;
      shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
      shidi.hDlg = hDlg;
      //initialzes the dialog based on the dwFlags parameter
      SHInitDialog(&shidi);
			return TRUE;
			
		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
	return FALSE;
}


