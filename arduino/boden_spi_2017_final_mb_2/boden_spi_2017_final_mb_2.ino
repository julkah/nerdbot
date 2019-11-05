

#include <t3spi.h>
/*
 * 2017_Feb_03
 *  Bodensensor Ring SPI
 *  
 *  
 *  
 *  
 *  ACHTUNG! DIES IST NICHT DAS ORIGINAL!
 *  
 *  keine Erkennung von schwarzer Linie!
 *  Nullübergang herausgefiltert!
 *  Linienerkennung eines einzelnen Sensors herausgefiltert!
 *  Zustand bei Ausschlag von Sensor 0 und Sensor 29 (vorne und hinten) eliminiert!
 *  
 *  
 *  
 *  
 *  
 *  
 */

// Diese Werte müssen evtl. noch angepasst werden, später als Variable per SPI ?
#define weiss_schwellwert     175 // 200
#define schwarz_schwellwert   50  // eigentlich 60, aber dann sieht er auch "Dreck"

#define CS10        0x0A    // SPI Cable Select auf Pin 10
T3SPI SPI_SLAVE;

int bodenled = 8;
int add0 = 2; // 8 port Multiplexer Adressleitungen
int add1 = 3;
int add2 = 4;

int sensor[] = {A0, A1, A2, A3, A6, A7, A8, A9}; // 8 Ananlogeingänge für 8 Multiplexer

// Array, in den die 64 Sensorwerte geschrieben werden
int puffer[]    =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Zuordnungstabellen für schwarz und weiß -> hier steht, auf welchem Port die LEDs liegen
int weiss[]   =  { 61,  5, 36, 60,  4, 35, 59,  3, 11, 24,  8, 40, 32, 25,  9, 33, 57, 26, 10, 34, 58, 30, 22, 38, 62, 31, 15, 39, 63, 37};  
int schwarz[] =  { 45, 29, 52, 44, 28, 51, 43, 27, 19,  0, 16, 56, 48,  1, 17, 49, 41,  2, 18, 50, 42,  6, 14, 54, 46,  7, 23, 55, 47, 53};

int weisswinkel;
int schwarzwinkel;
int weissabstand;
int schwarzabstand;
int weissflag;
int schwarzflag;
int weisstotzeit;
int schwarztotzeit; 

int weissabst_alt, weisswink_alt, weisszeit_alt;

// Speicherwerte für den SPI-Bus
byte spi_w_a;                       // Der Abstand liegt zwischen -20 und 20. Für byte addieren wir 100
byte spi_s_a;
byte spi_w_w;                       // Der Winkel liegt zwischen -180 und 180. Das ist für byte nicht geeignet. Wir nehmen 0 bis 60 und müssen im Hauptprogramm *6 -180 rechnen
byte spi_s_w;

elapsedMicros zeit;
elapsedMillis flagzeit;

byte detect (int flag)      // Hilfsfunktion, um die Flag (-100,  0..60) zu einer detect-flag (0,1) zu wandeln
{
  if (flag == -100) return(0);
      else       return(1); 
}

