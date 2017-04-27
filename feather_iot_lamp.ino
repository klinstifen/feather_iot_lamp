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

#define PIN 4
#define NUMPIXELS      25
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid     = "<YOUR SSID>";
const char* password = "<YOUR PASSWORD>";

ESP8266WebServer server(80);
String webString="";

#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display = Adafruit_SSD1306();

int delayval = 500; // delay for half a second
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t off = strip.Color(0,0,0);

void setup() {

  WiFi.begin(ssid, password);

  Serial.println("OLED FeatherWing test");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  Serial.println("OLED begun");
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Connecting.");
  display.display();
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    display.print(".");
    display.display();
  }
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.print("IP:");
  display.println(WiFi.localIP());
  display.display();
  
  server.on("/", handle_root);
  server.on("/red", [](){
    alert_color(red);
  });
  server.on("/green", [](){
    alert_color(green);
  });
  server.on("/yellow", [](){
    alert_color(yellow);
  });
  
  server.begin();
  display.setCursor(0,10);
  display.println("Server online.");
  display.display();
  strip.begin(); // This initializes the NeoPixel library.
}

void loop() {
  server.handleClient();
}

void handle_root(){
  alert_color(off);
  server.send ( 200, "text/html", "hello world");
}

void alert_color(uint32_t c){
  String s;

  if(c == red){
    s = "red";
  } else if( c == green){
    s = "green";
  } else if (c == yellow){
    s = "yellow";
  } else {
    s = "nominal";   
  }
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.print("IP:");
  display.println(WiFi.localIP());
  display.setCursor(0,10);
  display.println("Alert Status:" + s);
  display.display();
  strip.begin();
  for(int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i, c); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    //delay(500);
  }
  
  server.send ( 200, "text/html", "status:" + s);
}
