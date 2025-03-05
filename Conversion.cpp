#include "Conversion.h"

#define AREASELECTION		0
#define LENGTHSELECTION		1
#define LIQUIDSELECTION		2
#define TEMPSELECTION		3
#define WEIGHTSELECTION		4

// AREA SELECTION OPTIONS
#define ACRES		0
#define SQINCHES	1
#define SQFEET		2
#define SQYARDS		3
#define SQMILES		4
#define HECTARES	5
#define SQCMS		6
#define SQMETERS	7
#define SQKMS		8		// square kilometers

// LENGTH SELECTION OPTIONS
#define MILES		0
#define YARDS		1
#define FEET		2
#define INCHES		3
#define METERS		4
#define KILOMETERS	5
#define DECIMETERS	6
#define CENTIMETERS	7
#define MILLIMETERS	8

// LIQUID SELECTION OPTIONS
#define GALLONS		0
#define QUARTS		1
#define PINTS		2
#define CUPS		3
#define OUNCES		4
#define UKOUNCES	5
#define TBSPS		6
#define TSPS		7
#define LITERS		8
#define MILLILITERS	9		// also known as CCs.

// TEMPERATURE SELECTION OPTIONS
#define FAHRENHEIT	0
#define CELSIUS		1
#define KELVIN		2

// WEIGHT SELECTION OPTIONS
#define POUNDS		0
#define WT_OUNCES	1
#define GRAMS		2
#define KILOGRAMS	3
#define	LONGTONS	4


// Global Variables:
extern HINSTANCE		hInst;			// The current instance
extern HWND				hwndCB;			// The command bar handle


