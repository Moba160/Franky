struct HtmlPage {
   String anchor;   // Darstellung im Menü. Wenn "", dann wird der Link nicht ins Menü aufgenommen
   String url;      // ohne führendes "/"
   String mimeType; // für "text/html" wird auch ein html-File in der url erwartet
};

#define numPages 9
#define LastPageMarker "last"

HtmlPage page[numPages] = {
  {"System", "system.html", "text/html"},
  {"Z21", "z21.html", "text/html"},
  {"Einstellungen", "config.html", "text/html" },
  // {"X-Bus", "sniffer.html", "text/html" },
  // {"OTA-Update", "ota", "text/html" },
  {"&Uuml;ber", "about.html", "text/html"},
  {"", "w3.css", "text/css"},
  {"", "websocket.js", "text/javascript"},
  {LastPageMarker, "", ""},
  
};