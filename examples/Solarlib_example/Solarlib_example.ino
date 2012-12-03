/* Solarlib_example
  **WARNING**
  This will likely only work on 32-bit ARM based Arduino variants,
  such as Teensy 3.0 and the Due, not on older 8-bit ATMEGA-based 
  boards like the UNO or MEGA series.
  **********
  This sketch illustrates the basic use of the Solarlib library.
  You must also have the Time library installed. 
  This sketch will output a series of values and times to the
  Serial monitor to demonstrate the usage of the solar 
  calculator. A full list of extractor functions can be found
  in the Solarlib.h and Solarlib.cpp library files. 
  
  The solar calculator needs to be initialized using the 
  initSolarCalc() function found in the setup loop.
  
  The output can be checked against the NOAA calculator here:
  http://www.esrl.noaa.gov/gmd/grad/solcalc/
*/

#include <math.h>
#include <Time.h>
#include <Solarlib.h>

// Time zone offset (hr), zones west of GMT are negative
#define timeZoneOffset -8 
// A fixed time used for testing: 2012-11-04 12:00
#define myUnixDate 1352030400 
// Latitude, values north of equator are positive
#define mylat 36.62  
// Longitude, values west of GMT are negative
#define mylon -121.9 
// Create time variable
atime_t t; 

void setup () {
    Serial.begin(115200); // Set serial port speed
    // While the serial stream is not open, do nothing:
    while (!Serial) ;
    // Initialize the Solar Calculator with the time zone
    // offset, latitude, and longitude for the site.
    initSolarCalc(timeZoneOffset, mylat, mylon);
    // Initialize the Time library
    setTime(myUnixDate); 
    // Initialize the time variable t
    t = now(); 
}

void loop() {
  Serial.print("Current time: ");
  printDateTime(t);
  Serial.print("Solar Elevation, degrees: ");
  Serial.println(getSEC_Corr(t),4);
  Serial.print("Solar Azimuth, degrees: ");
  Serial.println(getSAA(t),4);
  Serial.print("Sunrise: ");
  printDateTime(getSunriseTime(t));
  Serial.print("Sunset: ");
  printDateTime(getSunsetTime(t));
  Serial.print("Day length, minutes: ");
  Serial.println(getSunDuration(t));
  Serial.println();
  t = t + 3600; // Increment time by 1 hour
  delay(1000);
}


// Utility function to print time and date nicely
void printDateTime(atime_t t){
  Serial.print(year(t));
  printDateDigits(month(t));
  printDateDigits(day(t));
  Serial.print(" ");
  Serial.print(hour(t));
  printDigits(minute(t));
  printDigits(second(t));
  Serial.println();
}
// Utility function to print month/day digits nicely
void printDateDigits(int digits){
  Serial.print("-");
  if(digits < 10) Serial.print("0");
  Serial.print(digits); 
}
// Utility function for time value printing
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

