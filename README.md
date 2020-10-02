# ESP32_Advanced_Weather_Server
Using an ESP32 to display Open-Weather-Map data using a Webserver

An ESP32 is used to obtain OWM Data and then displays this on a web-page using a webserver

Monitor the serial port for the assigned IP address. E.g. http://192.168.0.19/ or use the logical name  http://wxserver.local/

Then connect to server with http://192.168.0.19/ or http://wxserver.local/

System Set-up
1. Enter your City/Location name, choose any name.
2. Enter your location Latitude and Longitude, e.g. 51.38, -2.33
   - Use negative numbers for South of the Equator and East of the Meridian.
3. Enter your Language e.g. 'en', see below for OWM supported languages.
4. Enter your locations Hemisphere, North or South.
5. Enter your desired wind units either MPS (Metres-per-Second), MPH (Miles-per-hour), KPH (Kilometres-per-hour) or KTS (Knots)
6. Enter your desired Units either Metric or Imperial.
7. Enter the text for each item in the Language Translation Table. e.g. for 'de' 'Pressure' enter 'Druck'
    - For months you could use '01' for JAN, etc.
    
OWM Supports the following languages:
af Afrikaans, al Albanian, ar Arabic, az Azerbaijani
bg Bulgarian
ca Catalan, cz Czech
da Danish, de German
el Greek, en English, eu Basque
fa Persian (Farsi), fi Finnish, fr French
gl Galician
he Hebrew, hi Hindi, hr Croatian, hu Hungarian
id Indonesian, it Italian
ja Japanese
kr Korean
la Latvian, lt Lithuanian
mk Macedonian
no Norwegian, nl Dutch
pl Polish, pt Portuguese, pt_br Português Brasil
ro Romanian, ru Russian
sv, se Swedish, sk Slovak, sl Slovenian, sp, es Spanish, sr Serbian
th Thai, tr Turkish
ua, uk Ukrainian
vi Vietnamese
zh_cn Chinese Simplified, zh_tw Chinese Traditional, zu Zulu
    - Example: German set lauguage = 'de'
    
 ![alt_text, width="600"](/WX V2-1.jpg)

