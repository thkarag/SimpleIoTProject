// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)

void onAltitudeChange();
void onHumidityChange();
void onPressureChange();

CloudTemperatureSensor temperature;
CloudLength altitude;
CloudRelativeHumidity humidity;
CloudPressure pressure;

void initProperties(){

  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(altitude, READWRITE, ON_CHANGE, onAltitudeChange);
  ArduinoCloud.addProperty(humidity, READWRITE, ON_CHANGE, onHumidityChange);
  ArduinoCloud.addProperty(pressure, READWRITE, ON_CHANGE, onPressureChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
