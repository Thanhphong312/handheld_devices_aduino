#include <SoftwareSerial.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
class Protocol {
public:
    
    //get post server
    virtual int connect() = 0;
    virtual String get(char* path = "") = 0;
    virtual int post(char* path = "", char* data = "") = 0;
    virtual ~Protocol() {}
};

class HTTP : public Protocol {
  private:
    char* host = "api.timezonedb.com";
    int port = 80;
    char* path = "/v2.1/get-time-zone?key=9X1N2H0EY3ZJ&format=json&by=position&lat=10.7769&lng=106.7009";
    WiFiEspClient espClient;
  public:
    HTTP(WiFiEspClient espClient = WiFiEspClient(), char* host = "", int port = 0, char* path = ""){
      this->host = (host!="")?host:this->host;
      this->port = (port!=0)?port:this->port;
      this->path = (path!="")?path:this->path;
      this->espClient = espClient;
    }
    ~HTTP(){

    }
    int connect(){
      if(this->espClient.connect(this->host, this->port)){
        return 1;
      }else{
        return 0;
      }
    }
    String get(char* path = "" ) override {
      String response;
      char request[250]; // Adjust the size according to your string length
      sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: ESP8266\r\nContent-Type: application/json\r\n\r\n" , path, this->host);
      Serial.println(request);    
      if(this->espClient.connect(this->host, this->port)){
        this->espClient.print(request);
        
        long int time = millis();
        int value = 0;
        while ( (time + 10000) > millis())
        {
          while ( this->espClient.available())
          {
            char c =  this->espClient.read();
            response += c;
            value++;
            if(value==52){
              break;
            }
          }
          if(value==52){
             break;
          }
        }
        Serial.print("Response: ");
        Serial.println(response);
      }
      return response;
    }
    int post(char* path = "", char* data = "") override {
      String response = ""; 
      char request[250]; // Adjust the size according to your string length
      sprintf(request, "POST %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: ESP8266\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s" , path, this->host, strlen(data) ,data);
      Serial.println(request);
      if(this->espClient.connect(this->host, this->port)){
        this->espClient.print(request);
        long int time = millis();
        while ( (time + 10000) > millis())
        {
          while ( this->espClient.available())
          {
            char c =  this->espClient.read();
            response += c;
          }
        }
        Serial.print("Response: ");
        Serial.println(response);
        Serial.println("Request complete.");
      }
      if(strstr(response.c_str(), "201") || strstr(response.c_str(), "200")){
        return 1;
      } else {
        return 0;
      }
    }
};

class MQTT : public Protocol {
private:
    char* broker = "dev.iotlab.net.vn";
    int port = 1883;
    char* username = "api1@Iotlab";
    char* password = "Iotlab@2023";
    char* topic = "iotla/data_collective";
    char* qos = "Q0";
    int rtn = 0;
    char* clientID = "ESP8266Client";
    PubSubClient client;
public:
    MQTT(PubSubClient client = PubSubClient(), char* broker = "", int port = 1883, char* username = "", char* password = "", char* topic = "", char* qos = "Q0", int rtn = 0){
      this->broker = (broker!="")?broker:this->broker;
      this->port = (port!=1183)?port:this->port;
      this->username = (username!="")?username:this->username;
      this->password = (password!="")?password:this->password;
      this->topic = (topic!="")?topic:this->topic;
      this->qos = qos;
      this->rtn = rtn;
      this->client = client;
      this->client.setServer(this->broker, this->port);
      // this->client.connect(this->clientID.c_str(), this->username.c_str(), this->password.c_str());
      // this->client.disconnect();
    }
    ~MQTT(){

    }
    int connect(){
      
      // if (this->client.connected()) {
      //   Serial.println("connected");
        // client.disconnect();
      //   return 1;
      // }else{
      //   Serial.println("no connected");
      //   return 0;
      // }
      // delay(10000);
      return 1;
    }
    String get(char* path = "") override {
      return "";
    }
    int post(char* path = "", char* data = "") override {
      if(this->client.connect(this->clientID, this->username, this->password)){
        this->client.publish(this->topic, data);
        this->client.close()
      }
      return "";
    }
};



