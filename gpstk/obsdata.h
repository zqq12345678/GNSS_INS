/*==============================================================================
*  This file is part of GPSTk, the GPS Toolkit.
*  
*  Copyright (C/C++) 2020, Beihang University All Rights Reserved.
*
*  Author: ZHANG Q.Q; E-mail:zhangqieqie@buaa.edu.cn
*  
*  version: $Revision 1.0 $Data: 2020/04/17 18:29:00 $
*
*  history: 2020/04/17	1.0 new
						int addobsdata(obsd_t *data);
						obsd_t* getobsdata(int epoch);
						obsd_t* getobsdata(gtime_t t);
						obsd_t* getobsdata(const double *ep);
						int	getobsnum(void);
						int	sortobs(void);
*==============================================================================*/
/**
 * @file ObsData.hpp
 * read observation data from rinexx.x file, etc.
 */
#ifndef ObsData_H_
#define ObsData_H_

#include <vector>
#include "constant.h"
#include "gpstime.h"

namespace gpstk
{

	struct obsd_t{				/* observation data record */
		gtime_t time;			/* receiver sampling time (GPST) */
		unsigned char sat,rcv;	/* satellite/receiver number */
		unsigned char SNR [NFREQ]; /* signal strength (0.25 dBHz) */
		unsigned char LLI [NFREQ]; /* loss of lock indicator */
		unsigned char code[NFREQ]; /* code indicator (CODE_???) */
		double L[NFREQ];		/* observation data carrier-phase (cycle) */
		double P[NFREQ];		/* observation data pseudorange (m) */
		float  D[NFREQ];		/* observation data doppler frequency (Hz) */
	};

	int cmpobs(const void *p1, const void *p2);/* it can't be a member function */

	class obs_t /* class ObsData */
	{

		public:

			obs_t();						/* constructor */
			
			int addobsdata(obsd_t *data);	/* add observation data to vector */
			obsd_t* getobsdata(int epoch);	/* get the observation data at an epoch */
			obsd_t* getobsdata(gtime_t t);	/* get the observation data at time */
			obsd_t* getobsdata(const double *ep);/* get the observation data at year/month/day/hour/min/sec */
			int	getobsnum(void);				/* get the number of observations */
			int	sortobs(void);					/* sort the observation by time */

			virtual ~obs_t();				/* destructor */

		private:
			int mn;					/* recorded the number of observation */
			std::vector<obsd_t> mobs; /* vector for storage observation data */


	};  /* class ObsData */

}// namespace

#endif //OBSDATA_H_