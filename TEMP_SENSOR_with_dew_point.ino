#include "DHT.h"
#include "Adafruit_Sensor.h"

#define DHTPIN 2 // Could use any other pin when needed
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT22 initialized!");

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  
  double dewPoint(double celsius, double humidity)
{
  // (STEP 1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (STEP 2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}


  // reading temperature or humidity takes 250 milliseconds (The DHT22 is a very slow sensor!)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // to check if any readings failed, exit and try again
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hi = dht.computeHeatIndex(f, h);
  float hiDegC = dht.convertFtoC(hi);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature 1: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Temperature 2: "); // 2 readings each are taken for averaging later, creating more accurate results
  Serial.print(hiDegC);
  Serial.print(" *C ");
  Serial.print(hi);
  Serial.print(" *F ");
  Serial.print("Dew Point (*C): ");
  Serial.println(dewPoint(t, h));
}

