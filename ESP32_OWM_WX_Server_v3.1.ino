/* ESP32 Weather Server, gets data from Open Weather Map, decodes and then displays it on a webpage.
  Uses mDNS service to allow a logical name to be used e.g. http://Wxserver.local instead of http://192.168.0.16/
  ####################################################################################################################################
  This software, the ideas and concepts is Copyright (c) David Bird 2021. All rights to this software are reserved.

  Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
  1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
  2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
  3. You may not, except with my express written permission, distribute or commercially exploit the content.
  4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

  The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
  software use is visible to an end-user.

  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY
  OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  See more at http://www.dsbird.org.uk
*/


//LANGUAGES
#include "credentials_EN.h" // See "credentials_EN.h" tab and enter your OWM API key and set the Wifi SSID and PASSWORD
//#include "credentials_ES.h"   // o la pestaña "credentials_ES.h" e introduce tu propia clave de OWM API y ajusta Wifi SSID y PASSWORD#include "credentials_ES_PRIVADA.h"


#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson Install by: Sketch/ Include Library / Manage Libaries / enter 'JSON', find, select then install
#include <WiFi.h>           // Built-in
#include <WebServer.h>      // Built-in
#include "time.h"           // Built-in
#include "ESPmDNS.h"        // Built-in
#include <HTTPClient.h>     // Built-in
#include <SPIFFS.h>         // Built-in
//########################################################################################
//Languagetranslation values, adjust "text" values for your language

//Temperature, Dewpoint, Humidity, Windchill, HeatIndex, Windspeed, Pressure, Rainfall, Snowfall
String TXT_FORECAST_GRAPHS = "5 day / 3 hour forecast";
String TXT_TEMPDEWPOINT    = "Temperature / Dewpoint";
String TXT_TEMPERATURE     = "Temperature";
String TXT_FEELSLIKE       = "Feelslike";
String TXT_HUMIDITY        = "Humidity";
String TXT_DEWPOINT        = "Dewpoint";
String TXT_WINDCHILL       = "Windchill";
String TXT_WCHILL_HINDEX   = "Windchill / Heat Index";
String TXT_HEATINDEX       = "Heat Index";
String TXT_WINDSPEED       = "Windspeed";
String TXT_PRESSURE        = "Pressure";
String TXT_CLOUDCOVER      = "Cloud Cover";
String TXT_VISIBILITY      = "Visibility";
String TXT_PRECIPITATION   = "Precipitation";
String TXT_RAINFALL        = "Rainfall";
String TXT_RAINRATE        = "Rainrate";
String TXT_SNOWFALL        = "Snowfall";
String TXT_SNOWRATE        = "Snowrate";
String TXT_UVINDEX         = "UV Index";
//Moon phases
String TXT_MOON_PHASE1     = "New";
String TXT_MOON_PHASE2     = "Waxing Crescent";
String TXT_MOON_PHASE3     = "First Quarter";
String TXT_MOON_PHASE4     = "Waxing Gibbous";
String TXT_MOON_PHASE5     = "Full";
String TXT_MOON_PHASE6     = "Waning Gibbous";
String TXT_MOON_PHASE7     = "Third Quarter";
String TXT_MOON_PHASE8     = "Waning Crescent";
//General
const char* TXT_UPDATED    = ""; // or "Updated:"
//Wind
String TXT_N               = "N";
String TXT_NE              = "NE";
String TXT_E               = "E";
String TXT_SE              = "SE";
String TXT_S               = "S";
String TXT_SW              = "SW";
String TXT_W               = "W";
String TXT_NW              = "NW";
enum   WindspdType {MPS, KPH, MPH, KTS}; // metres-per-second, kilometres-per-hour, miles-per-hour, knots
WindspdType WindspeedUnits = MPH;      // Change to your desired display format e.g. for KPH 'MetricWindspeed = KPH';
String TXT_WINDUNITS       = "mph";    // Ensure text here matahces units chosen above! Use a leading space
//Windspeed warning colours based on Beaufort scale in kph *** NOTE these are HTML colours, don't translate!
String Calm                = "white";           // <1      km/hr Force 0
String LightAir            = "lightcyan";       // 1-5     km/hr Force 1
String LightBreeze         = "paleturquoise";   // 6-11    km/hr Force 2
String GentleBreeze        = "aquamarine";      // 12-19   km/hr Force 3
String ModerateBreeze      = "turquoise";       // 20-28   km/hr Force 4
String FreshBreeze         = "mediumturquoise"; // 29-38   km/hr Force 5
String StrongBreeze        = "darkturquoise";   // 39-49   km/hr Force 6
String HighWind            = "lightseagreen";   // 50-61   km/hr Force 7
String Gale                = "yellow";          // 62-74   km/hr Force 8
String SevereGale          = "gold";            // 75-88   km/hr Force 9
String Storm               = "orange";          // 89-102  km/hr Force 10
String ViolentStorm        = "red";             // 103-117 km/hr Force 11
String Hurricane           = "crimson";         // >118    km/hr Force 12
String TXT_BFS0            = "Calm";
String TXT_BFS1            = "Light Air";
String TXT_BFS2            = "Light Breeze";
String TXT_BFS3            = "Gentle Breeze";
String TXT_BFS4            = "Moderate Breeze";
String TXT_BFS5            = "Fresh Breeze";
String TXT_BFS6            = "Strong Breeze";
String TXT_BFS7            = "High Wind";
String TXT_BFS8            = "Gale";
String TXT_BFS9            = "Severe Gale";
String TXT_BFS10           = "Storm";
String TXT_BFS11           = "Violent Storm";
String TXT_BFS12           = "Hurricane";
//Days of the week
String TXT_SUN             = "Sun";
String TXT_MON             = "Mon";
String TXT_TUE             = "Tue";
String TXT_WED             = "Wed";
String TXT_THU             = "Thu";
String TXT_FRI             = "Fri";
String TXT_SAT             = "Sat";
const char* weekday_D[]    = { TXT_SUN.c_str(), TXT_MON.c_str(), TXT_TUE.c_str(), TXT_WED.c_str(), TXT_THU.c_str(), TXT_FRI.c_str(), TXT_SAT.c_str() };
//Months
String TXT_JAN             = "Jan";
String TXT_FEB             = "Feb";
String TXT_MAR             = "Mar";
String TXT_APR             = "Apr";
String TXT_MAY             = "May";
String TXT_JUN             = "Jun";
String TXT_JUL             = "Jul";
String TXT_AUG             = "Aug";
String TXT_SEP             = "Sep";
String TXT_OCT             = "Oct";
String TXT_NOV             = "Nov";
String TXT_DEC             = "Dec";
const char* month_M[]      = { TXT_JAN.c_str(), TXT_FEB.c_str(), TXT_MAR.c_str(), TXT_APR.c_str(), TXT_MAY.c_str(), TXT_JUN.c_str(), TXT_JUL.c_str(), TXT_AUG.c_str(), TXT_SEP.c_str(), TXT_OCT.c_str(), TXT_NOV.c_str(), TXT_DEC.c_str() };
//Graph Colours
const String legendColour   = "black"; // Only use HTML colour names
const String titleColour    = "purple";
const String backgrndColour = "gainsboro";
const String data1Colour    = "blue";
const String data2Colour    = "red";
// High Low and Relative Humidity
String TXT_HI               = "Hi";
String TXT_LO               = "Lo";
String TXT_RH               = "RH";
// Menu items
String TXT_HOME             = "Home";
String TXT_WEATHERMAP       = "Weather Map";
String TXT_SYSTEMSETUP      = "System Setup";
String TXT_PREFILLSETTINGS  = "Pre-Fill Settings";
String TXT_HELP             = "Help";
String TXT_LANGUAGE_TABLE   = "Language Translation Table";

int PageWidth               = 1024; // Adjust for desired webpage width
int ForecastPeriods         = 9;    // Adjust number of forecast periods to display, maximum 96hrs (96/3 / 8 = 4-days
//################  VERSION  ###########################################
String version = "3.0";      // Programme version, see change log at end

//################ PROGRAM VARIABLES and OBJECTS ################
String Time_str, Date_str, webpage, Wx_Description, WindUnits = " mps", DataFile = "params.txt"; // strings for Time, Date and webpage, etc

#define max_readings 40 // OWM provides 5-days of every 3-hours data
#define max_periods  9  // maximum 3-hour display items

typedef struct { // For current Day and then Day 1, 2, 3, etc
  int    Dt;
  String Icon;
  String Description;
  String Description2;
  String Time;
  String Trend;
  float  uvIndex;
  float  Temperature;
  float  Feelslike;
  float  Humidity;
  float  High;
  float  Low;
  float  Winddir;
  float  Windspeed;
  float  Gust;
  float  Rainfall;
  float  Rainrate;
  float  Snowfall;
  float  Snowrate;
  float  Pressure;
  float  Pop;
  int    Cloudcover;
  int    Visibility;
  int    Sunrise;
  int    Sunset;
} Forecast_record_type;

Forecast_record_type WxConditions[1];
Forecast_record_type WxForecast[max_readings];

float pressure_readings[max_readings]    = {0};
float temperature_readings[max_readings] = {0};
float humidity_readings[max_readings]    = {0};
float dewpoint_readings[max_readings]    = {0};
float windspeed_readings[max_readings]   = {0};
float windgust_readings[max_readings]    = {0};
float windchill_readings[max_readings]   = {0};
float heatindex_readings[max_readings]   = {0};
float rain_readings[max_readings]        = {0};
float snow_readings[max_readings]        = {0};
int   wait                               = 0;
int   update_time                        = 300000; // Every 5-mins in milli-seconds
bool  Refresh_WXData                     = true;

WebServer webserver(80);  // Set the port you wish to use, a browser default is 80, but any port can be used, if you set it to 5555 then connect with http://nn.nn.nn.nn:5555/
WiFiClient client;        // wifi client object

const char* ServerName = "wxserver"; // Connect to the server with http://wxserver.local/ e.g. name = "myserver" then use address http://myserver.local/

