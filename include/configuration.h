#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Arduino.h"
#include "Z21.h"

#define PRODUCT_VERSION "1.3.1"
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
#define colorLibMode TFT_BLUE // wenn im Bibliotheksmodus

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
#define MAX_LOCOS 100 // in gleichzeitiger Steuerung mögliche Lokzahl. Erhöhung nur mit Bedacht, kleiner 67000 Heapgröße bringt Abstürze!
#define LOCO_CYCLE 250 // Zyklus, alle wieviel ms die Lokbeschleunigung ausgerechnet wird

#define COLOR_ACCELERATING TFT_DARKGREEN
#define COLOR_DECELERATING TFT_RED
#define COLOR_TAKENOVER TFT_BLUE

// ----------------------------------------------------------------------------------------------------
// Schalten

// Anzahl maximal verwalteter Weichen
#define MAX_ACCESSORIES 70

// Gleislinienfarbe
#define TRACK_COLOR colorForeground

// dto für hervorgehobenen Fahrstraße (Fahrstraßenwahl)
#define SELECTED_ROUTE_COLOR TFT_RED

// dto für den bereits gestellten Fahrstraßenteil
#define SET_ROUTE_COLOR TFT_GREEN

// Verzögerung zwischen Weichen-/Signalumlauf
#define SWITCH_DELAY 500



// ----------------------------------------------------------------------------------------------------
// Namen für gespeicherte Präferenzen
// Es handelt sich hier nicht wirklich um Konfigurierbarkeit - unabhängig von den Namen bleibt
// das Verhalten gleich

#define prefNameBeta "beta"

#define prefNameSSID "ssid"
#define prefNamePasswd "passwd"
#define prefNameSSID2 "ssid2"
#define prefNamePasswd2 "passwd2"

#define prefNameFrankyIndex "frankyIndex"

#define prefNameLocoChannelAddr "chAddr"
#define prefNameZ21IPAddr "z21IPAddr"
#define prefNamePOAPIKey "poAPIToken"
#define prefNamePOUserKey "poUserKey"

#define prefNameGerdOffs "GerdOffs"

#define prefNameLocoLib "locoLib"
#define prefNameDriveAutomatically "driveAutom" // nicht länger als 14 Zeichen. Noch nachschauen, warum

#endif
