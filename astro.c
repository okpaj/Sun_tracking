/*
 * astro.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Okpaj
 */


#include "astro.h"
#include <math.h>
#include <stdio.h>

const float pif = 3.141592653f;


float D2R(float d_) { return ((d_ / 180) * pif); }
float R2D(float r_) { return ((r_ * 180) / pif); }
float H2R(float H_) { return (((H_ * 15) /180) * pif);}
float R2H(float h_) { return ((h_* 180) / pif) /15;}

float UT_from_LCT(float loc_time, int zone, int daylight )
{
    return (loc_time - zone - daylight);
};


float Days_from_J2000(float UT, int day, int month, int year)
{

    int A, B, C;
    float jd;

    A = 367 * year;
    B = floor( (7 * (year + floor((month+9)/12.0))) /4.0 );
    C = floor((275*month)/9.0);

    jd = A - B + C + day + UT/24 - 730531.5;// 1721013.5-2451545;

    return jd;
}

// number of TT centuries since the year 2000
float Centuries_from_2000(float JD_2000)
{
    return JD_2000 /  36525;
};

// Greenwich mean sidereal time in hours at 0h UT1
// D - number of TT centuries since the year 2000
float GMST(float D_UT, float H)
{
    float f;
    f = 6.697375f + 0.065709824279f*D_UT + 1.0027379f*H;
    //f = fmaf(1.0027379f, H, 6.697375f);
    //f = fmaf(0.065709824279f, D_UT ,f);
    //while (f > 24) {f -= 24; }
    f = fmodf(f, 24);

    return f;
}
/*
float GMST(float GMST_UT0, float H)
{
    float f;
    f = GMST_UT0 + 1.0027379*H ;
    while (f > 24) {f -= 24; }

    return f;
}
*/
float Local_Hour_Angle_rad(float GMTS_h, float RA_rad, float obs_long_rad)
{
    float LHA;

    LHA = H2R(GMTS_h) - RA_rad + obs_long_rad;
    while (LHA < 0) LHA += 2*pif;
    while (LHA > 2*pif) LHA -= 2*pif;

    return LHA;
}

float Azimuth_rad(float sin_LHA, float cos_LHA, float tan_dec, \
                  float cos_geo_lat, float sin_geo_lat)
{
    float f, m;

    m = tan_dec * cos_geo_lat - sin_geo_lat * cos_LHA;

    //f = atan2( m, -sin_LHA);
    f = atan2( -sin_LHA, m);

    return f;
}

float sin_altitude(float cos_LHA, float cos_dec, float sin_dec, \
                  float cos_geo_lat, float sin_geo_lat)
{
    float f;

    f = cos_LHA*cos_dec*cos_geo_lat+sin_dec*sin_geo_lat;

    return f;
}

/*
 * // https://aa.usno.navy.mil/faq/GAST
 * float GMST_simplified(float D)
 * {
 *     float f;
 *     f = 18.697375 + 24.065709824279*D;
 *     while (f > 24) {f -= 24; }
 *
 *     return f;
 * }
 */

float Sun_Longitude_Deg(float D)
{
    float g, q, L;
    float gr;

    // g, q, and L are in degrees
    // Mean anomaly of the Sun:
    g = 357.529f + 0.98560028f * D;
    //g = fmaf(0.98560028f , D, 357.529f);

    if (g >= 360) //while (g > 360)
    { //g -= 360;
            g = fmodf(g, 360);
    } else {
        while (g < 0) { g += 360;}
    }

    // Mean longitude of the Sun:
    q = 280.459f + 0.98564736f * D;
    //q = fmaf(0.98564736f, D, 280.459f);
    if (q >= 360) //while (q > 360)
    { //q -= 360;
        q = fmodf(q, 360);
    } else {
        while (q < 0) { q += 360;}
    }

    // Sun's geocentric apparent ecliptic longitude (adjusted for aberration):

    gr = D2R(g);
    L = q + 1.915f*sinf(gr) + 0.020f*sinf(2*gr);

    //L = fmaf(1.915f, sinf(gr), q);
    //L = fmaf(0.020f, sinf(2*gr), L);

    return L;
}

float Sun_RAscension_rad(float L_rad, float obliq_rad)
{
    float RA;
    float s, c;
    int znS,znC;
    /*
    S = sinf(L_rad);
    C = cosf(L_rad);

    if (sinf(L_rad) < 0 ) S = 0;
    if (cosf(L_rad) < 0 ) C = 0;
    */
    //tan_RA = atan2(cosf(L_rad), cosf(obliq_rad)*sinf(L_rad));
    s = cosf(obliq_rad)*sinf(L_rad);
    c = cosf(L_rad);

    // atan(y/x); returns value [-pi, +pi]
    RA = atan2f( s, c);

    if (s >= 0.0f) znS = 1; else znS = -1;
    if (c >= 0.0f) znC = 1; else znC = -1;

    if ( (s > 0) && (c > 0) ) ; // 1Q nothing to do
    if ( (s > 0) && (c < 0) ) ; //2Q
    if ( (s < 0) && (c < 0) ) RA += 2*pif; //3Q ; +360
    if ( (s < 0) && (c > 0) ) RA += 2*pif; //4Q ; +360 to have positive value

    return RA;
}

float Sun_sin_declination(float L_rad, float obliq_rad)
{
    float sindec;

    sindec = sinf(obliq_rad)* sinf(L_rad);

    return sindec;
}

float Ecliptic_mean_obliquity_deg(float D)
{
    //mean obliquity of the ecliptic, in degrees:
     return 23.439f-0.36e-6f*D;
    //return fmaf(-0.36e-6f, D, 23.439f);
}


