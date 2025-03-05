#include "Calculator.h"

// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle

cd calc;								// calculation variables.

// Mesage handler for the Loan Amortization dialog.
LRESULT CALLBACK CalcView(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	int count;

	switch (message)
	{	case WM_INITDIALOG:
			SHMENUBARINFO mbi;      
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_CALC_MENU;
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

			ui_resetComboBox(hDlg, IDC_CALCLCD);	// erase combobox contents.
			ui_addToComboBox(hDlg, IDC_CALCLCD, 0.0, 3);

			count = 0;
			while (count < amort_getIndex())
			{
				ui_addToComboBox(hDlg, IDC_CALCLCD, (float) amort_getPrincipal(), 3);
				ui_addToComboBox(hDlg, IDC_CALCLCD, (float) amort_getMonthlyPayment(), 3);
				count++;
			}

			SetDlgItemTextW(hDlg, IDC_CALCLCD, L"0");
			SetDlgItemTextW(hDlg, IDC_CALCMEMORYLIGHT, L" ");

			calc_init();
			return TRUE;
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			switch(wmEvent)					
			{	case CBN_SELCHANGE:			// triggered at end of event-processing of Combobox.
				{   float displayedval;
					TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
					HWND hwComboBox;

					switch(wmId)
					{	case IDC_CALCLCD:
							hwComboBox = GetDlgItem(hDlg, IDC_CALCLCD);//get the control window
							ComboBox_SelectString(hwComboBox, 10, NULL);
							ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
							swscanf(buf, L"%f", &displayedval);
							calc_setValue(displayedval);
							break;

						default:
							break;
					}

					LocalFree(buf);
					return TRUE;
				}
					break;

				case CBN_EDITCHANGE:		// triggered at end of event-processing of keypress.
					break;
			}


            switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_CALC_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;

				case IDM_CALC_LOAN:
				    DialogBox(hInst, (LPCTSTR)IDD_LOANFORM, hDlg, (DLGPROC) LoanForm);
//                    DialogBox(hInst, (LPCTSTR)IDD_LOANINFO, hDlg, (DLGPROC) LoanView);
					break;

				case IDM_CALC_RESET:
					ui_resetComboBox(hDlg, IDC_CALCLCD);	// erase combobox contents.
					ui_addToComboBox(hDlg, IDC_CALCLCD, (float) calc_getValue(), 3);
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCLCD:
					return TRUE;
					break;

				case IDC_CALCCLEAR:
					calc_clear();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCEQUALS:
				{	double calc_val = calc_equals();
					ui_addToComboBox(hDlg, IDC_CALCLCD, (float) calc_val, 3);
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
				}
					break;

				case IDC_CALCBS:
					calc_backspace();
					calc.floatingPointLocation = 16;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.floatingPointLocation = 0;
					calc.integerPlaces = 0;
					break;

				case IDC_CALC0:
				case IDC_CALC1:
				case IDC_CALC2:
				case IDC_CALC3:
				case IDC_CALC4:
				case IDC_CALC5:
				case IDC_CALC6:
				case IDC_CALC7:
				case IDC_CALC8:
				case IDC_CALC9:
					calc_addDigit(wmId-IDC_CALC0);	// send digit to calculator module.
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCPLUS:
					calc_add();
					break;

				case IDC_CALCMINUS:
					calc_subtract();
					break;

				case IDC_CALCMULT:
					calc_multiply();
					break;

				case IDC_CALCDIV:
					calc_divide();
					break;

				case IDC_CALCAND:
					calc_and();
					break;

				case IDC_CALCOR:
					calc_or();
					break;

				case IDC_CALCXOR:
					calc_xor();
					break;

				case IDC_CALCMOD:
					calc_mod();
					break;

				case IDC_CALCSIN:
					calc_sin();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCCOS:
					calc_cos();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCTAN:
					calc_tan();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCASIN:
					calc_asin();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCACOS:
					calc_acos();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCATAN:
					calc_atan();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;


				case IDC_CALCSQRT:
					calc.floatingPointLocation = 0;
					calc_sqrt();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCPOINT:
					calc_bumpDecimalPoint();
					break;

				case IDC_CALCMEMPLUS:
					SetDlgItemTextW(hDlg, IDC_CALCMEMORYLIGHT, L"M");
					calc_memplus();
					break;

				case IDC_CALCMEMMINUS:
					SetDlgItemTextW(hDlg, IDC_CALCMEMORYLIGHT, L"M");
					calc_memminus();
					break;

				case IDC_CALCMEMRETRIEVE:
					calc_memretrieve();
					calc.floatingPointLocation = 16;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.floatingPointLocation = 0;
					calc.integerPlaces = 0;
					break;

				case IDC_CALCMEMCLEAR:
					SetDlgItemTextW(hDlg, IDC_CALCMEMORYLIGHT, L" ");
					calc_memclear();
					break;

				case IDC_CALCPLUSMINUS:
					calc_plusminus();
					calc.floatingPointLocation = 16;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.floatingPointLocation = 0;
					calc.integerPlaces = 0;
					break;

				case IDC_CALCBIN:
					calc_displayBinary();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCDEC:
					calc_displayDecimal();
					calc.floatingPointLocation = 0;
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					calc.integerPlaces = 0;
					break;

				case IDC_CALCHEX:
					calc_displayHex();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCOCT:
					calc_displayOctal();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCLEFTSHIFT:
					calc_leftShift();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;

				case IDC_CALCRIGHTSHIFT:
					calc_rightShift();
					SetDlgItemTextW(hDlg, IDC_CALCLCD, calc_getString());
					break;
			}
			break;

		case WM_CTLCOLOREDIT:
//		{	int a = LOWORD(wParam);
//			int b = LOWORD(wParam);
//			if ((a+b) == IDC_CALC0)
//				SetDlgItemTextW(hDlg, IDC_CALCLCD, L"1");
//		}
		break;
	}
	return FALSE;
}





