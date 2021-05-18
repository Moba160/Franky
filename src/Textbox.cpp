#include "Textbox.h"

// ----------------------------------------------------------------------------------------------------
//

Textbox::Textbox(TFT_eSPI* tft, int style, int length, String value, int x, int y, int alignment, int textSize) : Widget(tft, style, x, y, alignment, 0, 0) {
    this->textSize = textSize;
    this->value = value;
    for (int i=0; i<length; i++) blank += " ";
}

// ----------------------------------------------------------------------------------------------------
//

Widget* Textbox::setVisible(bool visible) {
    if (visible) {
        tft->setTextDatum(alignment);
        tft->setTextColor(focus ? focusColor : fgColor, bgColor);
        tft->drawString(blank, x, y, textSize);
        tft->drawString(value, x, y, textSize);
    } else {
        tft->setTextDatum(alignment);
        tft->setTextColor(focus ? focusColor : fgColor, bgColor);
        tft->drawString(blank, x, y, textSize);       
    }
    return this;
}