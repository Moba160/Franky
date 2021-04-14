#include <Arduino.h>
#include <Z21.h>
#include "configuration.h"
#include "Pref.h"

// ----------------------------------------------------------------------------------------------------
// Webserver.cpp ist allgemein geschrieben. Die Franky-Spezifik findet hier statt



// ----------------------------------------------------------------------------------------------------
// Ersetzt %VAR% in HTML-Seite

#define FlagYes "<button class='w3-btn w3-circle w3-teal w3-tiny'>Ein</button>"
#define FlagNo "<button class='w3-btn w3-circle w3-gray w3-tiny'>Aus</button>"

String htmlPageReplace(String var) {

  if (var == "Z21_PORT") {
    return String(Z21_PORT);

  }  else if (var == "SET_HOST_NAME") { // der designierte Hostname, nicht zu verwechseln mit WiFi.getHostName(), welches erst nach Reboot benutzt wird
    String index = Pref::get(prefNameFrankyIndex, "");
    if (index == "0") index = "";
    return HOST_NAME + index;

  } else if (var == "Z21_ADDR") {
    return Z21::ipAddress;

  } else if (var == "Z21_HWVERSION") {
    return Z21::hwVersion;

  } else if (var == "Z21_VERSION") {
    return Z21::xbusVersion;

  } else if (var == "Z21_CSID") {
    return Z21::csID;

  } else if (var == "Z21_SERIALNUMBER") {
    return Z21::serialNbr;

  } else if (var == "Z21_FWVERSION") {
    return Z21::fwVersion;

  } else if (var == "Z21_CURRMAIN") {
    return Z21::currMain;

  } else if (var == "Z21_CURRPROG") {
    return Z21::currProg;

  } else if (var == "Z21_TEMP") {
    return Z21::temp;

  } else if (var == "Z21_TEMPOK") {
    return Z21::highTemp ? "zu hoch" : "in Ordnung";

  } else if (var == "Z21_VOLTAGEOK") {
    return Z21::lowVoltage ? "zu niedrig" : "in Ordnung";

  } else if (var == "Z21_TRACKPOWERSTATE") {
    return Z21::isTrackPowerOff ? "Aus" : "Ein";

  } else if (var == "Z21_EMERGENCYSTOP") {
    return Z21::emergencyStop ? "Ein" : "Aus";

  } else if (var == "Z21_SHORTCUT") {
    return Z21::shortCircuit ? "Ja" : "Nein";

  } else if (var == "Z21_PROGMODE") {
    return Z21::isProgStateOff ? "Inaktiv" : "Aktiv";

  } else if (var == "FLAG_VALIDITY_HINT") {
    return Z21::flagsValid ? "" : "(noch nicht ausgelesen)";

  } else if (var == "HIDE_FLAGS") {
    return Z21::flagsValid ? "block" : "none"; 

  } else if (var == "BCF_BASIC") {
    return Z21::BCF_BASIC ? FlagYes : FlagNo;

  } else if (var == "BCF_RBUSFB") {
    return Z21::BCF_RBUSFB ? FlagYes : FlagNo;

  } else if (var == "BCF_RC") {
    return Z21::BCF_RC ? FlagYes : FlagNo;

  } else if (var == "BCF_SYS") {
    return Z21::BCF_SYS ? FlagYes : FlagNo;

  } else if (var == "BCF_ALLLOCO") {
    return Z21::BCF_ALLLOCO ? FlagYes : FlagNo;

  } else if (var == "BCF_ALLRC") {
    return Z21::BCF_ALLRC ? FlagYes : FlagNo;

  } else if (var == "BCF_CANFB") {
    return Z21::BCF_CANFB ? FlagYes : FlagNo;

  } else if (var == "BCF_LNMSG") {
    return Z21::BCF_LNMSG ? FlagYes : FlagNo;

  } else if (var == "BCF_LNLOCO") {
    return Z21::BCF_LNLOCO ? FlagYes : FlagNo;

  } else if (var == "BCF_LNSwitch") {
    return Z21::BCF_LNSwitch ? FlagYes : FlagNo;

  } else if (var == "BCF_LNFB") {
    return Z21::BCF_LNFB ? FlagYes : FlagNo;
  }
  
  return "";
}

void wsReceived(String received) {
  
}