void spi0_isr(void)
{
  // wir haben im SPI Bus immer 16bit Pakete, also wollen wir die sinnvoll nutzen
  // Da bei einer Fehlverbindung entweder 65535 (alle Bits high) oder 0 (alle Bits low)
  // gesendet werden, setzen wir als Kontolle das Bit15 auf low und das Bit0 auf high
  // So haben wir nie 0 und nie mehr als 32767 als zu lesende Werte.
  // Es bleiben noch 14 Bit zur Datenübertragung
  // wir brauchen 7 Bit für den Abstand "a" und 6 bit für den Winkel "w" und 1 bit für detect "x"
  // Also ergibt sich folgendes Protokoll

  // Bit    15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
  //        0   x   w   w   w   w   w   w   a   a   a   a   a   a   a   1     für weiss
  //        0   x   w   w   w   w   w   w   a   a   a   a   a   a   a   1     für schwarz
  
  // wir schicken also 2 Pakete!
  // Dann wollen wir das mal bauen:

  spi_w_w = spi_w_w % 64; // Auf 6 bit begrenzen ->  sollte nichts machen
  spi_s_w = spi_s_w % 64; // Auf 6 bit begrenzen ->  sollte nichts machen
  spi_w_a = spi_w_a %128; // Auf 7 bit begrenzen ->  sollte nichts machen
  spi_s_a = spi_s_a %128; // Auf 7 bit begrenzen ->  sollte nichts machen
                                                                  
  volatile uint16_t spi_weiss_paket   = detect(weissflag)  *256*64 + spi_w_w*256 + spi_w_a*2 +1;  // detect auf bit 14, Winkel auf bit 8 bis 13, Abstand auf bit 1 bis 7, "1" auf bit 0
  //volatile uint16_t spi_schwarz_paket = detect(schwarzflag)*256*64 + spi_s_w*256 + spi_s_a*2 +1;  // detect auf bit 14, Winkel auf bit 8 bis 13, Abstand auf bit 1 bis 7, "1" auf bit 0

  // Ach ja, das Paket muss ja auch geschickt werden...
  volatile uint16_t returnData[1] = {};                           // Es wird ein array erwartet, also erzeugen wir einen mit Länge 2
  volatile uint16_t data[1] = {};                                 // SPI geht immer in zwei Richtungen, also hier zwei Empfangs-Pakete...
  returnData[0] = spi_weiss_paket;                                // Und schreiben die Pakete rein
  //returnData[1] = spi_schwarz_paket;
  SPI_SLAVE.rxtx16(data, returnData, 1);                          // Die gelesenen Pakete wird sofort verworfen   // hier steht jetzt 2, vorher 1, ist das richtig?
}


void setup() 
{                
  //set_zuordnung();
  
  pinMode(bodenled, OUTPUT);   
  
  pinMode(add0, OUTPUT);
  pinMode(add1, OUTPUT);
  pinMode(add2, OUTPUT);
  
  // spi setup
  SPI_SLAVE.begin_SLAVE(SCK, MOSI, MISO, CS0); 
  SPI_SLAVE.setCTAR_SLAVE(16, SPI_MODE0);
  NVIC_ENABLE_IRQ(IRQ_SPI0);
  
  
  weisstotzeit = 0;
  schwarztotzeit=0;
  weissflag = -100;
  schwarzflag = -100;
} // end of setup()



void sensorread() // liest alle 64 Sensorwerte aus und speicher sie im Puffer ab.
{
  for (int lauf = 0 ; lauf < 8 ; lauf++)
  {
    if ((lauf & 1) == 1) {digitalWrite(add0,HIGH);} else {digitalWrite(add0,LOW);} // die Adressports werden digital hochgezählt
    if ((lauf & 2) == 2) {digitalWrite(add1,HIGH);} else {digitalWrite(add1,LOW);}
    if ((lauf & 4) == 4) {digitalWrite(add2,HIGH);} else {digitalWrite(add2,LOW);}
    delayMicroseconds(1); // damit der multiplexer umschalten kann
    for (int sread = 0 ; sread < 8 ; sread++)
    {
      puffer[lauf*8+sread] = analogRead(sensor[sread]);
    }
  }
} // end of sensorread()

