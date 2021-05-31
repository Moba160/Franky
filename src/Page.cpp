#include "Page.h"
#include "LocoPage.h"
#include "ProgPage.h"
#include "InfoPage.h"
#include "TrackPage.h"

#include "M5Btn.h"
#include "functions.h"
#include <M5Stack.h>
#include <configuration.h>
#include <Z21.h>
#include "Webserver.h"

TFT_eSPI* Page::tft;

int Page::fgColor = TFT_WHITE;
int Page::bgColor = TFT_BLACK;

int Page::row;
int Page::col;

Page* Page::navigationGrid[PAGE_ROWS][PAGE_COLS];
bool Page::blocked = false;

// int Page::oldFocusIndex, Page::newFocusIndex;


// ----------------------------------------------------------------------------------------------------
//

void Page::begin(TFT_eSPI* tft) {

  M5Btn::begin(tft);
  Page::tft = tft;

  ProgPage* pp = new ProgPage(NAV_ALL);
  LocoPage* lp = new LocoPage(NAV_ALL);
  InfoPage* ip = new InfoPage(NAV_ALL);
  TrackPage *tp = new TrackPage(NAV_ALL);

  // Initialisierung oben hat nicht funktioniert, Konstruktoren wurden nicht gerufen
  Page* tmp[PAGE_ROWS][PAGE_COLS] = {
      {0, tp, 0},
      {ip, lp, pp}
    };

    memcpy(navigationGrid, tmp, sizeof(tmp));

    // initiale Seite
    row = 1; col = 1; 
}

// ----------------------------------------------------------------------------------------------------
//

void Page::loop() {
}

// ----------------------------------------------------------------------------------------------------
//

#define NAV_IND_LEN  10 // Länge des Indikatorbalkens
#define NAV_IND_WIDTH 4 // Dicke 

void Page::setVisible(bool visible, bool clearScreen) {

  this->visible = visible;
  // setFunction(M5Btn::B, FN_FOCUS);
  if (clearScreen) tft->fillScreen(bgColor);
  tft->setTextColor(fgColor, bgColor);
  for (int i=0; i<this->numWidgets; i++) this->widgets[i]->setVisible(visible);
  if (visible) setButtons(LAYER0);

}

// ----------------------------------------------------------------------------------------------------
//

void Page::navigationHint() {

  if (blocked || !visible) return;

  //----------------- Diese Stati nicht mehr im Record, sondern in Notifikation, oder???? Ansonsten fehlen sie dort

  int color = colorAllConnected;
  if (WiFi.status() != WL_CONNECTED) color = colorWiFiDisconnected;
  else if (millis() - Z21::lastReceived > Z21_HEARTBEAT) color = colorDigiStationDisconnected;
  else if (Z21::getProgState() == BoolState::On) color = colorProgMode;
  else if (Z21::getTrackPowerState() == BoolState::Off) color = colorTrackPowerOff;

  // Status: Balken oben
  tft->fillRect(0, 0, TFT_W, NAV_IND_WIDTH, color);

  // Status: LED-RING
  if (Z21::getTrackPowerState() == BoolState::Off) M5Btn::ledRing(255, 255, 0, 10);
  else M5Btn::ledRing(0, 0, 0, 10);

  if (isBeta()) {
    tft->setTextColor(TFT_RED, bgColor);
    tft->setTextDatum(TC_DATUM);
    if (Pref::get("beta") == ON && isBeta()) tft->drawString(" Beta ", TFT_W/2, 0, 2);
  }

  // Am jeweiligen Rand Navigationshinweis (dass es dort eine Nachbarseite gibt):

  // oben
  if (isNavigable(NAV_UP)) 
    tft->fillRect(tft->width()/2-NAV_IND_LEN/2, 0, NAV_IND_LEN, NAV_IND_WIDTH, fgColor);

  // unten
  if (isNavigable(NAV_DOWN)) 
    tft->fillRect(tft->width()/2-NAV_IND_LEN/2, tft->height()-NAV_IND_WIDTH+1, NAV_IND_LEN, NAV_IND_WIDTH, fgColor);

  // links
  if (isNavigable(NAV_LEFT)) 
    tft->fillRect(0, tft->height()/2-NAV_IND_LEN/2, NAV_IND_WIDTH, NAV_IND_LEN, fgColor);
  
  // rechts
  if (isNavigable(NAV_RIGHT)) 
    tft->fillRect(tft->width()-NAV_IND_WIDTH+1, tft->height()/2-NAV_IND_LEN/2, NAV_IND_WIDTH, NAV_IND_LEN, fgColor);

}

