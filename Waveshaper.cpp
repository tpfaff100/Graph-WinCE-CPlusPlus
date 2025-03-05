#include "Waveshaper.h"
#include "SndTools.h"

// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle

#define ID_TIMER WM_USER + 300

#define SM				180				// middle of Waveshaper screen horiz axis.
#define OSCILLATORCOUNT	8
#define BANKCOUNT		2
#define PRESCOUNT		10

float wav_res = (float) .3;             // resolution of graph
static float wav_xoff = 0;				// x offset at which to draw waveform.

static wavedata wd[BANKCOUNT][OSCILLATORCOUNT];	// define all oscillators for a specific bank.
static bankdata bd[BANKCOUNT];			// define bank-specific data.

static wavedata wp[PRESCOUNT][BANKCOUNT][OSCILLATORCOUNT];	// wave presets.
static bankdata bp[PRESCOUNT][BANKCOUNT];					// algorighm complement to wave presets.

int currentChannel = 0;					// currently assigned oscillator.
int currentBank = 0;					// currently assigned bank of oscillators.
int waveVolume = 0;						// 0=soft, 1=medium, 2=loud volume for sound playback.
BOOL enableXvsYMode = FALSE;
BOOL forceSOM = FALSE;					// force single oscillator mode for easy toggling.


char *plotdata = NULL;
int   plotdatalen;

// Mesage handler for the Loan Amortization dialog.
LRESULT CALLBACK Waveshape(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	RECT rcClient;
	int count;

	switch (message)
	{	
		case WM_INITDIALOG:
			SHMENUBARINFO mbi;      
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_WAVESHAPE_MENU;
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
//			SetTimer(hDlg, ID_TIMER, 350, NULL);
			currentChannel = 0;

			ui_resetComboBox(hDlg, IDC_WAVEAMPLITUDE);
			ui_resetComboBox(hDlg, IDC_WAVECYCLES);
			ui_resetComboBox(hDlg, IDC_WAVEPHASE);
			ui_resetComboBox(hDlg, IDC_WAVERES);
			ui_resetComboBox(hDlg, IDC_WAVEOPCHOICE);

			ui_addToComboBox(hDlg, IDC_WAVECYCLES, (float) 0.25, 1);
			ui_addToComboBox(hDlg, IDC_WAVECYCLES, (float) 0.50, 1);
			ui_addToComboBox(hDlg, IDC_WAVECYCLES, (float) 0.75, 1);
			for (count = 1; count < 50; count++)
				ui_addToComboBox(hDlg, IDC_WAVECYCLES, (float) (count), 0);
			for (count = 50; count <= 100; count+=5)
				ui_addToComboBox(hDlg, IDC_WAVECYCLES, (float) (count), 0);

			for (count = 0; count <= 100; count++)
				ui_addToComboBox(hDlg, IDC_WAVEPHASE, ((float) count/ (float)100.0), 1);

			for (count = 0; count <= 20; count++)
			{	ui_addToComboBox(hDlg, IDC_WAVEAMPLITUDE, (float) (count*10), 0);

				if (count == 0)
				{	ui_addToComboBox(hDlg, IDC_WAVERES, (float) 0.005, 2);
					ui_addToComboBox(hDlg, IDC_WAVERES, (float) 0.01, 2);
					ui_addToComboBox(hDlg, IDC_WAVERES, (float) 0.025, 2);
					ui_addToComboBox(hDlg, IDC_WAVERES, (float) 0.05, 2);
				}
				else
					ui_addToComboBox(hDlg, IDC_WAVERES, ((float) count)/ (float)10.0, 2);
			}

			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"single oscillator mode (SOM)");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"show all oscillators");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"add all oscillators");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2+O3");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2+O3+04");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2+O3+04+O5");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2+O3+04+O5+O6");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2+O3+04+O5+O6+O7");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2+O3+04+O5+O6+O7+O8");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2-O3");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1+O2-O3-O4");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2*O3");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2*O3*O4");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2*O3*O4*O5");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2*O3*O4*O5*O6");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2*O3*O4*O5*O6*O7");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2*O3*O4*O5*O6*O7*O8");
			ui_addToComboBoxStr(hDlg, IDC_WAVEOPCHOICE, L"O1*O2+O3+O4");

			ui_setComboIndex(hDlg, IDC_WAVEOPCHOICE, bd[currentBank].blendAlgorithm);

			resetUI(hDlg);

			plotdata = (char *) malloc(100000);
			ZeroMemory(plotdata, sizeof(100000));
			initWavePresets();
			return TRUE;
			break;


		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
            switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_WAVESHAPE_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;

				case IDM_WAVEBANKX:
					enableXvsYMode = FALSE;
					bd[currentBank].blendAlgorithm = ui_getComboIndex(hDlg, IDC_WAVEOPCHOICE);
					currentBank = 0;
					SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"Bank X");
					restoreUI(hDlg);
					break;

				case IDM_WAVEBANKY:
					enableXvsYMode = FALSE;
					bd[currentBank].blendAlgorithm = ui_getComboIndex(hDlg, IDC_WAVEOPCHOICE);
					currentBank = 1;
					SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"Bank Y");
					restoreUI(hDlg);
					break;

				case IDM_WAVEXVSY:			// plot bank x vs bank y on their respective axes.
					enableXvsYMode = TRUE;					
					SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"X vs Y");
					restoreUI(hDlg);
					Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESOM), BST_INDETERMINATE);
					break;

				case IDC_WAVEBANKTEXT:
					if (enableXvsYMode == TRUE)		// transition from x vs y mode to Bank X
					{	
						enableXvsYMode = FALSE;
						bd[currentBank].blendAlgorithm = ui_getComboIndex(hDlg, IDC_WAVEOPCHOICE);
						currentBank = 0;
						SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"Bank X");
