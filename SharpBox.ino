
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <vector>
#include <TimeAlarms.h>
#include <FS.h>
#include <WebSocketsServer.h>
#include <Hash.h>
//#include <GDBStub.h>    //   este es un Debugger que se puede usar si la aplicacion no es muy grande

#define ONE_HOUR 3600000UL
#define TEMP_SENSOR_PIN D2
#define RELAY_1_PIN D5
#define RELAY_2_PIN D6
#define RELAY_3_PIN D7
//#include "data/index.h"
//#include <webElements.h>
#include "/home/michel/Dropbox/Arduino/SharpBox/classes.h"

   //Component btnRelay1("btn","btnRelay1","btnRelay1","Relay 1 OFF");
   //Component btnRelay2("btn","btnRelay2","btnRelay2","Relay 2 OFF");
   //Component lbl1("lbl","textLabel","textLabel","Label");
   //Body body(1);
//#include "/data/MAIN_page.html"



ESP8266WebServer server (80);       // create a web server on port 80
DNSServer dnsServer;
WiFiManager wifiManager;
WiFiUDP UDP;                   // Create an instance of the WiFiUDP class to send and receive UDP messages
WebSocketsServer webSocket = WebSocketsServer(81);

const char *OTAName = "ESP8266";         // A name and a password for the OTA service
//const char *OTAPassword = "esp8266";
#define DEVICE_NAME "ESP8266 DEVICE"

const char* mdnsName = "sharpBoxDummy";        // Domain name for the mDNS responder, address is "basementController.local" no hace falta saber el IP

IPAddress timeServerIP;        // The time.nist.gov NTP server's IP address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;          // NTP time stamp is in the first 48 bytes of the message

const String nl = "/nl";
byte packetBuffer[NTP_PACKET_SIZE];      // A buffer to hold incoming and outgoing packets


File fsUploadFile;                                    // a File variable to temporarily store the received file







float temperature;
float humidity;
float tempSetpoint;
float countdownMinutes;
boolean processRunning = false;
boolean loggerRunning = false;
int loggerMinutes;
String tempControl;
String tempDirection;
String chkCountdownTimer;
String relay_1_state;
const unsigned long intervalNTP = ONE_HOUR; // Update the time every hour
unsigned long prevNTP = 0;
uint32_t timeUNIX = 0;   
uint32_t timeNow = 0;                      // The most recent timestamp received from the time server
                   // The most recent timestamp received from the time server
unsigned long lastNTPResponse = 0;
unsigned long countdownSetTime = 0;




    Page page("Sharp Box","The smart controller.");
  //String f[5] = { "uno","dos","tres","cuatro","Cinco" };
  String f[2] = { "OFF" , "ON" };
  ComboBox comboBox1 ( "combo1",2,f);
  ComboBox comboBox2 ( "combo2",2,f);
  //ComboBox comboBox3 ( "combo3",2,f);
  AnalogIn analogIn1 ( A0,"analog1");
  DigitalIn digitalIn1 ( D1,"digital1");
  Logger logger1("logger1", "/dataLog.csv" );
  DigitalOutput digitalOut1 ( D1,"digiOut1","digiout1");
  RelayOutput   relay1 (RELAY_1_PIN,"Relay 1 110VAC","relay1");
  RelayOutput   relay2 (RELAY_2_PIN,"Relay 2 110VAC","relay2");
  RelayOutput   relay3 (RELAY_3_PIN,"Relay 3 110VAC","relay3");
  Dsb18B20 tempSensor ( TEMP_SENSOR_PIN ,"Temp_Probe");   // habria que crearlo solo si encontro el sensor
  //Dht11 dht1( D5,"dht1");
  Button button1("btn1","button1");
 //  KeyPad keypad1 ("keypad1");
 // KeyPadCommand keypadCom("keyPadCom1");
  EditBox edit1 ("edit1","edit1","text");
  EditBox edit2 ("edit2","edit2","text");
  Label label1 ("label1","this is Label1");
  Label label2 ("label2","this is Label2");
    Graphic graphic1("graphic1");
