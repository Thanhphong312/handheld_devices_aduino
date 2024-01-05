#ifndef Esp8266_h
#define Esp8266_h
#include <stdio.h>
#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
#include "protocol.h"
using namespace std;

class Esp8266{
	private :
		int rx = 10;
		int tx = 11;
		int timeout = 5000;
    int speed = 9600;
    char* ssid = "GIA KHANH";
    char* pass = "77777777";
    String status = "disconnect";
    Protocol *protocol = nullptr;
    WiFiEspClient espClient;
    PubSubClient client;
    SoftwareSerial esp8266{rx ,tx};
    HTTP *http = nullptr;
    MQTT *mqtt = nullptr;
	public :
    Esp8266(){
      
    }
    ~Esp8266(){
       // Giải phóng bộ nhớ đã cấp phát cho protocol
      this->close(); // Đóng kết nối và giải phóng tài nguyên mạng
    }
    void setESP(){
      SoftwareSerial newesp8266{this->rx, this->tx};
      this->esp8266 = newesp8266;
      this->client = PubSubClient(espClient);
      esp8266.begin(9600);
      WiFi.init(&this->esp8266);
    }
    String getStatus(){
      return this->status;
    }
    void setStatus(String status){
      this->status = status;
    }
    void setTimeout(int timeout){
			this->timeout = timeout;
		}
    void setPin(int rx, int tx){
      this->rx = rx;
      this->tx = tx;
    }

    void setSpeed(int value){
       this->esp8266.begin(value);
    }
		void setConfigWifi(char* ssid, char* pass){
			this->ssid = ssid;
			this->pass = pass;
		}

    int connectWifi(){
      int delay = 0;
      WiFi.begin(this->ssid, this->pass);
      while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Attempting to connect to WiFi...");
        if (WiFi.begin(this->ssid, this->pass) != WL_CONNECTED) {
          Serial.println("Failed to connect to WiFi. Retrying...");
          if(delay==2){
            break;
          }
          delay++;
        }
      }
      if(WiFi.status() == WL_CONNECTED){
        this->status = "connected";
        return 1;
      }else{
        this->status = "disconnected";
        return 0;
      }
    }
    int connectHTTP(){
      return protocol->connect();
    }
    int connectMQTT(){
      return protocol->connect();
    }

    void setHTTP(char* host = "", int port = 0){
        protocol = new HTTP(this->espClient, host , port );
    }
    void setMQTT(char* broker = "", int port = 1883, char* username = "", char* password = "", char* topic = "", char* qos = "Q0", int rtn = 0){
        protocol = new MQTT(this->client, broker, port, username, password, topic, qos, rtn);
    }
    String getSsid(){
      return this->ssid;
    }
    String getPass(){
      return this->pass;
    }
    String get(char* path = ""){
      return protocol->get(path);
    }
    int post(char* path = "", char* data = ""){
      return protocol->post(path, data);
    }
    void close(){
      this->espClient.stop();
      // this->espClientmqtt.stop();
      this->client.disconnect();
      delete protocol;
    }
};
#endif

