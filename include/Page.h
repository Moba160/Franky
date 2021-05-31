#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>
#include <Z21.h>
#include <Z21Observer.h>
#include <M5Btn.h>
#include "Widget.h"
#include <Softkey.h>

/*
  Abstrakte Basisklasse der GUI
  Die GUI besteht aus einem Array aus Seiten, die von Page abgeleitet werden.
  Mit Wischgesten bewegt man sich von einer Seite zur anderen (zwischen den
  Elementen des Arrays)
*/

// Dimension des Seiten-Arrays, es gibt also (maximal) PAGE_ROWS x PAGE_COLS Seiten,
// wobei nicht zu besetzende Plätze im Array mit 0 gekennzeichnet werden
#define PAGE_ROWS 2
#define PAGE_COLS 3

// Softkey-Ebenen
#define LAYER0 0
#define LAYER1 1

// scheinbar verdreht
#define TFT_W TFT_HEIGHT
#define TFT_H TFT_WIDTH

// In welche Richtung darf von einer Seite aus navigiert (gewischt) werden?
// Es ist nicht immer erwünscht, von jeder Seite zu jeder zu kommen
#define NAV_LEFT  0b1000
#define NAV_RIGHT 0b1001
#define NAV_UP    0b0100
#define NAV_DOWN  0b0010
#define NAV_ALL NAV_LEFT | NAV_RIGHT | NAV_UP | NAV_DOWN

#define MaxWidgets 100

class Page : public Z21Observer {

    public:
        
        Page(char navigable) : navigable{navigable} {}

        static void begin(TFT_eSPI* tft);
        static void setBlocked(boolean blocked) { Page::blocked = blocked; }
        static bool isBlocked() { return blocked; }
        static void loop();

        virtual void trackPowerStateChanged(BoolState trackPowerState) override;
        virtual void shortCircuitStateChanged(BoolState shortCircuitState) override;
        virtual void emergencyStopStateChanged(BoolState emergencyStopState) override;
        virtual void progStateChanged(BoolState progState) override;
        virtual void progResult(ProgResult result, int value) override {}
        virtual void traceEvent(FromToZ21 direction, long diffLastSentReceived, String message, String parameters) override;

        virtual void locoWasDriven(int addr) {}

        virtual void setVisible(bool visible, bool clearScreen); // müsste protected sein
        virtual void navigationHint();
        boolean isVisible() { return visible; };

        static void handlePageSwitchAndFocus(M5Btn::ButtonType button);
        virtual void buttonPressed(M5Btn::ButtonType btn) {};
        Softkey* getSoftkey(M5Btn::ButtonType button);
        Softkey* getSoftkey(String caption);
        String getFunction(M5Btn::ButtonType button);
        void setFunction(M5Btn::ButtonType button, String caption);

        virtual bool isBeta() = 0;
        
        static Page* currentPage() { return navigationGrid[row][col]; }
        Widget* focussedWidget();

        static int bgColor;
        static int fgColor;

  protected:

    virtual void focusChanged() {};
    Widget* lastFocussed = 0;

    static TFT_eSPI* tft;
    boolean visible = false;
    static bool blocked;

    int numSoftkeys=0;
    #define MAX_SOFT_KEYS (NumHwButtons*2*2 + MaxFct/4*NumHwButtons*2) // erster Teil: zwei Bedienebenen mit je 2*3 Softkeys. Zweiter Teil: Funktionen erfordern jede eine Softkey plus Navigationstasten
    Softkey* softkeys[MAX_SOFT_KEYS]; 

    // Buttons in Abhängigkeit vom Modus neu setzen, dabei ist <level> die Buttonebene (Umschalten wie eine Art Shift-Taste)
    void setButtons(int level);

    // Aktive Ebene für Softkeys
    int layer = LAYER0;

    // benachbarte Seiten
    static Page* navigationGrid[PAGE_ROWS][PAGE_COLS];

    static int row, col;

    int numWidgets = 0;
    Widget* widgets[MaxWidgets];

  private:
  
    static bool isNavigable(int navigation);
    char navigable;
};

#endif