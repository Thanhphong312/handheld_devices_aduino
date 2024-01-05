#ifndef SDCard_h
#define SDCard_h
#include <SD.h>
#include <ArduinoJson.h>
class SDCard{
  private:
    int SC = 53;
    SDClass sd;
    File myfile;
  public:
    SDCard(){
    }
    ~SDCard(){ 

    }
    void setSDCard(){
      sd.begin(SC);
    }
    void createFile(const String namefile) {
      File file = sd.open(namefile, FILE_WRITE);
      if (file) {
        file.close();
        Serial.println("File created successfully: " + namefile);
      } else {
        Serial.println("Error creating the file: " + namefile);
      }
    }

    int writeFile(String namefile, String content){
      String fullPath = namefile; // Concatenate the path and filename
      File file = sd.open(fullPath, FILE_WRITE);
      if (file) {
        // Write the data to the file
        file.println(content);
        file.close();
        Serial.println("Data written to the file.");
        return 1;
      }
      return 0;
    }
    String readFile(String namefile) {
      String fullPath =  namefile; // Concatenate the path and filename
      if (sd.exists(fullPath)) {
        File readfile = sd.open(fullPath, FILE_READ);
        Serial.println("File exist");
        String fileContent = readfile.readString();
        readfile.close();
        return fileContent;
      }else{
        this->createFile(fullPath);
        Serial.println("File no exist");
      }
      return ""; 
    }

