#include "configuration.h"

#include "Route.h"
#include "Node.h"
#include "Switch.h"
#include "Signal.h"
#include <Z21.h>

extern int split(String s, char listSeparator, String elements[]);
Route* Route::routes[MAX_ROUTES];
int Route::numRoutes = 0;

// ----------------------------------------------------------------------------------------------------
//
Route::Route(String name, String shortName, int kind, String path, String flank) : name{name}, shortName{shortName}, kind{kind} {

  const int maxElements = 20;

  String elementList[maxElements];
  int numElems = split(path, ',', elementList);

  // Fahrweg extrahieren

  runningPath = new RouteElement[numElems + 1];
  Node* node;
  Node::State state;

  for (int i = 0; i < numElems; i++) {

    boolean nonSwitchable = elementList[i].indexOf('+') == -1 && elementList[i].indexOf('-') == -1;
    if (nonSwitchable) {
      node = Node::getByName(elementList[i]);
    } else {
      String num = "";
      for (int c = 0; c < elementList[i].length(); c++) {
        if (isDigit(elementList[i][c])) num += elementList[i][c];
      }
      node = Node::getByAddr(num.toInt());
      state = elementList[i].endsWith("+");
    }
    if (node == 0) {
      Serial.printf("Knoten %s in Fahrstraße '%s' benutzt, aber nicht deklariert.\n", elementList[i].c_str(), name.c_str());
    }
    runningPath[i].node = node;
    runningPath[i].targetState = state;
  }
  runningPath[numElems].node = 0; // Endemarkierung

  // Flankenschutz extrahieren

  numElems = split(flank, ',', elementList);

  flankProtection = new RouteElement[numElems + 1];

  for (int i = 0; i < numElems; i++) {

    String num = "";
    for (int c = 0; c < elementList[i].length(); c++) {
      if (isDigit(elementList[i][c])) num += elementList[i][c];
    }
    node = Node::getByAddr(num.toInt());
    state = elementList[i].endsWith("+");

    if (node == 0) {
      Serial.printf("Knoten %s im Flankenschutz zu Fahrstraße '%s' benutzt, aber nicht deklariert.\n", elementList[i].c_str(), name.c_str());
    }
    flankProtection[i].node = node;
    flankProtection[i].targetState = state;
  }
  flankProtection[numElems].node = 0; // Endemarkierung


  if (numRoutes < MAX_ROUTES) {
    routes[numRoutes++] = this;
  } else {
    Serial.printf("Zu viele (%d) Fahrstraßen, max. %d möglich\n", numRoutes, MAX_ROUTES);
  }

  // dump();

}

// ----------------------------------------------------------------------------------------------------
//

void Route::begin() {
#include "Topology.h"
}

// ----------------------------------------------------------------------------------------------------
//

int Route::getNumRoutes() {
  return numRoutes;
}


// ----------------------------------------------------------------------------------------------------
//

Route* Route::getById(int i) {
  if (i >= 0 && i < MAX_ROUTES) return routes[i];
  return 0;
}


// ----------------------------------------------------------------------------------------------------
//

boolean Route::touchesLeftStationSide() {
  return (kind & RT_ELEFT) > 0 || (kind & RT_ALEFT) > 0 || (kind & RT_THROUGH) > 0;
}

// ----------------------------------------------------------------------------------------------------
//
boolean Route::touchesRightStationSide() {
  return (kind & RT_ERIGHT) > 0 || (kind & RT_ARIGHT) > 0 || (kind & RT_THROUGH) > 0;
}

// ----------------------------------------------------------------------------------------------------
//

boolean Route::usability(int i, int flag) {
  return (flag & routes[i] -> kind) > 0;
}


// ----------------------------------------------------------------------------------------------------
//

void Route::set(TFT_eSPI* tft, Textbox* box) {
  draw(SET_ROUTE_COLOR, tft, box);
}

// ----------------------------------------------------------------------------------------------------
//

void Route::draw(int color, TFT_eSPI* tft, Textbox* box) {

  Node* node1; Node* node2;
  Node::State targetState;

  boolean routeSettingMode = color == SET_ROUTE_COLOR;

  for (int run = 0; run < 2; run++) { // erster Durchlauf (0): Fahrweg, zweiter (1): Flankenschutz
    int i = 0;

    while ((run == 0 ? runningPath[i].node : flankProtection[i].node) != 0) {
      if (i > 0) {
        node1 = run == 0 ? runningPath[i - 1].node : flankProtection[i - 1].node;
        node2 = run == 0 ? runningPath[i].node : flankProtection[i].node;
        targetState = run == 0 ? runningPath[i - 1].targetState : flankProtection[i - 1].targetState;

        if (routeSettingMode && node1 -> isSwitch()) {
          box->setValue("                                      ")->setVisible(true);
          box->setValue((run == 0 ? "W" : "Flankenschutz W") + String(node1 -> addr) + (targetState ? "+" : "-"))->setVisible(true);
          Z21::LAN_X_SET_TURNOUT(node1 -> addr, targetState);
          delay(SWITCH_DELAY);
        }
        if (run == 0) tft->drawLine(node1 -> getX(), node1 -> getY(), node2 -> getX(), node2 -> getY(), color);
      }
      i++;
    }
  }

  if (routeSettingMode) {

    Node* node;

    // Signale am Schluss stellen

    int i = 0;
    while (runningPath[i].node != 0) {
      node = runningPath[i].node;
      if (node -> isSignal()) {
        box->setValue("                                      ")->setVisible(true);
        box->setValue("S" + String(node -> addr) + (runningPath[i].targetState ? " Halt" : " Fahrt"))->setVisible(true);
        Z21::LAN_X_SET_TURNOUT(node -> addr, runningPath[i].targetState);
        delay(SWITCH_DELAY);
      }
      i++;
    }
    box->setValue("                                      ")->setVisible(true);
  }
}

// ----------------------------------------------------------------------------------------------------
//

void Route::drawAll(TFT_eSPI* tft, Textbox* box) {
  for (int i = 0; i < MAX_ROUTES; i++) {
    if (routes[i] != 0) routes[i] -> draw(TRACK_COLOR, tft, box);
  }
}

// ----------------------------------------------------------------------------------------------------
//
void Route::dump() {

  // Irreführende Ausgabe: da der targetState fst-bezogen ist und nicht Teil des Elements, wird
  // beim Dump der aktuelle Elementzustand, NICHT der Zielzustand in der Fst ausgegeben, sollte für
  // Testzwecke trotzdem OK sein

  Serial.printf("Fahrstraße %s (%s): \n", name.c_str(), shortName.c_str());

  Serial.print(" Fahrweg:\n\t");
  int i = 0;
  while (runningPath[i].node != 0) {
    runningPath[i].node -> dump(); Serial.print(" ");
    i++;
  }
  Serial.println();

  Serial.print(" Flankenschutz:\n\t");
  i = 0;
  while (flankProtection[i].node != 0) {
    flankProtection[i].node -> dump(); Serial.print(" ");
    i++;
  }
  Serial.println();

}
