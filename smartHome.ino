
///////////////////////////////////////////////////////////////////////////////////////BME280
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



#define SEALEVELPRESSURE_HPA (1013.25)
// we want to reset sensor periodically to check it's status
int bme280_reset_counter = 0;
// we can't connecto more than two sensors on same I2C bus
const int bme280_arr_size = 2;

struct bme280_struct {
  uint8_t addr = 0x00;
  int id;
  Adafruit_BME280 bme280_obj;
  bool ok;
  bool is_null;
};
bme280_struct bme280_arr[2];
///////////////////////////////////////////////////////////////////////////////////////BME280













/**
   Simple example to demo the esp-link MQTT client
*/

#include <ELClient.h>
#include <ELClientCmd.h>
#include <ELClientMqtt.h>




// Initialize a connection to esp-link using the normal hardware serial port both for
// SLIP and for debug messages.
ELClient esp(&Serial, &Serial);

// Initialize CMD client (for GetTime)
ELClientCmd cmd(&esp);

// Initialize the MQTT client
ELClientMqtt mqtt(&esp);

// Callback made from esp-link to notify of wifi status changes
// Here we just print something out for grins
//Обратный звонок сделан по esp-ссылке для уведомления об изменениях статуса wifi
// Здесь мы просто что-то распечатываем

//Синхронизация с ESP-Link, это требуется в начале любого эскиза и инициализирует
// обратные вызовы к обратному вызову изменения статуса Wi-Fi. Обратный вызов вызывается с начальным
//статус сразу после завершения Sync  //ниже

void wifiCb(void* response) {
  ELClientResponse *res = (ELClientResponse*)response;
  if (res->argc() == 1) {
    uint8_t status;
    res->popArg(&status, 1);

    if (status == STATION_GOT_IP) {
      Serial.println("WIFI CONNECTED");
    } else {
      Serial.print("WIFI NOT READY: ");
      Serial.println(status);
    }
  }
}

bool connected;

// Callback when MQTT is connected
void mqttConnected(void* response) {
  Serial.println("MQTT connected!");
  //mqtt.subscribe("/esp-link/1");
  //mqtt.subscribe("/hello/world/#");
   mqtt.subscribe("/devices/#");
  //mqtt.subscribe("/esp-link/2", 1);
  //mqtt.publish("/esp-link/0", "test1");
  connected = true;
}

// Callback when MQTT is disconnected
void mqttDisconnected(void* response) {
  Serial.println("MQTT disconnected");
  connected = false;
}

