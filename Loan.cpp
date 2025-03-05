#include "Loan.h"

#define ID_TIMER WM_USER + 300

// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle

static int loanNumber = 1;
static int asyncDrawStage = 0;


// Mesage handler for the Loan Form dialog.
LRESULT CALLBACK LoanForm(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	static HWND hWndListView;

	switch (message)
	{   case WM_INITDIALOG:
		{   float count;
			TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);			
			SHMENUBARINFO mbi;				// create menubar for loanform dialog.
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_LOANFORM_MENU;
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

			swprintf(buf, L"Loan#%d",amort_getIndex()+1);
			SetDlgItemTextW(hDlg, IDC_LOANTITLE, buf);
			loanNumber++;


			ui_resetComboBox(hDlg, IDC_MORTGAGEAMT);

			ui_addToComboBox(hDlg, IDC_MORTGAGEAMT, (float) amort_getPrincipal(), 0);
            for (count = 50000; count <= 1000000; count+= 5000)
				ui_addToComboBox(hDlg, IDC_MORTGAGEAMT, count, 0);

			for (count = 4.75; count <= 8.0; count+= .125)
			    ui_addToComboBox(hDlg, IDC_INTERESTRATE, count, 2);

			for (count = 0; count <= 3000.0; count+= 100.0)
				ui_addToComboBox(hDlg, IDC_ADDTLPYMT, count, 0);

			ui_addToComboBox(hDlg, IDC_PMI, 0, 0);
			for (count = 100.0; count <= 500.0; count+= 10)
				ui_addToComboBox(hDlg, IDC_PMI, count, 0);

			for (count = 0.0; count <= 2500.0; count+= 100)
				ui_addToComboBox(hDlg, IDC_ANNUALINS, count, 0);

			for (count = 0; count <= 5000.0; count+= 100)
				ui_addToComboBox(hDlg, IDC_ANNUALTAX, count, 0);

			ui_addToComboBox(hDlg, IDC_YEARS, 1, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 2, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 3, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 5, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 7, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 10, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 15, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 30, 0);
			ui_addToComboBox(hDlg, IDC_YEARS, 40, 0);

			resetLoanForm(hDlg);

//			SetTimer(hDlg, ID_TIMER, 10000, NULL);

			amort_init(0, 2001, 206150, 0, 6.875, 360);	// init first time around
			amort_calculatePayment();
			amort_amortize();

			LocalFree(buf);
		}
			return TRUE;
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 


////////////////////////////////////////////// LOOK FOR AN EVENT TYPE FROM UI WIDGET.
			switch (wmEvent)				// here we are looking for key presses and 
			{								// selections from ComboBox widgets.
				case CBN_SELCHANGE:			// triggered at end of event-processing of Combobox. 
					updateUI(hDlg, wmId);	// combo box has changed- update entire UI.
					amort_amortize();
					ui_repaintWindow(hDlg);		// indirectly trigger a WM_PAINT message.
					break;

			
				case CBN_EDITCHANGE:		// triggered at end of event-processing of keypress.
				{   TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
					float fprincipal, finterestrate, faddlpymt;
					int npayments;
					float monpay;
					float val;


					GetDlgItemTextW(hDlg, IDC_MORTGAGEAMT, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &fprincipal);

					GetDlgItemTextW(hDlg, IDC_INTERESTRATE, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &finterestrate);

					GetDlgItemTextW(hDlg, IDC_YEARS, buf, MAX_TF_SIZE);
					swscanf(buf, L"%d", &npayments);
					npayments *= 12;

					GetDlgItemTextW(hDlg, IDC_ADDTLPYMT, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &faddlpymt);

					if (fprincipal > 0 && finterestrate > 0 && 
						npayments > 0 && npayments < 1000)  // paranoia
					{
														// reinitialize the loan.
						amort_init(0, 2001, (double) fprincipal, (double) faddlpymt, 
											(double) finterestrate, npayments);
						monpay = (float) amort_calculatePayment();

						swprintf(buf, L"%5.2f", monpay);					// get monthly payment for property.
						SetDlgItemTextW(hDlg, IDC_LOANPAYMENT, buf);		// display monthly payment.

						GetDlgItemTextW(hDlg, IDC_PMI, buf, MAX_TF_SIZE);	// get annual insurance cost.
						swscanf(buf, L"%f", &val);							// store in val.
						monpay += val;										

						GetDlgItemTextW(hDlg, IDC_ANNUALINS, buf, MAX_TF_SIZE);// get annual insurance cost.
						swscanf(buf, L"%f", &val);							// store in val.
						monpay += val/12;										

						GetDlgItemTextW(hDlg, IDC_ANNUALTAX, buf, MAX_TF_SIZE);// get annual tax cost.
						swscanf(buf, L"%f", &val);							// store in val.
						monpay += val/12;

						swprintf(buf, L"%5.2f", monpay);					// get total monthly cost.
						SetDlgItemTextW(hDlg, IDC_MONTHLYPAYMENT, buf);		// display total monthly cost.
						amort_amortize();
					}
					LocalFree(buf);
					ui_repaintWindow(hDlg);		// indirectly trigger a WM_PAINT message.
					return TRUE;
				}
					break;
			}


			// Parse the menu selections:
			switch (wmId)
			{
/*			    case IDM_LOANFORM_NEXT:
					SetDlgItemTextW(hDlg, IDC_MORTGAGEAMT, L"0");
					SetDlgItemTextW(hDlg, IDC_INTERESTRATE, L"0");
					SetDlgItemTextW(hDlg, IDC_YEARS, L"0");
					SetDlgItemTextW(hDlg, IDC_ADDTLPYMT, L"0");
					SetDlgItemTextW(hDlg, IDC_LOANPAYMENT, L"0");
					SetDlgItemTextW(hDlg, IDC_MONTHLYPAYMENT, L"0");
					SetDlgItemTextW(hDlg, IDC_PMI, L"0.00");
					SetDlgItemTextW(hDlg, IDC_ANNUALINS, L"0.00");
					SetDlgItemTextW(hDlg, IDC_ANNUALTAX, L"0.00");
					amort_getNext();
					break;

				case IDM_LOANFORM_PREVIOUS:
					SetDlgItemTextW(hDlg, IDC_MORTGAGEAMT, L"0");
					SetDlgItemTextW(hDlg, IDC_INTERESTRATE, L"0");
					SetDlgItemTextW(hDlg, IDC_YEARS, L"0");
					SetDlgItemTextW(hDlg, IDC_ADDTLPYMT, L"0");
					SetDlgItemTextW(hDlg, IDC_LOANPAYMENT, L"0");
					SetDlgItemTextW(hDlg, IDC_MONTHLYPAYMENT, L"0");
					SetDlgItemTextW(hDlg, IDC_PMI, L"0.00");
					SetDlgItemTextW(hDlg, IDC_ANNUALINS, L"0.00");
					SetDlgItemTextW(hDlg, IDC_ANNUALTAX, L"0.00");
					amort_getPrevious();
					break;
*/

				case IDC_LOANFORM_NEXTINFO:		// display next in series of canvases.
					asyncDrawStage++;			// enable next drawing stage.
					ui_repaintWindow(hDlg);		// indirectly trigger a WM_PAINT message.
					break;

				case IDM_LOANFORM_RESET:
					resetLoanForm(hDlg);
					break;

				case IDM_LOANFORM_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					break;

				case IDOK:
				case IDCANCEL:
				case IDM_LOANFORM_AMORTIZE:
				{   TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
					float fprincipal, finterestrate, faddlpymt;
					int npayments;

					GetDlgItemTextW(hDlg, IDC_MORTGAGEAMT, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &fprincipal);

					GetDlgItemTextW(hDlg, IDC_INTERESTRATE, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &finterestrate);

					GetDlgItemTextW(hDlg, IDC_YEARS, buf, MAX_TF_SIZE);
					swscanf(buf, L"%d", &npayments);
					npayments *= 12;

					GetDlgItemTextW(hDlg, IDC_ADDTLPYMT, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &faddlpymt);
														// reinitialize the loan.
					amort_init(0, 2001, fprincipal, faddlpymt, finterestrate, npayments);
					amort_calculatePayment();
					amort_amortize();

					EndDialog(hDlg, LOWORD(wParam));
					DialogBox(hInst, (LPCTSTR)IDD_LOANINFO, hDlg, (DLGPROC) LoanView);
					LocalFree(buf);
					return TRUE;
				}
				break;
			}
			break;

		case WM_PAINT:
		{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
			PAINTSTRUCT ps;
			RECT rcClient;
			GetClientRect (hDlg, &rcClient);
			HDC hdc = BeginPaint (hDlg, &ps);

			if (amort_getPrincipal() < 100 || amort_getPrincipal() > 500000000)
				return TRUE;

			switch(asyncDrawStage%6)
			{
				case 0:
					break;

				case 1:
				{	princIntBal *pib = amort_getPIB();

					if (pib != NULL)
					{	int count;
						int months = amort_getDuration();
						float total;
						float autosizer;						// amount to divide maximum value in graph by.

						total = pib->p[months-1]+pib->p[months-2]+pib->p[months-3]+pib->p[months-4]+pib->p[months-5]+
									pib->p[months-6]+pib->p[months-7]+pib->p[months-8]+pib->p[months-9]+pib->p[months-10]+
									pib->p[months-11]+pib->p[months-12]+pib->p[months-13];
						autosizer = total / 75;

						ExtTextOut(hdc, 5, 160, ETO_OPAQUE, NULL, L"Principal", 9, NULL);
						for (count = 0; count < months; count+=12)
						{   total = pib->p[0+count]+pib->p[1+count]+pib->p[2+count]+pib->p[3+count]+pib->p[4+count]+
									pib->p[5+count]+pib->p[6+count]+pib->p[7+count]+pib->p[8+count]+pib->p[9+count]+
									pib->p[10+count]+pib->p[11+count]+pib->p[12+count];
							
							Rectangle(hdc, (float)count/1.5, 270, (float)count/1.5+12, 270-total/autosizer);
							if (count % 120 == 0)
							{	swprintf(buf, L"%2d", count/12);
								ExtTextOut(hdc, count/1.5-9, 250, ETO_OPAQUE, NULL, buf, 2, NULL);

							}
						}
					}
				}
					break;

				case 2:
				{	princIntBal *pib = amort_getPIB();

					if (pib != NULL)
					{	int count;
						int months = amort_getDuration();
						float total;
						float autosizer;						// amount to divide maximum value in graph by.
					
						ExtTextOut(hdc, 5, 160, ETO_OPAQUE, NULL, L"Interest", 8, NULL);
						for (count = 0; count < months; count+=12)
						{   total = pib->i[0+count]+pib->i[1+count]+pib->i[2+count]+pib->i[3+count]+pib->i[4+count]+
									pib->i[5+count]+pib->i[6+count]+pib->i[7+count]+pib->i[8+count]+pib->i[9+count]+
									pib->i[10+count]+pib->i[11+count]+pib->i[12+count];
							if (count == 0)						// 0th year of loan will be the maximum value in graph.
								autosizer = total / 75;
							Rectangle(hdc, (float)count/1.5, 270, (float)count/1.5+12, 270-total/autosizer);
							if (count % 120 == 0)
							{	swprintf(buf, L"%2d", count/12);
								ExtTextOut(hdc, (int) (count/1.5)-9, 250, ETO_OPAQUE, NULL, buf, 2, NULL);

							}
						}
					}
				}
					break;

				case 3:
				{	princIntBal *pib = amort_getPIB();

					if (pib != NULL)
					{	int count;
						int months = amort_getDuration();
						float total;
						float autosizer;						// amount to divide maximum value in graph by.
					
						ExtTextOut(hdc, 5, 160, ETO_OPAQUE, NULL, L"Balance", 7, NULL);
						for (count = 0; count < months; count+=12)
						{   total = pib->b[0+count]+pib->b[1+count]+pib->b[2+count]+pib->b[3+count]+pib->b[4+count]+
									pib->b[5+count]+pib->b[6+count]+pib->b[7+count]+pib->b[8+count]+pib->b[9+count]+
									pib->b[10+count]+pib->b[11+count]+pib->b[12+count];
							if (count == 0)						// 0th year of loan will be the maximum value in graph.
								autosizer = total / 75;
							Rectangle(hdc, (float)count/1.5, 270, (float)count/1.5+12, 270-total/autosizer);
							if (count % 120 == 0)
							{	swprintf(buf, L"%2d", count/12);
								ExtTextOut(hdc, count/1.5-9, 250, ETO_OPAQUE, NULL, buf, 2, NULL);

							}
						}
					}
				}
					break;

				case 4:
				{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);

					if (amort_getMonthlyPayment() > 0)
					{	double monpay;				// monthly payment for each interest rate.
						double rate[12];			// list of interest rates we want to show.
						int count;
						rate[10] = amort_getAnnualRate();// get user's chosen interest rate.

						rate[0] = rate[10] - 1.0;		// make some related interest rates.
						rate[2] = rate[10] - .5;
						rate[4] = rate[10] - .375;
						rate[6] = rate[10] - .25;
						rate[8] = rate[10] - .125;

						rate[1] = rate[10] + .125;
						rate[3] = rate[10] + .25;
						rate[5] = rate[10] + .375;
						rate[7] = rate[10] + .5;
						rate[9] = rate[10] + .75;
						rate[11] = rate[10] + 1.0;

						ExtTextOut(hdc, 5, 160, ETO_OPAQUE, NULL, L"Payments at other rates:", 24, NULL);

						for (count = 0; count < 12; count++)
						{	amort_setAnnualRate( (double) rate[count]);
							monpay = amort_calculatePayment();

							swprintf(buf, L"%5.3f", rate[count]);
							if (count % 2 == 0)
								ExtTextOut(hdc, 5, 175+count/2*15, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);
							else
								ExtTextOut(hdc, 130, 175+count/2*15, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);

							swprintf(buf, L"%5.2f/mo", monpay);	// convert monthly payment into string.
							if (count % 2 == 0)
								ExtTextOut(hdc, 45, 175+count/2*15, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);
							else
								ExtTextOut(hdc, 170, 175+count/2*15, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);
						}
						amort_setAnnualRate(rate[10]);	// reset back to user's interest rate.
					}
					LocalFree(buf);
				}
					break;

				case 5:
				{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
					float principal = (float) amort_getPrincipal();
					float interest = (float) amort_getTotalInterest();
					swprintf(buf, L"Principal to be paid on loan: %10.2f", principal);
					ExtTextOut(hdc, 5, 180, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);
					swprintf(buf, L"Interest to be paid on loan: %10.2f", interest);
					ExtTextOut(hdc, 5, 195, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);
					swprintf(buf, L"Total cost of loan: %10.2f", principal+interest);
					ExtTextOut(hdc, 5, 210, ETO_OPAQUE, NULL, buf, wcslen(buf), NULL);
					LocalFree(buf);
				}
					break;
				
				default:
					break;
			}

			EndPaint(hDlg, &ps);
			LocalFree(buf);
		}
			break;
			
		case WM_TIMER:
			asyncDrawStage++;			// enable next drawing stage.
			ui_repaintWindow(hDlg);		// indirectly trigger a WM_PAINT message.
			break;

	}
	return FALSE;
}