//						ui_setComboIndex(hDlg, IDC_WAVEOPCHOICE, bd[currentBank].blendAlgorithm);
						restoreUI(hDlg);
					}
					else
					if (currentBank == 0)			// transition from Bank X to Bank Y
					{	bd[currentBank].blendAlgorithm = ui_getComboIndex(hDlg, IDC_WAVEOPCHOICE);
						currentBank = 1;
						SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"Bank Y");
//						ui_setComboIndex(hDlg, IDC_WAVEOPCHOICE, bd[currentBank].blendAlgorithm);
						restoreUI(hDlg);
					}
					else
					if (currentBank == 1)			// transition from Bank Y to x vs y
					{	enableXvsYMode = TRUE;					
						SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"X vs Y");
						restoreUI(hDlg);
					}
					break;


				case IDM_WAVESHAPERESET:
					resetUI(hDlg);
					ZeroMemory(plotdata, sizeof(100000));
					break;

				case IDM_WAVEVOLSOFT:
				case IDM_WAVEVOLMED:
				case IDM_WAVEVOLLOUD:
					waveVolume = IDM_WAVEVOLSOFT - wmId;
					break;

				case IDM_WAVEPLAYXY:
					wave_play(plotdata, plotdatalen, waveVolume, 10);
					break;

				case IDM_WAVEPREF001:
				case IDM_WAVEPREF002:
				case IDM_WAVEPREF003:
				case IDM_WAVEPREF004:
				case IDM_WAVEPREF005:
				case IDM_WAVEPREF006:
				case IDM_WAVEPREF007:
				case IDM_WAVEPREF008:
				case IDM_WAVEPREF009:
					useWavePreset(hDlg, wmId-IDM_WAVEPREF001);	// choose waveform preset.
					enableXvsYMode = TRUE;					
					SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"X vs Y");
					restoreUI(hDlg);
					break;

				case IDC_WAVESIN:
					wd[currentBank][currentChannel].sinSwitch = !wd[currentBank][currentChannel].sinSwitch;
					ui_repaintWindow(hDlg);
					break;
				case IDC_WAVECOS:
					wd[currentBank][currentChannel].cosSwitch = !wd[currentBank][currentChannel].cosSwitch;
					ui_repaintWindow(hDlg);
					break;
				case IDC_WAVETAN:
					wd[currentBank][currentChannel].tanSwitch = !wd[currentBank][currentChannel].tanSwitch;
					ui_repaintWindow(hDlg);
					break;
				case IDC_WAVESSQU:
					wd[currentBank][currentChannel].squareSinSwitch = !wd[currentBank][currentChannel].squareSinSwitch;
					ui_repaintWindow(hDlg);
					break;
				case IDC_WAVECSQU:
					wd[currentBank][currentChannel].squareCosSwitch = !wd[currentBank][currentChannel].squareCosSwitch;
					ui_repaintWindow(hDlg);
					break;
				case IDC_WAVETANLIM:
					wd[currentBank][currentChannel].limTanSwitch = !wd[currentBank][currentChannel].limTanSwitch;
					ui_repaintWindow(hDlg);
					break;
				case IDC_WAVETRI:
					wd[currentBank][currentChannel].triSwitch = !wd[currentBank][currentChannel].triSwitch;
					ui_repaintWindow(hDlg);
					break;

				case IDC_WAVESOM:
					if (!enableXvsYMode)
					{	forceSOM = !forceSOM;
						ui_repaintWindow(hDlg);
					}
					else
						ui_repaintWindow(hDlg);
					break;

				case IDC_WAVBANK1:
				case IDC_WAVBANK2:
				case IDC_WAVBANK3:
				case IDC_WAVBANK4:
				case IDC_WAVBANK5:
				case IDC_WAVBANK6:
				case IDC_WAVBANK7:
				case IDC_WAVBANK8:
					currentChannel = wmId - IDC_WAVBANK1;
					restoreUI(hDlg);				// restore all button/checkbox/etc settings.
					break;

				default:
					break;
			}

			switch(wmEvent)
			{   
				case CBN_SELCHANGE:			// triggered at end of combo-box selection.
				{
					TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
					HWND hwComboBox;
					hwComboBox = GetDlgItem(hDlg, IDC_WAVEAMPLITUDE);
					if (wmId == IDC_WAVEAMPLITUDE)
						ComboBox_SelectString(hwComboBox, 10, NULL);
					ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &wd[currentBank][currentChannel].amplitude);

					hwComboBox = GetDlgItem(hDlg, IDC_WAVECYCLES);
					if (wmId == IDC_WAVECYCLES)
						ComboBox_SelectString(hwComboBox, 10, NULL);
					ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &wd[currentBank][currentChannel].cycles);

					hwComboBox = GetDlgItem(hDlg, IDC_WAVEPHASE);
					if (wmId == IDC_WAVEPHASE)
						ComboBox_SelectString(hwComboBox, 10, NULL);
					ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &wd[currentBank][currentChannel].phase);

					hwComboBox = GetDlgItem(hDlg, IDC_WAVERES);
					if (wmId == IDC_WAVERES)
					{	ComboBox_SelectString(hwComboBox, 10, NULL);
						ZeroMemory(plotdata, sizeof(100000)); // plot data is trash if resolution changes.
					}

					ComboBox_GetText(hwComboBox, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &wav_res);
					LocalFree(buf);
					ui_repaintWindow(hDlg);
				}
					break;

				case CBN_EDITCHANGE:		// triggered at end of event-processing of keypress.
				{   float amplitude, cycles, phase;
					TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);

					GetDlgItemTextW(hDlg, IDC_WAVEAMPLITUDE, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &amplitude);
					wd[currentBank][currentChannel].amplitude = amplitude;
					GetDlgItemTextW(hDlg, IDC_WAVECYCLES, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &cycles);
					wd[currentBank][currentChannel].cycles = cycles;
					GetDlgItemTextW(hDlg, IDC_WAVEPHASE, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &phase);
					wd[currentBank][currentChannel].phase = phase;	
					GetDlgItemTextW(hDlg, IDC_WAVERES, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &wav_res);
					
					if (wav_res <= (float) .05)
						wav_res = (float) .05;
					if (cycles < (float) .15)
						cycles = (float) .15;
					if (amplitude < 1)
						amplitude = 1;

					LocalFree(buf);
					ui_repaintWindow(hDlg);
					return TRUE;
				}
					break;
			}
				break;

		case WM_PAINT:
		{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
			int waveCombineChoice = ui_getComboIndex(hDlg, IDC_WAVEOPCHOICE);
			bd[currentBank].blendAlgorithm = ui_getComboIndex(hDlg, IDC_WAVEOPCHOICE);

		    GetClientRect (hDlg, &rcClient);
			hdc = BeginPaint (hDlg, &ps);

			if (enableXvsYMode)
				paintXvsY(hdc);
			else
			{   if (bd[currentBank].blendAlgorithm == 0 || forceSOM)
					paintOneOscillator(hdc, currentChannel);
				else
				if (bd[currentBank].blendAlgorithm == 1)
					paintAllOscillators(hdc);
				else
					paintBlendedOscillators(hdc, waveCombineChoice);
			}

			EndPaint(hDlg, &ps);
			LocalFree(buf);
		}
			break;

		case WM_TIMER:
			ui_repaintWindow(hDlg);
			break;

		case WM_DESTROY:
