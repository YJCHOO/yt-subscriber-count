#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h>

//OLED Display info
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//init OLED Driver
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//WiFi credential
char ssid[] = "<Your Wi-Fi SSID>";
char password[] = "<Your Wi-Fi Password>";

//Google Cloud Api
#define YT_DATA_API_KEY "<Your YouTube Data API Key>"

//YouTube Channel ID
#define YT_CHANNEL_ID "<Your YouTube Channel ID>"

//Use this client to connect a secure server.
WiFiClientSecure client;
YoutubeApi api(YT_DATA_API_KEY, client);

unsigned long interval = 60*60*1000UL;
bool isInvert = false;

// 'Youtube Logo', 32x32px
// Byte Array
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 
  0xff, 0xf8, 0xff, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 
  0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void setup() {
  Serial.begin(115200);
  Wire.begin(0, 2); 
  client.setInsecure();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //clear the initial Adafruit Industries Logo
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  
  display.print("Connecting WiFi: ");
  display.print(ssid);
  display.display();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    display.print(".");
    display.display();
    delay(500);  
  }
  
  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("WiFi connected.");
  display.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  display.println(ip);
  display.display();
}

void loop() {
  display.invertDisplay(isInvert);
  if(api.getChannelStatistics(YT_CHANNEL_ID)) {
    display.clearDisplay();
    display.drawBitmap(5, 15, myBitmap, 32, 32, WHITE);

    display.setCursor(70, 23);
    display.println(api.channelStats.subscriberCount);
    
    display.setCursor(45, 33);
    display.println("Subscribers");
      
    display.display();
  }
  delay(interval);
  isInvert = !isInvert;
}