//KeypadControl keypadControl1("keyPadCtrl1");

  ActiveControl control1 ("control1" , &digitalIn1 ,"=",  &edit2  , &graphic1 , &analogIn1 );
  ActiveControl control2 ("control2" , &tempSensor , ">", &edit1 , &relay3 , &edit2 );
  //ActiveControl control3 ("control3" , &tempSensor , "=", &edit1 , &relay1 , &edit2 );// xq hay problemas en la creacion de esto ?
  Set set1 ("set1",&relay1);
  Set set2 ("set2",&relay2);
  //Set set3 ("set3",&relay2);
 // KeyPad keypad2 ("keypad2");   //     POR ALGUNA RAZON ESTO LO TRABA Y NO DA NINGUN HTML DE SALIDA
  Program program1 ("program1");
  //Program program2 ("program2");
  //Pause pause1 ("pause1",1);
  LabelFreeHeap lblFreeHeap("lblHeap","");
  TimeLabel lblTime("lblTime","Label Time");

//  IfCommand if1("If numero 1",&edit1,&edit2);
///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              SETUP                                          ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////

  void setup() {
 
  //  if1.getHtml();
 //page.addElement(&if1);
  
  // put your setup code here, to run once:
   // pinMode ( RELAY_1_PIN , OUTPUT );
    //pinMode ( RELAY_2_PIN , OUTPUT );
    //pinMode ( TEMP_SENSOR_PIN , INPUT_PULLUP );
   // relay1.update(1);
  //  relay2.update(1);
  Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("\r\n");


  

//WiFi.softAP("TestAP", "");
  //wifiManager.resetSettings(); // Esto es para probar de conectar de nuevo al AP, se olvida de los SSID

  startSPIFFS();               // Start the SPIFFS and list all contents

  if (!wifiManager.autoConnect("AutoConnectAP")) {  // manager crea un AP para conectarse la primera vez
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  
  startOTA();                  // Start the OTA service


  startMDNS();                 // Start the mDNS responder

  startServer();               // Start a HTTP server with a file read handler and an upload handler

  startUDP();                  // Start listening for UDP messages to port 123

  WiFi.hostByName(ntpServerName, timeServerIP); // Get the IP address of the NTP server
  Serial.print("Local Ip " +WiFi.localIP());
  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);

  //sendNTPpacket(timeServerIP);
   // setSyncProvider(getTime);

//edit1.appendText("mamamam");
   
         program1.addCommand(&set1);
       program1.addCommand(&set2);
//               program1.addCommand(&keypadControl1);

       program1.addCommand(&control1);
     //  program1.addCommand(&logger1);
       program1.addCommand(&control2);
       //pause1.start();
 //      if1.addCommand(&set2);
   //   if1.addCommand(&set3);
 //      program1.addCommand(&if1);    //  esto esta produciendo error
//       pause1.start();
  page.addElement(&lblTime);
  page.addString("<br>");
 //   page.addElement(&control1);
  //     page.addElement(&keypad1);        // Parece que el Keypad da problemas, numero de elementos ????  El Keypad tambien tiene problemas !!!
     page.addElement(&relay1);
    page.addElement(&relay2);
    page.addElement(&relay3);
    page.addElement(&graphic1);
    page.addElement(&lblFreeHeap);

    page.addElement(&program1);       // El program es el que esta haciendo randoms problems

    //page.addElement(&tempSensor);
//    page.addElement(&analogIn1);
//    page.addElement(&digitalIn1);
//    page.addElement(&set1);
    //page.updateElements();
    //label1.update("newValue");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    page.getHtml();
    Serial.print("html:");
    Serial.println(ElementsHtml::html);
}



unsigned long lastUpdate;

///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              LOOP                                          ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////

