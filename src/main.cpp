#include <M5Stack.h>
#include <M5Btn.h>
#include <Pref.h>
#include <configuration.h>

// ----------------------------------------------------------------------------------------------------
// Webserver und Netzwerk

#include <WiFi.h>       
#include <WebServer.h>    
#include <AutoConnect.h>

WebServer Server;          
AutoConnect  portal(Server);
AutoConnectConfig config;
WiFiUDP Udp;   

// ----------------------------------------------------------------------------------------------------
// GUI

#include "Page.h"

// ----------------------------------------------------------------------------------------------------
// Z21

#include <Z21.h>


// ----------------------------------------------------------------------------------------------------
//

  AutoConnectText header("text", "Publishing the WiFi signal strength to MQTT channel. RSSI value of ESP8266 to the channel created on ThingSpeak", "font-family:serif;color:#4682b4;size:20;");
  ACText(caption1, "IP-Adresse der Z21");
  ACSubmit(save, "SPEICHERN", "/z21_save");
  AutoConnectInput input("z21Adress", "", "Z21-IP-Adresse", "192.168.178.200");
  AutoConnectAux  z21settings("/z21settings", "Z21-Einstellungen", true, { header, caption1, input, save });

  ACText(caption2, "Save parameters");
  ACSubmit(start, "START", "/mqtt_start"); 
  AutoConnectAux  aux2("/mqtt_save", "MQTT Setting", false, { caption2, start });
  AutoConnectAux  aux3("/mqtt_start", "MQTT Start");

void setup() {

  Pref::begin();

  M5.begin(true, false, true, false);
  Wire.begin(); // für Drehregler

  Serial.begin(115200);
  
  M5.Speaker.begin(); // ohne: Pfeifton

  // Bildschirm, solang nicht WLAN verbunden
  M5.lcd.fillScreen(colorBgInConnection);
  M5.lcd.setTextColor(TFT_RED, colorBgInConnection);
  M5.lcd.setTextSize(2);
  #define ConnWaitTime 20000
  M5.lcd.println("Verbinde mit WLAN ...\n");
  M5.lcd.setTextColor(colorFgInConnection, colorBgInConnection);
  M5.lcd.print(
    "Sollte das laenger als " + String(ConnWaitTime/1000) + "s"
    "dauern, spannt Franky ein eigenes WLAN ");
  M5.lcd.setTextColor(TFT_RED, colorBgInConnection);
  M5.lcd.print("esp32ap");
  M5.lcd.setTextColor(colorFgInConnection, colorBgInConnection);
  M5.lcd.println(" auf. "
    "Verbinden Sie sich damit. Bei einem Smartphone er-\n"
    "halten Sie meist danach\n"
    "\"im Netzwerk anmelden\".\nTippen Sie darauf und ge-\n"
    "langen auf eine Konfigura-\n"
    "tionswebseite. "
    "Alternativ rufen Sie im Browser");
  M5.lcd.setTextColor(TFT_RED, colorBgInConnection);
  M5.lcd.print("192.168.4.1/_ac");
  M5.lcd.setTextColor(colorFgInConnection, colorBgInConnection);
  M5.lcd.print(" auf.");
  M5.lcd.setTextSize(1);
 
  ////////// Webserver

  config.beginTimeout = ConnWaitTime; // Max. Blockierung von portal.begin()
  config.title = PRODUCT_NAME; // Webseitentitel statt "AutoConnect"
  config.ota = AC_OTA_BUILTIN; 
  config.autoReconnect = true; 
  // config.preserveAPMode = true;
  config.reconnectInterval = 1;   // Alle 1*30s Neuverbindungsversuch, wenn disconnected
  config.principle = AC_PRINCIPLE::AC_PRINCIPLE_RSSI; // Bei mehreren WLANs das signalstärkste
  // config.immediateStart = true; // nur verwenden, um AP-Mode zu erzwingen
  portal.config(config); 

  // noch ausprobieren
  // https://hieromon.github.io/AutoConnect/menu.html: Menüeinträge gezielt ausblenden
  // https://hieromon.github.io/AutoConnect/menu.html: ac_ austauschen

  // ------



  portal.join({z21settings, aux2, aux3 });

  portal.onConnect([](IPAddress& ipaddr){
    Serial.printf("Verbunden mit %s, IP %s\n", WiFi.SSID().c_str(), ipaddr.toString().c_str());
  });


  portal.begin();  //Verbinden kann dauern, keine Fortschrittsanzeige
  Serial.println("Webserver gestartet: " + WiFi.localIP().toString());

  Z21::setIPAddress(Pref::get(prefNameZ21IPAddr, "192.168.178.200"));

  // GUI initialisieren
  Page::begin(&M5.lcd);
  if (!Page::isBlocked()) Page::currentPage()->setVisible(true, true);

}

// ----------------------------------------------------------------------------------------------------
//

bool UDPServerInitialised = false;
long lastHeartbeatSent = 0;

void loop() {

  // WLAN-Verbindung managen
  portal.handleClient();

  // Da die WLAN-Verbindung noch nicht unbedingt abgeschlossen ist,
  // kann der UDP-Server auch erst nach dem Verbindungsaufbau gestartet werden
  if (WiFi.status() == WL_CONNECTED) {

    if (!UDPServerInitialised) {
      Udp.begin(Z21_PORT);
      UDPServerInitialised = true;
      // ez.addEvent(Z21::heartbeat, 1000); // regelmäßig mit Z21 sprechen
      Z21::init();
    }

    if (millis() - lastHeartbeatSent > Z21_HEARTBEAT) {
      Z21::heartbeat();
      lastHeartbeatSent = millis();
      Z21::LAN_X_GET_LOCO_INFO(4);
      Z21::LAN_GET_BROADCASTFLAGS();
    }

  }

  // Notifikationen erhalten
  Z21::receive();

  // Buttons und Drehregler einlesen
  M5Btn::loop();

}