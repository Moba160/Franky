#include "Widget.h"

int Widget::fgColor = TFT_WHITE;
int Widget::bgColor = TFT_BLACK;
int Widget::focusColor = TFT_YELLOW;

// ----------------------------------------------------------------------------------------------------
//

Widget::Widget(TFT_eSPI* tft, int style, int x, int y, int alignment, int width, int height) :
    tft{tft}, x{x}, y{y}, alignment{alignment}, width{width}, height{height}, style{style} {
}