//			KillTimer(hDlg, ID_TIMER);
			if (plotdata != NULL)
				free(plotdata);
			break;

		default:
			break;
	}
	return FALSE;
}

void restoreUI(HWND hDlg)			// call this after changing bank -> requiring display update.
{
	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
	HWND hwComboBox;

	hwComboBox = GetDlgItem(hDlg, IDC_WAVEAMPLITUDE);
	swprintf(buf, L"%f", wd[currentBank][currentChannel].amplitude);
	ComboBox_SetText(hwComboBox, buf);
	SetDlgItemTextW(hDlg, IDC_WAVEAMPLITUDE, buf);

	hwComboBox = GetDlgItem(hDlg, IDC_WAVECYCLES);
	swprintf(buf, L"%f", wd[currentBank][currentChannel].cycles);
	ComboBox_SetText(hwComboBox, buf);
	SetDlgItemTextW(hDlg, IDC_WAVECYCLES, buf);

	hwComboBox = GetDlgItem(hDlg, IDC_WAVEPHASE);
	swprintf(buf, L"%f", wd[currentBank][currentChannel].phase);
	ComboBox_SetText(hwComboBox, buf);
	SetDlgItemTextW(hDlg, IDC_WAVEPHASE, buf);

	hwComboBox = GetDlgItem(hDlg, IDC_WAVERES);
	swprintf(buf, L"%f", wav_res);
	ComboBox_SetText(hwComboBox, buf);
	SetDlgItemTextW(hDlg, IDC_WAVERES, buf);
	
	ui_setComboIndex(hDlg, IDC_WAVEOPCHOICE, bd[currentBank].blendAlgorithm);
	
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESIN), wd[currentBank][currentChannel].sinSwitch);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVECOS), wd[currentBank][currentChannel].cosSwitch);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVETAN), wd[currentBank][currentChannel].tanSwitch);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESSQU), wd[currentBank][currentChannel].squareSinSwitch);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVECSQU), wd[currentBank][currentChannel].squareCosSwitch);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVETANLIM), wd[currentBank][currentChannel].limTanSwitch);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVETRI), wd[currentBank][currentChannel].triSwitch);
	ui_repaintWindow(hDlg);
	LocalFree(buf);
}


