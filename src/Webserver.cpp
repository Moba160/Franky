/*
    Zunächst Angebot eines Access Point, damit man sich über hier definierte
    Webseite auf das so aufgespannte eigene WLAN-Netz aufschalten kann und
    darüber die Zugangsdaten (und andere Konfigurationsdaten) für das später
    eigentlich zu verwendende WLAN-Netz eingeben kann

*/

#include <SPIFFS.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include "./Webserver.h"
// #include "Configuration.h"
#include "configuration.h"
#include "Menu.h"

#include <AsyncTCP.h>
#include <AsyncElegantOTA.h>
#include <ESPmDNS.h>

#include "Pref.h"
#include <Z21.h>

#include "Page.h"

bool Webserver::inAPMode = true;
WiFiMulti wifiMulti;

extern int split(String s, char listSeparator, String elements[]);
extern String htmlPageReplace(String var);
extern void wsReceived(String received);

// Webserver
#define HTTP_PORT 80
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer Webserver::webServer(HTTP_PORT);

// Websocket
AsyncWebSocket ws("/ws"); // "/ws" auch in Javascript verwenden!
AsyncWebSocketClient* Webserver::wsClient;

// ----------------------------------------------------------------------------------------------------
//

void Webserver::webconfig() {

  webServer.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {

    // Index-Seite
    if (!inAPMode) {

      request->send(SPIFFS, "/system.html", String(), false, processor);

    // Formular für Eingabe WLAN-Daten
    } else {

      M5.lcd.fillScreen(TFT_BLUE);
      M5.lcd.setTextColor(TFT_WHITE);
      M5.lcd.setTextDatum(CC_DATUM);
      M5.lcd.drawString("2", TFT_W / 2, TFT_H * 0.20, 8);
      M5.lcd.drawString("Client hat sich verbunden - ", TFT_W / 2, TFT_H * 0.70, 2);
      M5.lcd.drawString("dort im Browser die Konfiguration vornehmen", TFT_W / 2, TFT_H * 0.80, 2);
      request->send(200, "text/html",
      "<style>"
      "html { font-family: Helvetica; }"
      "body { background-color: #0000FF; }"
      "td { padding:10px; }"
      "</style>"
      "<h1 style=\"color:#FFFFFF\"><font size=+4>2 </font>" PRODUCT_NAME " konfigurieren</h1>"
      "<font size+=2>"
      "<form action=\"/get\">"
      "<table border=0 style=\"color:#FFFFFF\">"
      "<tr><td>SSID</td><td><input type=\"text\" name=\"ssid\" value=\"" + Pref::get(prefNameSSID) + "\"></td></tr>"
      "<tr><td>Passwort</td><td><input type=\"text\" name=\"password\" value=\"" + Pref::get(prefNamePasswd) + "\"></td></tr>"
      "<tr><td>Z21-Adresse</td><td><input type=\"text\" name=\"z21addr\" value=\"" + Pref::get(prefNameZ21IPAddr, Z21_DEFAULT_ADDR) + "\"></td></tr>"
      "<tr><td><input type=\"submit\" value=\"Speichern\"></td><td></td></tr>"
      "</table>"
      "<p/>"
      "</form>"
      "</font>"
      );
    }
  });

  // Daraufhin zurückgelieferte ausgefüllte Daten des Formulars
  webServer.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String ssid, passwd, z21addr;
      bool credentialsChanged = false;
      if (request->hasParam("ssid")) {
          ssid = request->getParam("ssid")->value();
          Serial.printf("SSID: %s\n", ssid.c_str());
          if (Pref::get(prefNameSSID) != ssid) {
              Pref::set(prefNameSSID, ssid);
              credentialsChanged = true;
          }
      } 
      if (request->hasParam("password")) {
          passwd = request->getParam("password")->value();
          Serial.printf("password: %s\n", passwd.c_str());
          if (Pref::get(prefNamePasswd) != passwd) {
              Pref::set(prefNamePasswd, passwd);
              credentialsChanged = true;
          }
      } 
      if (request->hasParam("z21addr")) {
          z21addr = request->getParam("z21addr")->value();
          Serial.printf("z21addr: %s\n", z21addr.c_str());
          Pref::set(prefNameZ21IPAddr, z21addr);
      } 

      M5.lcd.fillScreen(TFT_BLUE);
      M5.lcd.setTextColor(TFT_WHITE);
      M5.lcd.setTextDatum(CC_DATUM);
      M5.lcd.drawString("3", TFT_W / 2, TFT_H * 0.20, 8);
      M5.lcd.drawString("Konfigurationsdaten erhalten" + String(credentialsChanged ? " - Neustart" : ""), 
          TFT_W / 2, TFT_H * 0.80, 2);
  
      String message = "Daten empfangen. "
          + String(credentialsChanged ? "Neustart, da sich WLAN-Daten ge&auml;ndert haben." : "");
      request->send(200, "text/html", 
          "<style>"
          "html { font-family: Helvetica; }"
          "body { background-color: #0000FF; color: #FFFFFF; }"
          "</style>"
          "<body>"
          "<font size=+2>" + message + "</font>"
          "</body>"
          );
          if (credentialsChanged) ESP.restart();
  });

  WiFi.mode(WIFI_MODE_APSTA); // AP = Access Point, STA = Station Mode
  String index = Pref::get(prefNameFrankyIndex, "");
  if (index == "0") index = "";
  WiFi.setHostname((HOST_NAME + index).c_str()); 
  inAPMode = true;

  String ssid = Pref::get(prefNameSSID); ssid.trim();
  String passwd = Pref::get(prefNamePasswd); passwd.trim();
  String ssid2 = Pref::get(prefNameSSID2); ssid2.trim();
  String passwd2 = Pref::get(prefNamePasswd2); passwd2.trim();
  Serial.printf("Gelesen SSID: %s (alternativ: %s), passwd: %s (alternativ: %s)\n", ssid.c_str(), ssid2.c_str(), passwd.c_str(), passwd2.c_str());

  // WLAN-Daten wurden bereits eingegeben (und gespeichert)
  if ((ssid != "" && passwd != "") || (ssid2 != "" && passwd2 != "")) {

      inAPMode = false;

      // WLAN-Bildschirm mit Countdown
      wifiMulti.addAP(ssid.c_str(), passwd.c_str());
      wifiMulti.addAP(ssid2.c_str(), passwd2.c_str());

      M5.lcd.fillScreen(TFT_BLUE);
      M5.lcd.setTextColor(TFT_WHITE);
      M5.lcd.setTextDatum(CC_DATUM);
      M5.lcd.drawString(String(PRODUCT_NAME) + " " + PRODUCT_VERSION, TFT_W / 2, TFT_H * 0.15, 4);
      M5.lcd.drawString("Verbinde mit WLAN SSID(s)", TFT_W / 2, TFT_H * 0.30, 2);
      M5.lcd.drawString(ssid + (ssid2 == "" ? "" : " oder "), TFT_W / 2, TFT_H * 0.50, 4);
      if (ssid2 != "") M5.lcd.drawString(ssid2, TFT_W / 2, TFT_H * 0.60, 4);


      int connCount = 10; int pixels=30; 
      // while (WiFi.waitForConnectResult() != WL_CONNECTED && connCount > 0) { // scheint dann nie Countdown auszulösen
      while (wifiMulti.run() != WL_CONNECTED && connCount > 0) {
          M5.lcd.fillRect(0, TFT_H * 0.80 - pixels/2, TFT_W, pixels, TFT_BLUE);
          M5.lcd.drawString(String(connCount--), TFT_W / 2, TFT_H * 0.80, 4);
          delay(500);
      }

      MDNS.begin((HOST_NAME + index).c_str());
      // MDNS.addService("http", "tcp", 80); // wohl nur für OTA-Update

      Webserver::begin();

      if (connCount != 0) {
          M5.lcd.fillRect(0, TFT_H * 0.80 - pixels/2, TFT_W, pixels, TFT_BLUE);
          M5.lcd.drawString(WiFi.localIP().toString(), TFT_W / 2, TFT_H * 0.80, 4);
          Serial.printf("IP-Adresse: %s\n", WiFi.localIP().toString().c_str());
          Page::setBlocked(false);
      } else {
          inAPMode = true;
      }
  }
  
  // Keine WLAN-Daten (beim allerersten Start), Access Point einrichten
  if (inAPMode) {

    // AP-Bildschirm, um auf zu verwendente Adresse hinzuweisen
    WiFi.softAP(PRODUCT_NAME, "", 13); // 13=Channel, ohne (default 1) wird AP manchmal nicht aufgespannt
    Serial.printf("AP auf Adresse %s\n",  WiFi.softAPIP().toString().c_str());
    M5.lcd.fillScreen(TFT_BLUE);
    M5.lcd.setTextColor(TFT_WHITE);
    M5.lcd.setTextDatum(CC_DATUM);
    M5.lcd.drawString("1", TFT_W / 2, TFT_H * 0.20, 8);
    M5.lcd.drawString("Verbinden Sie sich mit dem WLAN", TFT_W / 2, TFT_H * 0.50, 2);
    M5.lcd.setTextColor(TFT_YELLOW);
    M5.lcd.drawString(HOST_NAME, TFT_W / 2, TFT_H * 0.62, 4);
    M5.lcd.setTextColor(TFT_WHITE);
    M5.lcd.drawString("(kein Passwort erforderlich)", TFT_W / 2, TFT_H * 0.70, 2);
    char buffer[50];
    sprintf(buffer, "Eigene IP: %s", WiFi.softAPIP().toString().c_str());
    M5.lcd.setTextColor(TFT_YELLOW);
    M5.lcd.drawString(buffer, TFT_W / 2, TFT_H * 0.90, 2);
    WiFi.begin();
    Page::setBlocked(true);

    Webserver::begin();
  }

}

