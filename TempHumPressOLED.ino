/**********************************
   Display temperature, humidity and
   pressure. Use a DHT22 sensor to 
   acquire data about room relative
   humidity and temperature. Use a 
   BMP180 sensor to acquire pressure
   data. Display the values on a 
   0.96" OLED screen an through
   serial communication.
***********************************/

// Libraries
#include <Adafruit_SSD1306.h>
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
#define ALT 125.0 // My altitude in meters

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
  {
    Serial.println(F("BMP180 init success"));
    Serial.print(F("Altitude set at "));
    Serial.print(ALT);
    Serial.println(F(" m above sea level"));
    Serial.println(); // blank line
    
  }
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println(F("BMP180 init fail"));
    while(1); // Pause forever

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("BMP180 init fail"));
  }

}

void loop() {

// Variables
char status;
double T, P, H; // temperature, pressure, relative humidity

  // Pressure measurements
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
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
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
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALT = current altitude in m.
          // Result: P = sea-level compensated pressure in mb

          P = pressure.sealevel(P,ALT);
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");
        }
        else Serial.println(F("error retrieving pressure measurement\n"));
      }
      else Serial.println(F("error starting pressure measurement\n"));
    }
    else Serial.println(F("error retrieving temperature measurement\n"));
  }
  else Serial.println(F("error starting temperature measurement\n"));
  
  // Serial print pressure
  Serial.print("Pressure: ");
  Serial.print(P,1);
  Serial.print(" mb\t");

  // Read humidity and temperature
  H = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(H,1);
  Serial.print("%\t\t");

  T = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(T,1);
  Serial.println("°C");
  Serial.println();             // blank line

  // Display values on screen
  display.clearDisplay();       // avoid "noise" on the display due to serial print commands (unknown cause)
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
  display.print(H,1);
  display.println("%");
  display.display();

  delay(10000);
  display.clearDisplay();

}
