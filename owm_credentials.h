// Change to your WiFi credentials
const char* ssid     = "your_WiFi_SSID";
const char* password = "your_WiFi_PASSWORD";

// Use your own API key by signing up for a free developer account at https://openweathermap.org/
String apikey       = "your_OWM_API_Key";                     // See: https://openweathermap.org/
const char server[] = "api.openweathermap.org";

// EXAMPLE API CALLS:
//http://api.openweathermap.org/data/2.5/forecast?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=24
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=1
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric&cnt=1&lang=en
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric
//http://api.openweathermap.org/data/2.5/weather?q=Melksham,UK&APPID=your_OWM_API_key&mode=json&units=metric
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
                                                          // See: https://www.ntppool.org/en/    

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
