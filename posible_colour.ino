#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdint.h>
LiquidCrystal_I2C lcd(0x27,20,4);

//servo
#include <Servo.h>
Servo SERR;
int pos = 0;
const int AD = 10;

// S0, S1 control sensitivity
#define SENSOR_0 4
#define SENSOR_1 5
// S2, S3 control colour choice
#define SENSOR_2 6
#define SENSOR_3 7
#define SENSOR_OUT 8

// Our colours
#define SENSOR_RED      LOW, LOW
#define SENSOR_GREEN    HIGH, HIGH
#define SENSOR_BLUE     LOW, HIGH

// Frequency scaling
#define SENSOR_SCALE_20 HIGH, LOW

// initialise the sensor
void sensor_init() {
  // S0 through S3 are output pins
  pinMode(SENSOR_0, OUTPUT);
  pinMode(SENSOR_1, OUTPUT);
  pinMode(SENSOR_2, OUTPUT);
  pinMode(SENSOR_3, OUTPUT);

  pinMode(SENSOR_OUT, INPUT);

  // freq scale to 20%
  digitalWrite(SENSOR_0, HIGH);
  digitalWrite(SENSOR_1, LOW);

  return;
}

// setup the sensor for a colour
void sensor_set_colour(int s2, int s3) {
  digitalWrite(SENSOR_2, s2);
  digitalWrite(SENSOR_3, s3);
  
  return;
}

// read an input colour
int sensor_read() {
  return pulseIn(SENSOR_OUT, LOW);
}

// delay the needed time between sensor reads to prevent overlap if we're too speedy
void sensor_delay() {
  // delay in milis, change if needed
  delay(100);
  return;
}

// c is a letter corresponding to r, g, or b.
// uint8_t is an unsigned 8-bit int (our colours are 0-255, 2^8=256)
uint8_t sensor_get_colour(char c) {
  // modify the map values to fine tune for each colour if needed.
  // you can call sensor_set_colour() and sensor_read() individually, this is a helper function.
  switch (c) {
    case 'r': {
      sensor_set_colour(SENSOR_RED);     //34,93,255
      return 255 - ((uint8_t) map(sensor_read(), 35, 93, 255, 0));
    }
    case 'g': {
      sensor_set_colour(SENSOR_GREEN);
      return 255 - ((uint8_t) map(sensor_read(), 30, 99, 255, 0));
    }
    case 'b': {                           // 20,84,255
      sensor_set_colour(SENSOR_BLUE);
      return 255 - ((uint8_t) map(sensor_read(), 20, 114, 255, 0));
    }
    default: {
      // if the colour is undefined return zero, which would be unlikely to return naturally.
      Serial.println("Tried to get unknown colour:");
      Serial.print(c);
      return 0;
    }
  }
}

void sensor_print(uint8_t red, uint8_t green, uint8_t blue) {
  Serial.print("Red: ");
  Serial.print(red);

  Serial.print("\tGreen: ");
  Serial.print(green);

  Serial.print("\tBlue: ");
  Serial.print(blue);

  // Create a new line
  Serial.println("");
}

void setup() {
  // set our sensor pin modes and stuff
  sensor_init();

  // init serial comms
  Serial.begin(9600);
}

void loop() {
  // Read each colour sequentially

  uint8_t red = sensor_get_colour('r');
  sensor_delay();
  
  uint8_t blue = sensor_get_colour('b');
  sensor_delay();

  sensor_print(red, 0, blue);

  if (red > blue) {
    Serial.println("Red!");
  } else {
    Serial.println("Blue!");
  }
}

//void setup() {
//  lcd.init();                       
//  lcd.backlight();
// 
//  //servo
//  SERR.attach(AD);
//  
//  // Setting the outputs
//  pinMode(S0, OUTPUT);
//  pinMode(S1, OUTPUT);
//  pinMode(S2, OUTPUT);
//  pinMode(S3, OUTPUT);
//  
//  // Setting the sensorOut as an input
//  pinMode(sensorOut, INPUT);
//  
//  // Setting frequency scaling to 20%
//  digitalWrite(S0,HIGH);
//  digitalWrite(S1,LOW);
//  
//  // Begins serial communication
//  Serial.begin(9600);
//}
//
//void loop() {
//  lcd.setCursor(0, 0);
//  // Setting RED (R) filtered photodiodes to be read
//  digitalWrite(S2,LOW);
//  digitalWrite(S3,LOW);
//  
//  // Reading the output frequency
//  redFrequency = pulseIn(sensorOut, LOW);
//  // Remaping the value of the RED (R) frequency from 0 to 255
//  // You must replace with your own values. Here's an example: 
//  // redColor = map(redFrequency, 70, 120, 255,0);
//  redColor = map(redFrequency, 34, 93, 255,0);
//  
//  // Printing the RED (R) value
//  Serial.print("R = ");
//  Serial.print(redColor);
//  delay(100);
//  
//  // Setting GREEN (G) filtered photodiodes to be read
//  digitalWrite(S2,HIGH);
//  digitalWrite(S3,HIGH);
//  
//  // Reading the output frequency
//  greenFrequency = pulseIn(sensorOut, LOW);
//  // Remaping the value of the GREEN (G) frequency from 0 to 255
//  // You must replace with your own values. Here's an example: 
//  // greenColor = map(greenFrequency, 100, 199, 255, 0);
//  greenColor = map(greenFrequency, 30, 99, 255, 0);
//  
//  // Printing the GREEN (G) value  
//  Serial.print(" G = ");
//  Serial.print(greenColor);
//  delay(100);
// 
//  // Setting BLUE (B) filtered photodiodes to be read
//  digitalWrite(S2,LOW);
//  digitalWrite(S3,HIGH);
//  
//  // Reading the output frequency
//  blueFrequency = pulseIn(sensorOut, LOW);
//  // Remaping the value of the BLUE (B) frequency from 0 to 255
//  // You must replace with your own values. Here's an example: 
//  // blueColor = map(blueFrequency, 38, 84, 255, 0);
//  blueColor = map(blueFrequency, 20, 84, 255, 0);
//  
//  // Printing the BLUE (B) value 
//  Serial.print(" B = ");
//  Serial.println(blueColor);
//  delay(100);
//
//  // Checks the current detected color and prints
//  // a message in the serial monitor
//  if(redColor > greenColor && redColor > blueColor){
//         lcd.clear();
//      lcd.print("RED detected!");
//         for (pos = 90; pos >= 0; pos -= 1) {
//SERR.write(pos);}
//   
//  }
//  if(greenColor > redColor && greenColor > blueColor){
//    lcd.print("GREEN detected!");
//  }
//  if(blueColor > redColor && blueColor > greenColor){
//       lcd.clear();
//    lcd.print("BLUE detected!");
//      for (pos = 0; pos <= 90; pos += 6) {
//SERR.write(pos);}
//
//  }
//}
