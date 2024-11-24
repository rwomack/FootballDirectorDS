


        /*

		 File:- jul_date.cpp

         Football Director 95

         Julian date routines

         27/11/94 12:50

		*/


        #include <stdio.h>

		#define MINYEAR  -4712   /* if changed, initialize_JD() must be modified */
		#define MAXYEAR   4000   /* may be increased if the fact that year 4000 and all its multiples may not be leap years is ignored, or the function leap(y) is modified */

		int month_size[] ={
			0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
		};
		long JD[MAXYEAR-MINYEAR+1]; /* Julian Day numbers for Jan 1, year+4712.
                           	Note that this refers to astromical years, which begin at noon, not midnight;
                               the value for civil dates is .5 less than this */


 /* ======================= function prototypes ============================ */
 /* all the following functions assume the Julian calendar ended on Oct 4, 1582,
   and the Gregorian calendar started on Oct 15, 1582 */

	void initialize_JD ();			/* fill array JD[] */
        float julian_day(int month, float day, int year);       /* return Julian Day number for date month/day/year */
        void JDtoMDY (float jul, int *month, float *day, int *year);    /* set month/day/year to proper values for Julian Day 'jul' */
	int leap(int year);
	int day_of_year(int month, int day, int year);
	int month_day	(int year, int day_num, int *month, int *day);
	int year_size(int year);
	int days_in_month(int month, int year);


 /* astromomers assign Julian day numbers to a day and fraction of a day,
   hence the argument 'day' is a float */
 /* ------------------------------------------------------------------------ */
float julian_day(int month, float day, int year)
	{
  		long jul;
  		int day_int;           /* day_int is the integer part of day */
      float fraction;       /* fraction is the fractional part of day */
		  int i;
      float jul_dbl;

  		i = year + 4712;
  		jul = JD[i];                      /* Julian number of Jan 1.5 of given year */
  		day_int = (int)day;               /* integer part of argument 'day' */
  		fraction = day - day_int;         /* fractional part of 'day' */
  		jul += (day_of_year(month,day_int,year)-1); /* add enough to each month,day of 'year' */

  		/* subtract .5 from jul to get to start of civil day(midnight) then add fraction of day called for */
      jul_dbl = (float)jul;
      jul_dbl = jul_dbl - .5f + fraction;
  		return(jul_dbl);
	}

 /* set month/day/year to proper values for Julian Day 'jul' */
 /* ------------------------------------------------------------------------ */
void JDtoMDY(float jul, int *month, float *day, int *year)
	{
  		int i=0, y;
  		long J0,jul_int;
      float fraction;
  		int day_num,day_int;

  		/* break input value jul into long integer and fractional parts */
  		jul += .5;        			/* correct for difference in civil and astronomical days */
  		jul_int = (long)jul; 		/* integral part of jul */
  		fraction = jul - jul_int;    	/* fractional part of jul */

  		/* find largest integer i with JD[i] <= jul */
  		while(JD[i] <= jul_int && i < MAXYEAR-MINYEAR+1){
    			i++;
    		}

  		J0 = JD[--i];        			/* J0 = Julian day number of Jan 1.5 of the largest year which contains the input number */
  		y = i - 4712;        			/* the year corresponding to the input Julian number */
  		day_num = (int)(jul - J0 + 1); /* difference between Julian day number of (noon) Jan 1 of year y and the specified Julian day number */
  		*year = y;
  		month_day(y, day_num, month, &day_int);  /* convert day_num to month/day_int of year y */
  		*day = day_int + fraction;     /* convert day_int to fractional day */
  		return;
	}

 /* JD[i] gives the Julian day number the first day of the year i+4712,
   at noon(i.e, the astronomical Julian day number of Jan 1.5.
   Since the numbering system began on Jan 1, -4712, JD[0] = 0 */
 /* ------------------------------------------------------------------------ */
void initialize_JD()
	{
  		int year, num=0, d;

		JD[0] = 0;
  		/* Calculate JD[year+4712] inductively from JD[0] = 0. If MINYEAR is not
     		defined to be -4712, this process must be modified */
  		for (year = MINYEAR+1; year <= MAXYEAR; year++) {
    			num++;
    			d = year_size(year-1);
    			JD[num] = JD[num-1] + d;  /* add number of days in prededing year */
  		}
  		return;
	}

 /* assumes change from Julian to Gregorian calendar in 1582 */
 /*---------------------------------------------------------------------------*/
int leap(int year)
	{
 		return(year<1582?(year%4==0):year%4==0&&year%100!=0||year%400==0);
	}


 /* set size of year, recognizing leap years and 1582 */
 /* assumes change from Julian to Gregorian calendar in 1582 */
 /*---------------------------------------------------------------------------*/
int year_size(int year)
	{
  		if (year == 1582){
    			return(355);
    	}
  		else{
    			return(365 + leap(year));
    	}
	}


 /* assumes change from Julian to Gregorian calendar in 1582 */
 /*---------------------------------------------------------------------------*/
int day_of_year(int month, int day, int year)
	{
  		int i;

  		if (year == 1582 && month == 10 && day >= 15){
    			day -= 10;
    	}
  		for (i = 1; i < month; i ++){
    			day += days_in_month(i, year);
    	}
  		return(day);
	}


 /* set month and day for given day_num, year */
 /* assumes change from Julian to Gregorian calendar in 1582 */
 /*---------------------------------------------------------------------------*/
int month_day	(int year, int day_num, int *month, int *day)
	{
  		int i,d;

  		for (i=1; day_num > days_in_month(i,year); i++) {
    			d = days_in_month(i,year);
    			day_num -= d;
  		}
  		if (year == 1582 && i == 10 && day_num > 4){
    			day_num +=10;     /* compensate for missing days in Oct 1582 */
    		}

  		*month = i;
  		*day = day_num;
  		return(1);
	}


 /* assumes change from Julian to Gregorian calendar in 1582 */
 /*----------------------------------------------------------------------------*/
int days_in_month(int month, int year)
	{
  		int num_days = month_size[month];
  		if (leap(year)){
    		if (month == 2){
      			num_days = 29;             /* leap day */
			}
		}
  		if (year == 1582) {
    		if (month == 10) {
      			num_days = 21;    /* October, 1582 had only 21 days */
			}
		}
  		return(num_days);
	}


