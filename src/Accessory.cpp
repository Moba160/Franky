#include "Accessory.h"

// ----------------------------------------------------------------------------------------------------
//

bool Accessory::isNode() { return false; }
bool Accessory::isSignal() { return false; }
bool Accessory::isSwitch() { return false; }

// ----------------------------------------------------------------------------------------------------
//

Accessory::Accessory(int addr, int x, int y) : Node (addr, x, y) {}
