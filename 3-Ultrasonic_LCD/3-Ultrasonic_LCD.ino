/*GOAL OF THE PROGRAM: Ultrasonic distance sensor using an HC-SR04 module and LCD display to show measured distance in centimeters.

MAIN THEME:
The main theme of this code is the use of digital input (button press) and analog input (DHT sensor) to control digital output (LCD display).
The HC-SR04 sensor sends out an ultrasonic pulse and measures the time it takes for the pulse to bounce off an object and return to the sensor.
The code then calculates the distance based on the time it takes for the pulse to travel back and forth.
The loop function then repeats, continuously measuring the distance and updating the display.*/

#define trigPin 10          //define trigPin as pin 10 on the Arduino board, connected to the trigger pin of the HC-SR04 module.
#define echoPin 13          //define echoPin as pin 13 on the Arduino board, connected to the echo pin of the HC-SR04 module.
#include <LiquidCrystal.h>  //include the LiquidCrystal library for the LCD display.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;     //define the pin connections for the LCD display.
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                      //create an lcd object using the defined pin connections.

float duration, distance;                      //declare variables for storing the duration of the echo pulse and the calculated distance.

void setup() {                                 //put your setup code here, to run once:
Serial.begin(9600);                        //initialize serial communication at 9600 baud.
pinMode(trigPin, OUTPUT);                           //set the trigPin as an OUTPUT pin.
pinMode(echoPin, INPUT);                           //set the echoPin as an INPUT pin.
  lcd.begin(16, 2);                          //initialize the LCD with 16 columns and 2 rows.
}

void loop() {                                 // put your main code here, to run repeatedly:

//send a 20 micro seconds pulse to the trigPin, thus "triggering" the ultrasonic sensor.
digitalWrite(trigPin, LOW);                   //set the trigger pin to LOW before sending the pulse
delayMicroseconds(2);                         //wait for 2 microseconds before sending the pulse
digitalWrite(trigPin, HIGH);                  //set the trigger pin to HIGH to send the pulse       
delayMicroseconds(20);                        //keep the trigger pin HIGH for 20 microseconds (duration of the pulse)        
digitalWrite(trigPin, LOW);                   //set the trigger pin back to LOW after sending the pulse

duration = pulseIn(echoPin, HIGH);            //measure the duration of the echo pulse in microseconds. This duration represents the time it takes for the ultrasonic pulse to travel to the object and back to the sensor.
distance = (duration /2 ) * 0.0343;           //calculate the distance in centimeters. The division by 2 accounts for the round-trip time of the pulse (from the sensor to the object and back), 
//and 0.0343 is the conversion factor from microseconds to centimeters (since the speed of sound in air is approximately 343 meters per second).

Serial.print("Distance: ");                   //print the distance to the serial monitor.            
if (distance >= 400 || distance <= 2) {       //the valid range of the HC-SR04 sensor is 2 cm to 400 cm
  Serial.println("Out of Range");             //if the distance is out of that said range, print "Out of Range" instead. 
} else {
  Serial.print(distance);                     //otherwise, proceed to print the value of distance
  Serial.println(" cm");                      //print the unit "cm" to identify the value of distance printed
  delay(500);                                 //wait for 500 milliseconds before updating the display
}

lcd.clear();                                //clear the LCD display before showing new data   
lcd.setCursor(0,0);                         //set the cursor to the beginning of the first row
lcd.print("Distance: ");                    //display the distance on the LCD. If the distance is out of range, display "Out of Range" instead.
if (distance >= 400 || distance <= 2) {     //check if the measured distance is within the valid range (2cm to 400cm)
  lcd.setCursor(0,1);                       //set the cursor to the beginning of the second row
  lcd.print("Out of Range");                //print "Out of Range" on the second row if the distance is not within the valid range
} else {
  lcd.setCursor(0,1);                       //set the cursor to the beginning of the second row
  lcd.print(distance);                      //print the measured distance on the second row
  lcd.print(" cm");                         //print the unit "cm" next to the distance value
}
}