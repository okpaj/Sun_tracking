/*
 * astro_driver.h
 *
 *  Created on: Nov 18, 2023
 *      Author: Okpaj
 */

#ifndef INC_ASTRO_DRIVER_H_
#define INC_ASTRO_DRIVER_H_

#include <stdint.h>

/*
 * TODO change names to be meaningfull
 */
struct astro_data_t {
	float loc_time;
	int zone, daylight;
	int day, month, year;
	float geo_lat, geo_long;

	float UT;
	float Days_J2000;
	float T;
	float GMSTH;

	float SL, RA_rad, RA, RA_degs, Declin, obliq, LHA;
	float Az, tan_Az, Az_deg;
	float sa, Alt, Alt_deg;
	float dec, sgl, cgl;
};

struct astro_iter_t {
	uint8_t curr_iteration;
	struct astro_data_t *adh;
};

void astro_init(struct astro_data_t *);
void astro_set_geo(struct astro_data_t *, float geo_lat, float geo_long, int zone, int daylight);
void astro_set_date(struct astro_data_t *, int day, int month, int year);
void astro_set_local_time(struct astro_data_t *, int hour, int minute, int second);
void astro_calculate(struct astro_data_t *);
float astro_get_azimuth_deg(struct astro_data_t *adh);
float astro_get_altitude_deg(struct astro_data_t *adh);
// uint8_t astro_stepper_calculate(struct astro_data_t *adh, uint8_t step);

void astro_iterate_start(struct astro_iter_t *aih);
void astro_iterate_advance(struct astro_iter_t *aih);
uint8_t astro_iterate_done(struct astro_iter_t *aih);
void astro_iterate_set_astro(struct astro_iter_t *aih, struct astro_data_t *adh);


#endif /* INC_ASTRO_DRIVER_H_ */