// ===============================================================================================
// Unterstützte Webseiten
// ===============================================================================================

// ----------------------------------------------------------------------------------------------------
//

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/html", "Seite nicht gefunden. Ist sie &uuml;ber <tt>spiffs run -t uploadfs</tt> in den SPIFFS-Bereich geladen worden?");
}

void Webserver::begin() {

  webServer.onNotFound(notFound);

  webServer.on("/index.html", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/system.html", String(), false, processor);
  });

  for (int i = 0; i < numPages; i++) {

    String url = "/" + page[i].url;
    String realURL = url == "/ota" ? "/update" : url;
    String mimeType = page[i].mimeType;

    if (page[i].anchor == LastPageMarker) break;

    if (page[i].mimeType == "text/html") {
      Serial.printf("Webseite %s -> %s registriert\n", url.c_str(), realURL.c_str());
      webServer.on(url.c_str(), HTTP_GET, [url, realURL](AsyncWebServerRequest * request) {
        Serial.printf("Request für %s real: %s\n", url.c_str(), realURL.c_str());
        request->send(SPIFFS, realURL, String(), false, processor);
      });
    } else {
      Serial.printf("Datei %s registriert (%s)\n", url.c_str(), mimeType.c_str());
      webServer.on(url.c_str(), HTTP_GET, [url, mimeType](AsyncWebServerRequest * request) {
        request->send(SPIFFS, url, mimeType);
      });
    }

  }

  ws.onEvent(onWsEvent);
  webServer.addHandler(&ws);

