#ifndef SWITCH_H
#define SWITCH_H

/*
  Weichenobjekt
*/

#include "Accessory.h"

class Switch: public Accessory {

  public:

    Switch(int addr, int x, int y);

    virtual bool isNode();
    virtual bool isSwitch();
    virtual bool isSignal();

    virtual void dump();

};

#endif
