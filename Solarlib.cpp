/* Solarlib.cpp		Luke Miller December 2012
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
 * values. Developed on Arduino software version 1.0.2 
 * 
 * You can check your results against the NOAA calculator:
 * http://www.esrl.noaa.gov/gmd/grad/solcalc/

*/
#include "Arduino.h"
#include "math.h"
#include "Time.h"
#include "Solarlib.h"

static SolarElements SE; // cache that holds all calculated values

// initSolar function
void initSolarCalc(int tzOffset, double lat, double lon){
	SE.tzOffset = tzOffset; // Set time zone offset
	SE.lat = lat;	// Set current site latitude
	SE.lon = lon;	// Set current site longitude
}
// Return time zone offset when user asks for it. 
// Zones west of GMT are negative.
int gettzOffset(){
	return SE.tzOffset;
}
// Return latitude when user asks for it.
double getlat(){
	return SE.lat;
}
// Return longitude when user asks for it.
double getlon(){
	return SE.lon;
}
double gettimeFracDay(time_t t){
	calcSolar(t, SE);
	return SE.timeFracDay;
}
long getunixDays(time_t t){
	calcSolar(t, SE);
	return SE.unixDays;
}
double getJDN(time_t t){
	calcSolar(t, SE);
	return SE.JDN;
}
// Extract Julian Century
double getJCN(time_t t){
	calcSolar(t, SE);
	return SE.JCN;
}
// Extract GMLS
double getGMLS(time_t t){
	calcSolar(t, SE);
	return SE.GMLS;
}
double getGMAS(time_t t){
	calcSolar(t, SE);
	return SE.GMAS;
}
// Extract Eccentricity of Earth Orbit
double getEEO(time_t t){
	calcSolar(t, SE);
	return SE.EEO;
}
// Extract Sun Equation of Center
double getSEC(time_t t){
	calcSolar(t, SE);
	return SE.SEC;
}
// Extract Sun True Longitude (degrees)  
double getSTL(time_t t){
	calcSolar(t, SE);
	return SE.STL;
}
// Extract Sun True Anomaly (degrees)
double getSTA(time_t t){
	calcSolar(t, SE);
	return SE.STA;
}
// Extract Sun Radian Vector
double getSRV(time_t t){
	calcSolar(t, SE);
	return SE.SRV;
}
// Extract Sun Apparent Longitude (degrees)
double getSAL(time_t t){
	calcSolar(t, SE);
	return SE.SAL;
}
// Extract Mean Oblique Ecliptic (degrees)
double getMOE(time_t t){
	calcSolar(t, SE);
	return SE.MOE;
}
// Extract Oblique correction (degrees)
double getOC(time_t t){
	calcSolar(t, SE);
	return SE.MOE;
}
// Extract Sun Right Ascension (degrees)
double getSRA(time_t t){
	calcSolar(t, SE);
	return SE.SRA;
}
// Extract Sun Declination (degrees)
double getSDec(time_t t){
	calcSolar(t, SE);
	return SE.SDec;
}
// Extract var y
double getvy(time_t t){
	calcSolar(t, SE);
	return SE.vy;
}
// Extract Equation of Time
double getEOT(time_t t){
	calcSolar(t, SE);
	return SE.EOT;
}
// Extract Hour Angle Sunrise (degrees)
double getHAS(time_t t){
	calcSolar(t, SE);
	return SE.HAS;
}
// Extract Solar Noon (fraction of a day)
double getSolarNoonfrac(time_t t){
	calcSolar(t,SE);
	return SE.SolarNoonfrac;
}
// Extract Solar Noon Days (days since 1970-1-1, local time zone)
double getSolarNoonDays(time_t t){
	calcSolar(t, SE);
	return SE.SolarNoonDays;
}
// Extract Solar Noon Time (Time object, seconds since 1970-1-1)
time_t getSolarNoonTime(time_t t){
	calcSolar(t, SE);
	return SE.SolarNoonTime;
}
// Extract Sunrise (seconds since 1970-1-1, local time zone)
double getSunrise(time_t t){
	calcSolar(t, SE);
	return SE.Sunrise;
}
// Extract Sunrise as Time object (seconds since 1970-1-1, local time zone)
time_t getSunriseTime(time_t t){
	calcSolar(t, SE);
	return SE.SunriseTime;
}
// Extract Sunset (seconds since 1970-1-1, local time zone)
double getSunset(time_t t){
	calcSolar(t, SE);
	return SE.Sunset;
}
// Extract Sunset as Time object (seconds since 1970-1-1, local time zone)
time_t getSunsetTime(time_t t){
	calcSolar(t, SE);
	return SE.SunsetTime;
}
// Extract Sunlight Duration (day length, minutes)
double getSunDuration(time_t t){
	calcSolar(t, SE);
	return SE.SunDuration;
}
// Extract True Solar Time (minutes)
double getTST(time_t t){
	calcSolar(t, SE);
	return SE.TST;
}
// Extract Hour Angle (degrees)
double getHA(time_t t){
	calcSolar(t, SE);
	return SE.HA;
}
// Extract Solar Zenith Angle (degrees)
double getSZA(time_t t){
	calcSolar(t, SE);
	return SE.SZA;
}
// Solar Elevation Angle (degrees above horizontal)
double getSEA(time_t t){
	calcSolar(t, SE);
	return SE.SEA;
}
// Approximate Atmospheric Refraction (degrees)
double getAAR(time_t t){
	calcSolar(t, SE);
	return SE.AAR;
}
// Solar Elevation Corrected for Atmospheric refraction (degrees)
double getSEC_Corr(time_t t){
	calcSolar(t, SE);
	return SE.SEC_Corr;
}
// Extract Solar Azimuth Angle (degrees clockwise from North)
double getSAA(time_t t){
	calcSolar(t, SE);
	return SE.SAA;
}

