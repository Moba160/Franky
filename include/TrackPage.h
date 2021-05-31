#ifndef TRACKPAGE_H
#define TRACKPAGE_H

#include "Page.h"
#include "Textbox.h"

// ----------------------------------------------------------------------------------------------------
// Benutzbarkeit einer Fahrstraße
// Benötigt als eine Art Filter für die Auswahl einer Fahrstraßengruppe über Tasten;
// individuelle Fahrstraßen werden dann innerhalb der Gruppe über den Drehregler ausgewählt

#define RT_ELEFT         1
#define RT_ALEFT         2
#define RT_THROUGH       4
#define RT_ERIGHT        8
#define RT_ARIGHT       16
#define RT_SH_TOP       32
#define RT_SH_BOT       64
#define RT_SH_Q        128
#define RT_SH_S        256

class TrackPage: public Page, public Z21Observer {

  public:
    TrackPage(char navigable);
    virtual void setVisible(bool visible, bool clearScreen) override;
    virtual void buttonPressed(M5Btn::ButtonType button) override;
    virtual bool isBeta() override { return false; }

  private:

    void matchFirstRoute();

    int routeFilter = RT_ELEFT;
    int selectedRoute = 0;
    void drawTracks();

    Textbox* fstName;
    Textbox* progress;
    Textbox* leftSymbol;
    Textbox* rightSymbol;

};

#endif