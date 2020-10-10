const char* ssid     = "your_WiFi_SSID";
const char* password = "your_WiFi_PASSWORD";

// // Use your own API key by signing up for a free developer account at https://openweathermap.org/
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

String City             = "Algorta";
String Latitude         = "43.3";                      // Required location Latitude
String Longitude        = "-3.02";                      // Required location Longitude
String Language         = "es";                           // NOTE: Only the weather description is translated by OWM
// Languages supported: ar, bg, ca, de, el, en, es, fa, fi, fr, gl, hr, hu, it, ja, kr, la, lt, mk, nl, pl, pt, ro, ru,
//                      se, sk, sl, tr, ua, vi, zh_cn, zh_tw
String Hemisphere       = "north";                        // or "south"
String Units            = "M";                            // Use 'M' for Metric or I for Imperial
//const char* Timezone    = "GMT0BST,M3.5.0/01,M10.5.0/02"; // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* Timezone    = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // e.g. Germany
// See below for examples
const char* ntpServer   = "pool.ntp.org";                 // Or, choose a time server close to you, but in most cases it's best to use pool.ntp.org to find an NTP server
// then the NTP system decides e.g. 0.country-code.pool.ntp.org, 1.country-code.pool.ntp.org etc...
// UK "0.uk.pool.ntp.org"
// EU "0.europe.pool.ntp.org"
// DE "0.de.pool.ntp.org"
// US "0.north-america.pool.ntp.org"
// See: https://www.ntppool.org/en/    AND see below for examples

const String DEF_TXT_LANGUAGE_TABLE  = "Tabla de traducción de idioma";
const String DEF_TXT_FORECAST_GRAPHS = "Pronóstico a 5 días / 3 horas";
const String DEF_TXT_TEMPDEWPOINT    = "Temperatura / Punto de rocío";
const String DEF_TXT_TEMPERATURE     = "Temperatura";
const String DEF_TXT_FEELSLIKE       = "Sensación";
const String DEF_TXT_HUMIDITY        = "Humedad";
const String DEF_TXT_DEWPOINT        = "Punto de rocío";
const String DEF_TXT_WINDCHILL       = "Sensación térmica";
const String DEF_TXT_WCHILL_HINDEX   = "Sensación térmica / Índice de calor";
const String DEF_TXT_HEATINDEX       = "Indice de calor";
const String DEF_TXT_WINDSPEED       = "Velocidad del viento";
const String DEF_TXT_PRESSURE        = "Presión";
const String DEF_TXT_CLOUDCOVER      = "Nubosidad";
const String DEF_TXT_VISIBILITY      = "Visibilidad";
const String DEF_TXT_PRECIPITATION   = "Precipitación";
const String DEF_TXT_RAINFALL        = "Lluvia";
const String DEF_TXT_RAINRATE        = "Promedio de lluvia";
const String DEF_TXT_SNOWFALL        = "Nieve";
const String DEF_TXT_SNOWRATE        = "Promedio de nieve";
const String DEF_TXT_UVINDEX         = "Índice UV";
  //Moon phases
const String DEF_TXT_MOON_PHASE1     = "Nueva";
const String DEF_TXT_MOON_PHASE2     = "Creciente";
const String DEF_TXT_MOON_PHASE3     = "Primer cuarto";
const String DEF_TXT_MOON_PHASE4     = "Menguante";
const String DEF_TXT_MOON_PHASE5     = "Llena";
const String DEF_TXT_MOON_PHASE6     = "Menguante";
const String DEF_TXT_MOON_PHASE7     = "Tercer cuarto";
const String DEF_TXT_MOON_PHASE8     = "Creciente menguante";
  //Wind
const String DEF_TXT_N               = "N";
const String DEF_TXT_NE              = "NE";
const String DEF_TXT_E               = "E";
const String DEF_TXT_SE              = "SE";
const String DEF_TXT_S               = "S";
const String DEF_TXT_SW              = "SO";
const String DEF_TXT_W               = "O";
const String DEF_TXT_NW              = "NO";
const String DEF_TXT_WINDUNITS       = "mph"; // Ensure text here matahces units chosen above! Use a leading space
//Windspeed warning colours based on Beaufort scale in kph *** NOTE these are HTML colours, don't translate!
const String DEF_TXT_BFS0            = "Calma";
const String DEF_TXT_BFS1            = "Aire ligero";
const String DEF_TXT_BFS2            = "Brisa ligera";
const String DEF_TXT_BFS3            = "Suave brisa";
const String DEF_TXT_BFS4            = "Brisa moderada";
const String DEF_TXT_BFS5            = "Brisa fresca";
const String DEF_TXT_BFS6            = "Brisa fuerte";
const String DEF_TXT_BFS7            = "Fuerte viento";
const String DEF_TXT_BFS8            = "Vendaval";
const String DEF_TXT_BFS9            = "Vendaval severo";
const String DEF_TXT_BFS10           = "Tormenta";
const String DEF_TXT_BFS11           = "Tormenta violenta";
const String DEF_TXT_BFS12           = "Huracán";
//Wind description
const String DEF_TXT_MBFS0           = "Fuerza-0";
const String DEF_TXT_MBFS1           = "Fuerza-1";
const String DEF_TXT_MBFS2           = "Fuerza-2";
const String DEF_TXT_MBFS3           = "Fuerza-3";
const String DEF_TXT_MBFS4           = "Fuerza-4";
const String DEF_TXT_MBFS5           = "Fuerza-5";
const String DEF_TXT_MBFS6           = "Fuerza-6";
const String DEF_TXT_MBFS7           = "Fuerza-7";
const String DEF_TXT_MBFS8           = "Fuerza-8";
const String DEF_TXT_MBFS9           = "Fuerza-9";
const String DEF_TXT_MBFS10          = "Fuerza-10";
const String DEF_TXT_MBFS11          = "Fuerza-11";
const String DEF_TXT_MBFS12          = "Fuerza-12";
  //Days of the week
const String DEF_TXT_SUN             = "Dom";
const String DEF_TXT_MON             = "Lun";
const String DEF_TXT_TUE             = "Mar";
const String DEF_TXT_WED             = "Mié";
const String DEF_TXT_THU             = "Jue";
const String DEF_TXT_FRI             = "Vie";
const String DEF_TXT_SAT             = "Sáb";
  //Months
const String DEF_TXT_JAN             = "Ene";
const String DEF_TXT_FEB             = "Feb";
const String DEF_TXT_MAR             = "Mar";
const String DEF_TXT_APR             = "Abr";
const String DEF_TXT_MAY             = "May";
const String DEF_TXT_JUN             = "Jun";
const String DEF_TXT_JUL             = "Jul";
const String DEF_TXT_AUG             = "Ago";
const String DEF_TXT_SEP             = "Sep";
const String DEF_TXT_OCT             = "Oct";
const String DEF_TXT_NOV             = "Nov";
const String DEF_TXT_DEC             = "Dic";
  // High Low and Relative Humidity
const String DEF_TXT_HI               = "Máx.";
const String DEF_TXT_LO               = "Mín.";
const String DEF_TXT_RH               = "Humedad";
const String DEF_TXT_HOME             = "Principal";
const String DEF_TXT_WEATHERMAP       = "Mapa Meteorológico";
const String DEF_TXT_SYSTEMSETUP      = "Ajustes";
const String DEF_TXT_PREFILLSETTINGS  = "Pre-Carga Ajustes";
const String DEF_TXT_HELP             = "Ayuda";

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
