#include "esp_camera.h"
#include <WiFi.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";

const char* ssid = "esp32";
const char* password = "1234567";

const char * udpAddress = "192.168.0.37";
const char * controllerName = "c_esp32";

int button1Count = 0;
int button2Count = 0;

int button1 = 15;
int ledPin = 14;

int updateRate = 50;
int newControllerThresh = 5000/updateRate;
bool newControllerAvailable = true;

int changeCount = 0;

const int udpPort = 8080;

String test1 = "anyas  Wi-Fi";
String test2 = "mesi1971";


/*
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
     input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";
*/

void startCameraServer();

AsyncUDP Audp;
WiFiUDP udp;
//AsyncWebServer server(80); //should this be here???

/*
bool updateValues = false;


void notFound(AsyncWebServerRequest *request) {
  Serial.println("not found");
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  if(updateValues){
    Serial.println("it is indeed true");
  } else {
    Serial.println("it is false");
  }
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}


const char *ssid1 = "esp32";
const char *password1 = "123456";
//ok so instead of an async webserver try and make it a regular webserver, that way it cant continue until you get the submit button
//and then its problem solved

//another thing, it doesnt look like the file is actually saved when the esp restarts, maybe its because i flashed it? not sure.
void setupNewController() {

   if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

   WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  Serial.println("esp32 server started");
  server.begin();
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String username;
    String password;

      username = request->getParam(PARAM_INPUT_1)->value();
         password = request->getParam(PARAM_INPUT_2)->value();
       //  password += ","; 
         String inputMessage = username + "," + password;

 Serial.println("updated values is now true");
   
   writeFile(SPIFFS, "/wifiCred.txt", inputMessage.c_str());

       Serial.println("i got " + inputMessage);

    request->send(200, "text/html", "Updated username and password options. Restarting controller" );
    server.end();
    delay(1000);
    Serial.println("they are now true");
    updateValues = true;
  //  ESP.restart();          
    server.end();        
  });
  server.onNotFound(notFound);
  server.begin();
}


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

void getWifiInfo()
{
  String yourInputString = readFile(SPIFFS, "/wifiCred.txt");
  String yourInputStringCopy = String(yourInputString);
  //split this string based on a comma, then update the wifi ssid and password
  int firstIndex = yourInputString.indexOf(',');
  Serial.print("the index is: ");
  Serial.println(firstIndex);

 test1 = yourInputString.substring(0,firstIndex);
  test2 = yourInputStringCopy.substring(firstIndex+1, yourInputStringCopy.length());

  
 // ssid = getValue(yourInputString, ',', 0).c_str();
  //password = getValue(yourInputString, ',', 1).c_str();
  Serial.print("ssid: ");
  Serial.println(test1);
 Serial.print("password: ");
  Serial.println(test2);

  delay(5000);
}

*/
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode(button1,INPUT);
 //pinMode(ledPin, OUTPUT);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
  
//have the led blink three times, if the button is pressed in those three times then set up the new controller
/*
for(int i = 0; i < 6; i++){
 // digitalWrite(ledPin, !digitalRead(ledPin));
if(digitalRead(button1) == LOW){
  Serial.println("the button is low, initializing new controller...");
  setupNewController();
  while(!updateValues){
    Serial.println("update values are false");
    
    delay(1000);
  }
}
  delay(1000);
}

Serial.println("calling get wifi info");
getWifiInfo(); //this puts in the values for ssid and password

//here read from the file and get the values for ssid and password
*/
Serial.println("starting wifi");
  WiFi.begin(test1.c_str(), test2.c_str());
  int count = 0;

 
  while (WiFi.status() != WL_CONNECTED) {
    //blink led here
  //  digitalWrite(ledPin, !digitalRead(ledPin));
  
    delay(500);
    count++;
    if(count > 10){
      Serial.println("RESTARTING...");
      delay(1000);
      ESP.restart();
      }
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected, listening for computer...");
  //blink led fast here

/*
  for(int i = 0; i < 10; i++){
    //  digitalWrite(ledPin, !digitalRead(ledPin));
    delay(200);
  }*/
  
  /* if(Audp.listen(1234)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        Audp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
        
            if(strcmp((char *) packet.data(), "pc") == 0){
                 
                 packet.printf(controllerName);         
             //   udp.begin(packet.localIP(),packet.localPort());
               Audp.close();
           
               
                 }
            //reply to the client
        
            //check here if the packet is from the computer, if yes then use its ip address and port
        
        }); */
          udp.begin(WiFi.localIP(), 1234);
        
        //read values from wifi udp, keep track of the time passed, if too much time passed then just create a new controller
        
   //}

Serial.println("Camera server started");
  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}


void loop() {
  //if its not connected then show it with the led
// if(!connected){
    //Send a packet
   // Serial.println("ERROR, not connected");
 //continue;
 // }
if(digitalRead(button1) == LOW){
//send upd message
//Serial.println("button is high");
   udp.beginPacket(udpAddress,udpPort);
    udp.printf("1 1");
    udp.endPacket();
    changeCount = 0;
 // button1Count++;
} else if(changeCount < 5){
   udp.beginPacket(udpAddress,udpPort);
    udp.printf("0 0");
    udp.endPacket();
    changeCount++; 
  
  delay(updateRate);

}
}
