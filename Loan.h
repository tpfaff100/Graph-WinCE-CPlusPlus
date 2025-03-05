#include "Amort.h"
#include "UITools.h"
#include "resource.h"


LRESULT CALLBACK LoanForm(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LoanView(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void resetLoanForm(HWND hDlg);
void updateUI(HWND hDlg, int widgetHit);