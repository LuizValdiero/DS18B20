#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

//config
//#define REWRITE_DEVICE_ID
//PINS
#define LED D5
#define DS18B20_PINS D6
// 
#define MAX_NUMBER_DS18_DEVICES 2


OneWire temperature_bus(DS18B20_PINS);
DallasTemperature ds(&temperature_bus);

DeviceAddress ds_devices[MAX_NUMBER_DS18_DEVICES];

int number_of_ds18_devices = 0;

void print_temperature(DallasTemperature ds, DeviceAddress device);
void print_address(DeviceAddress deviceAddress);

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  ds.begin();
  number_of_ds18_devices = ds.getDS18Count();
  Serial.printf("\n\rNumber of DS18 Family Devices: %d\n\r", number_of_ds18_devices);
  //Serial.print();
  Serial.print("\n\r");

  for (int i = 0; i < number_of_ds18_devices; i++)
  {
    ds.getAddress(ds_devices[i], i);
#ifdef REWRITE_DEVICE_ID
    ds.setUserData(ds_devices[i], (int16_t) i);
#endif
    printf("\n\rDevice %d: ", i);
    print_address(ds_devices[i]);
  }
}

void loop() {
  delay(5000);
  digitalWrite(LED, HIGH);
  Serial.print("\n\r");
  ds.requestTemperatures();
  delay(500);  
  for(int i = 0; i < number_of_ds18_devices; i++)
  {
    print_temperature(ds, ds_devices[i]);    
  }
  digitalWrite(LED, LOW);

}

void print_address(DeviceAddress deviceAddress)
{ 
  Serial.printf("\rAddrs: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    Serial.printf("%x%x", deviceAddress[i]>>4, deviceAddress[i]&0xf);
  }
  Serial.printf("\n\r");
}

void print_temperature(DallasTemperature ds, DeviceAddress device)
{
  if (ds.isConnected(device))
  {
    printf("ds18(user data): %d\n\r", ds.getUserData(device));
    print_address(device);
    printf("Temp: %f\n\r", ds.getTempC(device));
  } else
  {
    printf("Device not found! ");
    print_address(device);
  }
}
