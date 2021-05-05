#include <LocoPage.h>
#include <SPIFFS.h>
#include <M5Btn.h>
#include "functions.h"
#include <Pref.h>

// Ränder
#define BORDER_TOP 15
#define BORDER_LEFT 8
#define BORDER_RIGHT 8

#define FPref "F"

#define INITIAL_DELTA "100" // anfängliches Delta beim Adressschalten

void LocoPage::dump(char* message) {
  Serial.printf("%s (%s)\n", message, driveManually ? "manuell" : "aut. bremsen/beschl");
  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    Serial.printf("%cCh %d Loco @%d=%d GUI @%d=%d\n", 
      (i==channel ? '>':' '), 
      i+1, 
      loco[i]->addr, loco[i]->fst, 
      addr[i]->getValue(), loco[i]->targetFst
    );
  }
  Serial.println();
}

Loco* LocoPage::selectedLoco;

// ----------------------------------------------------------------------------------------------------
// Konstruktor: Oberfläche initialisieren

#define BTNSIZE 30
#define DIAMETER 20

#define F_PER_PAGE 4 // vob 6 Softkeys können 4 benutzt werden, weil ...
#define SKIP_PER_PAGE 2 // ... UP/DOWN abgezogen werden müssen

int fctIndexToSoftkeyIndex(int fctIndex) {
  if (fctIndex >= 1 and fctIndex <= 4) return fctIndex + 1;
  if (fctIndex >= 5 and fctIndex <= 8) return fctIndex + 3;
  if (fctIndex >= 9 and fctIndex <= 12) return fctIndex + 5;
  if (fctIndex >= 13 and fctIndex <= 16) return fctIndex + 7;
  if (fctIndex >= 17 and fctIndex <= 20) return fctIndex + 9;
  if (fctIndex >= 21 and fctIndex <= 24) return fctIndex + 11;
  if (fctIndex >= 25 and fctIndex <= 28) return fctIndex + 13;

}

