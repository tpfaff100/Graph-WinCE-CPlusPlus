#define MAX_CALC_CHARS   255

#include <math.h>
#include "StdAfx.h"
#include "resource.h"
#include "Loan.h"


#define NON_OP		0
#define ADD_OP		1
#define SUB_OP		2
#define MUL_OP		3
#define DIV_OP		4
#define AND_OP		5
#define OR_OP		6
#define XOR_OP		7
#define MOD_OP		8

#define D_BINARY    1
#define D_DECIMAL   2
#define D_HEX		3
#define D_OCTAL		4

typedef struct calcData
{   
	TCHAR *calcbuf;
	double value;
	double holding;
	double memval;
	int    op;              // operations: ADD_OP, SUB_OP, MUL_OP, DIV_OP
	int    displayMode;     // modes: D_BINARY, D_DECIMAL, D_HEX, D_OCTAL
	int    floatingPointLocation;
	int    integerPlaces;
	BOOL   allowAddDigits;

} cd;


LRESULT CALLBACK CalcView(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


void	calc_init(void);
void    calc_deinit(void);

void	calc_addDigit(int val);
void	calc_backspace(void);
void	calc_bumpDecimalPoint();

TCHAR  *calc_getString(void);
double	calc_getValue(void);
void	calc_setValue(double value);

void	calc_clear(void);
void	calc_ce(void);
void	calc_add(void);
void	calc_subtract(void);
void	calc_multiply(void);
void	calc_divide(void);
void	calc_and(void);
void	calc_or(void);
void	calc_xor(void);
void	calc_mod(void);

double	calc_equals(void);

void	calc_sin(void);
void	calc_cos(void);
void	calc_tan(void);
void	calc_asin(void);
void	calc_acos(void);
void	calc_atan(void);

void    calc_sqrt(void);

void    calc_memplus(void);
void    calc_memminus(void);
void    calc_memretrieve(void);
void    calc_memclear(void);

void	calc_plusminus(void);

void    calc_displayBinary(void);
void    calc_displayDecimal(void);
void    calc_displayHex(void);
void    calc_displayOctal(void);

void	calc_leftShift(void);
void	calc_rightShift(void);


