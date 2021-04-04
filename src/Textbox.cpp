#include "Textbox.h"

// ----------------------------------------------------------------------------------------------------
//

Textbox::Textbox(TFT_eSPI* tft, int style, String value, int x, int y, int alignment, int textSize) : Widget(tft, style, x, y, alignment, 0, 0) {
    this->textSize = textSize;
    this->value = value;
}

// ----------------------------------------------------------------------------------------------------
//

Widget* Textbox::setVisible(bool visible) {
    if (visible) {
        tft->setTextDatum(alignment);
        tft->setTextColor(focus ? focusColor : fgColor, bgColor);
        tft->drawString(value, x, y, textSize);
    }
    return this;
}