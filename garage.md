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
