#include "Tables.h"

										// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle

LRESULT CALLBACK Table_Equivalents(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	static HWND hWndListView;

	switch (message)
	{   case WM_INITDIALOG:
		{   TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, 2048);			
			SHMENUBARINFO mbi;				// create menubar for loanform dialog.
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_SIMPLE_MENU;
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

			swprintf(buf, L"1 inch = \t\t2.4cm\r\n1 foot = \t\t0.3048 m\r\n1 yard = \t\t0.9144 meters\r\n1 mile = \t\t5280 feet\r\n1 mile = \t\t1.609 kilometers\r\n1 acre = \t\t43560 square feet\r\n1 acre = \t\t4840 square yards\r\n1 acre = \t\t0.405 hectare\r\n\r\n1 millimeter =\t.003937 inch\r\n1 centimeter =\t0.3937 inch\r\n1 decimeter =\t3.937 inches\r\n1 meter =\t39.37 inches\r\n1 dekameter =\t32.808 feet\r\n1 kilometer = \t0.621 miles\r\n1 kilometer = \t3280.8 feet\r\n\r\n1 quart US =\t0.946 liters\r\n1 quart US =\t0.833 British quart\r\n1 teaspoon = \t1/3 tablespoon\r\n1 tablespoon =\t3 teaspoons");

			SetDlgItemTextW(hDlg, IDC_TABLEEQUTEXT, buf);
			LocalFree(buf);
		}

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

		    switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_SIMPLE_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;

				case IDM_SIMPLE_COPY:
				{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, 2048);
					GetDlgItemTextW(hDlg, IDC_TABLEEQUTEXT, buf, 2048);
					OpenClipboard(hDlg);
					SetClipboardData(CF_UNICODETEXT, buf);
					LocalFree(buf);
					return TRUE;
				}
					break;
			}
			break;

		
		default:
			break;
	}
	return FALSE;
}



LRESULT CALLBACK Table_Financials(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	static HWND hWndListView;

	switch (message)
	{   case WM_INITDIALOG:
		{   TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, 2048);			
			SHMENUBARINFO mbi;				// create menubar for loanform dialog.
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_SIMPLE_MENU;
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

			swprintf(buf, L"Dow Jones Industrial Average (Close)\r\n1900\t51.79\r\n1910\t59.60\r\n1920\t71.95\r\n1930\t164.60\r\n1940\t131.13\r\n1950\t235.41\r\n1960\t615.88\r\n1970\t838.91\r\n1980\t963.98\r\n1990\t2663.66\r\n2000\t10786.85\r\n\r\nUS 10 Year Bond\r\n1900\t2.89\r\n1910\t3.46\r\n1920\t5.40\r\n1930\t3.22\r\n1940\t1.88\r\n1950\t3.18\r\n1960\t3.44\r\n1970\t6.50\r\n1980\t12.43\r\n1990\t8.08\r\n2000\t5.12\r\n\r\nJapan Nikkei 225 Stock Average\r\n1920\t27.44\r\n1930\t16.82\r\n1940\t37.42\r\n1950\t101.91\r\n1960\t1356.70\r\n1970\t1987.09\r\n1980\t7116.38\r\n1990\t23848.69\r\n2000\t13785.69\r\n\r\nUK British Pound (USD Per GDP)\r\n1900\t4.86\r\n1910\t4.86\r\n1920\t3.52\r\n1930\t4.85\r\n1940\t4.03\r\n1950\t2.80\r\n1960\t2.80\r\n1970\t2.39\r\n1980\t2.39\r\n1990\t1.93\r\n2000\t1.49");

			SetDlgItemTextW(hDlg, IDC_TABLEEQUTEXT, buf);
			LocalFree(buf);
		}

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

		    switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_SIMPLE_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;

				case IDM_SIMPLE_COPY:
				{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, 2048);
					GetDlgItemTextW(hDlg, IDC_TABLEEQUTEXT, buf, 2048);
					OpenClipboard(hDlg);
					SetClipboardData(CF_UNICODETEXT, buf);
					LocalFree(buf);
					return TRUE;
				}
					break;
			}
			break;

		
		default:
			break;
	}
	return FALSE;
}




