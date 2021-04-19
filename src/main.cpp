/*
  FRANKY - Drahtloser Handregler für X-Bus-basierte Zentralen (Z21, DR5000, XP Multi)
*/

#include <M5Stack.h>
#include <M5Btn.h>
#include <Pref.h>
#include <configuration.h>

// ----------------------------------------------------------------------------------------------------
// Webserver und Netzwerk

#include <WiFi.h>        
#include "Webserver.h"
WiFiUDP Udp;

// ----------------------------------------------------------------------------------------------------
// GUI

#include "Page.h"

// ----------------------------------------------------------------------------------------------------
// Z21

#include <Z21.h>

// ----------------------------------------------------------------------------------------------------
//

void setup() {

  // Präferenzen
  Pref::begin();

  // M5Stack
  M5.begin(true, false, true, false);
  M5.Speaker.begin(); // ohne: Pfeifton
  M5.Power.begin();
  Serial.begin(115200);
  Wire.begin(); // für Drehregler

  // Filesystem
  SPIFFS.begin();

  // Webserver, und wenn nötig, Access Point für erstmalige Credentials-Eingabe

  // Kommentare entfernen, um für Testzwecke AP-Modus zu erzwingen:
  // Pref::set(prefNameSSID, "");
  // Pref::set(prefNamePasswd, "");
  Webserver::webconfig(); 

  // Z21-Adresse bekanntgeben (kann über Webserver geändert werden)
  Z21::setIPAddress(Pref::get(prefNameZ21IPAddr, Z21_DEFAULT_ADDR));

  // Adressoffset?
  Z21::setAddrOffs(Pref::get(prefNameGerdOffs, "0").toInt());

  // GUI initialisieren
  Page::begin(&M5.lcd);
  if (!Page::isBlocked()) Page::currentPage()->setVisible(true, true);

}

// ----------------------------------------------------------------------------------------------------
//

bool UDPServerInitialised = false;
long lastHeartbeatSent = -Z21_HEARTBEAT; // gleich anfangs Heartbeat erzwingen

void loop() {

  // Da die WLAN-Verbindung noch nicht unbedingt abgeschlossen ist,
  // kann der UDP-Server auch erst nach dem Verbindungsaufbau gestartet werden
  if (WiFi.status() == WL_CONNECTED) {

    if (!UDPServerInitialised) {
      Udp.begin(Z21_PORT);
      UDPServerInitialised = true;
      Z21::init();
    }

    if (millis() - lastHeartbeatSent > Z21_HEARTBEAT) {
      Z21::heartbeat();
      lastHeartbeatSent = millis();
    }

  }

  // Notifikationen erhalten
  Z21::receive();

  // Buttons und Drehregler einlesen
  M5Btn::loop();

}