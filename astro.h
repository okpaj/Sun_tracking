/*
 * astro.h
 *
 *  Created on: Nov 18, 2023
 *      Author: Okpaj
 */

#ifndef ASTRO_H_INCLUDED
#define ASTRO_H_INCLUDED

float D2R(float d_);
float R2D(float r_);
float H2R(float H_);
float R2H(float h_);

float UT_from_LCT(float loc_time, int zone, int daylight );

float Days_from_J2000(float UT, int day, int month, int year);

// number of TT centuries since the year 2000
float Centuries_from_2000(float JD_2000);

float GMST(float Centuries, float H);
//float GMST(float GMST_UT0, float H);

float Local_Hour_Angle_rad(float GMTS_h, float RA_h, float obs_long_deg);

float Azimuth_rad(float sin_LHA, float cos_LHA, float tan_dec, \
                  float cos_geo_lat, float sin_geo_lat);
float sin_altitude(float cos_LHA, float cos_dec, float sin_dec, \
                  float cos_geo_lat, float sin_geo_lat);

float Sun_Longitude_Deg(float D);
float Sun_RAscension_rad(float L_rad, float obliq_rad);

float Sun_sin_declination(float L_rad, float obliq_rad);

float Ecliptic_mean_obliquity_deg(float D);

float Sun_set_time(void);


// Greenwich mean sidereal time in hours
// D = number of days since J2000
//float GMST_simplified(float D);

//float Sun_Azimuth(float D);


#endif // ASTRO_H_INCLUDED
