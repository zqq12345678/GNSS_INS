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
 * @file gpstime.cpp
 * time class, including various time operations objects.
 */

#include "constant.h"
#include "gpstime.h"
#include "math.h"

namespace gpstk
{

	gpstime::gpstime(){  /* constructor */

	}

	/* add time --------------------------------------------------------------------
	* add time to gtime_t struct
	* args   : gtime_t t        I   gtime_t struct
	*          double sec       I   time to add (s)
	* return : gtime_t struct (t+sec)
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::timeadd(gtime_t t, double sec)
	{
		double tt;
		t.sec+=sec; tt=floor(t.sec); t.time+=(int)tt; t.sec-=tt;
		return t;
	}

	/* time difference -------------------------------------------------------------
	* difference between gtime_t structs
	* args   : gtime_t t1,t2    I   gtime_t structs
	* return : time difference (t1-t2) (s)
	*-----------------------------------------------------------------------------*/
	double gpstime::timediff(gtime_t t1, gtime_t t2)
	{
		return difftime(t1.time,t2.time)+t1.sec-t2.sec;
	}

	/* convert calendar day/time to time -------------------------------------------
	* convert calendar day/time to gtime_t struct
	* args   : double *ep       I   day/time {year,month,day,hour,min,sec}
	* return : gtime_t struct
	* notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::epoch2time(const double *ep)
	{
		const int doy[]={1,32,60,91,121,152,182,213,244,274,305,335};
		gtime_t time={0};
		int days,sec,year=(int)ep[0],mon=(int)ep[1],day=(int)ep[2];

		if (year<1970||2099<year||mon<1||12<mon) return time;

		/* leap year if year%4==0 in 1901-2099 */
		days=(year-1970)*365+(year-1969)/4+doy[mon-1]+day-2+(year%4==0&&mon>=3?1:0);
		sec=(int)floor(ep[5]);
		time.time=(time_t)days*86400+(int)ep[3]*3600+(int)ep[4]*60+sec;
		time.sec=ep[5]-sec;
		return time;
	}

	/* time to calendar day/time ---------------------------------------------------
	* convert gtime_t struct to calendar day/time
	* args   : gtime_t t        I   gtime_t struct
	*          double *ep       O   day/time {year,month,day,hour,min,sec}
	* return : none
	* notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
	*-----------------------------------------------------------------------------*/
	void gpstime::time2epoch(gtime_t t, double *ep)
	{
		const int mday[]={ /* # of days in a month */
			31,28,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31,
			31,29,31,30,31,30,31,31,30,31,30,31,31,28,31,30,31,30,31,31,30,31,30,31
		};
		int days,sec,mon,day;

		/* leap year if year%4==0 in 1901-2099 */
		days=(int)(t.time/86400);
		sec=(int)(t.time-(time_t)days*86400);
		for (day=days%1461,mon=0;mon<48;mon++) {
			if (day>=mday[mon]) day-=mday[mon]; else break;
		}
		ep[0]=1970+days/1461*4+mon/12; ep[1]=mon%12+1; ep[2]=day+1;
		ep[3]=sec/3600; ep[4]=sec%3600/60; ep[5]=sec%60+t.sec;
	}

	/* time to day of year ---------------------------------------------------------
	* convert time to day of year
	* args   : gtime_t t        I   gtime_t struct
	* return : day of year (days)
	*-----------------------------------------------------------------------------*/
	double gpstime::time2doy(gtime_t t)
	{
		double ep[6];

		time2epoch(t,ep);
		ep[1]=ep[2]=1.0; ep[3]=ep[4]=ep[5]=0.0;
		return timediff(t,epoch2time(ep))/86400.0+1.0;
	}

	/* gps time to time ------------------------------------------------------------
	* convert week and tow in gps time to gtime_t struct
	* args   : int    week      I   week number in gps time
	*          double sec       I   time of week in gps time (s)
	* return : gtime_t struct
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::gpst2time(int week, double sec)
	{
		gtime_t t=epoch2time(gpst0);

		if (sec<-1E9||1E9<sec) sec=0.0;
		t.time+=86400*7*week+(int)sec;
		t.sec=sec-(int)sec;
		return t;
	}
	/* time to gps time ------------------------------------------------------------
	* convert gtime_t struct to week and tow in gps time
	* args   : gtime_t t        I   gtime_t struct
	*          int    *week     IO  week number in gps time (NULL: no output)
	* return : time of week in gps time (s)
	*-----------------------------------------------------------------------------*/
	double gpstime::time2gpst(gtime_t t, int *week)
	{
		gtime_t t0=epoch2time(gpst0);
		time_t sec=t.time-t0.time;
		int w=(int)(sec/(86400*7));

		if (week) *week=w;
		return (double)(sec-w*86400*7)+t.sec;
	}

	/* galileo system time to time -------------------------------------------------
	* convert week and tow in galileo system time (gst) to gtime_t struct
	* args   : int    week      I   week number in gst
	*          double sec       I   time of week in gst (s)
	* return : gtime_t struct
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::gst2time(int week, double sec)
	{
		gtime_t t=epoch2time(gst0);

		if (sec<-1E9||1E9<sec) sec=0.0;
		t.time+=86400*7*week+(int)sec;
		t.sec=sec-(int)sec;
		return t;
	}
	/* time to galileo system time -------------------------------------------------
	* convert gtime_t struct to week and tow in galileo system time (gst)
	* args   : gtime_t t        I   gtime_t struct
	*          int    *week     IO  week number in gst (NULL: no output)
	* return : time of week in gst (s)
	*-----------------------------------------------------------------------------*/
	double gpstime::time2gst(gtime_t t, int *week)
	{
		gtime_t t0=epoch2time(gst0);
		time_t sec=t.time-t0.time;
		int w=(int)(sec/(86400*7));

		if (week) *week=w;
		return (double)(sec-w*86400*7)+t.sec;
	}
	/* beidou time (bdt) to time ---------------------------------------------------
	* convert week and tow in beidou time (bdt) to gtime_t struct
	* args   : int    week      I   week number in bdt
	*          double sec       I   time of week in bdt (s)
	* return : gtime_t struct
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::bdt2time(int week,double sec)
	{
		gtime_t t=epoch2time(bdt0);

		if (sec<-1E9||1E9<sec) sec=0.0;
		t.time+=86400*7*week+(int)sec;
		t.sec=sec-(int)sec;
		return t;
	}
	/* time to beidouo time (bdt) --------------------------------------------------
	* convert gtime_t struct to week and tow in beidou time (bdt)
	* args   : gtime_t t        I   gtime_t struct
	*          int    *week     IO  week number in bdt (NULL: no output)
	* return : time of week in bdt (s)
	*-----------------------------------------------------------------------------*/
	double gpstime::time2bdt(gtime_t t, int *week)
	{
		gtime_t t0=epoch2time(bdt0);
		time_t sec=t.time-t0.time;
		int w=(int)(sec/(86400*7));

		if (week) *week=w;
		return (double)(sec-w*86400*7)+t.sec;
	}

	/* gpstime to utc --------------------------------------------------------------
	* convert gpstime to utc considering leap seconds
	* args   : gtime_t t        I   time expressed in gpstime
	* return : time expressed in utc
	* notes  : ignore slight time offset under 100 ns
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::gpst2utc(gtime_t t)
	{
		gtime_t tu;
		int i;

		for (i=0;i<(int)sizeof(leaps)/(int)sizeof(*leaps);i++) {
			tu=timeadd(t,leaps[i][6]);
			if (timediff(tu,epoch2time(leaps[i]))>=0.0) return tu;
		}
		return t;
	}

	/* utc to gpstime --------------------------------------------------------------
	* convert utc to gpstime considering leap seconds
	* args   : gtime_t t        I   time expressed in utc
	* return : time expressed in gpstime
	* notes  : ignore slight time offset under 100 ns
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::utc2gpst(gtime_t t)
	{
		int i;

		for (i=0;i<(int)sizeof(leaps)/(int)sizeof(*leaps);i++) {
			if (timediff(t,epoch2time(leaps[i]))>=0.0) return timeadd(t,-leaps[i][6]);
		}
		return t;
	}

	/* gpstime to bdt --------------------------------------------------------------
	* convert gpstime to bdt (beidou navigation satellite system time)
	* args   : gtime_t t        I   time expressed in gpstime
	* return : time expressed in bdt
	* notes  : ref [8] 3.3, 2006/1/1 00:00 BDT = 2006/1/1 00:00 UTC
	*          no leap seconds in BDT
	*          ignore slight time offset under 100 ns
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::gpst2bdt(gtime_t t)
	{
		return timeadd(t,-14.0);
	}

	/* bdt to gpstime --------------------------------------------------------------
	* convert bdt (beidou navigation satellite system time) to gpstime
	* args   : gtime_t t        I   time expressed in bdt
	* return : time expressed in gpstime
	* notes  : see gpst2bdt()
	*-----------------------------------------------------------------------------*/
	gtime_t gpstime::bdt2gpst(gtime_t t)
	{
		return timeadd(t,14.0);
	}


	/* time to day and sec -------------------------------------------------------*/
	double gpstime::time2sec(gtime_t time, gtime_t *day)
	{
		double ep[6],sec;
		time2epoch(time,ep);
		sec=ep[3]*3600.0+ep[4]*60.0+ep[5];
		ep[3]=ep[4]=ep[5]=0.0;
		*day=epoch2time(ep);
		return sec;
	}

	/* utc to gmst -----------------------------------------------------------------
	* convert utc to gmst (Greenwich mean sidereal time)
	* args   : gtime_t t        I   time expressed in utc
	*          double ut1_utc   I   UT1-UTC (s)
	* return : gmst (rad)
	*-----------------------------------------------------------------------------*/
	double gpstime::utc2gmst(gtime_t t, double ut1_utc)
	{
		const double ep2000[]={2000,1,1,12,0,0};
		gtime_t tut,tut0;
		double ut,t1,t2,t3,gmst0,gmst;

		tut=timeadd(t,ut1_utc);
		ut=time2sec(tut,&tut0);
		t1=timediff(tut0,epoch2time(ep2000))/86400.0/36525.0;
		t2=t1*t1; t3=t2*t1;
		gmst0=24110.54841+8640184.812866*t1+0.093104*t2-6.2E-6*t3;
		gmst=gmst0+1.002737909350795*ut;

		return fmod(gmst,86400.0)*PI/43200.0; /* 0 <= gmst <= 2*PI */
	}

	gpstime::~gpstime(){ /* destructor */

	}

}// namespace



