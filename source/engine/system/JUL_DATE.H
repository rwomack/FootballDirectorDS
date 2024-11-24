	void initialize_JD ();			/* fill array JD[] */
        float julian_day(int month, float day, int year);       /* return Julian Day number for date month/day/year */
        void JDtoMDY (float jul, int *month, float *day, int *year);    /* set month/day/year to proper values for Julian Day 'jul' */
	int leap(int year);

