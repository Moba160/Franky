#include "Node.h"
#include "configuration.h"
#include <Z21.h>

Node* Node::node[MAX_ACCESSORIES];

// ----------------------------------------------------------------------------------------------------
//

Node::Node(int addr, int x, int y) : addr{addr}, x{x}, y{y} {

  boolean created = false;

  for (int i = 0; i < MAX_ACCESSORIES; i++) {
    if (node[i] == 0) {
      created = true;
      node[i] = this;
      break;
    }
  }
  if (!created) {
    // Kein freien Platz gefunden? Letztes Zubehör überschreiben
    Serial.println("Warnung: nicht genügend Speicherplätze für Zubehör, letztes wird immer überschrieben!.");
    delete node[MAX_ACCESSORIES - 1];
    node[MAX_ACCESSORIES - 1] = this;
  }
}

Node::Node(String name, int x, int y) : name{name}, x{x}, y{y} {

  boolean created = false;

  for (int i = 0; i < MAX_ACCESSORIES; i++) {
    if (node[i] == 0) {
      created = true;
      node[i] = this;
      break;
    }
  }
  if (!created) {
    // Kein freien Platz gefunden? Letztes Zubehör überschreiben
    Serial.println("Warnung: nicht genügend Speicherplätze für Zubehör, letztes wird immer überschrieben!.");
    delete node[MAX_ACCESSORIES - 1];
    node[MAX_ACCESSORIES - 1] = this;
  }
}

// ----------------------------------------------------------------------------------------------------
//

Node* Node::getByAddr(int addr) {
  for (int i = 0; i < MAX_ACCESSORIES; i++) {
    if (node[i] != 0) {
      if (node[i] -> addr == addr) {
        return node[i];
      }
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------------------------------
//

Node* Node::getByName(String name) {
  for (int i = 0; i < MAX_ACCESSORIES; i++) {
    if (node[i] != 0) {
      if (node[i] -> name == name) {
        return node[i];
      }
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------------------------------
//

int Node::getX() { return x; }
int Node::getY() { return y; }


// ----------------------------------------------------------------------------------------------------
//

bool Node::isNode() {
  return true;
}
bool Node::isSignal() {
  return false;
}
bool Node::isSwitch() {
  return false;
}

// ----------------------------------------------------------------------------------------------------
//

void Node::dump() {
  Serial.printf("%s ", name.c_str());
}
