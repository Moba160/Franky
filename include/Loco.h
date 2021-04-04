#ifndef LOCO_H
#define LOCO_H

#include <Z21.h>
#include "configuration.h"

#define MaxFst 126

class Loco {

  public:

    static int currLoco;
    int addr = 3;
    int acc = 5;
    int dec = 3;
    int fst = 0;
    int targetFst = 0;
    float speed = fst;

    boolean forward = true;
    boolean takenOver = false;
    
    boolean fct[MaxFct+1];
    int lastFunction = 5;

    static Loco* loco[MAX_LOCOS];

    static Loco* addLoco(int addr);

    static Loco* getLoco(int addr);
    static Loco* existsLoco(int addr);
    static void setLoco();

    char setTargetFst(int value);
    char increaseAcceleration(int8_t delta);
    char increaseDeceleration(int8_t delta);

    void increaseAddress(int8_t delta);

    bool isAccelerating();
    bool isDecelerating();
    bool isCoasting();

    // Regelmäßig von Scheduler zu rufende Loksteuerung (bremst/beschleunigt alle Loks)
    static void drive();

    // Regelmäßig von Scheduler zu rufende Auffrischen der Lokinfo, damit Notifikationen noch ankommen
    static void refresh();  

  private:
    Loco(int itsAddr);
    static void dumpLocos();
};

#endif
