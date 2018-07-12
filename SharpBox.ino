
#include <OneWire.h>
#include <DallasTemperature.h>
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

#include <TimeAlarms.h>
#include <FS.h>

#define ONE_HOUR 3600000UL
#define TEMP_SENSOR_PIN D2
#define RELAY_1_PIN D5
#define RELAY_2_PIN D6
#include "data/index.h"
#include <webElements.h>
#include "/home/michel/Arduino/SharpBox/classes.h"

   Component btnRelay1("btn","btnRelay1","btnRelay1","Relay 1 OFF");
   Component btnRelay2("btn","btnRelay2","btnRelay2","Relay 2 OFF");
   Component lbl1("lbl","textLabel","textLabel","Label");
   Body body(1);
//#include "/data/MAIN_page.html"



ESP8266WebServer server (80);       // create a web server on port 80
DNSServer dnsServer;
WiFiManager wifiManager;
WiFiUDP UDP;                   // Create an instance of the WiFiUDP class to send and receive UDP messages

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
AlarmId logAlarm;



OneWire oneWire(TEMP_SENSOR_PIN);        // Set up a OneWire instance to communicate with OneWire devices
DallasTemperature tempSensors(&oneWire); // Create an instance of the temperature sensor class



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
uint32_t timeUNIX = 0;                      // The most recent timestamp received from the time server
unsigned long lastNTPResponse = 0;
unsigned long countdownSetTime = 0;




    Page page;

  RelayOutput   relay1 (RELAY_1_PIN,"Relay 1 110VAC","relay1");
  RelayOutput   relay2 (RELAY_2_PIN,"Relay 2 110VAC","relay2");
  Dsb18B20 tempSensor ( TEMP_SENSOR_PIN ,"Temperature Probe");   // habria que crearlo solo si encontro el sensor
  EditBox edit1 ("edit1");
  EditBox edit2 ("edit2");
  ActiveControl control1 ("Control 1" , &tempSensor , &edit1 , &relay1 , &edit2 );
  Set set1 ("set1",&relay2);
  
///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              SETUP                                          ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////

  void setup() {
    page.addElement(&relay1);
    page.addElement(&relay2);
    page.addElement(&tempSensor);
    page.addElement(&edit1);
    page.addElement(&edit2);
    page.addElement(&control1);
    page.addElement(&set1);
    
  // put your setup code here, to run once:
   // pinMode ( RELAY_1_PIN , OUTPUT );
    //pinMode ( RELAY_2_PIN , OUTPUT );
    pinMode ( TEMP_SENSOR_PIN , INPUT_PULLUP );
    relay1.update(1);
    relay2.update(1);
  Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("\r\n");

  tempSensors.setWaitForConversion(false); // Don't block the program while the temperature sensor is reading
  tempSensors.begin();                     // Start the temperature sensor
  if (tempSensors.getDeviceCount() == 0) {
    Serial.print("No DS18x20 temperature sensor found on pin %d. Rebooting.\r\n   pin:"+String( TEMP_SENSOR_PIN));
    Serial.flush();
    //ESP.reset();
  }
  

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

  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);

  sendNTPpacket(timeServerIP);
   // setSyncProvider(getTime);

}




