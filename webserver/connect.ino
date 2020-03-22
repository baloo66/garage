const char* ssid = "tinkerbell";              // SSID des WLANs
const char* password = "cafebabecafebabe";    // Password der WLANs



void Connect() {      // Funktionsaufruf "Connect();" muss im Setup eingebunden werden
  // WiFi.disconnect();     // sollte ESP den AP-Modus nicht verlassen ...
  WiFi.persistent(false);   // Netzwerkname oder Passwort sollen nicht in den Flash geschrieben werden
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);   // am WLAN anmelden

  byte i = 0;
  while (WiFi.status() != WL_CONNECTED) { // schleife, bis WLAN Anmeldung funktioniert hat - oder eben nicht :-(
    pinMode(LED_BUILTIN, OUTPUT);         // waehrend der Anmeldung lassen wir die LED blinken
    digitalWrite(LED_BUILTIN, 0);
    delay(500);
    digitalWrite(LED_BUILTIN, 1);
    delay(500);
    Serial.printf(" %d sek\n", ++i);      // und geben eine NAchricht aus
    if (i > 9) {    // beim 10. Versuch abbrechen und neu beginnen ...
      Serial.print("\nVerbindung zum AP fehlgeschlagen !\n\n");
      ESP.restart();
    }
  }
  Serial.println("\nVerbunden mit: " + WiFi.SSID());    // Ausgabe derInformationen und der IP
  Serial.println("Esp8266 IP: " + WiFi.localIP().toString());
}
