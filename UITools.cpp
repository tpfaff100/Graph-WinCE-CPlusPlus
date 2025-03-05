#include "stdafx.h"
#include "afxwin.h"
#include "Graph2.h"
#include <commctrl.h>
#include <windowsx.h>
#include "Amort.h"
#include "UITools.h"
#include "common.h"


// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle


void ui_clearWindow(HWND hWnd)
{	InvalidateRect(hWnd, NULL, TRUE);// Clear the window.
	UpdateWindow(hWnd);
}

void ui_repaintWindow(HWND hWnd)	// pass in either HWND or HDLG.
{	RECT rc;
	GetWindowRect(hWnd, &rc);
	InvalidateRect(hWnd, &rc, TRUE);
	UpdateWindow(hWnd);
}

void ui_drawBitmap(HWND hWnd, int imgno, int x, int y)
{
	HBITMAP hbm = LoadBitmap(hInst,MAKEINTRESOURCE(imgno));

	HDC hdc = GetDC(hWnd);
	HDC hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbm);

	
	BITMAP bm;// Get the size of the bitmap.
	GetObject(hbm, sizeof(BITMAP), &bm );

	BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcTemp, 0, 0, SRCCOPY);

	ReleaseDC(hWnd,hdc);
}


void ui_resetComboBox(HWND hwnd, int uiwidgetval)
{	HWND hwComboBox = GetDlgItem(hwnd, uiwidgetval);//get the control window
	ComboBox_ResetContent(hwComboBox); 
    ComboBox_ShowDropdown(hwComboBox, FALSE);
}

void ui_addToComboBoxStr(HWND hwnd, int uiwidgetval, TCHAR *title)
{	HWND hwComboBox = GetDlgItem(hwnd, uiwidgetval);//get the control window
    ComboBox_AddString(hwComboBox, title);
	ComboBox_SetText(hwComboBox, title);
	ComboBox_SetCurSel(hwComboBox, 0);
}

int  ui_getComboIndex(HWND hwnd, int uiwidgetval)
{	HWND hwComboBox = GetDlgItem(hwnd, uiwidgetval);//get the control window
	return ComboBox_GetCurSel(hwComboBox);
}

void ui_setComboIndex(HWND hwnd, int uiwidgetval, int index)
{	HWND hwComboBox = GetDlgItem(hwnd, uiwidgetval);//get the control window
	ComboBox_SetCurSel(hwComboBox, index);
}

void ui_addToComboBox(HWND hwnd, int uiwidgetval, float value, int type)
{   HWND hwComboBox = GetDlgItem(hwnd, uiwidgetval);//get the control window
    TCHAR szBufW[255];					// text to show in control.

	switch(type)
	{   case 0:
            wsprintf(szBufW, L"%d", (int) value);
	        break;
		case 1:
			wsprintf(szBufW, L"%4.2f", value);
			break;
		case 2:
			wsprintf(szBufW, L"%4.3f", value);
			break;
		case 3:
			wsprintf(szBufW, L"%f", value);
			break;
		case 4:
			wsprintf(szBufW, L"%4.4f", value);
			break;	
		default:
			return;
			break;

	}

//	if (!ComboBox_FindStringExact(hwComboBox, 0, szBufW))	// refrain from duplicate entries.
	{
		ComboBox_AddString(hwComboBox, szBufW);
		ComboBox_SetText(hwComboBox, szBufW);
		ComboBox_SetCurSel(hwComboBox, 0);
	}
}



HWND CreateImageView (HWND hWndParent)
{
	HWND hWndList;      		// handle to list view window
	RECT rcl;           		// rectangle for setting size of window
//	int index;					// index used in for loops
//	LV_COLUMN lvC;				// list view column structure
//	TCHAR szText[MAX_PATH];		// place to store some text
//	LVITEM lvI;				// list view item structure
//	int iSubItem;				// index into column header string table
//	RECT rcClient;

	// Ensure that the common control DLL is loaded.
	InitCommonControls();
	
	// Get the size and position of the parent window.
	GetClientRect(hWndParent, &rcl);
	
	// Create the list view window that starts out in details view
    // and supports label editing.
	hWndList = CreateWindowEx( 0L, WC_LISTVIEW,              // list view class
		TEXT("Amortization Chart"),                         // no default text
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | WM_INITDIALOG |
		LVS_EDITLABELS | LVS_NOSORTHEADER ,	// styles
		rcl.top, rcl.left,
		rcl.right - rcl.left, rcl.bottom - rcl.top,
		hWndParent,
		(HMENU) ID_LISTVIEW,
		hInst,
		NULL );

	if (hWndList == NULL )
		return NULL;
	return hWndList;
}



HWND CreateListView(HWND hWndParent)
{	
	HWND hWndList;      		// handle to list view window
	RECT rcl;           		// rectangle for setting size of window
	InitCommonControls();		// Ensure that the common control DLL is loaded.
	
	GetClientRect(hWndParent, &rcl);// Get the size and position of the parent window.
	hWndList = CreateWindowEx( 0L,
		WC_LISTVIEW,                // list view class
		TEXT("Amortization Chart"),                         // no default text
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | WM_INITDIALOG |
		LVS_EDITLABELS | LVS_NOSORTHEADER ,	// styles
		rcl.top, rcl.left,
		rcl.right - rcl.left, rcl.bottom - rcl.top,
		hWndParent,
		(HMENU) ID_LISTVIEW,
		hInst,
		NULL );

		return hWndList;
}