#ifdef FEAT_OTOA
  AsyncElegantOTA.begin(&webServer);
#endif

  webServer.begin();

}

// ===============================================================================================
// Platzhalter auf Webseiten ersetzen
// ===============================================================================================

// ----------------------------------------------------------------------------------------------------
// Durch processor() geschickte Webseiten werden auf %PLATZHALTER% geprüft und diese hier ersetzt

String Webserver::processor(const String& var) {

  // ==== Spezielle Ersetzungen ===================================================================

  if (var == "PERCENT") {
    // % hat Sonderbedeutung, weil es einen zu ersetzenden %WERT% begrenzt
    // Da es in HTML aber z.B. bei "width:50%" benutzt wird, hier künstlich behandeln.
    // Im HTML-Text muss dann stehen: "width:50%PERCENT%"
    return "%";

    // ==== Menüband ==============================================================================

  } else if (var == "MENU") {
    String menu =
      "<!DOCTYPE html>"
      "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">"
      "<html>"
      "<title>" + String(PRODUCT_NAME) + "</title>"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<link rel=\"stylesheet\" href=\"w3.css\">"
      "<head>"
      "<script type=\"text/javascript\" src=\"websocket.js\"></script>"
      "</head>"
      "<body>"
      "<div class=\"w3-container\">"
      "<!-- https://www.w3schools.com/w3css/w3css_buttons.asp -->";

    // einzelne Menüeinträge
    for (int i = 0; i < numPages; i++) {
      if (page[i].anchor == LastPageMarker) break;
      
      String realURL = page[i].url == "ota" ? "update" : page[i].url;
      if (page[i].anchor != "") {
        menu += "<a href=\"" + realURL + "\" class=\"w3-btn w3-round-large w3-teal w3-hover-gray\">" + page[i].anchor + "</a> ";
      }
    }

    menu+= "</div>";

    return menu;

    // ==== Sichtbarkeit wg. Feature =================================================================

  } else if (var == "MQTT_VISIBILITY") {
#ifdef FEAT_MQTT
    return "initial";
#else
    return "none";
#endif

  } else if (var == "PUSHOVER_VISIBILITY") {
#ifdef FEAT_PUSHOVER
    return "initial";
#else
    return "none";
#endif

    // ==== Normale Ersetzungen ===================================================================

  } else if (var == "PRODUCT_NAME") {
    return PRODUCT_NAME;

  } else if (var == "PRODUCT_VERSION") {
    return PRODUCT_VERSION;

  } else if (var == "IP_ADDRESS") {
    return WiFi.localIP().toString();

  } else if (var == "HOST_NAME") {
    return WiFi.getHostname();

  } else if (var == "TIMESTAMP") {
    return __TIMESTAMP__;

  } else if (var == "BUILD_TIME") {
    return __DATE__ " " __TIME__;

  } else if (var == "UPTIME") {
    static char str[20];
    long ss = millis() / 1000;
    int s = ss % 60;

    long ms = ss / 60;
    int m = ms % 60;

    long hs = ss / 3600;
    int h = hs % 24;

    // int ds = hs / 24;

    sprintf(str, "%d:%02d:%02d", h, m, s);
    return (String(str));

  } else if (var == "UPTIME") {
    static char str[20];
    long ss = millis() / 1000;
    int s = ss % 60;

    long ms = ss / 60;
    int m = ms % 60;

    long hs = ss / 3600;
    int h = hs % 24;

    // int ds = hs / 24; // Tage

    sprintf(str, "%02d:%02d:%02d", h, m, s);
    return (String(str));

  } else if (var == "BATTERY_LEVEL") {
    int level = M5.Power.getBatteryLevel();
    return (level == -1 ? "Unbekannt" : String(level) + " Prozent") + (M5.Power.isCharging() ? " (l&auml;dt)" : "");

  } else if (var == "FREE_HEAP") {
    return String(ESP.getFreeHeap()) + " Bytes";

  } else if (var == "SPIFFS_USED") {
    return String(SPIFFS.usedBytes());

  } else if (var == "SPIFFS_FILES") {
    String fileList = "<table border=0>\n";
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
      fileList += "<tr><td align=right>" + String(file.size()) + "</td><td></td><td>" + String(file.name()) + "</td></tr>\n";
      file.close();
      file = root.openNextFile();
    }
    fileList += "</table>";
    file.close();
    return fileList;

  } else if (var == "IP_ADDR") {
    return WiFi.localIP().toString();

    // ==== Preferences, beginnen immer mit PREF_================================================

#define PREF_PREFIX "PREF_"

  } else if (var.startsWith(PREF_PREFIX)) {

    String elems[5];
    split(var, '_', elems); // PREF_type:size:maxlength_NAME
    String prefName = elems[2];
    String params[5];
    split(elems[1], ':', params);

    return inputBox(prefName, params[0], params[1].toInt(), params[2].toInt());
  }

  // ==== Buttons, beginnen immer mit BTN_================================================

