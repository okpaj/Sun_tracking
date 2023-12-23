/*
 * astro_driver.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Okpaj
 */

#include <stdint.h>
#include <math.h>

#include "astro_driver.h"
#include "astro.h"

void astro_calculate(struct astro_data_t *adh) {

    adh->UT 		= UT_from_LCT(adh->loc_time, adh->zone, adh->daylight);
    adh->Days_J2000 = Days_from_J2000(0.0f, adh->day, adh->month, adh->year);
    adh->GMSTH 		= GMST(adh->Days_J2000, adh->UT);
    adh->Days_J2000 = Days_from_J2000(adh->UT, adh->day, adh->month, adh->year);
    adh->SL 		= Sun_Longitude_Deg(adh->Days_J2000);
    adh->obliq 		= Ecliptic_mean_obliquity_deg(adh->Days_J2000);
    adh->RA_rad 	= Sun_RAscension_rad(D2R(adh->SL), D2R(adh->obliq));
    adh->Declin		= Sun_sin_declination(D2R(adh->SL), D2R(adh->obliq));
    //adh->LHA 		= Local_Hour_Angle_rad(adh->GMSTH, adh->RA_rad, D2R(18.0f));
    adh->LHA 		= Local_Hour_Angle_rad(adh->GMSTH, adh->RA_rad, adh->geo_long);
    adh->dec 		= asinf(adh->Declin);
    //adh->geo_lat 	= D2R(50.0f);
    //adh->sgl 		= sinf(adh->geo_lat);
    //adh->cgl 		= cosf(adh->geo_lat);
    /*
     * TODO use Declin as sin in sin_altitude calling,
     * TODO also use native sqrt to calc cos(dec)
     * TODO remove calc of dec as asin(DEclin) above
     */
    adh->sa 		= sin_altitude(cosf(adh->LHA), cosf(adh->dec), sinf(adh->dec), \
    		adh->cgl, adh->sgl);
    adh->Alt		= asinf(adh->sa);
    adh->Alt_deg	= R2D(adh->Alt);
    adh->Az 		= Azimuth_rad(sinf(adh->LHA), cosf(adh->LHA), tanf(adh->dec), \
    		adh->cgl, adh->sgl);
    adh->Az_deg 	=  R2D(adh->Az);

   	if (adh->Az_deg < 0) adh->Az_deg += 360;

}

float astro_get_azimuth_deg(struct astro_data_t *adh) {
	return adh->Az_deg;
}

float astro_get_altitude_deg(struct astro_data_t *adh) {
	return adh->Alt_deg;
}

static uint8_t astro_stepper_calculate(struct astro_data_t *adh, uint8_t step) {
	uint8_t next_step;


	next_step = step;

	switch (step) {
	case 0:
		++next_step;
		break;
	case 1:
		adh->UT 		= UT_from_LCT(adh->loc_time, adh->zone, adh->daylight);
	    adh->Days_J2000 = Days_from_J2000(0.0f, adh->day, adh->month, adh->year);
	    adh->GMSTH 		= GMST(adh->Days_J2000, adh->UT);
	    adh->Days_J2000 = Days_from_J2000(adh->UT, adh->day, adh->month, adh->year);
		++next_step;
		break;
	case 2:
	    adh->SL 		= Sun_Longitude_Deg(adh->Days_J2000);
		++next_step;
	    break;
	case 3:
	    adh->obliq 		= Ecliptic_mean_obliquity_deg(adh->Days_J2000);
		++next_step;
	    break;
	case 4:
	    adh->RA_rad 	= Sun_RAscension_rad(D2R(adh->SL), D2R(adh->obliq));
		++next_step;
	    break;
	case 5:
	    adh->Declin		= Sun_sin_declination(D2R(adh->SL), D2R(adh->obliq));
		++next_step;
	    break;
	case 6:
	    //adh->LHA 		= Local_Hour_Angle_rad(adh->GMSTH, adh->RA_rad, D2R(18.0f));
	    adh->LHA 		= Local_Hour_Angle_rad(adh->GMSTH, adh->RA_rad, adh->geo_long);
	    adh->dec 		= asinf(adh->Declin);
		++next_step;
	    break;
	case 7:
	    //adh->geo_lat 	= D2R(50.0f);
	    //adh->sgl 		= sinf(adh->geo_lat);
		++next_step;
	    break;
	case 8:
	    //adh->cgl 		= cosf(adh->geo_lat);
		++next_step;
	    break;
	case 9:
	    adh->sa 		= sin_altitude(cosf(adh->LHA), cosf(adh->dec), sinf(adh->dec), \
	    		adh->cgl, adh->sgl);
		++next_step;
	    break;
	case 10:
		adh->Alt		= asinf(adh->sa);
		adh->Alt_deg	= R2D(adh->Alt);
		++next_step;
		break;
	case 11:
	    adh->Az 		= Azimuth_rad(sinf(adh->LHA), cosf(adh->LHA), tanf(adh->dec), \
	    		adh->cgl, adh->sgl);
		++next_step;
	    break;
	case 12:
	    adh->Az_deg 	=  R2D(adh->Az);
	   	if (adh->Az_deg < 0) {
	   		adh->Az_deg += 360;
	   	}
	   	next_step = 0;
	   	break;
	default:
		next_step = 0;
		break;
	}

	   	return next_step;
}

void astro_init(struct astro_data_t *adh) {
	;
}

void astro_set_geo(struct astro_data_t *adh, float geo_lat, float geo_long, int zone, int daylight) {

	adh->geo_lat 	= D2R(geo_lat);
	adh->geo_long 	= D2R(geo_long);
    adh->sgl 		= sinf(adh->geo_lat);
    adh->cgl 		= cosf(adh->geo_lat);

	adh->zone = zone;
	adh->daylight = daylight;
}

void astro_set_date(struct astro_data_t *adh, int day, int month, int year) {
	adh->day = day;
	adh->month = month;
	adh->year = year;
}

void astro_set_local_time(struct astro_data_t *adh, int hour, int minute, int second) {
	adh->loc_time = (float)hour + ((float)minute)/60 + ((float)second)/3600;
}

void astro_iterate_start(struct astro_iter_t *aih) {
	aih->curr_iteration = astro_stepper_calculate(aih->adh, 0);
}

void astro_iterate_advance(struct astro_iter_t *aih) {
	aih->curr_iteration = astro_stepper_calculate(aih->adh, aih->curr_iteration);
}

uint8_t astro_iterate_done(struct astro_iter_t *aih) {
	return !(aih->curr_iteration);
}

void astro_iterate_set_astro(struct astro_iter_t *aih, struct astro_data_t *adh) {
	aih->adh = adh;
	aih->curr_iteration = 0;
}
