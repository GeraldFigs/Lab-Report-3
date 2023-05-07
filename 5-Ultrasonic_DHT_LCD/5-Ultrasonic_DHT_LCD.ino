/* GOAL: This code demonstrates the integration of HC-SR04 Ultrasonic Range Finder enhanced with a DHT22 Temp. and Humidity sensor.
It displays the results namely the temperature in Celsius, speed of sound, humidity, and the distance
on an LCD as well as on the serial monitor.

MAIN THEME: use digital input (button press) and analog input (Ultrasonic Sensor) combined with (DHT Sensor) to control digital output (LCD display).
By combining both sensors, they can provide more accurate distance measurements.
The DHT11 measures temperature and humidity, which can be used to calculate the speed of sound.
Since the speed of sound varies with temperature and humidity, this information helps improve the accuracy of distance calculations made by the HC-SR04 ultrasonic sensor.
*/

// Include necessary libraries
#include <LiquidCrystal.h> // For LCD display
// Dependant upon Adafruit_Sensors Library
#include "DHT.h";
// Include NewPing Library
#include "NewPing.h"

// Define Constants

#define DHTPIN 7         // DHT-22 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 11
#define TRIGGER_PIN  10 // HC-SR04 Trigger pin
#define ECHO_PIN     13 // HC-SR04 Echo pin
#define MAX_DISTANCE 400 // HC-SR04 max distance in cm

// Create a NewPing object
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Define Variables
float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius
float duration; // Stores HC-SR04 pulse duration value
float distance; // Stores calculated distance in cm
float soundsp;  // Stores calculated speed of sound in M/S
float soundcm;  // Stores calculated speed of sound in cm/ms
int iterations = 5;

// LCD pin definitions
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// Initialize LCD display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Push button and debounce initialization
const int PB = 8;         //push button input pin
int buttonState = 0;      //the state of the push button
int lastButtonState = 0;  //the last state of push button (lastDebounceTime)
int currentPage = 1;      //the current page on the LCD display
unsigned long debounceDelay = 50;   //add a small delay for debounce


// Initialize DHT sensor for normal 16mhz Arduino

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  pinMode (PB, INPUT_PULLUP); // Set the push button (PB) as an input with internal pull-up
  Serial.begin (9600);  // Initialize Serial communication at 9600 baud
  dht.begin();  // Initialize the DHT sensor
  lcd.begin(16, 2); // Set up the LCD's number of columns and rows
}

void loop()
{
  //ButtonState
  int buttonState = digitalRead(PB); // Read the push button (PB) state
  
  Serial.print("ButState = "); // Print string "ButState" to Serial Monitor
  Serial.println(buttonState); // Print the value of 'buttonState' to the Serial Monitor

  delay(1000);  // Delay so DHT-22 sensor can stabalize

  hum = dht.readHumidity();  // Get Humidity value
  temp = dht.readTemperature(); // Get Temperature value

  // Calculate the Speed of Sound in M/S
  soundsp = 331.4 + (0.606 * temp) + (0.0124 * hum);

  // Convert to cm/ms
  soundcm = soundsp / 10000;  

  // Get the median ping value (in microseconds) from the HC-SR04 ultrasonic sensor over 'iterations' number of readings
  duration = sonar.ping_median(iterations); 

  // Calculate the distance
  distance = (duration / 2) * soundcm;

  // Send results to Serial Monitor
  Serial.print("Sound: "); // Print "Sound: " to the Serial Monitor
  Serial.print(soundsp); // Print the speed of sound value to the Serial Monitor
  Serial.print(" m/s, "); // Print " m/s, " to the Serial Monitor
  Serial.print("Humid: "); // Print "Humid: " to the Serial Monitor
  Serial.print(hum); // Print the humidity value to the Serial Monitor
  Serial.print(" %, Temp: "); // Print " %, Temp: " to the Serial Monitor
  Serial.print(temp); // Print the temperature value to the Serial Monitor
  Serial.print(" C, "); // Print " C, " to the Serial Monitor
  Serial.print("Distance: "); // Print "Distance: " to the Serial Monitor

  if (distance >= 400 || distance <= 2) { // Check if the distance is out of range (either below 2 cm or above 400 cm)
    Serial.print("Out of range"); // Print "Out of range" to the Serial Monitor
  }
  else {
    Serial.print(distance); // Print the distance value to the Serial Monitor
    Serial.print(" cm");  // Print " cm" to the Serial Monitor
  }

  Serial.println(" ");  // Print a newline to the Serial Monitor, moving the cursor to the next line

// Check if button state has changed
if (buttonState != lastButtonState){     // Compare whether PB state has changed
    if (buttonState == LOW){               // If PB is pressed
      currentPage++;                      // Next page (increment +1)
       if (currentPage > 4){              // If the current page is greater than 4
         currentPage = 1;                 // Reset back to page 1
       }                     
 }
 lastButtonState = buttonState;           //Update the last button state to the current button state
 delay(debounceDelay);                    //Apply a very small delay for debounce
}

lcd.clear();  // Clear LCD display

// Display the appropriate values based on the current page on LCD
  switch (currentPage) {  // Use a switch statement to determine which value to display
    case 1: { // Display temperature on page 1
      lcd.setCursor(0, 0); // Set cursor to the first row, first column
      lcd.print("Temperature:"); // Print "Temperature:"
      lcd.setCursor(0, 1); // Set cursor to the second row, first column
      lcd.print(temp); // Print the temperature value
      lcd.print(char(223)); // Print the degree symbol
      lcd.print(F("C")); // Print "C" for Celsius
      break; // Exit the switch statement
    }

    case 2: { // Display speed of sound on page 2
      lcd.setCursor(0, 0); // Set cursor to the first row, first column
      lcd.print("Speed of Sound:"); // Print "Speed of Sound:"
      lcd.setCursor(0,1); // Set cursor to the second row, first column
      lcd.print(soundsp); // Print the speed of sound value
      lcd.print(" m/s"); // Print " m/s" for meters per second
      break; // Exit the switch statement
    }
    case 3: {  // Display humidity on page 3
      lcd.setCursor(0, 0); // Set cursor to the first row, first column
      lcd.print("Humidity:"); // Print "Humidity:"
      lcd.setCursor(0, 1); // Set cursor to the second row, first column
      lcd.print(hum); // Print the humidity value
      lcd.print("%"); // Print the percentage symbol (%)
      break; // Exit the switch statement
    }
    
    case 4: { // Display distance on page 4
      lcd.setCursor(0, 0); // Set cursor to the first row, first column
      lcd.print("Distance:"); // Print "Distance:"
    if (distance >= 400 || distance <= 2) { // If the distance is out of range (either below 2 cm or above 400 cm)
      lcd.setCursor(0, 1); // Set cursor to the second row, first column
      lcd.print("Out of range"); // Print "Out of range"
      break; // Exit the switch statement
    } else { // If the distance is within range
      lcd.setCursor(0, 1); // Set cursor to the second row, first column
      lcd.print(distance); // Print the distance value
      lcd.print(" cm"); // Print " cm" for centimeters
    }
    break; // Exit the switch statement 
  }
  }
}