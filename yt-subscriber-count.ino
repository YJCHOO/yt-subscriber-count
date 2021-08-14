
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <YoutubeApi.h>
#include <ArduinoJson.h>

//OLED Display info
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//WiFi credential
char ssid[] = "<Your Wi-Fi SSID>";
char password[] = "<Your Wi-Fi Password>";

//Google Cloud Api
#define YT_DATA_API_KEY "<Your YouTube Data API Key>"

//YouTube Channel ID
#define YT_CHANNEL_ID "<Your YouTube Channel ID>"

WiFiClientSecure client;
YoutubeApi api(YT_DATA_API_KEY, client);

unsigned long interval = 60*60*1000UL;

// 'Youtube Logo', 32x32px
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

long subs = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(0, 2); 
  client.setInsecure();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //clear the initail Adafruit Industries Logo
  display.clearDisplay();
  display.display();

  initDisplay();

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
  initDisplay();
  
  display.println("WiFi connected.");
  display.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  display.println(ip);
  display.display();
}

void loop() {  
  if(api.getChannelStatistics(YT_CHANNEL_ID)) {
    display.clearDisplay();
    display.drawBitmap(5, 15, myBitmap, 32, 32, WHITE);

    display.setCursor(70, 23);
    display.setTextColor(WHITE);
    display.println(api.channelStats.subscriberCount);
    
    display.setCursor(45, 33);
    display.println("Subscribers");
      
    display.display();
  }
  delay(interval);
}

void initDisplay () {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}
