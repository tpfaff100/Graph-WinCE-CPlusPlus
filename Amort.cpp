#include "amort.h"

#define MAXLOANS   25

static ad amort[MAXLOANS];
static int amortIndex = 0;

static princIntBal *pib = NULL;		// implicitly allocated by amort_amortize()- dealloc later.
									// pib can stick around until program exit since only one is ever needed.

char *loanSummary = NULL;

void amort_setAnnualRate(double rate)		// just call amort_calculatePayment() after this.
{	amort[amortIndex].monthlyrate = rate/12.0/100.0;
}

double amort_getMonthlyPayment(void)		// return monthly payment after amort_init is called.
{   return amort[amortIndex].monthlypayment;
}

double amort_getPrincipal(void)
{   return amort[amortIndex].principal;
}

double amort_getAnnualRate(void)
{   return amort[amortIndex].monthlyrate*12.0*100.0;
}

int amort_getMonths(void)
{	return amort[amortIndex].months;
}

int amort_getDuration(void)					// get number of monthly payments necessary to pay off loan.
{   return amort[amortIndex].monthsWithReduction;   
}

double amort_getPrincipalReduction(void)
{	return amort[amortIndex].principalReduction;
}

double amort_getTotalInterest(void)			// call this only after calling amort_amortize().
{	return amort[amortIndex].totalInterest;
}

void amort_getNext(void)
{   if (amortIndex < MAXLOANS)
        amortIndex++;
}

void amort_getPrevious(void)
{    if (amortIndex > 0)
        amortIndex--;
}

int amort_getIndex(void)
{	return amortIndex;
}

princIntBal *amort_getPIB()				// get amortization table pointer.  (must deallocate later).
{   return pib;
}

                                        // initalize variables to invalid values.
void amort_init(int mo, int year, double loanamount, double addtlpymt, double annualrate, int months)            
{	                					// if addtlpymt > 0, number of months of loan decreases.
	amort[amortIndex].months = months;             // normal duration of loan if no additional prinicpal is paid down.
    amort[amortIndex].principal = (float) loanamount;

    amort[amortIndex].startmonth = mo;
    amort[amortIndex].startyear = year;

    amort[amortIndex].monthlyrate = annualrate/12;
	amort[amortIndex].monthlyrate /= 100.0;

	amort[amortIndex].principalReduction = addtlpymt;

	amort[amortIndex].totalInterest = 0;	// we don't know what this is yet!
}

void amort_free(void)					// this probably never really needs to be called.
{   free(pib);							// the amortization table should stay around until 
}										// the program exits.

	

double amort_calculatePayment()
{   double time = 1.0;
    for (int x = 1; x <= amort[amortIndex].months; x++)
        time = time * (1+amort[amortIndex].monthlyrate);

    time = 1 / time;
    time = 1 - time;
    amort[amortIndex].monthlypayment = amort[amortIndex].principal * amort[amortIndex].monthlyrate / time;
    return amort[amortIndex].monthlypayment;
}


int amort_amortize()						// amortize based on private vars principal and months.
{   double balance;							// returns an int that represents the actual number
    double annualInterest=0;				// of months it will take to pay off the loan.
    double totalInterest=0;
    double principal;                       // principal reduction per month in loan.
    int count;
//        String monthTitle;

	double principalReduction = amort[amortIndex].principalReduction;

    balance = amort[amortIndex].principal;              // get loan balance (total outstanding principal).
    count = 0;

	if (pib == NULL)						// we allocate one and only one of these until program exit.
		pib = (princIntBal *) malloc(sizeof(princIntBal));  // lazily make an amortization schedule buffer..

//        strcat(loanSummary, "Year 0\n");
//        strcat(loanSummary, "Month\t\tInterst\tPrincipal\tBalance\n");

	for (count = 0; count < MAXPIBSIZE; count++)	// zero all entries before amortizing.
	{	pib->p[count] = 0.0;
		pib->i[count] = 0.0;
		pib->b[count] = 0.0;
	}
	count = 0;


    do
    {   
		double interest = balance*amort[amortIndex].monthlyrate;
        annualInterest += interest;
        principal = amort[amortIndex].monthlypayment - interest;
        principal += principalReduction;

        if (principal > balance)
        {   amort[amortIndex].monthlypayment = interest + balance;
            principal = balance;
        }

        balance -= principal;
        if (balance < 1)
        {   principal += balance;
            amort[amortIndex].monthlypayment += balance;
            balance = 0;
        }

//            monthTitle = getDate(count+startmonth);
//            loanSummary += monthTitle+"\t"+interest+"\t"+principal+"\t"+balance+"\n";
		pib->p[count] = ((int) (principal*100.0+.5)) / (float) 100.0;
		pib->i[count] = ((int) (interest*100.0+.5)) / (float) 100.0;
		pib->b[count] = ((int) (balance*100.0+.5)) / (float) 100.0;

//            if (monthTitle.startsWith("Dec") || counhbt == amort[amortIndex].months || balance == 0)
        if ( (count+amort[amortIndex].startmonth+1) % 12 == 0 || count == amort[amortIndex].months || balance == 0)
        {   
//				loanSummary += "YTD-  Interest: "+annualInterest+"\n\n";
            totalInterest += annualInterest;
            annualInterest = 0;
            if (count < amort[amortIndex].months && principal > 0)
            {   //loanSummary += "Year "+((count/12)+1)+"\n";
                //loanSummary += "Month\t\tInterst\tPrincipal\tBalance\n";
            }
        }
    }
    while (count++ < amort[amortIndex].months && balance > 0);

//        loanSummary += "Total Interest: "+totalInterest + "  Total Payment: "+(monthlypayment*count)+"\n";
    if (count < amort[amortIndex].months)
    {   
//			loanSummary += "\nBy overpaying your principal by "+principalReduction;
//            loanSummary += " you've shortened the life\nof your loan from "+(int)(months/12);
//            loanSummary += " years to "+count/12+" years and "+count%12+" months\n\n";
    }
	amort[amortIndex].monthsWithReduction = count;
	amort[amortIndex].totalInterest = totalInterest;
	return count;				// return actual months it will take to pay off loan.
}								// (may be different than npayments variable.


