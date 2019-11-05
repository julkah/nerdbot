

/* 
 *  
 *  Haupt-Programm
 *  hier bitte nichts Ã¤ndern, es wird in einer dauer loop gelesen und gehandelt
 *   
*/

#include "header.h"
#include "functions.h"
#include "handeln.h"

// setup() ist in functions enthalten

void loop() 
{
  //delay(30);
  Wire.resetBus();
  lesen();                        // in functions
  blau = blau_lesen();
  ir_data[0] = 0;
  modus = taster_auswerten();     // in handeln
  licht = lichtschranke();
  led_setzen();                   // in handeln
  led_schalten();                 // in functions
  //Serial.println(ballentfernung);
  if(blau.toInt() < 0)
  {
    near = true;
  }
  else if((blau.toInt()-2) < ballentfernung)
  {
    led_hinten += LED2;
    near = true;
  }
  else
  {
    led_hinten += LED3;
    near = false;
  }
  //Serial.println(tastbyte);
  //Serial.println(modus);
  //Serial.println(" Ballrichtung: " + String(ballrichtung) + " VTorRichtung: " + String(vtorrichtung) + " HTorRichtung: " + String(htorrichtung) + " SS: " + String(ss));
  //Serial.println("weiss:  " + String(sieht_weiss) + "  ,  " + String(weisswinkel) + "  abstand:  " + String(weissabstand));
  //Serial.println("ad_batt:  " + String(ad_batt) + "  ad_lichtschranke:  " + String(ad_lichtschranke) + "  ad_2:  " + String(ad_2) + "  ad_3:  " + String(ad_3));
  //Serial.println(licht);
  /*Serial.print("torricht: ");
  Serial.println(htorrichtung);
  Serial.print("torhent     : ");
  Serial.println(htorentfernung);*/
  /*Serial.println("print");
  Serial.print(ballrichtung);
  Serial.print("   ;   ");*/
  //Serial.println(blau);
  //Serial.println(kompass);
  /*Serial.print(xposv);
  Serial.print("   ;   ");
  Serial.println(disth);*/
  //Serial.println(torfarbe);
  //Serial.println(ballentfernung);
  delay(10);

  if(modus == 0)
  {
    blau_senden(String(0));
    roll();                      // in handeln
  }
  else if(modus == 1)
  {
    handeln();
    blau_senden(String(ballentfernung));
  }
  else if(modus == 2)
  {
    set_heading();
    modus = 0;
  }
  else if(modus == 3)
  {
    if(blue == 0 || blue == 2)
    {
      blue = 1;                                          //wir haben blaues Tor
    torfarbe = 1;
    modus = 0;
    led_vorne += (ROT);
    led_schalten();
    delay(300);
    }
    else
    {
      blue = 2;                                          //wir haben gelbes Tor
    torfarbe = 2;
    modus = 0;
    led_vorne += (ROT);
    led_schalten();
    delay(300);
    }
    
  }
  else if(modus == 4)
  {
    modus = 0;
  }
  else if(modus == 5)
  {
    digitalWrite(KICKER, HIGH);
  delay(15);
  digitalWrite(KICKER, LOW);
  delay(500);
    modus = 0;
  }
  else if(modus == 6)
  {
    if(fast == true)
    {
    torfarbe = 3;
    modus = 0;
    fast = false;
    }
    else
    {
    torfarbe = 4;
    modus = 0;
    fast = true;
    }
    delay(200);
  }
  else if(modus == 7)
  {
    torfarbe = 5;
    modus = 0;
    led_vorne += (ROT);
    led_schalten();
    delay(200);
  }
  else if(modus == 8)
  {
    if(!band){torfarbe = 6; aufnahme = true; band = true; delay(200);}else if(band){torfarbe = 7; aufnahme = false; band = false; delay(200);}
    modus = 0;
  }
  else if(modus == 9)
  {
    
  }
  if(odroid1)
  {
    if(fast)
    {
    led_vorne += (L_GRUEN);
    led_schalten();
    }
    else
    {
    led_vorne += (L_GELB);
    led_schalten();
    }
  }
  if(odroid2)
  {
    if(fast)
    {
    led_vorne += (R_GRUEN);
    led_schalten();
    }
    else
    {
    led_vorne += (R_GELB);
    led_schalten();
    }
  }
  if(band)
  {
    if(aufnahmetime > 500 && aufnahmetime < 1000)
    {
      led_vorne += (ROT);
      led_schalten();
    }
    else if(aufnahmetime > 1000)
    {
      aufnahmetime = 0;
    }
  }
  if(ad_batt < 160)
  {
    if(battime > 500 && battime < 1000)
    {
      led_hinten += (LED1 + LED2 + LED3 + LED4);
      led_schalten();
    }
    else if(battime > 1000)
    {
      battime = 0;
    }
  }
}





