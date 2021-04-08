@echo "-------- Franky-Software aktualisieren -----------------------------------------------------------------"
git pull
@echo "-------- Libraries prüfen und ggf. aktualisieren -------------------------------------------------------"
pio lib update
@echo "-------- Dateien in data/ in ESP32 hochladen -----------------------------------------------------------"
pio run -t uploadfs 
@echo "-------- Software bauen und in ESP32 hochladen ---------------------------------------------------------"
pio run -t upload
@echo "-------- Wenn es keine Fehler gab, ist Franky zweimal neu gestartet worden und nun betriebsbereit ------"