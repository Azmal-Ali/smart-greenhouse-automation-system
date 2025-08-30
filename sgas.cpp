// Include required libraries
#define BLYNK_TEMPLATE_ID "TMPL6kyYobYJd"
#define BLYNK_TEMPLATE_NAME "ggg"
#define BLYNK_AUTH_TOKEN "nTurBCldnO3iggr2-bVIww62l0RTbatj"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Define your authentication information and Wi-Fi credentials
char auth[] = "nTurBCldnO3iggr2-bVIww62l0RTbatj";
char ssid[] = "D-20-09";
char pass[] = "0189718976";

// Define pins for various sensors and actuators
#define DHT_type DHT11
#define DHT_pin D8
#define soil_moisture_pin A0
#define soil_moisture_VCC D6
#define LDR_VCC D5
#define relay_leds D3
#define relay_fan D4
#define relay_pump D7

// Create a DHT object
DHT dht(DHT_pin, DHT_type);

// Define variables to store sensor data
float humidity = 0;
float humidity = 0;
int light = 0;
int moisture = 0;
float moisture_percentage = 0;

// Create a timer object
SimpleTimer timer;

// Define threshold values for different variables
int lum_thresh = 25;
int moist_thresh = 400;
int hum_thresh = 85;
int temp_thresh = 30;
int pump_timer = 350;
bool leds_ON = false;

// Create LED widget objects
WidgetLED led1(V4);
WidgetLED led2(V5);
WidgetLED led3(V6);

// Setup function: runs once at startup
void setup() {
    // Initialize sensor pins and modes
    // Soil Moisture
    pinMode(soil_moisture_VCC, OUTPUT);
    digitalWrite(soil_moisture_VCC, LOW);

    
  // DHT
  pinMode(DHT_pin, INPUT);

   // LDR
   pinMode(LDR_VCC, OUTPUT);
   digitalWrite(LDR_VCC, LOW);
  
   // Relays
   pinMode(relay_pump, OUTPUT);
   digitalWrite(relay_pump, LOW);

   pinMode(relay_leds, OUTPUT);
   digitalWrite(relay_leds, LOW);

   pinMode(relay_fan, OUTPUT);
   digitalWrite(relay_fan, LOW);

   Serial.begin(115200); // Initialize serial communication
   delay(10);

   Blynk.begin(auth, ssid, pass); // Initialize Blynk connection
   dht.begin(); // Initialize DHT sensor

   // Turn on status LEDs
    led1.on();
    led2.on();
    led3.on();

    // Set up the function to be called at regular intervals
    timer.setInterval(10000L, getData);
}


// Main loop function
void loop() {
    Blynk.run(); // Execute Blynk tasks
    timer.run(); // Execute timer tasks
}

// Function to collect and process sensor data
void getData() {
    Serial.println("Analysis Started");
    getDhtData();
    getSoilMoistureData();
    getLightData();
    controlLights();
    controlPump();
    controlFan();
}

// Function to collect DHT sensor data
void getDhtData(void) {
     delay(500);
     float tempIni = temperature;
     float humIni = humidity;
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // Check for sensor reading errors
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
         temperature = tempIni;
         humidity = humIni;
         return;
           }
           Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" °C");
             Serial.print("Humidity: ");
              Serial.print(humidity);
               Serial.print(humidity);
               Blynk.virtualWrite(V2, humidity);
               Blynk.virtualWrite(V0, temperature);
               }

 // Function to collect soil moisture data
 void getSoilMoistureData(void) {
    digitalWrite(soil_moisture_VCC, HIGH);
      delay(500);
      moisture = analogRead(soil_moisture_pin);
       moisture_percentage = 100 - ((moisture * 100) / 1023);
       Serial.print("Soil Moisture: ");
       Serial.println(moisture);
         Serial.print("Soil Moisture Percentage: ");
         Serial.print(moisture_percentage);
         Serial.println(" %");
         Blynk.virtualWrite(V3, moisture_percentage);
         digitalWrite(soil_moisture_VCC, LOW);
         }
  
// Function to collect light intensity data
void getLightData(void) {
    delay(500);
    digitalWrite(LDR_VCC, HIGH);
    delay(500);
    light = analogRead(soil_moisture_pin);
     light = (light * 100) / 1024;
     Serial.print("Light Intensity: ");
     Serial.print(light);
     Serial.println(" %");
      Blynk.virtualWrite(V1, light);
      digitalWrite(LDR_VCC, LOW);
      }
      
 // Function to control LEDs based on light intensity
 void controlLights() {
    if (light < lum_thresh && (!leds_ON)) {
        Serial.println("LEDs ON");
         digitalWrite(relay_leds, LOW);
            Blynk.setProperty(V5, "color", "#00FF2E");
             } else if (light < lum_thresh && leds_ON) {
                 Serial.println("LEDs already ON");
                 } else {
                     Serial.println("LEDs OFF");
                     digitalWrite(relay_leds, HIGH);
                     Blynk.setProperty(V5, "color", "#FF0000");
                       }
                        delay(200);
                        }

// Function to control the water pump
void controlPump() {
    if (moisture < moist_thresh) {
        Serial.println("PUMP OFF");
        digitalWrite(relay_pump, LOW);
        Blynk.setProperty(V4, "color", "#FF0000");
         } else {
            Serial.println("PUMP ON");
             digitalWrite(relay_pump, HIGH);
              Blynk.setProperty(V4, "color", "#00FF2E");
               delay(pump_timer);
               digitalWrite(relay_pump, LOW);
               Blynk.setProperty(V4, "color", "#FF0000");
               }
               }

// Function to control the fan based on humidity and temperature
void controlFan() {
    if (humidity > hum_thresh || temperature > temp_thresh) {
        Serial.println("FAN ON");
         digitalWrite(relay_fan, LOW);
          Blynk.setProperty(V6, "color", "#00FF2E");
          } else {
             Serial.println("FAN OFF");
             digitalWrite(relay_fan, HIGH);
             Blynk.setProperty(V6, "color", "#FF0000");
              }
              }






