// Callback when an MQTT message arrives for one of our subscriptions
void mqttData(void* response) {
  ELClientResponse *res = (ELClientResponse *)response;

  Serial.print("Received: topic=");
  String topic = res->popString();
  Serial.println(topic);
  
  Serial.print("data=");
  String data = res->popString();
  Serial.println(data);

 Serial.println("----------------------------next if---------------------------------------------------------------------------------------------------------");

 
 
      Serial.println(topic);                                                      ////////////////////////////
     // String topicwords = getValue(topic, '/', 0);//devices                      
     // Serial.println(topic);                                                      ///////////////////////////
//      Serial.println("1"+topicwords+"1");                                         ///////////////////////////
  

    String topicwords = getValue(topic, '/', 1);//devices
    
    if(topicwords=="devices"){
      Serial.println("devices");Serial.println(topicwords);
      topicwords = getValue(topic, '/', 2);//lamps
      
      if(topicwords == "lamps"){
         Serial.println("lamps");Serial.println(topicwords);
         topicwords = getValue(topic, '/', 4);//command config schedules
         
         Serial.println("1"+topicwords+"1");
         if(topicwords=="config"){
                  Serial.println("config");Serial.println(topicwords);
                  //create oblect
                    pinMode(5, OUTPUT);
                   digitalWrite(5,LOW);
         }
         else if(topicwords =="command"){          
                 Serial.println("command");Serial.println(topicwords);

                 Serial.println("data");Serial.println(data);
                 topicwords = getValue(data, '|', 0);//
                 Serial.println("name");Serial.println(topicwords);

                if(topicwords =="on")
                {
                        digitalWrite(5,HIGH);
                }
                else if(topicwords == "off")
                {
                     digitalWrite(5,LOW);
                }
                 Serial.println("data");Serial.println(data);
                 topicwords = getValue(data, '|', 1);//
                 Serial.println("pin");Serial.println(topicwords);
               
                 digitalWrite(5,HIGH);                 
         }
         else if(topicwords =="schedules"){
                 Serial.println("schedules");Serial.println(topicwords);
                 digitalWrite(5,LOW);
                 delay(1000);
                 digitalWrite(5,HIGH);
         }         
      }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////lamp
            else  if(topicwords == "motionsensors"){
         Serial.println("motionsensors");Serial.println(topicwords);
         topicwords = getValue(topic, '/', 4);//command config schedules
         
         Serial.println("1"+topicwords+"1");
         if(topicwords=="config"){
                  Serial.println("config");Serial.println(topicwords);
                  //create oblect
                   pinMode(2, INPUT); //Объявляем пин, к которому подключен датчик движения, входом
                //    pinMode(5, OUTPUT);
                 //  digitalWrite(5,LOW);
         }
         else if(topicwords =="data"){
          
                 Serial.println("data");Serial.println(topicwords);               
               //  digitalWrite(5,HIGH);
                 
         }          
      }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////motionsensors  
        else  if(topicwords == "temperatures"){
         Serial.println("temperatures");Serial.println(topicwords);
         topicwords = getValue(topic, '/', 4);//command config schedules
         
         Serial.println("1"+topicwords+"1");
         if(topicwords=="config"){
           int id = getValue(topicwords, '|', 2).toInt();
           if (getValue(topicwords, '|', 2) == 'delete') {
             bme280_arr[id].bme280_obj = NULL;
             bme280_arr[id].is_null = true;
             bme280_arr[id].ok = false;
           } else {
             Adafruit_BME280 bme;
             
             bme280_arr[id].addr = 0x76 + id;
             bme280_arr[id].id = id;
             bme280_arr[id].bme280_obj = bme;
             bme280_arr[id].ok = false;
             bme280_arr[id].is_null = false;
           }
         }
         else if(topicwords =="data"){
                 Serial.println("data");Serial.println(topicwords);
         }          
      }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////temperatures
       else  if(topicwords == "motors"){
         Serial.println("motors");Serial.println(topicwords);
         topicwords = getValue(topic, '/', 4);//command config schedules
         
         Serial.println("1"+topicwords+"1");
         if(topicwords=="config"){
                  Serial.println("config");Serial.println(topicwords);
                  //create oblect
                    pinMode( 11, OUTPUT);
                    pinMode( 10, OUTPUT);
                    pinMode( 9, OUTPUT);
                    pinMode( 8, OUTPUT);
  
                   // pinMode(5, OUTPUT);
                   //digitalWrite(5,LOW);
                    digitalWrite(5,HIGH);
         }
         else if(topicwords =="data"){

                 rotation_1(1000, 500); // вращаю в одну сторону
                 delay(500);
                 rotation_2(1000, 500); // вращаю в другую сторону
                 Serial.println("data");Serial.println(topicwords);               
                 digitalWrite(5,HIGH);
                 
         }          
      }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////motors    
       else  if(topicwords == "curtains"){
         Serial.println("curtains");Serial.println(topicwords);
         topicwords = getValue(topic, '/', 4);//command config schedules
         
         Serial.println("1"+topicwords+"1");
         if(topicwords=="config"){
                  Serial.println("config");Serial.println(topicwords);
                  //create oblect
                    pinMode(5, OUTPUT);
                   digitalWrite(5,LOW);
         }
         else if(topicwords =="command"){
          
                 Serial.println("command");Serial.println(topicwords);
               
                 digitalWrite(5,HIGH);
                 
         }
         else if(topicwords =="schedules"){
                 Serial.println("schedules");Serial.println(topicwords);
                  digitalWrite(5,LOW);
                  delay(1000);
                 digitalWrite(5,HIGH);
         }         
      }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////curtains    
    }

   Serial.println("-------------------------------------------------------------------------------------- Topic---------------------------------------------------------------------------------------------------------");


// Serial.println("Topic");                                                      ////////////////////////////
 // Serial.println(topic);                                                      ////////////////////////////
  //if(topic == "/devices/lamps/1/command"){

  //   Serial.println("Topic");
   //   Serial.println(topic);
   
   //data+="|";
    //char str_array[data.length()];
    //data.toCharArray(str_array, data.length());

  //  char * pch = strtok (str_array,"|");

  //  while (pch != NULL)
  //  {
  //    Serial.println("while (pch != NULL)");
   //   Serial.println(pch);
  //    pch = strtok (NULL, "|");
  //  }
  //    Serial.println("while END");
    
  //}
   Serial.println("---------------------------- END if---------------------------------------------------------------------------------------------------------");



  
 // String word3 = getValue(" hi this is a split test", ' ', 0);
  //Serial.println(word3);
  
//   word3 = getValue(" hi this is a split test", ' ', 1);
 // Serial.println(word3);

   //word3 = getValue(" hi this is a split test", ' ', 2);
  //Serial.println(word3);

   //word3 = getValue(" hi this is a split test", ' ', 3);
  //Serial.println(word3);

  // word3 = getValue("hi this is a split test", ' ', 4);
  //Serial.println(word3);

   //word3 = getValue("hi this is a split test", ' ', 5);
  //Serial.println(word3);
  
 //  Serial.println("------------------------------------------------ split---------------------------------------------------------------------------------------------------------");


}


//////////////////////////////////////////////////////////////split bi idex
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}



