#include "Switch.h"

// ----------------------------------------------------------------------------------------------------
//

bool Switch::isNode() { return false; }
bool Switch::isSignal() { return false; }
bool Switch::isSwitch() { return true; }

// ----------------------------------------------------------------------------------------------------
//

Switch::Switch(int addr, int x, int y) : Accessory(addr, x, y) {
}

// ----------------------------------------------------------------------------------------------------
//

void Switch::dump() {
  Serial.printf("W%d%c", addr, state ? '+' : '-');
}
