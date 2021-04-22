#ifndef ROUTE_H
#define ROUTE_H

#include <M5Stack.h>
#include "Node.h"

#include "Route.h"
#include "Textbox.h"

// ----------------------------------------------------------------------------------------------------
// Benutzbarkeit einer Fahrstraße
// Benötigt als eine Art Filter für die Auswahl einer Fahrstraßengruppe über Tasten;
// individuelle Fahrstraßen werden dann innerhalb der Gruppe über den Drehregler ausgewählt

#define RT_ELEFT         1
#define RT_ALEFT         2
#define RT_THROUGH       4
#define RT_ERIGHT        8
#define RT_ARIGHT       16
#define RT_SH_TOP       32
#define RT_SH_BOT       64
#define RT_SH_Q        128
#define RT_SH_S        256

#define MAX_ROUTES 50

typedef struct {
  Node* node;
  Node::State targetState;
} RouteElement;

class Route {

  public:

    static void begin();

    Route(String name, String shortName, int kind, String path, String flank);
    static Route* getById(int i);

    static int getNumRoutes();

    String name = "";
    String shortName = "";

    static boolean usability(int i, int flag);
    boolean touchesLeftStationSide();
    boolean touchesRightStationSide();

    void draw(int color, TFT_eSPI* tft, Textbox* box);
    void set(TFT_eSPI* tft, Textbox* box);
    
    static void drawAll(TFT_eSPI* tft, Textbox* box);

  private:

    static Route* routes[MAX_ROUTES];
    static int numRoutes;

    int kind = 0;

    RouteElement* runningPath;
    RouteElement* flankProtection;

    void dump();
};

#endif
