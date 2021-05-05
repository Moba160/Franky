#include <Arduino.h>
#include <Z21.h>
#include "configuration.h"
#include "Pref.h"

// ----------------------------------------------------------------------------------------------------
// Webserver.cpp ist allgemein geschrieben. Die Franky-Spezifik findet hier statt

// ----------------------------------------------------------------------------------------------------
// Ersetzt %VAR% in HTML-Seite

String yesno(String id, bool yes) {
  if (yes) return "<button class='w3-btn w3-circle w3-teal w3-tiny' id='" + id + "'>Ein</button>";
  else return "<button class='w3-btn w3-circle w3-gray w3-tiny' id='" + id + "'>Aus</button>";
}

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
    return Z21::toString(Z21::getTrackPowerState());

  } else if (var == "Z21_EMERGENCYSTOP") {
    return Z21::getEmergencyStopState() == BoolState::On ? "Ein" : "Aus";

  } else if (var == "Z21_SHORTCUT") {
    return Z21::getShortCircuitState() == BoolState::On ? "Ja" : "Nein";

  } else if (var == "Z21_PROGMODE") {
    return Z21::getProgState() == BoolState::On ? "Aktiv" : "Inaktiv";

  } else if (var == "FLAG_VALIDITY_HINT") {
    return Z21::flagsValid ? "" : "(noch nicht ausgelesen)";

  } else if (var == "HIDE_FLAGS") {
    return Z21::flagsValid ? "block" : "none"; 

  } else if (var == "BCF_BASIC") {
    return yesno("BCF_BASIC", Z21::BCF_BASIC);

  } else if (var == "BCF_RBUSFB") {
    return yesno("BCF_RBUSFB", Z21::BCF_RBUSFB);

  } else if (var == "BCF_RC") {
    return yesno("BCF_RC", Z21::BCF_RC);

  } else if (var == "BCF_SYS") {
    return yesno("BCF_SYS", Z21::BCF_SYS);

  } else if (var == "BCF_ALLLOCO") {
    return yesno("BCF_ALLLOCO", Z21::BCF_ALLLOCO);

  } else if (var == "BCF_ALLRC") {
    return yesno("BCF_ALLRC", Z21::BCF_ALLRC);

  } else if (var == "BCF_CANFB") {
    return yesno("BCF_CANFB", Z21::BCF_CANFB);

  } else if (var == "BCF_LNMSG") {
    return yesno("BCF_LNMSG", Z21::BCF_LNMSG);

  } else if (var == "BCF_LNLOCO") {
    return yesno("BCF_LNLOCO", Z21::BCF_LNLOCO);

  } else if (var == "BCF_LNSwitch") {
    return yesno("BCF_LNSwitch", Z21::BCF_LNSwitch);

  } else if (var == "BCF_LNFB") {
    return yesno("BCF_LNFB", Z21::BCF_LNFB);
  }
  
  return "";
}

void wsReceived(String received) {
  
}
