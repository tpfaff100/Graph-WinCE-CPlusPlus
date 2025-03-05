#ifndef TOOLS

#define TOOLS 1

#include "stdafx.h"
#include "common.h"

typedef struct tableData
{   
	TCHAR *title;				// title of table.
	int colsizes[10];			// horizontal sizes of each column.
	TCHAR *colnames[10];		// titles of column headings.
	int colcount;				// number of columns in table.
	int dsetcount;
} td;

HWND CreateImageView (HWND hWndParent);
//HWND CreateAmortizationListView (HWND hWndParent);       // Creates list view and initializes it.
HWND CreateGenListView (HWND hWndParent, td *tdata);       // Creates generic list view and initializes it.

HWND CreateListView(HWND hWndParent);
HWND FillListView(HWND hWndList, td *tdata);

// general UI functions.
void ui_clearWindow(HWND hWnd);
void ui_repaintWindow(HWND hWnd);
void ui_drawBitmap(HWND hWnd, int id, int x, int y);
void ui_resetComboBox(HWND hwnd, int uiwidgetval);
int  ui_getComboIndex(HWND hwnd, int uiwidgetval);
void ui_setComboIndex(HWND hwnd, int uiwidgetval, int index);
void ui_addToComboBoxStr(HWND hwnd, int uiwidgetval, TCHAR *title);
void ui_addToComboBox(HWND hwnd, int uiwidgetval, float value, int type);

#endif