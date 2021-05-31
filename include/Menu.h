struct HtmlPage {
   String anchor;   // Darstellung im Menü. Wenn "", dann wird der Link nicht ins Menü aufgenommen
   String url;      // ohne führendes "/"
   String mimeType; // für "text/html" wird auch ein html-File in der url erwartet
};

#define numPages 14
#define LastPageMarker "last"

HtmlPage page[numPages] = {
  {"System", "system.html", "text/html"},
  {"Z21", "z21.html", "text/html"},
  {"Einstellungen", "config.html", "text/html" },
  {"Lokbibliothek", "locoData.html", "text/html" },
  {"X-Bus", "sniffer.html", "text/html" },
  {"&Uuml;ber", "about.html", "text/html"},
  {"", "w3.css", "text/css"},
  {"", "websocket.js", "text/javascript"},
  {"", "locoData.csv", "text/plain"},
  {"", "N025.xml", "text/xml"},
  {"", "DCX76.xml", "text/xml"},
  {"", "decoder.xsl", "text/xsl"},  
  {"", "decoder.js", "text/javascript"},  
  {LastPageMarker, "", ""},
};

  // {"OTA-Update", "ota", "text/html" },