LocoPage::LocoPage(char navigable) : Page(navigable) {

  // Widgets

  widgets[numWidgets++] = speed = new Numberbox(tft, 0, "%d", 7, 0, 0, 126, TFT_W/2, TFT_H/2, CC_DATUM, 8); 
  speed->setIncrement(1);
  widgets[numWidgets++] = targetSpeed = new Numberbox(tft, 0, "%d", 8, 0, 0, 126, TFT_W - BORDER_RIGHT, TFT_H/2, MR_DATUM, 4);   
  targetSpeed->setIncrement(10);
  widgets[numWidgets++] = direction = new Symbolbox(tft, TRIANGLE_UPDOWN | NOAUTOFOCUS, false, BTNSIZE, BTNSIZE, BORDER_LEFT + BTNSIZE, TFT_H/2 - 20);
  widgets[numWidgets++] = headlights = new Symbolbox(tft, CIRCLE_FILLEDEMPTY | NOAUTOFOCUS, true, DIAMETER, DIAMETER, BORDER_LEFT + BTNSIZE, TFT_H/2 + 20);

  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    widgets[numWidgets++] = addr[i] = new Numberbox(tft, NOAUTOFOCUS, "%d", 12, Pref::get(prefNameLocoChannelAddr + String(i), String(i+1)).toInt(), 1, MaxLocoAddr, TFT_W*(2*i+1)/10, BORDER_TOP, TC_DATUM, 4);
  }
  
  if (driveManually) {
    speed->setFocus(true);
    targetSpeed->setVisible(false);
  } else {
    targetSpeed->setFocus(true);
  }
  addr[channel]->setSelected(true);

  // ab index 1 die Funktionen  1 bis MaxFct unterbringen -> direktes Mapping beider Indizes
  //  NumFct/4 Seiten für Funktionen. Links up/down, auf restlichen Softkeys Funktionstasten
  int f=1;
  for (int l=2; l<2+MaxFct/F_PER_PAGE; l++) {
    softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_UP, M5Btn::AA, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
    softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_DOWN, M5Btn::A, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
    softkeys[numSoftkeys++] = new Softkey(tft, 0, FPref +String(f++), M5Btn::BB, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
    softkeys[numSoftkeys++] = new Softkey(tft, 0, FPref+String(f++), M5Btn::CC, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
    softkeys[numSoftkeys++] = new Softkey(tft, 0, FPref+String(f++), M5Btn::B, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
    softkeys[numSoftkeys++] = new Softkey(tft, 0, FPref+String(f++), M5Btn::C, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);   
  }

  // Softkeys Ebene 1
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_HEADLIGHTS, M5Btn::A, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, driveManually ? FN_CHANGE_TARGETSPEED : FN_CHANGE_SPEED, M5Btn::B, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CHANNELS_PLUS, M5Btn::C, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_UP, M5Btn::AA, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CHANGE_ADDR, M5Btn::BB, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CHANNELS_MINUS, M5Btn::CC, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);

  // Softkeys Ebene 2
  softkeys[numSoftkeys++] = new Softkey(tft, 0, "1", M5Btn::A, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, "10", M5Btn::B, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, INITIAL_DELTA, M5Btn::C, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_DOWN, M5Btn::AA, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, "50", M5Btn::BB, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, "500", M5Btn::CC, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);



  // Etc

  Z21::addObserver(static_cast<Page*>(this));

  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    loco[i] = Loco::addLoco(Pref::get(prefNameLocoChannelAddr + String(i), String(i+1)).toInt());
  }

  selectedLoco = Loco::addLoco(Pref::get(prefNameLocoChannelAddr + String("2"), "3").toInt());

}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::setVisible(bool visible, bool clearScreen) {
  Page::setVisible(visible, clearScreen);
  if (visible) {
    navigationHint();
    Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue());
  }
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::driveLoco() {
  loco[channel]->takenOver=false;
  Z21::LAN_X_SET_LOCO_DRIVE(
    loco[channel]->addr, 
    loco[channel]->forward ? Forward : Backward,
    loco[channel]->fst);  
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::focusChanged() {}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::buttonPressed(M5Btn::ButtonType button) {

  // ===================== Fahren

   // Links-/Rechtsdreh -> Werterniedrigung/-erhöhung (Geschwindigkeit oder Adresse)
  if (button == M5Btn::RotaryLeft || button == M5Btn::RotaryRight) {
    focussedWidget()->buttonPressed(button);
    if (focussedWidget() == addr[channel]) {
      loco[channel]->addr = addr[channel]->getValue();
      Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue()); 
    }

    if (focussedWidget() == speed) {
      targetSpeed->setValue(speed->getValue())->setVisible(true);
      loco[channel]->fst = loco[channel]->targetFst = speed->getValue();
      driveLoco();
    }

    if (focussedWidget() == targetSpeed) {
      loco[channel]->targetFst = targetSpeed->getValue();
    }
  
  // Licht
  } else  if (getFunction(button) == FN_HEADLIGHTS) {
    headlights->toggle(); 
    loco[channel]->fct[0] = headlights->getValue();
    Z21::LAN_X_SET_LOCO_FUNCTION(loco[channel]->addr, 0, loco[channel]->fct[0]);

  // Funktionen
  } else if (getFunction(button).startsWith(FPref)) {
    String caption = getFunction(button);
    int f = caption.substring(String(FPref).length()).toInt();
    if (f != 0) { // Falls eine Funktion regulär mit FPref anfängt, aber keine Form hat F# mit # = numerisch     
      Softkey* softkey = getSoftkey(caption);
      softkey -> setActivated(!softkey->isActivated())->setVisible(true);
      Z21::LAN_X_SET_LOCO_FUNCTION(addr[channel]->getValue(), f, softkey->isActivated());
    }
  // Manueller Betrieb: Fahrstufe 0 setzen oder Richtungswechsel
  } else if (button == M5Btn::RotaryKnob && focussedWidget() == speed) {
    // Wenn schon bei 0 -> Richtungswechsel
    if (speed->getValue() == 0) {
      direction->toggle();
      loco[channel]->forward = direction->getValue();
    } else {
      speed->setMinValue(); loco[channel]->fst  = speed->getValue();;
      targetSpeed->setMinValue();  loco[channel]->targetFst = targetSpeed->getValue();
    }
    driveLoco();

  // Automatischer Betrieb: Fahrstufe 0 setzen oder Richtungswechsel
  } else if (button == M5Btn::RotaryKnob && focussedWidget() == targetSpeed) {
    // Wenn schon bei 0 -> Richtungswechsel
    if (targetSpeed->getValue() == 0) {
      direction->toggle();
      loco[channel]->forward = direction->getValue();
    } else {
      targetSpeed->setMinValue();  loco[channel]->targetFst = targetSpeed->getValue();
    }
    driveLoco();

  // Maximalfahrstufe
  } else if (button == M5Btn::RotaryKnobLong && (focussedWidget() == speed || focussedWidget() == targetSpeed)) {
    if (focussedWidget() == speed) { speed->setMaxValue(); loco[channel]->fst = speed->getValue(); }
    targetSpeed->setMaxValue(); loco[channel]->targetFst = targetSpeed->getValue();
    driveLoco();  

  // ===================== Ebenen und Inkrementänderung 

  } else if (getFunction(button) == CAPTION_UP) {
    if (!inAddressChangeMode and layer == 0) {
        layer +=2;
    } else {
      layer++;
    }
    layer = layer % numLayers;
    setButtons(layer);

    if (inAddressChangeMode) {
      getSoftkey(INITIAL_DELTA)->setActivated(true);
      for (int i=0; i<MAX_LOCO_CHANNELS; i++) addr[i]->setIncrement(String(INITIAL_DELTA).toInt());
    }

  } else if (getFunction(button) == CAPTION_DOWN) {
    if (!inAddressChangeMode and layer == 2) {
      layer = (layer - 2 + numLayers)  % numLayers;
    } else {
      for (int i=0; i<MAX_LOCO_CHANNELS; i++) addr[i]->setIncrement(1);
      layer = (layer - 1 + numLayers)  % numLayers;
    }
    setButtons(layer);

  } else if (
      getFunction(button) == "1" || 
      getFunction(button) == "10" ||
      getFunction(button) == "50" ||
      getFunction(button) == "INITIAL_DELTA" ||
      getFunction(button) == "500") {

    int incr = getFunction(button).toInt();
    for (int i=0; i<MAX_LOCO_CHANNELS; i++) addr[i]->setIncrement(incr);
    for (int i=0; i<numSoftkeys; i++) {
      if (softkeys[i]->isActivated()) softkeys[i]->setActivated(false)->setVisible(true);
    }
    getSoftkey(button)->setActivated(true)->setVisible(true);  // hervorheben, dass nun dieses Delta gilt
    addr[channel]->increase(incr);

  // ===================== andere Tasten

    } else if (getFunction(button) == FN_CHANGE_ADDR) {
      inAddressChangeMode = true;
      focussedWidget()->setFocus(false)->setVisible(true);
      addr[channel]->setFocus(true)->setVisible(true);

      setFunction(M5Btn::B, driveManually ? FN_CHANGE_SPEED : FN_CHANGE_TARGETSPEED);

  // Adresskanäle nach rechts durchschalten
  } else if (getFunction(button) == FN_CHANNELS_PLUS) {
    Pref::set(prefNameLocoChannelAddr + String(channel), String(addr[channel]->getValue()));
    int oldChannel = channel;
    channel = (channel + 1) % MAX_LOCO_CHANNELS;
    locoChannelChanged(oldChannel, channel);

  // Adresskanäle nach links durchschalten
  } else if (getFunction(button) == FN_CHANNELS_MINUS) {
    // transferData(toMemory);
    Pref::set(prefNameLocoChannelAddr + String(channel), String(addr[channel]->getValue()));
    int oldChannel = channel;
    channel = (channel - 1 + MAX_LOCO_CHANNELS) % MAX_LOCO_CHANNELS;
    locoChannelChanged(oldChannel, channel);

  // Minimaladresse
  } else if (button == M5Btn::RotaryKnob && focussedWidget() == addr[channel]) {
    addr[channel]->setMinValue();
    Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue());

  // Maximaladresse
  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == addr[channel]) {
    addr[channel]->setMaxValue();
    Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue());

  // Fahrmodus manuell -> automatisch
  } else if (getFunction(button) == FN_CHANGE_TARGETSPEED) {
    inAddressChangeMode = false;
    focussedWidget()->setFocus(false)->setVisible(true);
    speed->setFocus(false)->setVisible(true);
    targetSpeed->setFocus(true)->setVisible(true);
    driveManually = false;
    setFunction(M5Btn::B, FN_CHANGE_SPEED);
    setButtons(LAYER0);
  
  // Fahrmodus automatisch -> manuell
  } else if (getFunction(button) == FN_CHANGE_SPEED) {
    inAddressChangeMode = false;
    if (focussedWidget() == targetSpeed) {
      targetSpeed->setFocus(false)->setVisible(false);
    } else focussedWidget()->setFocus(false)->setVisible(true);
    speed->setFocus(true)->setVisible(true);
    targetSpeed->setFocus(false)->setVisible(true);
    driveManually = true;
    setFunction(M5Btn::B, FN_CHANGE_TARGETSPEED);
    setButtons(LAYER0);
  }

}

