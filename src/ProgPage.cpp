#include <ProgPage.h>
#include <LocoPage.h>
#include <M5Btn.h>
#include <SPIFFS.h>
#include "functions.h"
#include "configuration.h"
#include <Z21.h>


// ----------------------------------------------------------------------------------------------------
//

ProgPage::ProgPage(char navigable) : Page(navigable) {
  widgets[numWidgets++] = cvAddress = new Numberbox(tft, 0, "CV %d", 20, 1, 1, 1024, TFT_W/2, 40, CC_DATUM, 4);
  widgets[numWidgets++] = cvValue =   new Numberbox(tft, 0, "%d", 10, 0, 1, 255, TFT_W/2, TFT_H/2, CC_DATUM, 8); 
  widgets[numWidgets++] = pomAddress = new Numberbox(tft, 0, "Adr. %d", 20, 3, 1, MaxLocoAddr, TFT_W/2, TFT_H*0.85, CR_DATUM, 2);

  cvAddress->setFocus(true);

  Z21::addObserver(static_cast<Page*>(this));
}

// ----------------------------------------------------------------------------------------------------
//

void ProgPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CVREAD, M5Btn::A, 0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_FOCUS, M5Btn::B, 0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CVWRITE, M5Btn::C, 0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_POMWRITE, M5Btn::CC, 0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  
  setButtons(0);
  pomAddress->setValue(LocoPage::currentLoco()->getAddr())->setVisible(visible);

  if (visible) navigationHint();
  
}

// ----------------------------------------------------------------------------------------------------
//

void ProgPage::buttonPressed(M5Btn::ButtonType button) {

  if (!visible) return;

  if (clearRing) {
    clearRing = false;
    M5Btn::ledRing(0, 0, 0, 10);
  }

  if (button == M5Btn::RotaryLeft || button == M5Btn::RotaryRight) {
    cvValue->setVisible(true);
    focussedWidget()->buttonPressed(button);

  } else if (button == M5Btn::RotaryKnob && focussedWidget() == cvValue) {
    cvValue->setVisible(true);
    cvValue->setMinValue();

  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == cvValue) {
    cvValue->setVisible(true);
    cvValue->setMaxValue();

  } else if (button == M5Btn::RotaryKnob && focussedWidget() == cvAddress) {
    cvAddress->setMinValue();

  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == cvAddress) {
    cvAddress->setMaxValue();

  } else if (getFunction(button) == FN_CVREAD) {
    cvValue->setVisible(true);
    M5Btn::ledRing(200, 200, 200,  20); clearRing = true;
    Z21::LAN_X_CV_READ(cvAddress->getValue());

  } else if (getFunction(button) == FN_CVWRITE) {
    M5Btn::ledRing(200, 200, 200,  20); clearRing = true;
    Z21::LAN_X_CV_WRITE(cvAddress->getValue(), cvValue->getValue());

  } else if (getFunction(button) == FN_POMWRITE) {
    M5Btn::ledRing(200, 200, 200,  20); clearRing = true;
    Z21::LAN_X_CV_POM_WRITE_BYTE(LocoPage::currentLoco()->getAddr(), cvAddress->getValue(), cvValue->getValue());
  } 

}

// ----------------------------------------------------------------------------------------------------
//

void ProgPage::progResult(ProgResult result, int value) {

  if (!visible) return;

  cvValue->setValue(value); 
  if (result == Success)  {
    cvValue->setVisible(true,colorValueOK);
    M5Btn::ledRing(0, 200, 0, 20); clearRing = true;
  } else {
    cvValue->setVisible(true, colorValueNOK);
    M5Btn::ledRing(200, 0, 0, 20); clearRing = true;
  }
  cvValue -> setVisible(true);
  Z21::LAN_X_SET_TRACK_POWER(true); // Abschluss des Programmiermodus!

}