void loop() {
  unsigned long currentMillis = millis();

 
  server.handleClient();                      // run the server
  ArduinoOTA.handle();                        // listen for OTA events

  
if (( currentMillis - lastUpdate ) > 1000 ) {   //  now it updates every 5 seconds
  Serial.println("Time: "+String(currentMillis/1000));
              timeNow = timeUNIX + ( (currentMillis - lastNTPResponse) / 1000 );

    tempSensor.update();// ElementsHtml::javaQueue.add("console.log('tmpSensorUpdate');");
    //analogIn1.update();
    //digitalIn1.update();
                lblTime.update(timeNow);

    lblFreeHeap.update();
    //control1.update();
    program1.run();
    lastUpdate = currentMillis;
     Serial.println("Heap Left: "+String(ESP.getFreeHeap(),DEC));//+" :Frag: " +String(ESP.getHeapFragmentation(),DEC)+"   Max-SIze = "+ String(ESP.getMaxFreeBlockSize()));
     String ss= page.getJavaQueue();            // Get the JavaScript Queue from page
     //Serial.println(ss);
     if (ss!="") webSocket.broadcastTXT(ss);   //  WebSoket necesita una variable, no puedo poner page.getJavaQueue directamente
   
}
    webSocket.loop();
yield();
}


///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              methods                                         ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////


void startOTA() { // Start the OTA service
  ArduinoOTA.setHostname(OTAName);
  // ArduinoOTA.setPassword(OTAPassword);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\r\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready\r\n");
}

