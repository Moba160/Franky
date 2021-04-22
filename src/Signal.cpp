#include "Signal.h"

// ----------------------------------------------------------------------------------------------------
//

bool Signal::isNode() { return false; }
bool Signal::isSignal() { return true; }
bool Signal::isSwitch() { return false; }

// ----------------------------------------------------------------------------------------------------
//

Signal::Signal(int addr, int x, int y) : Accessory(addr, x, y) {
}

// ----------------------------------------------------------------------------------------------------
//

void Signal::dump() {
  Serial.printf("S%d%c", addr, state ? '+' : '-');
}
