#ifndef LOCOPAGE_H
#define LOCOPAGE_H

#include "Page.h"
#include <Z21Observer.h>
#include "Symbolbox.h"
#include "Numberbox.h"
#include "Textbox.h"
#include "Loco.h"
#include "configuration.h"

class LocoPage: public Page, public Z21Observer {

  public:
    LocoPage(char navigable);

    static Loco* currentLoco() { return selectedLoco; }

    virtual void setVisible(bool visible, bool clearScreen);

    virtual void buttonPressed(M5Btn::ButtonType btn);

    virtual void locoInfoChanged(int addr, Direction dir, int fst, bool takenOver, int numSpeedSteps, bool f[]) override;


  private:

    // void dump(char*);
    void driveLoco();
    void focusChanged() override;
    void transferData(bool toDisplay);
    void handleLocoChannel(int oldChannel, int channel);

    Loco* loco[MAX_LOCO_CHANNELS];
    int channel = MAX_LOCO_CHANNELS/2;
    static Loco* selectedLoco;

    Numberbox* addr[MAX_LOCO_CHANNELS]; 
    int addressIndex;

    Symbolbox* direction;
    Symbolbox* headlights;
    Numberbox* speed;
    Numberbox* targetSpeed;

};

#endif