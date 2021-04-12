#ifndef WEBSERVER_H_
#define WEBSERVER_H_ // WEBSERVER_H bereits von ESPAsyncWebserver.h definiert

#include <ESPAsyncWebServer.h>

class Webserver {

  public:
    static void begin();

    static void webconfig();

    // Für Websocket-Kommunikation: sende <s>
    static void send(String s);

    // Wenn Parameter <parameter> in Webseite gefunden wird, soll er durch <value> ersetzt werden
    static void send(String parameter, String value);

    // Fügt eine <input/>-Box in die Webseite ein. size=sichtbare Breite, maxlength=Inputlänge
    static String inputBox(String name, String type, int size, int maxlength);

    // Fügt <submit/>-Button in die Webseite ein, bei der dann <script> ausgeführt wird
    static String submitButton(String name, String script);

    static AsyncWebSocketClient* wsClient;
    
    static AsyncWebServer webServer;

    
  private:
    static bool inAPMode; // im Access Point Mode?
    static String processor(const String& var);
    static void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t *data, size_t len);

};

#endif
