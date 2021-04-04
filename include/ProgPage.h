#ifndef PROGPAGE_H
#define PROGPAGE_H

#include "Page.h"
#include "Numberbox.h"
#include "Z21Observer.h"

class ProgPage: public Page, public Z21Observer {

  public:
    ProgPage(char navigable);
    virtual void setVisible(bool visible, bool clearScreen);

    virtual void progResult(ProgResult result, int value);


    virtual void buttonPressed(M5Btn::ButtonType button);

  private:
    Numberbox* cvAddress;
    Numberbox* cvValue;

};

#endif