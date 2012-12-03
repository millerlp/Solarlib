Currently this library only compiles under the Teeny 3.0 modified version
of Arduino 1.0.2 provided by PJRC.com. See the PJRC forum for downloads
http://forum.pjrc.com/forums/6-Announcements

To install this library, place the folder containing all of the Solarlib
files into the libraries directory in your Arduino folder (Mac) or 
under the libraries folder inside arduino-1.0.2 (Windows).

/* Solarlib		Luke Miller December 2012
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