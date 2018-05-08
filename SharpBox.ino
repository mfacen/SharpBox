#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>                 
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <WiFiUdp.h>

#include <FS.h>

#define ONE_HOUR 3600000UL

ESP8266WebServer server (80);       // create a web server on port 80
DNSServer dnsServer;
WiFiManager wifiManager;
WiFiUDP UDP;                   // Create an instance of the WiFiUDP class to send and receive UDP messages

const char *OTAName = "ESP8266";         // A name and a password for the OTA service
//const char *OTAPassword = "esp8266";
#define DEVICE_NAME "ESP8266 DEVICE"

const char* mdnsName = "sharpBox";        // Domain name for the mDNS responder, address is "basementController.local" no hace falta saber el IP

IPAddress timeServerIP;        // The time.nist.gov NTP server's IP address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;          // NTP time stamp is in the first 48 bytes of the message

const String nl = "/nl";
byte packetBuffer[NTP_PACKET_SIZE];      // A buffer to hold incoming and outgoing packets


File fsUploadFile;                                    // a File variable to temporarily store the received file




OneWire oneWireD4(D4);        // Set up a OneWire instance to communicate with OneWire devices
DallasTemperature tempSensors(&oneWireD4); // Create an instance of the temperature sensor class


long now = 0;

float temperature;
float humidity;
const unsigned long intervalNTP = ONE_HOUR; // Update the time every hour
unsigned long prevNTP = 0;
uint32_t timeUNIX = 0;                      // The most recent timestamp received from the time server
unsigned long lastNTPResponse = 0;

// ########################################
// CLASES
// ########################################

class Input {
  int value;
  int minValue;
  int maxValue;
  byte kind;
  const char *unit;
  const char *descriptor;
  int pin;
  
  public:
  Input ( byte type , byte _pin ) {
    pin = _pin;
    if ( type == 0 ) { // type Dsb18B20
      minValue = 0;
      maxValue = 150;
      unit = "grados";
      kind = type;
      descriptor = "ds18b20";
        tempSensors.setWaitForConversion(false); // Don't block the program while the temperature sensor is reading
      tempSensors.begin();                     // Start the temperature sensor
        if (tempSensors.getDeviceCount() == 0) {
    Serial.printf("No DS18x20 temperature sensor found on pin %d. Rebooting.\r\n");
    Serial.flush();;
  }
      
    }
  };

 
  
  void update() {
    if ( kind == 0 ) {
    //value = analogRead(pin);  // aqui en realidad va la sub para leer el sensor de temp.
    }
  }
  };

 class Output {
    public:

  int value;
  int minValue;
  int maxValue;
  byte kind;
  const char *unit;
  const char *descriptor;
  byte pin;
  Input *input;
  
  Output ( byte type , byte _pin) {
    pin = _pin;
    
    if ( type == 0 ) {  // type on/off
      minValue = 0;
      maxValue = 1;
      unit = "apagado/encendido";
      kind = type;
      descriptor = "On/Off";
    }
  };

    
  void update( int newValue) {
    value = newValue;
    if ( kind == 0 && newValue == 1 ) {
      digitalWrite(pin, HIGH);
    }
    if ( kind == 0 && newValue == 0 ) {
      digitalWrite(pin, LOW);
    }
  }
};

class Timer {
  public:

  long on;
  long off;
  unsigned char weekdays;
  boolean active;
  int output;
  int input;
  Timer ( long tmrOn, long tmrOff, unsigned char _weekdays , boolean _active){
    int input;
    on = tmrOn;
    off = tmrOff;
    weekdays = _weekdays;
    active = _active;
    int output;
  }

  void attachOutput (int _output){
    output = _output;
  }
  
  void process(){
    if ( ( now > on) && ( now < off ) ) {
      output = input;
    }
  }
};

Timer *timer1;
  
Output   output1(0,D3);

