#include "Widget.h"

// ----------------------------------------------------------------------------------------------------
//

Widget::Widget(TFT_eSPI* tft, int style, int x, int y, int alignment, int width, int height) :
    tft{tft}, x{x}, y{y}, alignment{alignment}, width{width}, height{height}, style{style} {
}