void auswertung()
{
 int weissmin=-100;
 int weissmax=-100;
 int schwarzmin = -100;
 int schwarzmax = -100;
 
  for (int lauf = 0 ; lauf < 30 ; lauf++) 
  {
    if (puffer[weiss[lauf]] > weiss_schwellwert) 
    { 
        if (weissmin == -100) weissmin=lauf;
        weissmax=lauf; 
    }
    if (puffer[schwarz[lauf]] < schwarz_schwellwert) 
    { 
        if (schwarzmin == -100) schwarzmin=lauf;
        schwarzmax=lauf; 
    }
  }

  if (weissmin   > weissmax   ) weissmin   = weissmin  -30;

  if(abs(weissmin - weissmax) < 2)
  {
    weissmin = -100;
    weissmax = -100;
  }
  
  /*Serial.print(" weissmin: "); Serial.print(weissmin);
  Serial.print(" weissmax: "); Serial.print(weissmax);
  Serial.print(" schwarzmin: "); Serial.print(schwarzmin);
  Serial.print(" schwarzmax: "); Serial.print(schwarzmax);*/
  
  // wir haben jetzt einen ersten und letzten Wert, der weiß gesehen hat und ebenso für schwarz
  // Die Richtung der Linie ist der Mittelwert aus Min und Max
  // also zwischen 0 und 30
  // wir nehmen nur min+max statt (min+max)/2
  // und bekommen werte zwischen 0 und 60
  // Da wir Winkel haben wollen (-180 bis 180), müssen wir im Hauptprogramm mit *6 -180 umrechnen:
  
  //  Irgendwie haben wir nicht bei 0 Grad angefangen zu zählen, wir kompensieren das hier ein für alle Mal...
  //  winkel = (winkel +2) modulo 60

  //Serial.println(String(weissmin) + "   ;    " + String(weissmax));
  if(weissmin == 0 && weissmax == 29)
  {
    weisswinkel = weissflag; 
  }
  else
  {
    weisswinkel = (weissmin+weissmax+2)%60; 
  }

  
  schwarzwinkel = (schwarzmin+schwarzmax+2)%60; 
  
  //Serial.print(" weisswinkel: "); Serial.print(weisswinkel);
  //Serial.println(" schwarzwinkel: "); Serial.print(schwarzwinkel);
  

  // jetzt wird der Abstand berechnet. 20 steht für "außerhalb des Roboters"
  if (weissmin > -100) weissabstand = 15-abs(weissmax-weissmin);
  else weissabstand = 20;
    
  if (schwarzmin > -100) schwarzabstand = 15-abs(schwarzmax-schwarzmin);
  else schwarzabstand = 20;

  /*Serial.print(" weissabstand: "); Serial.print(weissabstand);
  Serial.print(" schwarzabstand: "); Serial.print(schwarzabstand);
  Serial.println();*/

  // so weit so gut, aber wir haben jetzt immer Werte von vorne aus gesehen und mit negativen Abständen,
  // wenn wir mit der Mitte des Bots über die Linie hinaus sind

  // Das ist gut, solange wir vorwärts über die Linie gefahren sind.

  // Es gibt ein Problem, wenn wir rückwärts über die Linie fahren und der Übergang zwischen letztem (30) und erstem (0) Sensor
  
  // innerhalb der erkannten Linie ist.
  
  // Beispiel : weissmin = 5, weissmax = 25
  // Das liefert nach obiger Rechnung einen Winkel von (5+25) * 6 -180 = 0 und einen Abstand von 15-abs(20) = -5,
  // also 0 Grad und Abstand -5
  // kämen wir von vorne, wäre das ein super Wert, kommen wir aber tatsächlich von hinten, sollten wir
  // statt dessen einen Winkel von 180 Grad und einen Abstand von 5 erhalten
  
  // Das gleich gilt auch für folgendes Beispiel:
  // Beispiel2:  weissmin = 14 und weissmax = 18
  // regulär ergibt das Winkel = (32)*6-180 = 12 Grad und Abstand = 15-abs(4) = 11
  // also 12 Grad und Abstand 11
  // wenn wir aber von hinten gekommen sind, müssten es
  // -168 Grad und Abstand -11 sein.

  // es kommt also darauf an, von wo wir die Linie das erste mal haben kommen sehen, 
  // um zu entscheiden, welcher Wert stimmt.

  // Aus diesem Grund gibt es jetzt noch den schwarzflag und den weissflag
  // steht dieser auf -100, so wurde die Linie noch nicht vorher gesehen
  // in diesem Fall nehmen wir denjenigen der beiden möglichen Werte, der einen positiven Abstand hat,
  // weil wir annehmen, dass wir die Linie sonst schon vorher erkannt hätten, wenn wir über sie gefahren wären
  // ist dies der Wert ohne den "Nulldurchgang", setzen wir die Flag auf winkel, 
  // ist dies der Wert mit "Nulldurchgang", setzen wir die Flag auf winkel um 30 (also 180 Grad) weitergedreht.
  // Steht der Wert für die Flag schon auf gedrehtem oder echtem Winkel, so bleibt die Flag erhalten, auch wenn dann irgendwann negative Anstände entstehen.

  // Wird keine Linie mehr erkannt, wird die Flag wieder auf -100 gesetzt...
  
  // Aber Achtung, wir sollten hier eine "Totzeit" einbauen, damit minimale Aussetzer in der Erkennung nicht 
  // zu einem "Richtungssprung" führen.
  // Wir nehmen als Totzeit fürs Erste 4 Messungen, das entspricht 50 Millisekunden. Oder ist das zu wenig?
  // (Der Roboter fährt bei absolutem Vollgas rund 10-12 cm in 50 Millisekunden
  // Aber bei "normalem Liniengewarte" natürlich nur ein paar Millimeter)
  
  // Die Totzeit wird aber erst nach und nach aufgebaut, damit eine einmalige "Falscherkennung"
  // einer Linie bei der nächsten Messung nicht zu einem falsch positiven Ergebnis führt.

  // genug erklärt: es geht los:
  if (weissabstand == 20)               // es wurde keine weiße Linie gesehen
  {
    if (weisstotzeit < 2 )              // nicht < 1, wir erlauben eine Fehlmessung 
    {
      spi_w_a=20;                       // SPI - Wert aus "keine Linie" setzen
      if(flagzeit > 50)
      {
        weissflag = -100;
      }
      
      weisstotzeit =0;
    }
    else weisstotzeit--;                // SPI - Wert nicht ändern
  }
  else                                  // weiße Linie
  {
    
    flagzeit = 0;
    if (weisstotzeit < 10) weisstotzeit++; 
    if (weissflag == -100)                 // Linie ist neu
    {
      if (weissabstand <0) weissflag=((weisswinkel+30) % 60); // bei neue Linie nicht zu erwarten, also umdrehen
          else weissflag=weisswinkel;                         // alles gut, nicht drehen
    }
    if (abs(weissflag-weisswinkel) > 15)                 // Nulldurchgang erkannt, es wird auf jeden Fall gedreht
    {
        weissabstand = - weissabstand;  // jetzt ist es > 0
        weisswinkel = ((weisswinkel+30) % 60);   
    }
    
    weissflag=weisswinkel;              //Anpassen, falls der Roboter sich dreht.
    
    spi_w_a = 100+weissabstand;
    spi_w_w = weisswinkel;
  }
/*Serial.print(" weissflag: "); Serial.print(weissflag);*/
        
    weisszeit_alt = weisstotzeit;
   
 
} // end of auswertung()


