/*==============================================================================
*  This file is part of GPSTk, the GPS Toolkit.
*  
*  Copyright (C/C++) 2020, Beihang University All Rights Reserved.
*
*  Author: ZHANG Q.Q
*  E-mail: zhangqieqie@buaa.edu.cn
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
 * @file ObsData.cpp
 * operations relating to observation data 
 */

#include <iostream>
#include <algorithm>

#include "Constant.h"
#include "ObsData.h"


namespace gpstk
{
	
	obs_t::obs_t(){  /* constructor */

		this->mn=0;	/* initial counter */
		this->mobs.clear();/* clear all the data in the vector */

	}

	/* get obs number -------------------------------------------------------------
	* get the number of observation
	* args   : void
	* return : the number of observations in vector
	*-----------------------------------------------------------------------------*/
	int obs_t::getobsnum(void){

		return this->mn; /* return the number of observation */

	}

	/* add observation data to vector ---------------------------------------------
	* get the number of observation
	* args   : obsd_t *data
	* return : the number of observations in vector
	*-----------------------------------------------------------------------------*/
	int obs_t::addobsdata(obsd_t* data){/* add observation data */
		
		this->mn++;/* add a new observation */
		mobs.push_back(*data);/* push observation into vector */
		return this->mn; /* return the current number of observations */
	}

	/* get the observation data from vector ---------------------------------------
	* get the observation data at epoch/time from vector
	* args   : int epoch
	* return : obsd_t* struct
	*-----------------------------------------------------------------------------*/
	obsd_t* obs_t::getobsdata(int epoch)
	{

		if (epoch<=this->mn) { /* return the observation data */
			return &this->mobs[epoch];
		}
		else{
			/* an error message */
			return NULL;
		}
	}
	/* get observation data based on gps time */
	obsd_t* obs_t::getobsdata(gtime_t t)
	{
		
		gpstime time; /* instantiable gpstime class */
		
		for(int i=0;i<this->mn;i++){
			if(abs(time.timediff(this->mobs[i].time,t))<=DTTOL){

				return &this->mobs[i];
			}
		}
		return NULL;
	}
	/* get observation data based on utc time */
	obsd_t* obs_t::getobsdata(const double *ep)
	{

		gpstime time; /* instantiable gpstime class */

		gtime_t t=time.epoch2time(ep); /*convert calendar time to time*/

		for(int i=0;i<this->mn;i++){
			if(abs(time.timediff(this->mobs[i].time,t))<=DTTOL){

				return &this->mobs[i];
			}
		}
		return NULL;
	}

	/* compare observation data --------------------------------------------------*/
	int cmpobs(const void *p1, const void *p2)
	{
		gpstime time;
		obsd_t *q1=(obsd_t *)p1,*q2=(obsd_t *)p2;
		double tt=time.timediff(q1->time,q2->time);

		if (fabs(tt)>DTTOL) return tt<0?-1:1;
		if (q1->rcv!=q2->rcv) return (int)q1->rcv-(int)q2->rcv;
		return (int)q1->sat-(int)q2->sat;
	}

	/* sort and unique observation data --------------------------------------------
	* sort and unique observation data by time, rcv, sat
	* args   : obs_t *obs    IO     observation data
	* return : number of epochs
	*-----------------------------------------------------------------------------*/
	int	obs_t::sortobs(void)
	{
		int i,j,n;
		gpstime time;

		if (this->mn<=0) return 0;
		
		qsort(&this->mobs,this->mn,sizeof(obsd_t),cmpobs);

		/* delete duplicated data */
		for (i=j=0;i<this->mn;i++) {
			if (this->mobs[i].sat!=this->mobs[j].sat||
				this->mobs[i].rcv!=this->mobs[j].rcv||
				time.timediff(this->mobs[i].time,this->mobs[j].time)!=0.0) {
					this->mobs[++j]=this->mobs[i];
			}
		}
		this->mn=j+1;

		for (i=n=0;i<this->mn;i=j,n++) {
			for (j=i+1;j<this->mn;j++) {
				if (time.timediff(this->mobs[j].time,this->mobs[i].time)>DTTOL) break;
			}
		}
		return n;

	}


	obs_t::~obs_t(){ /* destructor */

	}

}// namespace
