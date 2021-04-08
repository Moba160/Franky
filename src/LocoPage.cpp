#include <LocoPage.h>
#include <SPIFFS.h>
#include <M5Btn.h>
#include "functions.h"
#include <Pref.h>

// Ränder
#define BORDER_TOP 15
#define BORDER_LEFT 8
#define BORDER_RIGHT 8

// void LocoPage::dump(char* message) {
//   Serial.println(message);
//   for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
//     Serial.printf("%cCh %d Loco %d GUI %d\n", (i==channel ? '>':' '), i+1, loco[i]->addr, addr[i]->getValue());
//   }
//   Serial.println();
// }

Loco* LocoPage::selectedLoco = Loco::addLoco(Pref::get(prefNameLocoChannelAddr + String("2"), "3").toInt());

// ----------------------------------------------------------------------------------------------------
//

#define BTNSIZE 30
#define DIAMETER 20

LocoPage::LocoPage(char navigable) : Page(navigable) {
  widgets[numWidgets++] = speed = new Numberbox(tft, 0, "%d", 7, 0, 0, 126, TFT_W/2, TFT_H/2, CC_DATUM, 8); 
  widgets[numWidgets++] = targetSpeed = new Numberbox(tft, 0, "%d", 8, 0, 0, 126, TFT_W - BORDER_RIGHT, TFT_H/2, MR_DATUM, 4);   
  widgets[numWidgets++] = direction = new Symbolbox(tft, TRIANGLE_UPDOWN | NOAUTOFOCUS, false, BTNSIZE, BTNSIZE, BORDER_LEFT + BTNSIZE, TFT_H/2 - 20);
  widgets[numWidgets++] = headlights = new Symbolbox(tft, CIRCLE_FILLEDEMPTY | NOAUTOFOCUS, true, DIAMETER, DIAMETER, BORDER_LEFT + BTNSIZE, TFT_H/2 + 20);

  // diese Widgets müssen die zuletzt angelegten sein
  addressIndex = numWidgets; 
  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    widgets[numWidgets++] = addr[i] = new Numberbox(tft, NOAUTOFOCUS, "%d", 12, Pref::get(prefNameLocoChannelAddr + String(i), String(i+1)).toInt(), 1, MaxLocoAddr, TFT_W*(2*i+1)/10, BORDER_TOP, TC_DATUM, 4);
  }
  
  speed->setFocus(true);
  addr[channel]->setSelected(true);

  Z21::addObserver(static_cast<Page*>(this));

  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    loco[i] = Loco::addLoco(Pref::get(prefNameLocoChannelAddr + String(i), String(i+1)).toInt());
  }

  if (!blocked) transferData(true);

}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::transferData(bool toDisplay) {
  if (toDisplay) {
    speed->setValue(loco[channel]->fst)->setVisible(true);
    targetSpeed->setValue(loco[channel]->targetFst)->setVisible(true);
    addr[channel]->setValue(loco[channel]->addr)->setVisible(true);
    direction->setValue(loco[channel]->forward)->setVisible(true);
    headlights->setValue(loco[channel]->fct[0])->setVisible(true);
  } else {
    loco[channel]->fst = speed->getValue();
    loco[channel]->targetFst = targetSpeed->getValue();  
    loco[channel]->addr = addr[channel]->getValue(); 
    loco[channel]->forward = direction->getValue();
    loco[channel]->fct[0] = headlights->getValue();
  }
}


// ----------------------------------------------------------------------------------------------------
//

void LocoPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  M5Btn::setFunction(M5Btn::A, FN_HEADLIGHTS);
  M5Btn::setFunction(M5Btn::BB, FN_CHANGE_ADDR);
  M5Btn::setFunction(M5Btn::C, FN_CHANNELS_PLUS);
  M5Btn::setFunction(M5Btn::CC, FN_CHANNELS_MINUS);


  M5Btn::setButtonBarActive(visible);

  if (visible) navigationHint();
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::driveLoco() {
  transferData(false);
  Z21::LAN_X_SET_LOCO_DRIVE(
    loco[channel]->addr, 
    loco[channel]->forward ? Forward : Backward,
    loco[channel]->fst);  
}

