#include "Amort.h"
#include "UITools.h"
#include "resource.h"

LRESULT CALLBACK ConversionView(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void fillUnitsWidgets(HWND hDlg, int unitNo);

void updateUnitsDisplay(HWND hDlg);
float convert(float invalue, int category, int originbase, int newbase);