//########################################################################################
void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(__FILE__);
  Serial.println("Starting...");
  StartSPIFFS();
  RecoverSettings(); // NOTE: After programming SPIFFS is erased
  //SPIFFS.remove("/"+DataFile);             // In-case SPIFFS file gets corrupted, it can happen!
  if (StartWiFi() == WL_CONNECTED) {         // Start the WiFi service
    Serial.println("WiFi Started...");
  }
  SetupDeviceName(ServerName);               // Set up the logical name
  if (SetupTime() == true) {
    Serial.println("Time started...");       // Start the time services
  }
  webserver.on("/", Homepage);               // Process a homepage request
  webserver.on("/wxmap", Weathermap);        // Process a weather map request
  webserver.on("/setup", SystemSetup);       // Process a setup request
  webserver.on("/prefillEN", PreFill);       // Process a prefill data request
  webserver.on("/handleinput", HandleInput); // Process inputs from the client
  webserver.on("/help", Help);               // Process a help request
  webserver.onNotFound(handleNotFound);      // Handle page not found errors
  webserver.begin();                         // Start the webserver
  Refresh_WXData = true;                     // Get the current weather data
}
//#########################################################################################
void loop() {
  webserver.handleClient();   // Wait for a client to connect and when they do process their requests
  if (millis() > update_time + wait || Refresh_WXData == true) { // Wait for time-out or request for data
    UpdateData();
  }
}
//#########################################################################################
void UpdateData() {
  Refresh_WXData  = false;
  byte Attempts   = 1;
  bool RxWeather  = false,
       RxForecast = false,
       RxUVindex  = false;
  WiFiClient client;       // WiFi client object
  while ((RxWeather == false || RxForecast == false || RxUVindex == false) && Attempts <= 2) { // Try up-to 2 times for Weather and Forecast data
    if (RxWeather   == false) RxWeather  = obtain_wx_data(client, "weather");
    if (RxForecast  == false) RxForecast = obtain_wx_data(client, "forecast");
    if (RxUVindex   == false) RxUVindex  = obtain_wx_data(client, "onecall");
    Attempts++;
  }
  int r = 0;
  do {
    if (Units == "I") pressure_readings[r] = WxForecast[r].Pressure * 0.02953;   else pressure_readings[r] = WxForecast[r].Pressure;
    temperature_readings[r] = WxForecast[r].Temperature;
    humidity_readings[r]    = WxForecast[r].Humidity;
    dewpoint_readings[r]    = DewPoint(temperature_readings[r], humidity_readings[r]);
    windspeed_readings[r]   = Convert_to_WS_Units(WxForecast[r].Windspeed);
    windgust_readings[r]    = Convert_to_WS_Units(WxForecast[r].Gust);
    windchill_readings[r]   = calc_windchill(temperature_readings[r], windspeed_readings[r]);
    heatindex_readings[r]   = calc_heatindex(temperature_readings[r], humidity_readings[r]);
    if (Units == "I") rain_readings[r] = WxForecast[r].Rainfall * 0.0393701; else rain_readings[r] = WxForecast[r].Rainfall;
    if (Units == "I") snow_readings[r] = WxForecast[r].Snowfall * 0.0393701; else snow_readings[r] = WxForecast[r].Snowfall;
    r++;
  } while (r < max_readings);
  WxConditions[0].Windspeed = Convert_to_WS_Units(WxConditions[0].Windspeed);
  WxConditions[0].Gust      = Convert_to_WS_Units(WxConditions[0].Gust);
  int HeatIndexAverage      = 0;
  r = 0; do {
    HeatIndexAverage += heatindex_readings[r];
    r++;
  } while (r < max_readings);
  HeatIndexAverage /= max_readings;
  if ((Units == "M" && HeatIndexAverage <= 28) || (Units == "I" && HeatIndexAverage <= 82)) { // Heatindex only applies if Temperature >= 28°C or >=82°F
    r = 0; do {
      heatindex_readings[r] = 0; // Don't display Heat Index when average temperature is out of bounds
      r++;
    } while (r < max_readings);
  }
  UpdateLocalTime();
  wait = millis();
}
//#########################################################################################
void Homepage() {
  String WDirArrow = "";
  Wx_Description = TitleCase(WxConditions[0].Description);
  if (WxConditions[0].Description2 != "") Wx_Description += ", " + TitleCase(WxConditions[0].Description2);
  append_page_header(true);
  webpage += "<div class='style6'><h3>" + Wx_Description + "</h3></div>";
  webpage += "<div class='row'>";
  webpage += " <div class='headcolumn'>";
  webpage += "  <img src='http://openweathermap.org/img/w/" + WxConditions[0].Icon + ".png' alt='wx' style='width:90%'>";
  webpage += " </div>";
  webpage += " <div class='headcolumn'>";
  webpage += "  <table class='style5'>"; // Opened table-1
  webpage += "   <tr>";
  webpage += "    <th colspan='3' style='font-size:4.5em' class='" + TempToClass(WxConditions[0].Temperature) + "'>" + String(WxConditions[0].Temperature, 0) + "&deg;</th>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td>" + TXT_HI + "</td><td>" + TXT_LO + "</td><td>" + TXT_RH + "</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td class='" + TempToClass(WxConditions[0].High) + "'>"     + String(WxConditions[0].High, 0)     + "&deg;</td>";
  webpage += "    <td class='" + TempToClass(WxConditions[0].Low) + "'>"      + String(WxConditions[0].Low, 0)      + "&deg;</td>";
  webpage += "    <td class='" + HumiToClass(WxConditions[0].Humidity) + "'>" + String(WxConditions[0].Humidity, 0) + "%</td>";
  webpage += "   </tr>";
  webpage += "  </table>"; // Closed table-1
  webpage += " </div>";
  webpage += " <div class='headcolumn'>";
  webpage += "  <table class='style5'>";// Opened table-2
  webpage += "   <tr>";
  webpage += "    <td style='text-align:left'>" + TXT_PRESSURE + "</td>";
  webpage += "    <td style='text-align:left'>" + String(WxConditions[0].Pressure, (Units == "M" ? 0 : 1)) + WxConditions[0].Trend + "</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td style='text-align:left'>" + TXT_FEELSLIKE + "</td>";
  webpage += "    <td style='text-align:left'>" + String(WxConditions[0].Feelslike, 0) + "&deg;</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td style='text-align:left'>" + TXT_DEWPOINT + "</td>";
  webpage += "    <td style='text-align:left'>" + String(DewPoint(WxConditions[0].Temperature, WxConditions[0].Humidity), 0) + "&deg;</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td style='text-align:left'>" + TXT_CLOUDCOVER + "</td>";
  webpage += "    <td style='text-align:left'>" + String(WxConditions[0].Cloudcover) + "%</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td style='text-align:left'>" + TXT_VISIBILITY + "</td>";
  webpage += "    <td style='text-align:left'>" + String(WxConditions[0].Visibility * (Units == "M" ? 1 : 1.0936133), 0) + String(Units == "M" ? " M" : " Yds") + "</td>";
  webpage += "   </tr>";
  if (WxConditions[0].Rainrate > 0) {
    webpage += "   <tr>";
    webpage += "    <td style='text-align:left'>" + TXT_RAINRATE + "</td>";
    webpage += "    <td style='text-align:left'>" + String(WxConditions[0].Rainrate * (Units == "M" ? 1 : 1.0936133), 1) + String(Units == "M" ? " mm/Hr" : " in/Hr") + "</td>";
    webpage += "   </tr>";
  }
  if (WxConditions[0].Snowrate > 0) {
    webpage += "   <tr>";
    webpage += "    <td style='text-align:left'>" + TXT_SNOWRATE + "</td>";
    webpage += "    <td style='text-align:left'>" + String(WxConditions[0].Snowrate * (Units == "M" ? 1 : 1.0936133), 1) + String(Units == "M" ? " mm/Hr" : " in/Hr") + "</td>";
    webpage += "   </tr>";
  }
  webpage += "   <tr>";
  webpage += "    <td style='text-align:left'>" + TXT_UVINDEX + "</td>";
  webpage += "    <td style='text-align:left'>" + String(round(WxConditions[0].uvIndex), 0) + " (" + UVIndexLevel(round(WxConditions[0].uvIndex)) + ")</td>";
  webpage += "   </tr>";
  webpage += "  </table>"; // Closed table-2
  webpage += "  </div>";
  webpage += " <div class='headcolumn'>";
  webpage += "  <table class='style1'>"; // Opened table-3
  webpage += "   <tr>";
  webpage += "    <td>" + WindDegToDir(WxConditions[0].Winddir, &WDirArrow) + " / " + String(WxConditions[0].Winddir, 0) + "&deg;" + WDirArrow + "</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td style='background-color:" + WindToClass(WxConditions[0].Windspeed, true) + "'>";
  webpage +=      String(WxConditions[0].Windspeed, WindspeedUnits == MPS ? 1 : 0) + " / " + String(WxConditions[0].Gust, WindspeedUnits == MPS ? 1 : 0) + " " + TXT_WINDUNITS + "</td>";
  webpage += "   </tr>";
  webpage += "   <tr>";
  webpage += "    <td style='background-color:" + WindToClass(WxConditions[0].Windspeed, true) + "'>" + WindToName(WxConditions[0].Windspeed) + "</td>";
  webpage += "   </tr>";
  webpage += "  </table>"; // Closed table-3
  webpage += " </div>";
  webpage += "</div>";
  webpage += "<table class='center'>"; // Opened table-4
  webpage += "  <tr>";
  webpage += "   <td>"; SunrisesetIcon(); webpage += "&uarr; " + ConvertUnixTime(WxConditions[0].Sunrise).substring(0, 5) + "</td>";
  webpage += "   <td>"; SunrisesetIcon(); webpage += "&darr; " + ConvertUnixTime(WxConditions[0].Sunset).substring(0, 5)  + "</td>";
  webpage += "   <td>"; webpage += GetMoonImage();   webpage += " " + GetMoonPhase() + "</td>";
  webpage += "  </tr>";
  webpage += "</table>"; // Closed table-4
  /// Next section every 3-hours
  webpage += "<hr>";
  webpage += "<div class='newtable'>";
  webpage += " <table class='newtable'>"; // Opened table-5
  webpage += "  <tr>";
  int r = 0;
  r = 0; do {
    if (r < max_periods - 1) webpage += "<td class='borderight' colspan='3' ><b>" + DayOfWeek(WxForecast[r].Dt) + "</b> (" + HourMinute(WxForecast[r].Dt) + ")</td>";
    else webpage += "<td colspan='3' ><b>" + DayOfWeek(WxForecast[r].Dt) + "</b> (" + HourMinute(WxForecast[r].Dt) + ")</td>";
    r++;
  } while (r < max_periods);
  webpage += "  </tr>";
  webpage += "  <tr>";
  r = 0; do {
    if (r < max_periods - 1) webpage += "<td class='borderight' colspan='3'><img src='http://openweathermap.org/img/w/" + WxForecast[r].Icon + ".png' alt='Forecast Symbol' style='width:5em;height:5em'></td>";
    else webpage += "<td colspan='3'><img src='http://openweathermap.org/img/w/" + WxForecast[r].Icon + ".png' alt='Forecast Symbol' style='width:5em;height:5em'></td>";
    r++;
  } while (r < max_periods);
  webpage += "  </tr>";
  webpage += "  <tr>";
  r = 0; do {
    if (r < max_periods - 1) webpage += "<td style='font-size:1.6em' colspan='3' class='borderight " + TempToClass(WxForecast[r].Temperature) + "'>" + String(WxForecast[r].Temperature, 0) + "&deg;</td>";
    else webpage += "<td style='font-size:1.6em' colspan='3' class='" + TempToClass(WxForecast[r].Temperature) + "'>" + String(WxForecast[r].Temperature, 0) + "&deg;</td>";
    r++;
  } while (r < max_periods);
  webpage += "  </tr>";
  webpage += "  <tr>";
  r = 0; do {
    if (r < max_periods - 1) webpage += "<td colspan='2'>" + RainDrop() + "</td><td class='borderight' style='padding:0;font-size:1.1em'>" + String(WxForecast[r].Pop * 100, 0) + "%</td>";
    else webpage += "<td colspan='2'>" + RainDrop() + "</td><td style='padding:0;font-size:1.1em'>" + String(WxForecast[r].Pop * 100, 0) + "%</td>";
    r++;
  } while (r < max_periods);
  webpage += "  </tr>";
  webpage += "  <tr>";
  r = 0; do {
    webpage += " <td class='" + TempToClass(WxForecast[r].High) + "'>" + String(WxForecast[r].High, 0) + "&deg;</td>";
    webpage += " <td class='" + TempToClass(WxForecast[r].Low) + "'>"  + String(WxForecast[r].Low, 0)  + "&deg;</td>";
    if (r < max_periods - 1) webpage += "<td class='borderight " + HumiToClass(WxForecast[r].Humidity)    + "'>" + String(WxForecast[r].Humidity, 0) + "%</td>";
    else webpage += "<td class='" + HumiToClass(WxForecast[r].Humidity) + "'>" + String(WxForecast[r].Humidity, 0) + "%</td>";
    r++;
  } while (r < max_periods);
  webpage += "  </tr>";
  webpage += "  <tr>";
  r = 0; do {
    WindDegToDir(WxForecast[r].Winddir, &WDirArrow);
    webpage += "<td colspan='3'><div class='numC " + WindToClass(windspeed_readings[r], true) + "' style='background-color:" + WindToClass(windspeed_readings[r], true) + "'>";
    webpage += String(windspeed_readings[r], WindspeedUnits == MPS ? 1 : 0) + WDirArrow + "</div></td>";
    r++;
  } while (r < max_periods);
  webpage += "  </tr>";
  webpage += " </table>"; // Closed table-5
  webpage += "</div><br>";
  // Next section Beaufort table display
  webpage += "<table class='center' style='width:35%;font-size:0.7em;'>";
  webpage += " <tr>";
  webpage += "  <td style='background-color:silver'>Beaufort Scale</td>";
  webpage += "  <td style='background-color:" + WindToClass(0, false)   + "'>0</td>";
  webpage += "  <td style='background-color:" + WindToClass(5, false)   + "'>1</td>";
  webpage += "  <td style='background-color:" + WindToClass(11, false)  + "'>2</td>";
  webpage += "  <td style='background-color:" + WindToClass(19, false)  + "'>3</td>";
  webpage += "  <td style='background-color:" + WindToClass(28, false)  + "'>4</td>";
  webpage += "  <td style='background-color:" + WindToClass(38, false)  + "'>5</td>";
  webpage += "  <td style='background-color:" + WindToClass(49, false)  + "'>6</td>";
  webpage += "  <td style='background-color:" + WindToClass(61, false)  + "'>7</td>";
  webpage += "  <td style='background-color:" + WindToClass(74, false)  + "'>8</td>";
  webpage += "  <td style='background-color:" + WindToClass(88, false)  + "'>9</td>";
  webpage += "  <td style='background-color:" + WindToClass(102, false) + "'>10</td>";
  webpage += "  <td style='background-color:" + WindToClass(117, false) + "'>11</td>";
  webpage += "  <td style='background-color:" + WindToClass(118, false) + "'>12</td>";
  webpage += " </tr>";
  webpage += "</table>";
  add_graphs();
  append_page_footer();
  webserver.send(200, "text/html", webpage);
  webpage = "";
}
//#########################################################################################
void Weathermap() {
  append_page_header(true);
  webpage += "<h4>Weather Map</h4>";
  webpage += "<iframe src='https://openweathermap.org/weathermap?basemap=map&cities=false&layer=radar&lat=";
  webpage +=  Latitude + "&lon=" + Longitude + "&zoom=10' width='100%' height='600px'></iframe>";
  append_page_footer();
  webserver.send(200, "text/html", webpage);
  webpage = "";
}
//#########################################################################################
void Help() {
  append_page_header(true);
  webpage += "<div style='text-align:left;'>";
  webpage += "<h4>System Set-up</h4>";
  webpage += "<a>1. Enter your City/Location name, choose any name.</a><br>";
  webpage += "<a>2. Enter your location Latitude and Longitude, e.g. 51.38, -2.33</a><br>";
  webpage += "<a>&nbsp;&nbsp;&nbsp;<i>- Use negative numbers for South of the Equator and East of the Meridian.</i></a><br>";
  webpage += "<a>3. Enter your Language e.g. 'en', see below for OWM supported languages.</a><br>";
  webpage += "<a>4. Enter your locations Hemisphere, North or South.</a><br>";
  webpage += "<a>5. Enter your desired wind units either MPS (Metres-per-Second), MPH (Miles-per-hour), KPH (Kilometres-per-hour) or KTS (Knots)</a><br>";
  webpage += "<a>6. Enter your desired Units either Metric or Imperial.</a><br>";
  webpage += "<a>7. Enter the text for each item in the Language Translation Table. e.g. for 'de' 'Pressure' enter 'Druck'</a><br>";
  webpage += "<a><i>&nbsp;&nbsp;&nbsp; - For months you could use '01' for JAN, etc.</i></a><br>";
  webpage += "<a>8. Click on ENTER to save</a><br>";
  webpage += "<h4>Open Weather Map supports the following languages:</h4>";
  webpage += "<div class='row'><div class='helpcolumn'>";
  webpage += "<a>af Afrikaans, al Albanian, ar Arabic, az Azerbaijani</a><br>";
  webpage += "<a>bg Bulgarian</a><br>";
  webpage += "<a>ca Catalan, cz Czech</a><br>";
  webpage += "<a>da Danish, de German</a><br>";
  webpage += "<a>el Greek, en English, eu Basque</a><br>";
  webpage += "<a>fa Persian (Farsi), fi Finnish, fr French</a><br>";
  webpage += "<a>gl Galician</a><br>";
  webpage += "<a>he Hebrew, hi Hindi, hr Croatian, hu Hungarian</a><br>";
  webpage += "<a>id Indonesian, it Italian</a><br>";
  webpage += "<a>ja Japanese</a><br>";
  webpage += "<a>kr Korean</a><br>";
  webpage += "</div><div class='helpcolumn'>";
  webpage += "<a>la Latvian, lt Lithuanian</a><br>";
  webpage += "<a>mk Macedonian<a/><br>";
  webpage += "<a>no Norwegian, nl Dutch</a><br>";
  webpage += "<a>pl Polish, pt Portuguese, pt_br Português Brasil</a><br>";
  webpage += "<a>ro Romanian, ru Russian</a><br>";
  webpage += "<a>sv, se Swedish, sk Slovak, sl Slovenian, sp, es Spanish, sr Serbian</a><br>";
  webpage += "<a>th Thai, tr Turkish</a><br>";
  webpage += "<a>ua, uk Ukrainian</a><br>";
  webpage += "<a>vi Vietnamese</a><br>";
  webpage += "<a>zh_cn Chinese Simplified, zh_tw Chinese Traditional, zu Zulu</a><br>";
  webpage += "</div></div>";
  webpage += "<a>&nbsp;&nbsp;&nbsp;<i> - Example: German set lauguage = 'de'</i></a><br>";
  webpage += "<h4>Weather descriptions used and translated by OpenWeatherMap:</h4>";
  webpage += "<a>Clear sky, Few clouds, Scattered clouds, Broken clouds, Shower rain, Rain, Heavy Intensity Rain, Moderate Rain, Light Rain, Thunderstorm, Snow, Mist</a><br>";
  webpage += "<h4>UV Index</h4>";
  webpage += "<a>The UV index (or ultraviolet index) is an international standard for measuring the ultraviolet radiation at a particular place and time.</a><br>";
  webpage += "<a><i>&nbsp;&nbsp;&nbsp; - Note: OWM issues the UV index at 1200 / Midday for the next 24-hours</i></a><br>";
  webpage += "<h4>Moon Phases</h4>";
  webpage += "<table class='helptable'>";
  webpage += "<tr><td>Phase-1</td><td>New</td></tr>";
  webpage += "<tr><td>Phase-2</td><td>Waxing Crescent</td></tr>";
  webpage += "<tr><td>Phase-3</td><td>First Quarter</td></tr>";
  webpage += "<tr><td>Phase-4</td><td>Waxing Gibbous</td></tr>";
  webpage += "<tr><td>Phase-5</td><td>Full</td></tr>";
  webpage += "<tr><td>Phase-6</td><td>Waning Gibbous</td></tr>";
  webpage += "<tr><td>Phase-7</td><td>Last Quarter</td></tr>";
  webpage += "<tr><td>Phase-8</td><td>Waning Crescent</td></tr>";
  webpage += "</table>";
  webpage += "<h4>Beafort Scale wind speeds</h4>";
  webpage += "<table class='helptable'>";
  webpage += "<tr><td>Force-0</td><td>'Calm'</td><td>&lt;1 knot &lt;1 mph &lt;2 km/h &lt;0.5 m/s</td></tr>";
  webpage += "<tr><td>Force-1</td><td>'Light Air'</td><td>1-3 knots 1-3 mph 2-5 km/h 0.5-1.5 m/s</td></tr>";
  webpage += "<tr><td>Force-2</td><td>'Light Breeze'</td><td>4-6 knots 4-7 mph 6-11 km/h 1.6-3.3 m/s</td></tr>";
  webpage += "<tr><td>Force-3</td><td>'Gentle Breeze'</td><td>7-10 knots 8-12 mph 12-19 km/h 3.4-5.5 m/s</td></tr>";
  webpage += "<tr><td>Force-4</td><td>'Moderate Breeze'</td><td>11-16 knots 13-18 mph 20-28 km/h 5.6-7.9 m/s</td></tr>";
  webpage += "<tr><td>Force-5</td><td>'Fresh Breeze'</td><td>17-21 knots 19-24 mph 29-38 km/h 8.0-10.7 m/s</td></tr>";
  webpage += "<tr><td>Force-6</td><td>'Strong Breeze'</td><td>22-27 knots 25-31 mph 39-48 km/h 10.8-13.8 m/s</td></tr>";
  webpage += "<tr><td>Force-7</td><td>'High Wind'</td><td>28-33 knots 32-38 mph 50-61 km/h 13.9-17.1 m/s</td></tr>";
  webpage += "<tr><td>Force-8</td><td>'Gale'</td><td>34-40 knots 39-46 mph 62-74 km/h 17.2-20.7 m/s</td></tr>";
  webpage += "<tr><td>Force-9</td><td>'Severe Gale'</td><td>41-47 knots 47-54 mph 75-88 km/h 20.8-24.4 m/s</td></tr>";
  webpage += "<tr><td>Force-10</td><td>'Storm'</td><td>48-55 knots 55-63 mph 89-102 km/h 24.5-28.4 m/s</td></tr>";
  webpage += "<tr><td>Force-11</td><td>'Violent Storm'</td><td>56-63 knots 64-72 mph 103-117 km/h 28.5-32.6 m/s</td></tr>";
  webpage += "<tr><td>Force-12</td><td>'Hurricane'</td><td>&gt;64 knots &gt;73 mph &gt;118 km/h > 32.6 m/s</td></tr>";
  webpage += "</table>";
  webpage += "<h4>OWM API Documentation</h4>";
  webpage += "<p>See the OpenWeatherMap site for more details of their API data: <a href='https://openweathermap.org/api'>Here</a></p>";
  webpage += "</div>";
  append_page_footer();
  webserver.send(200, "text/html", webpage);
  webpage = "";
}
//#########################################################################################
void SystemSetup() {
  String Windunits = "";
  if (WindspeedUnits == MPS) Windunits = "mps";
  if (WindspeedUnits == MPH) Windunits = "mph";
  if (WindspeedUnits == KPH) Windunits = "kph";
  if (WindspeedUnits == KTS) Windunits = "kts";
  append_page_header(true);
  webpage += "<h3>System Setup, change entries then Enter</h3>";
  webpage += "<FORM action='/handleinput' method='post'>";
  webpage += "<fieldset>";
  webpage += " <legend>System Setup Values: </legend>";
  webpage += " <table>";
  webpage += " <tr>";
  webpage += "  <td colspan='2'>City:<input type='text' name='city' value='" + City + "'></td>";
  webpage += "  <td colspan='1'>Lat:</td><td colspan='2'><input type='text' name='latitude' value='" + Latitude + "'></td>";
  webpage += "  <td colspan='1'>Lon:</td><td colspan='2'><input type='text' name='longitude' value='" + Longitude + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>Language:</td><td><input type='text' name='language' maxlength='2' size='2' value='" + Language + "'></td>";
  webpage += "  <td>Hemisphere:</td>";
  webpage += "  <td><select name='hemisphere'>";
  webpage += "  <option value='north' " + String(Hemisphere == "north" ? " selected='selected'" : "") + ">North</option>";
  webpage += "  <option value='south' " + String(Hemisphere == "south" ? " selected='selected'" : "") + ">South</option>";
  webpage += "  </select></td>";
  webpage += "  <td>Wind Units:</td>";
  webpage += "  <td><select name='windunits'>";
  webpage += "  <option value='mps' " + String(Windunits == "mps" ? " selected='selected'" : "") + ">MPS</option>";
  webpage += "  <option value='mph' " + String(Windunits == "mph" ? " selected='selected'" : "") + ">MPH</option>";
  webpage += "  <option value='kph' " + String(Windunits == "kph" ? " selected='selected'" : "") + ">KPH</option>";
  webpage += "  <option value='kts' " + String(Windunits == "kts" ? " selected='selected'" : "") + ">KTS</option>";
  webpage += " </select></td>";
  webpage += " <td>Units:</td>";
  webpage += " <td><select name='units'>";
  webpage += "  <option value='M' " + String(Units == "M" ? " selected='selected'" : "") + ">M</option>";
  webpage += "  <option value='I' " + String(Units == "I" ? " selected='selected'" : "") + ">I</option>";
  webpage += " </select></td>";
  webpage += "</tr></table>";
  webpage += "</fieldset>";
  // Language translations
  webpage += "<br>";
  //webpage += "<FORM action='/handleinput' method='post'>";
  webpage += "<fieldset>";
  webpage += " <legend>" + TXT_LANGUAGE_TABLE + ":</legend>";
  webpage += " <table>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_FORECAST_GRAPHS + ":</td><td><input type='text' name='TXT_FORECAST_GRAPHS' value='" + TXT_FORECAST_GRAPHS + "'></td><td></td><td></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_TEMPERATURE + ":</td><td><input type='text' name='TXT_TEMPERATURE' value='" + TXT_TEMPERATURE + "'></td>";
  webpage += "  <td>" + TXT_FEELSLIKE + ":</td><td><input type='text' name='TXT_FEELSLIKE'   value='" + TXT_FEELSLIKE   + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_HEATINDEX + ":</td><td><input type='text' name='TXT_HEATINDEX' value='" + TXT_HEATINDEX + "'></td>";
  webpage += "  <td>" + TXT_TEMPDEWPOINT + ":</td><td><input type='text' name='TXT_TEMPDEWPOINT' value='" + TXT_TEMPDEWPOINT + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_HUMIDITY + ":</td><td><input type='text' name='TXT_HUMIDITY' value='" + TXT_HUMIDITY + "'></td>";
  webpage += "  <td>" + TXT_DEWPOINT + ":</td><td><input type='text' name='TXT_DEWPOINT' value='" + TXT_DEWPOINT + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_WINDSPEED + ":</td><td><input type='text' name='TXT_WINDSPEED' value='" + TXT_WINDSPEED + "'></td>";
  webpage += "  <td>" + TXT_WINDCHILL + ":</td><td><input type='text' name='TXT_WINDCHILL' value='" + TXT_WINDCHILL + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_WCHILL_HINDEX + ":</td><td><input type='text' name='TXT_WCHILL_HINDEX' value='" + TXT_WCHILL_HINDEX + "'></td>";
  webpage += "  <td>" + TXT_PRESSURE + ":</td><td><input type='text' name='TXT_PRESSURE' value='" + TXT_PRESSURE + "'></td>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_CLOUDCOVER + ":</td><td><input type='text' name='TXT_CLOUDCOVER' value='" + TXT_CLOUDCOVER + "'></td>";
  webpage += "  <td>" + TXT_VISIBILITY + ":</td><td><input type='text' name='TXT_VISIBILITY' value='" + TXT_VISIBILITY + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_UVINDEX + ":</td><td><input type='text' name='TXT_UVINDEX' value='" + TXT_UVINDEX + "'></td>";
  webpage += "  <td>" + TXT_PRECIPITATION + ":</td><td><input type='text' name='TXT_PRECIPITATION' value='" + TXT_PRECIPITATION + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_RAINFALL + ":</td><td><input type='text' name='TXT_RAINFALL' value='" + TXT_RAINFALL + "'></td>";
  webpage += "  <td>" + TXT_RAINFALL + ":</td><td><input type='text' name='TXT_RAINRATE' value='" + TXT_RAINRATE + "'></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "  <td>" + TXT_SNOWFALL + ":</td><td><input type='text' name='TXT_SNOWFALL' value='" + TXT_SNOWFALL + "'></td>";
  webpage += "  <td>" + TXT_SNOWRATE + ":</td><td><input type='text' name='TXT_SNOWRATE' value='" + TXT_SNOWRATE + "'></td>";
  webpage += " </tr>";
  webpage += "</table>";
  webpage += "<br>";
  webpage += "<div class='row'>";
  webpage += "<div class='column'>";
  webpage += "<table>";
  webpage += " <tr><td>" + TXT_MOON_PHASE1 + ":</td><td><input type='text' name='TXT_MOON_PHASE1' value='" + TXT_MOON_PHASE1 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE2 + ":</td><td><input type='text' name='TXT_MOON_PHASE2' value='" + TXT_MOON_PHASE2 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE3 + ":</td><td><input type='text' name='TXT_MOON_PHASE3' value='" + TXT_MOON_PHASE3 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE4 + ":</td><td><input type='text' name='TXT_MOON_PHASE4' value='" + TXT_MOON_PHASE4 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE5 + ":</td><td><input type='text' name='TXT_MOON_PHASE5' value='" + TXT_MOON_PHASE5 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE6 + ":</td><td><input type='text' name='TXT_MOON_PHASE6' value='" + TXT_MOON_PHASE6 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE7 + ":</td><td><input type='text' name='TXT_MOON_PHASE7' value='" + TXT_MOON_PHASE7 + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MOON_PHASE8 + ":</td><td><input type='text' name='TXT_MOON_PHASE8' value='" + TXT_MOON_PHASE8 + "'></td></tr>";
  webpage += "</table></div><div class='column'>";
  webpage += "<table>";
  webpage += " <tr><td>" + TXT_N  + ":</td><td><input type='text' name='TXT_N'  maxlength='2' size='2' value='" + TXT_N  + "'></td></tr>";
  webpage += " <tr><td>" + TXT_NE + ":</td><td><input type='text' name='TXT_NE' maxlength='2' size='2' value='" + TXT_NE + "'></td></tr>";
  webpage += " <tr><td>" + TXT_E  + ":</td><td><input type='text' name='TXT_E'  maxlength='2' size='2' value='" + TXT_E  + "'></td></tr>";
  webpage += " <tr><td>" + TXT_SE + ":</td><td><input type='text' name='TXT_SE' maxlength='2' size='2' value='" + TXT_SE + "'></td></tr>";
  webpage += " <tr><td>" + TXT_S  + ":</td><td><input type='text' name='TXT_S'  maxlength='2' size='2' value='" + TXT_S  + "'></td></tr>";
  webpage += " <tr><td>" + TXT_SW + ":</td><td><input type='text' name='TXT_SW' maxlength='2' size='2' value='" + TXT_SW + "'></td></tr>";
  webpage += " <tr><td>" + TXT_W  + ":</td><td><input type='text' name='TXT_W'  maxlength='2' size='2' value='" + TXT_W  + "'></td></tr>";
  webpage += " <tr><td>" + TXT_NW + ":</td><td><input type='text' name='TXT_NW' maxlength='2' size='2' value='" + TXT_NW + "'></td></tr>";
  webpage += "</table></div><div class='column'>";
  webpage += "<table>";
  webpage += " <tr><td>" + TXT_HI + ":</td><td><input type='text' name='TXT_HI' maxlength='3' size='3' value='" + TXT_HI + "'></td></tr>";
  webpage += " <tr><td>" + TXT_LO + ":</td><td><input type='text' name='TXT_LO' maxlength='3' size='3' value='" + TXT_LO + "'></td></tr>";
  webpage += " <tr><td>" + TXT_RH + ":</td><td><input type='text' name='TXT_RH' maxlength='3' size='3' value='" + TXT_RH + "'></td></tr>";
  webpage += "</table></div><div class='column'>";
  // Days of Week
  webpage += "<table>";
  webpage += " <tr><td>" + TXT_SUN + ":</td><td><input type='text' name='TXT_SUN' maxlength='3' size='3' value='" + TXT_SUN + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MON + ":</td><td><input type='text' name='TXT_MON' maxlength='3' size='3' value='" + TXT_MON + "'></td></tr>";
  webpage += " <tr><td>" + TXT_TUE + ":</td><td><input type='text' name='TXT_TUE' maxlength='3' size='3' value='" + TXT_TUE + "'></td></tr>";
  webpage += " <tr><td>" + TXT_WED + ":</td><td><input type='text' name='TXT_WED' maxlength='3' size='3' value='" + TXT_WED + "'></td></tr>";
  webpage += " <tr><td>" + TXT_THU + ":</td><td><input type='text' name='TXT_THU' maxlength='3' size='3' value='" + TXT_THU + "'></td></tr>";
  webpage += " <tr><td>" + TXT_FRI + ":</td><td><input type='text' name='TXT_FRI' maxlength='3' size='3' value='" + TXT_FRI + "'></td></tr>";
  webpage += " <tr><td>" + TXT_SAT + ":</td><td><input type='text' name='TXT_SAT' maxlength='3' size='3' value='" + TXT_SAT + "'></td></tr>";
  webpage += "</table></div></div>";
  // Months
  webpage += "<br>";
  webpage += "<div class='row'><div class='column'>";
  webpage += "<table>";
  webpage += " <tr><td>" + TXT_JAN + ":</td><td><input type='text' name='TXT_JAN' maxlength='3' size='3' value='" + TXT_JAN + "'></td></tr>";
  webpage += " <tr><td>" + TXT_FEB + ":</td><td><input type='text' name='TXT_FEB' maxlength='3' size='3' value='" + TXT_FEB + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MAR + ":</td><td><input type='text' name='TXT_MAR' maxlength='3' size='3' value='" + TXT_MAR + "'></td></tr>";
  webpage += " <tr><td>" + TXT_APR + ":</td><td><input type='text' name='TXT_APR' maxlength='3' size='3' value='" + TXT_APR + "'></td></tr>";
  webpage += " <tr><td>" + TXT_MAY + ":</td><td><input type='text' name='TXT_MAY' maxlength='3' size='3' value='" + TXT_MAY + "'></td></tr>";
  webpage += " <tr><td>" + TXT_JUN + ":</td><td><input type='text' name='TXT_JUN' maxlength='3' size='3' value='" + TXT_JUN + "'></td></tr>";
  webpage += " <tr><td>" + TXT_JUL + ":</td><td><input type='text' name='TXT_JUL' maxlength='3' size='3' value='" + TXT_JUL + "'></td></tr>";
  webpage += " <tr><td>" + TXT_AUG + ":</td><td><input type='text' name='TXT_AUG' maxlength='3' size='3' value='" + TXT_AUG + "'></td></tr>";
  webpage += " <tr><td>" + TXT_SEP + ":</td><td><input type='text' name='TXT_SEP' maxlength='3' size='3' value='" + TXT_SEP + "'></td></tr>";
  webpage += " <tr><td>" + TXT_OCT + ":</td><td><input type='text' name='TXT_OCT' maxlength='3' size='3' value='" + TXT_OCT + "'></td></tr>";
  webpage += " <tr><td>" + TXT_NOV + ":</td><td><input type='text' name='TXT_NOV' maxlength='3' size='3' value='" + TXT_NOV + "'></td></tr>";
  webpage += " <tr><td>" + TXT_DEC + ":</td><td><input type='text' name='TXT_DEC' maxlength='3' size='3' value='" + TXT_DEC + "'></td></tr>";
  webpage += "</table></div>";
  // Beaufort scale
  webpage += "<br><div class='column'>";
  webpage += "<table>";
  webpage += " <tr><td>" + DEF_TXT_MBFS0  + ":</td><td><input type='text' name='TXT_BFS0'  maxlength='17' size='17' value='" + TXT_BFS0  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS1  + ":</td><td><input type='text' name='TXT_BFS1'  maxlength='17' size='17' value='" + TXT_BFS1  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS2  + ":</td><td><input type='text' name='TXT_BFS2'  maxlength='17' size='17' value='" + TXT_BFS2  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS3  + ":</td><td><input type='text' name='TXT_BFS3'  maxlength='17' size='17' value='" + TXT_BFS3  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS4  + ":</td><td><input type='text' name='TXT_BFS4'  maxlength='17' size='17' value='" + TXT_BFS4  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS5  + ":</td><td><input type='text' name='TXT_BFS5'  maxlength='17' size='17' value='" + TXT_BFS5  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS6  + ":</td><td><input type='text' name='TXT_BFS6'  maxlength='17' size='17' value='" + TXT_BFS6  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS7  + ":</td><td><input type='text' name='TXT_BFS7'  maxlength='17' size='17' value='" + TXT_BFS7  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS8  + ":</td><td><input type='text' name='TXT_BFS8'  maxlength='17' size='17' value='" + TXT_BFS8  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS9  + ":</td><td><input type='text' name='TXT_BFS9'  maxlength='17' size='17' value='" + TXT_BFS9  + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS10 + ":</td><td><input type='text' name='TXT_BFS10' maxlength='17' size='17' value='" + TXT_BFS10 + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS11 + ":</td><td><input type='text' name='TXT_BFS11' maxlength='17' size='17' value='" + TXT_BFS11 + "'></td></tr>";
  webpage += " <tr><td>" + DEF_TXT_MBFS12 + ":</td><td><input type='text' name='TXT_BFS12' maxlength='17' size='17' value='" + TXT_BFS12 + "'></td></tr>";
  webpage += "</table></div>";
  // Mernu items
  webpage += "<br><div class='column'>";
  webpage += "<table>";
  webpage += " <tr><td>" + TXT_HOME + "</td><td><input type='text'            name='TXT_HOME'            maxlength='10' size='10' value='" + TXT_HOME + "'></td></tr>";
  webpage += " <tr><td>" + TXT_WEATHERMAP + "</td><td><input type='text'      name='TXT_WEATHERMAP'      maxlength='17' size='17' value='" + TXT_WEATHERMAP + "'></td></tr>";
  webpage += " <tr><td>" + TXT_SYSTEMSETUP + "</td><td><input type='text'     name='TXT_SYSTEMSETUP'     maxlength='17' size='17' value='" + TXT_SYSTEMSETUP + "'></td></tr>";
  webpage += " <tr><td>" + TXT_PREFILLSETTINGS + "</td><td><input type='text' name='TXT_PREFILLSETTINGS' maxlength='17' size='17' value='" + TXT_PREFILLSETTINGS + "'></td></tr>";
  webpage += " <tr><td>" + TXT_HELP + "</td><td><input type='text'            name='TXT_HELP'            maxlength='17' size='17' value='" + TXT_HELP  + "'></td></tr>";
  webpage += "</table></div>";
  webpage += "</div>";
  webpage += "<br><input type='submit' value='Enter'><br><br>";
  webpage += "</fieldset></form>";
  append_page_footer();
  webserver.send(200, "text/html", webpage);
  webpage = "";
}
//#########################################################################################
void HandleInput() {
  City                = webserver.arg("city");                City.trim();
  Latitude            = webserver.arg("latitude");            Latitude.trim();
  Longitude           = webserver.arg("longitude");           Longitude.trim();
  Language            = webserver.arg("language");            Language.trim();
  Hemisphere          = webserver.arg("hemisphere");          Hemisphere.trim(); Hemisphere.toLowerCase();
  WindUnits           = webserver.arg("windunits");           WindUnits.trim(); WindUnits.toLowerCase();
  Units               = webserver.arg("units");               Units.trim();
  TXT_UVINDEX         = webserver.arg("TXT_UVINDEX");         TXT_UVINDEX.trim();
  TXT_FORECAST_GRAPHS = webserver.arg("TXT_FORECAST_GRAPHS"); TXT_FORECAST_GRAPHS.trim();
  TXT_TEMPERATURE     = webserver.arg("TXT_TEMPERATURE");     TXT_TEMPERATURE.trim();
  TXT_FEELSLIKE       = webserver.arg("TXT_FEELSLIKE");       TXT_FEELSLIKE.trim();
  TXT_TEMPDEWPOINT    = webserver.arg("TXT_TEMPDEWPOINT");    TXT_TEMPDEWPOINT.trim();
  TXT_HUMIDITY        = webserver.arg("TXT_HUMIDITY");        TXT_HUMIDITY.trim();
  TXT_DEWPOINT        = webserver.arg("TXT_DEWPOINT");        TXT_DEWPOINT.trim();
  TXT_HEATINDEX       = webserver.arg("TXT_HEATINDEX");       TXT_HEATINDEX.trim();
  TXT_WINDSPEED       = webserver.arg("TXT_WINDSPEED");       TXT_WINDSPEED.trim();
  TXT_WINDCHILL       = webserver.arg("TXT_WINDCHILL");       TXT_WINDCHILL.trim();
  TXT_WCHILL_HINDEX   = webserver.arg("TXT_WCHILL_HINDEX");   TXT_WCHILL_HINDEX.trim();
  TXT_PRESSURE        = webserver.arg("TXT_PRESSURE");        TXT_PRESSURE.trim();
  TXT_CLOUDCOVER      = webserver.arg("TXT_CLOUDCOVER");      TXT_CLOUDCOVER.trim();
  TXT_VISIBILITY      = webserver.arg("TXT_VISIBILITY");      TXT_VISIBILITY.trim();
  TXT_PRECIPITATION   = webserver.arg("TXT_PRECIPITATION");   TXT_PRECIPITATION.trim();
  TXT_RAINFALL        = webserver.arg("TXT_RAINFALL");        TXT_RAINFALL.trim();
  TXT_RAINRATE        = webserver.arg("TXT_RAINRATE");        TXT_RAINRATE.trim();
  TXT_SNOWFALL        = webserver.arg("TXT_SNOWFALL");        TXT_SNOWFALL.trim();
  TXT_SNOWRATE        = webserver.arg("TXT_SNOWRATE");        TXT_SNOWRATE.trim();
  TXT_MOON_PHASE1     = webserver.arg("TXT_MOON_PHASE1");     TXT_MOON_PHASE1.trim();
  TXT_MOON_PHASE2     = webserver.arg("TXT_MOON_PHASE2");     TXT_MOON_PHASE2.trim();
  TXT_MOON_PHASE3     = webserver.arg("TXT_MOON_PHASE3");     TXT_MOON_PHASE3.trim();
  TXT_MOON_PHASE4     = webserver.arg("TXT_MOON_PHASE4");     TXT_MOON_PHASE4.trim();
  TXT_MOON_PHASE5     = webserver.arg("TXT_MOON_PHASE5");     TXT_MOON_PHASE5.trim();
  TXT_MOON_PHASE6     = webserver.arg("TXT_MOON_PHASE6");     TXT_MOON_PHASE6.trim();
  TXT_MOON_PHASE7     = webserver.arg("TXT_MOON_PHASE7");     TXT_MOON_PHASE7.trim();
  TXT_MOON_PHASE8     = webserver.arg("TXT_MOON_PHASE8");     TXT_MOON_PHASE8.trim();
  TXT_HI              = webserver.arg("TXT_HI");              TXT_HI.trim();
  TXT_LO              = webserver.arg("TXT_LO");              TXT_LO.trim();
  TXT_RH              = webserver.arg("TXT_RH");              TXT_RH.trim();
  TXT_SUN             = webserver.arg("TXT_SUN");             TXT_SUN.trim();
  TXT_MON             = webserver.arg("TXT_MON");             TXT_MON.trim();
  TXT_TUE             = webserver.arg("TXT_TUE");             TXT_TUE.trim();
  TXT_WED             = webserver.arg("TXT_WED");             TXT_WED.trim();
  TXT_THU             = webserver.arg("TXT_THU");             TXT_THU.trim();
  TXT_FRI             = webserver.arg("TXT_FRI");             TXT_FRI.trim();
  TXT_SAT             = webserver.arg("TXT_SAT");             TXT_SAT.trim();
  TXT_JAN             = webserver.arg("TXT_JAN");             TXT_JAN.trim();
  TXT_FEB             = webserver.arg("TXT_FEB");             TXT_FEB.trim();
  TXT_MAR             = webserver.arg("TXT_MAR");             TXT_MAR.trim();
  TXT_APR             = webserver.arg("TXT_APR");             TXT_APR.trim();
  TXT_MAY             = webserver.arg("TXT_MAY");             TXT_MAY.trim();
  TXT_JUN             = webserver.arg("TXT_JUN");             TXT_JUN.trim();
  TXT_JUL             = webserver.arg("TXT_JUL");             TXT_JUL.trim();
  TXT_AUG             = webserver.arg("TXT_AUG");             TXT_AUG.trim();
  TXT_SEP             = webserver.arg("TXT_SEP");             TXT_SEP.trim();
  TXT_OCT             = webserver.arg("TXT_OCT");             TXT_OCT.trim();
  TXT_NOV             = webserver.arg("TXT_NOV");             TXT_NOV.trim();
  TXT_DEC             = webserver.arg("TXT_DEC");             TXT_DEC.trim();
  TXT_N               = webserver.arg("TXT_N");               TXT_N.trim();
  TXT_NE              = webserver.arg("TXT_NE");              TXT_NE.trim();
  TXT_E               = webserver.arg("TXT_E");               TXT_E.trim();
  TXT_SE              = webserver.arg("TXT_SE");              TXT_SE.trim();
  TXT_S               = webserver.arg("TXT_S");               TXT_S.trim();
  TXT_SW              = webserver.arg("TXT_SW");              TXT_SW.trim();
  TXT_W               = webserver.arg("TXT_W");               TXT_W.trim();
  TXT_NW              = webserver.arg("TXT_NW");              TXT_NW.trim();
  TXT_BFS0            = webserver.arg("TXT_BFS0");            TXT_BFS0.trim();
  TXT_BFS1            = webserver.arg("TXT_BFS1");            TXT_BFS1.trim();
  TXT_BFS2            = webserver.arg("TXT_BFS2");            TXT_BFS2.trim();
  TXT_BFS3            = webserver.arg("TXT_BFS3");            TXT_BFS3.trim();
  TXT_BFS4            = webserver.arg("TXT_BFS4");            TXT_BFS4.trim();
  TXT_BFS5            = webserver.arg("TXT_BFS5");            TXT_BFS5.trim();
  TXT_BFS6            = webserver.arg("TXT_BFS6");            TXT_BFS6.trim();
  TXT_BFS7            = webserver.arg("TXT_BFS7");            TXT_BFS7.trim();
  TXT_BFS8            = webserver.arg("TXT_BFS8");            TXT_BFS8.trim();
  TXT_BFS9            = webserver.arg("TXT_BFS9");            TXT_BFS9.trim();
  TXT_BFS10           = webserver.arg("TXT_BFS10");           TXT_BFS10.trim();
  TXT_BFS11           = webserver.arg("TXT_BFS11");           TXT_BFS11.trim();
  TXT_BFS12           = webserver.arg("TXT_BFS12");           TXT_BFS12.trim();
  TXT_HOME            = webserver.arg("TXT_HOME");            TXT_HOME.trim();
  TXT_WEATHERMAP      = webserver.arg("TXT_WEATHERMAP");      TXT_WEATHERMAP.trim();
  TXT_SYSTEMSETUP     = webserver.arg("TXT_SYSTEMSETUP");     TXT_SYSTEMSETUP.trim();
  TXT_PREFILLSETTINGS = webserver.arg("TXT_PREFILLSETTINGS"); TXT_PREFILLSETTINGS.trim();
  TXT_HELP            = webserver.arg("TXT_HELP");            TXT_HELP.trim();
  TXT_LANGUAGE_TABLE  = webserver.arg("TXT_LANGUAGE_TABLE");  TXT_LANGUAGE_TABLE.trim();
  Units.toUpperCase();
  if (WindUnits == "mps") {
    WindspeedUnits = MPS;
    TXT_WINDUNITS  = "mps";
  }
  if (WindUnits == "mph") {
    WindspeedUnits = MPH;
    TXT_WINDUNITS  = "mph";
  }
  if (WindUnits == "kph") {
    WindspeedUnits = KPH;
    TXT_WINDUNITS  = "kph";
  }
  if (WindUnits == "kts") {
    WindspeedUnits = KTS;
    TXT_WINDUNITS  = "kts";
  }
  SaveSettings();
  UpdateData();
  Homepage();
}
//#########################################################################################
void PreFill() {
  City                = "Bath";
  Latitude            = "51.3704";
  Longitude           = "-2.1376";
  Language            = "en";
  Hemisphere          = "north";
  Units               = "M";
  TXT_FORECAST_GRAPHS = DEF_TXT_FORECAST_GRAPHS;
  TXT_TEMPDEWPOINT    = DEF_TXT_TEMPDEWPOINT;
  TXT_TEMPERATURE     = DEF_TXT_TEMPERATURE;
  TXT_FEELSLIKE       = DEF_TXT_FEELSLIKE;
  TXT_HUMIDITY        = DEF_TXT_HUMIDITY;
  TXT_DEWPOINT        = DEF_TXT_DEWPOINT;
  TXT_WINDCHILL       = DEF_TXT_WINDCHILL;
  TXT_WCHILL_HINDEX   = DEF_TXT_WCHILL_HINDEX;
  TXT_HEATINDEX       = DEF_TXT_HEATINDEX;
  TXT_WINDSPEED       = DEF_TXT_WINDSPEED;
  TXT_PRESSURE        = DEF_TXT_PRESSURE;
  TXT_CLOUDCOVER      = DEF_TXT_CLOUDCOVER;
  TXT_VISIBILITY      = DEF_TXT_VISIBILITY;
  TXT_PRECIPITATION   = DEF_TXT_PRECIPITATION;
  TXT_RAINFALL        = DEF_TXT_RAINFALL;
  TXT_RAINRATE        = DEF_TXT_RAINRATE;
  TXT_SNOWFALL        = DEF_TXT_SNOWFALL;
  TXT_SNOWRATE        = DEF_TXT_SNOWRATE;
  TXT_UVINDEX         = DEF_TXT_UVINDEX;
  //Moon phases
  TXT_MOON_PHASE1     = DEF_TXT_MOON_PHASE1;
  TXT_MOON_PHASE2     = DEF_TXT_MOON_PHASE2;
  TXT_MOON_PHASE3     = DEF_TXT_MOON_PHASE3;
  TXT_MOON_PHASE4     = DEF_TXT_MOON_PHASE4;
  TXT_MOON_PHASE5     = DEF_TXT_MOON_PHASE5;
  TXT_MOON_PHASE6     = DEF_TXT_MOON_PHASE6;
  TXT_MOON_PHASE7     = DEF_TXT_MOON_PHASE7;
  TXT_MOON_PHASE8     = DEF_TXT_MOON_PHASE8;
  //Wind
  TXT_N               = DEF_TXT_N;
  TXT_NE              = DEF_TXT_NE;
  TXT_E               = DEF_TXT_E;
  TXT_SE              = DEF_TXT_SE;
  TXT_S               = DEF_TXT_S;
  TXT_SW              = DEF_TXT_SW;
  TXT_W               = DEF_TXT_W;
  TXT_NW              = DEF_TXT_NW;
  WindspeedUnits      = MPH;    // Change to your desired display format e.g. for KPH 'MetricWindspeed = KPH';
  TXT_WINDUNITS       = DEF_TXT_WINDUNITS; // Ensure text here matahces units chosen above! Use a leading space
  //Windspeed warning colours based on Beaufort scale in kph *** NOTE these are HTML colours, don't translate!
  TXT_BFS0            = DEF_TXT_BFS0;
  TXT_BFS1            = DEF_TXT_BFS1;
  TXT_BFS2            = DEF_TXT_BFS2;
  TXT_BFS3            = DEF_TXT_BFS3;
  TXT_BFS4            = DEF_TXT_BFS4;
  TXT_BFS5            = DEF_TXT_BFS5;
  TXT_BFS6            = DEF_TXT_BFS6;
  TXT_BFS7            = DEF_TXT_BFS7;
  TXT_BFS8            = DEF_TXT_BFS8;
  TXT_BFS9            = DEF_TXT_BFS9;
  TXT_BFS10           = DEF_TXT_BFS10;
  TXT_BFS11           = DEF_TXT_BFS11;
  TXT_BFS12           = DEF_TXT_BFS12;
  //Days of the week
  TXT_SUN             = DEF_TXT_SUN;
  TXT_MON             = DEF_TXT_MON;
  TXT_TUE             = DEF_TXT_TUE;
  TXT_WED             = DEF_TXT_WED;
  TXT_THU             = DEF_TXT_THU;
  TXT_FRI             = DEF_TXT_FRI;
  TXT_SAT             = DEF_TXT_SAT;
  //Months
  TXT_JAN             = DEF_TXT_JAN;
  TXT_FEB             = DEF_TXT_FEB;
  TXT_MAR             = DEF_TXT_MAR;
  TXT_APR             = DEF_TXT_APR;
  TXT_MAY             = DEF_TXT_MAY;
  TXT_JUN             = DEF_TXT_JUN;
  TXT_JUL             = DEF_TXT_JUL;
  TXT_AUG             = DEF_TXT_AUG;
  TXT_SEP             = DEF_TXT_SEP;
  TXT_OCT             = DEF_TXT_OCT;
  TXT_NOV             = DEF_TXT_NOV;
  TXT_DEC             = DEF_TXT_DEC;
  TXT_HI              = DEF_TXT_HI;
  TXT_LO              = DEF_TXT_LO;
  TXT_RH              = DEF_TXT_RH;
  TXT_HOME            = DEF_TXT_HOME;
  TXT_WEATHERMAP      = DEF_TXT_WEATHERMAP;
  TXT_SYSTEMSETUP     = DEF_TXT_SYSTEMSETUP;
  TXT_PREFILLSETTINGS = DEF_TXT_PREFILLSETTINGS;
  TXT_HELP            = DEF_TXT_HELP;
  TXT_LANGUAGE_TABLE  = DEF_TXT_LANGUAGE_TABLE;
  SaveSettings();
  UpdateData();
  Homepage();
}
//#########################################################################################
void append_page_header(bool Header) {
  webpage  = "<!DOCTYPE html>";
  webpage += "<html lang='" + Language + "' charset='utf-8' xml:lang='" + Language + "' xmlns='http://www.w3.org/1999/xhtml'>";
  webpage += "<head>";
  webpage += "<meta http-equiv='refresh' content='180'>"; // 180-sec screen refresh time
  webpage += "<meta http-equiv='content-type' content='text/html; charset=utf-8'>";
  webpage += "<title>Weather Webserver</title>";
  webpage += "<style>";
  webpage += " body           {width:68em;margin-left:auto;margin-right:auto;font-family:Arial,Helvetica,sans-serif;font-size:14px;";
  webpage += "                 color:blue;background-color:#e1e1ff;text-align:center;}";
  webpage += " table, th, td, tr {border-collapse:collapse;text-align:center;padding:0.357em;margin:0 0 0 0;}";
  webpage += " *              {box-sizing:border-box;}";
  webpage += " .column        {float:left;width:auto;padding:0.214em;}";
  webpage += " .headcolumn    {float:left;width:25%;padding:0.214em;}";
  webpage += " .helpcolumn    {float:left;width:30%;padding:0.214em;}";
  webpage += " .row:after     {content:'';clear:both;display:table;}";
  webpage += " .newtable     *{table-layout:fixed;padding:0.357em;padding-top:0.14em;padding-bottom:0;text-align:center;}";
  webpage += " .newtable   td {width:7em;}";
  webpage += " .helptable    *{table-layout:fixed;padding-top:0;padding-bottom:0;padding-left:0;text-align:left;}";
  webpage += " .helptable tr td {width:auto;}";
  webpage += " .helptable tr td:first-child{width:auto;}";
  webpage += " .style1        {font-size:1.78em;background-color:#e7d8e7;}";
  webpage += " .style2        {font-size:2.14em;color:#9796fd;line-height:80%;}";
  webpage += " .style5        {font-size:1.14em; background-color:#e7d8e7;}";
  webpage += " .style6        {font-size:2.14em;color:#cb96fd;line-height:80%;}";
  webpage += " .center        {margin-left:auto;margin-right:auto;}";
  webpage += " h3             {margin-top:0.3em;margin-bottom:0.3em;}";
  webpage == " ul             {list-style-type:none;margin:0;padding:0.142em;overflow:hidden;background-color:#B4DAFF;font-size:1.142em;}";
  webpage += " li             {list-style-type:none;float:left;border-right:0.1em solid #bbb;}last-child {border-right:none;}";
  webpage += " li a           {display:inline;padding:0.357em 0.357em;text-decoration:none;}";
  webpage += " li a:hover     {background-color:#F8F8F8;}";
  webpage += " h1             {background-color:#d8d8d8;}";
  webpage += " #outer         {width:100%;display:flex;justify-content:center;}";
  webpage += " .tempH         {color:salmon;}";
  webpage += " .tempM         {color:seaGreen;}";
  webpage += " .tempL         {color:blue;}";
  webpage += " .humiH         {color:salmon;}";
  webpage += " .humiM         {color:seaGreen;}";
  webpage += " .humiL         {color:blue;}";
  webpage += " h1.conditions  {color:slateblue;font-size:1.7em;font-weight:normal;border-collapse:collapse;margin:0 auto;}";
  webpage += " #conditions    {color:blue;font-size:1.0em;margin-left:auto;margin-right:auto;vertical-align:top;margin:0 auto}";
  webpage += " #conditions td {padding:0px 1.4em 0 1.4em}";
  webpage += " .numC          {border-radius:50%;width:3em;height:3em;padding:0.2em;padding-top:0.75em;background:#fff;border:0.1em solid #666;color:#666;";
  webpage += "                 margin-left:auto;margin-right:auto;font-size:1em;}";
  webpage += " footer         {padding:0.1em 0.1em 0.1em 0.1em;background-color:#e7d8e7;font-size:1em;}";
  webpage += " fieldset       {border:0.1em solid blue;}";
  webpage += " .borderight    {border-right:1px dotted blue;}";
  webpage += "</style>";
  webpage += "</head>";
  webpage += "<body>";
  UpdateLocalTime();
  webpage += "<h1><span>ESP Weather Server </span><span style='font-size:12px;'> (" + Time_str + " / " + Date_str + " )</span></h1>";
  if (Header) {
    webpage += "<h3 class='style2'>" + City + "</h3>";
  }
}
//#########################################################################################
void append_page_footer() { // Saves repeating many lines of code for HTML page footers
  webpage += "<footer><ul>";
  webpage += "<li><a href = '/'>" + DEF_TXT_HOME + "</a></li>";
  webpage += "<li><a href = '/wxmap'>" + DEF_TXT_WEATHERMAP + "</a></li>";
  webpage += "<li><a href = '/setup'>" + DEF_TXT_SYSTEMSETUP + "</a></li>";
  webpage += "<li><a href = '/prefillEN'>" + DEF_TXT_PREFILLSETTINGS + "</a></li>";
  webpage += "<li><a href = '/help'>" + DEF_TXT_HELP + "</a></li></ul>";
  webpage += "<p>&copy; Open Weather Map Data and Icons 2021 / ";
  webpage += "&copy; " + String(char(byte(0x40 >> 1))) + String(char(byte(0x88 >> 1))) + String(char(byte(0x5c >> 1))) + String(char(byte(0x98 >> 1))) + String(char(byte(0x5c >> 1)));
  webpage += String(char((0x84 >> 1))) + String(char(byte(0xd2 >> 1))) + String(char(0xe4 >> 1)) + String(char(0xc8 >> 1)) + String(char(byte(0x40 >> 1)));
  webpage += String(char(byte(0x64 / 2))) + String(char(byte(0x60 >> 1))) + String(char(byte(0x64 >> 1))) + String(char(0x62 >> 1)) + "</p>";
  webpage += "</footer></body></html>";
}
//#########################################################################################
void add_graphs() {
  int r = 0;
  String WDirArrow = "";
  //webpage += "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>";
  webpage += "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>";
  webpage += "<script type='text/javascript'>";
  webpage += "google.charts.load('current', {'packages':['corechart']});";
  webpage += "google.charts.setOnLoadCallback(drawPressure);";
  webpage += "google.charts.setOnLoadCallback(drawTemperature);";
  webpage += "google.charts.setOnLoadCallback(drawWindChillHeatIndex);";
  webpage += "google.charts.setOnLoadCallback(drawHumidity);";
  webpage += "google.charts.setOnLoadCallback(drawWindspeedGustSpeed);";
  webpage += "google.charts.setOnLoadCallback(drawRainfall);";
  // Pressure Graph
  webpage += "function drawPressure() {";
  webpage += " var data = google.visualization.arrayToDataTable([";
  webpage += " ['Hour', '" + TXT_PRESSURE + "'],";
  r = 0;
  do {
    webpage += "[" + String(r) + "," + String(pressure_readings[r]) + "],";
    r++;
  } while (r < max_readings);
  webpage += " ]);";
  webpage += " var options = {";
  webpage += "  title: '" + TXT_PRESSURE + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour  + "' },";
  webpage += "  hAxis: {color: '#FFF'},";
  webpage += "  vAxis: {color: '#FFF', title: '" + String((Units == "M" ? "hPa" : "in")) + "'},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width: 310,";
  webpage += "  height: 194,";
  webpage += "  colors:['" + data1Colour + "','" + data2Colour + "'],";
  webpage += "  chartArea:{left:50,top:20,bottom:50,right:10,width:'100%',height:'100%'},";
  webpage += "  legend: { position: 'bottom' }";
  webpage += " };";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('chart_divP'));";
  webpage += "  chart.draw(data, options);";
  webpage += " }";
  // Temperature Graph
  webpage += "function drawTemperature() {";
  webpage += " var data = google.visualization.arrayToDataTable([";
  webpage += "  ['Hour', '" + TXT_TEMPERATURE + "', '" + TXT_DEWPOINT + "'],";
  r = 0;
  do {
    webpage += "[" + String(r) + "," + String(temperature_readings[r]) + "," + String(dewpoint_readings[r]) + "],";
    r++;
  } while (r < max_readings);
  webpage += "  ]);";
  webpage += " var options = {";
  webpage += "  title: '" + TXT_TEMPDEWPOINT + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour  + "' },";
  webpage += "  hAxis: {color: '#FFF'},";
  webpage += "  vAxis: {color: '#FFF', title: '" + String(Units == "M" ? "°C" : "°F") + "'},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width: 310,";
  webpage += "  height: 194,";
  webpage += "  colors:['" + data1Colour + "','" + data2Colour + "'],";
  webpage += "  chartArea:{left:50,top:20,bottom:50,right:10,width:'100%',height:'100%'},";
  webpage += "  legend: { position: 'bottom' }";
  webpage += " };";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('chart_divT'));";
  webpage += "  chart.draw(data, options);";
  webpage += " }";
  // Windchill / Heatindex Graph
  webpage += "function drawWindChillHeatIndex() {";
  webpage += " var data = google.visualization.arrayToDataTable([";
  webpage += "  ['Hour', '" + TXT_WINDCHILL + "','" + TXT_HEATINDEX + "'],";
  r = 0;
  do {
    webpage += "[" + String(r) + "," + String(windchill_readings[r]) + "," + String(heatindex_readings[r]) + "],";
    r++;
  } while (r < max_readings);
  webpage += "   ]);";
  webpage += " var options = {";
  webpage += "  title: '" + TXT_WCHILL_HINDEX + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour  + "' },";
  webpage += "  hAxis: {color: '#FFF'},";
  webpage += "  vAxis: {color: '#FFF', title: '" + String(Units == "M" ? "°C" : "°F") + "'},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width: 310,";
  webpage += "  height: 194,";
  webpage += "  colors:['" + data1Colour + "','" + data2Colour + "'],";
  webpage += "  chartArea:{left:50,top:20,bottom:50,right:10,width:'100%',height:'100%'},";
  webpage += "  legend: { position: 'bottom' }";
  webpage += " };";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('chart_divWCHI'));";
  webpage += "  chart.draw(data, options);";
  webpage += " }";
  // Humidity Graph
  webpage += "function drawHumidity() {";
  webpage += " var data = google.visualization.arrayToDataTable([";
  webpage += "  ['Hour', '" + TXT_HUMIDITY + "'],";
  r = 0;
  do {
    webpage += "[" + String(r) + "," + String(humidity_readings[r]) + "],";
    r++;
  } while (r < max_readings);
  webpage += "  ]);";
  webpage += " var options = {";
  webpage += "  title: '" + TXT_HUMIDITY + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour  + "' },";
  webpage += "  hAxis: {color: '#FFF'},";
  webpage += "  vAxis: {color: '#FFF', title: '%', minValue: 0, ticks: [0, 20, 40, 60, 80, 100]},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width: 310,";
  webpage += "  height: 194,";
  webpage += "  colors:['" + data1Colour + "','" + data2Colour + "'],";
  webpage += "  chartArea:{left:50,top:20,bottom:50,right:10,width:'100%',height:'100%'},";
  webpage += "  legend: { position: 'bottom' }";
  webpage += " };";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('chart_divH'));";
  webpage += "  chart.draw(data, options);";
  webpage += " }";
  // Windspeed Graph
  webpage += "function drawWindspeedGustSpeed() {";
  webpage += " var data = google.visualization.arrayToDataTable([";
  webpage += "  ['Hour', '" + TXT_WINDSPEED + "'],";
  r = 0;
  do {
    webpage += "[" + String(r) + "," + String(windspeed_readings[r]) + "],";
    r++;
  } while (r < max_readings);
  webpage += "  ]);";
  webpage += " var options = {";
  webpage += "  title: '" + TXT_WINDSPEED + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour  + "' },";
  webpage += "  hAxis: {color: '#FFF'},";
  webpage += "  vAxis: {color: '#FFF', title: '" + TXT_WINDUNITS + "'},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width: 310,";
  webpage += "  height: 194,";
  webpage += "  colors:['" + data1Colour + "','" + data2Colour + "'],";
  //webpage += "  vAxis:{viewWindow:{min:0}},"; // Surpress negative axis
  webpage += "  chartArea:{left:50,top:20,bottom:50,right:10,width:'100%',height:'100%'},";
  webpage += "  legend: { position: 'bottom' }";
  webpage += " };";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('chart_divWSGS'));";
  webpage += "  chart.draw(data, options);";
  webpage += " }";
  // Precipitation Graph
  webpage += "function drawRainfall() {";
  webpage += " var data = google.visualization.arrayToDataTable([";
  webpage += "  ['Hour', '" + TXT_RAINFALL + "','" + TXT_SNOWFALL + "'],";
  r = 0;
  do {
    webpage += "['" + String(r) + "'," + String(rain_readings[r]) + "," + String(snow_readings[r]) + "],";
    r++;
  } while (r < max_readings);
  webpage += " ]);";
  webpage += " var options = {";
  webpage += "  title: '" + TXT_PRECIPITATION + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour  + "' },";
  webpage += "  hAxis: {color: '#FFF', slantedText:false },";
  webpage += "  vAxis: {color: '#FFF', title: '" + String((Units == "M" ? "mm" : "in")) + "',";
  webpage += "  viewWindowMode:'explicit',";
  webpage += "  viewWindow: {min:0}},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width: 310,";
  webpage += "  height: 194,";
  webpage += "  colors:['" + data1Colour + "','" + data2Colour + "'],";
  //webpage += "  vAxis:{viewWindow:{min:0}},"; // Surpress negative axis
  webpage += "  chartArea:{left:50,top:20,bottom:50,right:10,width:'100%',height:'100%'},";
  webpage += "  legend: {position:'bottom'}";
  webpage += "};";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('chart_divRain'));";
  webpage += "  chart.draw(data, options);";
  webpage += "}";
  webpage += "</script>";
  // Start of main HTML page content
  webpage += "<hr>";
  webpage += "<h1 id='conditions' style='color:blueViolet;font-weight:normal;font-size:1.2em;'>" + TXT_FORECAST_GRAPHS + "</h1>";
  webpage += "<div id='outer'>";
  webpage += "<table>";
  webpage += "<tr>";
  webpage += "  <td><div id='chart_divP'></div></td>";
  webpage += "  <td><div id='chart_divT'></div></td>";
  webpage += "  <td><div id='chart_divWCHI'></div></td>";
  webpage += " </tr>";
  webpage += " <tr>";
  webpage += "   <td><div id='chart_divH'></div></td>";
  webpage += "   <td><div id='chart_divWSGS'></div></td>";
  webpage += "   <td><div id='chart_divRain'></div></td>";
  webpage += " </tr>";
  webpage += "</table>";
  webpage += "</div>";
}
//#########################################################################################
// Handle file not found situation
void handleNotFound() {
  String message;
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i = 0; i < webserver.args(); i++) {
    message += " NAME:" + webserver.argName(i) + "\n VALUE:" + webserver.arg(i) + "\n";
  }
  webserver.send(404, "text/plain", message);
  Serial.print(message);
}
//#########################################################################################
uint8_t StartWiFi() {
  Serial.print("\r\nConnecting to: "); Serial.println(String(ssid));
  IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  delay(1200);
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool   AttemptConnection = true;
  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  }
  return connectionStatus;
}
//#########################################################################################
boolean SetupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, "time.nist.gov"); //(gmtOffset_sec, daylightOffset_sec, ntpServer)
  setenv("TZ", Timezone, 1);  //setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
  delay(200);
  bool TimeStatus = UpdateLocalTime();
  return TimeStatus;
}
//#########################################################################################
boolean UpdateLocalTime() {
  struct tm timeinfo;
  char   time_output[30], day_output[30], update_time[30];
  while (!getLocalTime(&timeinfo, 15000)) { // Wait for up to 15-sec for time to synchronise
    return false;
  }
  //See http://www.cplusplus.com/reference/ctime/strftime/
  Serial.println(&timeinfo, "%a %b %d %Y   %H:%M:%S");      // Displays: Saturday, June 24 2017 14:05:49
  if (Units == "M") {
    sprintf(day_output, "%s %02u-%s-%04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeinfo);  // Creates: '14:05:49'
    sprintf(time_output, "%s %s", TXT_UPDATED, update_time);
  }
  else
  {
    strftime(day_output, sizeof(day_output), "%a %b-%d-%Y", &timeinfo); // Creates  'Sat May-31-2019'
    strftime(update_time, sizeof(update_time), "%r", &timeinfo);        // Creates: '02:05:49pm'
    sprintf(time_output, "%s %s", TXT_UPDATED, update_time);
  }
  Date_str = day_output;
  Time_str = time_output;
  return true;
}
//#########################################################################################
String HourMinute(int unix_time) {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  // See http://www.cplusplus.com/reference/ctime/strftime/
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  if (Units == "M") {
    strftime(output, sizeof(output), "%H:%M", now_tm);
  }
  else
  {
    strftime(output, sizeof(output), "%I:%M%P", now_tm);
  }
  return String(output);
}
//#########################################################################################
void SetupDeviceName(const char *DeviceName) {
  if (MDNS.begin(DeviceName)) { // The name that will identify your device on the network
    Serial.println("mDNS responder started");
    Serial.print("Device name: ");
    Serial.println(DeviceName);
    MDNS.addService("n8i-mlp", "tcp", 23); // Add service
  }
  else {
    Serial.println("Error setting up MDNS responder");
  }
}
//#########################################################################################
String WindDegToDir(float winddirection, String * WDirArrow) {
  if (winddirection >= 337.5 || winddirection < 22.5)  {
    *WDirArrow = "&uArr;";
    return TXT_N;
  }
  if (winddirection >=  22.5 && winddirection < 67.5)  {
    *WDirArrow = "&neArr;";
    return TXT_NE;
  }
  if (winddirection >=  67.5 && winddirection < 112.5) {
    *WDirArrow = "&rArr;";
    return TXT_E;
  }
  if (winddirection >= 112.5 && winddirection < 157.5) {
    *WDirArrow = "&seArr;";
    return TXT_SE;
  }
  if (winddirection >= 157.5 && winddirection < 202.5) {
    *WDirArrow = "&dArr;";
    return TXT_S;
  }
  if (winddirection >= 202.5 && winddirection < 247.5) {
    *WDirArrow = "&swArr;";
    return TXT_SW;
  }
  if (winddirection >= 247.5 && winddirection < 292.5) {
    *WDirArrow = "&lArr;";
    return TXT_W;
  }
  if (winddirection >= 292.5 && winddirection < 337.5) {
    *WDirArrow = "&nwArr;";
    return TXT_NW;
  }
  return "?";
}
//#########################################################################################
String WindToClass(float windspeed, bool convert) {               // converts windspeed to colours representing the Beaufort scale
  if (convert) {
    if (WindspeedUnits == MPS) windspeed *= 3.6;      // Convert mps to kph for a kph based Beaufort scale
    if (WindspeedUnits == MPH) windspeed *= 1.609344; // Convert mph to kph for a kph based Beaufort scale
    if (WindspeedUnits == KTS) windspeed *= 1.852;    // Convert kts to kph for a kph based Beaufort scale
  }
  if (windspeed <  2)                      return Calm;           // 0
  if (windspeed >= 2   && windspeed < 6)   return LightAir;       // 1
  if (windspeed >= 6   && windspeed < 12)  return LightBreeze;    // 2
  if (windspeed >= 12  && windspeed < 20)  return GentleBreeze;   // 3
  if (windspeed >= 20  && windspeed < 29)  return ModerateBreeze; // 4
  if (windspeed >= 29  && windspeed < 39)  return FreshBreeze;    // 5
  if (windspeed >= 39  && windspeed < 50)  return StrongBreeze;   // 6
  if (windspeed >= 50  && windspeed < 62)  return HighWind;       // 7
  if (windspeed >= 62  && windspeed < 75)  return Gale;           // 8
  if (windspeed >= 75  && windspeed < 89)  return SevereGale;     // 9
  if (windspeed >= 89  && windspeed < 103) return Storm;          // 10
  if (windspeed >= 103 && windspeed < 118) return ViolentStorm;   // 11
  if (windspeed >= 118)                    return Hurricane;      // 12
  return "White";
}
//#########################################################################################
String WindToName(float windspeed) {                // converts windspeed to Beaufort scale names
  if (WindspeedUnits == MPS) windspeed *= 3.6;      // Convert mps to kph for a kph based Beaufort scale
  if (WindspeedUnits == MPH) windspeed *= 1.609344; // Convert mph to kph for a kph based Beaufort scale
  if (WindspeedUnits == KTS) windspeed *= 1.852;    // Convert kts to kph for a kph based Beaufort scale
  if (windspeed <  2)                      return TXT_BFS0;  // Force 0
  if (windspeed >= 2   && windspeed < 6)   return TXT_BFS1;  // Force 1
  if (windspeed >= 6   && windspeed < 12)  return TXT_BFS2;  // Force 2
  if (windspeed >= 12  && windspeed < 20)  return TXT_BFS3;  // Force 3
  if (windspeed >= 20  && windspeed < 29)  return TXT_BFS4;  // Force 4
  if (windspeed >= 29  && windspeed < 39)  return TXT_BFS5;  // Force 5
  if (windspeed >= 39  && windspeed < 50)  return TXT_BFS6;  // Force 6
  if (windspeed >= 50  && windspeed < 62)  return TXT_BFS7;  // Force 7
  if (windspeed >= 62  && windspeed < 75)  return TXT_BFS8;  // Force 8
  if (windspeed >= 75  && windspeed < 89)  return TXT_BFS9;  // Force 9
  if (windspeed >= 89  && windspeed < 103) return TXT_BFS10; // Force 10
  if (windspeed >= 103 && windspeed < 118) return TXT_BFS11; // Force 11
  if (windspeed >= 118)                    return TXT_BFS12; // Force 12
  return "White";
}
//#########################################################################################
float Convert_to_WS_Units(float windspeed) {
  if (Units == "M" && WindspeedUnits == MPH) windspeed *= 2.23693629; // mps to mph
  if (Units == "M" && WindspeedUnits == KPH) windspeed *= 3.6;        // mps to kph
  if (Units == "M" && WindspeedUnits == KTS) windspeed *= 1.94384449; // mps to kts
  if (Units == "I" && WindspeedUnits == MPS) windspeed *= 0.44704;    // mph to mps
  if (Units == "I" && WindspeedUnits == KPH) windspeed *= 1.609344;   // mph to kph
  if (Units == "I" && WindspeedUnits == KTS) windspeed *= 0.86897624; // mph to kts
  return windspeed;
}
//#########################################################################################
String GetMoonImage() {
  String MoonPhase = GetMoonPhase();
  if (Hemisphere == "north") {
    if (MoonPhase == TXT_MOON_PHASE1) return MoonNew();            // New;              0%  illuminated
    if (MoonPhase == TXT_MOON_PHASE2) return MoonWaxingCrescent(); // Waxing crescent; 25%  illuminated
    if (MoonPhase == TXT_MOON_PHASE3) return MoonFirstQuarter();   // First quarter;   50%  illuminated
    if (MoonPhase == TXT_MOON_PHASE4) return MoonWaxingGibbous();  // Waxing gibbous;  75%  illuminated
    if (MoonPhase == TXT_MOON_PHASE5) return MoonFull();           // Full;            100% illuminated
    if (MoonPhase == TXT_MOON_PHASE6) return MoonWaningGibbous();  // Waning gibbous;  75%  illuminated
    if (MoonPhase == TXT_MOON_PHASE7) return MoonThirdQuarter();   // Third quarter;   50%  illuminated
    if (MoonPhase == TXT_MOON_PHASE8) return MoonWaningCrescent(); // Waning crescent; 25%  illuminated
  }
  else
  {
    if (MoonPhase == TXT_MOON_PHASE1) return MoonNew();            // New;              0%  illuminated
    if (MoonPhase == TXT_MOON_PHASE2) return MoonWaningCrescent(); // Waning crescent; 25%  illuminated
    if (MoonPhase == TXT_MOON_PHASE3) return MoonThirdQuarter();   // Third quarter;   50%  illuminated
    if (MoonPhase == TXT_MOON_PHASE4) return MoonWaningGibbous();  // Waning gibbous;  75%  illuminated
    if (MoonPhase == TXT_MOON_PHASE5) return MoonFull();           // Full;            100% illuminated
    if (MoonPhase == TXT_MOON_PHASE6) return MoonWaxingGibbous();  // Waxing gibbous;  75%  illuminated
    if (MoonPhase == TXT_MOON_PHASE7) return MoonFirstQuarter();   // First quarter;   50%  illuminated
    if (MoonPhase == TXT_MOON_PHASE8) return MoonWaxingCrescent(); // Waxing crescent; 25%  illuminated
  }
  return MoonPhase;
}
//#########################################################################################
String GetMoonPhase() {
  time_t now = time(NULL);
  struct tm * now_utc = gmtime(&now);
  const int day_utc   = now_utc->tm_mday;
  const int month_utc = now_utc->tm_mon + 1;
  const int year_utc  = now_utc->tm_year + 1900;
  return MoonPhase(day_utc, month_utc, year_utc, Hemisphere);
}
//#########################################################################################
String MoonPhase(int d, int m, int y, String hemisphere) {
  int c, e;
  double jd;
  int b;
  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  c   = 365.25 * y;
  e   = 30.6  * m;
  jd  = c + e + d - 694039.09;     /* jd is total days elapsed */
  jd /= 29.53059;                  /* divide by the moon cycle (29.53 days) */
  b   = jd;                        /* int(jd) -> b, take integer part of jd */
  jd -= b;                         /* subtract integer part to leave fractional part of original jd */
  b   = jd * 8 + 0.5;              /* scale fraction from 0-8 and round by adding 0.5 */
  b   = b & 7;                     /* 0 and 8 are the same phase so modulo 8 for 0 */
  //if (hemisphere == "south") b = 7 - b;
  if (b == 0) return TXT_MOON_PHASE1; // New;              0%  illuminated
  if (b == 1) return TXT_MOON_PHASE2; // Waxing crescent; 25%  illuminated
  if (b == 2) return TXT_MOON_PHASE3; // First quarter;   50%  illuminated
  if (b == 3) return TXT_MOON_PHASE4; // Waxing gibbous;  75%  illuminated
  if (b == 4) return TXT_MOON_PHASE5; // Full;            100% illuminated
  if (b == 5) return TXT_MOON_PHASE6; // Waning gibbous;  75%  illuminated
  if (b == 6) return TXT_MOON_PHASE7; // Third quarter;   50%  illuminated
  if (b == 7) return TXT_MOON_PHASE8; // Waning crescent; 25%  illuminated
  return "";
}
String UVIndexLevel(float UVI) {
  if (UVI <= 2)  return "L";
  if (UVI <= 5)  return "M";
  if (UVI <= 7)  return "H";
  if (UVI <= 10) return "VH";
  if (UVI  > 10) return "EX";
  return "";
}
//#########################################################################################
float DewPoint(float Temperature, float RH) {
  float DP = 0;
  if (Units == "I") Temperature = (Temperature - 32) * 5.0 / 9.0; // Convert F to C
  DP = (237.3 * (log(RH / 100) + ( (17.27 * Temperature) / (237.3 + Temperature)))) / (17.27 - (log(RH / 100) + ( (17.27 * Temperature) / (237.3 + Temperature) )));
  //Where: Tdew = dew point temperature in degrees Celsius (°C), T = air temperature in degrees Celsius (°C), RH = relative humidity (%), ln = natural logarithm.
  if (Units == "I") DP = DP * 9.0 / 5.0 + 32; // Convert C to F
  return DP;
}
//#########################################################################################
float calc_windchill(int Temperature, int WindSpeed) {
  float result;
  // Derived from wind_chill = 13.12 + 0.6215 * Tair - 11.37 * POWER(wind_speed,0.16)+0.3965 * Tair * POWER(wind_speed,0.16)
  WindSpeed *= 1.852; // Convert to Kph
  result = 13.12 + 0.6215 * Temperature - 11.37 * pow(WindSpeed, 0.16) + 0.3965 * Temperature * pow(WindSpeed, 0.16);
  if (result < 0 ) {
    return result - 0.5;
  } else {
    return result + 0.5;
  }
}
//#########################################################################################
float calc_heatindex(int T, int RH) {
  float HI = 0;
  // The computation of the heat index is a refinement of a result obtained by multiple regression analysis carried out by Lans P. Rothfusz and
  // described in a 1990 National Weather Service (NWS) Technical Attachment (SR 90-23).
  // The regression equation of Rothfusz is:
  //   HI = -42.379 + 2.04901523*T + 10.14333127*RH - .22475541*T*RH - .00683783*T*T - .05481717*RH*RH + .00122874*T*T*RH + .00085282*T*RH*RH - .00000199*T*T*RH*RH
  // where T is temperature in degrees F and RH is relative humidity in percent.
  // HI is the heat index expressed as an apparent temperature in degrees F.
  //
  // If the RH is less than 13% and the temperature is between 80 and 112 degrees F, then the following adjustment is subtracted from HI:
  //   ADJUSTMENT = [(13-RH)/4]*SQRT{[17-ABS(T-95.)]/17}
  // where ABS and SQRT are the absolute value and square root functions, respectively.
  // On the other hand, if the RH is greater than 85% and the temperature is between 80 and 87 degrees F, then the following adjustment is added to HI:
  //   ADJUSTMENT = [(RH-85)/10] * [(87-T)/5]
  // The Rothfusz regression is not appropriate when conditions of temperature and humidity warrant a heat index value below about 80 degrees F. In those cases, a simpler formula is applied to calculate values consistent with Steadman's results:
  if (Units == "M") T = T * 9.0 / 5.0 + 32;
  if (T >= 80) {
    HI = -42.379 + 2.04901523 * T + 10.14333127 * RH - 0.22475541 * T * RH - 0.00683783 * sq(T) - 0.05481717 * sq(RH) + 0.00122874 * sq(T) * RH + 0.00085282 * T * sq(RH) - 0.00000199 * sq(T) * sq(RH);
    if (RH <= 13 && T >= 80 && T <= 112) {
      HI -= ((13 - RH) / 4) * sqrt((17 - abs(T - 95.)) / 17);
    }
    if (RH >= 85 && T >= 80 && T <= 87) {
      HI += ((RH - 85) / 10) * ((87 - T) / 5);
    }
  }
  else
    HI = 0.5 * (T + 61.0 + ((T - 68.0) * 1.2) + (RH * 0.094));
  if (Units == "M") HI = (HI - 32) / 9.0 * 5.0;
  return HI;
}
//#########################################################################################
float WindChill(int Temperature, int WindSpeed) {
  float result;
  // Derived from wind_chill = 13.12 + 0.6215 * Tair - 11.37 * POWER(wind_speed,0.16)+0.3965 * Tair * POWER(wind_speed,0.16)
  if (Units == "I") WindSpeed = WindSpeed * 1.852; // Convert to Kph
  result = 13.12 + 0.6215 * Temperature - 11.37 * pow(WindSpeed, 0.16) + 0.3965 * Temperature * pow(WindSpeed, 0.16);
  if (result < 0 ) {
    return result - 0.5;
  } else {
    return result + 0.5;
  }
}
//#########################################################################################
int HeatIndex(float T, int RH) {
  if ( Units == "M" ) T = (T * 9 / 5) + 32;
  int tHI = (-42.379 + (2.04901523 * T) + (10.14333127 * RH) - (0.22475541 * T * RH) - (0.00683783 * sq(T)) - (0.05481717 * sq(RH))  + (0.00122874 * sq(T) * RH) + (0.00085282 * T * sq(RH)) - (0.00000199 * sq(T) * sq(RH)));
  if ( Units == "M" ) tHI = (tHI - 32) / 9.0 * 5.0;
  return tHI;
  //where   HI = -42.379 + 2.04901523*T + 10.14333127*RH - 0.22475541*T*RH - 0.00683783*T*T - 0.05481717*RH*RH + 0.00122874*T*T*RH + 0.00085282*T*RH*RH - 0.00000199*T*T*RH*RH
  //tHI = heat index (°F) T = air temperature (°F) (t > 57oF) RH = relative humidity (%)
}
//#########################################################################################
void Convert_Readings_to_Imperial() {
  WxConditions[0].Pressure = hPa_to_inHg(WxConditions[0].Pressure);
  WxForecast[1].Rainfall   = mm_to_inches(WxForecast[1].Rainfall);
  WxForecast[1].Snowfall   = mm_to_inches(WxForecast[1].Snowfall);
}
//#########################################################################################
String TempToClass(float T) {
  if (Units == "I") T = (T - 32) / 9.0 * 5.0; // Back to Celcius for this comparison
  if (T > 27)             return "tempH";  // Red
  if (T >= 16 && T <= 27) return "tempM";  // Green
  if (T < 16)             return "tempL";  // Blue
  return "tempM";
}
//#########################################################################################
String HumiToClass(float H) {
  if (H >  80)            return "humiH"; // Red    > 80%
  if (H >= 40 && H <= 80) return "humiM"; // Green  40-80%
  if (H <  40)            return "humiL"; // Blue   < 40%
  return "humiM";
}
//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
bool DecodeWeather(WiFiClient& json, String Type) {
  Serial.print("\nCreating object...and ");
  // allocate the JsonDocument
  DynamicJsonDocument doc(25 * 1024);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  Serial.println(" Decoding " + Type + " data...................");
  if (Type == "weather") {
    Serial.println("Getting : " + Type + " data");
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    WxConditions[0].Description  = root["weather"][0]["description"].as<char*>(); Serial.println("Des1: " + String(WxConditions[0].Description));
    WxConditions[0].Description2 = root["weather"][1]["description"].as<char*>(); Serial.println("Des2: " + String(WxConditions[0].Description2));
    WxConditions[0].Icon         = root["weather"][0]["icon"].as<char*>();        Serial.println("Icon: " + String(WxConditions[0].Icon));
    WxConditions[0].Temperature  = root["main"]["temp"];                          Serial.println("Temp: " + String(WxConditions[0].Temperature));
    WxConditions[0].Feelslike    = root["main"]["feels_like"];                    Serial.println("FLik: " + String(WxConditions[0].Feelslike));
    WxConditions[0].Pressure     = root["main"]["pressure"];                      Serial.println("Pres: " + String(WxConditions[0].Pressure));
    WxConditions[0].Humidity     = root["main"]["humidity"];                      Serial.println("Humi: " + String(WxConditions[0].Humidity));
    WxConditions[0].Low          = root["main"]["temp_min"];                      Serial.println("TLow: " + String(WxConditions[0].Low));
    WxConditions[0].High         = root["main"]["temp_max"];                      Serial.println("THig: " + String(WxConditions[0].High));
    WxConditions[0].Windspeed    = root["wind"]["speed"];                         Serial.println("WSpd: " + String(WxConditions[0].Windspeed));
    WxConditions[0].Gust         = root["wind"]["gust"];                          Serial.println("WGus: " + String(WxConditions[0].Gust));
    WxConditions[0].Winddir      = root["wind"]["deg"];                           Serial.println("WDir: " + String(WxConditions[0].Winddir));
    WxConditions[0].Cloudcover   = root["clouds"]["all"];                         Serial.println("CCov: " + String(WxConditions[0].Cloudcover)); // in per-unit of % of cloud cover
    WxConditions[0].Visibility   = root["visibility"];                            Serial.println("Visi: " + String(WxConditions[0].Visibility)); // in metres
    WxConditions[0].Sunrise      = root["sys"]["sunrise"];                        Serial.println("SRis: " + String(WxConditions[0].Sunrise));
    WxConditions[0].Sunset       = root["sys"]["sunset"];                         Serial.println("SSet: " + String(WxConditions[0].Sunset));
    WxConditions[0].Rainrate     = doc["rain"]["1h"];                             Serial.println("RRat: " + String(WxConditions[0].Rainrate));
    WxConditions[0].Snowrate     = doc["snow"]["1h"];                             Serial.println("SRat: " + String(WxConditions[0].Snowrate));
  }
  if (Type == "forecast") {
    Serial.println("Getting : " + Type + " data");
    //Serial.println(json);
    Serial.print(F("\nReceiving Forecast period - ")); //------------------------------------------------
    JsonArray list                  = root["list"];
    for (byte r = 0; r < max_readings; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      WxForecast[r].Dt          = list[r]["dt"];                                    Serial.println("DT  : " + String(WxForecast[r].Dt));
      WxForecast[r].Temperature = list[r]["main"]["temp"];                          Serial.println("Temp: " + String(WxForecast[r].Temperature));
      WxForecast[r].Low         = list[r]["main"]["temp_min"];                      Serial.println("TLow: " + String(WxForecast[r].Low));
      WxForecast[r].High        = list[r]["main"]["temp_max"];                      Serial.println("THig: " + String(WxForecast[r].High));
      WxForecast[r].Pressure    = list[r]["main"]["pressure"];                      Serial.println("Pres: " + String(WxForecast[r].Pressure));
      WxForecast[r].Humidity    = list[r]["main"]["humidity"];                      Serial.println("Humi: " + String(WxForecast[r].Humidity));
      WxForecast[r].Description = list[r]["weather"][0]["description"].as<char*>(); Serial.println("Desc: " + String(WxForecast[r].Description));
      WxForecast[r].Icon        = list[r]["weather"][0]["icon"].as<char*>();        Serial.println("Icon: " + String(WxForecast[r].Icon));
      WxForecast[r].Cloudcover  = list[r]["clouds"]["all"];                         Serial.println("CCov: " + String(WxForecast[r].Cloudcover)); // in % of cloud cover
      WxForecast[r].Windspeed   = list[r]["wind"]["speed"];                         Serial.println("WSpd: " + String(WxForecast[r].Windspeed));
      WxForecast[r].Winddir     = list[r]["wind"]["deg"];                           Serial.println("WDir: " + String(WxForecast[r].Winddir));
      WxForecast[r].Rainfall    = list[r]["rain"]["3h"];                            Serial.println("Rain: " + String(WxForecast[r].Rainfall));
      WxForecast[r].Snowfall    = list[r]["snow"]["3h"];                            Serial.println("Snow: " + String(WxForecast[r].Snowfall));
      WxForecast[r].Pop         = list[r]["pop"];                                   Serial.println("Pop : " + String(WxForecast[r].Pop * 100));  // in per-unit of % probability
    }
    //------------------------------------------
    float pressure_trend = WxForecast[1].Pressure - WxForecast[0].Pressure; // Measure pressure slope between ~now and 3-hours later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  WxConditions[0].Trend = "&neArr;";
    if (pressure_trend < 0)  WxConditions[0].Trend = "&seArr;";
    if (pressure_trend == 0) WxConditions[0].Trend = "&rArr;";
    if (Units == "I") Convert_Readings_to_Imperial();
  }
  if (Type == "onecall") {
    Serial.println("Getting : " + Type + " data");
    WxConditions[0].uvIndex = root["value"];                                         Serial.println("UVIn:  " + String(WxConditions[0].uvIndex));
  }
  return true;
}
//#########################################################################################
String ConvertUnixTime(int unix_time) {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  if (Units == "M") {
    strftime(output, sizeof(output), "%H:%M %d/%m/%y", now_tm);
  }
  else {
    strftime(output, sizeof(output), "%I:%M%P %m/%d/%y", now_tm);
  }
  return output;
}
//#########################################################################################
String DayOfWeek(int unix_time) {
  // Returns 'Mon' or 'Tue' or the contents of the array weekday_D[n] ...
  // See http://www.cplusplus.com/reference/ctime/strftime/
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  strftime(output, sizeof(output), "%w", now_tm);
  return String(weekday_D[atoi(output)]);
}
//#########################################################################################
bool obtain_wx_data(WiFiClient& client, const String& RequestType) {
  String units = (Units == "M" ? "metric" : "imperial");
  client.stop(); // close connection before sending a new request
  // api.openweathermap.org/data/2.5/forecast?lat={lat}&lon={lon}&appid={API key}
  // Request type is either 'weather', 'forecast' or 'uvi'
  HTTPClient http;
  String uri = "/data/2.5/" + RequestType + "?lat=" + Latitude + "&lon=" + Longitude + "&APPID=" + apikey + "&mode=json&units=" + units + "&lang=" + Language;
  if (RequestType != "weather") {
    uri += "&cnt=" + String(max_readings);
  }
  if (RequestType == "onccall")     {
    uri += "&exclude=minutely,hourly,daily,alerts";
  }
  Serial.println(uri);
  //http.begin(uri,test_root_ca); //HTTPS example connection
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    if (!DecodeWeather(http.getStream(), RequestType)) return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}
//#########################################################################################
String TitleCase(String text) {
  if (text.length() > 0) {
    String temp_text = text.substring(0, 1);
    temp_text.toUpperCase();
    return temp_text + text.substring(1); // Title-case the string
  }
  else return text;
}
//#########################################################################################
float mm_to_inches(float value_mm) {
  return 0.0393701 * value_mm;
}
//#########################################################################################
float hPa_to_inHg(float value_hPa) {
  return 0.02953 * value_hPa;
}
//#########################################################################################
////////////// SPIFFS Support ////////////////////////////////
// For ESP8266 See: http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html
void StartSPIFFS() {
  Serial.println("Starting SPIFFS");
  boolean SPIFFS_Status;
  SPIFFS_Status = SPIFFS.begin();
  if (SPIFFS_Status == false)
  { // Most likely SPIFFS has not yet been formated, so do so
#ifdef ESP8266
    Serial.println("Formatting SPIFFS Please wait .... ");
    if (SPIFFS.format() == true) Serial.println("SPIFFS formatted successfully");
    if (SPIFFS.begin() == false) Serial.println("SPIFFS failed to start...");
#else
    Serial.println("Formatting SPIFFS...");
    SPIFFS.begin(true); // Now format SPIFFS
    File datafile = SPIFFS.open("/" + DataFile, FILE_READ);
    if (!datafile || !datafile.isDirectory()) {
      Serial.println("SPIFFS failed to start..."); // If ESP32 nothing more can be done, so delete and then create another file
      SPIFFS.remove("/" + DataFile); // The file is corrupted!!
      datafile.close();
    }
#endif
  }
  else Serial.println("SPIFFS Started successfully...");
}
//#########################################################################################
void RecoverSettings() {
  Serial.println("Reading Settings");
  File dataFile = SPIFFS.open("/" + DataFile, FILE_READ);
  if (dataFile) { // if the file is available, read it
    while (dataFile.available()) {
      City                = dataFile.readStringUntil('\n'); City.trim(); // Must trim recovered text
      Language            = dataFile.readStringUntil('\n'); Language.trim();
      Hemisphere          = dataFile.readStringUntil('\n'); Hemisphere.trim();
      WindUnits           = dataFile.readStringUntil('\n'); WindUnits.trim();
      Units               = dataFile.readStringUntil('\n'); Units.trim();
      TXT_FORECAST_GRAPHS = dataFile.readStringUntil('\n'); TXT_FORECAST_GRAPHS.trim();
      TXT_TEMPERATURE     = dataFile.readStringUntil('\n'); TXT_TEMPERATURE.trim();
      TXT_FEELSLIKE       = dataFile.readStringUntil('\n'); TXT_FEELSLIKE.trim();
      TXT_TEMPDEWPOINT    = dataFile.readStringUntil('\n'); TXT_TEMPDEWPOINT.trim();
      TXT_HUMIDITY        = dataFile.readStringUntil('\n'); TXT_HUMIDITY.trim();
      TXT_DEWPOINT        = dataFile.readStringUntil('\n'); TXT_DEWPOINT.trim();
      TXT_HEATINDEX       = dataFile.readStringUntil('\n'); TXT_HEATINDEX.trim();
      TXT_WINDSPEED       = dataFile.readStringUntil('\n'); TXT_WINDSPEED.trim();
      TXT_WINDCHILL       = dataFile.readStringUntil('\n'); TXT_WINDCHILL.trim();
      TXT_WCHILL_HINDEX   = dataFile.readStringUntil('\n'); TXT_WCHILL_HINDEX.trim();
      TXT_PRESSURE        = dataFile.readStringUntil('\n'); TXT_PRESSURE.trim();
      TXT_CLOUDCOVER      = dataFile.readStringUntil('\n'); TXT_CLOUDCOVER.trim();
      TXT_VISIBILITY      = dataFile.readStringUntil('\n'); TXT_VISIBILITY.trim();
      TXT_PRECIPITATION   = dataFile.readStringUntil('\n'); TXT_PRECIPITATION.trim();
      TXT_RAINFALL        = dataFile.readStringUntil('\n'); TXT_RAINFALL.trim();
      TXT_SNOWFALL        = dataFile.readStringUntil('\n'); TXT_SNOWFALL.trim();
      TXT_MOON_PHASE1     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE1.trim();
      TXT_MOON_PHASE2     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE2.trim();
      TXT_MOON_PHASE3     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE3.trim();
      TXT_MOON_PHASE4     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE4.trim();
      TXT_MOON_PHASE5     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE5.trim();
      TXT_MOON_PHASE6     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE6.trim();
      TXT_MOON_PHASE7     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE7.trim();
      TXT_MOON_PHASE8     = dataFile.readStringUntil('\n'); TXT_MOON_PHASE8.trim();
      TXT_HI              = dataFile.readStringUntil('\n'); TXT_HI.trim();
      TXT_LO              = dataFile.readStringUntil('\n'); TXT_LO.trim();
      TXT_RH              = dataFile.readStringUntil('\n'); TXT_RH.trim();
      TXT_UVINDEX         = dataFile.readStringUntil('\n'); TXT_UVINDEX.trim();
      Latitude            = dataFile.readStringUntil('\n'); Latitude.trim();
      Longitude           = dataFile.readStringUntil('\n'); Longitude.trim();
      TXT_SUN             = dataFile.readStringUntil('\n'); TXT_SUN.trim();
      TXT_MON             = dataFile.readStringUntil('\n'); TXT_MON.trim();
      TXT_TUE             = dataFile.readStringUntil('\n'); TXT_TUE.trim();
      TXT_WED             = dataFile.readStringUntil('\n'); TXT_WED.trim();
      TXT_THU             = dataFile.readStringUntil('\n'); TXT_THU.trim();
      TXT_FRI             = dataFile.readStringUntil('\n'); TXT_FRI.trim();
      TXT_SAT             = dataFile.readStringUntil('\n'); TXT_SAT.trim();
      TXT_JAN             = dataFile.readStringUntil('\n'); TXT_JAN.trim();
      TXT_FEB             = dataFile.readStringUntil('\n'); TXT_FEB.trim();
      TXT_MAR             = dataFile.readStringUntil('\n'); TXT_MAR.trim();
      TXT_APR             = dataFile.readStringUntil('\n'); TXT_APR.trim();
      TXT_MAY             = dataFile.readStringUntil('\n'); TXT_MAY.trim();
      TXT_JUN             = dataFile.readStringUntil('\n'); TXT_JUN.trim();
      TXT_JUL             = dataFile.readStringUntil('\n'); TXT_JUL.trim();
      TXT_AUG             = dataFile.readStringUntil('\n'); TXT_AUG.trim();
      TXT_SEP             = dataFile.readStringUntil('\n'); TXT_SEP.trim();
      TXT_OCT             = dataFile.readStringUntil('\n'); TXT_OCT.trim();
      TXT_NOV             = dataFile.readStringUntil('\n'); TXT_NOV.trim();
      TXT_DEC             = dataFile.readStringUntil('\n'); TXT_DEC.trim();
      TXT_N               = dataFile.readStringUntil('\n'); TXT_N.trim();
      TXT_NE              = dataFile.readStringUntil('\n'); TXT_NE.trim();
      TXT_E               = dataFile.readStringUntil('\n'); TXT_E.trim();
      TXT_SE              = dataFile.readStringUntil('\n'); TXT_SE.trim();
      TXT_S               = dataFile.readStringUntil('\n'); TXT_S.trim();
      TXT_SW              = dataFile.readStringUntil('\n'); TXT_SW.trim();
      TXT_W               = dataFile.readStringUntil('\n'); TXT_W.trim();
      TXT_NW              = dataFile.readStringUntil('\n'); TXT_NW.trim();
      TXT_BFS0            = dataFile.readStringUntil('\n'); TXT_BFS0.trim();
      TXT_BFS1            = dataFile.readStringUntil('\n'); TXT_BFS1.trim();
      TXT_BFS2            = dataFile.readStringUntil('\n'); TXT_BFS2.trim();
      TXT_BFS3            = dataFile.readStringUntil('\n'); TXT_BFS3.trim();
      TXT_BFS4            = dataFile.readStringUntil('\n'); TXT_BFS4.trim();
      TXT_BFS5            = dataFile.readStringUntil('\n'); TXT_BFS5.trim();
      TXT_BFS6            = dataFile.readStringUntil('\n'); TXT_BFS6.trim();
      TXT_BFS7            = dataFile.readStringUntil('\n'); TXT_BFS7.trim();
      TXT_BFS8            = dataFile.readStringUntil('\n'); TXT_BFS8.trim();
      TXT_BFS9            = dataFile.readStringUntil('\n'); TXT_BFS9.trim();
      TXT_BFS10           = dataFile.readStringUntil('\n'); TXT_BFS10.trim();
      TXT_BFS11           = dataFile.readStringUntil('\n'); TXT_BFS11.trim();
      TXT_BFS12           = dataFile.readStringUntil('\n'); TXT_BFS12.trim();
      TXT_HOME            = dataFile.readStringUntil('\n'); TXT_HOME.trim();
      TXT_WEATHERMAP      = dataFile.readStringUntil('\n'); TXT_WEATHERMAP.trim();
      TXT_SYSTEMSETUP     = dataFile.readStringUntil('\n'); TXT_SYSTEMSETUP.trim();
      TXT_PREFILLSETTINGS = dataFile.readStringUntil('\n'); TXT_PREFILLSETTINGS.trim();
      TXT_HELP            = dataFile.readStringUntil('\n'); TXT_HELP.trim();
      TXT_LANGUAGE_TABLE  = dataFile.readStringUntil('\n'); TXT_LANGUAGE_TABLE.trim();
    }
    dataFile.close();
    Serial.println("    Recovered City Name of : " + City);
    Serial.println("Recovered NW Definition of : " + TXT_NW);
  }
}
//#########################################################################################
// “r”   read, read only
// “r +” read and write. The pointer is positioned at the start of the file
// “w”   write, write. The existing content is deleted. The file is created if it does not exist
// “w +” opens file for reading and writing. The file is created if it does not exist, otherwise it is truncated. The pointer is positioned at the start of the file
// “a”   append, opens file to add data, file is created if not exists. Pointer is positioned at end of file if it already exists
// “a +” append, opens file to add data, file is created if not exists. Pointer is positioned at start of file for reading and at end for writing (appending)
void SaveSettings() {
  Serial.println("Getting ready to save settings...");
  Serial.println("Have valid entry for City name of : " + City);
  Serial.println("  Have valid definition for NW of : " + TXT_NW);
  if (TXT_SNOWFALL != "") { // Any entry to test
    File dataFile = SPIFFS.open("/" + DataFile, FILE_WRITE);
    if (dataFile) { // Save settings
      Serial.println("Saving settings...");
      dataFile.println(City);
      dataFile.println(Language);
      dataFile.println(Hemisphere);
      dataFile.println(WindUnits);
      dataFile.println(Units);
      dataFile.println(TXT_FORECAST_GRAPHS);
      dataFile.println(TXT_TEMPERATURE);
      dataFile.println(TXT_FEELSLIKE);
      dataFile.println(TXT_TEMPDEWPOINT);
      dataFile.println(TXT_HUMIDITY);
      dataFile.println(TXT_DEWPOINT);
      dataFile.println(TXT_HEATINDEX);
      dataFile.println(TXT_WINDSPEED);
      dataFile.println(TXT_WINDCHILL);
      dataFile.println(TXT_WCHILL_HINDEX);
      dataFile.println(TXT_PRESSURE);
      dataFile.println(TXT_CLOUDCOVER);
      dataFile.println(TXT_VISIBILITY);
      dataFile.println(TXT_PRECIPITATION);
      dataFile.println(TXT_RAINFALL);
      dataFile.println(TXT_SNOWFALL);
      dataFile.println(TXT_MOON_PHASE1);
      dataFile.println(TXT_MOON_PHASE2);
      dataFile.println(TXT_MOON_PHASE3);
      dataFile.println(TXT_MOON_PHASE4);
      dataFile.println(TXT_MOON_PHASE5);
      dataFile.println(TXT_MOON_PHASE6);
      dataFile.println(TXT_MOON_PHASE7);
      dataFile.println(TXT_MOON_PHASE8);
      dataFile.println(TXT_HI);
      dataFile.println(TXT_LO);
      dataFile.println(TXT_RH);
      dataFile.println(TXT_UVINDEX);
      dataFile.println(Latitude);
      dataFile.println(Longitude);
      dataFile.println(TXT_SUN);
      dataFile.println(TXT_MON);
      dataFile.println(TXT_TUE);
      dataFile.println(TXT_WED);
      dataFile.println(TXT_THU);
      dataFile.println(TXT_FRI);
      dataFile.println(TXT_SAT);
      dataFile.println(TXT_JAN);
      dataFile.println(TXT_FEB);
      dataFile.println(TXT_MAR);
      dataFile.println(TXT_APR);
      dataFile.println(TXT_MAY);
      dataFile.println(TXT_JUN);
      dataFile.println(TXT_JUL);
      dataFile.println(TXT_AUG);
      dataFile.println(TXT_SEP);
      dataFile.println(TXT_OCT);
      dataFile.println(TXT_NOV);
      dataFile.println(TXT_DEC);
      dataFile.println(TXT_N);
      dataFile.println(TXT_NE);
      dataFile.println(TXT_E);
      dataFile.println(TXT_SE);
      dataFile.println(TXT_S);
      dataFile.println(TXT_SW);
      dataFile.println(TXT_W);
      dataFile.println(TXT_NW);
      dataFile.println(TXT_BFS0);
      dataFile.println(TXT_BFS1);
      dataFile.println(TXT_BFS2);
      dataFile.println(TXT_BFS3);
      dataFile.println(TXT_BFS4);
      dataFile.println(TXT_BFS5);
      dataFile.println(TXT_BFS6);
      dataFile.println(TXT_BFS7);
      dataFile.println(TXT_BFS8);
      dataFile.println(TXT_BFS9);
      dataFile.println(TXT_BFS10);
      dataFile.println(TXT_BFS11);
      dataFile.println(TXT_BFS12);
      dataFile.println(TXT_HOME);
      dataFile.println(TXT_WEATHERMAP);
      dataFile.println(TXT_SYSTEMSETUP);
      dataFile.println(TXT_PREFILLSETTINGS);
      dataFile.println(TXT_HELP);
      dataFile.println(TXT_LANGUAGE_TABLE);
      dataFile.close();
    }
    Serial.println("Settings saved");
  }
}
//#########################################################################################
void SunrisesetIcon() {
  webpage += "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACMAAAAjCAYAAAAe2bNZAAAD/ElEQVRYR+2Wf0xVZRjHP8+5h4sXK0xnEPMPbDRXjICQ";
  webpage += "+sOFCLb+0Va2/KPm0rbmBhkBc/1ebG3mH60cayPbmm2UOWvmctammNfaXHOp0MRkAi7HvfQDUPB6Ubj3PO2cCxcu3B8EtbnF+9fZed/3eT7n+3yf9z3CLTTkFmJh";
  webpage += "HiZRNeaV+X8oo71ryxn2PcNQx2kpsnbPpTvn7Bntrawn6HuPqxcOSwnr/nMY7alcgoRfQMJHJOfHM5MTqr/iZYb9H3D1wgEp4amYuVNkY1IFfEIxl0XQZLAzUkb9";
  webpage += "5R+BbgWOS86JipiEE8ockhIej5k7zS6EGuAkg6yWNYTmDtO7Jh+1TgEZIJskx/vZeFD9s/I5rvvfYKjjqBRa1dH3Z1jpQEAawpNSzMFUJZyRMnYQ9ZU3IPo2cAkj";
  webpage += "vUCyj1xPFFwVoZVvUNYhfC3FbEgFYs/PHGZgbSajoztQaZZs709RBQ48tIRrgYX8ERwhmNsnDV6nFHqWexFexGK3PMj5WcGonXQ4tAmP2SyLWwbjBdH9+dlYZi3C";
  webpage += "YxiSy9AND32BEIbRi+HyYhq7pKbrXNy9xzFZxGYsvFJC5+Q105RR/+odwGsInVhUkXPX9yJfhqNKfF6wGdN4F8h23hkCQzfgr0DkOTJGEHcj6fKmvNR5c4qPmsDx";
  webpage += "03d0sl42Eo0dB6ZsA0gDUOAEcbFVsk587Ej/ReFODF6J+eL4MDhN7DKO4bntCaluD2gbjxDmh7G9PpSddNGUFMZJ2p7vZtHSOkS3YEmlLPP26N7CbbhonCZ9Ihgn";
  webpage += "kN1L5ldSd+lpPYmHDL4lTBsh3pGH6Z8aK6mB1b8+Q3IOBbU5Pw+32RZp7SkjGYwDpLDA86zUXNyr7bgln5FEZp5RN+m+wkaEbXGDpIRxytVKYHnpeKfNGkb35C7A";
  webpage += "k9mOcM+sYCKblLT0lVLbGXOVJCyT/l62HKXUWWAhmEa/ZHlbtPmBFbjFblNzDjAgac/L9u49eo5V3GTZmKPsBrkoRZy1Y0fLpL1l1ah8GE3oki7J8ubpvqJSRO2r";
  webpage += "IP5IVabxXZZRL6/+9r7+wjFCTNxvyqdSwpZYmP7KR4FakMjNatIhmS11ur+gADVs88b310xhXGlVUt/dpB28TphVY4yCcFDuwzk6Uhq4vzHvjsV3L+xAJXLIJeym";
  webpage += "a5MPvdhVhmhf0KpY+laPN6HCNoxvO0dR7tRRhyzmf0NdiBiomeG5X0Smt7UtoAG+Kxbn/WFcEyfw1Jw6GBz5NRRiRKPn7cQSw0RMgys2jH1cu5MRq5V41obpHoCf";
  webpage += "fRGwZNaS5HUI2TD2PXM7mvwvLFESlwmXB5BWPy7TmF0MsY0qBFJ6Jpli//bcPEwiReeVmVfmn3bb30bWeTWF4WXMAAAAAElFTkSuQmCC' alt=''>";
}
//#########################################################################################
String RainDrop() {
  String webpage = "";
  webpage += "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACMAAAAjCAYAAAAe2bNZAAAC10lEQVRYR+2WXUiTURjH/+e8r7NmfqwQYaFROcP8mJJ1";
  webpage += "VXQbUZaBFxGVaX4VMa2IvFtXCZKZKbOBKBTRB6Q3fdBNUiAE1mxos2YFktVFpRBuzd69J860wdjW+07nMti5PM85z/93/ufjOQTLqJFlxII4TLjdiDsTHWfOT2+A";
  webpage += "l6WgRTe8FAc/sm2qcl5HgpSFrtyd/xam1rYPK0i/7/65iAlWY3u0gdQ5UzqWjPWuUUDI9AEQzOCNzoiHWe+iCaQMUzOUALbyApKkJrB5aT5LEm6iI/9QWBgzo76Y";
  webpage += "mchqgZVh6u3l0LBOAOkBSQmTIZFsdBg/hBSrnTwLCgLL2pbowZiGLQCpC5mQsQNoL+oLih105mHNjM3XP6k1oi/HoQZI2ZlTY5UQZi+BsbRAZwC4tXm4ZngdJNT4";
  webpage += "agAy5m6cID9Fa7Gq26cMwxPundgPw/deeEnqnACbxWRyA25vtASB1I1XQDPT4696XOGL9jhuGbqV3FEHw7NUja5GincHZCbCnTAI6+bPQcnNT0RMpznAaHZAjMpv";
  webpage += "waRatG0d+BuQehilZfF44/A2yOR5yKEU7bhsNMUO5qh9C9LkIf4QBTYGSLQVnYVnYgfDlUyOeyCeMrB5IP7M0KQ+aDLOoTl1fGlg+KPm+aHDxZRvAQL8kaSaSiR6";
  webpage += "dwMUcJMH0Bd0w0wkpZ1e+Jkp/7QLmVMdeK8vQb9uWklITXxhMPV2HQRmg8jWwUVvwFp4WI2Y0piFwVTbrVglV0MmgMiACe0e3M25rySmFI8c5rSzGF73S/ypmjyD";
  webpage += "h44gI98YSVEMBRY5zMkXPRDFiqDSgF9laCvpV1p99G5T+R0B+k28Ss/9awIa68WVomOxg+FKJ2zPoKHb/X8b3kcY8DOxCV25zbGFafhYBPnrIxCS4RcmwmN480px";
  webpage += "lXhiC8PVauwF0JAjEOV0uOkIKOuGpXBqMSA+gxebIJrz4zDh3Iw781848xtx5cUkp4hSdwAAAABJRU5ErkJggg==' alt=''>";
  return webpage;
}
//#########################################################################################
void CloudCover() {
  webpage += "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACMAAAAZCAYAAAC7OJeSAAADBklEQVRIS82WT2gUdxTHP292Znezf4wm0kpLEUqhiEkg";
  webpage += "2cUiKaX0Uv8usbqb+BcPKqhRtInQloK2PfTWoqcWCl6kpd1Ni/TPqfTYQzHYIioICkLrySSuGhN3d36vjNHoZv9kkjWQd5ph3vu+z3vz3m9GWEQmi4iFxmHe3hsm";
  webpage += "P9mNaDuiMYSruKULXPzx5lwLbQwmmX4dI8cRNgEvwaPi8qAXEessF1adg1PGL9T8YZK9fah+DSypk+wPSrqHf7L/+QHyB/PG9hcxGsSMjzD88wOSmQ0ov/pJAFyh";
  webpage += "YL/JpW/HZvOvD5Po3QfmEMhrgA2MAr8D7wCvTIurgjyR0sdvqyz1Vwz/cHCeMKcsElfOAnvqCazbfFgdJ3TPdsIFgQnPVxWnWJyI/fLTl7FnYosEpI2/vr9WT696";
  webpage += "ZxKZj4HPagWu29xPIGCPN0WWGBGJz/RT1bxxS3cnJ++v+O38GccjRDjJcPbTucEkti8H16tgWa3AjT3H7keizc9WXtXVlEo3HhYetDpOuGgFAsWhwQ5v42paZWeS";
  webpage += "ma0ouVoR61P9pVi81QVCs81AteeqWsgNtFWNrYTpSh9AxFvZSlOlp/fDUccJtcwHpCxGuJ49vtpbjGl7CrNmSytucAfoXqCrIpkq7246qPHmFyZEJNIwDJB9f3VZ";
  webpage += "M6ZuOvuSWJoDXVktyfpUvxsKx/K2HRQRqTlLcwVUdW/lBjpefhIndKfiTDb9DfpqNbGNPcfGI9FmC2iaa7LZ/I0xE0OD7dNdFpLpbahkq3fkSCEWb/FOsXkN62ww";
  webpage += "3nPvGMgNtC31roVE5jRwtNqMbOn76LZtB5f7EW3Ex6gpDg20B4VE7+egH1SIGZdtuz7Ji0hzI4n8xrrG3BY602ux5M8aMHdFpN5X2W8uX35T29SV+Q6hryzCGLbu";
  webpage += "PDliWVarL6Xn4DQF07E7iv3wm5lAqfSJO6Fg5NFwLbSpqlt+Andm3sKiG4gjaOq9wf2hcDQKPJdDrl5BivnX189V+ovL3novmBljxoYG21t8wSwYxQzhRQXzPxvQ";
  webpage += "5G1i5eTBAAAAAElFTkSuQmCC' alt=''>";
}
//#########################################################################################
String MoonNew() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MC";
  webpage += "gsOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQE";
  webpage += "BD/wAARCAAjACMDASIAAhEBAxEB/8QAGwAAAgIDAQAAAAAAAAAAAAAAAAgGCQECBwT/xAAsEAABAwQCAQQABQUAAAAAAAABAgMEBQYHEQASIQgTIjEJFCNRYTI0Q";
  webpage += "XGB/8QAFQEBAQAAAAAAAAAAAAAAAAAAAAH/xAAZEQEBAQEBAQAAAAAAAAAAAAAAAREhMVH/2gAMAwEAAhEDEQA/AK68N4breX63MaZqUah27Q2BOuG4ZyVGJSonb";
  webpage += "qFKCfLjqz8W2U/JxXgaAUpPVJOaPTniFblDwzgyjXo+ztp257+aFRdlHf8AW1C/tWEgkgDoskBO1nmM1yl4h9O+McK0J4svXlS27/ul1pWjLdmb/Isq/wA9Woob0";
  webpage += "AddnXDrzxa+T0MLTfV0wpbse7PTrhmtwHzpbKbJp8BaEn7CHYbTLqT+x77/AJ56a3iPEecLfmXb6aI1SotzU5pyXUcfzpKpinY7aey3KZIUPcdUlKSsx3OzhHcoW";
  webpage += "vr14uPJFj++K9ji8aVeltVF6DUKVJbkMvMnSklKgf8AR+t+djfGfBHeHLCr0/DpunP9xP5txTOp1Itq+GY1dYgeypYiyJDCHJbaT2HxElT4SAAAnQAGuHGmF49X1";
  webpage += "MWo4lu+O8X4FdxhbSWVjylDkSA1DeQD+4djObH774vnGOxHXLczhiRj00XdUWafctFmvTrAqMp5DLDqpCgqRSlrVpKFOOfqsqWdFxbjZUn3E84vfGOL1x1XJlvXf";
  webpage += "b02nTYLpafQ8ypJQr70oEbSdEHR14O+JzgjPNmmnHnEMtJKluKCUpH2Sfoc2jxpEt0MxY7jzijoIbSVE/8ABxgcK4Sp1uwGs551ZepVg0l4lKO3SRWJKNlMGGNgu";
  webpage += "PrI6kp+LSe61qT1AK3BbJ6Wsw44wv6erDx3et2QYtYg0huW+y+8lK0IlkymgQTsfpvo1/GuHKQ8tZKreYMkV/JNwobbmV2WX/YbA9uMykBDMdGgPg00httPjekDf";
  webpage += "nhxJgiXLB/w6r1uTPd3P4qzVMYva3KRCb/INVuGzLkxQpStpblrQZKEaAASHOoAAAGuHDih4czemDAGG8a1288f4to0WrxYzj7L05K6mhCwkkENTFOt+CPrrrlIm";
  webpage += "Scs5Hy/W03Dkm759dmNo9pgPqCWYzegPbYZQA0yjwPi2lKd+db4cOJMES4cOHKP/9k=' alt=''>";
  return webpage;
}