// Mesage handler for the Unit Conversion dialog.
LRESULT CALLBACK ConversionView(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	int wmId, wmEvent;
	int count;

	switch (message)
	{	
		case WM_INITDIALOG:
			SHMENUBARINFO mbi;      
			memset(&mbi, 0, sizeof(SHMENUBARINFO));
			mbi.cbSize     = sizeof(SHMENUBARINFO);
			mbi.hwndParent = hDlg;
			mbi.nToolBarId = IDM_CONVERSION_MENU;
			mbi.hInstRes   = hInst;
			mbi.nBmpId     = 0;
			mbi.cBmpImages = 0;	
        
			if (!SHCreateMenuBar(&mbi))
				MessageBox(hDlg, L"SHCreateMenuBar Failed", L"Error", MB_OK);
			hwndCB = mbi.hwndMB;

			SHINITDLGINFO shidi;                 // WinCE3.0/ipaq UI-specific gunk.
			shidi.dwMask = SHIDIM_FLAGS;         // create a DONE button and size it.
			shidi.hDlg = hDlg;
			shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
			SHInitDialog(&shidi);   //initialzes the dialog based on the dwFlags parameter

			ui_resetComboBox(hDlg, IDC_CONV_UNITTYPE);	// define types of units to translate in ui.
			ui_addToComboBoxStr(hDlg, IDC_CONV_UNITTYPE, L"Area");
			ui_addToComboBoxStr(hDlg, IDC_CONV_UNITTYPE, L"Distance");
			ui_addToComboBoxStr(hDlg, IDC_CONV_UNITTYPE, L"Liquid");
			ui_addToComboBoxStr(hDlg, IDC_CONV_UNITTYPE, L"Temperature");
			ui_addToComboBoxStr(hDlg, IDC_CONV_UNITTYPE, L"Weight");

			ui_resetComboBox(hDlg, IDC_CONV_INPUT1);
			for(count = 0; count <= 100; count++)
			{	
				if (count % 5 == 0 || count < 10)
				{	ui_addToComboBox(hDlg, IDC_CONV_INPUT1, (float) count, 0);
					ui_addToComboBox(hDlg, IDC_CONV_INPUT2, (float) count, 0);
				}
			}
			ui_setComboIndex(hDlg, IDC_CONV_INPUT1, 1);				
			ui_setComboIndex(hDlg, IDC_CONV_INPUT2, 1);

			SHSipPreference(hDlg, SIP_UP);		// display the virtual keyboard.
			fillUnitsWidgets(hDlg, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE));
			updateUnitsDisplay(hDlg);
			return TRUE;
			break;

		case WM_SETFOCUS:
			SHSipPreference(hDlg, SIP_UP);
			break;
		case WM_KILLFOCUS:
			SHSipPreference(hDlg, SIP_DOWN);	// SIP_DOWN uses a timer - interferes with other window.
			break;

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
            switch(wmId)
			{   
			    case IDOK:
			    case IDCANCEL:
				case IDM_CONVERSION_EXIT:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
					break;

				case IDC_CONV_UNITTYPE:			// choose a mode of unit transfer.
					fillUnitsWidgets(hDlg, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE));
					updateUnitsDisplay(hDlg);
					break;

				default:
					break;
			}

			switch(wmEvent)
			{	
				case CBN_SELCHANGE:			// triggered at end of event-processing of Combobox.
				{   float displayedval;
					TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
					HWND hwComboBox1, hwComboBox2;

					switch(wmId)
					{	case IDC_CONV_INPUT1:				// recalc as a result of new input value.
							updateUnitsDisplay(hDlg);
/*							hwComboBox1 = GetDlgItem(hDlg, IDC_CONV_INPUT1);// combo box #1
							hwComboBox2 = GetDlgItem(hDlg, IDC_CONV_INPUT2);// combo box #2
							ComboBox_SelectString(hwComboBox1, 0, NULL);
							ComboBox_GetText(hwComboBox1, buf, MAX_TF_SIZE);
							swscanf(buf, L"%f", &displayedval);
							displayedval = convert(displayedval, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
						                   ui_getComboIndex(hDlg, IDC_CONV_UNIT1),
										   ui_getComboIndex(hDlg, IDC_CONV_UNIT2));
							swprintf(buf, L"%f", displayedval);
							ComboBox_SetText(hwComboBox2, buf);
*/
							break;

						case IDC_CONV_INPUT2:				// recalc as a result of new input value.
							hwComboBox1 = GetDlgItem(hDlg, IDC_CONV_INPUT2);// combo box #2
							hwComboBox2 = GetDlgItem(hDlg, IDC_CONV_INPUT1);// combo box #1
							ComboBox_SelectString(hwComboBox1, 0, NULL);
							ComboBox_GetText(hwComboBox1, buf, MAX_TF_SIZE);
							swscanf(buf, L"%f", &displayedval);
							displayedval = convert(displayedval, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
						                   ui_getComboIndex(hDlg, IDC_CONV_UNIT2),
										   ui_getComboIndex(hDlg, IDC_CONV_UNIT1));
							swprintf(buf, L"%f", displayedval);
							ComboBox_SetText(hwComboBox2, buf);
							break;

						case IDC_CONV_UNIT1:				// update values when units change.
						{	hwComboBox1 = GetDlgItem(hDlg, IDC_CONV_INPUT1);// combo box #1
							hwComboBox2 = GetDlgItem(hDlg, IDC_CONV_INPUT2);// combo box #2
							ComboBox_GetText(hwComboBox1, buf, MAX_TF_SIZE);
							swscanf(buf, L"%f", &displayedval);
							displayedval = convert(displayedval, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
												   ui_getComboIndex(hDlg, IDC_CONV_UNIT1),
												   ui_getComboIndex(hDlg, IDC_CONV_UNIT2));
							swprintf(buf, L"%f", displayedval);
							ComboBox_SetText(hwComboBox2, buf);
						}
							break;

						case IDC_CONV_UNIT2:				// update values when units change.
						{	hwComboBox1 = GetDlgItem(hDlg, IDC_CONV_INPUT1);// combo box #1
							hwComboBox2 = GetDlgItem(hDlg, IDC_CONV_INPUT2);// combo box #2
							ComboBox_GetText(hwComboBox1, buf, MAX_TF_SIZE);
							swscanf(buf, L"%f", &displayedval);
							displayedval = convert(displayedval, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
												   ui_getComboIndex(hDlg, IDC_CONV_UNIT1),
												   ui_getComboIndex(hDlg, IDC_CONV_UNIT2));
							swprintf(buf, L"%f", displayedval);
							ComboBox_SetText(hwComboBox2, buf);
						}
							break;


						default:
							break;
					}

					LocalFree(buf);
					return TRUE;
				}
					break;


				case CBN_EDITCHANGE:		// triggered at end of event-processing of keypress.
				{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);		
					float value;
					UINT uiwidget1, uiwidget2;

					if (wmId == IDC_CONV_INPUT1)		// if number selected from first combobox...
					{	uiwidget1 = IDC_CONV_INPUT1;
						uiwidget2 = IDC_CONV_INPUT2;
					}
					else
					if (wmId == IDC_CONV_INPUT2)		// if number selected from second combobox...
					{	uiwidget1 = IDC_CONV_INPUT2;
						uiwidget2 = IDC_CONV_INPUT1;
					}
					else								// woops... this shouldn't happen.
						return FALSE;

					GetDlgItemTextW(hDlg, uiwidget1, buf, MAX_TF_SIZE);
					swscanf(buf, L"%f", &value);

					if (uiwidget1 == IDC_CONV_INPUT1)
						value = convert(value,  ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
												ui_getComboIndex(hDlg, IDC_CONV_UNIT1),
												ui_getComboIndex(hDlg, IDC_CONV_UNIT2));
					else
						value = convert(value,  ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
												ui_getComboIndex(hDlg, IDC_CONV_UNIT2),
												ui_getComboIndex(hDlg, IDC_CONV_UNIT1));


					swprintf(buf, L"%f", value);
					SetDlgItemTextW(hDlg, uiwidget2, buf);
					LocalFree(buf);
				}
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}