#define BTN_PREFIX "BTN_"

  else if (var.startsWith(BTN_PREFIX)) {

    int btnLen = String(BTN_PREFIX).length();
    String btnName = var.substring(btnLen);

    if (btnName == "Speichern") return submitButton(btnName, "save()");
    if (btnName == "Reset") return submitButton(btnName, "purge()");
    if (btnName == "Reboot") return submitButton(btnName, "reboot()");
  }

  // ==== Benutzerdefiniert ================================================

  return htmlPageReplace(var);

}

// ----------------------------------------------------------------------------------------------------
// Erzeugt Inputbox für <name>
String Webserver::inputBox(String name, String type, int size, int maxlength) {

  String value;
  if (type == "checkbox") {
    value = Pref::get(name) == ON ? "checked" : "";
  } else {
    value = "value=\"" + Pref::get(name) + "\"";
  }

  return
    "<input name=\"" + name + "\" "
    "type=\"" + type + "\" "
    "size=\"" + size + "\" "
    "maxlength=\"" + maxlength + "\" " +
    value +
    "/>";
}

// ----------------------------------------------------------------------------------------------------
// Erzeugt Button
String Webserver::submitButton(String name, String script) {
  return
    "<input type=\"submit\" "
    "value=\"" + name + "\" "
    "onclick=\"" + script + "\" "
    "/>";
}

