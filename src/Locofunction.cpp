#include "Locofuntion.h"

// ----------------------------------------------------------------------------------------------------
//

Locofunction::Locofunction(int index, int mappedTo, char* shortName, char* longName) : index{index}, mappedTo{mappedTo} /*, longName{longName}*/ {
    if (shortName[0] == 0) {
        sprintf(this->shortName, "F%d", index);
    }
    // if (longName == "") this->longName = this->shortName; 
}

// ----------------------------------------------------------------------------------------------------
//

void Locofunction::setShortName(const char* shortName) { 
    strncpy(this->shortName, shortName, SHORT_NAME_LEN); 
    this->shortName[SHORT_NAME_LEN-1] = 0;
}

void Locofunction::dump() {
    // Serial.printf("Funktion %d-%d: %s/%s %s\n", index, mappedTo, shortName.c_str(), longName.c_str(), active ? "EIN" : "AUS");
}
