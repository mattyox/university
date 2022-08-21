// Including necessary libraries
#include <LiquidCrystal.h> // Library for the LCD
#include <OneWire.h> // Library for the 1-Wire bus
#include <DallasTemperature.h> // Library for the DS18B20 temperature sensor

// Defining the LCD pin connections
#define RS_PIN 2  // Register Select pin
#define E_PIN 4   // Enable pin
#define D4_PIN 8  // Data 4 pin
#define D5_PIN 9  // Data 5 pin
#define D6_PIN 10 // Data 6 pin
#define D7_PIN 11 // Data 7 pin

// Defining the LCD dimensions
#define COLUMNS 16
#define ROWS 2

// Defining, where the data pin of 1-Wire bus is connected
#define ONE_WIRE_BUS 7

// Defining temperature sensor precision
// 9 - 0,5 [°C]
// 10 - 0,25 [°C]
// 11 - 0,125 [°C]
// 12 - 0,0625 [°C]
#define TEMPERATURE_PRECISION 11

// Defining the number of decimal places to be displayed on LCD
#define DECIMAL_PLACES 1

// Setting up a LCD instance
LiquidCrystal lcd(RS_PIN, E_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Setting up a oneWire instance to communicate
OneWire oneWireDevice(ONE_WIRE_BUS);

// Passing oneWire reference to Dallas Temperature
DallasTemperature thermometer(&oneWireDevice);

// Variable for storing the measured temperature value
double temperature;

// Variable for the created degree character
char degree_char;

void setup()
{
  // Initialising the LCD
  lcd.begin(COLUMNS, ROWS);

  // Assigning the bit values of degree character
  byte degree_bits[8] =
  {
    0b01110,
    0b10001,
    0b10001,
    0b10001,
    0b01110,
    0b00000,
    0b00000,
    0b00000
  };

  // Creating the degree character
  lcd.createChar(0, degree_bits);

  // Assigning the degree character
  degree_char = byte(0);

  // Initialising the temperature sensor
  thermometer.begin();

  // Setting the temperature sensor resolution
  thermometer.setResolution(TEMPERATURE_PRECISION);
}

void loop()
{
  // Measuring the temperature
  thermometer.requestTemperatures();

  // Getting the measured temperature in degrees Celsius
  temperature = thermometer.getTempCByIndex(0);

  // Checking the result of measurement
  if (temperature != DEVICE_DISCONNECTED_C)
  {
    // Printing the measured temperature
    lcd.clear();
    lcd.print("Temperature:");
    lcd.setCursor(0, 1);
    lcd.print(String(temperature, DECIMAL_PLACES) + " " + degree_char + "C");
  }
  else
  {
    // Printing error message on the LCD
    lcd.clear();
    lcd.print("Could not read");
    lcd.setCursor(0, 1);
    lcd.print("the temperature");
  }

  // Additional delay 5 [ms]
  delay(5000);
}
