/*

  Topologiedaten für Fahrstraßenseite

  Hier ist die Stelle zum Anpassen an ein eigenes Gleisbild

*/

// ----------------------------------------------------------------------------------------------------

#define TRACK_DISTANCE 13

// Y-Koordinate des ersten Gleises

#define Y_OFFS 5

// Die einzelnen Gleislinien

#define  Y1 Y_OFFS +  1 * TRACK_DISTANCE
#define  Y2 Y_OFFS +  2 * TRACK_DISTANCE
#define  Y3 Y_OFFS +  3 * TRACK_DISTANCE
#define  Y4 Y_OFFS +  4 * TRACK_DISTANCE
#define  Y5 Y_OFFS +  5 * TRACK_DISTANCE
#define  Y6 Y_OFFS +  6 * TRACK_DISTANCE
#define  Y7 Y_OFFS +  7 * TRACK_DISTANCE
#define  Y8 Y_OFFS +  8 * TRACK_DISTANCE
#define  Y9 Y_OFFS +  9 * TRACK_DISTANCE
#define Y10 Y_OFFS + 10 * TRACK_DISTANCE

// Knoten im Gleisbild

new Node("B1",   41, Y1); // Bogen Gleis 1 links
new Node("V1",   50, Y1); // virtuelles Zielsignal Gleis 1 links
new Signal(13, 103, Y1);
new Switch(25, 118, Y1); 
new Switch(31, 144, Y1);
new Node("B2",  178, Y1); // Bogen Gleis 1 rechts

new Switch(27, 127, Y2);
new Switch(29, 135, Y2);
new Switch(30, 135, Y2);
new Node("P12", 174, Y2); // Prellbock Gleis 12 
new Switch(47, 187, Y2); 
new Signal(72, 213, Y2);
new Switch(65, 292, Y2);
new Signal(99, 298, Y2);
new Node("Lak", 320, Y2); // Prellbock Schattenbahnhof S-Bahn (Lankwitz Lak)

new Node("P1",    8, Y3); // Prellbock Gleis 1
new Switch(2,   24, Y3); 
new Node("V2",  50, Y3); // virtuelles Zielsignal Gleis 2 links
new Signal(14, 103, Y3); 
new Switch(26, 126, Y3);
new Switch(28, 135, Y3);
new Switch(48, 195, Y3);
new Switch(49, 214, Y3);
new Node("B3",  283, Y3); // Bogen Schattenbahnhof S-Bahn

new Node("nHal",  0, Y4); // Bereichsgrenze Ausfahrt nach Hl
new Switch(1,  17, Y4);    
new Signal(8,  50, Y4);
new Signal(50, 223, Y4); 
new Signal(51, 251, Y4);
new Switch(60, 288, Y4);
new Signal(73, 298, Y4);
new Node("vAhb", 320, Y4); // Bereichsgrenze Einfahrt aus Ahb

new Node("vHal",  0, Y5); // Bereichsgrenze Einfahrt aus Hl 
new Switch(3, 26, Y5);  
new Switch(4, 26, Y5);
new Signal(36, 186, Y5);
new Switch(52, 260, Y5);
new Switch(58, 280, Y5);
new Switch(59, 280, Y5);
new Node("nAhb", 320, Y5); // Bereichsgrenze Ausfahrt nach Ahb

new Node("AusziehL", 10, Y6); // Prellbock Gleis 5a
new Switch(5,  35, Y6);
new Switch(6,  35, Y6);
new Signal(10, 78, Y6);
new Signal(37, 147, Y6);
new Switch(56, 259, Y6);
new Switch(57, 271, Y6);
new Node("AusziehR", 311, Y6); // Prellbock Gleis 5b 

new Switch(7,  44, Y7);
new Switch(9,  62, Y7);
new Signal(12,  78, Y7);
new Signal(38, 147, Y7);
new Switch(41, 173, Y7);
new Switch(43, 182, Y7);
new Switch(55, 250, Y7);

new Switch(11,  71, Y8);
new Switch(34, 154, Y8);
new Switch(42, 173, Y8);
new Switch(44, 182, Y8);
new Switch(45, 193, Y8);
new Switch(54, 242, Y8);

new Node("X91",  79, Y9); // Bogen Gleis 8
new Switch(33, 145, Y9);
new Node("X93", 183, Y9);  
new Switch(46, 202, Y9);
new Switch(53, 233, Y9);
new Node("X96", 276, Y9); // Prellbock Hafen 

new Node("X101",  70, Y10); // Bogen Gleis 9 links
new Node("X102", 136, Y10); // Bogen Gleis 9 rechts


// Die erste angelegte Fahrstraße ist gleichzeitig die, mit der initial bei der Anzeige begonnen wird

// Durchfahrten Gl. 3/4
new Route("Durchf Hal > Ahb", ">", RT_THROUGH, "vHal,W3+,W4+,S36-,W52+,W58+,W59+,nAhb", "W1+,W5+,W6+,W51+,W57+,W60+");
new Route("Durchf Hal < Ahb", "<", RT_THROUGH, "vAhb,S73-,W60+,W50+,S8-,W1+,nHal", "W49+,W4+,W3+");
new Route("", "", 0, "W51-,W52-", ""); // Verbindung Hauptgleise

// Einfahrt von Halle und Ausfahrt nach Ahb
new Route("Einf v Hal n Gl 4", "> 4", RT_ELEFT, "vHal,W3+,W4+,S36+", "W1+,W5+,W6+");
new Route("Ausf v Gl 4 n Ahb", "4 >", RT_ARIGHT, "S36-,W52+,W58+,W59+,nAhb", "W51+,W57+,W60+");

