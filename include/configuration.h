#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Arduino.h"
#include "Z21.h"

#define PRODUCT_VERSION "1.1.1"
#define PRODUCT_NAME "Franky"
#define HOST_NAME "Franky" // z.B. für Netzwerk

// ----------------------------------------------------------------------------------------------------
// Features

#define FEAT_OTA


// ----------------------------------------------------------------------------------------------------
// WLAN

// Autoconnect nutzen. Wenn auskommentiert, eigene Implementierung verwenden
// #define AUTOCONNECT

// ----------------------------------------------------------------------------------------------------
// Farben

// Standardvorder- und -hintergrund
#define colorForeground TFT_WHITE
#define colorBackgroud TFT_BLACK

// Sonderzustände
#define colorFocus TFT_YELLOW // wenn ein Widget auf Eingabe reagieren kann
#define colorTakenOver TFT_BLUE // Lok wird fremdgesteuert
// Während WLAN noch nicht verbunden ist
#define colorFgInConnection TFT_WHITE
#define colorBgInConnection TFT_BLUE

// Statusbalken oben
#define colorWiFiDisconnected TFT_RED // Nicht mit WLAN verbunden
#define colorDigiStationDisconnected TFT_BLUE // Zwar mit WLAN, aber nicht mit Z21 verbunden
#define colorTrackPowerOff TFT_ORANGE // Zwar mit WLAN/Z21 verbunden, aber Gleisspannung aus
#define colorAllConnected TFT_DARKGREEN // Alles verbunden

// Programmiermodus
#define colorProgMode TFT_PURPLE // Balkenfarbe im Programmiermodus
#define colorValueOK TFT_GREEN // CV-Ergebnis ok
#define colorValueNOK TFT_RED // CV-Ergebnis nicht erhalten

// ----------------------------------------------------------------------------------------------------
// Loksteuerung

#define MAX_LOCO_CHANNELS 5 // maximale Lokkanäle (eigentlich eigentständige Seiten), muss ungerade sein
#define MAX_LOCOS 20 // in gleichzeitiger Steuerung mögliche Lokzahl

// ----------------------------------------------------------------------------------------------------
// Namen für gespeicherte Präferenzen
// Es handelt sich hier nicht wirklich um Konfigurierbarkeit - unabhängig von den Namen bleibt
// das Verhalten gleich

#define prefNameBeta "beta"

#define prefNameSSID "ssid"
#define prefNamePasswd "passwd"

#define prefNameFrankyIndex "frankyIndex"

#define prefNameLocoChannelAddr "chAddr"
#define prefNameZ21IPAddr "z21IPAddr"
#define prefNamePOAPIKey "poAPIToken"
#define prefNamePOUserKey "poUserKey"

#define prefNameGerdOffs "GerdOffs"

#endif
