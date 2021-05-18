@echo -------- Franky-Software aktualisieren -----------------------------------------------------------------
git pull
@echo -------- Libraries prüfen und ggf. aktualisieren -------------------------------------------------------
pio update
pio lib update
@echo -------- Sofern in userdata/ benutzerspezifische Dateien sind, werden diese nach data/ kopiert ---------
@>nul 2>nul dir /a-d /s "userdata\*" && (@copy /y userdata\* data) || (@echo          Keine Dateien kopiert)
@echo -------- Dateien in data/ in ESP32 hochladen -----------------------------------------------------------
pio run -t uploadfs 
@echo -------- Software bauen und in ESP32 hochladen ---------------------------------------------------------
pio run -t upload
@echo -------- Wenn es keine Fehler gab, ist Franky zweimal neu gestartet worden und nun betriebsbereit ------