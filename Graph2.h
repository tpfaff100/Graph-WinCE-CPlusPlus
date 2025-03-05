// LISTVIEW.H
#include "resource.h"
// constants                            
#define ID_LISTVIEW     1000

#define NUM_ITEMS       360
#define NUM_COLUMNS     4
#define MAX_ITEMLEN     64
#define MAX_ADDRESS     64
#define MAX_CITY        16
#define MAX_PRICE       12
#define BITMAP_WIDTH    16
#define BITMAP_HEIGHT   16
#define LG_BITMAP_WIDTH	32
#define LG_BITMAP_HEIGHT 32

// menu commands

// Options menu

//#define IDM_LARGEICON   100
//#define IDM_SMALLICON   101
//#define IDM_LISTVIEW    102
//#define IDM_REPORTVIEW  103
//#define IDM_EXIT        104

// Help menu
#define IDM_ABOUT       300
#define IDM_LISTVIEW    301

// icons

#define REDMOND         401
#define BELLEVUE        402
#define SEATTLE         403

// stringtable defines
#define IDS_YEAR        50
#define IDS_PRINCIPAL   51
#define IDS_INTEREST    52
#define IDS_BALANCE     53	

// Function prototypes

// procs
long APIENTRY MainWndProc(HWND, UINT, UINT, LONG);


//functions
BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
HWND CreateAmortizationListView(HWND);
LRESULT NotifyHandler(HWND, UINT, WPARAM, LPARAM);
int CALLBACK ListViewCompareProc(LPARAM, LPARAM, LPARAM);

// structures
typedef struct tagHOUSEINFO
{
    TCHAR szAddress[MAX_ADDRESS];
    TCHAR szCity[MAX_CITY];
    int iPrice;
    int iBeds;
    int iBaths;
} HOUSEINFO;