void resetSettings(void)
{   int count, bcount;
	currentBank = 0;
	currentChannel = 0;
	enableXvsYMode = FALSE;
	for (bcount = 0; bcount < BANKCOUNT; bcount++)
	{
		for (count = 0; count < OSCILLATORCOUNT; count++)		// init oscillator values.
		{   wd[bcount][count].sinSwitch = FALSE;
			wd[bcount][count].cosSwitch = FALSE;
			wd[bcount][count].tanSwitch = FALSE;
			wd[bcount][count].squareSinSwitch = FALSE;
			wd[bcount][count].squareCosSwitch = FALSE;
			wd[bcount][count].limTanSwitch = FALSE;
			wd[bcount][count].triSwitch = FALSE;

			wd[bcount][count].amplitude = 50.0;
			wd[bcount][count].cycles = 1.0;
			wd[bcount][count].phase = 0.0;
		}
		bd[bcount].blendAlgorithm = 0;
	}
}

void resetPresets(void)
{   int count, bcount, ccount;;
	currentBank = 0;
	currentChannel = 0;
	enableXvsYMode = FALSE;
	for (ccount = 0; ccount < PRESCOUNT; ccount++)	// reset presets
	{   for (bcount = 0; bcount < BANKCOUNT; bcount++)
		{   for (count = 0; count < OSCILLATORCOUNT; count++)		// init oscillator values.
			{   wp[ccount][bcount][count].sinSwitch = FALSE;
				wp[ccount][bcount][count].cosSwitch = FALSE;
				wp[ccount][bcount][count].tanSwitch = FALSE;
				wp[ccount][bcount][count].squareSinSwitch = FALSE;
				wp[ccount][bcount][count].squareCosSwitch = FALSE;
				wp[ccount][bcount][count].limTanSwitch = FALSE;
				wp[ccount][bcount][count].triSwitch = FALSE;

				wp[ccount][bcount][count].amplitude = 50.0;
				wp[ccount][bcount][count].cycles = 1.0;
				wp[ccount][bcount][count].phase = 0.0;
			}
			bp[count][bcount].blendAlgorithm = 0;
		}
	}

}

void resetUI(HWND hDlg)
{	resetSettings();

	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESIN), FALSE);	// turn SIN on by default.
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESIN), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVECOS), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVETAN), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESSQU), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVECSQU), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVETANLIM), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVETRI), FALSE);
	Button_SetCheck(GetDlgItem(hDlg,IDC_WAVESOM), FALSE);

	ui_setComboIndex(hDlg, IDC_WAVEAMPLITUDE, 5);
	ui_setComboIndex(hDlg, IDC_WAVECYCLES, 3);
	ui_setComboIndex(hDlg, IDC_WAVEPHASE, 0);
	ui_setComboIndex(hDlg, IDC_WAVERES, 4);

	SetDlgItemTextW(hDlg, IDC_WAVEBANKTEXT, L"Bank X");

	ui_repaintWindow(hDlg);
}



