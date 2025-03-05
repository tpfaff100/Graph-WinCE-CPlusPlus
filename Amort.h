#ifndef AMORT

#define AMORT

#include "stdafx.h"
#include "afxwin.h"

#define MAXPIBSIZE	1200				// make space for a [maximum] 100 year loan!

typedef struct amortizationData
{   double monthlyrate;				    // interest rate of our loan (per month)
    double monthlypayment;				// total monthly payment for loan.
	double principal;					// original principal of loan.
	double principalReduction;			// additional principal reduction paid per month.
	double totalInterest;				// total interest paid during life of loan.
	int months;							// total number of months of original loan (commonly 360).
    int monthsWithReduction;            // if extra principal is made every month, loan lifetime decreases.
	int startmonth;						// originating month of loan (1-12)
	int startyear;						// originating year of loan (e.g. 2001)
} ad;

typedef struct principalInterestBalance
{   float p[MAXPIBSIZE];				// maximum loan duration supported is 100 years.
	float i[MAXPIBSIZE];
	float b[MAXPIBSIZE];
} princIntBal;


// amortization module.
void    amort_init(int mo, int year, double loanamount, double addtlpymt, double rate, int months);       
void	amort_free(void);							// free princIntBal allocation after done.

double  amort_calculatePayment(void);
int     amort_amortize(void);				// amortize based on private vars principal and months.
double	amort_getAnnualRate(void);
void	amort_setAnnualRate(double rate);	// just call amort_calculatePayment() after this.
int		amort_getMonths(void);
int	    amort_getDuration();
double  amort_getPrincipal(void);
princIntBal *amort_getPIB();                // get amortization table from amort module.
double	amort_getPrincipalReduction(void);
double  amort_getMonthlyPayment(void);
double  amort_getTotalInterest(void);		// call this only after calling amort_amortize().
void    amort_getNext(void);
void    amort_getPrevious(void);
int		amort_getIndex(void);

#endif