// ----------------------------------------------------------------------------------------------------
// Erst nach Verlassen der Adresseingabe wird auf neue Lok umgestellt

void LocoPage::focusChanged() {
  if (oldFocusIndex >= addressIndex) {
    loco[channel] = Loco::addLoco(addr[channel]->getValue());
    transferData(true);
    Pref::set(prefNameLocoChannelAddr + String(channel), String(addr[channel]->getValue()));
    Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue());
  }
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::buttonPressed(M5Btn::ButtonType button) {

  // Lichtbutton
  if (M5Btn::getFunction(button) == FN_HEADLIGHTS) {
    headlights->toggle(); 
    transferData(false);
    Z21::LAN_X_SET_LOCO_FUNCTION(loco[channel]->addr, 0, loco[channel]->fct[0]);

  // Links-/Rechtsdreh -> Werterniedrigung/-erhöhung
  } else if (button == M5Btn::RotaryLeft || button == M5Btn::RotaryRight) {
    focussedWidget()->buttonPressed(button);
    if (focussedWidget() == addr[channel]) loco[channel]->addr = addr[channel]->getValue();

    if (focussedWidget() == speed) {
      driveLoco();
    }

  // Fahrstufe 0
  } else if (button == M5Btn::RotaryKnob && focussedWidget() == speed) {

    // Wenn schon bei 0 -> Richtungswechsel
    if (loco[channel]->fst == 0) {
      direction->toggle();
    } else {
      speed->setMinValue();
    }
    driveLoco();

  // Maximalfahrstufe
  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == speed) {
    speed->setMaxValue();
    loco[channel]->fst = speed->getValue();
    driveLoco();

  // Minimaladresse
  } else if (button == M5Btn::RotaryKnob && focussedWidget() == addr[channel]) {
    addr[channel]->setMinValue();

  // Maximaladresse
  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == addr[channel]) {
    addr[channel]->setMaxValue();

  // Zielfahrstufe 0
  } else if (button == M5Btn::RotaryKnob && focussedWidget() == targetSpeed) {
    targetSpeed->setMinValue();

  // Zielfahrstufe maximal
  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == targetSpeed) {
    targetSpeed->setMaxValue();

  } else if (M5Btn::getFunction(button) == FN_CHANGE_ADDR) {
    focussedWidget()->setFocus(false)->setVisible(true);
    addr[channel]->setFocus(true)->setVisible(true);

  // Kanäle nach rechts durchschalten
  } else if (M5Btn::getFunction(button) == FN_CHANNELS_PLUS) {
    // transferData(false);
    Pref::set(prefNameLocoChannelAddr + String(channel), String(addr[channel]->getValue()));
    int oldChannel = channel;
    channel = (channel + 1) % MAX_LOCO_CHANNELS;
    handleLocoChannel(oldChannel, channel);
    transferData(true);

  // Kanäle nach links durchschalten
  } else if (M5Btn::getFunction(button) == FN_CHANNELS_MINUS) {
    // transferData(false);
    Pref::set(prefNameLocoChannelAddr + String(channel), String(addr[channel]->getValue()));
    int oldChannel = channel;
    channel = (channel - 1 + MAX_LOCO_CHANNELS) % MAX_LOCO_CHANNELS;
    handleLocoChannel(oldChannel, channel);
    transferData(true);
  }

}

// ----------------------------------------------------------------------------------------------------
// 

void LocoPage::handleLocoChannel(int oldChannel, int channel) {

  bool hadFocus = addr[oldChannel]->hasFocus();

  // gewähltes Adressfeld gemäß Channel selektieren
  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    addr[i]->setSelected(i==channel);
    addr[i]->setFocus(false)->setVisible(true);
  }

  addr[channel]->setFocus(hadFocus)->setVisible(true);
  if (oldChannel != channel) Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue());

  selectedLoco = loco[channel];

}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::locoInfoChanged(int addr, Direction dir, int fst, bool takenOver, int numSpeedSteps, bool f[]) {
  if (loco[channel]->addr == addr) {
    direction->setValue(dir == Forward)->setVisible(true);
    headlights->setValue(f[0])->setVisible(true);
    speed->setValue(fst)->setVisible(true);
  }
}
