#include <Arduino.h>
#include <Z21.h>
#include "configuration.h"
#include "Pref.h"

// ----------------------------------------------------------------------------------------------------
// Webserver.cpp ist allgemein geschrieben. Die Franky-Spezifik findet hier statt



// ----------------------------------------------------------------------------------------------------
// Ersetzt %VAR% in HTML-Seite

String htmlPageReplace(String var) {
  if (var == "Z21_PORT") {
    return String(Z21_PORT);
  }  else if (var == "SET_HOST_NAME") { // der designierte Hostname, nicht zu verwechseln mit WiFi.getHostName(), welches erst nach Reboot benutzt wird
    String index = Pref::get(prefNameFrankyIndex, "");
    if (index == "0") index = "";
    return HOST_NAME + index;
  }
  
  return "";
}

void wsReceived(String received) {
  
}
