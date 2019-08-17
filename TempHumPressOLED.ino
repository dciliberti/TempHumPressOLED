/**********************************
   Temperature, humidity and pressure
   display. Use a DHT22 sensor to 
   acquire data about room relative
   humidity and temperature. Use a 
   BMP180 sensor to acquire pressure
   data. Display the values on a 
   0.96" OLED screen.
***********************************/

// Libraries
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <OLED_I2C.h>
#include <DHT.h>
#include <SFE_BMP180.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(2, DHT22);

// Create the SFE_BMP180 object
SFE_BMP180 pressure;
// #define ALTITUDE 50.0 // My altitude in meters

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen
  display.display();
  delay(2000);

  // Clear the buffer
  display.clearDisplay();

  // Initialize the temperature sensor
  dht.begin();

  Serial.println(F("REBOOT"));
  // Initialize the pressure sensor (it is important to get calibration values stored on the device).
  if (pressure.begin())
    Serial.println(F("BMP180 init success"));
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println(F("BMP180 init fail\n\n"));
    while(1); // Pause forever

//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.setCursor(0, 0);
//    display.println("BMP180 init fail");
  }

}

void loop() {

// Variables
float hum;        // Stores humidity value
// float temp;       // Stores temperature value
// float humidex;    // Stores humidex index value
char status;
double T, P;

  // Pressure measurements

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
//  Serial.println();
//  Serial.print("provided altitude: ");
//  Serial.print(ALTITUDE,0);
//  Serial.print(" meters, ");
//  Serial.print(ALTITUDE*3.28084,0);
//  Serial.println(" feet");
//  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
//      // Print out the measurement:
//      Serial.print("temperature: ");
//      Serial.print(T,2);
//      Serial.print(" deg C, ");
//      Serial.print((9.0/5.0)*T+32.0,2);
//      Serial.println(" deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
//          Serial.print("absolute pressure: ");
//          Serial.print(P,2);
//          Serial.print(" mb, ");
//          Serial.print(P*0.0295333727,2);
//          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: P = sea-level compensated pressure in mb

          P = pressure.sealevel(P,50); // we're at 50 meters (San Paolo Bel Sito, NA)
//          Serial.print("relative (sea-level) pressure: ");
//          Serial.print(P,2);
//          Serial.print(" mb, ");
//          Serial.print(P*0.0295333727,2);
//          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, P = baseline pressure in mb.
          // Result: a = altitude in m.

//          a = pressure.altitude(P,P);
//          Serial.print("computed altitude: ");
//          Serial.print(a,0);
//          Serial.print(" meters, ");
//          Serial.print(a*3.28084,0);
//          Serial.println(" feet");
        }
        else Serial.println(F("error retrieving pressure measurement\n"));
      }
      else Serial.println(F("error starting pressure measurement\n"));
    }
    else Serial.println(F("error retrieving temperature measurement\n"));
  }
  else Serial.println(F("error starting temperature measurement\n"));
  

  // Read humidity and temperature
  hum = dht.readHumidity();
//  Serial.print("Humidity: ");
//  Serial.print(hum,1);
//  Serial.print("%\t");

  T = dht.readTemperature();
//  Serial.print("Temperature: ");
//  Serial.print(T,1);
//  Serial.print("°C\t");

  // Display values on screen
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Room data");

  display.print("P:");
  display.print(P,1);
  display.println("mb");

  display.print("Tmp: ");
  display.print(T,1);
  display.println("C");

  display.print("Hum: ");
  display.print(hum,1);
  display.println("%");
  display.display();

//  // Calculates humidex index and display its status
//  humidex = temp + (0.5555 * (0.06 * hum * pow(10,0.03*temp) -10));
//  Serial.print("Humidex: ");
//  Serial.print(humidex);
//  Serial.print("\t");
//
//  // Humidex category
//  if (humidex < 20){
//    Serial.println("No index");
//    display.println("No index");
//  }
//  else if (humidex >= 20 && humidex < 27){
//    Serial.println("Comfort");
//    display.println("Comfort");
//  }
//  else if (humidex >= 27 && humidex < 30){
//    Serial.println("Caution");
//    display.println("Caution");
//  }
//  else if (humidex >= 30 && humidex < 40){
//    Serial.println("Extreme caution");
//    display.println("CAUTION");
//  }
//  else if (humidex >= 40 && humidex < 55){
//    Serial.println("Danger");
//    display.println("Danger");
//  }
//  else {
//    Serial.println("Extreme danger");
//    display.println("DANGER");
//  }
//
//  display.display();

  delay(10000);
  display.clearDisplay();

}