void startSPIFFS() { // Start the SPIFFS and list all contents
  SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {                      // List the file system contents
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

void startMDNS() { // Start the mDNS responder
  MDNS.begin(mdnsName);                        // start the multicast domain name server
  Serial.print("mDNS responder started: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}

void startUDP() {
  Serial.println("Starting UDP");
  UDP.begin(123);                          // Start listening for UDP messages to port 123
  Serial.print("Local port:\t");
  Serial.println(UDP.localPort());
}

////////////////////////////////////////////
//      SERVER START                       //
////////////////////////////////////////////


void startServer() { // Start a HTTP server with a file read handler and an upload handler
  
   server.on("/edit.html",  HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
    server.send(200, "text/plain", "");
  }, handleFileUpload); 
   
   server.on("/list", HTTP_GET, handleFileList);
    server.on("/delete", HTTP_GET, handleFileDelete);
   server.on("/getData", handleGetData);
   server.on("/btnClick", HTTP_GET , handleBtnClick );

  // server.on("/reset",HTTP_GET , handleReset );
  server.on("/index.html", HTTP_GET , handleIndex1);
  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
  // and check if the file exists

  //Added
  //SSDP makes device visible on windows network
  server.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(server.client());
  });
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(DEVICE_NAME);
  SSDP.setURL("/");
  SSDP.begin();
  SSDP.setDeviceType("upnp:rootdevice");
  server.begin();                             // start the HTTP server
  Serial.println("HTTP server started.");
}

////////////////////////////////////////////////////////
////   SERVER HANDLERS                  ///////////////
///////////////////////////////////////////////////////

void handleNotFound() { // if the requested file or page doesn't exist, return a 404 not found error
  if (!handleFileRead(server.uri())) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}
void handleIndex1() {
//String reply;
 // reply+= page.getHtml();
 // Serial.println( reply);
  
  server.send(200,"text/html",page.getHtml());

}

void handleBtnClick() {                             //////////////   HANDLE BUTTON CLICK
  String buttonName = "undefinido";
  String buttonValue = "undefinido";
  String buttonDataValue = "undefinido";
  String reply = "console.log('no reply');";


  if (server.hasArg("button")) {
    buttonName = server.arg("button");
    buttonValue = server.arg("value");
    buttonDataValue = server.arg("datavalue");

      for (int i=0; i<page.elementCount; i++){
       // Serial.println(page.listOfElements[i]->getIdOwner(buttonName)->id);
          if (page.listOfElements[i]->id==(buttonName) )  {
                        reply=page.listOfElements[i]->postCallBack(page.listOfElements[i],buttonValue);
           }
      }
  for (int i=0; i<ElementsHtml::allHTMLElements.size(); i++){
        //Serial.println(ElementsHtml::allHTMLElements[i]->id);
          if (ElementsHtml::allHTMLElements[i]->id==(buttonName) )  {
                        Serial.println("sent post call back to: " + buttonName);
                        reply=ElementsHtml::allHTMLElements[i]->postCallBack(ElementsHtml::allHTMLElements[i],buttonValue);
           }
           
  }
          if (buttonName == "Time") {
            timeUNIX = buttonValue.toInt();
            lastNTPResponse = millis();
        }
  reply="console.log('fake handleBtnClick reply');";
  server.send(200, "text/plain", reply );

  

  }
}


void handleGetData(){
  String reply=page.getJavaQueue();
  reply = "";
//String   reply = "console.log('getData Reply');";
  Serial.println ("HandleGetData sub in SharpBox.ino "+reply);
          server.send(200, "text/plain", reply );

}



bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleFileUpload() { // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  String path;
  if (upload.status == UPLOAD_FILE_START) {
    path = upload.filename;
    if (!path.startsWith("/")) path = "/" + path;
    if (!path.endsWith(".gz")) {                         // The file server always prefers a compressed version of a file
      String pathWithGz = path + ".gz";                  // So if an uploaded file is not compressed, the existing compressed
      if (SPIFFS.exists(pathWithGz))                     // version of that file must be deleted (if it exists)
        SPIFFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: "); Serial.println(path);
    fsUploadFile = SPIFFS.open(path, "w");               // Open the file for writing in SPIFFS (create if it doesn't exist)
    path = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {                                   // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location", "/success.html");     // Redirect the client to the success page
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void handleFileDelete() {                     // Usage   192.168.1.13/delete?file=temp.csv
  if (!server.hasArg("file")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = server.arg("file");
  Serial.println("handleFileDelete: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");

  SPIFFS.remove(path);
  server.send(200, "text/plain", "deleted" + path);
}

void handleFileList() {
    if (!server.hasArg("dir")) {
        server.send(500, "text/plain", "BAD ARGS");
        return;
    }

    String path = server.arg("dir");
    Serial.println("handleFileList: " + path);
    Dir dir = SPIFFS.openDir(path);
    path = String();
    int memoryUsed = 0;
    String output = "[";
    while (dir.next()) {
        File entry = dir.openFile("r");
        if (output != "[") output += ',';
        bool isDir = false;
        output += "{\"type\":\"";
        output += (isDir) ? "dir" : "file";
        output += "\",\"name\":\"";
        output += String(entry.name()).substring(1) + "\"";
        output += "Size: " + String(entry.size());
        memoryUsed += entry.size();
        output += "\"}";
        output += "\n";
        entry.close();
    }

    output += "]";
    output += "    Total memory used: " + String (memoryUsed);

    server.send(200, "text/json", output);
}

//////////////////////////////////////////////////////////////////
///////////////  Web Socket Event
//////////////////////////////////////////////////////////////////
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    String ss;
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "console.log('Connected');");
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            ss = page.getJavaQueue();
            webSocket.sendTXT(num, ss );

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}



/*__________________________________________________________HELPER_FUNCTIONS__________________________________________________________*/

String formatBytes(size_t bytes) { // convert sizes in bytes to KB and MB
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

unsigned long getTime() { // Check if the time server has responded, if so, get the UNIX time, otherwise, return 0
  if (UDP.parsePacket() == 0) { // If there's no response (yet)
    return 0;
}
  UDP.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  // Combine the 4 timestamp bytes into one 32-bit number
  uint32_t NTPTime = (packetBuffer[40] << 24) | (packetBuffer[41] << 16) | (packetBuffer[42] << 8) | packetBuffer[43];
  // Convert NTP time to a UNIX timestamp:
  // Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
  const uint32_t seventyYears = 2208988800UL;
  // subtract seventy years:
  uint32_t UNIXTime = NTPTime - seventyYears ;
  return UNIXTime;
}


void sendNTPpacket(IPAddress& address) {
  Serial.println("Sending NTP request to " + String ( address));
  memset(packetBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode

  // send a packet requesting a timestamp:
  UDP.beginPacket(address, 123); // NTP requests are to port 123
  UDP.write(packetBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

long mapFloat ( float x , float in_min , float in_max  , float out_min , float out_max ) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
