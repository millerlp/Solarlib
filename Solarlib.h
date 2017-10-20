/* Solarlib.h		Luke Miller December 2012
 * Released into the public domain (originally based on U.S. Govt. products)
 * No warranty given or implied.
 * 
 * A library of functions for Arduino to calculate aspects of solar position
 * in the sky using a time value, latitude, longitude, and time zone.
 * Output includes estimates of current sun elevation and azimuth (position in 
 * the sky), sunrise, solar noon, and sunset times for the current day, and 
 * various other statistics for the sun at the given time. Results should be
 * accurate for the years 1901 to 2099, for locations between +/- 72 latitude.
 * Calculations are based on spreadsheet and information found at:
 * http://www.esrl.noaa.gov/gmd/grad/solcalc/calcdetails.html
 * 
 * Initialize the solar calculator using the initSolarCalc() function, which
 * lets you specify:
 * tzOffset 	- time zone Offset from Greenwich Mean Time (UTC). Time zones 
 * 				west of GMT should be given negative values. 
 * 				For example, Pacific Standard Time is -8 
 * 				
 * lat			- Latitude of the location you want to use. Values north of the
 * 				equator are positive, given in decimal degrees.
 * 				 				
 * lon			- Longitude of the location you want to use. Values in the 
 * 				western hemisphere have negative values (0 to -180), given in 
 * 				decimal degrees. 
 * 				For example: Monterey, California has lat/lon (36.62, -121.904)
 * 				
 * After running initSolarCalc() in the setup loop, you can call any of the 
 * extractor functions to get the position of the sun and sunrise/sunset times.
 * Each extractor function requires a Time value as input, from the Time 
 * library. The Time is given as seconds since 1970-1-1 00:00 (unix epoch).
 * Useful functions, supplied with a time value t as the sole argument:
 * 		getSAA(t)		- Solar Azimuth (degrees clockwise from North)
 * 		getSEC_Corr(t)	- Solar Elevation, corrected for diffraction (degrees)
 * 		getSZA(t)		- Solar Zenith angle (degrees below straight up)
 * 		getSunriseTime(t)	- Sunrise Time for the current day 	(Time object)
 * 		getSunsetTime(t)	- Sunset Time for the current day	(Time object)
 * 		getSolarNoonTime(t)	- Solar Noon for the current day	(Time object)
 * 		getSunDuration(t)	- Minutes of Sunlight for the current day	
 * 		Many more found below...			 			
 *
 * **WARNING**
 * Only tested on 32-bit ARM Teensy 3.0. This library will probably fail 
 * horribly on 8-bit AVR products due to limits in precision of double/float 
 * values. You can check your results against the NOAA calculator:
 * http://www.esrl.noaa.gov/gmd/grad/solcalc/

*/
#ifndef Solarlib_h
#define Solarlib_h

#include "Arduino.h"
#include "math.h"
#include "Time.h"

#define julianUnixEpoch  2440587.5 // julian days to start of unix epoch