void paintOneOscillator(HDC hdc, int index)
{
	float count;
	float PI = (float) 3.14159265358979*2;
	float divisor = SCREEN_WIDTH/PI;	// fits PI to the device display width.
	int yout;

	for (count = 0; count < SCREEN_WIDTH; count+=wav_res)
	{   if ((int) count % 5 == 0)				// draw a median line.
			SetPixel(hdc, (int) count, SM, 5);

		if(wd[currentBank][index].sinSwitch)
		{	yout = (int) (sin(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);
			SetPixel(hdc, (int) count, SM-yout, 5);
		}
		if(wd[currentBank][index].cosSwitch)
		{	yout = (int) (cos(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);
			SetPixel(hdc, (int) count, SM-yout, 5);
		}
		if(wd[currentBank][index].tanSwitch)
		{	yout = (int) (tan(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);
			SetPixel(hdc, (int) count, SM-yout, 5);
		}
		if (wd[currentBank][index].squareSinSwitch)
		{   double val = (int)(sin(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);
			if (val < 0)
				yout = (int) -wd[currentBank][index].amplitude;
			else
				yout = (int) wd[currentBank][index].amplitude;
			SetPixel(hdc, (int) count, SM-yout, 5);
		}
		if (wd[currentBank][index].squareCosSwitch)
		{   double val = (int)(cos(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);
			if (val < 0)
				yout = (int) -wd[currentBank][index].amplitude;
			else
				yout = (int) wd[currentBank][index].amplitude;
			SetPixel(hdc, (int) count, SM-yout, 5);
		}
		if (wd[currentBank][index].limTanSwitch)
		{   double lim = wd[currentBank][index].amplitude;
			double val = (int)(tan(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);
			if (val > lim || val < -lim)
				yout = 0;
			else
				yout = (int) (tan(wd[currentBank][index].phase*PI+count*wd[currentBank][index].cycles/divisor)*wd[currentBank][index].amplitude);;
			SetPixel(hdc, (int) count, SM-yout, 5);
		}
	}
}


void paintAllOscillators(HDC hdc)
{	int count;
	for (count = 0; count < OSCILLATORCOUNT; count++)
		paintOneOscillator(hdc, count);
}


void paintBlendedOscillators(HDC hdc, int blendAlgorithm)	// blend oscillators using specified algorithm.
{
	float count;
	int osccount;
	float PI = (float) 3.14159265358979*2;
	float divisor = SCREEN_WIDTH/PI;	// fits PI to the device display width.
	float y[OSCILLATORCOUNT];
	int yout;
	int index = 0;

	for (count = 0; count < SCREEN_WIDTH; count+=wav_res)
	{   if ((int) count % 5 == 0)				// draw a median line.
			SetPixel(hdc, (int) count, SM, 5);

		for (osccount=0; osccount< OSCILLATORCOUNT; osccount++)
			y[osccount] = 0;

	    for (osccount=0; osccount< OSCILLATORCOUNT; osccount++)
		{   
			if(wd[currentBank][osccount].sinSwitch)
				y[osccount] += (float) (sin(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);
			if(wd[currentBank][osccount].cosSwitch)
				y[osccount] += (float) (cos(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);
			if(wd[currentBank][osccount].tanSwitch)
				y[osccount] += (float) (tan(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);
			if (wd[currentBank][osccount].squareSinSwitch)
			{   double val = (int)(sin(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);
				if (val < 0)
					y[osccount] -= (float) wd[currentBank][osccount].amplitude;
				else
					y[osccount] += (float) wd[currentBank][osccount].amplitude;
			}
			if (wd[currentBank][osccount].squareCosSwitch)
			{   double val = (int)(cos(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);
				if (val < 0)
					y[osccount] -= (float) wd[currentBank][osccount].amplitude;
				else
					y[osccount] += (float) wd[currentBank][osccount].amplitude;
			}
			if (wd[currentBank][osccount].limTanSwitch)
			{   double lim = wd[currentBank][osccount].amplitude;
				double val = (int)(tan(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);
				if (val > lim || val < -lim)
					y[osccount] += 0;
				else
					y[osccount] += (int) (tan(wd[currentBank][osccount].phase*PI+count*wd[currentBank][osccount].cycles/divisor)*wd[currentBank][osccount].amplitude);;
			
			}
		}

		switch(blendAlgorithm)
		{
			case 2:
				yout = (int) (y[0]+y[1]+y[2]+y[3]+y[4]+y[5]+y[6]+y[7])/8;
				break;
			case 3:
				yout = (int) y[0];
				break;
			case 4:
				yout = (int) (y[0]+y[1])/2;
				break;
			case 5:
				yout = (int) (y[0]+y[1]+y[2])/3;
				break;
			case 6:
				yout = (int) (y[0]+y[1]+y[2]+y[3])/4;
				break;
			case 7:
				yout = (int) (y[0]+y[1]+y[2]+y[3]+y[4])/5;
				break;
			case 8:
				yout = (int) (y[0]+y[1]+y[2]+y[3]+y[4]+y[5])/6;
				break;
			case 9:
				yout = (int) (y[0]+y[1]+y[2]+y[3]+y[4]+y[5]+y[6])/7;
				break;
			case 10:
				yout = (int) (y[0]+y[1]+y[2]+y[3]+y[4]+y[5]+y[6]+y[7])/8;
				break;
			case 11:
				yout = (int) (y[0]+y[1]-y[2])/3;
				break;
			case 12:
				yout = (int) (y[0]+y[1]-y[2]-y[3])/3;
				break;
			case 13:
				yout = (int) (y[0]*y[1])/30;
				break;
			case 14:
				yout = (int) (y[0]*y[1]*y[2])/3000;
				break;
			case 15:
				yout = (int) (y[0]*y[1]*y[2]*y[3])/100000;
				break;
			case 16:
				yout = (int) (y[0]*y[1]*y[2]*y[3]*y[4])/4000000;
				break;
			case 17:
				yout = (int) (y[0]/2*y[1]/2*y[2]/2*y[3]/2*y[4]/2*y[5]/2)/4000000;
				break;
			case 18:
				yout = (int) (y[0]/3*y[1]/3*y[2]/3*y[3]/3*y[4]/3*y[5]/3*y[6]/3)/5000000;
				break;
			case 19:
				yout = (int) (y[0]/4*y[1]/4*y[2]/4*y[3]/4*y[4]/4*y[5]/4*y[6]/4*y[7]/4)/5000000;
				break;
			case 20:
				yout = (int) (y[0]*y[1]+y[2]+y[3])/40;
				break;
			default:
				break;
		}
		SetPixel(hdc, (int) count, SM-yout, 5);
		if (yout <= 127 || yout >= -127)			// place x and y data into 
			plotdata[index*2+currentBank] = yout;	// appropriate sound channels.
		else
			plotdata[index*2+currentBank] = 0;
		index++;

	}
	plotdatalen = index * 2;
}





void paintXvsY(HDC hdc)
{
	float count;
	int bcount;
	int osccount;
	float PI = (float) 3.14159265358979*2;
	float divisor = SCREEN_WIDTH/PI;	// fits PI to the device display width.
	float y[OSCILLATORCOUNT];
	int x1,y1;
	float yout;
	int index = 0;

	for (count = 0; count < SCREEN_WIDTH; count+=wav_res)
	{   x1 = 0;
		y1 = 0;

		for (bcount = 0; bcount < 2; bcount++)
		{
			for (osccount=0; osccount< OSCILLATORCOUNT; osccount++)
				y[osccount] = 0;

			for (osccount=0; osccount< OSCILLATORCOUNT; osccount++)
			{   
				if(wd[bcount][osccount].sinSwitch)
					y[osccount] += (float) (sin(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);
				if(wd[bcount][osccount].cosSwitch)
					y[osccount] += (float) (cos(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);
				if(wd[bcount][osccount].tanSwitch)
					y[osccount] += (float) (tan(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);
				if (wd[bcount][osccount].squareSinSwitch)
				{   double val = (int)(sin(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);
					if (val < 0)
						y[osccount] -= (float) wd[bcount][osccount].amplitude;
					else
						y[osccount] += (float) wd[bcount][osccount].amplitude;
				}
				if (wd[bcount][osccount].squareCosSwitch)
				{   double val = (int)(cos(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);
					if (val < 0)
						y[osccount] -= (float) wd[bcount][osccount].amplitude;
					else
						y[osccount] += (float) wd[bcount][osccount].amplitude;
				}
				if (wd[bcount][osccount].limTanSwitch)
				{   double lim = wd[bcount][osccount].amplitude;
					double val = (int)(tan(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);
					if (val > lim || val < -lim)
						y[osccount] += 0;
					else
						y[osccount] += (float) (tan(wd[bcount][osccount].phase*PI+count*wd[bcount][osccount].cycles/divisor)*wd[bcount][osccount].amplitude);				}


			}

			switch(bd[bcount].blendAlgorithm)
			{
				case 0:
				case 1:
				case 3:
					yout = y[0];
					break;
				case 2:
					yout = (y[0]+y[1]+y[2]+y[3]+y[4]+y[5]+y[6]+y[7])/8;
					break;
				case 4:
					yout = (y[0]+y[1])/2;
					break;
				case 5:
					yout = (y[0]+y[1]+y[2])/3;
					break;
				case 6:
					yout = (y[0]+y[1]+y[2]+y[3])/4;
					break;
				case 7:
					yout = (y[0]+y[1]+y[2]+y[3]+y[4])/5;
					break;
				case 8:
					yout = (y[0]+y[1]+y[2]+y[3]+y[4]+y[5])/6;
					break;
				case 9:
					yout = (y[0]+y[1]+y[2]+y[3]+y[4]+y[5]+y[6])/7;
					break;
				case 10:
					yout = (y[0]+y[1]+y[2]+y[3]+y[4]+y[5]+y[6]+y[7])/8;
					break;
				case 11:
					yout = (y[0]+y[1]-y[2])/3;
					break;
				case 12:
					yout = (y[0]+y[1]-y[2]-y[3])/3;
					break;
				case 13:
					yout = (y[0]*y[1])/30;
					break;
				case 14:
					yout = (y[0]*y[1]*y[2])/3000;
					break;
				case 15:
					yout = (y[0]*y[1]*y[2]*y[3])/100000;
					break;
				case 16:
					yout = (y[0]*y[1]*y[2]*y[3]*y[4])/4000000;
					break;
				case 17:
					yout = (y[0]/2*y[1]/2*y[2]/2*y[3]/2*y[4]/2*y[5]/2)/4000000;
					break;
				case 18:
					yout = (y[0]/3*y[1]/3*y[2]/3*y[3]/3*y[4]/3*y[5]/3*y[6]/3)/5000000;
					break;
				case 19:
					yout = (y[0]/4*y[1]/4*y[2]/4*y[3]/4*y[4]/4*y[5]/4*y[6]/4*y[7]/4)/5000000;
					break;
				case 20:
					yout = (y[0]*y[1]+y[2]+y[3])/40;
					break;
				default:
					break;
			}
			
			if (bcount == 0)
				x1 = (int) (yout*1.5);
			else
				y1 = (int) (yout*1.5);
		}
//		SetPixel(hdc, count, SM-(int)x1-50, 5);
//		SetPixel(hdc, count, SM-(int)y1+50, 5);
		SetPixel(hdc, 120+x1, SM-y1, 5);
				
		if (x1 <= 127 || x1 >= -127)			// place x and y data into 
			plotdata[index*2] = x1;	// appropriate sound channels.
		else
			plotdata[index*2] = 0;

		if (y1 <= 127 || y1 >= -127)
			plotdata[index*2+1] = y1;
		else
			plotdata[index*2+1] = 0;

		index++;

	}
	plotdatalen = index*2;			// store total length of array (with both oscillations)
}


void initWavePresets(void)
{	int presetno, bank, oscillator;
	resetPresets();	

	presetno = 0;
	bank = 0;
	oscillator = 0;			// circle object.
	wp[presetno][bank][oscillator].amplitude = 50;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 3;
	bp[presetno][bank].favoriteResolution = .5;// only necessary to set first bank (bank x).
	bank = 1;
	wp[presetno][bank][oscillator].amplitude = 50;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 3;


///////////////////////////////////////////////////////////
	presetno = 1;			// triangle.
	bank = 0;
	oscillator = 0;		
	wp[presetno][bank][oscillator].amplitude = 30;
	wp[presetno][bank][oscillator].cycles = 2;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 100;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	bp[presetno][bank].favoriteResolution = (float).3;// only necessary to set first bank (bank x).

	bank = 1;
	oscillator = 0;
	wp[presetno][bank][oscillator].amplitude = 30;
	wp[presetno][bank][oscillator].cycles = 2;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 100;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	
///////////////////////////////////////////////////////////
	presetno = 2;			// box.
	bank = 0;
	oscillator = 0;		
	wp[presetno][bank][oscillator].amplitude = 15;
	wp[presetno][bank][oscillator].cycles = 3;
	wp[presetno][bank][oscillator].phase = .75;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	bp[presetno][bank].favoriteResolution = (float).3;// only necessary to set first bank (bank x).

	bank = 1;
	oscillator = 0;
	wp[presetno][bank][oscillator].amplitude = 15;
	wp[presetno][bank][oscillator].cycles = 3;
	wp[presetno][bank][oscillator].phase = .75;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	
///////////////////////////////////////////////////////////
	presetno = 3;			// circular s
	bank = 0;
	oscillator = 0;			// describe x waveform.
	wp[presetno][bank][oscillator].amplitude = 30;
	wp[presetno][bank][oscillator].cycles = 50;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 100;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	bp[presetno][bank].favoriteResolution = (float).05;// only necessary to set first bank (bank x)..

	bank = 1;
	oscillator = 0;			// describe x waveform.
	wp[presetno][bank][oscillator].amplitude = 30;
	wp[presetno][bank][oscillator].cycles = 50;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	wp[presetno][bank][oscillator].squareSinSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 100;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 5;

	///////////////////////////////////////////////////////////
	presetno = 4;			// Three Petal Flower.
	bank = 0;
	oscillator = 0;			// describe x waveform.
	wp[presetno][bank][oscillator].amplitude = 70;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;

	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 70;
	wp[presetno][bank][oscillator].cycles = 60;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	
	oscillator = 2;
	wp[presetno][bank][oscillator].amplitude = 50;
	wp[presetno][bank][oscillator].cycles = 57;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	
	bp[presetno][bank].blendAlgorithm = 5;
	bp[presetno][bank].favoriteResolution = (float) .025;// only necessary to set first bank (bank x).

	bank = 1;
	oscillator = 0;			// describe x waveform.
	wp[presetno][bank][oscillator].amplitude = 70;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;

	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 70;
	wp[presetno][bank][oscillator].cycles = 60;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	
	oscillator = 2;
	wp[presetno][bank][oscillator].amplitude = 50;
	wp[presetno][bank][oscillator].cycles = 57;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	
	bp[presetno][bank].blendAlgorithm = 5;

//////////////////////////////////////////////////////////
	presetno = 5;
	bank = 0;
	oscillator = 0;			// describe x waveform.
	wp[presetno][bank][oscillator].amplitude = 50;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	wp[presetno][bank][oscillator].tanSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 3;
	bp[presetno][bank].favoriteResolution = (float) .05;// only necessary to set first bank (bank x).
	bank = 1;				
	wp[presetno][bank][oscillator].amplitude = 20;
	wp[presetno][bank][oscillator].cycles = 35;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;

	oscillator = 1;			// describe y waveform.
	wp[presetno][bank][oscillator].amplitude = 20;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].tanSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 13;

//////////////////////////////////////////////////////////
	presetno = 6;			// circle of circles
	bank = 0;				// describe x waveform.
	oscillator = 0;			
	wp[presetno][bank][oscillator].amplitude = 10;
	wp[presetno][bank][oscillator].cycles = 6;
	wp[presetno][bank][oscillator].phase = .75;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 96;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	bp[presetno][bank].favoriteResolution = (float) .1;// only necessary to set first bank (bank x).

	bank = 1;			// describe y waveform.
	oscillator = 0;
	wp[presetno][bank][oscillator].amplitude = 10;
	wp[presetno][bank][oscillator].cycles = 6;
	wp[presetno][bank][oscillator].phase = .75;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 96;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
//////////////////////////////////////////////////////////
	presetno = 7;			// pear
	bank = 0;				// describe x waveform.
	oscillator = 0;			
	wp[presetno][bank][oscillator].amplitude = 10;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].phase = .75;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 96;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 4;
	bp[presetno][bank].favoriteResolution = (float) .1;// only necessary to set first bank (bank x).

	bank = 1;			// describe y waveform.
	oscillator = 0;
	wp[presetno][bank][oscillator].amplitude = 10;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].phase = .75;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 96;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 13;
//////////////////////////////////////////////////////////
	presetno = 8;			// circle cycle
	bank = 0;				// describe x waveform.
	oscillator = 0;			
	wp[presetno][bank][oscillator].amplitude = 10;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].phase = 0;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 96;
	wp[presetno][bank][oscillator].sinSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 13;
	bp[presetno][bank].favoriteResolution = (float) .1;// only necessary to set first bank (bank x).

	bank = 1;			// describe y waveform.
	oscillator = 0;
	wp[presetno][bank][oscillator].amplitude = 10;
	wp[presetno][bank][oscillator].cycles = 1;
	wp[presetno][bank][oscillator].phase = 0;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	oscillator = 1;
	wp[presetno][bank][oscillator].amplitude = 110;
	wp[presetno][bank][oscillator].cycles = 96;
	wp[presetno][bank][oscillator].cosSwitch = TRUE;
	bp[presetno][bank].blendAlgorithm = 13;
}

void useWavePreset(HWND hDlg, int index)
{
	memcpy(&wd, &wp[index], sizeof(wavedata)*BANKCOUNT*OSCILLATORCOUNT);
	memcpy(&bd, &bp[index], sizeof(bankdata)*BANKCOUNT);
	if (bp[index][0].favoriteResolution > 0.0)	// only check in 0th bank.
		wav_res = bp[index][0].favoriteResolution;
	else
		wav_res = (float) 0.025;
}

