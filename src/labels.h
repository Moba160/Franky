#ifndef LABELS_H
#define LABELS_H

// Wird erst nach Clean des Projektes wirksam, weil durch -D nicht Bestandteil der
// Abhängigkeiten

// Menu item: Configure new AP
#undef  AUTOCONNECT_MENULABEL_CONFIGNEW
#define AUTOCONNECT_MENULABEL_CONFIGNEW   "WLAN verbinden"

// Menu item: Open SSIDs
#undef  AUTOCONNECT_MENULABEL_OPENSSIDS
#define AUTOCONNECT_MENULABEL_OPENSSIDS   "Gespeicherte WLANS"

// Menu item: Disconnect
#undef  AUTOCONNECT_MENULABEL_DISCONNECT
#define AUTOCONNECT_MENULABEL_DISCONNECT  "{{CUR_SSID}} trennen"

// Menu item: Reset...
#undef  AUTOCONNECT_MENULABEL_RESET
#define AUTOCONNECT_MENULABEL_RESET       "Zurücksetzen"

// Menu item: HOME
#undef  AUTOCONNECT_MENULABEL_HOME
#define AUTOCONNECT_MENULABEL_HOME        "HOME"

// Menu item: Update
#undef  AUTOCONNECT_MENULABEL_UPDATE
#define AUTOCONNECT_MENULABEL_UPDATE      "OTA-Update"

// Menu item: Device Info
#undef  AUTOCONNECT_MENULABEL_DEVINFO
#define AUTOCONNECT_MENULABEL_DEVINFO     "Geräteinfo"

// Button label: RESET
#undef  AUTOCONNECT_BUTTONLABEL_RESET
#define AUTOCONNECT_BUTTONLABEL_RESET     "Zurücksetzen"

// Button label: UPDATE
#undef  AUTOCONNECT_BUTTONLABEL_UPDATE
#define AUTOCONNECT_BUTTONLABEL_UPDATE     "Hochladen"

// Page title: Page not found
#undef  AUTOCONNECT_PAGETITLE_NOTFOUND
#define AUTOCONNECT_PAGETITLE_NOTFOUND "Seite nicht gefunden"

// Page title: AutoConnect resetting
#undef  AUTOCONNECT_PAGETITLE_RESETTING
#define AUTOCONNECT_PAGETITLE_RESETTING "Setzt zurück"

// Page title: AutoConnect statistics
#undef  AUTOCONNECT_PAGETITLE_STATISTICS
#define AUTOCONNECT_PAGETITLE_STATISTICS "Statistik"

// Page statistics row: Established connection
#undef  AUTOCONNECT_PAGESTATS_ESTABLISHEDCONNECTION
#define AUTOCONNECT_PAGESTATS_ESTABLISHEDCONNECTION "Verbunden mit"

// Page statistics row: Mode
#undef  AUTOCONNECT_PAGESTATS_MODE
#define AUTOCONNECT_PAGESTATS_MODE "Modus"

// Page statistics row: IP
#undef  AUTOCONNECT_PAGESTATS_IP
#define AUTOCONNECT_PAGESTATS_IP "IP"

// Page statistics row: GW
#undef  AUTOCONNECT_PAGESTATS_GATEWAY 
#define AUTOCONNECT_PAGESTATS_GATEWAY "Gateway"

// Page statistics row: Subnet mask
#undef  AUTOCONNECT_PAGESTATS_SUBNETMASK
#define AUTOCONNECT_PAGESTATS_SUBNETMASK "Subnetzmaske"

// Page statistics row: SoftAP IP
#undef  AUTOCONNECT_PAGESTATS_SOFTAPIP
#define AUTOCONNECT_PAGESTATS_SOFTAPIP "SoftAP IP"

// Page statistics row: AP MAC
#undef  AUTOCONNECT_PAGESTATS_APMAC
#define AUTOCONNECT_PAGESTATS_APMAC "AP MAC"

// Page statistics row: STA MAC
#undef  AUTOCONNECT_PAGESTATS_STAMAC
#define AUTOCONNECT_PAGESTATS_STAMAC "STA MAC"

// Page statistics row: Channel
#undef  AUTOCONNECT_PAGESTATS_CHANNEL
#define AUTOCONNECT_PAGESTATS_CHANNEL "Kanal"

// Page statistics row: dBm
#undef  AUTOCONNECT_PAGESTATS_DBM
#define AUTOCONNECT_PAGESTATS_DBM "dBm"

// Page statistics row: Chip ID
#undef  AUTOCONNECT_PAGESTATS_CHIPID
#define AUTOCONNECT_PAGESTATS_CHIPID "Chip ID"

// Page statistics row: CPU Freq.
#undef  AUTOCONNECT_PAGESTATS_CPUFREQ
#define AUTOCONNECT_PAGESTATS_CPUFREQ "CPU-Frequenz"

// Page statistics row: Flash size
#undef  AUTOCONNECT_PAGESTATS_FLASHSIZE
#define AUTOCONNECT_PAGESTATS_FLASHSIZE "Flashgröße"

// Page statistics row: Free memory
#undef  AUTOCONNECT_PAGESTATS_FREEMEM
#define AUTOCONNECT_PAGESTATS_FREEMEM "Freier Speicher"

// Page title: AutoConnect config
#undef  AUTOCONNECT_PAGETITLE_CONFIG
#define AUTOCONNECT_PAGETITLE_CONFIG "WLAN verbinden"

// Page config text: Total:
#undef  AUTOCONNECT_PAGECONFIG_TOTAL
#define AUTOCONNECT_PAGECONFIG_TOTAL "Gesamt:"

