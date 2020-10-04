//#########################################################################################
void PreFill_ES() {
  City                = "Algorta";
  Latitude            = "43.35";
  Longitude           = "-3";
  Language            = "es";
  Hemisphere          = "north";
  Units               = "M";
  TXT_FORECAST_GRAPHS = "Pronóstico a 5 días / 3 horas";
  TXT_TEMPDEWPOINT    = "Temperatura / Punto de rocío";
  TXT_TEMPERATURE     = "Temperatura";
  TXT_FEELSLIKE       = "Sensación";
  TXT_HUMIDITY        = "Humedad";
  TXT_DEWPOINT        = "Punto de rocío";
  TXT_WINDCHILL       = "Sensación térmica";
  TXT_WCHILL_HINDEX   = "Sensación térmica / Indice de calor";
  TXT_HEATINDEX       = "Indice de calor";
  TXT_WINDSPEED       = "Velocidad del viento";
  TXT_PRESSURE        = "Presión";
  TXT_CLOUDCOVER      = "Nubosidad";
  TXT_VISIBILITY      = "Visibilidad";
  TXT_PRECIPITATION   = "Precipitación";
  TXT_RAINFALL        = "Lluvia";
  TXT_RAINRATE        = "Promedio de lluvia";
  TXT_SNOWFALL        = "Nieve";
  TXT_SNOWRATE        = "Promedio de nieve";
  TXT_UVINDEX         = "Índice UV";
  //Moon phases
  TXT_MOON_PHASE1     = "Nueva";
  TXT_MOON_PHASE2     = "Creciente";
  TXT_MOON_PHASE3     = "Primer cuarto";
  TXT_MOON_PHASE4     = "Menguante";
  TXT_MOON_PHASE5     = "Llena";
  TXT_MOON_PHASE6     = "Menguante";
  TXT_MOON_PHASE7     = "Tercer cuarto";
  TXT_MOON_PHASE8     = "Creciente menguante";
  //Wind
  TXT_N               = "N";
  TXT_NE              = "NE";
  TXT_E               = "E";
  TXT_SE              = "SE";
  TXT_S               = "S";
  TXT_SW              = "SO";
  TXT_W               = "O";
  TXT_NW              = "NO";
  WindspeedUnits      = KPH;    // Change to your desired display format e.g. for KPH 'MetricWindspeed = KPH';
  TXT_WINDUNITS       = "km/h"; // Ensure text here matahces units chosen above! Use a leading space
  //Windspeed warning colours based on Beaufort scale in kph *** NOTE these are HTML colours, don't translate!
  TXT_BFS0            = "Calma";
  TXT_BFS1            = "Aire ligero";
  TXT_BFS2            = "Brisa ligera";
  TXT_BFS3            = "Suave brisa";
  TXT_BFS4            = "Moderate Breeze";
  TXT_BFS5            = "Brisa moderada";
  TXT_BFS6            = "Brisa fuerte";
  TXT_BFS7            = "Fuerte viento";
  TXT_BFS8            = "Vendaval";
  TXT_BFS9            = "Vendaval severo";
  TXT_BFS10           = "Tormenta";
  TXT_BFS11           = "Tormenta violenta";
  TXT_BFS12           = "Huracán";
//Days of the week
  TXT_SUN             = "Dom";
  TXT_MON             = "Lun";
  TXT_TUE             = "Mar";
  TXT_WED             = "Mie";
  TXT_THU             = "Jue";
  TXT_FRI             = "Vie";
  TXT_SAT             = "Sab";
//Months
  TXT_JAN             = "Ene";
  TXT_FEB             = "Feb";
  TXT_MAR             = "Mar";
  TXT_APR             = "Abr";
  TXT_MAY             = "May";
  TXT_JUN             = "Jun";
  TXT_JUL             = "Jul";
  TXT_AUG             = "Ago";
  TXT_SEP             = "Sep";
  TXT_OCT             = "Oct";
  TXT_NOV             = "Nov";
  TXT_DEC             = "Dic";
  TXT_HI               = "Máx.";
  TXT_LO               = "Mín.";
  TXT_RH               = "Humedad";
  SaveSettings();
  UpdateData();
  Homepage();
}
