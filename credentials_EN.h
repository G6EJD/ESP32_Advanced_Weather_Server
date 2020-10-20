// Change to your WiFi credentials
const char* ssid     = "your_WiFi_SSID";
const char* password = "your_WiFi_PASSWORD";

// Use your own API key by signing up for a free developer account at https://openweathermap.org/
String apikey       = "your_OWM_API_key";                   // See: https://openweathermap.org/
const char server[] = "api.openweathermap.org";

// EXAMPLE API CALLS:
//http://api.openweathermap.org/data/2.5/forecast?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=24
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=1
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=1&lang=en
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OQM_API_key&mode=json&units=metric
//http://api.openweathermap.org/data/2.5/forecast?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=2
//https://openweathermap.org/weathermap?basemap=map&cities=false&layer=radar&lat=51&lon=-2&zoom=10

String City             = "Bath";
String Latitude         = "51.3704";                      // Required location Latitude
String Longitude        = "-2.1376";                      // Required location Longitude
String Language         = "en";                           // NOTE: Only the weather description is translated by OWM
// Languages supported: ar, bg, ca, de, el, en, es, fa, fi, fr, gl, hr, hu, it, ja, kr, la, lt, mk, nl, pl, pt, ro, ru,
//                      se, sk, sl, tr, ua, vi, zh_cn, zh_tw
String Hemisphere       = "north";                        // or "south"
String Units            = "M";                            // Use 'M' for Metric or I for Imperial
const char* Timezone    = "GMT0BST,M3.5.0/01,M10.5.0/02"; // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
//const char* Timezone    = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // e.g. Germany
// See below for examples
const char* ntpServer   = "pool.ntp.org";                 // Or, choose a time server close to you, but in most cases it's best to use pool.ntp.org to find an NTP server
// then the NTP system decides e.g. 0.country-code.pool.ntp.org, 1.country-code.pool.ntp.org etc...
// UK "0.uk.pool.ntp.org"
// EU "0.europe.pool.ntp.org"
// DE "0.de.pool.ntp.org"
// US "0.north-america.pool.ntp.org"
// See: https://www.ntppool.org/en/    AND see below for examples

