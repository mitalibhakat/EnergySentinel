#define BLYNK_PRINT Serial
#include<WiFi.h>
#include<WiFiClient.h>
#include<BlynkSimpleEsp32.h>

#include <PZEM004Tv30.h>
PZEM004Tv30 pzem1(Serial2, 16, 17);

#define BLYNK_TEMPLATE_ID "TMPL3kF3J9DuY"
#define BLYNK_TEMPLATE_NAME "ENERGY METER"
#define BLYNK_AUTH_TOKEN "gJdqqL8mk3XHgfqblF0CYcv0nIvxot3E"
char auth[] = "gJdqqL8mk3XHgfqblF0CYcv0nIvxot3E";
char ssid [] = "plot28 upper floor";
char pass [] = "Sashagd123";

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  
#define OLED_RESET -1       
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float VOLTAGE=0;
float CURRENT=0;
float POWER=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 10);
  display.println("tech ");
  display.setCursor(10, 35);
  display.println("silicon");
  display.display();
  Serial.println("\nFZEM004T Testing");
  delay(2000);

    Serial.begin(9600);
Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  display.display();
}


void loop() {
   
   Blynk.run();
VOLTAGE = pzem1.voltage();
  VOLTAGE = zeroIfNan(VOLTAGE);
  CURRENT = pzem1.current();
  CURRENT = zeroIfNan(CURRENT);
  POWER = pzem1.power();
  POWER = zeroIfNan(POWER);
 Serial.println("");
 Serial.println("Voltage:" + String(VOLTAGE));
 Serial.println("Current:" + String(CURRENT));
 Serial.println("Power Active:" + String(POWER));
 Serial.println("---------- END ----------");
  Serial.println("");
  display_Update();
  display.display();

   if (millis() - lastMillis > 5000) {
            lastMillis = millis();
            
            Blynk.virtualWrite(V0, VOLTAGE);
            Blynk.virtualWrite(V1, CURRENT);            
            Blynk.virtualWrite(V2, POWER);          

          }         
}
float zeroIfNan(float v) {
  if (isnan(v))
    v = 0;
  return v;
}
void display_Update() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.print(VOLTAGE);
  display.print(" V");
  display.setCursor(5, 16);
  display.print(CURRENT);
  display.print(" A");
  display.setCursor(5, 32);
  display.print(POWER);
  display.print(" W");
  display.setCursor(5, 48);
  display.display();
  }