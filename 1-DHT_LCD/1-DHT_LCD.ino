/*GOAL OF THE PROGRAM: Monitoring and displaying temperature and humidity values from a DHT11 sensor on a 16x2 LCD screen, with additional information available through a serial monitor.

MAIN THEME: This code utilizes a DHT11 sensor to read temperature in Celsius and humidity values and displays them on a 16x2 LCD screen.
The code includes necessary libraries for LCD control and sensor support, and it initializes the intput DHT sensor, reads the temperature and humidity values, and displays them on the LCD screen as a monitor output.
It also provides detailed sensor information through a serial monitor.*/

//include necessary libraries
#include <LiquidCrystal.h>  //include the LiquidCrystal library for LCD control
#include <Adafruit_Sensor.h>  //include the Adafruit_Sensor library for sensor support
#include <DHT.h> //include the DHT library for DHT sensor
#include <DHT_U.h>  //include the DHT_U library for DHT sensor unified support

//define DHT sensor pin and type
#define DHTPIN 7     //digital pin 7 connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT type 11

//define LCD pin connections
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//initialize LiquidCrystal library with pin connections
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DHT_Unified dht(DHTPIN, DHTTYPE); //create DHT_Unified instance with sensor pin and type

uint32_t delayMS; //define delay between sensor readings

void setup() {

//configure LCD contrast pin and set up the LCD screen
  lcd.begin(16, 2); //set up the LCD's number of columns and rows:
  Serial.begin(9600); //commence serial monitor at 9600 baud rate

//Initialize DHT sensor 
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
}
void loop() {
//SENSOR CODE
  // Delay between measurements.
  delay(delayMS);
  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

//LCD CODE
  lcd.clear();    //clear the LCD screen

  //Temperature
  dht.temperature().getEvent(&event); // Get temperature event
  if (isnan(event.temperature)) {
    lcd.setCursor(0, 0);    //set the cursor position to the beginning of the first row
    lcd.print(F("Error reading temperature!"));
  }
  else {
    lcd.setCursor(0, 0);    //set the cursor position to the beginning of the first row
    lcd.print(F("Temp: ")); //print the label for temperature
    lcd.print(event.temperature);   //print the temperature value in Celsius
    lcd.print(char(223));  //print the degree symbol
    lcd.print(F("C"));   //print the unit 'C' for Celsius
  }
  //Humidity
  dht.humidity().getEvent(&event);  // Get humidity event
  if (isnan(event.relative_humidity)) {
    lcd.setCursor(0, 1);     //set the cursor position to the beginning of the second row
    lcd.print(F("Error reading humidity!"));
  }
  else {
    lcd.setCursor(0, 1);      //set the cursor position to the beginning of the second row
    lcd.print(F("Humidity: "));  //print the label for humidity
    lcd.print(event.relative_humidity);   //print the humidity value as a percentage
    lcd.print(F("%"));    //print the percentage symbol
  }
}