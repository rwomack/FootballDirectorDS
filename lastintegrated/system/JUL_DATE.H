	void initialize_JD ();			/* fill array JD[] */
        double julian_day(int month, double day, int year);  			/* return Julian Day number for date month/day/year */
        void JDtoMDY (double jul, int *month, double *day, int *year);  	/* set month/day/year to proper values for Julian Day 'jul' */
	int leap(int year);

