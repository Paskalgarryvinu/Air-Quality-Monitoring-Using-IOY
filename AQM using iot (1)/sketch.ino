#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

#define DHT_PIN 15        // GPIO15 for DHT11 DATA
#define MQ135_PIN 34      // GPIO34 for Potentiometer OUT
#define I2C_ADDR 0x27     // I2C address for the LCD
#define LCD_COLUMNS 16    // 16 columns for 16x2 LCD
#define LCD_LINES 2       // 2 rows for 16x2 LCD

DHTesp dhtSensor;
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize DHT11 sensor
  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Air Quality Mon");
  delay(2000); // Display the initial message for 2 seconds
  lcd.clear();
}

void loop() {
  // Read temperature and humidity from DHT11
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temperature = data.temperature;
  float humidity = data.humidity;

  // Read air quality value from potentiometer
  int airQualityRaw = analogRead(MQ135_PIN);
  float airQualityPPM = map(airQualityRaw, 0, 4095, 0, 500); // Map to 0-500 PPM range

  // Determine air quality status
  String airQualityStatus;
  if (airQualityPPM < 100) {
    airQualityStatus = "Good";
  } else if (airQualityPPM < 200) {
    airQualityStatus = "Moderate";
  } else {
    airQualityStatus = "Poor";
  }

  // Print values to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Air Quality: ");
  Serial.print(airQualityPPM);
  Serial.println(" PPM");
  Serial.print("Status: ");
  Serial.println(airQualityStatus);
  Serial.println("-----------------------");

  // Display values on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print((char)223); // Degree symbol
  lcd.print("C Hum:");
  lcd.print(humidity, 1);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Air: ");
  lcd.print(airQualityPPM, 1);
  lcd.print("PPM ");
  lcd.print(airQualityStatus);

  delay(2000); // Update every 2 seconds
}