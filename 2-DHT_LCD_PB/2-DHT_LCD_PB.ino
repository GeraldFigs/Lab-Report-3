/*GOAL OF THE PROGRAM: This code reads temperature and humidity values from a DHT11 sensor and displays them on a 16x2 LCD screen.

MAIN THEME: it allows user to switch between displaying temperature in Celsius, temperature in Fahrenheit, 
and humidity percentage by pressing a push button. The main theme of this code revolves around digital input 
(push button) and analog input (DHT11 sensor) controlling digital output (LCD screen display).*/

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

//define push button pin, state, and debounce variables
const int PB = 8;         //push button input pin
int buttonState = 0;      //set the initial state of the push button
int lastButtonState = 0;  //store the last state of the push button
int currentPage = 1;      //set the initial page on the LCD display
unsigned long debounceDelay = 50;   //add a small delay for debounce
unsigned long lastDebounceTime = 0;

DHT_Unified dht(DHTPIN, DHTTYPE); //create DHT_Unified instance with sensor pin and type

uint32_t delayMS; //define delay between sensor readings

void setup() {

pinMode (PB, INPUT_PULLUP); //set push button pin as input with internal pull-up resistor

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

}

void loop() {

// Code for debounce, LCD, display Temperature in Celsius, Humidity and Temperature in Fahrenheit

int actualButtonState = digitalRead(PB); //Read the state of the button

//debounce the button to avoid bouncing signals
if (actualButtonState != lastButtonState) {
  lastDebounceTime = millis();
}
if ((millis() - lastDebounceTime) > debounceDelay) {
  //if the button state has changed, and is stable, update buttonState variable
  if (actualButtonState != buttonState) {
    buttonState = actualButtonState;
    //if the button is pressed, cycle through display options
    if (buttonState == HIGH) {
      currentPage++;
      //reset count to 1 after reaching max count
      if(currentPage == 4){
        currentPage = 1;
      }
      //switch case statement to handle different display options
switch(currentPage){
case 1:
//Display temperature in Celsius
 sensors_event_t event;
 dht.temperature().getEvent(&event);// Read the temperature data from DHT11 sensor using temperature() function and store results in "event" variable.
 if (isnan(event.temperature)) { // If temperature event is not a number
            Serial.println(F("Error reading temperature!")); // Print error
          }
          else { // Print temperature event number with Celsius symbol
            lcd.clear();  //clear the LCD screen to refresh
            lcd.setCursor(0,0); //set the cursor position to the beginning of the first row
            lcd.print("Temp: ");  //print the label for temperature
            lcd.print(event.temperature);
            lcd.print(char(223)); //print the degree symbol
            lcd.print("C"); //print the unit 'C' for Celsius
          }
          break;
        case 2:
          //Display humidity
          dht.humidity().getEvent(&event);  // reads the humidity data from the DHT11 sensor using humidity() function and store the result in the "event" variable.
          if (isnan(event.relative_humidity)) { // If humidity event is not a number
            Serial.println(F("Error reading humidity!")); //print error
          }
          else {
            lcd.clear(); //clear the LCD screen to refresh
            lcd.setCursor(0,0); //set the cursor position to the beginning of the first row
            lcd.print("Humidity: ");  //print the label for humidity
            lcd.print(event.relative_humidity); //print the humidity value as a percentage
            lcd.print("%"); // Print percentage symbol
          }
          break;
        case 3:
          //Display temperature in Fahrenheit
          dht.temperature().getEvent(&event);
          if (isnan(event.temperature)) { // If temp event is not a number
            Serial.println(F("Error reading temperature!"));  //print error
          }
          else {  // Conversion from Celsius to Fahrenheit
            float f = (event.temperature * 1.8) + 32; //mathematically convert degrees Celsius to Fahrenheit
            lcd.clear();  //clear the LCD screen to refresh
            lcd.setCursor(0,0); //set the cursor position to the beginning of the first row
            lcd.print("Temp: ");  //print the label for temperature
            lcd.print(f); //print var 'f' which corresponds to the converted temperature to Fahrenheit
            lcd.print(char(223)); //print the degree symbol
            lcd.print("F"); //print the unit 'F' for Fahrenheit
          }
          break;
      }
    }
  }
}
//update lastButtonState to current reading for next cycle
lastButtonState = actualButtonState;
}