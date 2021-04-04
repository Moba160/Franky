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
    static char buf[50];

    if (visible) {
        tft->setTextDatum(alignment);
        if (selected) {
            if (focus) tft->setTextColor( bgColor, focusColor); 
            else tft->setTextColor(bgColor, fgColor); 
        } else {
            if (focus) tft->setTextColor(focusColor, bgColor); 
            else tft->setTextColor(fgColor, bgColor);            
        }
        
        sprintf(buf, pattern.c_str(), value);
        tft->drawString(blank, x, y, textSize);
        tft->drawString(String(buf), x, y, textSize);
    }
    return this;
}

// ----------------------------------------------------------------------------------------------------
//

void Numberbox::increase(int delta) {
    value += delta;
    if (value < minVal) value = minVal;
    if (value > maxVal) value = maxVal;
    setVisible(true);
}

// ----------------------------------------------------------------------------------------------------
//

void Numberbox::buttonPressed(M5Btn::ButtonType button) {
    if (button == M5Btn::RotaryRight) increase(1);
    if (button == M5Btn::RotaryLeft) increase(-1);
}