// ----------------------------------------------------------------------------------------------------
// 

void LocoPage::locoChannelChanged(int oldChannel, int channel) {

  bool hadFocus = addr[oldChannel]->hasFocus();

  // gewähltes Adressfeld gemäß Channel selektieren
  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    addr[i]->setSelected(i==channel);
    addr[i]->setFocus(false)->setVisible(true);
  }
  addr[channel]->setFocus(hadFocus)->setVisible(true);

  selectedLoco = loco[channel];
  Z21::LAN_X_GET_LOCO_INFO(selectedLoco->addr);

}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::update() {

  speed->setValue(loco[channel]->fst);
  targetSpeed->setValue(loco[channel]->targetFst)->setVisible(true);
  direction->setValue(loco[channel]->forward)->setVisible(true);
  headlights->setValue(loco[channel]->fct[0])->setVisible(true);  

  // Funktionszustände auf zugehörige Buttons übertragen
  for (int i=1; i<=MaxFct; i++) {
    Serial.printf("Taste %s ist %d\n", softkeys[fctIndexToSoftkeyIndex(i)]->getCaption().c_str(), loco[channel]->fct[i]);
    if (loco[channel]->fct[i] != softkeys[fctIndexToSoftkeyIndex(i)]->isActivated()) {
      Serial.printf("\that sich geändert\n");
      if (softkeys[fctIndexToSoftkeyIndex(i)]->getLayer() == layer) { // Layer aktiv = Softkey sichtbar
        Serial.printf("\tist sichtbar und wird gesetzt\n");
        softkeys[fctIndexToSoftkeyIndex(i)]->setActivated(loco[channel]->fct[i])->setVisible(true); // Anzeige (Update) forcieren
      }
    }
    softkeys[fctIndexToSoftkeyIndex(i)]->setActivated(loco[channel]->fct[i]);
    Serial.printf("\tsteht nun auf %d\n", softkeys[fctIndexToSoftkeyIndex(i)]->isActivated());
  }

  if (loco[channel]->takenOver) {
    speed->setVisible(true, COLOR_TAKENOVER); 
  } else {
    if (loco[channel]->isAccelerating()) speed->setVisible(true, COLOR_ACCELERATING);
    if (loco[channel]->isDecelerating()) speed->setVisible(true, COLOR_DECELERATING);
    if (loco[channel]->isCoasting()) speed->setVisible(true);
  }
}

// ----------------------------------------------------------------------------------------------------
// Lok ist gefahren worden ...

// ... durch Notifikation der Zentrale

void LocoPage::locoInfoChanged(int address, Direction dir, int fst, bool takenOver, int numSpeedSteps, bool f[]) {
  
  if (!visible) return;

  if (address == addr[channel]->getValue()) {
    loco[channel]->forward = dir == Forward;
    if (driveManually) loco[channel]->targetFst = fst;
    loco[channel]->takenOver = takenOver;
    if (driveManually) loco[channel]->fst = fst; // hier! Funktioniert nicht für Fremdssteuern?
    if (takenOver) loco[channel]->targetFst = fst; 
    for (int j=0; j<MaxFct; j++)  {
      loco[channel]->fct[j] = f[j];
    }
    update();    
  }

}

// ... durch Loco::drive()

void LocoPage::locoWasDriven(int addr) {
  if (visible) {
    update();
  }
}
