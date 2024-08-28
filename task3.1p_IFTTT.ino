#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

char ssid[] = "realme";
char pass[] = "Realme24";

WiFiClient client;

BH1750 lightMeter; // Create an instance of the BH1750 light sensor library

char HOST_NAME[] = "maker.ifttt.com"; // Website
String EVENT_NAME = "shivansh_light"; // IFTTT Event Name
String KEY = "https://maker.ifttt.com/trigger/shivansh_light/with/key/lm8b_NAvceliAya6BwcWAca1HqPkaYn9oqN1JVjBuPy"; // IFTTT Webhook Key
String value;

void setup() 
{
  WiFi.begin(ssid, pass);
  Serial.begin(9600); 

  Wire.begin();
  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));

  // Attempt to connect to WiFi
  connectWiFi();
}

void loop() 
{
  float light_intensity = lightMeter.readLightLevel();
  Serial.print("light_intensity: ");
  Serial.println(light_intensity);
  value = "?value1=" + String(light_intensity);

  delay(30000); // Wait for 30 seconds before reading light  
}

void connectWiFi() 
{
  Serial.println("Attempting to connect to WiFi...");
  WiFi.begin(ssid, pass);  // Start WiFi connection
  do 
  {
    delay(500);  // Check every 0.5 seconds for connection status
    Serial.print(".");
  } 
  while (WiFi.status() != WL_CONNECTED);
  Serial.println("WiFi connected!");
}

void triggerWebhook(String eventName) 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectWiFi();
  }

  if (client.connect(HOST_NAME, 80)) 
  {
    client.println("GET " + KEY + value + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
    delay(500);
    client.stop();
    Serial.println("Data sent successfully");
  }
  else 
  {
    Serial.println("Unable to connect to webhook server");
  }
}
