//#########################################################################################
void PreFill_EN() {
  City                = "Bath";
  Latitude            = "51.3704";
  Longitude           = "-2.1376";
  Language            = "en";
  Hemisphere          = "north";
  Units               = "M";
  TXT_FORECAST_GRAPHS = "5 day / 3 hour forecast";
  TXT_TEMPDEWPOINT    = "Temperature / Dewpoint";
  TXT_TEMPERATURE     = "Temperature";
  TXT_FEELSLIKE       = "Feelslike";
  TXT_HUMIDITY        = "Humidity";
  TXT_DEWPOINT        = "Dewpoint";
  TXT_WINDCHILL       = "Windchill";
  TXT_WCHILL_HINDEX   = "Windchill / Heat Index";
  TXT_HEATINDEX       = "Heat Index";
  TXT_WINDSPEED       = "Windspeed";
  TXT_PRESSURE        = "Pressure";
  TXT_CLOUDCOVER      = "Cloud Cover";
  TXT_VISIBILITY      = "Visibility";
  TXT_PRECIPITATION   = "Precipitation";
  TXT_RAINFALL        = "Rainfall";
  TXT_RAINRATE        = "Rainrate";
  TXT_SNOWFALL        = "Snowfall";
  TXT_SNOWRATE        = "Snowrate";
  TXT_UVINDEX         = "UV Index";
  //Moon phases
  TXT_MOON_PHASE1     = "New";
  TXT_MOON_PHASE2     = "Waxing Crescent";
  TXT_MOON_PHASE3     = "First Quarter";
  TXT_MOON_PHASE4     = "Waxing Gibbous";
  TXT_MOON_PHASE5     = "Full";
  TXT_MOON_PHASE6     = "Waning Gibbous";
  TXT_MOON_PHASE7     = "Third Quarter";
  TXT_MOON_PHASE8     = "Waning Crescent";
  //Wind
  TXT_N               = "N";
  TXT_NE              = "NE";
  TXT_E               = "E";
  TXT_SE              = "SE";
  TXT_S               = "S";
  TXT_SW              = "SW";
  TXT_W               = "W";
  TXT_NW              = "NW";
  WindspeedUnits      = MPH;    // Change to your desired display format e.g. for KPH 'MetricWindspeed = KPH';
  TXT_WINDUNITS       = "mph"; // Ensure text here matahces units chosen above! Use a leading space
  //Windspeed warning colours based on Beaufort scale in kph *** NOTE these are HTML colours, don't translate!
  TXT_BFS0            = "Calm";
  TXT_BFS1            = "Light Air";
  TXT_BFS2            = "Light Breeze";
  TXT_BFS3            = "Gentle Breeze";
  TXT_BFS4            = "Moderate Breeze";
  TXT_BFS5            = "Fresh Breeze";
  TXT_BFS6            = "Strong Breeze";
  TXT_BFS7            = "High Wind";
  TXT_BFS8            = "Gale";
  TXT_BFS9            = "Severe Gale";
  TXT_BFS10           = "Storm";
  TXT_BFS11           = "Violent Storm";
  TXT_BFS12           = "Hurricane";
  //Days of the week
  TXT_SUN             = "Sun";
  TXT_MON             = "Mon";
  TXT_TUE             = "Tue";
  TXT_WED             = "Wed";
  TXT_THU             = "Thu";
  TXT_FRI             = "Fri";
  TXT_SAT             = "Sat";
  //Months
  TXT_JAN             = "Jan";
  TXT_FEB             = "Feb";
  TXT_MAR             = "Mar";
  TXT_APR             = "Apr";
  TXT_MAY             = "May";
  TXT_JUN             = "Jun";
  TXT_JUL             = "Jul";
  TXT_AUG             = "Aug";
  TXT_SEP             = "Sep";
  TXT_OCT             = "Oct";
  TXT_NOV             = "Nov";
  TXT_DEC             = "Dec";
  TXT_HI              = "Hi";
  TXT_LO              = "Lo";
  TXT_RH              = "RH";
  SaveSettings();
  UpdateData();
  Homepage();
}
