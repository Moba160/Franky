#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*
    Buttons können Funktionen fest oder variabel zugeordnet werden. Welche dafür potenziell
    genutzt werden können, ist hier vermerkt. Ihr Wert ist gleichzeitig die Buttonbeschriftung

*/

// Fokus (zwischen Widgets wechseln). Diese wird immer fest auf Taste B (kurz) gelegt und ist
// nicht veränderlich
#define FN_FOCUS "< o >"

// Gleisspannung ein/aus
#define FN_TRACKPOWER_ON "Gleissp. ein"
#define FN_TRACKPOWER_OFF "Gleissp. aus"

// Spitzenbeleuchtung ein/aus (toggeln)
#define FN_HEADLIGHTS "Licht"

// Lokkanäle durchschalten
#define FN_CHANNELS_PLUS "Kanal+"
#define FN_CHANNELS_MINUS "Kanal-"

// Automatisch/manuell fahren umschalten
#define FN_DRIVE_AUTO "Automatisch"

// Adresse ändern
#define FN_CHANGE_ADDR "Adresse"

// Softkeyebenen umschalten auf LocoPage
#define FN_DELTA_START "Delta >"
#define FN_DELTA_END "< Delta"

// CV Read
#define FN_CVREAD "CV Read"

// CV Write
#define FN_CVWRITE "CV Write"

// PoM Write
#define FN_POMWRITE "PoM Write"

///// Gleisbildseite

#define FN_EINF_HL "Halle ->"   // Filter Einfahrt aus Halle
#define FN_AUSF_HL "<- Halle"   // Filter Ausfahrt nach Halle
#define FN_DURCHF "Durchfahrt"  // Filter Durchfahrt
#define FN_FILT2 "..."          // Umschalten auf anderen Filter
#define FN_EINF_AHB "<- Ahb"    // Filter Einfahrt aus Ahb
#define FN_AUSF_AHB "Ahb ->"    // Filter Ausfahrt nach Ahb

#define FN_SH_TOP   "Oben"   // Filter Rangieren "oben"
#define FN_SH_Q     "\\_/"   // Filter Rangieren "Rest"
#define FN_FILT1 ".."           // Umschalten auf anderen Filter
#define FN_SH_S     "<S>"    // Filter Rangieren S-Bahn
#define FN_SH_BOT   "Unten"  // Filter Rangieren "unten"

#endif