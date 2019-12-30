# TempHumPressOLED
Collect temperature, humidity and pressure data with and Arduino Nano (or Uno). Use a DHT22 sensor to acquire data about room relative humidity and temperature. Use a  BMP180 sensor to acquire pressure data. Display the values on a 0.96" OLED screen and through serial communication.

Just write your altitude at line 37 to correct the barometric pressure.

The development of this device is a step for the realization of a weather station.

## Example output on the serial port
```
REBOOT
BMP180 init success
Altitude set at 50.00 m above sea level

temperature: 17.84 deg C, 64.11 deg F
absolute pressure: 1018.04 mb, 30.07 inHg
relative (sea-level) pressure: 1024.10 mb, 30.25 inHg
Pressure: 1024.1 mb	Humidity: 47.0%		Temperature: 18.4°C

temperature: 17.84 deg C, 64.11 deg F
absolute pressure: 1017.99 mb, 30.06 inHg
relative (sea-level) pressure: 1024.05 mb, 30.24 inHg
Pressure: 1024.0 mb	Humidity: 46.5%		Temperature: 18.4°C

temperature: 17.83 deg C, 64.10 deg F
absolute pressure: 1017.97 mb, 30.06 inHg
relative (sea-level) pressure: 1024.02 mb, 30.24 inHg
Pressure: 1024.0 mb	Humidity: 46.5%		Temperature: 18.4°C
```
