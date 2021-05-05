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
    virtual void setVisible(bool visible, bool clearScreen) override;
    virtual bool isBeta() override { return false; }
    virtual void buttonPressed(M5Btn::ButtonType btn) override;
    virtual void focusChanged() override;

    // Zuletzt bediente Lok
    static Loco* currentLoco() { return selectedLoco; }

    // Von Z21 erhaltene Notifikationen
    virtual void locoInfoChanged(int addr, Direction dir, int fst, bool takenOver, int numSpeedSteps, bool f[]) override;

    // Durch automatisches Bremsen/Beschleunigen geänderte Geschwindigkeit
    virtual void locoWasDriven(int addr);

  private:

    // Fahrmodus. true: manuell fahren, false: automatisch bremsen/beschleunigen
    bool driveManually = true;

    // Lok ansteuern gemäß angeforderter Werte
    void driveLoco();

    // Anzeige/Refresh der Daten der aktuellen Lok im Display
    void update();

    // Gewählter Lokkanal hat sich geändert
    void locoChannelChanged(int oldChannel, int channel);

    // Im Adressmodus?
    bool inAddressChangeMode = false;
    // void handleDeltaButton();

    // Anzahl Layeer
    int numLayers = 2 + MaxFct/4;

    // Gespeicherte Loks je Lokkanal
    Loco* loco[MAX_LOCO_CHANNELS];

    // Aktueller Lokkanal, anfangs der in der Mitte
    int channel = MAX_LOCO_CHANNELS/2;

    // Zuletzt bediente Lok
    static Loco* selectedLoco;

    // Widgets für Bedienung
    Numberbox* addr[MAX_LOCO_CHANNELS]; 
    Symbolbox* direction;
    Symbolbox* headlights;
    Numberbox* speed;
    Numberbox* targetSpeed;

    void dump(char* message);

};

#endif