void fillUnitsWidgets(HWND hDlg, int unitNo)
{	int count;
	switch(unitNo)
	{	case AREASELECTION:
			ui_resetComboBox(hDlg, IDC_CONV_UNIT1);
			ui_resetComboBox(hDlg, IDC_CONV_UNIT2);
			for (count = IDC_CONV_UNIT1; count <= IDC_CONV_UNIT2; count++)
			{	ui_addToComboBoxStr(hDlg, count, L"Acres");
				ui_addToComboBoxStr(hDlg, count, L"Square Inches");
				ui_addToComboBoxStr(hDlg, count, L"Square Feet");
				ui_addToComboBoxStr(hDlg, count, L"Square Yards");
				ui_addToComboBoxStr(hDlg, count, L"Square Mile");
				ui_addToComboBoxStr(hDlg, count, L"Hectares");
				ui_addToComboBoxStr(hDlg, count, L"Square Cm");
				ui_addToComboBoxStr(hDlg, count, L"Square Meters");
				ui_addToComboBoxStr(hDlg, count, L"Square Kilometers");
			}
			ui_setComboIndex(hDlg, IDC_CONV_UNIT2, 5);				
			break;
	
		case LENGTHSELECTION:
			ui_resetComboBox(hDlg, IDC_CONV_UNIT1);
			ui_resetComboBox(hDlg, IDC_CONV_UNIT2);
			for (count = IDC_CONV_UNIT1; count <= IDC_CONV_UNIT2; count++)
			{	ui_addToComboBoxStr(hDlg, count, L"Miles");
				ui_addToComboBoxStr(hDlg, count, L"Yards");
				ui_addToComboBoxStr(hDlg, count, L"Feet");
				ui_addToComboBoxStr(hDlg, count, L"Inches");
				ui_addToComboBoxStr(hDlg, count, L"Meters");
				ui_addToComboBoxStr(hDlg, count, L"Kilometers");
				ui_addToComboBoxStr(hDlg, count, L"Decimeters");
				ui_addToComboBoxStr(hDlg, count, L"Centimeters");
				ui_addToComboBoxStr(hDlg, count, L"Millimeters");
			}
			ui_setComboIndex(hDlg, IDC_CONV_UNIT2, 5);
			break;

		case LIQUIDSELECTION:
			ui_resetComboBox(hDlg, IDC_CONV_UNIT1);
			ui_resetComboBox(hDlg, IDC_CONV_UNIT2);
			for (count = IDC_CONV_UNIT1; count <= IDC_CONV_UNIT2; count++)
			{	ui_addToComboBoxStr(hDlg, count, L"Gallons");
				ui_addToComboBoxStr(hDlg, count, L"Quarts");
				ui_addToComboBoxStr(hDlg, count, L"Pints");
				ui_addToComboBoxStr(hDlg, count, L"Cups");
				ui_addToComboBoxStr(hDlg, count, L"Ounces");
				ui_addToComboBoxStr(hDlg, count, L"UK Ounces");
				ui_addToComboBoxStr(hDlg, count, L"Tablespoons");
				ui_addToComboBoxStr(hDlg, count, L"Teaspoons");
				ui_addToComboBoxStr(hDlg, count, L"Liters");
				ui_addToComboBoxStr(hDlg, count, L"Milliliters/CCs");
			}
			ui_setComboIndex(hDlg, IDC_CONV_UNIT2, 4);
			break;

		case TEMPSELECTION:
			ui_resetComboBox(hDlg, IDC_CONV_UNIT1);
			ui_resetComboBox(hDlg, IDC_CONV_UNIT2);
			for (count = IDC_CONV_UNIT1; count <= IDC_CONV_UNIT2; count++)
			{
				ui_addToComboBoxStr(hDlg, count, L"Fahrenheit");
				ui_addToComboBoxStr(hDlg, count, L"Celsius");
				ui_addToComboBoxStr(hDlg, count, L"Kelvin");
			}
			ui_setComboIndex(hDlg, IDC_CONV_UNIT2, 1);
			break;

		case WEIGHTSELECTION:
			ui_resetComboBox(hDlg, IDC_CONV_UNIT1);
			ui_resetComboBox(hDlg, IDC_CONV_UNIT2);
			for (count = IDC_CONV_UNIT1; count <= IDC_CONV_UNIT2; count++)
			{	ui_addToComboBoxStr(hDlg, count, L"Pounds (lbs)");
				ui_addToComboBoxStr(hDlg, count, L"Ounces");
				ui_addToComboBoxStr(hDlg, count, L"Grams");
				ui_addToComboBoxStr(hDlg, count, L"Kilograms");
				ui_addToComboBoxStr(hDlg, count, L"Long Tons");
			}
			ui_setComboIndex(hDlg, IDC_CONV_UNIT2, 3);
			break;
	}
}



