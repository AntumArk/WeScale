#include <Arduino.h>

//
// This sketch does not use the ALARM registers and uses those 2 bytes as a counter
// these 2 bytes can be used for other purposes as well e.g. last temperature or
// a specific ID.
//

#include <OneWire.h>
#include <DallasTemperature.h>

#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;

// 2. Adjustment settings
const long LOADCELL_OFFSET = 46441;//2461 100.01 g weight. 
const float LOADCELL_DIVIDER = -11.4;//-18.18/2;

HX711 scale;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS D5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int count = 0;
#define MCU_DONE_PIN D6
void sleepRoutine(){
  for(int i=0;i<10;i++)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN,LOW);
    delay(300);
  }

  digitalWrite(MCU_DONE_PIN,HIGH);
  delay(1);
  digitalWrite(MCU_DONE_PIN,LOW);
}
void setup(void)
{
  pinMode(D0,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(MCU_DONE_PIN,OUTPUT);
  digitalWrite(MCU_DONE_PIN,LOW);

  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  
  sleepRoutine();
  // Start up the library
  sensors.begin();

  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN,64);
  scale.set_scale(LOADCELL_DIVIDER);
  scale.set_offset(LOADCELL_OFFSET);

}

void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));

  count++;
  sensors.setUserDataByIndex(0, count);
  int x = sensors.getUserDataByIndex(0);
  Serial.println(count);


    if (scale.wait_ready_timeout(1000)) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
    // 4. Acquire reading
    Serial.print("Weight: ");
    Serial.println(scale.get_units(10), 2);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1500);
}