///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              LOOP                                          ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevNTP > intervalNTP) { // Request the time from the time server every hour
    Serial.print ( String(currentMillis - prevNTP ) + "-- " + String (intervalNTP ));
    prevNTP = currentMillis;
    sendNTPpacket(timeServerIP);
  }
  uint32_t timeNtp = getTime();                   // Check if the time server has responded, if so, get the UNIX time
  if (timeNtp) {
    timeUNIX = timeNtp;
    //setTime(timeUNIX);
    Serial.print("NTP response:\t");
    Serial.println(timeUNIX);
    lastNTPResponse = millis();
    setTime(timeUNIX);
    Serial.println(day()+"-"+hour());
  } else if ((millis() - lastNTPResponse) > 24UL * ONE_HOUR) {
    Serial.println("More than 24 hours since last NTP response. Rebooting.");
    Serial.flush();
    ESP.reset();
  }

  server.handleClient();                      // run the server
  ArduinoOTA.handle();                        // listen for OTA events

  tempSensor.update();
    //        tempSensors.requestTemperatures(); // Request the temperature from the sensor (it takes some time to read it)

  delay (100);
  control1.update();
  
  //temperature =  tempSensors.getTempCByIndex(0);
  if ( tempControl == "true" ) {
    if ( tempDirection == "Heat" ){
      if ( temperature > tempSetpoint ) relay1.update(1);
      else relay1.update(0);
    }
    if ( tempDirection == "Cool" ){
      if ( temperature < tempSetpoint ) relay1.update(1);
      else relay1.update(0);
    }
  }

  if ( chkCountdownTimer == "true" ) {
    if ( millis() - countdownSetTime > countdownMinutes * 60000 ) {
        relay1.update(1); //  (apagado)
      }
    }
    
    

  if ( timeUNIX == 0 )  {                                    // If we didn't receive an NTP response yet, send another request
    sendNTPpacket(timeServerIP);
  delay(500);

}
}


// Subrutines

void saveDataLog() {
 File tempLog ;
 uint32_t actualTime = timeUNIX + (millis() - lastNTPResponse) / 1000;

      tempLog = SPIFFS.open("/dataLog.csv", "a"); // Write the time and the temperature to the csv file
      tempLog.print(actualTime);
      tempLog.print(',');
      tempLog.print(temperature);
      tempLog.print(',');
      tempLog.print(relay1.getState());
      tempLog.print(',');
      tempLog.print(relay2.getState());
      tempLog.print(',');
      tempLog.println( String (  countdownMinutes - ( millis()-countdownSetTime) / 60000 ) ); // output goes from 20 ( 0 ) to 38 ( 180 )
      
      tempLog.close();
      Serial.println("Logging data");
    }

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
   
  // server.on("/list", HTTP_GET, handleFileList);
  //  server.on("/delete", HTTP_GET, handleFileDelete);
   server.on("/connect", handleConnect);
   server.on("/readTimer", HTTP_GET , handleTimer );
   server.on("/btnClick", HTTP_GET , handleBtnClick );

   server.on("/set", HTTP_GET , handleSet);
   server.on("/start", HTTP_GET , handleStart);
  // server.on("/reset",HTTP_GET , handleReset );
  server.on("/index1", HTTP_GET , handleIndex1);
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
  String reply = "<html><head>  <link rel='stylesheet' type='text/css' href='style.css'></head>";
  reply+= btnRelay1.getHtml();
  reply+= btnRelay2.getHtml();
  reply+= lbl1.getHtml()+"<br>";

  reply+= tempSensor.getHtml()+"<br>";
  reply+= relay1.getHtml()+"<br>";
  reply+= relay2.getHtml()+"<br>";
  reply+= edit1.getHtml()+"<br>";
  reply+= control1.getHtml()+"<br>";
  reply+= set1.getHtml()+"<br>";
  
    reply+= body.getJavaScript();
reply+="<span id='errorLabel'></span>";
reply+= "</body></html>";  
  server.send(200,"text/html",reply);

}

void handleBtnClick() {                             //////////////   HANDLE BUTTON CLICK
  String buttonName = "undefined";
  String buttonValue = "undefined";
  String buttonDataValue = "undefined";
  String reply = "Undefined";


  if (server.hasArg("button")) {
    buttonName = server.arg("button");
    buttonValue = server.arg("value");
    buttonDataValue = server.arg("datavalue");

      for (int i=0; i<page.elementCount; i++){
        Serial.println(page.listOfElements[i]->id);
          if (page.listOfElements[i]->id == buttonName) {
            
            //RelayOutput el =  static_cast<RelayOutput> (page.listOfElements1[i]);
            reply=page.listOfElements[i]->postCallBack(buttonValue,buttonDataValue);
            //relay1.postCallBack();
            //el->postCallBack();
            //reply = page.listOfElements[i]->name +" relay Flipped";
          }
      }
      
//    if (buttonName == "relay1") {
//      page.listOfElements[0]->postCallBack();
//      reply =  (page.listOfElements[0]->name);//RelayOutputpage.listOfElements[1]->name;//->name ;//+" relay Flipped";
//    }
      
      
        server.send(200, "text/plain", reply );
  }
}


