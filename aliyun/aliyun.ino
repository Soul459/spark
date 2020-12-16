#include <ESP8266WiFi.h>   
#include <PubSubClient.h>  
#include <ArduinoJson.h>   
#include <Servo.h>
#include "aliyun_mqtt.h"
//需要安装crypto库

#define LED     D4
#define WIFI_SSID        "TP-LINK_F9CB"//WIFI
#define WIFI_PASSWD      "18717495170"//WIFI密码

#define PRODUCT_KEY      "a133aHJcL0o" //PRODUCT_KEY
#define DEVICE_NAME      "EPS8266Duan" //DEVICE_NAME
#define DEVICE_SECRET    "d8fb67768e3982089ae9d4fe8a0e2448"//DEVICE_SECRET

#define DEV_VERSION       "S-TH-WIFI-v1.0-20190220"    

#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST    "thing.event.property.post"
#define ALINK_TOPIC_DEV_INFO      "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""    
#define ALINK_VERSION_FROMA      "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"
unsigned long lastMs = 0;
int pos = 90;

WiFiClient   espClient;
PubSubClient mqttClient(espClient);
Servo myServo;

void init_wifi(const char *ssid, const char *password)      //连接WiFi
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi does not connect, try again ...");
        delay(500);
    }

    Serial.println("Wifi is connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.print("****** ");
    Serial.println((char *)payload);
    Serial.print("****** ");
   // https://arduinojson.org/v5/assistant/  json数据解析网站

    
 
  Serial.println((char *)payload);
   
    if (strstr(topic, ALINK_TOPIC_PROP_SET))
    {
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        int params_LightSwitch = root["params"]["LightSwitch"];
        if(params_LightSwitch==1)
        {Serial.println("led off");
        digitalWrite(LED, HIGH); 
        }
        else
        {Serial.println("led on");
        if (!digitalRead(LED)){
          digitalWrite(LED, HIGH);
          for(pos=90;pos>55;pos--){
           myServo.write(pos);           
           delay(15);} 
        }else{
          digitalWrite(LED, LOW);
          for(pos=55 ;pos<=90;pos++){
           myServo.write(pos);             // 使用pos变量数值设置伺服电机
           delay(15);
          }
        }
        }
        if (!root.success())
        {
            Serial.println("parseObject() failed");
            return;
        }
    }
}
void mqtt_version_post()
{
    char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"id\": 123,\"params\": {\"version\": \"%s\"}}", DEV_VERSION);
   // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(param);
    mqttClient.publish(ALINK_TOPIC_DEV_INFO, param);
}
void mqtt_check_connect()
{
    while (!mqttClient.connected())//
    {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("MQTT connect succeed!");
            //client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET);
            
            Serial.println("subscribe done");
            mqtt_version_post();
        }
    }
    
}

void mqtt_interval_post()
{
    char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"LightSwitch\":%d,\"CurrentTemperature\":%d}", !digitalRead(LED),random(0,55));
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}


void setup()
{

   
     myServo.attach(D8);
    pinMode(LED, OUTPUT);
    /* initialize serial for debugging */
    Serial.begin(115200);

    Serial.println("Demo Start");

    init_wifi(WIFI_SSID, WIFI_PASSWD);

    mqttClient.setCallback(mqtt_callback);
}

// the loop function runs over and over again forever
void loop()
{
   if (millis() - lastMs >= 50000)  //5S
    {
        lastMs = millis();
        mqtt_check_connect();
                 
       // mqtt_interval_post();
    }

    mqttClient.loop();

   
}
