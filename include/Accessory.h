#ifndef ACCESSORY_H
#define ACCESSORY_H

#include <M5Stack.h>

#include "Node.h"
#include "Configuration.h"

class Accessory : public Node {

  public:


    virtual bool isNode();
    virtual bool isSwitch();
    virtual bool isSignal();

  public:
    Accessory(int addr, int x, int y);



};

#endif
