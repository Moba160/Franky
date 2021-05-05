#ifndef WIDGET_H
#define WIDGET_H

/*
  Basis von Ausgabefeldern wie Textfeldern oder Buttons

*/

#include <M5Stack.h>
#include <M5Btn.h>

#define NOAUTOFOCUS 0b1
#define TRIANGLE_UPDOWN 0b10
#define CIRCLE_FILLEDEMPTY 0b100

class Widget {

  public:

    Widget(TFT_eSPI* tft, int style, int x, int y, int alignment, int width, int height);

    virtual Widget* setVisible(bool visible) = 0;

    virtual void setIncrement(int incr) {};

    void setSelected(boolean selected) { this->selected = selected; }
    bool isSelected() { return selected; }

    Widget* setFocus(bool focus) { this->focus = focus; return this; }
    bool hasFocus() { return focus; }
    bool canHaveFocus() { return (style & NOAUTOFOCUS) == 0; }

    virtual void buttonPressed(M5Btn::ButtonType button) {}
    virtual void toggle() {}

  protected:

    TFT_eSPI* tft;
    int x, y;
    int alignment;
    int width, height;
    int style;

    int textSize;

    int fgColor = TFT_WHITE;
    int bgColor = TFT_BLACK;
    int focusColor = TFT_YELLOW;

    // Für Buttons, die Zustand repräsentieren
    bool selected = false;

    // Hat Fokus und "Enter" betätigt
    bool focus = false;

};

#endif