void calc_init(void)
{	calc.value = 0.0;
    calc.holding = 0.0;
	calc.op = NON_OP;
    calc.memval = 0.0;
	calc.calcbuf = (TCHAR *) LocalAlloc (LPTR, MAX_CALC_CHARS + 1);
	swprintf(calc.calcbuf, L"%2.1f", 0);
	calc.floatingPointLocation = 0;
	calc.allowAddDigits = TRUE;
	calc.displayMode = D_DECIMAL;
}

void calc_denit(void)
{	LocalFree(calc.calcbuf);
}

void calc_addDigit(int val)
{   
	if (calc.integerPlaces + calc.floatingPointLocation > 15)
		return;

	if (!calc.allowAddDigits)
	{	calc.value = 0;
		calc.allowAddDigits = TRUE;
	}
		
    if (calc.floatingPointLocation > 0)
	{	double temp = (double) val;
		temp /= pow(10,calc.floatingPointLocation);
		calc.value += temp;
		if (calc.floatingPointLocation > 0)			// first time through is for decimal point.
			calc.floatingPointLocation++;
	}
	else
	{	calc.value = calc.value * 10 + val;
	    calc.integerPlaces++;
	}

	swprintf(calc.calcbuf, L"%.16g", calc.value);
}

void calc_backspace(void)
{	float val;
	calc.calcbuf = calc_getString();
	int len = wcslen(calc.calcbuf);
	if (len > 0)
	{	if (calc.calcbuf[len-2] == '.')
		{	len--;
			calc.floatingPointLocation = 0;
		}
		calc.calcbuf[len-1] = '\0';
		swscanf(calc.calcbuf, L"%f", &val);
		calc_setValue(val);
	}
}

void calc_bumpDecimalPoint(void)
{	if (calc.displayMode == D_DECIMAL)
		calc.floatingPointLocation++;
}




TCHAR *calc_getString(void)
{	
	switch(calc.displayMode)
	{
		case D_DECIMAL:
		{
			if (calc.floatingPointLocation == 0)
				swprintf(calc.calcbuf, L"%.16g", calc.value);
			else
			{								// handle round-off error by limiting precision.
				TCHAR *fmt = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
				fmt[0] = '%';
				if (calc.floatingPointLocation+calc.integerPlaces < 10)		// e.g. L"%5.5g"
				{	fmt[1] = '0'+calc.floatingPointLocation+calc.integerPlaces;
					fmt[2] = '.';
					fmt[3] = '0'+calc.floatingPointLocation+calc.integerPlaces;
					fmt[4] = 'g';
				}
				else														// e.g. L"%12.12g"
				{	fmt[1] = '.';
					fmt[2] = '1';
					fmt[3] = '0'+calc.floatingPointLocation+calc.integerPlaces-10;
					fmt[4] = 'g';
				}
				swprintf(calc.calcbuf, fmt, calc.value);

				LocalFree(fmt);
			}
		}


			break;

		case D_BINARY:
		{	
			TCHAR *binstr = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
			int val = (int) calc.value;
			int index = 0;
			while (val > 0)
			{	if (val % 2)
					binstr[index] = '1';
				else
					binstr[index] = '0';
				index++;
				val /= 2;
			}
			
			wcscpy(calc.calcbuf, binstr);
			wcsrev(calc.calcbuf);
			LocalFree(binstr);
		}
			break;
		case D_OCTAL:
			swprintf(calc.calcbuf, L"%o", (int) calc.value);
			calc.value = (double) (int) calc.value;
			break;
		case D_HEX:
			swprintf(calc.calcbuf, L"%x", (int) calc.value);
			calc.value = (double) (int) calc.value;
			break;
	}
	return calc.calcbuf;
}


double calc_getValue(void)
{	return calc.value;
}

void calc_setValue(double value)
{   calc.value = value;
	calc.allowAddDigits = FALSE;
}



