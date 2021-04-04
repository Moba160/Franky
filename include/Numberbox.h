#ifndef NUMBERBOX_H
#define NUMBERBOX_H

#include "Widget.h"

class Numberbox : public Widget {

    public:

        Numberbox(TFT_eSPI* tft, int style, String pattern, int length, int value, int minVal, int maxVal, int x, int y, int alignment, int textSize);

        virtual Widget* setVisible(bool visible);

        void increase(int delta);
        Widget* setMinValue() { value = minVal; setVisible(true); return this; }
        Widget* setMaxValue() { value = maxVal; setVisible(true); return this; }
        Widget* setValue(int value) { this->value = value; return this; }
        int getValue() { return value; }

        void setValueColor(int color) { fgColor = color; }

        virtual void buttonPressed(M5Btn::ButtonType button);

    private:

        int value, minVal, maxVal;
        int color = fgColor;
        String pattern;
        String blank = "";

};

#endif