#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server(80); // create WebServer listening on port 80



String sketchName() {                             // Dateiname für den Admin Tab
  char file[sizeof(__FILE__)] = __FILE__;
  char * pos = strchr(file, '.'); *pos = '\0';
  return file;
}  



void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.printf("\nSketchname: %s\nBuild: %s\t\tIDE: %d.%d.%d\n%s\n\n", (__FILE__), (__TIMESTAMP__), ARDUINO / 10000, ARDUINO % 10000 / 100, ARDUINO % 100 / 10 ? ARDUINO % 100 : ARDUINO % 10, ESP.getFullVersion().c_str());

  spiffs();         // file system initiailisieren ..
  Connect();        // connect to WLAN and report IP
  admin();          // admin Seite anzeigen
  
  espboardLed();    // Steuerung der OnBoard LED ueber WebSeite ...
  // onboardLed();

  server.begin();
}



void loop() {
  server.handleClient();
  if (millis() < 0x2FFF || millis() > 0xFFFFF0FF) {
    runtime();
  }
}
