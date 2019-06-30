
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
      char buf[12];
      
      itoa(bme280_arr[i].bme280_obj.readTemperature(), buf, 10);
      String topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/temperature";
      mqtt.publish(topic.c_str(), buf);
      
      itoa((bme280_arr[i].bme280_obj.readPressure() / 100.0F), buf, 10);
      topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/pressure";
      mqtt.publish(topic.c_str(), buf);

      itoa(bme280_arr[i].bme280_obj.readAltitude(SEALEVELPRESSURE_HPA), buf, 10);
      topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/altitude";
      mqtt.publish(topic.c_str(), buf);

      itoa(bme280_arr[i].bme280_obj.readHumidity(), buf, 10);
      topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/humidity";
      mqtt.publish(topic.c_str(), buf);
      
      
    } else {
      Serial.print("BME280 Sensor with id=");
      Serial.print(bme280_arr[i].id);
      Serial.print(", addr=");
      Serial.print(bme280_arr[i].addr);
      Serial.println(" is not ok, skipping");
    }
  }
  if (bme280_reset_counter <= 0) {
    for(int i = 0; i < bme280_arr_size; i++) {
      if (bme280_arr[i].is_null) {
        continue;
      }
      Serial.print("Resetting BME280 Sensor with id=");
      Serial.print(bme280_arr[i].id);
      Serial.print(", addr=");
      Serial.println(bme280_arr[i].addr);
      bme280_arr[i].ok = bme280_arr[i].bme280_obj.begin(bme280_arr[i].addr);
      if (!bme280_arr[i].ok){
        Serial.println("BME sensor was not ok");
        String topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/temperature";        
        mqtt.publish(topic.c_str(), "0");
        
          topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/pressure";        
        mqtt.publish(topic.c_str(), "0");
          topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/altitude";        
        mqtt.publish(topic.c_str(), "0");
          topic = "/devices/temperatures/" + bme280_arr[i].id + "/data/humidity";        
        mqtt.publish(topic.c_str(), "0");

        
      } else {
        Serial.println("BME sensor was ok");
      }
    }
    bme280_reset_counter = 10;
  } else {
    bme280_reset_counter--;
  }
}
