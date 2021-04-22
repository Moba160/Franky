#ifndef NODE_H
#define NODE_H

#include "Arduino.h"
#include "Configuration.h"

/*
  Gleisbildobjekte:
    - Knotenpunkt mit Lage x,y
    - Weichen, Signale (davon abzuleiten, haben ebenfalls Lage, aber zusätzlich Schaltzustände)
*/

class Node {

  public:

    typedef boolean State;

    Node(int addr, int x, int y);
    Node(String name, int x, int y);
    int getX();
    int getY();

    int addr=0; // muss noch protected werden

    virtual bool isNode();
    virtual bool isSwitch();
    virtual bool isSignal();

    // Node über Adresseoder Name zurückliefern, 0 wenn nicht gefunden
    static Node* getByAddr(int addr);
    static Node* getByName(String name);

    virtual void dump();

  private:
    String name = "";

  protected:
    // Liste aller  bekannten Zubehöre (Weichen, Signale)
    static Node* node[MAX_ACCESSORIES];

    int x = 0;
    int y = 0;

    boolean state = false;

};

#endif
