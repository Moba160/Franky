#include "Loco.h"
#include "configuration.h"
#include <Z21Types.h>
#include "Page.h"

Loco* Loco::loco[MAX_LOCOS];

// ----------------------------------------------------------------------------------------------------
//

Loco::Loco(int itsAddr) {
  addr = itsAddr;
  for (int i=0; i<MaxFct+1; i++) fct[i] = false;
}

// ----------------------------------------------------------------------------------------------------
//

char Loco::setTargetFst(int value) {
  targetFst = value;
  targetFst = max(0, min(targetFst, MaxFst));
  return targetFst;
}

// ----------------------------------------------------------------------------------------------------
//

char Loco::increaseAcceleration(int8_t delta) {
  acc += delta;
  acc = acc % 10;
  acc = max(0, min(acc, 9));
  return acc;
}

// ----------------------------------------------------------------------------------------------------
//

char Loco::increaseDeceleration(int8_t delta) {
  dec += delta;
  dec = dec % 10;
  dec = max(0, min(dec, 9));
  return dec;
}

// ----------------------------------------------------------------------------------------------------
//

void Loco::increaseAddress(int8_t delta) {
  addr += delta;
  if (addr < 1) addr = 1;
  if (addr > 9999) addr = 9999;
}

// ----------------------------------------------------------------------------------------------------
//

Loco* Loco::addLoco(int addr) {
  
  Loco* newLoco = existsLoco(addr);
  if (newLoco != 0) return newLoco;
  
  newLoco = new Loco(addr);
  for (int i=0; i<MAX_LOCOS; i++) {
    if (loco[i] == 0) {
      loco[i] = newLoco;
      return newLoco;
    }
  }
  // Kein freien Platz gefunden? Letzte Lok überschreiben
  delete loco[MAX_LOCOS-1];
  loco[MAX_LOCOS-1] = newLoco;
  return newLoco;
}

// ----------------------------------------------------------------------------------------------------
//

Loco* Loco::existsLoco(int addr) {
  for (int i=0; i<MAX_LOCOS; i++) {
    if (loco[i] != 0) {
      if (loco[i] -> addr == addr) {
        //dumpLocos();
        return loco[i];
      }
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------------------------------
//

void Loco::dumpLocos() {
  for (int i=0; i<MAX_LOCOS; i++) {
    if (loco[i] != 0) Serial.printf("%d: Adresse %d Geschw %d\n", i, loco[i]->addr, loco[i]->targetFst);
  }
}

Loco* Loco::getLoco(int addr) {
  //dumpLocos();
  for (int i=0; i<MAX_LOCOS; i++) {
    if (loco[i] -> addr == addr) return loco[i];
  }
  return 0;
}

// ----------------------------------------------------------------------------------------------------
//

bool Loco::isAccelerating() {
  return fst < targetFst;
}

// ----------------------------------------------------------------------------------------------------
//

bool Loco::isDecelerating() {
  return fst > targetFst;
}

// ----------------------------------------------------------------------------------------------------
//

bool Loco::isCoasting() {
  return fst == targetFst;
}

// ----------------------------------------------------------------------------------------------------
//

void Loco::drive () {

  for (int l = 0; l < MAX_LOCOS; l++) {

    if (loco[l] == 0) continue;

    if (loco[l] -> fst != loco[l] -> targetFst) {

      // Ausgeschaltete (0) Brems-/Beschleunigungsrate -> sofort Fst ändern
      if ((loco[l] -> isAccelerating() && loco[l] -> acc == 0) ||
          (loco[l] -> isDecelerating() && loco[l] -> dec == 0)) {
        loco[l] -> speed = loco[l] -> fst = loco[l] -> targetFst;
        // hier Callback zur Anzeigeaktualisierung
        continue;
      }

      if (loco[l] -> isAccelerating()) {
        loco[l] -> speed += 5.0/(loco[l] -> acc);
        if (loco[l] -> speed > MaxFst) loco[l] -> speed = MaxFst;
      }
      
      if (loco[l] -> isDecelerating()) {
        loco[l] -> speed -= 5.0/(loco[l] -> dec);
        if (loco[l] -> speed < 0) loco[l] -> speed = 0;
      }
           
      loco[l] -> fst = loco[l] -> speed;

      Z21::LAN_X_SET_LOCO_DRIVE(loco[l] -> addr, 
        loco[l] -> forward ? Forward : Backward, 
        loco[l] -> fst);

      // Update Info Anzeige 
      Page::currentPage() -> locoWasDriven(loco[l]->addr);
      // if (loco[l] -> fst == loco[l] -> targetFst) M5.Speaker.tone(200, 5);

      
    }

  }

  return;
}

// ----------------------------------------------------------------------------------------------------
//

void Loco::refresh () {

  // Z21::LAN_X_GET_LOCO_INFO(Interaction::getCurrentLoco()->addr);
  // return REFRESH_CYCLE;

}