void schreibe()
{
  
  Serial.print("weiß ");               Serial.print(detect(weissflag));
  Serial.print("    Abstand");    Serial.print(spi_w_a);
  Serial.print("    Winkel   ");     Serial.print(spi_w_w);

  Serial.print("    schwarz ");           Serial.print(detect(schwarzflag));
  Serial.print("    Abstand"); Serial.print(spi_s_a);
  Serial.print("    Winkel  ");  Serial.print(spi_s_w);
 
/*for (int lauf = 0 ; lauf < 30 ; lauf++) 
  {
    Serial.print(puffer[weiss[lauf]]); 
    Serial.print(" ");
  }
 Serial.println(); 
 for (int lauf = 0 ; lauf < 30 ; lauf++) 
  {
    Serial.print(puffer[schwarz[lauf]]); 
    Serial.print(" ");
  }   
 Serial.println();*/
 

  
  Serial.println();Serial.println();
}


// the loop routine runs over and over again forever:
void loop() 
{
  zeit = 0;
  digitalWrite(bodenled, HIGH);   
  delayMicroseconds(100);           // eigentlich nur 100  
  sensorread();                     //  dauert wohl auch 1ms - testen
  digitalWrite(bodenled, LOW);  
  auswertung();
  //schreibe(); // nur für die debug-Phase
  delay(1);
  //while (zeit < 12500) delayMicroseconds(1);                    // fülle auf 12500mics auf  -> das sind 80 Herz Auslesefrequenz! Das sollte reichen ;)
  //while (zeit < 1000000) delayMicroseconds(1);                   // fülle auf 1 Sekunde auf - nur Diagnose !!!
  
}