// Page config text: Hidden:
#undef  AUTOCONNECT_PAGECONFIG_HIDDEN
#define AUTOCONNECT_PAGECONFIG_HIDDEN "Verborgen:"

// Page config text: SSID
#undef  AUTOCONNECT_PAGECONFIG_SSID
#define AUTOCONNECT_PAGECONFIG_SSID "SSID"

// Page config text: Passphrase
#undef  AUTOCONNECT_PAGECONFIG_PASSPHRASE
#define AUTOCONNECT_PAGECONFIG_PASSPHRASE "Passwort"

// Page config text: Enable DHCP
#undef  AUTOCONNECT_PAGECONFIG_ENABLEDHCP
#define AUTOCONNECT_PAGECONFIG_ENABLEDHCP "DHCP verwenden"

// Page config text: Apply
#undef  AUTOCONNECT_PAGECONFIG_APPLY
#define AUTOCONNECT_PAGECONFIG_APPLY "Verbinden"

// Page title: AutoConnect credentials
#undef  AUTOCONNECT_PAGETITLE_CREDENTIALS
#define AUTOCONNECT_PAGETITLE_CREDENTIALS "AutoConnect Zugangsdaten"

// Page title: AutoConnect connecting
#undef  AUTOCONNECT_PAGETITLE_CONNECTING
#define AUTOCONNECT_PAGETITLE_CONNECTING "AutoConnect verbindet"

// Page title: AutoConnect connection failed
#undef  AUTOCONNECT_PAGETITLE_CONNECTIONFAILED
#define AUTOCONNECT_PAGETITLE_CONNECTIONFAILED "AutoConnect Verbindung fehlgeschlagen"

// Page connection failed: Connection Failed
#undef  AUTOCONNECT_PAGECONNECTIONFAILED_CONNECTIONFAILED
#define AUTOCONNECT_PAGECONNECTIONFAILED_CONNECTIONFAILED "Verbindung fehlgeschlagen"

// Page title: AutoConnect disconnected
#undef  AUTOCONNECT_PAGETITLE_DISCONNECTED
#define AUTOCONNECT_PAGETITLE_DISCONNECTED "Verbindung getrennt"

// Text: No saved credentials.
#undef  AUTOCONNECT_TEXT_NOSAVEDCREDENTIALS
#define AUTOCONNECT_TEXT_NOSAVEDCREDENTIALS "Keine gespeicherten Daten"

// Text: The update page caption
#undef  AUTOCONNECT_TEXT_UPDATINGFIRMWARE
#define AUTOCONNECT_TEXT_UPDATINGFIRMWARE "Firmware aktualisieren/Daten hochladen"

// Text: The update page's file selection button label

#undef  AUTOCONNECT_TEXT_SELECTFIRMWARE
#define AUTOCONNECT_TEXT_SELECTFIRMWARE "Datei auswählen (.bin=Firmware und führt zur Aktualisierung. Andere Dateien werden in den SPIFFS-Bereich hochgeladen<br><br>"

// Text: OTA success
#undef  AUTOCONNECT_TEXT_OTASUCCESS
#define AUTOCONNECT_TEXT_OTASUCCESS "Aktualisierung erfolgreich. Neustart - bitte einen Moment Geduld ..."

// Text: OTA success
#undef  AUTOCONNECT_TEXT_OTAUPLOADED
#define AUTOCONNECT_TEXT_OTAUPLOADED "Erfolgreich hochgeladen"

// Text: OTA failure
#undef  AUTOCONNECT_TEXT_OTAFAILURE
#define AUTOCONNECT_TEXT_OTAFAILURE "Hochladen fehlgeschlagen: "

// Text: Authenticaton failed
#undef  AUTOCONNECT_TEXT_AUTHFAILED
#define AUTOCONNECT_TEXT_AUTHFAILED "Authentifizierung fehlgeschlagen"

// Menu Text: Connecting
#undef  AUTOCONNECT_MENUTEXT_CONNECTING
#define AUTOCONNECT_MENUTEXT_CONNECTING "Verbinden"

// Menu Text: Disconnect
#undef  AUTOCONNECT_MENUTEXT_DISCONNECT
#define AUTOCONNECT_MENUTEXT_DISCONNECT "Trennen"

// Menu Text: Failed
#undef  AUTOCONNECT_MENUTEXT_FAILED
#define AUTOCONNECT_MENUTEXT_FAILED "Fehlgeschlagen"


// Menu colors
// The following three color code items determine the color scheme of
// the menu. In addition to hexadecimal color values, you can specify
// 140 standard color names that apply to CSS.
// To be no sense of discomfort, select the same series of values for
// the background and the active item.

#undef  AUTOCONNECT_MENUCOLOR_TEXT
#define AUTOCONNECT_MENUCOLOR_TEXT        "#e6e6fa"
#undef  AUTOCONNECT_MENUCOLOR_BACKGROUND
#define AUTOCONNECT_MENUCOLOR_BACKGROUND  "#3e2723"
#undef  AUTOCONNECT_MENUCOLOR_ACTIVE
#define AUTOCONNECT_MENUCOLOR_ACTIVE      "#4e342e"

// // Menu text foreground color
// #undef  AUTOCONNECT_MENUCOLOR_TEXT
// #define AUTOCONNECT_MENUCOLOR_TEXT        "#fff"

// // Menu background color
// #undef  AUTOCONNECT_MENUCOLOR_BACKGROUND
// #define AUTOCONNECT_MENUCOLOR_BACKGROUND  "#263238"

// // Background color with active menu items and mouse hover
// #undef  AUTOCONNECT_MENUCOLOR_ACTIVE

// #define AUTOCONNECT_MENUCOLOR_ACTIVE      "#37474f"

#endif