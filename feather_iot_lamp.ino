#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//#define DEBUG
#ifdef DEBUG
  #define DEBUG(x)  Serial.println(x)
#else
  #define DEBUG(x)
#endif

#define PIN 4
#define NUMPIXELS      25
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* SSID     = "<SSID>";
const char* PASSWORD = "<PASSWORD>";

ESP8266WebServer server(80);
String webString="";
String LAMPIP;

#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display = Adafruit_SSD1306();

uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(128, 128, 0);
uint32_t off = strip.Color(0,0,0);

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);

  DEBUG("OLED FeatherWing test");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  DEBUG("OLED begun");

  DEBUG("Initializing display");
  display.display();

  DEBUG("Clearing display buffer");
  display.clearDisplay();
  display.display();

  DEBUG("Setting text properties and positioning cursor");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  DEBUG("Connecting to WiFi");
  display.print("Connecting.");
  display.display();
  while(WiFi.status() != WL_CONNECTED){
    DEBUG("...waiting");
    delay(500);
    display.print(".");
    display.display();
  }
  DEBUG("WiFi connected");
  DEBUG("Get IP address");
  LAMPIP = WiFi.localIP().toString();
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  DEBUG("IP: " + LAMPIP);
  display.print("IP:");
  display.println(LAMPIP);
  display.display();

  DEBUG("Initialize webserver");
  server.on("/", handle_root);
  server.on("/red", [](){
    DEBUG("Illuminating lamp with red");
    alert_color(red);
  });
  server.on("/green", [](){
    DEBUG("Illuminating lamp with green");
    alert_color(green);
  });
  server.on("/yellow", [](){
    DEBUG("Illuminating lamp with yellow");
    alert_color(yellow);
  });
  server.begin();
  DEBUG("Webserver started");
  display.setCursor(0,10);
  display.println("Server online.");
  display.display();
  DEBUG("Initialize NeoPixels and set to green");
  strip.begin();
  for(int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i, green);
  }
  strip.show();
  DEBUG("---");
}

void loop() {
  server.handleClient();
}

void handle_root(){
  DEBUG("Processing root page request...");
  alert_color(off);
  server.send ( 200, "text/html", "hello world");
}

void alert_color(uint32_t c){
  String s;
  DEBUG("Update alert status text");
  if(c == red){
    s = "Critcal Alert!";
  } else if( c == green){
    s = "All systems Go!";
  } else if (c == yellow){
    s = "Caution!";
  } else {
    s = "nominal";
  }
  DEBUG("Alert Status: " + s);
  /*DEBUG("Reset display and position cursor");
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.print("IP:");
  display.println(LAMPIP);
  display.setCursor(0,10);
  display.println("Alert Status:" + s);
  display.display();
  DEBUG("Reinitialze Neopixels");
  strip.begin();*/
  DEBUG("Update Neopixel color");
  for(int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i, c);
  }
  strip.show();
  DEBUG("Return 200 response");
  DEBUG("---");
  server.send ( 200, "text/html", "Alert Status:" + s);
}
