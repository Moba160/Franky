#ifndef LOCOFUNCTION_H
#define LOCOFUNCTION_H

#include <Arduino.h>

#define SHORT_NAME_LEN 10

class Locofunction {
    public:

        Locofunction(int index, int mappedTo, char* shortName, char* longName);

        int getIndex() { return index; }
        int getMappedTo() { return mappedTo; }
        void setMappedTo(int mappedTo) { this->mappedTo = mappedTo; }

        bool isActive() { return active; }
        void setActive(bool active) { this->active = active; }

        String getShortName() { return shortName; }
        void setShortName(const char* shortName);
        // String getLongName() { return longName; }
        // void setLongName(String longName) { this->longName = longName; }

        void dump();

    private:

        byte index, mappedTo;
        char shortName[SHORT_NAME_LEN]; // char[] statt String wegen Speicherbedarf, gerade bei vielen Loks x viele Funktionen!
        // String longName;
        bool active = false;

};

#endif