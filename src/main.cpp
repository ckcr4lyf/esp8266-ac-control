// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define DHTPIN 5 // GPIO5 = D1 on the NodeMCU ESP8266
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const char * SSID = "BOM VPN";
const char * WIFI_PW = WPASSWORD; // WPASSWORD macro is defined via build flags as the system env `ENV_WIFI_PW`

const char* PARAM_MESSAGE = "message";

float t = 0.0;
float h = 0.0;

/*
  IR related stuffs begin
*/

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

uint16_t turn_ac_on[531] = {3336, 1684,  458, 1222,  460, 382,  458, 382,  458, 382,  458, 382,  460, 382,  460, 382,  458, 382,  458, 380,  460, 382,  460, 382,  458, 382,  460, 1224,  458, 382,  460, 382,  460, 380,  460, 382,  460, 382,  458, 382,  460, 380,  460, 382,  460, 382,  458, 382,  460, 380,  458, 382,  460, 382,  460, 382,  458, 382,  460, 382,  460, 380,  460, 1224,  458, 380,  462, 1222,  460, 1224,  458, 1224,  458, 1224,  458, 1222,  460, 1222,  462, 382,  458, 1224,  458, 1224,  458, 1224,  460, 1222,  460, 1222,  460, 1224,  458, 1224,  458, 1222,  460, 1222,  460, 382,  460, 382,  458, 382,  460, 380,  460, 382,  458, 382,  460, 382,  458, 382,  460, 382,  460, 382,  458, 1224,  458, 1222,  460, 382,  460, 382,  458, 1222,  460, 1222,  460, 1224,  458, 1222,  460, 382,  460, 382,  458, 1224,  458, 1224,  460, 380,  460, 382,  458, 382,  458, 1224,  460, 380,  460, 382,  460, 1222,  460, 382,  458, 382,  460, 1226,  456, 1222,  460, 382,  458, 1224,  458, 1224,  460, 380,  460, 1224,  458, 1224,  458, 382,  460, 1222,  460, 1222,  460, 382,  458, 382,  458, 1224,  458, 382,  460, 380,  460, 382,  460, 382,  458, 382,  458, 1222,  460, 1224,  458, 382,  458, 1224,  458, 1224,  458, 1224,  458, 382,  460, 382,  458, 382,  458, 1226,  456, 382,  460, 382,  458, 1224,  460, 380,  460, 1222,  460, 1224,  458, 1222,  460, 382,  458, 1226,  456, 1222,  460, 382,  458, 1222,  460, 382,  460, 382,  458, 382,  460, 382,  458, 382,  458, 382,  458, 382,  460, 380,  460, 1222,  460, 1224,  458, 1224,  458, 1224,  458, 1224,  458, 1224,  456, 1224,  458, 1224,  458, 382,  458, 382,  460, 382,  458, 382,  460, 382,  458, 382,  458, 382,  458, 384,  458, 1224,  458, 1224,  460, 1222,  460, 1224,  458, 1224,  458, 1224,  458, 1224,  458, 1224,  458, 380,  460, 382,  458, 382,  460, 380,  460, 382,  458, 382,  458, 382,  458, 382,  458, 1224,  458, 1224,  458, 1224,  458, 1224,  460, 1224,  458, 1222,  460, 1224,  458, 1224,  458, 382,  458, 382,  458, 382,  458, 382,  458, 382,  458, 382,  458, 382,  418, 422,  418, 1264,  458, 
1224,  418, 1264,  460, 1222,  460, 1222,  460, 1224,  458, 1226,  456, 1226,  458, 382,  458, 382,  458, 384,  416, 422,  418, 424,  418, 422,  418, 422,  418, 424,  416, 1264,  458, 1224,  458, 1222,  418, 1266,  456, 1222,  460, 1224,  458, 1224,  458, 1226,  418, 1266,  458, 1222,  460, 382,  418, 424,  416, 1264,  418, 1266,  416, 424,  418, 422,  418, 422,  458, 384,  416, 1266,  416, 1266,  458, 382,  418, 422,  418, 1266,  416, 1264,  418, 1266,  456, 382,  418, 422,  458, 384,  416, 1266,  456, 384,  416, 1264,  418, 1266,  416, 424,  418, 1264,  416, 1266,  456, 1224,  418, 424,  418, 1264,  418, 424,  418, 424,  416, 424,  418, 422,  418, 424,  416, 424,  418, 422,  418, 424,  418, 422,  418, 424,  418, 1264,  460, 1224,  416, 1266,  416, 1266,  418, 1266,  416, 1264,  418, 1266,  458, 1224,  418, 422,  418, 424,  416, 424,  418, 422,  418, 422,  418, 424,  418, 422,  418, 424,  418, 1264,  458, 1224,  418, 1266,  418, 1264,  418, 1264,  418, 1266,  418, 1264,  418, 1266,  416};  // HITACHI_AC264

