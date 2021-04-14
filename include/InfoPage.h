

#ifndef INFOPAGE_H
#define INFOPAGE_H

#include "Page.h"
#include "Textbox.h"

class InfoPage: public Page, public Z21Observer {

  public:
    InfoPage(char navigable);
    virtual void setVisible(bool visible, bool clearScreen);

    virtual void traceEvent(FromToZ21 direction, long diffLastSentReceived, String message, String parameters) override;

  private:

    String lastMessage="", lastParameters="";

    void update(String message, String parameters);

};

#endif