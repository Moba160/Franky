#include "TrackPage.h"
#include "Pref.h"
#include "configuration.h"
#include "functions.h"
#include "Route.h"

#include <Z21.h>

// ----------------------------------------------------------------------------------------------------
//

TrackPage::TrackPage(char navigable) : Page(navigable) {

  #define y TFT_H*0.67

  // Widgets

  fstName = new Textbox(tft, 0, 40, "", TFT_W/2, y, CC_DATUM, 4);
  progress = new Textbox(tft, 0, 60, "", TFT_W/2, TFT_H*0.75, CC_DATUM, 2);
  leftSymbol = new Textbox(tft, 0, 5, ">", 0, y, ML_DATUM, 4);
  rightSymbol = new Textbox(tft, 0, 5, ">", TFT_W, y, MR_DATUM, 4);

  // Softkeys Ebene 0
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_EINF_HL, M5Btn::A, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_AUSF_HL, M5Btn::AA, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_DURCHF, M5Btn::B, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_UP, M5Btn::BB, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_EINF_AHB, M5Btn::C, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_AUSF_AHB, M5Btn::CC, LAYER0, TFT_WHITE, TFT_BLUE, TFT_BLACK);

  // softkeys Ebene 1
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_SH_BOT, M5Btn::A, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_SH_TOP, M5Btn::AA, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, CAPTION_DOWN, M5Btn::B, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_SH_Q, M5Btn::C, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);
  softkeys[numSoftkeys++] = new Softkey(tft, 0, FN_SH_S, M5Btn::CC, LAYER1, TFT_WHITE, TFT_BLUE, TFT_BLACK);

}


void TrackPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  setButtons(LAYER0);

  fstName->setVisible(visible);
  progress->setVisible(visible);
  leftSymbol->setVisible(visible);
  rightSymbol->setVisible(visible);

  if (visible) {
    matchFirstRoute();
    navigationHint();
  }

}

// ----------------------------------------------------------------------------------------------------
//

void TrackPage::matchFirstRoute() {
    selectedRoute = 0;
    while (!Route::usability(selectedRoute, routeFilter)) {
        selectedRoute++; // Bedingung, damit nicht endlos: es existiert auch so eine Fahrstraße!
    }
    // Jetzt ist erste matchende Route ermittelt und ...
    drawTracks(); // ... wird angezeigt
}

// ----------------------------------------------------------------------------------------------------
//

void TrackPage::drawTracks() {

  Route::drawAll(tft, progress);
  
  Route::getById(selectedRoute) -> draw(SELECTED_ROUTE_COLOR, tft, progress);

  // tft->setTextSize(1);
  tft->setFont(&FreeSans12pt7b);

  Route* route = Route::getById(selectedRoute);

  // Zeile löschen
  fstName -> setValue("                                                                  ")->setVisible(true);

  if (route -> touchesLeftStationSide()) {
    leftSymbol->setValue(route -> shortName)->setVisible(true);
  }
  // Fahrstraßen auf der rechten Bf-Seite
  if (route -> touchesRightStationSide()) {
    rightSymbol->setValue(route -> shortName)->setVisible(true);
  }

  // Aufforderung, die Fahrstraße zu stellen
  fstName->setValue(route -> name)->setVisible(true);

}

// ----------------------------------------------------------------------------------------------------
// 

void TrackPage::buttonPressed(M5Btn::ButtonType button) {

  if (button == M5Btn::RotaryLeft || button == M5Btn::RotaryRight) {
    
    if (button == M5Btn::RotaryLeft) selectedRoute--;
    if (button == M5Btn::RotaryRight) selectedRoute++;
    selectedRoute = (selectedRoute + Route::getNumRoutes()) % Route::getNumRoutes();

    // "Weiterdrehen", bis passende Fst gefunden. Die bereits ermittelte nächste muss ja nicht passen
    while (!Route::usability(selectedRoute, routeFilter)) {
      if (button == M5Btn::RotaryLeft) selectedRoute--;
      if (button == M5Btn::RotaryRight) selectedRoute++;
      selectedRoute = (selectedRoute + Route::getNumRoutes()) % Route::getNumRoutes();
    }
   
    drawTracks();

  } else if (button == M5Btn::RotaryKnob) {
    Route::getById(selectedRoute)->set(tft, progress);
  } else if (getFunction(button) == FN_DURCHF) {
    routeFilter = RT_THROUGH; matchFirstRoute();

  } else if (getFunction(button) == FN_EINF_HL) {
    routeFilter = RT_ELEFT; matchFirstRoute();

  } else if (getFunction(button) == FN_EINF_AHB) {
    routeFilter = RT_ERIGHT; matchFirstRoute();

  } else if (getFunction(button) == FN_AUSF_HL) {
    routeFilter = RT_ALEFT; matchFirstRoute();

  } else if (getFunction(button) == FN_AUSF_AHB) {
    routeFilter = RT_ARIGHT; matchFirstRoute();

  } else if (getFunction(button) == CAPTION_UP) {
    setButtons(LAYER1);

  } else if (getFunction(button) == CAPTION_DOWN) {
    setButtons(LAYER0);

  } else if (getFunction(button) == FN_SH_TOP) {
    routeFilter = RT_SH_TOP; matchFirstRoute();

  } else if (getFunction(button) == FN_SH_Q) {
    routeFilter = RT_SH_Q; matchFirstRoute();

  } else if (getFunction(button) == FN_SH_S) {
    routeFilter = RT_SH_S; matchFirstRoute();

  } else if (getFunction(button) == FN_SH_BOT) {
    routeFilter = RT_SH_BOT; matchFirstRoute();

  }

}