void resetLoanForm(HWND hDlg)
{	ui_setComboIndex(hDlg, IDC_MORTGAGEAMT, 0);
	ui_setComboIndex(hDlg, IDC_INTERESTRATE, 25);
	ui_setComboIndex(hDlg, IDC_YEARS, 7);
	ui_setComboIndex(hDlg, IDC_ADDTLPYMT, 0);
	ui_setComboIndex(hDlg, IDC_LOANPAYMENT, 0);
	ui_setComboIndex(hDlg, IDC_MONTHLYPAYMENT, 0);
	ui_setComboIndex(hDlg, IDC_PMI, 0);
	ui_setComboIndex(hDlg, IDC_ANNUALINS, 0);
	ui_setComboIndex(hDlg, IDC_ANNUALTAX, 0);
	amort_init(0, 2001, 206150, 0, 6.875, 360);	// init first time around
	
	amort_calculatePayment();
	amort_amortize();
	updateUI(hDlg, -1);
	ui_repaintWindow(hDlg);		// indirectly trigger a WM_PAINT message.
}

void updateUI(HWND hDlg, int wmId)		// ui update when changes result from a combo box update.
{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
	float fprincipal, finterestrate, faddlpymt;
	int npayments;				
	HWND hwComboBox;
	
	hwComboBox = GetDlgItem(hDlg, IDC_MORTGAGEAMT);//get the control window
	if (wmId == IDC_MORTGAGEAMT)
		ComboBox_SelectString(hwComboBox, 10, NULL);
	ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
	swscanf(buf, L"%f", &fprincipal);

	hwComboBox = GetDlgItem(hDlg, IDC_INTERESTRATE);//get the control window
	if (wmId == IDC_INTERESTRATE)
		ComboBox_SelectString(hwComboBox, 10, NULL);
	ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
	swscanf(buf, L"%f", &finterestrate);


    hwComboBox = GetDlgItem(hDlg, IDC_YEARS);//get the control window
	if (wmId == IDC_YEARS)
		ComboBox_SelectString(hwComboBox, 10, NULL);
	ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
	swscanf(buf, L"%d", &npayments);
	npayments *= 12;

    hwComboBox = GetDlgItem(hDlg, IDC_ADDTLPYMT);//get the control window
	if (wmId == IDC_ADDTLPYMT)
		ComboBox_SelectString(hwComboBox, 10, NULL);
	ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
	swscanf(buf, L"%f", &faddlpymt);


	if (fprincipal > 0 && finterestrate > 0 && 
		npayments > 0 && npayments < 1000)  // paranoia
	{   float monpay, val;
										// reinitialize the loan.
		amort_init(0, 2001, (double) fprincipal, (double) faddlpymt, 
							(double) finterestrate, npayments);
		monpay = (float) amort_calculatePayment();
		
		swprintf(buf, L"%5.2f", monpay);					// get monthly payment for property.
		SetDlgItemTextW(hDlg, IDC_LOANPAYMENT, buf);		// display monthly payment.

		hwComboBox = GetDlgItem(hDlg, IDC_PMI);//get the control window
		if (wmId == IDC_PMI)
			ComboBox_SelectString(hwComboBox, 10, NULL);
		ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
		swscanf(buf, L"%f", &val);
		monpay += val;

		hwComboBox = GetDlgItem(hDlg, IDC_ANNUALINS);//get the control window
		if (wmId == IDC_ANNUALINS)
			ComboBox_SelectString(hwComboBox, 10, NULL);
		ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
		swscanf(buf, L"%f", &val);
		monpay += val/12;

		hwComboBox = GetDlgItem(hDlg, IDC_ANNUALTAX);//get the control window
		if (wmId == IDC_ANNUALTAX)
			ComboBox_SelectString(hwComboBox, 10, NULL);
		ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
		swscanf(buf, L"%f", &val);
		monpay += val/12;

		swprintf(buf, L"%5.2f", monpay);					// get total monthly cost.
		SetDlgItemTextW(hDlg, IDC_MONTHLYPAYMENT, buf);		// display total monthly cost.
	}


}