/////////////////////////////////////////////////////////////////////////////////////rotate motore
void rotation_1(int t, int s){ // В этой функции я повторяю таблицу для управления ШД
  for(int i = 0; i < s; i++){
    digitalWrite(11, HIGH); // 1
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, HIGH); // 2
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, LOW); // 3
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //4
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //5
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //6
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //7
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, HIGH); //8
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
   }
    Serial.println("rotate1 done");
  // Serial.println("rotate done"); //output  the serial  data 9. 
 }
 /////////////////////////////////////////////////////////////////////////////////////rotate motore back
 void rotation_2(int t, int s){ // В этой функции я повторяю таблицу для управления ШД, только в обратном порядке
    for(int i = 0; i < s; i++){
    digitalWrite(11, HIGH); //8
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //7
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //6
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //5
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, LOW); //4
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    delayMicroseconds(t);
    digitalWrite(11, LOW); // 3
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, HIGH);// 2
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
    digitalWrite(11, HIGH); // 1
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    delayMicroseconds(t);
   }
    Serial.println("rotate2 done");
   // Serial.println("rotate2 done"); //output  the serial  data 9. 
}









void mqttPublished(void* response) {
  Serial.println("MQTT published");
}

void setup() {
  Serial.begin(115200);
  Serial.println("EL-Client starting!");

  // Sync-up with esp-link, this is required at the start of any sketch and initializes the
  // callbacks to the wifi status change callback. The callback gets called with the initial
  // status right after Sync() below completes.
  esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
  bool ok;
  do {
    ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds//синхронизируетсvя с esp-link, блокирует до 2 секунд
    if (!ok) Serial.println("EL-Client sync failed!");
  } while (!ok);
  Serial.println("EL-Client synced!");

  // Set-up callbacks for events and initialize with es-link.
  mqtt.connectedCb.attach(mqttConnected);
  mqtt.disconnectedCb.attach(mqttDisconnected);
  mqtt.publishedCb.attach(mqttPublished);
  mqtt.dataCb.attach(mqttData);
  mqtt.setup();

  //Serial.println("ARDUINO: setup mqtt lwt");
  //mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

  Serial.println("EL-MQTT ready");





  /////////////////////////////////////buzzer
  // pinMode(4, OUTPUT);
     /////////////////////////////////////buzzer

     ///////////////////////////////////////////////////////////////////////////////////////////////////////photoresistor
    // pinMode(13, OUTPUT);
     ///////////////////////////////////////////////////////////////////////////////////////////////////////photoresistor
}

static int count;
static uint32_t last;
static uint32_t last1;
char buf[12];

void loop() {
  esp.Process();

  if (connected && (millis() - last) > 30000) {
    sendBmeData();
    last = millis();
  }
  //else{
    // Serial.println(connected);
     // Serial.println(millis());
     //  Serial.println(last);
   // }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////motionsensors
  //int pirVal = digitalRead(2); //Считываем значения с датчика движения. Если обнаружили движение, 
                                    //то транслируем сигнал тревоги в монитор порта и включаем светодиод
   if(  millis() - last1> 1000){// 5 sec
      if(digitalRead(2) == HIGH)
      {
        digitalWrite(13, HIGH);
        Serial.println("Тревога ");
        mqtt.publish("/devices/motionsensors/1/data", "Anxiety");
        //delay(2000);
      }
    else
    {
     // Serial.println("Сканирую ");
      digitalWrite(13,LOW);
      //delay(10);
    }
      last1 = millis();
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////motionsensors






  ///////////////////////////////////////////////////////////////////////////////////////////////////////temperature


  //Serial.print("Temperature = ");z
  //Serial.print(bme.readTemperature());
  //Serial.println("*C");

  //Serial.print("Pressure = ");
  //Serial.print(bme.readPressure() / 100.0F);
  //Serial.println("hPa");

  //Serial.print("Approx. Altitude = ");
  //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  //Serial.println("m");

  //Serial.print("Humidity = ");
  //Serial.print(bme.readHumidity());
  //Serial.println("%");

  //Serial.println();
  ///////////////////////////////////////////////////////////////////////////////////////////////////////temperature





//pinMode(13, OUTPUT);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////photoresistor
 unsigned int sensorValue = analogRead(A0); //Считываем значения с фоторезистора
  if(sensorValue<700) digitalWrite(5, HIGH); //Включаем
  else digitalWrite(5, LOW); // Выключаем
  //Serial.print(sensorValue, DEC); //Вывод данных с фоторезистора (0-1024)
  //Serial.println(""); 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////photoresistor




///////////////////////////////////////////buzzer active hight
 // pinMode(4, OUTPUT);
  //tone(4, 1000, 200);
  //delay(100);
  //noTone(4);
///////////////////////////////////////////buzzer active hight

///////////////////////////////////////////buzzer passive low
 // digitalWrite(4, HIGH);
  //delay(100);
  //digitalWrite(4, LOW);
  ///////////////////////////////////////////buzzer passive low



  /////////////////////////////////////////////////////////////////////////////////flame sensor 



 /////////////////////////////////////////////////////////////////////////////////flame sensor
 //if (analogRead(A1) < 1023){//if the data us less than 1023 then set off an alarm
  
   //   tone(4, 800, 800);
     // delay(200);
     // tone(4, 600, 800);
     // delay(200);
    //    mqtt.publish("/hello/world/arduino", 1);
// }
// else{
//   mqtt.publish("/hello/world/arduino", 2);
//  digitalWrite(4, LOW);// if else don't set off alarm

//}

  
}