///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              SETUP                                          ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("\r\n");
  
  tempSensors.setWaitForConversion(false); // Don't block the program while the temperature sensor is reading
  tempSensors.begin();                     // Start the temperature sensor
  
  Input input1(0,D2);
  input1.update ();
  output1.update(1);
    Timer timer1 ( 2000, 5000 , char ('01110110'), false );
  //wifiManager.resetSettings(); // Esto es para probar de conectar de nuevo al AP, se olvida de los SSID
 
    startSPIFFS();               // Start the SPIFFS and list all contents
    
  if(!wifiManager.autoConnect("AutoConnectAP")) {   // manager crea un AP para conectarse la primera vez
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
  startOTA();                  // Start the OTA service


  startMDNS();                 // Start the mDNS responder

  startServer();               // Start a HTTP server with a file read handler and an upload handler

  WiFi.hostByName(ntpServerName, timeServerIP); // Get the IP address of the NTP server
  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);

  sendNTPpacket(timeServerIP);
  delay(500);
  
}

///////////////////////////////////////////////////////////////////////////
////                                                               ////////
///////              LOOP                                          ////////
////                                                               ////////
///////////////////////////////////////////////////////////////////////////

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevNTP > intervalNTP) { // Request the time from the time server every hour
    prevNTP = currentMillis;
    sendNTPpacket(timeServerIP);
  }

  uint32_t time = getTime();                   // Check if the time server has responded, if so, get the UNIX time
  if (time) {
    timeUNIX = time;
    Serial.print("NTP response:\t");
    Serial.println(timeUNIX);
    lastNTPResponse = millis();
  } else if ((millis() - lastNTPResponse) > 24UL * ONE_HOUR) {
    Serial.println("More than 24 hours since last NTP response. Rebooting.");
    Serial.flush();
    ESP.reset();
  }
    server.handleClient();                      // run the server
  ArduinoOTA.handle();                        // listen for OTA events
  
  delay (1000);
  
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

void startServer() { // Start a HTTP server with a file read handler and an upload handler
                    // go to 'handleFileUpload'
 // server.on("/list", HTTP_GET, handleFileList);
//  server.on("/delete", HTTP_GET, handleFileDelete);
 // server.on("/info.html", handleInfo);
  server.on("/set", HTTP_GET , handleSet);
 // server.on("/setUdp",HTTP_GET , handleSetUdp);
 // server.on("/reset",HTTP_GET , handleReset );
  server.on("/",HTTP_GET , handleIndex );
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

//

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

void handleIndex() {

    uint32_t actualTime = timeUNIX + (millis() - lastNTPResponse) / 1000;

    FSInfo fs_info;
  SPIFFS.info(fs_info);

  float fileTotalKB = (float)fs_info.totalBytes / 1024.0; 
  float fileUsedKB = (float)fs_info.usedBytes / 1024.0; 
  
  String msg;
msg = msg + "<!DOCTYPE html><head><meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\">"+
"<meta content=\"utf-8\" http-equiv=\"encoding\"> <script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script> <script src=\"tween-min.js\"></script> "+
  "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"> "+
  "<script>var temperature = "+String(temperature)+";\n"+
  "var humidity = "+String(humidity)+";\n"+
  "</script><head>"+
  "<body>Time:<form><input type=\"submit\"  formaction=\"/set\"/ name=\"setpoint\">Relay On</input></form></body></html>";
  server.send(200, "text/html", msg);

}

void handleSet() {
  String msg;
  if ( server.hasArg("AC1")) {
    if (server.arg("setpoint") == "ON"  ) {
      digitalWrite (D3,HIGH);
      msg += "AC1 -- ON ";
    }
    if (server.arg("setpoint") == "OFF"  ) {
      digitalWrite (D3,LOW);
        msg += "AC1 -- OFF ";
    }
    

  if (msg != "") server.send(200,"text/plain",msg);
  else server.send(200,"text/plain","Command not found");
}

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
  uint32_t UNIXTime = NTPTime - seventyYears;
  return UNIXTime;
}


void sendNTPpacket(IPAddress& address) {
  Serial.println("Sending NTP request");
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



