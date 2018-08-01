// Library
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi settings
const char* ssid = "*****";
const char* password = "*****";

// Time to sleep (in seconds):
const int sleepTimeS = 10;

// Host
const char* host = "dweet.io";

const int ONE_WIRE_BUS = D5; // Data wire is plugged into port 2 on the Arduino

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices 

DallasTemperature sensors(&oneWire); // Pass oneWire reference to Dallas Temperature. 

void setup() 
{

  // Serial
  Serial.begin(115200);
  Serial.println("ESP8266 in normal mode");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Logging data to cloud
  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  sensors.requestTemperatures();
  float t = (sensors.getTempCByIndex(0));
  
  // This will send the request to the server
  client.print(String("GET /dweet/for/*****?temp=") + String(t) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

  // Sleep
  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTimeS * 1000000);
  
}

void loop() 
{

}
