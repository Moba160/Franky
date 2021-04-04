#include "Pref.h"
#include "configuration.h"

Preferences Pref::prefs;

// ----------------------------------------------------------------------------------------------------
//
void Pref::begin() {
  prefs.begin(PRODUCT_NAME, false);
}

// ----------------------------------------------------------------------------------------------------
//
void Pref::init(String key, String defValue) {
  prefs.putString(key.c_str(), prefs.getString(key.c_str(), defValue.c_str()));
}

// ----------------------------------------------------------------------------------------------------
//
void Pref::set(String key, String value) {
  if (get(key) == value) return; // kein unnötiges Schreiben (Lebenszeitverkürzung SRAM)
  prefs.putString(key.c_str(), value.c_str());
}

// ----------------------------------------------------------------------------------------------------
//
String Pref::get(String key) {
  return prefs.getString(key.c_str());
}
String Pref::get(String key, String defValue) {
  return prefs.getString(key.c_str(), defValue);
}