// Main function to calculate solar values. Requires a time value (seconds since
// 1970-1-1) as input. 
void calcSolar(time_t t, SolarElements &SE){
    // Calculate the time past midnight, as a fractional day value
	// e.g. if it's noon, the result should be 0.5.
	SE.timeFracDay = ((((double)(second(t)/60) + minute(t))/60) +
                   hour(t))/24;
    // unixDays is the number of whole days since the start
    // of the Unix epoch. The division sign will truncate any remainder
    // since this will be done as integer division.
    SE.unixDays = t / 86400;
    // calculate Julian Day Number
    SE.JDN = julianUnixEpoch + SE.unixDays;
    // Add the fractional day value to the Julian Day number. If the
    // input value was in the GMT time zone, we could proceed directly
    // with this value. 
    SE.JDN = SE.JDN + SE.timeFracDay;
    // Adjust JDN to GMT time zone
    SE.JDN = SE.JDN - ((double)SE.tzOffset / 24);
    // Calculate Julian Century Number
    SE.JCN = (SE.JDN - 2451545) / 36525;
    // Geometric Mean Longitude of Sun (degrees)
    SE.GMLS = (280.46646 + SE.JCN * (36000.76983 + SE.JCN * 0.0003032));
    // Finish GMLS calculation by calculating modolu(GMLS,360) as
    // it's done in R or Excel. C's fmod doesn't work in the same
    // way. The floor() function is from the math.h library.
    SE.GMLS = SE.GMLS - (360 * (floor(SE.GMLS/360)) );
    // Geometric Mean Anomaly of Sun (degrees)
    SE.GMAS = 357.52911 + (SE.JCN * (35999.05029 - 0.0001537 * SE.JCN));
    
    // Eccentricity of Earth Orbit
    SE.EEO = 0.016708634 - (SE.JCN * (0.000042037 + 0.0000001267 * SE.JCN));
    // Sun Equation of Center
    SE.SEC = sin(SE.GMAS * DEG_TO_RAD) * (1.914602 -
                                    (SE.JCN * (0.004817 + 0.000014 * SE.JCN))) +
    sin((2*SE.GMAS)* DEG_TO_RAD)*(0.019993-0.000101*SE.JCN) +
    sin((3*SE.GMAS)* DEG_TO_RAD) * 0.000289;
    // Sun True Longitude (degrees)
    SE.STL = SE.GMLS + SE.SEC;
    // Sun True Anomaly (degrees)
    SE.STA = SE.GMAS + SE.SEC;
    // Sun Radian Vector (Astronomical Units)
    SE.SRV = (1.000001018 * (1- SE.EEO * SE.EEO))/(1 + SE.EEO *
                                          cos(SE.STA * DEG_TO_RAD));
    // Sun Apparent Longitude (degrees)
    SE.SAL = SE.STL - 0.00569 - (0.00478 *
                           sin((125.04 - 1934.136 * SE.JCN) * DEG_TO_RAD));
    // Mean Oblique Ecliptic (degrees)
    SE.MOE = 23 + (26 + (21.448-SE.JCN * ( 46.815 + SE.JCN *
                                    (0.00059 - SE.JCN * 0.001813)))/60)/60;
    // Oblique correction (degrees)
    SE.OC = SE.MOE + 0.00256 * cos((125.04-1934.136*SE.JCN)*DEG_TO_RAD);
    // Sun Right Ascension (degrees)
    SE.SRA = (atan2(cos(SE.OC * DEG_TO_RAD) * sin(SE.SAL * DEG_TO_RAD),
                 cos(SE.SAL * DEG_TO_RAD))) * RAD_TO_DEG;
    // Sun Declination (degrees)
    SE.SDec = (asin(sin(SE.OC * DEG_TO_RAD) *
                 sin(SE.SAL * DEG_TO_RAD))) * RAD_TO_DEG;
    // var y
    SE.vy = tan((SE.OC/2) * DEG_TO_RAD) * tan((SE.OC/2) * DEG_TO_RAD);
    
    // Equation of Time (minutes)
    SE.EOT = 4 * ((SE.vy * sin(2 * (SE.GMLS * DEG_TO_RAD)) -
                2 * SE.EEO * sin(SE.GMAS * DEG_TO_RAD) +
                4 * SE.EEO * SE.vy * sin(SE.GMAS * DEG_TO_RAD) * 
                cos(2*(SE.GMLS*DEG_TO_RAD)) -
                0.5 * SE.vy * SE.vy * sin(4*(SE.GMLS * DEG_TO_RAD)) -
                1.25 * SE.EEO * SE.EEO * sin(2*(SE.GMAS* DEG_TO_RAD))) * 
                RAD_TO_DEG);
    // Hour Angle Sunrise (degrees)
    SE.HAS = acos((cos(90.833*DEG_TO_RAD)/
                (cos(SE.lat*DEG_TO_RAD) * cos(SE.SDec*DEG_TO_RAD))) -
               tan(SE.lat * DEG_TO_RAD) * tan(SE.SDec * DEG_TO_RAD)) * 
               RAD_TO_DEG ;
    // Solar Noon - result is given as fraction of a day
    // Time value is in GMT time zone
    SE.SolarNoonfrac = (720 - 4 * SE.lon - SE.EOT) / 1440 ;
    // SolarNoon is given as a fraction of a day. Add this
    // to the unixDays value, which currently holds the
    // whole days since 1970-1-1 00:00
    SE.SolarNoonDays = SE.unixDays + SE.SolarNoonfrac;
    // SolarNoonDays is in GMT time zone, correct it to
    // the input time zone
    SE.SolarNoonDays = SE.SolarNoonDays + ((double)SE.tzOffset / 24);
    // Then convert SolarNoonDays to seconds
    SE.SolarNoonTime = SE.SolarNoonDays * 86400;
    // Sunrise Time, given as fraction of a day
    SE.Sunrise = SE.SolarNoonfrac - SE.HAS * 4/1440;
    // Convert Sunrise to days since 1970-1-1
    SE.Sunrise = SE.unixDays + SE.Sunrise;
    // Correct Sunrise to local time zone from GMT
    SE.Sunrise = SE.Sunrise + ((double)SE.tzOffset / 24);
    // Convert Sunrise to seconds since 1970-1-1
    SE.Sunrise = SE.Sunrise * 86400;
    // Convert Sunrise to a time_t object (Time library)
    SE.SunriseTime = (time_t)SE.Sunrise;
    // Sunset Time
    SE.Sunset = SE.SolarNoonfrac + SE.HAS * 4/1440;
    // Convert Sunset to days since 1970-1-1
    SE.Sunset = SE.unixDays + SE.Sunset;
    // Correct Sunset to local time zone from GMT
    SE.Sunset = SE.Sunset + ((double)SE.tzOffset / 24);
    // Convert Sunset to seconds since 1970-1-1
    SE.Sunset = SE.Sunset * 86400;
    // Convert Sunset to a time_t object (Time library)
    SE.SunsetTime = (time_t)SE.Sunset;
    // Sunlight Duration (day length, minutes)
    SE.SunDuration = 8 * SE.HAS;
    // True Solar Time (minutes)
    SE.TST = (SE.timeFracDay * 1440 +
           SE.EOT + 4 * SE.lon - 60 * SE.tzOffset);
    // Finish TST calculation by calculating modolu(TST,360) as
    // it's done in R or Excel. C's fmod doesn't work in the same
    // way. The floor() function is from the math.h library.
    SE.TST = SE.TST - (1440 * (floor(SE.TST/1440)) );
    // Hour Angle (degrees)
    if (SE.TST/4 < 0) {
        SE.HA = SE.TST/4 + 180;
    } else if (SE.TST/4 >= 0) {
        SE.HA = SE.TST/4 - 180;
    }
    // Solar Zenith Angle (degrees)
    SE.SZA = (acos(sin(SE.lat * DEG_TO_RAD) *
                sin(SE.SDec* DEG_TO_RAD) +
                cos(SE.lat * DEG_TO_RAD) *
                cos(SE.SDec * DEG_TO_RAD) *
                cos(SE.HA * DEG_TO_RAD))) * RAD_TO_DEG;
    // Solar Elevation Angle (degrees above horizontal)
    SE.SEA = 90 - SE.SZA;
    // Approximate Atmospheric Refraction (degrees)
    if (SE.SEA > 85) {
        SE.AAR = 0;
    } else if (SE.SEA > 5) {
        SE.AAR = (58.1 / tan(SE.SEA * DEG_TO_RAD)) -
        0.07 / (pow(tan(SE.SEA * DEG_TO_RAD),3)) +
        0.000086 / (pow(tan(SE.SEA * DEG_TO_RAD),5));
    } else if (SE.SEA > -0.575) {
        SE.AAR = 1735 + SE.SEA * (-581.2 * SE.SEA *
                            (103.4 + SE.SEA * (-12.79 + SE.SEA * 0.711)));
    } else {
        SE.AAR = -20.772 / tan(SE.SEA * DEG_TO_RAD);
    }
    SE.AAR = SE.AAR / 3600.0;
    // Solar Elevation Corrected for Atmospheric
    // refraction (degrees)
    SE.SEC_Corr = SE.SEA + SE.AAR;
    // Solar Azimuth Angle (degrees clockwise from North)
    if (SE.HA > 0) {
        SE.SAA = (((acos((sin(SE.lat * DEG_TO_RAD) *
                       cos(SE.SZA * DEG_TO_RAD) -
                       sin(SE.SDec * DEG_TO_RAD)) /
                      (cos(SE.lat * DEG_TO_RAD) *
                       sin(SE.SZA * DEG_TO_RAD))) ) *
                RAD_TO_DEG) + 180);
        SE.SAA = SE.SAA - (360 * (floor(SE.SAA/360)));
    } else {
        SE.SAA = (540 - (acos((((sin(SE.lat * DEG_TO_RAD) *
                              cos(SE.SZA * DEG_TO_RAD))) -
                            sin(SE.SDec * DEG_TO_RAD)) /
                           (cos(SE.lat * DEG_TO_RAD) *
                            sin(SE.SZA * DEG_TO_RAD)))) *
               RAD_TO_DEG);
        SE.SAA = SE.SAA - (360 * (floor(SE.SAA/360)));
    }
}