void calc_perform(void)            // called to perform a binomial operation (+ - * /)
{   
	switch (calc.op)
	{	case ADD_OP:
			calc.holding += calc.value;
			break;
		case SUB_OP:
			calc.holding -= calc.value;
			break;
		case MUL_OP:
			calc.holding *= calc.value;
			break;
		case DIV_OP:
			if (calc.value > 0)
				calc.holding /= calc.value;
			break;
		case AND_OP:
			calc.holding = (int) calc.holding & (int) calc.value;
			break;
		case OR_OP:
			calc.holding = (int) calc.holding | (int) calc.value;
			break;
		case XOR_OP:
			calc.holding = (int) calc.holding ^ (int) calc.value;
			break;
		case MOD_OP:
			calc.holding = (int) calc.holding % (int) calc.value;
		default:
			break;
	}
	calc.floatingPointLocation = 0;
	calc.integerPlaces = 0;
}

void calc_clear(void)
{	calc.value = 0.0;
    calc.holding = 0.0;
	calc.op = NON_OP;
	calc.floatingPointLocation = 0;
	calc.integerPlaces = 0;
	calc.allowAddDigits = TRUE;			// paranoia.
}

void calc_ce(void)
{	calc.value = 0.0;
    calc.holding = 0.0;
	calc.op = NON_OP;
	calc.floatingPointLocation = 0;
	calc.integerPlaces = 0;
	calc.allowAddDigits = TRUE;
}

void calc_add(void)
{	calc_perform();
	calc.holding = calc.value;
    calc.op = ADD_OP;
    calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_subtract(void)
{   calc_perform();
	calc.holding = calc.value;
    calc.op = SUB_OP;
    calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_multiply(void)
{	calc_perform();
	calc.holding = calc.value;
    calc.op = MUL_OP;
    calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_divide(void)
{	calc_perform();
	calc.holding = calc.value;
    calc.op = DIV_OP;
    calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_and(void)
{	calc_perform();
	calc.holding = calc.value;
	calc.op = AND_OP;
	calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_or(void)
{	calc_perform();
	calc.holding = calc.value;
	calc.op = OR_OP;
	calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_xor(void)
{	calc_perform();
	calc.holding = calc.value;
	calc.op = XOR_OP;
	calc.value = 0;
	calc.allowAddDigits = FALSE;
}

void calc_mod(void)
{	calc_perform();
	calc.holding = calc.value;
	calc.op = MOD_OP;
	calc.value = 0;
	calc.allowAddDigits = FALSE;
}


double calc_equals(void)
{   if (calc.op != NON_OP)
	{   calc_perform();
		calc.value = calc.holding;
		calc.holding = 0;
		calc.op = NON_OP;
	}
	calc.allowAddDigits = FALSE;
    return calc.value;
}

void calc_sin(void)
{	calc.value = sin(calc.value);
    calc.allowAddDigits = FALSE;
}

void calc_cos(void)
{	calc.value = cos(calc.value);
    calc.allowAddDigits = FALSE;
}

void calc_tan(void)
{	calc.value = tan(calc.value);
    calc.allowAddDigits = FALSE;
}

void calc_asin(void)
{	calc.value = asin(calc.value);
    calc.allowAddDigits = FALSE;
}

void calc_acos(void)
{	calc.value = acos(calc.value);
    calc.allowAddDigits = FALSE;
}

void calc_atan(void)
{	calc.value = atan(calc.value);
    calc.allowAddDigits = FALSE;
}

void calc_sqrt(void)
{	calc.value = sqrt(calc.value);
	calc.allowAddDigits = FALSE;
}


void    calc_memplus(void)
{   calc_perform();
    calc.memval += calc.value;
}

void    calc_memminus(void)
{	calc_perform();
    calc.memval -= calc.value;
}

void    calc_memretrieve(void)
{	calc.value = calc.memval;
}

void    calc_memclear(void)
{   calc.memval = 0;
}

void	calc_plusminus(void)
{	calc.value =- calc.value;
	calc.allowAddDigits = FALSE;
}

void    calc_displayBinary(void)
{   calc.displayMode = D_BINARY;
	calc.allowAddDigits = FALSE;
}

void    calc_displayDecimal(void)
{	calc.displayMode = D_DECIMAL;
	calc.allowAddDigits = FALSE;
}

void    calc_displayHex(void)
{	calc.displayMode = D_HEX;
	calc.allowAddDigits = FALSE;
}

void    calc_displayOctal(void)
{	calc.displayMode = D_OCTAL;
	calc.allowAddDigits = FALSE;
}

void	calc_leftShift(void)
{	calc.value = (int) calc.value << 1;
	calc.allowAddDigits = FALSE;
}

void	calc_rightShift(void)
{	calc.value = (int) calc.value >> 1;
	calc.allowAddDigits = FALSE;
}
