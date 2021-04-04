#include "Symbolbox.h"

// ----------------------------------------------------------------------------------------------------
//

Symbolbox::Symbolbox(TFT_eSPI* tft, int style, bool value, int width, int height, int x, int y) : Widget(tft, style, x, y, alignment, 0, 0) {
    this->width = width;
    this->height = height;
    this->value = value;
}

// ----------------------------------------------------------------------------------------------------
//

Widget* Symbolbox::setVisible(bool visible) {
    if (visible) {
        tft->fillRect(x-width/2, y-width/2, width+2, height+2, bgColor);
        if ((style & TRIANGLE_UPDOWN)) {
            if (value) {
                tft->fillTriangle(x-width/2, y+height/2, x+width/2, y+height/2, x, y-height/2, fgColor);
            } else {
                tft->fillTriangle(x-width/2, y-height/2, x+width/2, y-height/2, x, y+height/2, fgColor);
            }
        } else if ((style & CIRCLE_FILLEDEMPTY)) {
            if (value) {
                tft->fillCircle(x, y, width/2, fgColor);
            } else {
                tft->fillCircle(x, y, width/2, bgColor);
            }
        }
    }
    return this;
}