//#########################################################################################
String MoonWaningCrescent() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MC";
  webpage += "gsOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQE";
  webpage += "BD/wAARCAAjACMDASIAAhEBAxEB/8QAGQABAAMBAQAAAAAAAAAAAAAAAAUHCAYJ/8QALRAAAQIFAgMHBQEAAAAAAAAAAQIDAAQFBhEHIQgSMRMUFSIyQWFCUXGBk";
  webpage += "aH/xAAYAQEAAwEAAAAAAAAAAAAAAAAAAQMEBf/EAB8RAAICAgEFAAAAAAAAAAAAAAABAhEDMRIEEyJRof/aAAwDAQACEQMRAD8A8z9PNO7s1TuySsuzKb3yozhJ8";
  webpage += "yghphtO63XVnZDaRuVH/SQDdmrvD3propZSlT1y1G57qOEuqZCZWnsLPs2g5edAOfMrkzj0iO04TKG9aFhTt4SjPJUa7zN94x52mAoobCT7ZUlavyEn6RFecSzs4";
  webpage += "wyxLTi3Cpx3YrO6vc/veMDzTyZ+EXSX0u4qMLezPp6whCN5SIQhAG39HphiR0wsqvtgzUjOU56WeaScdm+y842tB+yvQsfCgYqTipqLtVmZGZdWSW8IG+RjGf7k7";
  webpage += "xM8GWvFl2PUprTHV11TNo1x8TUrUOQL8LqATyhas7hpwBKVkenCVdOaOk4ydOPCJRyrSNRRUJF17vkrMNpSEutODmBSU5SoAHqDgjEcqK7XVcZren7ND8sdox9CE";
  webpage += "I6pnEIQgBE/I37eVNoy7blLknhSXM5kVudowM9SltWUpPyADCEQ0nsXRAE7mEIRIEIQgD//2Q==' alt=''>";
  return webpage;
}
//#########################################################################################
String MoonThirdQuarter() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MC";
  webpage += "gsOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQE";
  webpage += "BD/wAARCAAjACMDASIAAhEBAxEB/8QAGgABAAIDAQAAAAAAAAAAAAAAAAcIBAYJBf/EACwQAAEDBAAFAwQCAwAAAAAAAAECAwQABQYRBwgSEyExQVEyYXGBIjNic";
  webpage += "sH/xAAZAQACAwEAAAAAAAAAAAAAAAAABQEDBAL/xAAgEQACAQQCAwEAAAAAAAAAAAAAAQIDBBEhEjEiMjPB/9oADAMBAAIRAxEAPwDl3j+P3rK73BxvHLa/cLncn";
  webpage += "0x4sZlO1uuKOgB/0nwBsnQq7OD8jPDLFbN3uLWRTsgyZxrqFstMtuJb4yiN9Kn1AuSFD5QEJ3vRUPNRhyb42mJKufEIx0Oyo3VEiFY2EJCQp5Q+5CkJ/BUPerGss";
  webpage += "3LrfvNxckONOqLrjy1fPud73+P1Sq8uZ8+EHhI0UoLGWUW424dbMJzZ+02eEIkQJ2hnvKdKfsVK87rQKlvmMilOYOTQ28lDrignupIVr779aiSmFFuVNNlMtMUpS";
  webpage += "rTktvy0qSeEaLzCc7jlov70aYwk+UpcbbcaWf8AFXS4nfyip9dyJq8WE255LrUJClupYS59Hp/FXroEeATVL+V7jfB4L56XcphvT8QvzaYV+itJSpwNdW0PthXgu";
  webpage += "Nkkge4Kh42COg1yxjC8khN5jguYRrri92jJSw8gJDaAggdJWPKVEE7SdFKvUDQpFexdOrmS0+n+Gul5R0UK5pFsOXmCqL19hQUtvrUSrR+agqp55tBZYeXRLPZAe";
  webpage += "1GQonqV1K3v3PvUDU2tvlEzz9mKUpV5wK2HE+IWcYG867h2V3O0d/8AuRFkKQ27/uj6VfsGlKhpS0wTx0YWRZNfcsuS7xkVycmzHPqdWACf0ABXl0pQkksIBSlKk";
  webpage += "D//2Q==' alt=''>";
  return webpage;
}
//#########################################################################################
String MoonWaningGibbous() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MC";
  webpage += "gsOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQE";
  webpage += "BD/wAARCAAjACMDASIAAhEBAxEB/8QAGwABAAIDAQEAAAAAAAAAAAAAAAgJAwUHAgT/xAAuEAABAwQABAQEBwAAAAAAAAABAgMEAAUGEQcSITEIMkFRExRhgRYjQ";
  webpage += "lJxcrH/xAAaAQACAwEBAAAAAAAAAAAAAAAABAEDBQIG/8QAIhEAAgICAQMFAAAAAAAAAAAAAAECEQMEIQUxURIiQXHB/9oADAMBAAIRAxEAPwCtbhhwzyji5mMLC";
  webpage += "cSYaMuUSpx99fIxGZHnddVo6SnY7AkkgJBUQDPTh/4SfDZhUM2i+W1ecZKE6cn3ac5GhocA6hmKz2HsXnFH6J7VyPwnWNeLYHIymKEt3C8FTinDrnDCVKbQlPr3D";
  webpage += "ivuD6Cu5WG1TLEpF0uEd4RFL82zs83flGjzHqO/uKxt3YySk4RlSXj5Y1igkraI++JXgNiFq+O/hdrt8CZFJKmYGw2UjzJKSo9R7iolqBSSlQ0R0IqwjithV3kY7";
  webpage += "ccjRaZjCA+oISpOwN76AjfN01UA7xGXEukmO4nRS6rp96b0cjnCm7oqzRp2fHSlKeKibPBOQ0jhJhmWWwCUiP8AMWyex+lqQ08ohCtfvacQoe+zXY417Tc4TDN0Y";
  webpage += "W4xDHOlkrUEoO/Ir22E9Dr0FRB8JPiHtPBfJ5Nhz+FKuGCZCpCbmzHIL0N5PREpoHYJTvSk91J16pAqwK2YtgmUvDLsZy1q8YxePhvJeQ4FsDXQtBSOqDodlcpSe";
  webpage += "4Brz+7F4cj9a4fZ/n2OYveuDHkMttjAVyYMRPyZYU81sqPICk7CiTof72qqPP3mZGZXd6O18NtUlXKj0T9KtS8V/EzAOGvBT8P48hiL+UptpsHa1uK339+tVJS5L";
  webpage += "kyU7LdO1vLK1H6k7pnpUGoufk42HykYqUpWuLCt1jOb5jhchUvEMqu1ldX51QJjjHP/AG5SOb70pUNJqmHY9ZTm+X5tJTOy7Jbjd309ly5CnCP42a0dKUJJKkApS";
  webpage += "lSB/9k='>";
  return webpage;
}
//#########################################################################################
String MoonFull() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCg";
  webpage += "sOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBD";
  webpage += "/wAARCAAjACMDASIAAhEBAxEB/8QAGgABAAIDAQAAAAAAAAAAAAAAAAcIBQYJBP/EACsQAAECBQMDBAIDAQAAAAAAAAECAwAEBQYRBxIhCDFRIkFhgRNxMmJz0f/E";
  webpage += "ABoBAAIDAQEAAAAAAAAAAAAAAAEEAAUGAgP/xAAgEQACAwABBAMAAAAAAAAAAAABAgADEUEEEiExE1Fx/9oADAMBAAIRAxEAPwDnFpBpLcmst3M2rbym2Egflm5x4";
  webpage += "EtyrI7qIHKj4SOSfABIvjpl0ydOVryopSrYkbprIwlyoVx9bqVqzghDKMMoGfY7j29RiJ+me31WxpkZ+VKUTVd9cyoEb1IXwgD3xtxz/Y+YmygU2atxTTtSknS2/l";
  webpage += "ttGSFuEngAdsE8En5ik63qHdiqtgH1zGqkAGkTML0K6c9SHzZb+mVstzeVJbn6MwJJaUj/ADUCrGDyc/Iin3VJ0hPaJzExX7Krya9bjagXkKUkzcilRwlSwnhbeTt";
  webpage += "3jscA98mz9Ste6aJTGbpp9Pn5d5TziUejJHnG0HI5POe0ZyYeqdwStKt6+7daqkxU5FdPbmeEqXLOjltY5DnBPf1DPfwvTfbQ4YNo5E9HRXGZ5nLCEZ/UC1nrIveu";
  webpage += "Wk/ndSZ96WBJ5KEqO0/acGEaIEMNERPiXC0in2pOw7Tr8g2J6Vm6YJd1BPpS+3lpxs47EFIUPggxMMtXJapGUmq20uZbktiUpK1Yx7o+FDOcf9ipvSD1CW1plVn7H";
  webpage += "1Pbeds+ruh9Ew2ApVOm8AB3BBy2oABYHI7+YvlbVhWhXagJxi526zb9TeE2y6HUpDqFjOxC0EgkH3z7Yx3EZ/rB8LnvH4Y7Ue4eJJllhTNNZKKWiZlEMkBB3FKmlg";
  webpage += "DYST3O7AA55j0PaY0+rV2fQ/WWZWktKM8005yJVKUgqaSrunPxj2iZmrftqRseQp1vyjTEvLspQxwcbgM5V4P3FD+uPq5tXTq0Z/SfTuuylQu6ohTM47JKC0U5B4U";
  webpage += "VrBIDmP4oGSCdysYGaqmuy+0LXzGGYKutOeWvlckbk1nvGsUp38km7VXm2HB2Whs/jCvvZn7hGgEkkknJMI2iKEUKOJVk6diNpsvVPUbTp4PWRelWoxzuKJaZUlsn";
  webpage += "yUH0k/sQhBZQwwiQEj1Nqufqk6iLzpy6VcesNzTMm4MLYROFlCx4UG9uR+4ixSlKUVKJJJySe5MIQFRUGKMkJJ9xCEI6gn//2Q=='>";
  return webpage;
}
//#########################################################################################
String MoonWaxingGibbous() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCg";
  webpage += "sOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBD";
  webpage += "/wAARCAAjACMDASIAAhEBAxEB/8QAGgABAAIDAQAAAAAAAAAAAAAAAAgJBAYHBf/EAC0QAAEDAwIFAQkBAQAAAAAAAAECAwUABBEGIQcIEhMxURQVI0FCUmGBkRYy";
  webpage += "/8QAGQEAAgMBAAAAAAAAAAAAAAAAAQQCBQYD/8QAHxEAAgICAwADAAAAAAAAAAAAAQIAAxESBCExE1Hx/9oADAMBAAIRAxEAPwCqquq8BuDp4ozuL4H3ewodxId7Z";
  webpage += "X65V5A/NcqqYXLPDIjtMkO2z6nrtlRaS0MqWoj09MHJ338UtyrDXWSvsnWuzdzrsXyM8v8AxEjUxcMxNQEw0j4t1Z36rhkK+4peCgfkekFOx2NRE5huV/XfL3JoXK";
  webpage += "vMTGnrt5TNnM2Y+GpYye06jJLLuBnpJII3SVAHEy1/7LQHssjDqvGFybIbwpsoCklW2U/oefzXu6mio7iizKaC1dphxq/1HHJt7v2cYYS63u3cJR9LiVBKuoZGx2G";
  webpage += "TVRTy7qX2ZtljTVqwwBgyrOlZcxFXcFL30JII6LqPuXbV9P2uNqKVD+g0rQA57iUxKm/yx38VdR8S5Id1ZKUsIKXCnBIABGPI2wahBUsuSjUekr6dt9NaxeCmWXAp";
  webpage += "DXcKCrfYjHn0xSfOXNRP1OtJw0sr07bW95at203Bi9tnA3buJyfGSR0qPz2zttgGvNt+FzVw9ey0xqO1bt4XvPN3ah0udnJwjrz9Pp6V2u9tYSXgLNcGxbstoZBRg";
  webpage += "fDQ2Bg77Y2871Xjz684ekIvTNzwU4R6htZaWu0qtpqSj1hdvZtHIcZQ6NnHVf8AJKMpQnIz1HAy/Hqsvt+NPyPuyquxlfvE6etdUcR9U6jsCk2snM3l2wQNi2t5Sk";
  webpage += "n+EUrWaVs1GoAErCcxWVHScjD3jUjFXz9pdMnqbeZWULSfwRSlGCbZP8buMGqYn3FqDibqW+jSnoVZuSTvYUPRTYPSr9itJpSoqoXpRiEkn2KUpUoJ/9k=' alt=''>";
  return webpage;
}
//#########################################################################################
String MoonFirstQuarter() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCg";
  webpage += "sOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBD";
  webpage += "/wAARCAAjACMDASIAAhEBAxEB/8QAGwABAAIDAQEAAAAAAAAAAAAAAAYHAgQJCAH/xAArEAABAgUDAwMEAwAAAAAAAAABAgMABAUGEQcIIRITMSJRgRQyQaEVYYL/";
  webpage += "xAAZAQACAwEAAAAAAAAAAAAAAAAABQEEBgL/xAAeEQADAQEAAgMBAAAAAAAAAAAAAQIDEQRBEiExkf/aAAwDAQACEQMRAD8A5VQAJOB+YRk1juo6hx1DMAHs7blt8";
  webpage += "0h1Lt6Ut+8LZW/VplOUzsnOOtPI4/OFFPHPlJEQPdDs2q+hXfua1K6m47UbcSh9wlInKcVHCe+hPBQVHpDqQB1cEJJT1Wdo9QbkplpSd00aWnG3mlYbU2kk/bzjGc";
  webpage += "8H9xeXenbjRTLfv62G6i9Wqe5TFvJwkPyboBUhxPIWcZwo+pJwc8DCKvJ1y2+Srs+0W1nNTxr7OVcIkGoFpzViXxXrNmiS5Rqg/J9R8qShZCVfIwfmEPE1S6ip+Ef";
  webpage += "jJtQS4lRGQFAmMY+pICgSMgHmJA6tbOnH1WTIzLFPRMS4a+1QJSoKABQTnBJzjA94v9zTah1Ssz709WkydKbUZ1tpQymWISMsoUeQCfb2EVNscrNHn9HpGXQWGEsM";
  webpage += "hKnFEJS2RyVKJOMf2YrLfDvWsqg2lO6O6NXKxXK7P9bFUq1PWlUpINkdK0Nup4deIyn0elAKuSrgZPTDXbynGa9/wYzczn2jwRr3c9PvLWi87kpKwuRnazMmWWPDj";
  webpage += "SVlCF/KUg/MIgUI1USolSvQvb6+iEIR0Qb389XBTzSRWZ76E+Zb6hfaP+M4/UaMIQc4AhCEAH//2Q==' alt=''>";
  return webpage;
}
//#########################################################################################
String MoonWaxingCrescent() {
  String webpage = "<img src='data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCg";
  webpage += "sOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/2wBDAQMDAwQDBAgEBAgQCwkLEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBD";
  webpage += "/wAARCAAjACMDASIAAhEBAxEB/8QAGgABAAIDAQAAAAAAAAAAAAAAAAcJBQYIAf/EAC8QAAEDAwMCAwYHAAAAAAAAAAECAwQABQYHCBESMRMhIhQyUWFxgRUkM0JS";
  webpage += "cvD/xAAYAQADAQEAAAAAAAAAAAAAAAAAAQQFAv/EAB4RAAMBAAICAwAAAAAAAAAAAAABAgMRMQQFIUFR/9oADAMBAAIRAxEAPwCqqlKUATJt5wTT3U+7OYfl8GazL";
  webpage += "eI9nmwZJbcTz8UqCkHj+tZrcXtDzHQps5JAuDeR4mt1Lf4iygIehqV7iJLQJ6OeyVglJPlyD6ajrRy6z7Lmce4W1xbb7Y9JT3qwq1XGPllst9jznGFyJF5ty7U+GP";
  webpage += "JqVGc4/UR+5YHmFDzBAPHkDWZ5O2vjbKpfM/aKM5nSeH2VeUrZNSMNk6fZ7f8ACpfUV2ee7FCld1ICvQo/VPSfvStKWqSaJ2uPg1ulKUwJU22NJd1OghyP47fZbf8";
  webpage += "AIEgcferYsO0pbvp8By7RmIEX83GU4OVxkBIKmkr7gn/d6rb2Q4XLyfURT7ERTvg9ISenkA117u73Y4fovp8/pbgN/hXPPJrZZf8AYVJdRaQRwVvOAlIdA91oEkE9";
  webpage += "SuOADg+xm9t5yy7LMGph1RwHujv1syPcDnFztEhL8QXRUVp1PZzwEJZKh9S2Tz86VFq1rcUVrWpSlHkknkk/GlbmcLOFH4iRvl8nlKUroRm7Xm+YWO2u2myZNcrdD";
  webpage += "f5DrUSSplLgPcK6COofI1hCSTyTyTSlJJLoBSlKYH//2Q==' alt=''>";
  return webpage;
}
//#########################################################################################
/*
  Version 3. (24-Feb-2021)
   1. (2068 lines of code)
*/
