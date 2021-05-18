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

  fstName = new Textbox(tft, 0, 40, "", TFT_W/2, y, CC_DATUM, 4);
  progress = new Textbox(tft, 0, 60, "", TFT_W/2, TFT_H*0.75, CC_DATUM, 2);
  leftSymbol = new Textbox(tft, 0, 5, ">", 0, y, ML_DATUM, 4);
  rightSymbol = new Textbox(tft, 0, 5, ">", TFT_W, y, MR_DATUM, 4);

  
  
  // if (routeGroup == 1) {
  //   M5Btn::setFunction(M5Btn::A, FN_EINF_HL);
  //   M5Btn::setFunction(M5Btn::AA, FN_AUSF_HL);
  //   M5Btn::setFunction(M5Btn::B, FN_DURCHF);
  //   M5Btn::setFunction(M5Btn::BB, FN_FILT2);
  //   M5Btn::setFunction(M5Btn::C, FN_EINF_AHB);
  //   M5Btn::setFunction(M5Btn::CC, FN_AUSF_AHB); 
  // } else {
  //   M5Btn::setFunction(M5Btn::A, FN_SH_BOT);
  //   M5Btn::setFunction(M5Btn::AA, FN_SH_TOP);
  //   M5Btn::setFunction(M5Btn::B, FN_FILT1);
  //   M5Btn::setFunction(M5Btn::C, FN_SH_Q);
  // }
}


void TrackPage::setVisible(bool visible, bool clearScreen) {
  
  Page::setVisible(visible, clearScreen);

  setButtons(1);

  fstName->setVisible(visible);
  progress->setVisible(visible);
  leftSymbol->setVisible(visible);
  rightSymbol->setVisible(visible);

  if (visible) {
    matchFirstRoute();
  }

  if (visible) navigationHint();
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

    // "Weiterdrehen", bis passende Fst gefunden. Die bereits ermittelte nächste muss ja nicht passen
    while (!Route::usability(selectedRoute, routeFilter)) {
      if (button == M5Btn::RotaryLeft) selectedRoute--;
      if (button == M5Btn::RotaryRight) selectedRoute++;
      selectedRoute = (selectedRoute + Route::getNumRoutes()) % Route::getNumRoutes();
    }
   
    drawTracks();

  } else if (button == M5Btn::RotaryKnob) {
    Route::getById(selectedRoute)->set(tft, progress);
    fstName->setValue("Stelle " + Route::getById(selectedRoute)->name + "...");

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

  } else if (getFunction(button) == FN_FILT1) {
    routeGroup = 1;
    setButtons(0);

  } else if (getFunction(button) == FN_FILT2) {
    routeGroup = 2;
    setButtons(1);

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

