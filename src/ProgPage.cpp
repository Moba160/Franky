#include <ProgPage.h>
#include <SPIFFS.h>
#include "functions.h"
#include "configuration.h"
#include <Z21.h>


// ----------------------------------------------------------------------------------------------------
//

ProgPage::ProgPage(char navigable) : Page(navigable) {
  widgets[numWidgets++] = cvAddress = new Numberbox(tft, 0, "CV %d", 6, 1, 1, 1024, TFT_W/2, 40, CC_DATUM, 4);
  widgets[numWidgets++] = cvValue =   new Numberbox(tft, 0, "%d", 3, 0, 1, 255, TFT_W/2, TFT_H/2, CC_DATUM, 8); 

  cvAddress->setFocus(true);

  Z21::addObserver(static_cast<Page*>(this));
}

// ----------------------------------------------------------------------------------------------------
//

void ProgPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  M5Btn::setFunction(M5Btn::A,  FN_CVREAD);
  M5Btn::setFunction(M5Btn::C,  FN_CVWRITE);
  M5Btn::setFunction(M5Btn::CC, FN_POMWRITE);
  
  M5Btn::setButtonBarActive(visible);

  if (visible) navigationHint();
}

// ----------------------------------------------------------------------------------------------------
//

void ProgPage::buttonPressed(M5Btn::ButtonType button) {

  if (button == M5Btn::RotaryLeft || button == M5Btn::RotaryRight) {
    cvValue->setValueColor(fgColor);
    focussedWidget()->buttonPressed(button);

  } else if (button == M5Btn::RotaryKnob && focussedWidget() == cvValue) {
    cvValue->setValueColor(fgColor);
    cvValue->setMinValue();

  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == cvValue) {
    cvValue->setValueColor(fgColor);
    cvValue->setMaxValue();

  } else if (button == M5Btn::RotaryKnob && focussedWidget() == cvAddress) {
    cvAddress->setMinValue();

  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == cvAddress) {
    cvAddress->setMaxValue();

  } else if (M5Btn::getFunction(button) == FN_CVREAD) {
    cvValue->setValueColor(fgColor);
    Z21::LAN_X_CV_READ(cvAddress->getValue());

  } else if (M5Btn::getFunction(button) == FN_CVWRITE) {
    Z21::LAN_X_CV_WRITE(cvAddress->getValue(), cvValue->getValue());
  } 

}

// ----------------------------------------------------------------------------------------------------
//

void ProgPage::progResult(ProgResult result, int value) {

  cvValue->setValue(value); 
  cvValue->setValueColor(result == Success ? colorValueOK : colorValueNOK);
  cvValue -> setVisible(true);
  Z21::LAN_X_SET_TRACK_POWER(true); // Abschluss des Programmiermodus!

}