#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/12dc938c-51e9-40d0-b2ec-f4a45c154c80 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  CloudTemperatureSensor temperature;
  CloudLength altitude;
  CloudRelativeHumidity humidity;
  CloudPressure pressure;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <DFRobot_BME280.h>
#include <ArduinoSlack.h>
#include <ArduinoJson.h>

// Configure Slack
#define SLACK_SSL_FINGERPRINT "AC 95 5A 58 B8 4E 0B CD B3 97 D2 88 68 F5 CA C1 0A 81 E3 6E"
#define SLACK_ACCESS_TOKEN ""

// Setup BME280
typedef DFRobot_BME280_IIC    BME;
BME   bme(&Wire, 0x77);   // select TwoWire peripheral and set sensor address
#define SEA_LEVEL_PRESSURE    1010.0f

// Setup Slack
int status = WL_IDLE_STATUS;

WiFiSSLClient client;
ArduinoSlack slack(client, SLACK_ACCESS_TOKEN);

unsigned long delayBetweenRequests = 30000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due
bool firstStatus = true;


void printLastOperateStatus(BME::eStatus_t eStatus)
{
  switch(eStatus) {
  case BME::eStatusOK:    Serial.println("everything ok"); break;
  case BME::eStatusErr:   Serial.println("unknow error"); break;
  case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
  case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
  default: Serial.println("unknow status"); break;
  }
}

// Print Slack profile
void displayProfile(SlackProfile profile)
{
    if (!profile.error)
    {
        Serial.println("--------- Profile ---------");

        Serial.print("Display Name: ");
        Serial.println(profile.displayName);

        Serial.print("Status Text: ");
        Serial.println(profile.statusText);

        Serial.print("Status Emoji: ");
        Serial.println(profile.statusEmoji);

        Serial.print("Status Expiration: ");
        Serial.println(profile.statusExpiration);

        Serial.println("------------------------");
    }
    else
    {
        Serial.println("error getting profile");
    }
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Serial.begin(115200);
  bme.reset();
  Serial.println("bme read data test");
  while(bme.begin() != BME::eStatusOK) {
    Serial.println("bme begin faild");
    printLastOperateStatus(bme.lastOperateStatus);
    delay(2000);
  }
  Serial.println("bme begin success");
  delay(100);
  
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
  float   temp = bme.getTemperature();
  uint32_t    press = bme.getPressure();
  float   alti = bme.calAltitude(SEA_LEVEL_PRESSURE, press);
  float   humi = bme.getHumidity();
  
  temperature = temp;
  humidity = humi;
  altitude = alti;
  pressure = press;
  
  Serial.println();
  Serial.println("======== start print ========");
  Serial.print("temperature (unit Celsius): "); Serial.println(temp);
  Serial.print("pressure (unit pa):         "); Serial.println(press);
  Serial.print("altitude (unit meter):      "); Serial.println(alti);
  Serial.print("humidity (unit percent):    "); Serial.println(humi);
  Serial.println("========  end print  ========");

  delay(1000);

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(40);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(40);
  
  if (millis() > requestDueTime)
    {
        SlackProfile profile;
        if (temp > 28)
        {
            profile = slack.setCustomStatus("I am feeling hot", ":sweat:");
        }
        else
        {
            profile = slack.setCustomStatus("I am feeling ok", ":simple_smile:");
        }
        firstStatus = !firstStatus;
        displayProfile(profile);
        requestDueTime = millis() + delayBetweenRequests;
    }
  
}

/*
  Since Altitude is READ_WRITE variable, onAltitudeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onAltitudeChange()  {
  // Add your code here to act upon Altitude change
}
/*
  Since Humidity is READ_WRITE variable, onHumidityChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onHumidityChange()  {
  // Add your code here to act upon Humidity change
  
}
/*
  Since Pressure is READ_WRITE variable, onPressureChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onPressureChange()  {
  // Add your code here to act upon Pressure change
}

