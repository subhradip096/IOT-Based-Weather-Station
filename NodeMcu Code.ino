#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

char ssid[] = "Galaxy M21A2EF";   // your network SSID (name)
char pass[] = "password11";

WiFiClient  client;

unsigned long Channel_ID = 1685330; // Channel ID
const int Field_number = 1;
const int Field_number2 = 2;// Don't change
const char * WriteAPIKey = "4ZVDV5V7SLQEP0UK"; // Your write API Key


//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

float temp;
float hum;
float co2ppm;
float smoke;
int x = 0;
void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  while (!Serial) continue;
}

void loop() {
  internet();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Humidity:  ");
  hum = data["humidity"];
  Serial.println(hum);
  Serial.print("Recieved Temperature:  ");
  temp = data["temperature"];
  Serial.println(temp);
  Serial.print("CO2 in ppm:  ");
  co2ppm = data["CO2 PPM Level"];
  Serial.println(co2ppm);
  Serial.print("Rain value:  ");
  smoke = data["Rain"];
  Serial.println(smoke);
  Serial.println("-----------------------------------------");
  upload();
}
void upload()
{
  internet();
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  ThingSpeak.setField(3, co2ppm);
  ThingSpeak.setField(4, smoke);
  x = ThingSpeak.writeFields(Channel_ID, WriteAPIKey);
  if (x == 200)Serial.println("Data Updated.");
  if (x != 200)
  {
    Serial.println("Data upload failed, retrying....");
    delay(15000);
    upload();
  }
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}