HWND FillListView(HWND hWndList, td *tdata)
{
	int index;					// index used in for loops
	LV_COLUMN lvC;				// list view column structure
	TCHAR szText[MAX_PATH];		// place to store some text
	LVITEM lvI;				// list view item structure
	int iSubItem;				// index into column header string table
	// Now initialize the columns you will need.
	// Initialize the LV_COLUMN structure.
	// The mask specifies that the fmt, width, pszText, and subitem members 
	// of the structure are valid,
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align column
	lvC.cx = 50;            // width of column in pixels
	lvC.pszText = szText;
	
							// Add the columns.
	for (index = 0; index < tdata->colcount; index++)
	{	lvC.iSubItem = index;
		lvC.cx = tdata->colsizes[index];
		if (tdata->colnames[index] != NULL)
			wcscpy(szText, tdata->colnames[index]);
		if (ListView_InsertColumn(hWndList, index, &lvC) == -1)
			return NULL;
	}
	
	// Finally, add the actual items to the control.
	// Fill out the LV_ITEM structure for each of the items to add to the list.
	// The mask specifies the the pszText, iImage, lParam and state
	// members of the LV_ITEM structure are valid.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvI.state = 0;      
	lvI.stateMask = 0;  

	princIntBal *pib = amort_getPIB();	// get amortization table from Amort.c module.

	for (index = 0; index < tdata->dsetcount; index++)
	{
		lvI.iItem = index;
		lvI.iSubItem = 0;
        wsprintf(szText, L"%2d-%2d", index/12+1, index%12+1);
		lvI.pszText = szText; 
		lvI.cchTextMax = MAX_ITEMLEN;
		
		if (ListView_InsertItem(hWndList, &lvI) == -1)
			return NULL;
		
		for (iSubItem = 1; iSubItem < tdata->colcount; iSubItem++)
		{   
			switch(iSubItem)
			{   case 1:
				    wsprintf(szText, L"%5.2f", pib->p[index], index);
					break;
			    case 2:
                    wsprintf(szText, L"%5.2f", pib->i[index], index);
					break;
				case 3:
					wsprintf(szText, L"%5.2f", pib->b[index], index);
					break;
				default:
                    wsprintf(szText, L"Col %d Item %d",iSubItem, index);
					break;
			}
			ListView_SetItemText( hWndList, index, iSubItem, szText);
		}
	}
	return (hWndList);
}



HWND CreateGenListView (HWND hWndParent, td *tdata)       // Creates generic list view and initializes it.
{
	HWND hWndList;      		// handle to list view window
	RECT rcl;           		// rectangle for setting size of window
	int index;					// index used in for loops
	LV_COLUMN lvC;				// list view column structure
	TCHAR szText[MAX_PATH];		// place to store some text
	LVITEM lvI;					// list view item structure
	int iSubItem;				// index into column header string table

	InitCommonControls();		// Ensure that the common control DLL is loaded.
	GetClientRect(hWndParent, &rcl);// Get the size and position of the parent window.
	
	// Create the list view window that starts out in details view
    // and supports label editing.
	hWndList = CreateWindowEx( 0L, WC_LISTVIEW, tdata->title,
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | WM_INITDIALOG |
		LVS_EDITLABELS | LVS_NOSORTHEADER ,	// styles
		rcl.top, rcl.left, rcl.right - rcl.left, rcl.bottom - rcl.top,
		hWndParent,	(HMENU) ID_LISTVIEW, hInst,	NULL );

	if (hWndList == NULL )
		return NULL;
	
	// Now initialize the columns you will need.
	// Initialize the LV_COLUMN structure.
	// The mask specifies that the fmt, width, pszText, and subitem members 
	// of the structure are valid,
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align column
	lvC.cx = 50;            // width of column in pixels
	lvC.pszText = szText;
	
	// Add the columns.
	for (index = 0; index < tdata->colcount; index++)
	{	lvC.iSubItem = index;
		lvC.cx = tdata->colsizes[index];
		wcscpy(szText, tdata->colnames[index]);
		if (ListView_InsertColumn(hWndList, index, &lvC) == -1)
			return NULL;
	}
	
	// Finally, add the actual items to the control.
	// Fill out the LV_ITEM structure for each of the items to add to the list.
	// The mask specifies the the pszText, iImage, lParam and state
	// members of the LV_ITEM structure are valid.
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvI.state = 0;      
	lvI.stateMask = 0;  

//	int numberOfMonths = amort_getDuration();
//	princIntBal *pib = amort_getPIB();	// get amortization table from Amort.c module.

	for (index = 0; index < tdata->dsetcount; index++)
	{
		lvI.iItem = index;
		lvI.iSubItem = 0;
		// The parent window is responsible for storing the text. 
		// The list view control will send an LVN_GETDISPINFO 
		// when it needs the text to display.
//        wsprintf(szText, L"%g", tdata->dset[index][0]);
		lvI.pszText = szText; 
		lvI.cchTextMax = MAX_ITEMLEN;

		if (ListView_InsertItem(hWndList, &lvI) == -1)
			return NULL;

		for (iSubItem = 1; iSubItem < tdata->colcount; iSubItem++)
		{   
//			wsprintf(szText, L"%g", tdata->dset[index][iSubItem]);
//			wsprintf(szText, L"%d,%d", index, iSubItem);
			ListView_SetItemText( hWndList,
				index,
				iSubItem,
				szText);
		}

	}

	return (hWndList);
}


