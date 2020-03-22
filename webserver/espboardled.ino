void espboardLed() {
  pinMode(LED_BUILTIN, OUTPUT);     // OnBoardLed Esp Nodemcu Wemos
  server.on( "/led", espLed );
}

void espLed() {                  //Html Seite
  if (server.hasArg("led")) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));        // LED umschalten
    Serial.println(digitalRead(LED_BUILTIN) ? "LED ist aus" : "LED ist an");
  }
  String temp = "<!DOCTYPE html><html lang='de'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  temp += "<style>body{background-color:#87cefa;display:flex;flex-flow:column;align-items:center;}input{height:40px;width:120px;font-size:16px;}";
  temp += "[value$=n]{background-color:#adff2f;}[value$=s]{background-color:red;}</style><title>Onboard Led</title>";
  temp += "</head><body><h2>Onboard Led schalten</h2><h3>Esp8266 Nodemcu Wemos</h3><p>LED ist ";
  temp += digitalRead(LED_BUILTIN) ? "aus" : "an";
  temp += "<p><form action='/led' method='POST'><input name='led' type='submit' value='LED ";
  temp += digitalRead(LED_BUILTIN) ? "Ein" : "Aus";
  temp += "'></form></body></html>";
  server.send(200, "text/html", temp);
}
  
