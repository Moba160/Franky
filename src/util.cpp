/*

  Hilfsfunktionen

*/

#include <Arduino.h>

// ----------------------------------------------------------------------------------------------------
// Aufspalten eines Strings mit Trennzeichen in Array, liefert Anzahl Arrayelemente zurück

int split(String s, char listSeparator, String elements[]) {

  int numElems = 0;
  elements[0] = "";

  for (int i = 0; i < s.length(); i++) {
    if (s[i] == listSeparator) {
      numElems++;
      elements[numElems] = "";
    } else {
      elements[numElems] += s[i];
    }
  }

  return numElems + 1;
}
