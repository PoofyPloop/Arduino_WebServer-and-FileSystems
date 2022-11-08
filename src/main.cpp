// @author PoofyPloop

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

// 1-Wire sensor communication libary 
#include <OneWire.h> 
// DS18B20 sensor library 
#include <DallasTemperature.h> 
// arduinoJSON library
#include <ArduinoJson.h>

// Pin that the  DS18B20 is connected to 
const int oneWireBus = D3;      

// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(oneWireBus); 

// Pass our oneWire reference to Dallas Temperature sensor  
DallasTemperature DS18B20(&oneWire); 

// *********************************************************************
// data

// WiFi access parameters
const char* ssid = ""; 
const char* password = ""; 

// web server hosted internally to the IoT device.  Listen on TCP port 80,
// which is the default HTTP port.
ESP8266WebServer webServer(80);

// Sensor address variable
DeviceAddress sensorAddress;
char buffer[17];

// *********************************************************************
// this function examines the URL from the client and based on the extension
// determines the type of response to send.
bool loadFromLittleFS(String path) {
  bool bStatus;
  String contentType;
  
  // set bStatus to false assuming this will not work.
  bStatus = false;

  // assume this will be the content type returned, unless path extension 
  // indicates something else
  contentType = "text/plain";

  // DEBUG:  print request URI to user:
  Serial.print("Requested URI: ");
  Serial.println(path.c_str());

  // if no path extension is given, assume index.html is requested.
  if(path.endsWith("/")) path += "index.html";

  // look at the URI extension to determine what kind of data to 
  // send to client.
  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) contentType = "text/html";
  else if (path.endsWith(".htm"))  contentType = "text/html";
  else if (path.endsWith(".css"))  contentType = "text/css";
  else if (path.endsWith(".js"))   contentType = "application/javascript";
  else if (path.endsWith(".png"))  contentType = "image/png";
  else if (path.endsWith(".gif"))  contentType = "image/gif";
  else if (path.endsWith(".jpg"))  contentType = "image/jpeg";
  else if (path.endsWith(".ico"))  contentType = "image/x-icon";
  else if (path.endsWith(".xml"))  contentType = "text/xml";
  else if (path.endsWith(".pdf"))  contentType = "application/pdf";
  else if (path.endsWith(".zip"))  contentType = "application/zip";

  // try to open file in LittleFS filesystem
  File dataFile = LittleFS.open(path.c_str(), "r");
  // if dataFile <> 0, then it was opened successfully.
  if ( dataFile ) {
    if (webServer.hasArg("download")) contentType = "application/octet-stream";
    // stream the file to the client.  check that it was completely sent.
    if (webServer.streamFile(dataFile, contentType) != dataFile.size()) {
      Serial.println("Error streaming file: " + String(path.c_str()));
    }
    // close the file
    dataFile.close();
    // indicate success
    bStatus = true;
  }

  return bStatus;
}


// *********************************************************************
void handleWebRequests(){
  if (!loadFromLittleFS(webServer.uri())) {
    // file not found.  Send 404 response code to client.
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += webServer.uri();
    message += "\nMethod: ";
    message += (webServer.method() == HTTP_GET) ? "GET":"POST";
    message += "\nArguments: ";
    message += webServer.args();
    message += "\n";
    for (uint8_t i=0; i<webServer.args(); i++){
      message += " NAME:"+webServer.argName(i) + "\n VALUE:" + webServer.arg(i) + "\n";
    }
    webServer.send(404, "text/plain", message);
    Serial.println(message);
  }
}

void handleSensorData() {
  if (webServer.method() == HTTP_GET) {

    char split[3];
    byte index = 0;
    for (byte i = 0; i < 8; i++) {
        snprintf(split,3,"%02X",sensorAddress[i]); // convert a byte to a character string
        buffer[index] = split[0];
        index++;
        buffer[index] = split[1];
        index++;
    }

    // Get temps
    DS18B20.requestTemperatures();

    // Send celcius temperature to client
    if (webServer.argName(0) == "type" && webServer.arg(0) == "C") {
      webServer.send(200, "application/json", "{\"temperature\": " + String(DS18B20.getTempCByIndex(0)) + ", \"address\": \"" + buffer + "\"}");
    }

    // Send fahreheit temperature to client
    if (webServer.argName(0) == "type" && webServer.arg(0) == "F") {
      webServer.send(200, "application/json", "{\"temperature\": " + String(DS18B20.getTempFByIndex(0)) + ", \"address\": \"" + buffer + "\"}");
    }

  }
}

// *********************************************************************
// setup()
//
void setup() {
  String sStatus;

  // setup serial debug port
  Serial.begin(115200);
  
  Serial.println("\n\nCOMP-10184 - IoT Programming");
  Serial.println("Web Server Tutorial\n");

  //Initialize File System
  LittleFS.begin();
  Serial.println("File System Initialized");

  // attempt to connect to WiFi network
  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // condition that indicated whether the sensor is disconnected or not. if connected, returns sensor address
  if (!DS18B20.getAddress(sensorAddress, 0)) 
  {
    Serial.println("No DS18B20 temperature sensores are installed");
  }
  else {
    Serial.print("Found DS18B20 sensor with address: ");
    for (uint8_t i = 0; i < 8 ; i++) {
      if (sensorAddress[i] < 16) {}
        Serial.print(sensorAddress[i], HEX);
    }
    Serial.println();
  }

  // handler for version inforamtion
  webServer.on("/temperature", handleSensorData);

  // send all other web requests here by default.  
  // The filesystem will be searched for the requested resource
  webServer.onNotFound(handleWebRequests);  

  // start web server and print it's address.
  webServer.begin();
  Serial.printf("\nWeb server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

// *********************************************************************
// loop ()
//
void loop() {
  webServer.handleClient();
}
