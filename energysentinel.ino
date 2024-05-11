#define BLYNK_PRINT Serial
#include<WiFi.h>
#include<WiFiClient.h>
#include<BlynkSimpleEsp32.h>
#include <PZEM004Tv30.h>

#define BLYNK_TEMPLATE_ID "TMPL3kF3J9DuY"
#define BLYNK_TEMPLATE_NAME "ENERGY METER"
#define BLYNK_AUTH_TOKEN "HYMSYaRimMYcOdVe8HheBU3BQRyr-FTB" 

char auth[] = "HYMSYaRimMYcOdVe8HheBU3BQRyr-FTB";
char ssid[] = "SUDU";
char pass[] = "sudu2002";
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1     
#define SCREEN_ADDRESS 0x3C  
Adafruit_SSD1306 display(128,64, &Wire,-1);


float RS_=0;
float VOLTAGE=0;
float CURRENT=0;
float POWER=0;
float ENERGY=0.000;
unsigned long lastMillis = 0;
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;

  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
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
  ENERGY = pzem1.energy();
  ENERGY = zeroIfNan(ENERGY);

  Serial.println("");
  Serial.println("Voltage:" + String(VOLTAGE));
  Serial.println("Current:" + String(CURRENT));
  Serial.println("Power Active:" + String(POWER));
Serial.println("Energy:" + String(ENERGY));
  Serial.println("---------- END ----------");
  Serial.println("");
  Energy_Calculation();
  display_Update();


  display.display();
         if (millis() - lastMillis > 5000) {
            lastMillis = millis();
            
            Blynk.virtualWrite(V0, VOLTAGE);
            Blynk.virtualWrite(V1, CURRENT);            
            Blynk.virtualWrite(V2, POWER);
            Blynk.virtualWrite(V3, ENERGY);
            Blynk.virtualWrite(V4, RS_);           
          

          }         
}

float zeroIfNan(float v) {
  if (isnan(v))
    v = 0;
  return v;
}
void display_Update() {
  display.clearDisplay();
  display.setTextSize(1.7);
  display.setCursor(5, 0);
  display.print(VOLTAGE);
  display.print(" V");
  display.setCursor(5, 11);
  display.print(CURRENT);
  display.print(" A");
  display.setCursor(5, 22);
  display.print(POWER);
  display.print(" W");
  display.setCursor(5, 33);
  display.print(ENERGY);
  display.print("kwh"); 
  display.setCursor(5, 44);
  display.print(RS_);
  display.print("Rs_");
  display.setCursor(5, 40);
  display.display();
}

void Energy_Calculation() {
  if (ENERGY <= 50)
    RS_ = ENERGY * 1.5;
  else if (ENERGY > 50 && ENERGY <= 100)
    RS_ = 150 + (ENERGY - 50) * 2;
  else if (ENERGY > 100 && ENERGY <= 200)
    RS_ = 350 + (ENERGY - 2) * 3;
  else if (ENERGY > 200)
    RS_ = 950 + (ENERGY - 200) * 4;
  Serial.println("Rupees: " + String(RS_));
}
