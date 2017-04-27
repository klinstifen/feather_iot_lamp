# feather_iot_lamp
### Description:
For use with the Adafruit Feather HUZZAH w/ ESP8266 WiFi.  Once the HUZZAH connects to your specified network the OLED will display the lamp's IP address.  The lamp acts as a webserver and accepts requests on the following URLs:
- http://\<lamp IP address>/ *(Lamp is off and OLED says "Alert Status: nominal")*
- http://\<lamp IP address>/green *(Lamp is green and OLED says "Alert Status: green")*
- http://\<lamp IP address>/red *(Lamp is red and OLED says "Alert Status: red")*
- http://\<lamp IP address>/yellow *(Lamp is yellow and OLED says "Alert Status: yellow")*

System or networking monitoring systems could send simple CURL requests to specified URL for various alerts.

### Parts Needed:
See Thingiverse (http://www.thingiverse.com/thing:2266367)
