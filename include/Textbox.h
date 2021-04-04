#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Widget.h"

class Textbox : public Widget {

    public:

        Textbox(TFT_eSPI* tft, int style, String value, int x, int y, int alignment, int textSize);

        void setValue(String value) { this->value = value; setVisible(true); }
        virtual Widget* setVisible(bool visible);

        virtual void buttonPressed(M5Btn::ButtonType button) {};

    private:
    
        String value;

};

#endif