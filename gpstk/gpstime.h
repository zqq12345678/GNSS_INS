/*==============================================================================
*  This file is part of GPSTk, the GPS Toolkit.
*  
*  Copyright (C/C++) 2020, Beihang University All Rights Reserved.
*
*  Author: ZHANG Q.Q, E-mail:zhangqieqie@buaa.edu.cn
*  
*  version: $Revision 1.0 $Data: 2020/04/17 18:29:00 $
*
*  history: 2020/04/17 	1.0 new
						gtime_t timeadd(gtime_t t, double sec);		
						double  timediff(gtime_t t1, gtime_t t2);	
						gtime_t epoch2time(const double *ep);		
						void	time2epoch(gtime_t t,double *ep);	
						double  time2doy(gtime_t t);				
						gtime_t gpst2time(int week, double sec);	
						double  time2gpst(gtime_t t, int *week);	
						gtime_t gst2time(int week, double sec);		
						double  time2gst(gtime_t t, int *week);		
						gtime_t bdt2time(int week,double sec);		
						double  time2bdt(gtime_t t, int *week);		
						gtime_t gpst2utc(gtime_t t);				
						gtime_t utc2gpst(gtime_t t);				
						gtime_t gpst2bdt(gtime_t t);				
						gtime_t bdt2gpst(gtime_t t);				
						double  time2sec(gtime_t time, gtime_t *day);
						double  utc2gmst(gtime_t t, double ut1_utc); 
*==============================================================================*/
/**
 * @file gpstime.h
 * time class, including various time operations objects.
 */

#ifndef GPSTIME_H_
#define GPSTIME_H_

#include <ctime>

namespace gpstk{

	const static double gpst0[]={1980,1, 6,0,0,0}; /* gps time reference */
	const static double gst0 []={1999,8,22,0,0,0}; /* galileo system time reference */
	const static double bdt0 []={2006,1, 1,0,0,0}; /* beidou time reference */

	const static double leaps[][7]={ /* leap seconds {y,m,d,h,m,s,utc-gpst,...} */
	
		{2017,1,1,0,0,0,-18},
		{2015,7,1,0,0,0,-17},
		{2012,7,1,0,0,0,-16},
		{2009,1,1,0,0,0,-15},
		{2006,1,1,0,0,0,-14},
		{1999,1,1,0,0,0,-13},
		{1997,7,1,0,0,0,-12},
		{1996,1,1,0,0,0,-11},
		{1994,7,1,0,0,0,-10},
		{1993,7,1,0,0,0, -9},
		{1992,7,1,0,0,0, -8},
		{1991,1,1,0,0,0, -7},
		{1990,1,1,0,0,0, -6},
		{1988,1,1,0,0,0, -5},
		{1985,7,1,0,0,0, -4},
		{1983,7,1,0,0,0, -3},
		{1982,7,1,0,0,0, -2},
		{1981,7,1,0,0,0, -1}
	};

	struct gtime_t{         /* time struct */
		time_t time;        /* time (s) expressed by standard time_t */
		double sec;         /* fraction of second under 1 s */
	};//struct gtime_t

	class gpstime{			/* class of gpstime */

		public:			
			gpstime();		/* constructor */

			gtime_t timeadd(gtime_t t, double sec);		/* add time */
			double  timediff(gtime_t t1, gtime_t t2);	/* time difference */
			gtime_t epoch2time(const double *ep);		/* convert calendar day/time to time */
			void	time2epoch(gtime_t t,double *ep);	/* time to calendar day/time */
			double  time2doy(gtime_t t);				/* time to doy of year */
			gtime_t gpst2time(int week, double sec);	/* gps time to time */
			double  time2gpst(gtime_t t, int *week);	/* time to gps time */
			gtime_t gst2time(int week, double sec);		/* galileo time to time */
			double  time2gst(gtime_t t, int *week);		/* time galileo time*/
			gtime_t bdt2time(int week,double sec);		/* bds time to time */
			double  time2bdt(gtime_t t, int *week);		/* time to bds time */
			gtime_t gpst2utc(gtime_t t);				/* gps time to utc */
			gtime_t utc2gpst(gtime_t t);				/* utc to gps time */
			gtime_t gpst2bdt(gtime_t t);				/* gps time to bds time */
			gtime_t bdt2gpst(gtime_t t);				/* bds time to gps time */
			double  time2sec(gtime_t time, gtime_t *day);/* time to doy and sec */
			double  utc2gmst(gtime_t t, double ut1_utc);/* utc to gmst */

			virtual ~gpstime();/* destructor */

		private:

	};//class gpstime

}// namespace


#endif // GPSTIME_H_