// Einfahrt von Ahb und Ausfahrt nach Halle
new Route("Einf v Ahb n Gl 3", "3 <", RT_ERIGHT, "vAhb,S73-,W60+,W50+,S8+", "W49+");
new Route("Ausf v Gl 3 n Hal", "< 3", RT_ALEFT, "S8-,W1+,nHal", "W4+,W3+");

// Ein-/Ausfahrten von/nach Hl in/aus Überholgleise
new Route("Einf v Hal n Gl 5", ">> 5", RT_ELEFT, "vHal,W3-,W4+,W5+,W6-,S37+", "W1+");
new Route("Einf v Hal n Gl 6", ">> 6", RT_ELEFT, "vHal,W3-,W4+,W5-,W6-,W7+,W9-,S38+", "W1+");
new Route("Ausf v Gl 5 n Hal", "<< 5", RT_ALEFT, "S10-,W6-,W5+,W4-,W3-,W1-,nHal", "");
new Route("Ausf v Gl 6 n Hal", "<< 6", RT_ALEFT, "S12-,W9-,W7+,W6-,W5+,W4-,W3-,W1-,nHal", "");

// Ein-/Ausfahrten von/nach Ahb in/aus Überholgleise
new Route("Einf v Ahb n Gl 5", "5 <<", RT_ERIGHT, "vAhb,S73-,W60-,W59-,W58-,W57-,W56+,S10+", "");
new Route("Einf v Ahb n Gl 6", "6 <<", RT_ERIGHT, "vAhb,S73-,W60-,W59-,W58-,W57-,W56-,W55+,W43+,W41+,S12+", "");
new Route("Ausf v Gl 5 n Ahb", "5 >>", RT_ARIGHT, "S37-,W56+,W57-,W58+,W59-,nAhb", "W60+");
new Route("Ausf v Gl 6 n Ahb", "6 >>", RT_ARIGHT, "S38-,W41-,W43-,W55+,W56-,W57-,W58+,W59-,nAhb", "W60+");

new Route("", "", 0, "W49-,W50-", ""); // Verbindung Vorortbahn-/Hauptgleise

// Vorortbahn
new Route("Ausf v Gl 1 n Por", "1 >", RT_ARIGHT, "S13-,W25-,W27+,W28-,W48+,W49+,B3,W65-,Lak", "W26+,W28+,W50+");
new Route("Ausf v Gl 2 n Por", "2 >", RT_ARIGHT, "S14-,W26+,W28+,W48+,W49+,B3,W65-,Lak", "W25+,W31+,W47+,W50+");
new Route("Einf v Por n Gl 1", "1 <", RT_ERIGHT, "S99-,W65+,S72-,W47+,B2,W31+,W25+,V1,W2-,P1", "W48+,W28+,W26+");
new Route("Einf v Por n Gl 2", "2 <", RT_ERIGHT, "S99-,W65+,S72-,W47+,B2,W31-,W29+,W30+,W26-,V2,W2+,P1", "W48+");
new Route("Bereit-/Abstellen Gl 1", ">S1<", RT_SH_S, "V1,W25-,W27-,W29-,W30-,P12", "");
new Route("Bereit-/Abstellen Gl 2", ">S2<", RT_SH_S, "V2,W26-,W29-,W30-,P12", "");

// Rangieren, Höhe Gleis 6 bleiben

new Route("Rang Gl 5", ">5<", RT_SH_TOP, "AusziehL,W5+,W6+,W56+,W57+,AusziehR", "");
new Route("Rang Gl 6", ">6<", RT_SH_TOP, "AusziehL,W5+,W6-,W7-,W9+,W41+,W43+,W55-,W56-,W57+,AusziehR", "");
new Route("Rang Gl 7", ">7<", RT_SH_TOP, "AusziehL,W5+,W6-,W7-,W9-,W11-,W34+,W42-,W43-,W55-,W56-,W57+,AusziehR", "");
new Route("Rang Gl 8", ">8<", RT_SH_TOP, "AusziehL,W5+,W6-,W7-,W9-,W11+,X91,W33-,W34-,W42+,W43-,W55-,W56-,W57+,AusziehR", "");
new Route("Rang Gl 9", ">9<", RT_SH_TOP, "AusziehL,W5+,W6-,W7-,X101,X102,W33+,W34-,W42-,W43-,W55-,W56-,W57+,AusziehR", "");

// Rangieren, Höhe Gleis 7 bleiben

new Route("Rang Gl 6 (unten)", ">6<", RT_SH_BOT, "AusziehL,W5+,W6-,W7-,W9+,W41-,W44-,W45+,W54-,W55-,W56-,W57+,AusziehR", "");
new Route("Rang Gl 7 (unten)", ">7<", RT_SH_BOT, "AusziehL,W5+,W6-,W7-,W9-,W11-,W34+,W42+,W44+,W45+,W54-,W55+,W56-,W57+,AusziehR", "");
new Route("Rang Gl 8 (unten)", ">8<", RT_SH_BOT, "AusziehL,W5+,W6-,W7-,W9-,W11+,X91,W33-,W34-,W42+,W44+,W45+,W54-,W55+,W56-,W57+,AusziehR", "");
new Route("Rang Gl 9 (unten)", ">9<", RT_SH_BOT, "AusziehL,W5+,W6-,W7-,X101,X102,W33+,W34-,W42+,W44+,W45+,W54-,W55+,W56-,W57+,AusziehR", "");

// Querverbindungen unten am Gbf

new Route("Quer \\", "\\", RT_SH_Q, "W45-,W46-", "");
new Route("Quer /", "/", RT_SH_Q, "W53-,W54-", "");
new Route("Rang Gsch", "--", RT_SH_Q, "X93,W46+,W53-,X96", "");
