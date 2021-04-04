#ifndef SYMBOLBOX_H
#define SYMBOLBOX_H

#include "Widget.h"

class Symbolbox : public Widget {

    public:

        Symbolbox(TFT_eSPI* tft, int style, bool value, int width, int height, int x, int y);

        virtual Widget* setVisible(bool visible);

        virtual void toggle() { value = !value; setVisible(true); }
        virtual Widget* setValue(bool value) { this->value = value; return this; }
        bool getValue() { return value; }

    private:
    
        bool value = false;
        int width, height;

};

#endif