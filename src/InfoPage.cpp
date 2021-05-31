#include "InfoPage.h"
#include "Pref.h"
#include "configuration.h"

#include <Z21.h>
#include "functions.h"


// ----------------------------------------------------------------------------------------------------
//

#define yMessage 0.75

InfoPage::InfoPage(char navigable) : Page(navigable) {
  Z21::addObserver(static_cast<Page*>(this));

  (softkeys[numSoftkeys++] = trackPowerSoftKey = new Softkey(tft, 0, FN_TRACKPOWER, M5Btn::B, 0, TFT_WHITE, TFT_BLUE, TFT_BLACK))
    ->setActivated(Z21::getTrackPowerState() == BoolState::On);

}

// ----------------------------------------------------------------------------------------------------
//

void InfoPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  tft->setTextColor(fgColor, bgColor);

  String index = Pref::get(prefNameFrankyIndex, "");
  if (index == "0") index = "";

  tft->setTextDatum(CC_DATUM);

  tft->drawString("Registriert als \"" + String(HOST_NAME) + index + "\" mit IP", TFT_W/2, TFT_H*0.10, 2);
  tft->drawString(WiFi.localIP().toString(), TFT_W/2, TFT_H*0.20, 4);

  tft->drawString("Adressiere Z21 auf Port " + String(Z21_PORT) + " und IP", TFT_W/2, TFT_H*0.35, 2);
  tft->drawString(Z21::getIPAddress(), TFT_W/2, TFT_H*0.45, 4);

  tft->drawString("Zuletzt mit Z21 ausgetauschte Nachricht", TFT_W/2, TFT_H*0.60, 2);
  
  update(lastMessage, lastParameters);

  if (visible) navigationHint();
}

// ----------------------------------------------------------------------------------------------------
// Die Änderung der Buttoncaption erfolgt indirekt über die Rückmeldung

void InfoPage::buttonPressed(M5Btn::ButtonType button) {
  if (!visible) return;

  bool on = Z21::getTrackPowerState() == BoolState::On;
  on = !on;

  trackPowerSoftKey->setActivated(on)->setVisible(true);
  tft->setTextColor(fgColor, bgColor);

  Z21::LAN_X_SET_TRACK_POWER(on);
}

// ----------------------------------------------------------------------------------------------------
//

void InfoPage::update(String message, String parameters) {
  tft->fillRect(0, TFT_H*yMessage+1-15, TFT_W, 40, bgColor);
  tft->drawString(message, TFT_W/2, TFT_H*yMessage-5, 2);
  tft->drawString(parameters, TFT_W/2, TFT_H*yMessage+10, 2);
}

void InfoPage::trackPowerStateChanged(BoolState trackPowerState) {
    if (!visible) return;
    getSoftkey(FN_TRACKPOWER)->setActivated(trackPowerState == BoolState::On)->setVisible(true);
    navigationHint();
    tft->setTextColor(fgColor, bgColor);
}

void InfoPage::traceEvent(FromToZ21 direction, long diffLastSentReceived, String message, String parameters) {
  lastMessage = (direction == fromZ21 ? ">" : "<") + String(diffLastSentReceived) + "ms " + message;
  lastParameters = parameters;
  if (visible) update(lastMessage, parameters);
}


