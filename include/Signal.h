#ifndef SIGNAL_H
#define SIGNAL_H

/*
  Signalnobjekt
*/

#include "Accessory.h"

class Signal: public Accessory {

  public:

    Signal(int addr, int x, int y);

    virtual bool isNode();
    virtual bool isSwitch();
    virtual bool isSignal();

    virtual void dump();

};

#endif