typedef struct {
	int tzOffset;	// Time zone Offset, zones west of GMT are negative
	double lat;		// Latitude of site, values north of equator are positive
	double lon;		// Longitude of site, values west of GMT are negative
    double timeFracDay;  // Fraction of day past midnight for current time
    long unixDays;  // Days since 1970-1-1
    double JDN;     // Julian Day Number
    double JCN;     // Julian Century Number
    double GMLS;    // Geometric Mean Longitude of Sun
    double GMAS;    // Geometric Mean Anomaly of Sun
    double EEO;     // Eccentricity of Earth Orbit (degrees)
    double SEC;     // Sun Equation of Center 
    double STL;     // Sun True Longitude (degrees)
    double STA;     // Sun True Anomaly (degrees)
    double SRV;     // Sun Radian Vector (degrees)
    double SAL;     // Sun Apparent Longitude 
    double MOE;     // Mean Oblique Ecliptic (degrees)
    double OC;      // Oblique correction 
    double SRA;     // Sun Right Ascension (degrees)
    double SDec;    // Sun Declination (degrees)
    double vy;		// var y
    double EOT;     // Equation of Time (minutes)
    double HAS;     // Hour Angle Sunrise (degrees)
    double SolarNoonfrac;       // Solar noon (fractional day)
    double SolarNoonDays;   // Solar Noon (days since 1970-1-1)
    time_t SolarNoonTime;   // Solar Noon time (Time Object)
    double Sunrise;     // Sunrise time (unix time, seconds)
    time_t SunriseTime; // Sunrise time (Time object)
    double Sunset;      // Sunset times (unix time, seconds)
    time_t SunsetTime;  // Sunset time (Time object)
    double SunDuration; // Sunlight Duration (minutes)
    double TST;     // True Solar Time (minutes)
    double HA;      // Hour Angle (degrees)
    double SZA;  // Solar Zenith Angle (degrees)
    double SEA;     // Solar Elevation Angle (degrees)
    double AAR;  // Approximate Atmospheric Refraction 
    double SEC_Corr; // Solar Elevation, Corrected (degrees)
    double SAA; // Solar Azimuth Angle (degrees)
} SolarElements;
//----------------------------------------------------------------------------
// Functions
// Initialization function to put time zone offset, latitude, and longitude in
// the SolarElements structure
void initSolarCalc(int tzOffset, double lat, double lon);
// Return latitude used in solar calculations
double getlat(); 
// Return longitude used in solar calculations
double getlon(); 
// Return time zone offset. Zones west of GMT are negative
int gettzOffset(); 
// Get the fraction of the day past midnight for the current time
double gettimeFracDay(time_t t);
// Get whole days since 1970-1-1
long getunixDays(time_t t);
// Extract Julian Day number (adjusted to GMT time zone)
double getJDN(time_t t);
// Extract Julian Century
double getJCN(time_t t);	
// Extract Geometric Mean Longitudes of Sun (degrees)
double getGMLS(time_t t); 
// Extract Geometric Mean Anomaly of the Sun
double getGMAS(time_t t);
// Extract Eccentricity of Earth Orbit
double getEEO(time_t t);
// Extract Sun Equation of Center
double getSEC(time_t t);
// Extract Sun True Longitude (degrees)  
double getSTL(time_t t);
// Extract Sun True Anomaly (degrees)
double getSTA(time_t t);
// Extract Sun Radian Vector (Astronomical Units)
double getSRV(time_t t);
// Extract Sun Apparent Longitude (degrees)
double getSAL(time_t t);
// Extract Mean Oblique Ecliptic (degrees)
double getMOE(time_t t);
// Extract Oblique correction (degrees)
double getOC(time_t t);
// Extract Sun Right Ascension (degrees)
double getSRA(time_t t);
// Extract Sun Declination (degrees)
double getSDec(time_t t);
// Extract var y
double getvy(time_t t);
// Extract Equation of Time (minutes)
double getEOT(time_t t);
// Extract Hour Angle Sunrise (degrees)
double getHAS(time_t t);
// Extract Solar Noon (fraction of a day)
double getSolarNoonfrac(time_t t);
// Extract Solar Noon Days (days since 1970-1-1, local time zone)
double getSolarNoonDays(time_t t);
// Extract Solar Noon Time (Time object, seconds since 1970-1-1)
time_t getSolarNoonTime(time_t t);
// Extract Sunrise (seconds since 1970-1-1, local time zone)
double getSunrise(time_t t);
// Extract Sunrise as Time object (seconds since 1970-1-1, local time zone)
time_t getSunriseTime(time_t t);
// Extract Sunset time (seconds since 1970-1-1, local time zone)
double getSunset(time_t t);
// Extract Sunset as Time object (seconds since 1970-1-1, local time zone)
time_t getSunsetTime(time_t t);
// Extract Sunlight Duration (day length, minutes)
double getSunDuration(time_t t);
// Extract True Solar Time (minutes)
double getTST(time_t t);
// Extract Hour Angle (degrees)
double getHA(time_t t);
// Extract Solar Zenith Angle (degrees)
double getSZA(time_t t);
// Extract Solar Elevation Angle (degrees above horizontal)
double getSEA(time_t t);
// Extract Approximate Atmospheric Refraction (degrees)
double getAAR(time_t t);
// Extract Solar Elevation Corrected for Atmospheric refraction (degrees)
double getSEC_Corr(time_t t);
// Extract Solar Azimuth Angle (degrees clockwise from North)
double getSAA(time_t t);

// Main function to update the contents of the Solar Elements structure SE with
// new solar calculations, using the given Time t input. The initSolarCalc()
// function must previously have been run once so that the appropriate time zone
// offset, latitude, and longitude are set. 
void calcSolar(time_t t, SolarElements &SE);

#endif