
#include <Arduino.h>
// el-client
#include <ELClient.h>
#include <ELClientCmd.h>
#include <ELClientMqtt.h>
// BME280
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// sends mbe280 data to respective topic, resets sensors periodically
void sendBmeData() {
  for(int i = 0; i < bme280_arr_size; i++) {
    if (bme280_arr[i].is_null) {
      continue;
    } 
    if (bme280_arr[i].ok) {
      bme280_arr[i].bme280_obj.takeForcedMeasurement();
      String message = (String)bme280_arr[i].bme280_obj.readTemperature() + "|" + (String)bme280_arr[i].bme280_obj.readPressure() + "|" + (String)bme280_arr[i].bme280_obj.readHumidity() + '|' + (String)bme280_arr[i].bme280_obj.readAltitude(SEALEVELPRESSURE_HPA);
      String topic = "/devices/temperatures/" + (String) bme280_arr[i].id + "/data";
      mqtt.publish(topic.c_str(), message.c_str());
    }
  }
  if (bme280_reset_counter <= 0) {
    for(int i = 0; i < bme280_arr_size; i++) {
      if (bme280_arr[i].is_null) {
        continue;
      }      
      bme280_arr[i].ok = bme280_arr[i].bme280_obj.begin(bme280_arr[i].addr);
      if (!bme280_arr[i].ok){
        String topic = "/devices/temperatures/" + (String) bme280_arr[i].id + "/data";
        
        mqtt.publish(topic.c_str(), "no data|no data|no data|do data");
      }
    }
    bme280_reset_counter = 10;
  } else {
    bme280_reset_counter--;
  }
}