uint16_t turn_ac_off[531] = {3336, 1688,  416, 1268,  414, 426,  414, 426,  416, 424,  418, 424,  414, 426,  414, 426,  416, 426,  414, 426,  416, 426,  414, 426,  416, 426,  414, 1266,  416, 426,  414, 426,  414, 424,  416, 424,  416, 426,  414, 426,  416, 426,  414, 426,  414, 426,  416, 424,  416, 426,  416, 426,  414, 426,  414, 426,  416, 424,  416, 424,  416, 426,  414, 1268,  414, 426,  416, 1268,  414, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 426,  416, 1266,  416, 1266,  416, 1266,  416, 1268,  414, 1266,  416, 1266,  414, 1266,  416, 1268,  414, 1268,  414, 426,  414, 426,  414, 426,  416, 424,  416, 426,  414, 424,  416, 426,  414, 426,  414, 426,  416, 426,  416, 1266,  416, 1266,  416, 424,  418, 424,  416, 1268,  414, 1266,  416, 1268,  414, 1264,  418, 426,  414, 424,  416, 1266,  416, 1268,  414, 424,  416, 426,  414, 426,  414, 1266,  416, 426,  416, 424,  416, 1268,  416, 424,  416, 424,  416, 1268,  416, 1266,  416, 426,  416, 1268,  414, 1268,  414, 426,  414, 1266,  416, 1268,  414, 424,  416, 1268,  414, 1266,  416, 424,  416, 426,  416, 1266,  418, 422,  418, 426,  414, 424,  416, 426,  414, 426,  416, 1266,  416, 1266,  416, 424,  416, 1264,  418, 1266,  416, 1266,  416, 424,  418, 424,  416, 424,  418, 1266,  416, 422,  418, 424,  418, 1266,  416, 426,  414, 1266,  418, 1264,  418, 1266,  416, 422,  418, 1266,  418, 1266,  416, 424,  418, 1264,  418, 422,  418, 424,  416, 424,  416, 424,  418, 424,  416, 424,  416, 422,  418, 424,  418, 1266,  416, 1264,  418, 1264,  418, 1264,  416, 1266,  416, 1264,  418, 1264,  418, 1266,  416, 424,  418, 424,  416, 424,  418, 422,  418, 424,  418, 424,  416, 424,  418, 424,  416, 1266,  416, 1264,  418, 1264,  418, 1264,  418, 1266,  418, 1264,  416, 1266,  418, 1264,  418, 424,  416, 424,  418, 424,  416, 424,  416, 422,  418, 424,  418, 424,  416, 424,  416, 1266,  416, 1266,  418, 1266,  416, 1266,  416, 1266,  416, 1264,  418, 1264,  418, 1264,  418, 424,  416, 424,  416, 424,  416, 424,  418, 424,  416, 424,  416, 424,  416, 424,  418, 1266,  416, 
1264,  418, 1266,  416, 1266,  416, 1266,  416, 1268,  414, 1268,  414, 1266,  416, 426,  416, 424,  416, 424,  416, 426,  416, 426,  414, 426,  416, 424,  416, 426,  416, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 1268,  416, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 426,  416, 424,  416, 1268,  416, 1266,  414, 426,  416, 426,  414, 426,  414, 426,  416, 1266,  416, 1266,  416, 424,  416, 424,  416, 1266,  416, 1268,  416, 1268,  414, 424,  416, 424,  416, 426,  414, 424,  416, 426,  416, 1268,  414, 1266,  416, 426,  414, 1268,  416, 1266,  416, 1266,  416, 
1264,  416, 1266,  416, 424,  416, 424,  416, 426,  416, 424,  416, 424,  416, 426,  416, 424,  416, 424,  416, 426,  416, 424,  416, 1268,  414, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 1266,  416, 1268,  416, 426,  414, 426,  414, 426,  414, 426,  416, 424,  416, 426,  414, 426,  414, 426,  416, 1266,  416, 1266,  416, 1266,  416, 1268,  414, 1266,  416, 1268,  414, 1266,  416, 1266,  418};  // HITACHI_AC264

void Turn_ac_on(){
  Serial.println("Turning on AC...");
  irsend.sendRaw(turn_ac_on, 531, 38);
  Serial.println("Sent ON signal!");
}

void Turn_ac_off(){
  Serial.println("Turning off AC...");
  irsend.sendRaw(turn_ac_off, 531, 38);
  Serial.println("Sent OFF signal!");
}

AsyncWebServer server(80);

const char res[] PROGMEM = R"rawliteral(
up 1
temperature %TEMPERATURE%
humidity %HUMIDITY%
)rawliteral";

String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  } else if(var == "HUMIDITY"){
    return String(h);
  }

  return String();
}

// https://arduino.stackexchange.com/a/58612
#define LED_BUILTIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize device.
  irsend.begin();
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));

  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PW);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, LOW);
    request->send_P(200, "text/plain", res, processor);
    digitalWrite(LED_BUILTIN, HIGH);
  });

  server.on("/on", HTTP_POST, [](AsyncWebServerRequest *request){
    Turn_ac_on();
    request->send(200, "text/plain", "Sent ON signal!");
  });

  server.on("/off", HTTP_POST, [](AsyncWebServerRequest *request){
    Turn_ac_off();
    request->send(200, "text/plain", "Sent OFF signal!");
  });

  server.begin();  
}

void loop() {
  // Delay between measurements.
  delay(delayMS);

  // Check to see if WiFi is still connected
  if (WiFi.status() != WL_CONNECTED){
    Serial.println(F("Wifi not connected, will restart!"));
    digitalWrite(LED_BUILTIN, LOW); // turn on LED as an indicator in case restart doesn't work
    ESP.restart();
  }

  // Get temperature event and print its value.
  sensors_event_t event;

  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    t = event.temperature;
    Serial.println(F("°C"));
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    h = event.relative_humidity;
    Serial.println(F("%"));
  }
}