const String DEF_TXT_LANGUAGE_TABLE  = "Language Translation Table";
const String DEF_TXT_FORECAST_GRAPHS = "5 day / 3 hour forecast";
const String DEF_TXT_TEMPDEWPOINT    = "Temperature / Dewpoint";
const String DEF_TXT_TEMPERATURE     = "Temperature";
const String DEF_TXT_FEELSLIKE       = "Feelslike";
const String DEF_TXT_HUMIDITY        = "Humidity";
const String DEF_TXT_DEWPOINT        = "Dewpoint";
const String DEF_TXT_WINDCHILL       = "Windchill";
const String DEF_TXT_WCHILL_HINDEX   = "Windchill / Heat Index";
const String DEF_TXT_HEATINDEX       = "Heat Index";
const String DEF_TXT_WINDSPEED       = "Windspeed";
const String DEF_TXT_PRESSURE        = "Pressure";
const String DEF_TXT_CLOUDCOVER      = "Cloud Cover";
const String DEF_TXT_VISIBILITY      = "Visibility";
const String DEF_TXT_PRECIPITATION   = "Precipitation";
const String DEF_TXT_RAINFALL        = "Rainfall";
const String DEF_TXT_RAINRATE        = "Rainrate";
const String DEF_TXT_SNOWFALL        = "Snowfall";
const String DEF_TXT_SNOWRATE        = "Snowrate";
const String DEF_TXT_UVINDEX         = "UV Index";
//Moon phases
const String DEF_TXT_MOON_PHASE1     = "New";
const String DEF_TXT_MOON_PHASE2     = "Waxing Crescent";
const String DEF_TXT_MOON_PHASE3     = "First Quarter";
const String DEF_TXT_MOON_PHASE4     = "Waxing Gibbous";
const String DEF_TXT_MOON_PHASE5     = "Full";
const String DEF_TXT_MOON_PHASE6     = "Waning Gibbous";
const String DEF_TXT_MOON_PHASE7     = "Third Quarter";
const String DEF_TXT_MOON_PHASE8     = "Waning Crescent";
//Wind
const String DEF_TXT_N               = "N";
const String DEF_TXT_NE              = "NE";
const String DEF_TXT_E               = "E";
const String DEF_TXT_SE              = "SE";
const String DEF_TXT_S               = "S";
const String DEF_TXT_SW              = "SW";
const String DEF_TXT_W               = "W";
const String DEF_TXT_NW              = "NW";
const String DEF_TXT_WINDUNITS       = "mph"; // Ensure text here matahces units chosen above! Use a leading space
//Windspeed warning colours based on Beaufort scale in kph *** NOTE these are HTML colours, don't translate!
const String DEF_TXT_BFS0            = "Calm";
const String DEF_TXT_BFS1            = "Light Air";
const String DEF_TXT_BFS2            = "Light Breeze";
const String DEF_TXT_BFS3            = "Gentle Breeze";
const String DEF_TXT_BFS4            = "Moderate Breeze";
const String DEF_TXT_BFS5            = "Fresh Breeze";
const String DEF_TXT_BFS6            = "Strong Breeze";
const String DEF_TXT_BFS7            = "High Wind";
const String DEF_TXT_BFS8            = "Gale";
const String DEF_TXT_BFS9            = "Severe Gale";
const String DEF_TXT_BFS10           = "Storm";
const String DEF_TXT_BFS11           = "Violent Storm";
const String DEF_TXT_BFS12           = "Hurricane";
//Wind description
const String DEF_TXT_MBFS0           = "Force-0";
const String DEF_TXT_MBFS1           = "Force-1";
const String DEF_TXT_MBFS2           = "Force-2";
const String DEF_TXT_MBFS3           = "Force-3";
const String DEF_TXT_MBFS4           = "Force-4";
const String DEF_TXT_MBFS5           = "Force-5";
const String DEF_TXT_MBFS6           = "Force-6";
const String DEF_TXT_MBFS7           = "Force-7";
const String DEF_TXT_MBFS8           = "Force-8";
const String DEF_TXT_MBFS9           = "Force-9";
const String DEF_TXT_MBFS10          = "Force-10";
const String DEF_TXT_MBFS11          = "Force-11";
const String DEF_TXT_MBFS12          = "Force-12";
//Days of the week
const String DEF_TXT_SUN             = "Sun";
const String DEF_TXT_MON             = "Mon";
const String DEF_TXT_TUE             = "Tue";
const String DEF_TXT_WED             = "Wed";
const String DEF_TXT_THU             = "Thu";
const String DEF_TXT_FRI             = "Fri";
const String DEF_TXT_SAT             = "Sat";
//Months
const String DEF_TXT_JAN             = "Jan";
const String DEF_TXT_FEB             = "Feb";
const String DEF_TXT_MAR             = "Mar";
const String DEF_TXT_APR             = "Apr";
const String DEF_TXT_MAY             = "May";
const String DEF_TXT_JUN             = "Jun";
const String DEF_TXT_JUL             = "Jul";
const String DEF_TXT_AUG             = "Aug";
const String DEF_TXT_SEP             = "Sep";
const String DEF_TXT_OCT             = "Oct";
const String DEF_TXT_NOV             = "Nov";
const String DEF_TXT_DEC             = "Dec";
const String DEF_TXT_HI              = "Hi";
const String DEF_TXT_LO              = "Lo";
const String DEF_TXT_RH              = "RH";
const String DEF_TXT_HOME            = "Home";
const String DEF_TXT_WEATHERMAP      = "Weather Map";
const String DEF_TXT_SYSTEMSETUP     = "System Setup";
const String DEF_TXT_PREFILLSETTINGS = "Pre-Fill Settings";
const String DEF_TXT_HELP            = "Help";

// TIMEZOEN Definition examples
// NOTE: Because this code uses the SETENV variable the following offset values are ignored!
int  gmtOffset_sec      = 0;    // UK normal time is GMT, so GMT Offset is 0, for US (-5Hrs) is typically -18000, AU is typically (+8hrs) 28800
int  daylightOffset_sec = 3600; // In the UK DST is +1hr or 3600-secs, other countries may use 2hrs 7200 or 30-mins 1800 or 5.5hrs 19800 Ahead of GMT use + offset behind - offset

// Example time zones
//const char* Timezone = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe e.g. Austria, Belgium, France, Germany, Holland, Norway, Poland, Spain, Sweden
//const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3";       // Central Europe
//const char* Timezone = "EST-2METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe e.g. Belarus, Finland, Romania
//const char* Timezone = "EST5EDT,M3.2.0,M11.1.0";           // EST USA
//const char* Timezone = "CST6CDT,M3.2.0,M11.1.0";           // CST USA
//const char* Timezone = "MST7MDT,M4.1.0,M10.5.0";           // MST USA
//const char* Timezone = "NZST-12NZDT,M9.5.0,M4.1.0/3";      // Auckland
//const char* Timezone = "EET-2EEST,M3.5.5/0,M10.5.5/0";     // Asia
//const char* Timezone = "ACST-9:30ACDT,M10.1.0,M4.1.0/3":   // Australia