LRESULT CALLBACK Table_Ascii(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	static HWND hWndListView;

	switch (message)
	{   case WM_INITDIALOG:
		{   TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, 1500);
			SHMENUBARINFO mbi;				// create menubar for loanform dialog.
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_SIMPLE_MENU;
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

			wcscpy(buf, L"0x00\t0\tNUL\r\n0x01\t1\tSOH\r\n0x02\t2\tSTX\r\n0x03\t3\tETX\r\n0x04\t4\tEOT\r\n0x05\t5\tENQ\r\n0x06\t6\tACK\r\n0x07\t6\tBEL\r\n0x08\t8\tHT\r\n0x09\t9\tLF\r\n0x0a\t10\tLF\r\n0x0b\t11\tVT\r\n0x0c\t12\tFF\r\n0x0d\t13\tCR\r\n0x0e\t14\tSO\r\n0x0f\t15\tSI\r\n0x10\t16\tDLE\r\n0x11\t17\tDC1\r\n0x12\t18\tDC2\r\n0x13\t19\tDC3\r\n0x14\t20\tDC4\r\n0x15\t21\tNAK\r\n0x16\t22\tSYN\r\n0x17\t23\tETB\r\n0x18\t24\tCAN\r\n0x19\t25\tEM\r\n0x1a\t26\tSUB\r\n0x1b\t27\tESC\r\n0x1c\t28\tFS\r\n0x1d\t29\tGS\r\n0x1e\t30\tRS\r\n0x1f\t31\tUS\r\n0x20\t32\tSPC\r\n0x21\t33\t!\r\n0x22\t34\t\"\r\n0x23\t35\t#\r\n0x24\t36\t$\r\n0x25\t37\t\%\r\n0x26\t38\t&\r\n0x27\t39\t'\r\n0x28\t40\t(\r\n0x29\t41\t)\r\n0x2a\t42\t*\r\n0x2b\t43\t+\r\n0x2c\t44\t,\r\n0x2d\t45\t-\r\n0x2e\t46\t.\r\n0x2f\t47\t/\r\n0x30\t48\t0\r\n0x31\t49\t1\r\n0x32\t50\t2\r\n0x33\t51\t3\r\n0x34\t52\t4\r\n0x35\t53\t5\r\n0x36\t54\t6\r\n0x37\t55\t7\r\n0x38\t56\t8\r\n0x39\t57\t9\r\n0x3a\t58\t:\r\n0x3b\t59\t;\r\n0x3c\t60\t<\r\n0x3d\t61\t=\r\n0x3e\t62\t>\r\n0x3f\t63\t?\r\n0x40\t64\t@\r\n0x41\t65\tA\r\n0x42\t66\tB\r\n0x43\t67\tC\r\n0x44\t68\tD\r\n0x45\t69\tE\r\n0x46\t70\tF\r\n0x47\t71\tG\r\n0x48\t72\tH\r\n0x49\t73\tI\r\n0x4a\t74\tJ\r\n0x4b\t75\tK\r\n0x4c\t76\tL\r\n0x4d\t77\tM\r\n0x4e\t78\tN\r\n0x4f\t79\tO\r\n");
			wcscat(buf, L"0x50\t80\tP\r\n0x51\t81\tQ\r\n0x52\t82\tR\r\n0x53\t83\tS\r\n0x54\t84\tT\r\n0x55\t85\tU\r\n0x56\t86\tV\r\n0x57\t87\tW\r\n0x58\t88\tX\r\n0x59\t89\tY\r\n0x5a\t90\tZ\r\n0x5b\t91\t[\r\n0x5c\t92\t|\r\n0x5d\t93\t]\r\n0x5e\t94\t^\r\n0x5f\t95\t_\r\n0x60\t96\t`\r\n0x61\t97\ta\r\n0x62\t98\tb\r\n0x63\t99\tc\r\n0x64\t100\td\r\n0x65\t101\te\r\n0x66\t102\tf\r\n0x67\t103\tg\r\n0x68\t104\th\r\n0x69\t105\ti\r\n0x6a\t106\tj\r\n0x6b\t107\tk\r\n0x6c\t108\tl\r\n0x6d\t109\tm\r\n0x6e\t110\tn\r\n0x6f\t111\to\r\n0x70\t112\tp\r\n0x71\t113\tq\r\n0x72\t114\tr\r\n0x73\t115\ts\r\n0x74\t116\tt\r\n0x75\t117\tu\r\n0x76\t118\tv\r\n0x77\t119\tw\r\n0x78\t120\tx\r\n0x79\t121\ty\r\n0x7a\t122\tz\r\n0x7b\t123\t{\r\n0x7c\t124\t|\r\n0x7d\t125\t}\r\n0x7e\t126\t~\r\n0x7f\t127\tDEL");

			SetDlgItemTextW(hDlg, IDC_TABLEEQUTEXT, buf);
			LocalFree(buf);
		}

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

		    switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_SIMPLE_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;

				case IDM_SIMPLE_COPY:
				{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, 2048);
					GetDlgItemTextW(hDlg, IDC_TABLEEQUTEXT, buf, 2048);
					OpenClipboard(hDlg);
					SetClipboardData(CF_UNICODETEXT, buf);
					LocalFree(buf);
					return TRUE;
				}
					break;
			}
			break;

		
		default:
			break;
	}
	return FALSE;
}