// ----------------------------------------------------------------------------------------------------
//

bool Page::isNavigable(int navigation) {

  bool navigable = false;
  int newCol = col, newRow = row;

  switch(navigation) {
    case NAV_UP:
      if (row > 0) if (navigationGrid[newRow = row-1][col] != 0) if (currentPage()->navigable & NAV_UP) navigable = true;
      break;
    case NAV_DOWN:
      if (row < PAGE_ROWS - 1) if (navigationGrid[newRow = row+1][col] != 0) if (currentPage()->navigable & NAV_DOWN) navigable = true;
      break;
    case NAV_LEFT:
      if (col > 0) if (navigationGrid[row][newCol = col-1] != 0) if (currentPage()->navigable & NAV_LEFT) navigable = true;
      break;
    case NAV_RIGHT:
      if (col < PAGE_COLS - 1) if (navigationGrid[row][newCol = col+1] != 0) if (currentPage()->navigable & NAV_RIGHT) navigable = true;
      break;
    default: ; // Warnung unterdrücken
  }

  // In der Betaversion sind alle Seiten zugreifbar
  if (Pref::get(prefNameBeta) == ON) {
    return navigable;

  // sonst nur die, die nicht individuell als Beta gekennzeichnet sind
  } else {
    return navigable && !(navigationGrid[newRow][newCol]->isBeta());
  }
  
}

// ----------------------------------------------------------------------------------------------------
//

void Page::handlePageSwitchAndFocus(M5Btn::ButtonType button) {

  if (blocked) return;

  int oldCol = col, oldRow = row;

  // Umschalten auf Nachbarseite, sofern diese existiert (zweites if) und
  // wenn dorthin navigieren erlaubt (letztes if)

  // nach oben
  if (button == M5Btn::AB && isNavigable(NAV_UP)) row--;

  // nach unten
  else if (button == M5Btn::BC && isNavigable(NAV_DOWN)) row++;

  // nach links
  else if (button == M5Btn::CA && isNavigable(NAV_LEFT)) col--;
  
  // nach rechts
  else if (button == M5Btn::AC && isNavigable(NAV_RIGHT)) col++;

  // Falls es neue Seite gibt, alte verbergen und neue anzeigen
  if (oldCol != col || oldRow != row) {
    navigationGrid[oldRow][oldCol] -> setVisible(false, false);
    navigationGrid[row][col] -> setVisible(true, true);
  }
  
  // Fokuswechsel
  if (currentPage()->focussedWidget() != currentPage()->lastFocussed) {
    currentPage()->focusChanged();
  }
  if (button == M5Btn::B && currentPage()->getFunction(button) == FN_FOCUS) {
    
    int oldFocusIndex = -1, newFocusIndex = -1;

    for (int i=0; i<currentPage()->numWidgets; i++) 
      if (currentPage()->widgets[i]->hasFocus()) { oldFocusIndex = i; break; }

    for (int i=0; i<currentPage()->numWidgets; i++) {
      newFocusIndex = (oldFocusIndex + 1 + i) % currentPage()->numWidgets;
      if (currentPage()->widgets[newFocusIndex]->canHaveFocus()) break;
    }

    // neu zeichnen, dabei wandert Fokusdarstellung auf nächstes Widget
    if (oldFocusIndex != -1) { 
      currentPage()->widgets[oldFocusIndex]->setFocus(false); 
      currentPage()->widgets[oldFocusIndex]->setVisible(true); 
    }

    if (newFocusIndex != -1) { // falls es kein fokussiertes Widget gibt
      currentPage()->widgets[newFocusIndex]->setFocus(true);
      currentPage()->widgets[newFocusIndex]->setVisible(true);
      if (oldFocusIndex != newFocusIndex) currentPage()->focusChanged();
    }
 
  } 
}

