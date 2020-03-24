# Garage

## jeden Aufruf mit Sicheheit
    https, passwort/jwt 

## Funktionalität Garage
    Idee: RFID-Chip löst für n Zeiteinheiten die Möglichkeit aus, das Garagentor zu schliessen/öffnen und die Haustür zu öffenen
            RFID-Chip muss registriert sein (Möglichkeit zum Registrieren/Löschen von RFID-Chips)

            - RFID-Chip erkennen
            - Verwaltung für RFID-Chip (liste, erfassen, löschen), ID mit Namen


            - RFID-Leser        
            - Eingang für Taster für öffnen/schliessen 
            - Eingang für Ende-Taster Kontakt Garagentor
            - Ausgang für LED (rot/grün) für Status (Strom/online/gesperrt, Strom/connecting, Strom/Zeitintervall)
            - Ausgang Relais(?) für öffnen/schliessen Kontakt Garagentor-Motor

            - Protokoll der Nutzung (Uhrzeit? RTC, ntp, von Fritz!)

            Web-Kommandos
                GET /door     ->        Garagentor status -> nicht-zu/zu
                POST /door    ->        Garagentor tasten -> löst Motor-Steuerung aus
                
                GET /rfid     ->        Liste der RFIDs
                POST /rfid                  neue RFID anlegen
                DELETE /rfid/{id} ->        RFID löschen
                GET /rfidadmin      ->      Oberfläche RFID-Pflege

                ausgehend: Signal an Wohnung, dass Zeitintervall beginnt

            

## Funktionalität Wohnung
    Idee: Signal für Zeitintervall kommt, dann - wenn während dieser Zeit die Klingeltaste gedrückt wird,
          wird der Türöffner für 1 sekunde betätigt

            - Eingang: Klingeltaster
            - Austgang: Türtaster

          Web-Kommandos
            POST /doorinterval      -> Zeitintervall ausgelöst
            POST /door              -> Haustüre tasten



```
#include <stdbool.h>
#include <stdio.h>

typedef unsigned char byte;

#define TAG_LEN 4

byte OK[][TAG_LEN] = { { 0x01, 0x02, 0x03, 0x04 },
                       { 0x05, 0x06, 0x07, 0x08 },
                       { 0x09, 0x0A, 0x0B, 0x0C } };


bool compareOneTag(byte srcTag[TAG_LEN], byte testTag[TAG_LEN]) {
    bool result = true;
    for (int i=0; i<TAG_LEN; i++) {
        if (srcTag[i] != testTag[i]) {
            result = false;
        }
    }
    return result;
}



bool checkTag(byte testTag[TAG_LEN]) {
    for (int i=0; i < (sizeof(OK)/sizeof(OK[0])); i++) {
        if (compareOneTag(OK[i], testTag)) {
            return true;
        }
    }
    return false;
}


int main() {
    printf("Hello ...\n");
    byte TESTEE[TAG_LEN] = { 0x05, 0x06, 0x07, 0x08 };
    printf("result %d \n", checkTag(TESTEE));
    printf("... bye\n");

    return 0;
}
```