#include "Locofuntion.h"

// ----------------------------------------------------------------------------------------------------
//

Locofunction::Locofunction(int index, int mappedTo, String shortName, String longName) : index{index}, mappedTo{mappedTo}, shortName{shortName}, longName{longName} {
    if (shortName == "") this->shortName = "F" + String(index);
    if (longName == "") this->longName = this->shortName; 
}

// ----------------------------------------------------------------------------------------------------
//

void Locofunction::dump() {
    Serial.printf("Funktion %d-%d: %s/%s %s\n", index, mappedTo, shortName.c_str(), longName.c_str(), active ? "EIN" : "AUS");
}