// Mesage handler for the Loan Amortization dialog.
LRESULT CALLBACK LoanView(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;	
	static HWND hWndListView;

	switch (message)
	{	case WM_INITDIALOG:
			SHMENUBARINFO mbi;      
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_LOANINFO_MENU;
			mbi.hInstRes   = hInst;
			mbi.nBmpId     = 0;
			mbi.cBmpImages = 0;	
        
			if (!SHCreateMenuBar(&mbi))
				MessageBox(hDlg, L"SHCreateMenuBar Failed", L"Error", MB_OK);
			hwndCB = mbi.hwndMB;


			SHINITDLGINFO shidi;                 //WinCE3.0/ipaq UI-specific gunk.
			shidi.dwMask = SHIDIM_FLAGS;         // create a DONE button and size it.
			shidi.hDlg = hDlg;
			shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
			SHInitDialog(&shidi);   //initialzes the dialog based on the dwFlags parameter

			hWndListView = CreateListView( hDlg );
			if (hWndListView)
			{	td thedata;
				princIntBal *pib = amort_getPIB();	// get amortization table from Amort.c module.
				thedata.colcount = 4;
				thedata.colsizes[0] = 38;
				thedata.colsizes[1] = 58;
				thedata.colsizes[2] = 58;
				thedata.colsizes[3] = 74;
				thedata.colnames[0] = L"Pmt";
				thedata.colnames[1] = L"Principle";
				thedata.colnames[2] = L"Interest";
				thedata.colnames[3] = L"Balance";
				thedata.dsetcount = amort_getDuration();	// number of months for amortization.
				FillListView(hWndListView, &thedata);
			}
			else
				MessageBox (NULL, TEXT("Listview not created!"), NULL, MB_OK );

			{   HDC hdc;
//				PAINTSTRUCT ps;
//				hdc = BeginPaint (hDlg, &ps); 

				hdc = GetDC(hDlg);
				ReleaseDC(hDlg, hdc);

//				EndPaint(hDlg, &ps);
			}
			return TRUE;
			break;
			
		case WM_SETFONT:
		{	
//			HDC hdc;
//			hdc = GetDC(hDlg);

//			CFont *font = (CFont *) GetStockObject(SYSTEM_FONT);
//				HFONT__ *font = GetWindowFont(hDlg);
//			    HFONT__ *font = GetStockObject(SYSTEM_FONT);
//				CFont font;
/*VERIFY(font->CreateFont(
   8,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_NORMAL,                 // nWeight
   TRUE,                     // bItalic
   TRUE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   _T("Arial")));                 // lpszFacename
*/
//			SelectFont(hdc, font);
//			ReleaseDC(hDlg, hdc);
		}
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			switch(wmId)
			{
				case IDM_LOANINFO_EXIT:
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
				    DialogBox(hInst, (LPCTSTR)IDD_LOANFORM, hDlg, (DLGPROC) LoanForm);
					return TRUE;
					break;

				case IDM_LOANINFO_YEARLY:
					ListView_DeleteAllItems(hWndListView);
					break;

				case IDM_LOANINFO_MONTHLY:
					ListView_DeleteAllItems(hWndListView);
					break;

				default:
					break;
			}
			break;
	}
	return FALSE;
}