void updateUnitsDisplay(HWND hDlg)
{	TCHAR *buf = (TCHAR *)LocalAlloc (LPTR, MAX_TF_SIZE + 1);
	float displayedval;
	HWND hwComboBox1, hwComboBox2;
	hwComboBox1 = GetDlgItem(hDlg, IDC_CONV_INPUT1);// combo box #1
	hwComboBox2 = GetDlgItem(hDlg, IDC_CONV_INPUT2);// combo box #2
	ComboBox_SelectString(hwComboBox1, 0, NULL);
	ComboBox_GetText(hwComboBox1, buf, MAX_TF_SIZE);
	swscanf(buf, L"%f", &displayedval);
	displayedval = convert(displayedval, ui_getComboIndex(hDlg, IDC_CONV_UNITTYPE), 
				   ui_getComboIndex(hDlg, IDC_CONV_UNIT1),
				   ui_getComboIndex(hDlg, IDC_CONV_UNIT2));
	swprintf(buf, L"%f", displayedval);
	ComboBox_SetText(hwComboBox2, buf);
	LocalFree(buf);
}
		



float convert(float invalue, int category, int originbase, int newbase)
{   double outvalue = (float) invalue;
								
	switch(category)
	{
		case AREASELECTION:		// ACRES, SQINCHES, SQFEET, SQYARDS, HECTARES, SQCMS, SQMETERS
								// step 1:  convert value of specifed unit into generic unit (sq feet).
			if (originbase == ACRES)		// convert to square feet.
				outvalue *= 43560.0;		
			else
			if (originbase == SQINCHES)		// convert to square feet.
				outvalue *= .0069444444444444;
			else
			if (originbase == SQYARDS)
				outvalue *= 9.0;
			else
			if (originbase == SQMILES)
				outvalue *= 27878400.0;
			else
			if (originbase == HECTARES)
				outvalue *= 107639.10416709722;
			else
			if (originbase == SQCMS)
				outvalue *= 0.0010763910416709;
			else
			if (originbase == SQMETERS)
				outvalue *= 10.763910416709722;
			else
			if (originbase == SQKMS)
				outvalue *= 10763910.416709722;


								// step 2:  convert sq feet into specified unit.
			if (newbase == ACRES)
				outvalue /= 43560.0;
			else
			if (newbase == SQINCHES)
				outvalue /= .0069444444444444;
			else
			if (newbase == SQYARDS)
				outvalue /= 9.0;
			else
			if (newbase == SQMILES)
				outvalue /= 27878400.0;
			else
			if (newbase == HECTARES)
				outvalue /= 107639.10416709722; 
			else
			if (newbase == SQCMS)
				outvalue /= 0.0010763910416709;
			else
			if (newbase == SQMETERS)
				outvalue /= 10.763910416709722;
			else
			if (newbase == SQKMS)
				outvalue /= 10763910.416709722;

			break;

		case LENGTHSELECTION:	// MILES, YARDS, FEET, INCHES, METERS, KILOMETERS, DECIMETERS, CENTIMETERS, MILLIMETERS
								// step 1:  convert value of specifed unit into generic unit (feet).
			if (originbase == MILES)
				outvalue *= 5280.0;
			else
			if (originbase == YARDS)
				outvalue *= 3.0;
			else
			if (originbase == INCHES)
				outvalue *= 0.0833333333333333;
			else
			if (originbase == METERS)
				outvalue *= 3.28084;
			else
			if (originbase == KILOMETERS)
				outvalue *= 3280.084;
			else
			if (originbase == DECIMETERS)
				outvalue *= 0.328084;
			else
			if (originbase == CENTIMETERS)
				outvalue *= 0.0328084;
			else
			if (originbase == MILLIMETERS)
				outvalue *= 0.00328084;

								// step 2:  convert feet into specified unit.
			if (newbase == MILES)
				outvalue /= 5280.0;
			else
			if (newbase == YARDS)
				outvalue /= 3.0;
			else
			if (newbase == INCHES)
				outvalue /= 0.0833333333333333;
			else
			if (newbase == METERS)
				outvalue /= 3.28084;
			else
			if (newbase == KILOMETERS)
				outvalue /= 3280.084;
			else
			if (newbase == DECIMETERS)
				outvalue /= 0.328084;
			else
			if (newbase == CENTIMETERS)
				outvalue /= 0.0328084;
			else
			if (newbase == MILLIMETERS)
				outvalue /= 0.00328084;
			break;

		case LIQUIDSELECTION:	// GALLONS, QUARTS, PINTS, CUPS, OUNCES, UKOUNCES, TBSPS, TSPS, LITERS, MILLILITERS (CCs)
								// step 1:  convert value of specifed unit into generic unit (cup).
			if (originbase == GALLONS)
				outvalue *= 16.0;
			else
			if (originbase == QUARTS)
				outvalue *= 4.0;
			else
			if (originbase == PINTS)
				outvalue *= 2.0;
			else
			if (originbase == OUNCES)
				outvalue *= 0.125;
			else
			if (originbase == UKOUNCES)
				outvalue *= 8.32673874424331;
			else
			if (originbase == TBSPS)
				outvalue *= 0.0625;		// 1/16
			else
			if (originbase == TSPS)
				outvalue *= 0.02083333333333;		// 1/48
			else
			if (originbase == LITERS)
				outvalue *= 4.2267525;
			else
			if (originbase == MILLILITERS)
				outvalue *= 0.0042267525;

								// step 2:  convert cups into specified unit.
			if (newbase == GALLONS)
				outvalue /= 16.0;
			else
			if (newbase == QUARTS)
				outvalue /= 4.0;
			else
			if (newbase == PINTS)
				outvalue /= 2.0;
			else
			if (newbase == OUNCES)
				outvalue /= 0.125;
			else
			if (newbase == UKOUNCES)
				outvalue /= 8.32673874424331;
			else
			if (newbase == TBSPS)
				outvalue /= 0.0625;
			else
			if (newbase == TSPS)
				outvalue /= 0.02083333333333;
			else
			if (newbase == LITERS)
				outvalue /= 4.2267525;
			else
			if (newbase == MILLILITERS)
				outvalue /= 0.0042267525;

			break;

		case TEMPSELECTION:		// FAHRENHEIT, CELSIUS, KELVIN
								// step 1:  convert value of specifed unit into generic unit (fahrenheit).
			if (originbase == CELSIUS)
				outvalue = outvalue * 1.8 + 32;
			else
			if (originbase == KELVIN)
				outvalue = (outvalue - 273.15) * 1.8 + 32;   //f = (deg K-273.15)*9/5+32

			if (newbase == CELSIUS)
				outvalue = outvalue - 32 / 1.8;
			else
			if (newbase == KELVIN)
				outvalue = outvalue - 32 / 1.8 + 273.15;

			break;

		case WEIGHTSELECTION:	// POUNDS, WTOUNCES, GRAMS, KILOGRAMS, LONGTONS
								// step 1:  convert value of specified unit into generic unit (pounds).
			if (originbase == WT_OUNCES)
				outvalue *= 0.0625;
			else
			if (originbase == GRAMS)
				outvalue *= 0.0022046225;  //1/453.592395069904;
			else
			if (originbase == KILOGRAMS)
				outvalue *= 2.2046225;     //(1.0 / .453592395069904);
			else
			if (originbase == LONGTONS)
				outvalue *= 2240;

			if (newbase == WT_OUNCES)
				outvalue /= 0.0625;
			else
			if (newbase == GRAMS)
				outvalue /= 0.0022046225;
			else
			if (newbase == KILOGRAMS)
				outvalue /= 2.2046225;
			else
			if (newbase == LONGTONS)
				outvalue /= 2240;

			break;

		default:
			break;

	}
	return (float) outvalue;
}
