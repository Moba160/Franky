#include "Loco.h"
#include "configuration.h"
#include <Z21Types.h>
#include "SPIFFS.h"
#include "Page.h"

Loco* Loco::loco[MAX_LOCOS];
int Loco::minAddress = MaxLocoAddr;
int Loco::maxAddress = 1;

int split(String s, char listSeparator, String elements[]);

// ----------------------------------------------------------------------------------------------------
//

#define idxAddr 0
#define idxName 1
#define idxDecoder 2
#define idxAcc 3
#define idxDec 4
#define idxNumFunc 5
#define idxFunc 6
#define MAX_ELEM idxFunc + 1

#define SEPARATOR ';' // Trennzeichen Spalten Lokbeschreibung
#define LIST_SEP '|' // Trennzeichen Listein innerhalb Spalten
#define FMAP_SEP '=' // Trennzeichen Funktionsmapping
#define SHLONG_SEP '/' // Trennzeichen Short/Longname

static String elems[MAX_ELEM];
static String funcs[MaxFct+1];
static String funcSpec[4]; // z.B. 1=2=BrsQ/Bremsenquietschen
static String names[2]; // Kurz/Langname

void Loco::begin() {
  String line;
  int numElems;
  File file = SPIFFS.open(LOCO_FILE_NAME, FILE_READ);
  int numLocos=0;
  int headlineRead = false;

  for (int i=0; i<MAX_LOCOS; i++) loco[i] = 0;
  
  if(!file){
    Serial.printf("Kann Lokdatendatei %s nicht lesen.\n", LOCO_FILE_NAME);
    return;
  }
  
  while(file.available()) {
    
    // Lokbeschreibung ab 2. Zeile
    line = file.readStringUntil('\n');
    if (!headlineRead) {
      headlineRead = true;
      continue;
    }

    // line.trim();
    // if (line == "") continue; // Leerzeilen überlesen
    // if (line.charAt(0) == '#') continue; // Kommentarzeilen überlesen

    numLocos++;

    // Spalten
    numElems = split(line, SEPARATOR, elems);
    if (numElems > 0) {

      for (int i=0; i<numElems; i++) elems[i].trim();

      int addr = elems[idxAddr].toInt();
      if (addr < minAddress) minAddress = addr;
      if (addr > maxAddress) maxAddress = addr;

      // Neue Lok anlegen und ihre Attribute beschreiben
      Loco* loco = addLoco(addr);

      // Lokname
      if (numElems > idxName) loco->name = elems[idxName];

      // verbauter Decoder
      if (numElems > idxDecoder) loco->decoder = elems[idxDecoder];

      // Anfahr- und Bremsverzögerung
      if (numElems > idxAcc) if (elems[idxAcc] != "") loco->acc = elems[idxAcc].toInt(); 
      if (numElems > idxDec) if (elems[idxDec] != "") loco->dec = elems[idxDec].toInt();

      // Anzahl Funktionen
      if (numElems > idxNumFunc) if (elems[idxNumFunc] != "")  loco->setNumFct(elems[idxNumFunc].toInt());

      // Lokfunktionsspezifikation (Funktionsmapping, Kurz-/Langname)
      if (numElems > idxFunc) { 
        if (elems[idxFunc] != "") {
          loco->fctSpec = elems[idxFunc];
          int numF = split(elems[idxFunc], LIST_SEP, funcs); // funcs[] enthält Funktionsspecs wie z.B. 1=2=kurz/lang (F1 mappen auf F2, Kurzname kurz, Langname lang)
          for (int i=0; i<numF; i++) {
            int numSpec = split(funcs[i], FMAP_SEP, funcSpec); // funcSpec enthält eine Spec wie oben "wie z.B."
            int fctIndex = funcSpec[0].toInt(); // Index der Funktion
            int mappedTo = funcSpec[numSpec==2 ? 0 : 1].toInt(); // wird ggf. gemappt auf andere Funktion
            int numN = split(funcSpec[numSpec==2 ? 1 : 2], SHLONG_SEP, names);

            // diese so ermittelte Funktionsspezifikation überschreibt nun die Standardspezifikation (siehe Konstruktor von Locofunction)
            loco->fct[fctIndex]->setMappedTo(mappedTo);
            if (numN > 0) loco->fct[fctIndex]->setShortName(names[0].c_str());
            // if (numN > 1) loco->fct[fctIndex]->setLongName(names[1]); else loco->fct[fctIndex]->setLongName(names[0]);
            // loco->fct[fctIndex]->dump();
          }
        }
      }
    }
  }
  
  file.close();

  Serial.printf("Daten von %d Loks aus Datei %s gelesen.\n", numLocos, LOCO_FILE_NAME);
  if (numLocos > MAX_LOCOS) Serial.printf("Gespeichert werden konnten nur %d Loks\n", MAX_LOCOS);
}

// ----------------------------------------------------------------------------------------------------
//

Loco::Loco(int itsAddr) {
  addr = itsAddr;
  for (int i=0; i<MaxFct+1; i++) {
    fct[i] = new Locofunction(i, i, "", "");
  }
}

// ----------------------------------------------------------------------------------------------------
//

Loco::~Loco() {
  for (int i=0; i<MaxFct+1; i++) delete fct[i];
}

// ----------------------------------------------------------------------------------------------------
//

Locofunction* Loco::getFctByName(String shortName) {
  for (int i=0; i<MaxFct+1; i++) {
    if (fct[i]->getShortName() == shortName) return fct[i];
  }
  return 0;
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
// Wenn freier Platz da ist, wird ab Index 1 gespeichert. Anderenfalls immer fest auf Index 0.
// Das ist ein reservierter Platz, auf dem sich alle anonymen Loks befinden können, da die zunächst
// aus LOCO_FILE_NAME eingelesenen, bekannten Loks ja ab Index 1 stehen

Loco* Loco::addLoco(int addr) {
  
  Loco* newLoco = existsLoco(addr);
  if (newLoco != 0) return newLoco;
  
  newLoco = new Loco(addr);
  for (int i=1; i<MAX_LOCOS; i++) {
    if (loco[i] == 0) {
      loco[i] = newLoco;
      return newLoco;
    }
  }
  // Kein freien Platz gefunden? Erste Lok überschreiben
  if (loco[0] != 0) delete loco[0];
  loco[0] = newLoco;
  return newLoco;
}

// ----------------------------------------------------------------------------------------------------
//

Loco* Loco::existsLoco(int addr) {
  for (int i=0; i<MAX_LOCOS; i++) {
    if (loco[i] != 0) {
      if (loco[i] -> addr == addr) {
        return loco[i];
      }
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------------------------------
//

void Loco::dumpLocos(bool all) {
  if (all) {
    for (int i=0; i<MAX_LOCOS; i++) {
      if (loco[i] != 0) Serial.printf("%d: Adresse %d Geschw %d Anf/Brs %d/%d\n", i, loco[i]->addr, loco[i]->targetFst, loco[i]->acc, loco[i]->dec);
    }
  } else {
    int numLocos = 0;
    for (int i=0; i<MAX_LOCOS; i++) {
      if (loco[i] != 0) numLocos++;
    }
    Serial.printf("=== %d/%d Loks ===\n", numLocos, MAX_LOCOS);
  }
}

Loco* Loco::getLoco(int addr) {
  for (int i=0; i<MAX_LOCOS; i++) {
    if (loco[i] != 0) if (loco[i] -> addr == addr) return loco[i];
  }
  Serial.printf("======== Error: getLoco(%d) returns 0\n", addr);
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
        // Page::currentPage() -> locoWasDriven(loco[l]->addr);
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