// ===============================================================================================
// Websocket: Empfang und senden
// ===============================================================================================

// ----------------------------------------------------------------------------------------------------
// Websocket: Daten empfangen

void Webserver::onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t *data, size_t len) {

  Webserver::wsClient = client;

  // Bei Aufruf der HTML-Seite oder Wiederaufruf (Tabwechsel)
  if (type == WS_EVT_CONNECT) {
  } else if (type == WS_EVT_DISCONNECT) {
  } else if (type == WS_EVT_DATA) {

    char* buf = new char[len + 1];
    memcpy(buf, data, len);
    buf[len] = 0;
    String received = String(buf);
    String elements[2];

    // "purge" -> Preferences löschen
    if (received == "purge") {
      Pref::purge();

      // "reboot" -> Neustart
    } else if (received == "reboot") {
      ESP.restart();

      // Strings der Form "name | wert" bedeuten: Speichere name := wert in den Preferences
    } else if (split(received, '|', elements) == 2) {

      // alle so empfangenen Werte speichern
      Serial.printf("Speichere %s = %s\n", elements[0].c_str(), elements[1].c_str());
      Pref::set(elements[0], elements[1]);

    } else {

      // zur eigenen Behandlung an Nutzer geben
      wsReceived(received);
    }

    delete buf;
  }
}


// ----------------------------------------------------------------------------------------------------
// Websocket: Daten senden

void Webserver::send(String s) {
  if (wsClient != 0) wsClient -> text(s);
}

void Webserver::send(String parameter, String value) {
  send(parameter + "=" + value);
}
