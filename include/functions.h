#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/*
    Buttons können Funktionen fest oder variabel zugeordnet werden. Welche dafür potenziell
    genutzt werden können, ist hier vermerkt. Ihr Wert ist gleichzeitig die Buttonbeschriftung

*/

// Fokus (zwischen Widgets wechseln). Diese wird immer fest auf Taste B (kurz) gelegt und ist
// nicht veränderlich
#define FN_FOCUS "< v >"

// Spitzenbeleuchtung ein/aus (toggeln)
#define FN_HEADLIGHTS "Licht"

// Lokkanäle durchschalten
#define FN_CHANNELS_PLUS "Kanal+"
#define FN_CHANNELS_MINUS "Kanal-"

// Adresse ändern
#define FN_CHANGE_ADDR "Adr"

// CV Read
#define FN_CVREAD "CV Read"

// CV Write
#define FN_CVWRITE "CV Write"

// PoM Write
#define FN_POMWRITE "PoM Write"

#endif