void handleSet() {
  String msg;
  

  if ( server.hasArg("Relay")) {
    String t_relay = server.arg("Relay"); //Refer  xhttp.open("GET", "setLED?RELAYstate="+on/off, true);

    if ( t_relay == "1" ) {
      if ( server.arg("State") == "1" ) {
        relay1.update(0);
        Serial.println("Relay 1 on");
      }

      if ( server.arg("State") == "0") {
       relay1.update(1);
       Serial.println("Relay 1 off");
     }
   }
   if ( t_relay == "2" ) {
    if ( server.arg("State") == "1" ) {
      relay2.update(0);
      Serial.println("Relay 2 on");
    }

    if ( server.arg("State") == "0") {
     relay2.update(1);
     Serial.println("Relay 2 off");
   }
 }
    server.send(200, "text/plain", t_relay + " " +  (server.arg("State") == "0") ? "OFF" : "ON" ) ; //Send web page
  }

  // if (msg != "") server.send(200,"text/plain",msg);
  //else server.send(200,"text/plain","Command not found");


}


void handleTimer() {
  
 FSInfo fs_info;
 SPIFFS.info(fs_info);

 float fileTotalKB = (float)fs_info.totalBytes / 1024.0; 
 float fileUsedKB = (float)fs_info.usedBytes / 1024.0; 

 
 server.send(200, "text/plain",  relay1.getState() + "," + relay2.getState() + "," +  String (temperature) + "," + String (hour(now())) + "," + String(minute(now())) +
   "," +  String (  countdownMinutes - ( millis()-countdownSetTime) / 60000 )   + "," + (processRunning?"Run":"Stop"  ) + "," +
   String(countdownMinutes) + "," + String (tempSetpoint) + "," + String (loggerRunning) + "," + String ( loggerMinutes ) + "," +
   String( fileTotalKB - fileUsedKB ) );
} 



void handleStart(){

  
  if ( server.hasArg("tempSetpoint") ){
    tempSetpoint = server.arg("tempSetpoint").toInt();
  }
  if ( server.hasArg("countdownMinutes") ){
    countdownMinutes = server.arg("countdownMinutes").toInt();
  }
  if ( server.hasArg("chkTempControl") ){
    tempControl = ( server.arg("chkTempControl"));
  }
  if ( server.hasArg("tempDirection") ){
    tempDirection = ( server.arg("tempDirection"));
  }
  if ( server.hasArg("chkCountdownTimer") ){
    chkCountdownTimer = ( server.arg("chkCountdownTimer"));
    if (chkCountdownTimer == "true" ) countdownSetTime = millis();
  }
  if ( server.hasArg("loggerMinutes") ){
    loggerMinutes = server.arg("loggerMinutes").toInt();
  }
  
  if ( server.hasArg("chkLogger") ){
    String chkLogger =  server.arg("chkLogger");
    Serial.println(" logger argument = " + chkLogger);
    if (chkLogger == "true" ) {
      if (loggerRunning == false){
        loggerRunning = true; 
        Serial.println("loggerRunning = true");
        saveDataLog();
        logAlarm  = Alarm.timerRepeat( 10 , saveDataLog );
      }
    }
    else if (chkLogger == "false" ) {
     Serial.println("loggerRunning = false");

     loggerRunning = false;
     Alarm.free(logAlarm);
     
   }

 }


      server.send(200, "text/plain", "Running");//   temperature:"+String(tempSetpoint)+"   Minutes: "+ String(countdownMinutes) + " : Temperature Control is: "+ tempControl+" -- Cool/Heat: " + tempDirection+" -  Countdown Timer: " + chkCountdownTimer) ;
      processRunning = true;
      
    }

    void handleConnect () {
     // wifiManager.startConfigPortal("OnDemandAP");
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



