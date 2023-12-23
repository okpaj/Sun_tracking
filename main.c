/*
 *  Created on: Dec 23, 2023
 *      Author: Okpaj
 */

#include <stdio.h>
#include <stdlib.h>
#include "astro.h"
#include "astro_driver.h"

struct astro_data_t adh, adhit;
struct astro_iter_t aih;

int main()
{
    /* input */
    int day = 23, month = 12, year = 2023;
    int hour = 12, minute = 0, seconds = 0;
    float latitude = 50.0f, longitude = 19.0f;
    int zone = 1, daylight = 0;

    /* output */
    float curr_sun_az_deg, curr_sun_alt_deg;


    /*
     * Batch mode calculations
    */

    astro_init(&adh);
	astro_set_geo(&adh, latitude, longitude, zone, daylight);
	astro_set_date(&adh, day, month, year);
	astro_set_local_time(&adh, hour, minute, seconds);

	astro_calculate(&adh);

	curr_sun_az_deg = astro_get_azimuth_deg(&adh);
	curr_sun_alt_deg = astro_get_altitude_deg(&adh);

	printf("Sun position - batch calculations\n");
    printf("Azimuth %0.2f\n", curr_sun_az_deg);
    printf("Altitude %0.2f\n", curr_sun_alt_deg);


    /*
     * calculation with iterator
     */

    astro_init(&adhit);
	astro_set_geo(&adhit, latitude, longitude, zone, daylight);
	astro_set_date(&adhit, day, month, year);
	astro_set_local_time(&adhit, hour, minute, seconds);

    astro_iterate_set_astro(&aih, &adhit);
    astro_iterate_start(&aih);
	while ( !astro_iterate_done(&aih) ) {
		astro_iterate_advance(&aih);
	}

	curr_sun_az_deg = astro_get_azimuth_deg(&adhit);
	curr_sun_alt_deg = astro_get_altitude_deg(&adhit);

	printf("Sun position - iterator calculations\n");
    printf("Azimuth %0.2f\n", curr_sun_az_deg);
    printf("Altitude %0.2f\n", curr_sun_alt_deg);


    return 0;
}
