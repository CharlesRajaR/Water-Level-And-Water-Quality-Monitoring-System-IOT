#define BLYNK_TEMPLATE_ID "TMPL3fnszE_Oc"
#define BLYNK_TEMPLATE_NAME "water level monitoring and quality checker simulat"
#define BLYNK_DEVICE_NAME "level-quality-monitor"
#define BLYNK_AUTH_TOKEN "L8gYYUEtIx89ygAIylcr6M3b1-n6RD29"
#define PIN_TRIG 4
#define PIN_ECHO 2

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "Wokwi-GUEST";
const char* pass = "";

#define PH_SENSOR_PIN 34
#define TURBIDITY_SENSOR_PIN 35

BlynkTimer timer;



void sendData() {

  Serial.println("=============PH SENSOR OUTPUT===============");

  int analogValue = analogRead(PH_SENSOR_PIN);

  double pH = map(analogValue, 0, 4095, 0, 14);

  Serial.print("pH Value: ");
  Serial.println(pH);

  // Check water conditions and send alerts:

  // For drinking water, the ideal pH range is typically 6.5 to 8.5, 
  // as recommended by health organizations, though this can vary depending
  //  on local standards. Extreme pH levels (too acidic or too alkaline)
  //  can cause water to be unsuitable for drinking or even harmful to health.

  if (pH < 6.5) {
    Serial.println("Alert: Water is acidic!");
  } else if (pH > 8.5) {
    Serial.println("Alert: Water is alkaline!");
  } else {
    Serial.println("Water is neutral.");
  }

  Serial.println("=========ULTRASONIC SENSOR OUTPUT==========");
  int duration = pulseIn(PIN_ECHO, HIGH);
  Serial.print("Distance in CM: ");
  Serial.println(duration / 58);
  Serial.print("Distance in inches: ");
  Serial.println(duration / 148);

  float distance = duration / 58;
  if(distance > 360){
    Serial.println("Low level of water");
    Serial.println("Motor is ON");
  }
  else if(distance <= 20){
    Serial.println("the tank is filled");
    Serial.println("Motor is Off");
  }


  Serial.println("===========TURBIDITY SENSOR OUTPUT============");

  int analogValue1 = analogRead(TURBIDITY_SENSOR_PIN);

  int turbidity = map(analogValue1, 0, 4095, 0, 500);

  Serial.print("Turbidity Level: ");
  Serial.print(turbidity);
  Serial.println(" NTU");

  // Check if the turbidity level crosses the drinking water limit
  if(turbidity <= 5){
    Serial.println("Clear water, typically safe for drinking."); 
  }
  else if(turbidity > 5 && turbidity <= 50){
    
    Serial.println("Slightly cloudy, may need treatment for consumption.");
  }
  else if(turbidity > 50 && turbidity <= 100){
    Serial.println("High turbidity, often due to increased suspended particles, not safe for drinking.");
  }
  else{
    Serial.println("Extremely turbid water, likely caused by pollution or soil erosion, unsuitable for most uses.");
  }

  Blynk.virtualWrite(V0, pH);
  Blynk.virtualWrite(V1, turbidity);
  Blynk.virtualWrite(V2, distance);

  Serial.println("Data Sent to Blynk Successfully");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendData);
}

void loop(){
   Blynk.run();
   timer.run();
}
