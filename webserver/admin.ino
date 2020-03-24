const char* const PROGMEM flashChipMode[] = {"QIO", "QOUT", "DIO", "DOUT", "Unbekannt"};



void admin() {      // Funktionsaufruf "admin();" muss im Setup eingebunden werden
  File file = SPIFFS.open("/config.json", "r");
  if (file) {
    String newhostname = file.readStringUntil('\n');
    if (newhostname != "") {
      WiFi.hostname(newhostname.substring(1, newhostname.length() - 1));
      file.close();
    }
  }
  server.on("/admin/renew", handlerenew);
  server.on("/admin/once", handleonce);
  server.on("/reconnect", []() {
    server.send(204, "", "");
    WiFi.reconnect();
  });
  server.on("/restart", []() {
    server.send(204, "", "");
    //speichern();      //Wenn Werte vor dem Neustart gespeichert werden sollen
    ESP.restart();
  });
}



// 
void handlerenew() {    // Um die am ADC-Pin anliegende externe Spannung zu lesen, verwende analogRead (A0)
  server.send(200, "application/json", "[\"" + runtime() + "\",\"" + WiFi.RSSI() + "\",\"" + analogRead(A0) + "\"]");     // Json als Array
}
/*
  ADC_MODE(ADC_VCC);
  void handlerenew() {   // Zum Lesen der Modulversorgungsspannung (VCC), verwende ESP.getVcc()
  server.send(200, "application/json", "[\"" + runtime() + "\",\"" + WiFi.RSSI() + "\",\"" + ESP.getVcc() + "\"]");     // Json als Array
  }
*/


void handleonce() {
  if (server.arg(0) != "") {
    WiFi.hostname(server.arg(0));
    File f = SPIFFS.open("/config.json", "w");                    // Datei zum schreiben öffnen
    f.printf("\"%s\"\n", WiFi.hostname().c_str());
    f.close();
  }
  String temp = "{ \"File\": \""        + sketchName() + "\", " +
                  "\"Build\": \""       + __DATE__ + " " + __TIME__ + "\", " +
                  "\"SketchSize\": \""  + formatBytes(ESP.getSketchSize()) + "\", " +
                  "\"SketchSpace\": \"" + formatBytes(ESP.getFreeSketchSpace()) + "\", " +
                  "\"LocalIP\": \""     + WiFi.localIP().toString() + "\", " +
                  "\"Hostname\": \""    + WiFi.hostname() + "\", " +
                  "\"SSID\": \""        + WiFi.SSID() + "\", " +
                  "\"GatewayIP\": \""   + WiFi.gatewayIP().toString() + "\", " +
                  "\"Channel\": \""     + WiFi.channel() + "\", " +
                  "\"MacAddress\": \""  + WiFi.macAddress() + "\", " +
                  "\"SubnetMask\": \""  + WiFi.subnetMask().toString() + "\", " +
                  "\"BSSID\": \""       + WiFi.BSSIDstr() + "\", " +
                  "\"ClientIP\": \""    + server.client().remoteIP().toString() + "\", " +
                  "\"DnsIP\": \""       + WiFi.dnsIP().toString() + "\", " +
                  "\"ResetReason\": \"" + ESP.getResetReason() + "\", " +
                  "\"CpuFreqMHz\": \""  + F_CPU / 1000000 + "\", " +
                  "\"FreeHeap\": \""    + formatBytes(ESP.getFreeHeap()) + "\", " +
                  "\"ChipSize\": \""    +  formatBytes(ESP.getFlashChipSize()) + "\", " +
                  "\"ChipSpeed\": \""   + ESP.getFlashChipSpeed() / 1000000 + "\", " +
                  "\"ChipMode\": \""    + flashChipMode[ESP.getFlashChipMode()] + "\", " +
                  "\"IdeVersion\": \""  + ARDUINO + "\", " +
                  "\"CoreVersion\": \"" + ESP.getCoreVersion() + "\", " +
                  "\"SdkVersion\": \""  + ESP.getSdkVersion() + "\" }";
  server.send(200, "application/json", temp);
}



String runtime() {
  static uint8_t rolloverCounter {0};
  static uint32_t previousMillis {0};
  uint32_t currentMillis {millis()};
  if (currentMillis < previousMillis) rolloverCounter++;       // prüft Millis Ueberlauf
  previousMillis = currentMillis;
  uint32_t sek {(0xFFFFFFFF / 1000 ) * rolloverCounter + (currentMillis / 1000)};
  char buf[20];
  snprintf(buf, sizeof(buf), "%d Tag%s %02d:%02d:%02d", sek / 86400, sek < 86400 || sek >= 172800 ? "e" : "", sek / 3600 % 24, sek / 60 % 60, sek % 60);
  return buf;
}
