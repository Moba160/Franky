#ifndef LOCOFUNCTION_H
#define LOCOFUNCTION_H

#include <Arduino.h>

class Locofunction {
    public:

        Locofunction(int index, int mappedTo, String shortName, String longName);

        int getIndex() { return index; }
        int getMappedTo() { return mappedTo; }
        void setMappedTo(int mappedTo) { this->mappedTo = mappedTo; }

        bool isActive() { return active; }
        void setActive(bool active) { this->active = active; }

        String getShortName() { return shortName; }
        void setShortName(String shortName) { this->shortName = shortName; }
        String getLongName() { return longName; }
        void setLongName(String longName) { this->longName = longName; }

        void dump();

    private:

        int index, mappedTo;
        String shortName;
        String longName;
        bool active = false;

};

#endif