// Include the DHT11 library for interfacing with the temperature/humidity sensor.
#include <DHT11.h>

DHT11 dht11(12);  //temperature/humidity sensor

//Moisture sensor values
#define moisturePin A0
int dryValue = 177;
int wetValue = 520;
int percentage;

int moisture;

//Temp and humidity values
int temperature;
int humidity;

///Buzzer
//int buzzer = 13;

//Buttons
int tempH = 2;
int moistureBtn = 3;

//Button ISR booleans
volatile bool tempHMenu = false;
volatile bool moistureMenu = false;

void tempHISR() {
  tempHMenu = true;
  moistureMenu = false;
}

void moistureISR() {
  moistureMenu = true;
  tempHMenu = false;
}

void setup() {
  // Initialize serial communication to allow debugging and data readout.
  // Using a baud rate of 9600 bps.
  Serial.begin(9600);

  //pinMode(buzzer, OUTPUT);
  pinMode(tempH, INPUT_PULLUP);
  pinMode(moistureBtn, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(tempH), tempHISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(moistureBtn), moistureISR, FALLING);

  //Declare data headers
  Serial.print("Temperature(C), ");
  Serial.print("Humidity(%), ");
  Serial.print("Moisture(%)");
  Serial.println();
}

void loop() {
  //Allow data to be fed in
  soilMoisture();
  tempHumidity();

  //Serial readings of data from sensors
  Serial.print(temperature); //Tempearture
  Serial.print(" ");
  Serial.print(humidity); //Humidity
  Serial.print(" ");
  Serial.print(percentage); //Moisture
  Serial.print(" ");
  Serial.println();
  
  //Where the LCD code lives
  buttonHandler();
  delay(1000);
}

//Logic functions for Temprature, Humidity and Soil Moisture
void soilMoisture() {
  moisture = analogRead(moisturePin);
  percentage = map(moisture, dryValue, wetValue, 0, 100);
}

void tempHumidity() {
  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht11.readTemperatureHumidity(temperature, humidity);
}

//Print functions for Temprature, Humidity and Soil Moisture
void printTH() {
  //LCD Code will go here
}

void printMoisture() {
  //LCD Code will go here
}

void buttonHandler() {
  if (tempHMenu) {
    Serial.println("Temperature and humidity button pressed");
    //printTH();     // Show temperature and humidity
  } else if (moistureMenu) {
    Serial.println("Soil moisture button pressed");
    //printMoisture();  // Show soil moisture only
  }
}