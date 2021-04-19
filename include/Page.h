#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>
#include <Z21.h>
#include <Z21Observer.h>
#include <M5Btn.h>
#include "Widget.h"

/*
  Abstrakte Basisklasse der GUI
  Die GUI besteht aus einem Array aus Seiten, die von Page abgeleitet werden.
  Mit Wischgesten bewegt man sich von einer Seite zur anderen (zwischen den
  Elementen des Arrays)
*/

// Dimension des Seiten-Arrays, es gibt also (maximal) PAGE_ROWS x PAGE_COLS Seiten,
// wobei nicht zu besetzende Plätze im Array mit 0 gekennzeichnet werden
#define PAGE_ROWS 1
#define PAGE_COLS 3

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

        virtual void trackPowerStateChanged(bool trackPowerOff) override;
        virtual void progModeStateChanged(bool progModeOff) override;
        virtual void progResult(ProgResult result, int value) override {};

        virtual void setVisible(bool visible, bool clearScreen); // müsste protected sein
        virtual void navigationHint();
        boolean isVisible() { return visible; };

        static void handlePageSwitchAndFocus(M5Btn::ButtonType button);
        virtual void buttonPressed(M5Btn::ButtonType btn) {};

        virtual bool isBeta() { return false; }
        
        static Page* currentPage() { return navigationGrid[row][col]; }
        Widget* focussedWidget();

        static int bgColor;
        static int fgColor;

  protected:

    virtual void focusChanged() {};

    static TFT_eSPI* tft;
    boolean visible = false;
    static bool blocked;

    // benachbarte Seiten
    static Page* navigationGrid[PAGE_ROWS][PAGE_COLS];

    static int row, col;

    int numWidgets = 0;
    Widget* widgets[MaxWidgets];

    static int oldFocusIndex, newFocusIndex;


  private:
  
    static bool isNavigable(int navigation);
    char navigable;
};

#endif