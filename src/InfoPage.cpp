#include "InfoPage.h"
#include "Pref.h"
#include "configuration.h"

#include <Z21.h>

// ----------------------------------------------------------------------------------------------------
//

#define yMessage 0.80

InfoPage::InfoPage(char navigable) : Page(navigable) {
  Z21::addObserver(static_cast<Page*>(this));
}

// ----------------------------------------------------------------------------------------------------
//

void InfoPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  String index = Pref::get(prefNameFrankyIndex, "");
  if (index == "0") index = "";

  tft->setTextDatum(CC_DATUM);

  tft->drawString("Registriert als \"" + String(HOST_NAME) + index + "\" mit IP", TFT_W/2, TFT_H*0.15, 2);
  tft->drawString(WiFi.localIP().toString(), TFT_W/2, TFT_H*0.25, 4);

  tft->drawString("Adressiere Z21 auf Port " + String(Z21_PORT) + " und IP", TFT_W/2, TFT_H*0.45, 2);
  tft->drawString(Z21::getIPAddress(), TFT_W/2, TFT_H*0.55, 4);

  tft->drawString("Zuletzt mit Z21 ausgetauschte Nachricht", TFT_W/2, TFT_H*0.75, 2);
  
  update(lastMessage, lastParameters);

  if (visible) navigationHint();
}

// ----------------------------------------------------------------------------------------------------
//

void InfoPage::update(String message, String parameters) {
  tft->fillRect(0, TFT_H*yMessage+1, TFT_W, TFT_H, bgColor);
  tft->drawString(message, TFT_W/2, TFT_H*yMessage+7, 2);
  tft->drawString(parameters, TFT_W/2, TFT_H*yMessage+23, 2);
}

void InfoPage::traceEvent(FromToZ21 direction, long diffLastSentReceived, String message, String parameters) {
  lastMessage = (direction == fromZ21 ? ">" : "<") + String(diffLastSentReceived) + "ms " + message;
  lastParameters = parameters;
  if (visible) update(lastMessage, parameters);
}

