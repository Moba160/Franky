#include <LocoPage.h>
#include <SPIFFS.h>
#include <M5Btn.h>
#include "functions.h"
#include <Pref.h>

// Ränder
#define BORDER_TOP 15
#define BORDER_LEFT 8
#define BORDER_RIGHT 8

#define INITIAL_DELTA "10" // anfängliches Delta beim Adressschalten

void LocoPage::dump(char* message) {
  Serial.printf("%s (%s)\n", message, driveManually ? "manuell" : "aut. bremsen/beschl");
  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    Serial.printf("%cCh %d Loco @%d=%d GUI @%d=%d\n", 
      (i==channel ? '>':' '), 
      i+1, 
      loco[i]->getAddr(), loco[i]->fst, 
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

  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    loco[i] = Loco::addLoco(Pref::get(prefNameLocoChannelAddr + String(i), String(i+1)).toInt());
  }

  selectedLoco = Loco::addLoco(Pref::get(prefNameLocoChannelAddr + String("2"), "3").toInt());

  // Widgets

  widgets[numWidgets++] = locoInfo = new Textbox(tft, 0, 60, selectedLoco->name, TFT_W/2, 60, CC_DATUM, 4);
  widgets[numWidgets++] = speed = new Numberbox(tft, 0, "%d", 7, 0, 0, 126, TFT_W/2, TFT_H/2, CC_DATUM, 8); 
  speed->setIncrement(1);
  widgets[numWidgets++] = targetSpeed = new Numberbox(tft, 0, "%d", 8, 0, 0, 126, TFT_W - BORDER_RIGHT, TFT_H/2, MR_DATUM, 4);   
  targetSpeed->setIncrement(10);
  widgets[numWidgets++] = direction = new Symbolbox(tft, TRIANGLE_UPDOWN | NOAUTOFOCUS, false, BTNSIZE, BTNSIZE, BORDER_LEFT + BTNSIZE, TFT_H/2 - 20);
  widgets[numWidgets++] = headlights = new Symbolbox(tft, CIRCLE_FILLEDEMPTY | NOAUTOFOCUS, true, DIAMETER, DIAMETER, BORDER_LEFT + BTNSIZE, TFT_H/2 + 20);

  for (int i=0; i<MAX_LOCO_CHANNELS; i++) {
    widgets[numWidgets++] = addr[i] = new Numberbox(tft, NOAUTOFOCUS, "%d", 12, Pref::get(prefNameLocoChannelAddr + String(i), String(i+1)).toInt(), 1, MaxLocoAddr, TFT_W*(2*i+1)/10, BORDER_TOP, TC_DATUM, 4);
  }

  for (int i=numSoftkeys; i<MAX_SOFT_KEYS; i++) softkeys[i] = 0;

  // Softkeys Ebene 0
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_HEADLIGHTS, M5Btn::A, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = drivingModeSoftkey = new Softkey(tft, 0, FN_DRIVE_AUTO, M5Btn::B, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CHANNELS_PLUS, M5Btn::C, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = addrStepsSoftKey = new Softkey(tft, 0, CAPTION_UP, M5Btn::AA, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = libModeSoftkey = new Softkey(tft, 0, FN_CHANGE_ADDR, M5Btn::BB, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_CHANNELS_MINUS, M5Btn::CC, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);

  // Softkeys Ebene 1 - nur für Adressschritte
  softkeys[firstDeltaKey = numSoftkeys++] = new Softkey(tft, 0, "1", M5Btn::A, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  (softkeys[numSoftkeys++] = new Softkey(tft, 0, INITIAL_DELTA, M5Btn::B, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK))->setActivated(true);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, "100", M5Btn::C, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_DOWN, M5Btn::AA, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, "50", M5Btn::BB, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[lastDeltaKey = numSoftkeys++] = new Softkey(tft, 0, "500", M5Btn::CC, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);

  firstFunctionSoftkeyIndex = numSoftkeys;
  setFunctionSoftkeys();
  
  // Modi
  inLibMode = Pref::get(prefNameLocoLib, "off") == "on";
  libModeSoftkey->setActivated(inLibMode);
  driveManually = Pref::get(prefNameDriveAutomatically, "off") == "off";
  drivingModeSoftkey->setActivated(!driveManually);
  setMinMaxAddr();
  
  if (driveManually) {
    speed->setFocus(true);
    targetSpeed->setVisible(false);
  } else {
    targetSpeed->setFocus(true);
  }
  addr[channel]->setSelected(true);

  // Etc

  Z21::addObserver(static_cast<Page*>(this));

}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::setFunctionSoftkeys() {

  numSoftkeys = firstFunctionSoftkeyIndex;

  for (int i=numSoftkeys; i<MAX_SOFT_KEYS; i++) {
    if (softkeys[i] != 0) {
      delete(softkeys[i]); // da ja dynamisch - siehe unten - je Lok neu erzeugt wird: alte Softkeys abräumen
      softkeys[i]=0;
    }
  }

  int l=1; 
  // Ab Ebene 2 Funktionstasten

  for (int f=1; f <= loco[channel]->getNumFct(); f++) {
    switch(f%4) {
      case 1: // neue Seite beginnen (++) -> Navigationstasten und Funktionstaste auf BB
        l++;
        softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_DOWN, M5Btn::A, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
        softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_UP, M5Btn::AA, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
        softkeys[numSoftkeys++] = new Softkey(tft, 0, loco[channel]->getFct(f)->getShortName(), M5Btn::B, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
        break;
      case 2:
        softkeys[numSoftkeys++] = new Softkey(tft, 0, loco[channel]->getFct(f)->getShortName(), M5Btn::C, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
        break;
      case 3:
        softkeys[numSoftkeys++] = new Softkey(tft, 0, loco[channel]->getFct(f)->getShortName(), M5Btn::BB, l, TFT_WHITE, TFT_BLUE, TFT_BLACK);
        break;
      case 0:
        softkeys[numSoftkeys++] = new Softkey(tft, 0, loco[channel]->getFct(f)->getShortName(), M5Btn::CC, l, TFT_WHITE, TFT_BLUE, TFT_BLACK); 
        break;
    }  
  }
  numLayers=l+1;
  handleAddrStepKey();
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::handleAddrStepKey() {
  String oldCaption = addrStepsSoftKey->getCaption();
  String newCaption;
  
  // In Ebene 0 keine Umsschaltfunktion zu weiteren F-Seiten nötig, wenn nur F0 benötigt werden
  if (layer == LAYER0  && loco[channel]->getNumFct() == 0) {
    newCaption = "";

  // in Layer 1 nur Deltaschritte, von dort Zurückschalten abieten
  } else if (layer == LAYER1) {
    newCaption = CAPTION_DOWN;
    
  // Ansonsten Umschalttaste für weitere F-Seiten nötig  
  } else {
    newCaption = CAPTION_UP;
  }

  if (newCaption != oldCaption) {
      addrStepsSoftKey->setVisible(false);
      if (newCaption == "") return;
      addrStepsSoftKey->setCaption(newCaption)->setVisible(true);
  }

}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::setMinMaxAddr() {
  for (int c=0; c<MAX_LOCO_CHANNELS; c++) {
    addr[c]->setMinValue(inLibMode ? Loco::getMinAddress() : 1);
    addr[c]->setMaxValue(inLibMode ? Loco::getMaxAddress() : MaxLocoAddr);
  }
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::setVisible(bool visible, bool clearScreen) {
  Page::setVisible(visible, clearScreen);
  if (visible) {
    navigationHint();
    Z21::LAN_X_GET_LOCO_INFO(addr[channel]->getValue());
  }
  setButtons(layer);
  addrStepsSoftKey->setVisible(inAddressChangeMode || loco[channel]->getNumFct() != 0);
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::driveLoco() {
  loco[channel]->takenOver=false;
  Z21::LAN_X_SET_LOCO_DRIVE(
    loco[channel]->getAddr(), 
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
      if (inLibMode) {
        if (button == M5Btn::RotaryLeft) { // "weiter"drehen zur nächsten vorhandenen Adresse, wenn nicht jetzt auf bekannter Adresse stehend
          while (Loco::existsLoco(addr[channel]->getValue()) == 0 && addr[channel]->getValue() > Loco::getMinAddress()) {
            addr[channel]->increase(-1);
          }
        } else {
          while (Loco::existsLoco(addr[channel]->getValue()) == 0 && addr[channel]->getValue() < Loco::getMaxAddress()) {
            addr[channel]->increase(+1);
          }
        }
      }
      loco[channel] = Loco::addLoco(addr[channel]->getValue()); // falls noch nicht im Zugriff
      locoAddressChanged();
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
    loco[channel]->getFct(0)->setActive(headlights->getValue());
    Z21::LAN_X_SET_LOCO_FUNCTION(loco[channel]->getAddr(), 0, loco[channel]->getFct(0)->isActive());

  // Funktionen
  } else if (layer >= 1 && !inAddressChangeMode && loco[channel]->getFctByName(getFunction(button)) != 0) {
    Softkey* softkey = getSoftkey(getFunction(button));
    softkey->setActivated(!softkey->isActivated())->setVisible(true);
    Locofunction* fct = loco[channel]->getFctByName(getFunction(button));
    fct->setActive(softkey->isActivated());
    Z21::LAN_X_SET_LOCO_FUNCTION(addr[channel]->getValue(), fct->getMappedTo(), softkey->isActivated());

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
      for (int i=0; i<MAX_LOCO_CHANNELS; i++) addr[i]->setIncrement(String(INITIAL_DELTA).toInt());
    }

  } else if (getFunction(button) == CAPTION_DOWN) {
    if (!inAddressChangeMode and layer == 2) {
      layer = (layer - 2 + numLayers)  % numLayers;
    } else {
      for (int i=0; i<MAX_LOCO_CHANNELS; i++) addr[i]->setIncrement(1);
      layer = (layer - 1 + numLayers)  % numLayers;
      handleAddrStepKey();
    }
    setButtons(layer);

  } else if (
      getFunction(button) == "1" || 
      getFunction(button) == INITIAL_DELTA ||
      getFunction(button) == "50" ||
      getFunction(button) == "100" ||
      getFunction(button) == "500") {

    int incr = getFunction(button).toInt();
    for (int i=0; i<MAX_LOCO_CHANNELS; i++) addr[i]->setIncrement(incr);
    for (int i=firstDeltaKey; i<=lastDeltaKey; i++) {
      if (softkeys[i]->isActivated()) softkeys[i]->setActivated(false)->setVisible(true); // alte Deltas löschen
    }
    getSoftkey(button)->setActivated(true)->setVisible(true);  // hervorheben, dass nun dieses Delta gilt
    addr[channel]->increase(incr);

  // ===================== andere Tasten

    } else if (getFunction(button) == FN_CHANGE_ADDR) {

      // Sonderfall: wenn bereits im Adressmodus wird Umschalten des Bibliotheksmodus vollzogen ...
      if (inAddressChangeMode) {
        inLibMode = !inLibMode;
        setMinMaxAddr();
        libModeSoftkey->setActivated(inLibMode)->setVisible(true);
      } else {
        inAddressChangeMode = true;
        addrStepsSoftKey->setVisible(true);
        focussedWidget()->setFocus(false)->setVisible(true);
        addr[channel]->setFocus(true)->setVisible(true);
      }

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
    
    if (inLibMode) {
      addr[channel]->setValue(Loco::getMinAddress())->setVisible(true);
    } else {
      addr[channel]->setMinValue();
    }
    locoAddressChanged();

  // Maximaladresse
  } else if (button == M5Btn::RotaryKnobLong && focussedWidget() == addr[channel]) {
    if (inLibMode) {
      addr[channel]->setValue(Loco::getMaxAddress())->setVisible(true);
    } else {
      addr[channel]->setMaxValue();
    }    
    locoAddressChanged();

  // Nur Zurückschalten vom Adressmodus -> alten driveManually-State berücksichtigen, nicht umschalten!
  } else if (inAddressChangeMode && getFunction(button) == FN_DRIVE_AUTO) {
    inAddressChangeMode = false;
    focussedWidget()->setFocus(false)->setVisible(true);
    if (driveManually) speed->setFocus(true)->setVisible(true);
    else targetSpeed->setFocus(true)->setVisible(true);

  // Fahrmodus manuell -> automatisch
  } else if (getFunction(button) == FN_DRIVE_AUTO && driveManually) {
    inAddressChangeMode = false;
    focussedWidget()->setFocus(false)->setVisible(true);
    speed->setFocus(false)->setVisible(true);
    targetSpeed->setFocus(true)->setVisible(true);
    driveManually = false;
    drivingModeSoftkey->setActivated(!driveManually)->setVisible(true);
    setButtons(LAYER0);
    addrStepsSoftKey->setVisible(loco[channel]->getNumFct() != 0);
  
  // Fahrmodus automatisch -> manuell
  } else if (getFunction(button) == FN_DRIVE_AUTO && !driveManually) {
    inAddressChangeMode = false;
    if (focussedWidget() == targetSpeed) {
      targetSpeed->setFocus(false)->setVisible(false);
    } else focussedWidget()->setFocus(false)->setVisible(true);
    speed->setFocus(true)->setVisible(true);
    targetSpeed->setFocus(false)->setVisible(true);
    driveManually = true;
    drivingModeSoftkey->setActivated(!driveManually)->setVisible(true);
    setButtons(LAYER0);
    addrStepsSoftKey->setVisible(loco[channel]->getNumFct() != 0);
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
  locoAddressChanged();

}

// ----------------------------------------------------------------------------------------------------
// 

void LocoPage::locoAddressChanged() {
  int locoAddr = addr[channel]->getValue();

  Loco* loc = Loco::existsLoco(locoAddr);
  if (loc != 0) {
    locoInfo->setValue(loc->name)->setVisible(true);
  } else  {
    locoInfo->setVisible(false); 
  }

  Z21::LAN_X_GET_LOCO_INFO(locoAddr);

  // mit neuer Adresse auch die Softkeys für Funktionen anpassen
  setFunctionSoftkeys();
}

// ----------------------------------------------------------------------------------------------------
//

void LocoPage::update() {

  speed->setValue(loco[channel]->fst);
  targetSpeed->setValue(loco[channel]->targetFst)->setVisible(true);
  direction->setValue(loco[channel]->forward)->setVisible(true);
  headlights->setValue(loco[channel]->getFct(0)->isActive())->setVisible(true);  

  // Funktionszustände auf zugehörige Buttons übertragen
  for (int i=1; i<=loco[channel]->getNumFct(); i++) {
    for (int j=0; j<numSoftkeys; j++) {
      if (loco[channel]->getFct(i)->getShortName() == softkeys[j]->getCaption()) {
        softkeys[j]->setActivated(loco[channel]->getFct(i)->isActive());
        // Aktualisieren, wenn gerade sichtbar
        if (layer == softkeys[j]->getLayer()) softkeys[j]->setVisible(true);
      }
    }
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
      loco[channel]->getFct(j)->setActive(f[j]);
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