    String getHistorySensor(String namefile, int startline) {
      String data = ""; // Chuỗi dữ liệu sẽ được trả về
      if (sd.exists(namefile)) {
        Serial.println("File exist");
        File file = sd.open(namefile);
        if (file) {
          int lineNum = 0; // Biến đếm số dòng
          while (file.available()) {
            String line = file.readStringUntil('\n'); // Đọc một dòng
            if (lineNum == startline) {
              data = line;
              break;
            }
            lineNum++;
          }
          file.close(); // Đóng file khi đã đọc xong
        } else {
          Serial.println("Không thể mở file.");
        }
      }else{
        this->createFile(namefile);
        Serial.println("File no exist");
      }
      return data;
    }
    String getHistoryByID(String namefile, int idToFind){
      File file = sd.open(namefile);
      if (!file) {
          Serial.println("Error opening file");
          return ""; // or some error indication
      }

      while (file.available()) {
          String line = file.readStringUntil('\n');
          Serial.println(line);
          int id;
          char value[10], datetime[20], status[2];
          sscanf(line.c_str(), "{\"id\":%d,\"value\":\"%9[^\"]\",\"datetime\":\"%19[^\"]\",\"status\":\"%1[^\"]\"}", &id, value, datetime, status);
          // char cstr[3];
          // sprintf(cstr, "%d", id);
          // char cstrfind[3];
          // sprintf(cstrfind, "%d", idToFind);
          // Serial.println(id);
          // Serial.println(value);
          // Serial.println(datetime);
          // Serial.println(status);
          // Serial.println(idToFind);
          if (id == idToFind) {
              file.close();
              return line;
          }
          // doc.clear();
      }

      file.close();
      return ""; // Indicates ID not found
    }
    int lengthHistory(String namefile){
      int lineNum = 0; // Biến đếm số dòng
      if (sd.exists(namefile)) {
        Serial.println("File exist");
        File file = sd.open(namefile);
        if (file) {

          // Đọc từng dòng trong file
          while (file.available()) {
            String line = file.readStringUntil('\n'); // Đọc một dòng

            lineNum++;
          }
          
          file.close(); // Đóng file khi đã đọc xong
        } else {
          Serial.println("Không thể mở file.");
        }
      }else{
        this->createFile(namefile);
        Serial.println("File no exist");
      }
      
      return lineNum;
    }
     void changeStatus(String namefile, int statusID, String newStatus) {
      File file = sd.open(namefile);
      if (!file) {
          Serial.println("Không thể mở file.");
          return;
      }

      File filenote = sd.open("note.txt", FILE_WRITE);
      if (!filenote) {
          Serial.println("Error creating/ opening the file: note.txt");
          file.close();
          return;
      }
      filenote.close(); // Close note.txt initially

      filenote = sd.open("note.txt", O_WRITE | O_TRUNC);
      if (!filenote) {
          Serial.println("Không thể mở file để ghi.");
          file.close();
          return;
      }

      while (file.available()) {
          String line = file.readStringUntil('\n');
          int id;
          char value[10], datetime[20], status[2];
          sscanf(line.c_str(), "{\"id\":%d,\"value\":\"%9[^\"]\",\"datetime\":\"%19[^\"]\",\"status\":\"%1[^\"]\"}", &id, value, datetime, status);

          if (statusID == id) {
              // Change status here
              char cstr[3];
              sprintf(cstr, "%d", id);
              line = "{\"id\":" + String(cstr) + ",\"value\":\"" + String(value) + "\",\"datetime\":\"" + String(datetime) + "\",\"status\":\"" + newStatus + "\"}";
              Serial.println("Đã thay đổi status.");
          }

          filenote.print(line + "\n"); // Write the line content to note.txt
      }
      file.close();
      filenote.close(); // Close note.txt

      filenote = sd.open("note.txt");
      if (!filenote) {
          Serial.println("Không thể mở file để đọc.");
          return;
      }

      File writefile = sd.open(namefile, O_WRITE | O_TRUNC);
      if (!writefile) {
          Serial.println("Không thể mở file để ghi 3.");
          filenote.close();
          return;
      }

      while (filenote.available()) {
          String line = filenote.readStringUntil('\n');
          writefile.print(line + "\n");
      }

      writefile.close();
      filenote.close();
      deleteContentFile("note.txt");

      Serial.println("Đã thay đổi trạng thái tại id " + String(statusID) + " trong file " + namefile + " thành " + String(newStatus));
    }
    void deleteContentFile(String namefile){
      String fullPath = namefile; // Concatenate the path and filename
      sd.remove(fullPath);
    }
    int setHistorySensor(String namefile, String id, String value, String datetime, String status = "0") {
      String getJson = "{\"id\":"+id+",\"value\":\""+value+"\",\"datetime\":\""+datetime+"\",\"status\":\""+status+"\"}"; //{"value":"2","datetime":"10/25/2023 07:36:00"}
      Serial.println(getJson);
      return writeFile(namefile, getJson);
    }
    void setConfig(String usernamewifi = "IOT", String passwordwifi = "ABC12345", String hosthttp = "api1@Iotlab", String porthttp=  "80", String brokermqtt = "dev.iotlab.net.vn", String portmqtt = "1883", String usernamemqtt = "api1@Iotlab", String passwordmqtt = "Iotlab@2023"){      
      String jsonString = "{\"usernamewifi\":\""+usernamewifi+"\",\"passwordwifi\":\""+passwordwifi+"\",\"hosthttp\":\""+hosthttp+"\",\"porthttp\":\""+porthttp+"\",\"brokermqtt\":\""+brokermqtt+"\",\"portmqtt\":\""+portmqtt+"\",\"usernamemqtt\":\""+usernamemqtt+"\",\"passwordmqtt\":\""+passwordmqtt+"\"}";  // Convert the char array to a String
      Serial.println(jsonString);
      sd.remove("config.txt");
      this->writeFile("config.txt",jsonString);
    }
    String getConfig(){
      return this->readFile("config.txt");
    }
    void setCalibrations(String slopePH = "-7.78", String interceptPH = "16.34", String slopeSAL = "16.3", String interceptSAL = "0"){        
      String jsonString = "{\"slopePH\":\""+slopePH+"\",\"interceptPH\":\""+interceptPH+"\",\"slopeSAL\":\""+slopeSAL+"\",\"interceptSAL\":\""+interceptSAL+"\"}";  // Convert the char array to a String
      Serial.println(jsonString);
      sd.remove("standar.txt");
      this->writeFile("standar.txt",jsonString);
    }
    String getCalibrations(){
      return this->readFile("standar.txt");
    }
};
#endif