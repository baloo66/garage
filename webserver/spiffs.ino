
const char HEADER[] PROGMEM = "HTTP/1.1 303 OK\r\nLocation:spiffs.html\r\nCache-Control: no-cache\r\n";
const char HELPER[] PROGMEM = R"(<form method="POST" action="/upload" enctype="multipart/form-data"><input type="file" name="upload"><input type="submit" value="Upload"></form>Lade die spiffs.html hoch.)";


void spiffs() {     // Funktionsaufruf "spiffs();" muss im Setup eingebunden werden
  Serial.println(SPIFFS.begin() ? "SPIFFS gestartet!" : "Sketch wurde mit \"no SPIFFS\" kompilliert!\n");
  server.on("/json", handleList);   // reagiert auf /json, /format und /upload ...
  server.on("/format", formatSpiffs);
  server.on("/upload", HTTP_POST, []() {}, handleUpload);
  server.onNotFound([]() {  // alles andere produziert 404
    if (!handleFile(server.urlDecode(server.uri())))
      server.send(404, "text/plain", "FileNotFound");
  });
}



void handleList() {                         // /json sendet Infos ueber alle Dateien an den Client als JSON-Array
  FSInfo fs_info; SPIFFS.info(fs_info);     // fuellt FSInfo Struktur mit Informationen ueber das Dateisystem
  Dir dir = SPIFFS.openDir("/");            // Auflistung aller im Spiffs vorhandenen Dateien
  String temp = "[";
  while (dir.next()) {
    if (temp != "[") temp += ',';
    temp += "{\"name\":\"" + dir.fileName().substring(1) + "\",\"size\":\"" + formatBytes(dir.fileSize()) + "\"}";
  }
  temp += ",{\"usedBytes\":\"" + formatBytes(fs_info.usedBytes * 1.05) + "\"," +             // Berechnet den verwendeten Speicherplatz + 5% Sicherheitsaufschlag
          "\"totalBytes\":\"" + formatBytes(fs_info.totalBytes) + "\",\"freeBytes\":\"" +    // Zeigt die Groesse des Speichers
          (fs_info.totalBytes - (fs_info.usedBytes * 1.05)) + "\"}]";                        // Berechnet den freien Speicherplatz + 5% Sicherheitsaufschlag
  server.send(200, "application/json", temp);
}



bool handleFile(String&& path) {      // /upload laedt Dateien hoch oder loescht diese
  if (server.hasArg("delete")) {
    SPIFFS.remove(server.arg("delete"));        // Datei löschen
    server.sendContent(HEADER);
    return true;
  }
  if (!SPIFFS.exists("/spiffs.html"))server.send(200, "text/html", HELPER);     // ermoeglicht das hochladen der spiffs.html
  if (path.endsWith("/")) path += "index.html";
  return SPIFFS.exists(path) ? ({File f = SPIFFS.open(path, "r"); server.streamFile(f, getContentType(path)); f.close(); true;}) : false;
}



void handleUpload() {                                       // Dateien vom Rechnenknecht oder Klingelkasten ins SPIFFS schreiben
  static File fsUploadFile;                                 // Hält den aktuellen Upload
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (upload.filename.length() > 30) {
      upload.filename = upload.filename.substring(upload.filename.length() - 30, upload.filename.length());  // Dateinamen auf 30 Zeichen kürzen
    }
    printf("handleFileUpload Name: /%s\n", upload.filename.c_str());
    fsUploadFile = SPIFFS.open("/" + server.urlDecode(upload.filename), "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    printf("handleFileUpload Data: %u\n", upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
    printf("handleFileUpload Size: %u\n", upload.totalSize);
    server.sendContent(HEADER);
  }
}



void formatSpiffs() {     // /format formatiert den Speicher
  SPIFFS.format();
  server.sendContent(HEADER);
}



const String formatBytes(size_t const& bytes) {     // lesbare Anzeige der Speichergroessen
  return bytes < 1024 ? static_cast<String>(bytes) + " Byte" : bytes < 1048576 ? static_cast<String>(bytes / 1024.0) + " KB" : static_cast<String>(bytes / 1048576.0) + " MB";
}



const String getContentType(const String& path) {     // ermittelt den Content-Typ
  using namespace mime;
  char buff[sizeof(mimeTable[0].mimeType)];
  for (size_t i = 0; i < maxType - 1; i++) {
    strcpy_P(buff, mimeTable[i].endsWith);
    if (path.endsWith(buff)) {
      strcpy_P(buff, mimeTable[i].mimeType);
      return static_cast<String>(buff);
    }
  }
  strcpy_P(buff, mimeTable[maxType - 1].mimeType);
  return static_cast<String>(buff);
}



bool freeSpace(uint16_t const& printsize) {     // testet, ob beim Speichern in Logdateien noch genuegend freier Platz verfügbar ist.
  FSInfo fs_info; SPIFFS.info(fs_info); // FSInfo Struktur mit Informationen ueber das Dateisystem fuellen
  //printf("Funktion: %s meldet in Zeile: %d FreeSpace: %s\n",__PRETTY_FUNCTION__,__LINE__,formatBytes(fs_info.totalBytes - (fs_info.usedBytes * 1.05)).c_str());
  return (fs_info.totalBytes - (fs_info.usedBytes * 1.05) > printsize) ? true : false;
}