// ----------------------------------------------------------------------------------------------------
//

void Page::setButtons(int layer) {
  this->layer = layer;
  for (int i=0; i<numSoftkeys; i++) 
    if (softkeys[i]->getLayer() != layer) softkeys[i]->setVisible(false);
  for (int i=0; i<numSoftkeys; i++) 
    if (softkeys[i]->getLayer() == layer) softkeys[i]->setVisible(true);
}


// ----------------------------------------------------------------------------------------------------
//

Widget* Page::focussedWidget() {
  for (int i=0; i<numWidgets; i++) {
    if (widgets[i]->hasFocus()) return widgets[i];
  }
  return 0;
}

void buttonPressed(M5Btn::ButtonType btn) {
    Page::handlePageSwitchAndFocus(btn);
    Page::currentPage()->buttonPressed(btn);
}

String Page::getFunction(M5Btn::ButtonType button) {
  for (int i=0; i<numSoftkeys; i++) {
    if (softkeys[i]->getButton()==button && softkeys[i]->getLayer()==layer) {
      // Serial.printf("Zu Button %d gehört in Layer %d Funktion %s\n", button, layer, (softkeys[i]->getCaption()).c_str());
      return softkeys[i]->getCaption();
    }
  }
  return "nicht gefunden";
}

Softkey* Page::getSoftkey(M5Btn::ButtonType button) {
  for (int i=0; i<numSoftkeys; i++) {
    if (softkeys[i]->getButton()==button && softkeys[i]->getLayer()==layer) {
      return softkeys[i];
    }
  }
  return 0;
}

Softkey* Page::getSoftkey(String caption) {
  for (int i=0; i<numSoftkeys; i++) {
    if (softkeys[i]->getCaption()==caption && softkeys[i]->getLayer()==layer) {
      return softkeys[i];
    }
  }
  return 0;
}


void Page::setFunction(M5Btn::ButtonType button, String caption) {
  for (int i=0; i<numSoftkeys; i++) 
    if (softkeys[i]->getButton()==button && softkeys[i]->getLayer()==layer) softkeys[i]->setCaption(caption);
}



// ----------------------------------------------------------------------------------------------------
//

void Page::trackPowerStateChanged(BoolState trackPowerState) {
  navigationHint();
  Webserver::send("Z21_TRACKPOWERSTATE", Z21::toString(Z21::getTrackPowerState()));
}

void Page::shortCircuitStateChanged(BoolState shortCircuitState) {
  navigationHint();
  Webserver::send("Z21_SHORTCUT", Z21::toString(Z21::getShortCircuitState(), "Ja", "Nein"));
}

void Page::emergencyStopStateChanged(BoolState emergencyStopState) {
  navigationHint();
  Webserver::send("Z21_EMERGENCYSTOP", Z21::toString(Z21::getEmergencyStopState()));  
}

void Page::progStateChanged(BoolState progState) {
  navigationHint();
  Webserver::send("Z21_PROGMODE", Z21::toString(Z21::getProgState(), "Aktiv", "Inaktiv"));  
}



void Page::traceEvent(FromToZ21 direction, long diffLastSentReceived, String message, String parameters) {
  Webserver::send(String(direction == toZ21 ? "&nbsp;&nbsp;&gt;" : "&lt;") + "|" + "timeDiff" + "|" + message + "|" + parameters);
}


