#include "Numberbox.h"

// ----------------------------------------------------------------------------------------------------
//

Numberbox::Numberbox(TFT_eSPI* tft, int style, String pattern, int length, int value, int minVal, int maxVal, int x, int y, int alignment, int textSize) : Widget(tft, style, x, y, alignment, 0, 0) {
    this->textSize = textSize;
    this->pattern = pattern;
    this->value = value;
    this->minVal = minVal;
    this->maxVal = maxVal;
    for (int i=0; i<length; i++) blank += " ";
}

// ----------------------------------------------------------------------------------------------------
//

Widget* Numberbox::setVisible(bool visible) {

    if (visible) {
        if (selected) {
            if (focus) tft->setTextColor(bgColor, focusColor); 
            else tft->setTextColor(bgColor, fgColor); 
        } else {
            if (focus) tft->setTextColor(focusColor, bgColor); 
            else tft->setTextColor(fgColor, bgColor);            
        }    
    } 
    draw(visible);
    return this;
}

Widget* Numberbox::setVisible(bool visible, int color) {
    tft->setTextColor(color, bgColor);
    draw(visible);
    return this;
}

void Numberbox::draw(bool visible) {
    static char buf[50];
    tft->setTextDatum(alignment);
    sprintf(buf, pattern.c_str(), value);
    tft->drawString(blank, x, y, textSize);
    if (visible) tft->drawString(String(buf), x, y, textSize); 
}

// ----------------------------------------------------------------------------------------------------
//

void Numberbox::increase(int delta) {
    if (delta < 0) {
        if (value % incr == 0) value -= incr;
        else value -= value % incr;
    } else {
        if (value % incr == 0) value += incr;
        else value += incr - value % incr;
    }

    if (value < minVal) value = minVal;
    if (value > maxVal) value = maxVal;

    setVisible(true);
}

// ----------------------------------------------------------------------------------------------------
//

void Numberbox::buttonPressed(M5Btn::ButtonType button) {
    if (button == M5Btn::RotaryRight) increase(incr);
    if (button == M5Btn::RotaryLeft) increase(-incr);
}