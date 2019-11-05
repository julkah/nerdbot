/*
 * Achtung, diese Funktionen sind das Ergebnis jahrelanger Entwicklung
 * Innerhalb der functions.h sollte eigentlich nichts geÃ¤ndert werden mÃ¼ssen.
 * Falls es doch jemandem erscheint, als mÃ¼sse hier etwas geÃ¤ndert werden, darf dies nur 
 * nach RÃ¼cksprache mit dem Coach erfolgen, da dann alle anderen Teams Ã¼ber die Ã„nderungen informiert werden mÃ¼ssen.
 * 
 * Das Verhalten des Roboters wird ausschlieÃŸlich in der handeln.h programmiert!!!
 */
 
// Hier die Auflistung der von auÃŸen nutzbaren functions:

void warteblink(int time)                         ;   //

void kick(int t)                                  ;
void kicktest(int t)                              ;
void dribbel(int s)                               ;

void fahre(int richtung, int geschw, int dreh)    ;
void aus()                                        ;
void roll()                                       ;

void lesen()                                      ;
void led_schalten()                               ;


// Hier die Implementation. Noch mal: Finger weg !

void warteblink(int time)
{
/*  for (int lauf=0;lauf<time; lauf++)
  {
    if ((lauf % 100)==0) 
    {
      digitalWrite(led, HIGH);
    }
    if ((lauf % 100)==50) 
    {
      digitalWrite(led, LOW);
    }
    delay(1);
  }
  digitalWrite(led, HIGH);
  */
  delay(time);
}

int spdToPwm(double spd) 
{
 //pwm 25..229 (<10%..<90%) fÃ¼r 0..100
 return (int) (25 + (205/100)*abs(spd)); 
}

int spdToPwmpol(double spd) 
{
 //pwm 25..229 (<10%..<90%) fÃ¼r 0..100
 return (int) (0 + (250/100)*abs(spd)); 
}


void schreibeMotoren(int p1, boolean e1, boolean d1, int p2, boolean e2, boolean d2, int p3, boolean e3, boolean d3) 
{
  analogWrite(PWM1, p1);
  digitalWrite(ENA1, e1);
  digitalWrite(DIR1, d1);
  analogWrite(PWM2, p2);
  digitalWrite(ENA2, e2);
  digitalWrite(DIR2, d2);
  analogWrite(PWM3, p3);
  digitalWrite(ENA3, e3);
  digitalWrite(DIR3, d3);
}

void kick(int t) 
{
  if(t > 20) t=20;
  if(t < 0) t=0;
  digitalWrite(KICKER, HIGH);
  delay(t);
  digitalWrite(KICKER, LOW);
  delay(100);                          //kein kick ohne 1/10 s Pause
}


void kicktest(int t)
{
  kick(t);
  warteblink(1000);
}


void dribbel(int s)
{
  if (s==1) digitalWrite(DRIBBLER, HIGH);
  else      digitalWrite(DRIBBLER, LOW);
}


void fahremaxxon(int richtung, int geschw, int dreh) 
{
  //richtung: -180..180 in Grad
  //geschw:   0..100
  //dreh:     -100..100
  

  //Variablen  
  int p1, p2, p3;    //PWM Werte
  boolean e1, e2, e3, d1, d2, d3;  //Richtungs Werte
  double spd_1, spd_2, spd_3;  //Geschwindigkeits Werte
  double richtung_rad;         //Richtung in Radiant
  double max_soll;                //Maximales Soll, die geschw des schnessten Rads
  double max_hat;               //Geschw den schnellsten Rads
  
  //Parameter fixen
  dreh = -dreh;                  //recht ist plus...
  if(geschw<0) geschw=0;
  if(geschw>100) geschw=100;
  richtung=richtung % 360;
  if(dreh>100) dreh=100;
  if(dreh<-100)dreh=-100;
 
  //Richtung in Radiant umrechnen 
  richtung_rad = richtung * PI/180.0;
  
  //Maximales Soll berechnen
  max_soll=geschw + abs(dreh);
  if(max_soll>100) max_soll=100;
   
  //Lineare Anteile berechnen
  spd_1=sin((1.0/3.0)*PI-richtung_rad)*geschw;  //60 Grad
  spd_2=sin((3.0/3.0)*PI-richtung_rad)*geschw;  //180 Grad
  spd_3=sin((5.0/3.0)*PI-richtung_rad)*geschw;  //300 Grad
   
  //AngulÃ¤re Anteile dazu addieren
  spd_1+=dreh;
  spd_2+=dreh;
  spd_3+=dreh;
   
  //auf maximales soll ziehen
  max_hat = abs(spd_1);
  if (abs(spd_2)>max_hat) max_hat= abs(spd_2);
  if (abs(spd_3)>max_hat) max_hat= abs(spd_3);
  if (max_hat > 0) 
  {
    spd_1=(1/max_hat)*max_soll*spd_1; //wenn 0 ?
    spd_2=(1/max_hat)*max_soll*spd_2;
    spd_3=(1/max_hat)*max_soll*spd_3;
  }
   
  //in pwm und dir umrechnen
  p1=spdToPwm(spd_1);
  p2=spdToPwm(spd_2);
  p3=spdToPwm(spd_3);
  
  if (spd_1 < 0 )       {  e1 = HIGH;  d1 = HIGH;  }
  else if (spd_1 > 0 )  {  e1 = HIGH;  d1 = LOW;  }
       else             {  e1 = LOW;   d1 = HIGH;  }

  if (spd_2 < 0 )       {  e2 = HIGH;  d2 = HIGH;  }
  else if (spd_2 > 0 )  {  e2 = HIGH;  d2 = LOW;  }
       else             {  e2 = LOW;   d2 = HIGH;  }
  
  if (spd_3 < 0 )       {  e3 = HIGH;  d3 = HIGH;  }
  else if (spd_3 > 0 )  {  e3 = HIGH;  d3 = LOW;  }
       else             {  e3 = LOW;   d3 = HIGH;  }
  
  
  //Werte an Motoren schreiben
  schreibeMotoren(p1, e1 , d1 ,p2, e2, d2, p3, e3, d3);
} // end of fahremaxxon


void fahrepololu(int richtung, int geschw, int dreh) 
{
  //richtung: -180..180 in Grad
  //geschw:   0..100
  //dreh:     -100..100
  

  //Variablen  
  int p1, p2, p3;    //PWM Werte
  boolean a1, a2, a3, b1, b2, b3;  //Richtungs Werte
  double spd_1, spd_2, spd_3;  //Geschwindigkeits Werte
  double richtung_rad;         //Richtung in Radiant
  double max_soll;                //Maximales Soll, die geschw des schnessten Rads
  double max_hat;               //Geschw den schnellsten Rads
  
  //Parameter fixen
  dreh = -dreh;                  //recht ist plus...
  if(geschw<0) geschw=0;
  if(geschw>100) geschw=100;
  richtung=richtung % 360;
  if(dreh>100) dreh=100;
  if(dreh<-100)dreh=-100;
 
  //Richtung in Radiant umrechnen 
  richtung_rad = richtung * PI/180.0;
  
  //Maximales Soll berechnen
  max_soll=geschw + abs(dreh);
  if(max_soll>100) max_soll=100;
  
  //Lineare Anteile berechnen
  spd_1=sin((1.0/3.0)*PI-richtung_rad)*geschw;  //60 Grad
  spd_2=sin((3.0/3.0)*PI-richtung_rad)*geschw;  //180 Grad
  spd_3=sin((5.0/3.0)*PI-richtung_rad)*geschw;  //300 Grad
   
  //AngulÃ¤re Anteile dazu addieren
  spd_1+=dreh;
  spd_2+=dreh;
  spd_3+=dreh;
  
  //auf maximales soll ziehen
  max_hat = abs(spd_1);
  if (abs(spd_2)>max_hat) max_hat= abs(spd_2);
  if (abs(spd_3)>max_hat) max_hat= abs(spd_3);
  if (max_hat > 0) 
  {
    spd_1=(1/max_hat)*max_soll*spd_1; //wenn 0 ?
    spd_2=(1/max_hat)*max_soll*spd_2;
    spd_3=(1/max_hat)*max_soll*spd_3;
  }
   
  //in pwm und dir umrechnen
  p1=spdToPwmpol(spd_1);
  p2=spdToPwmpol(spd_2);
  p3=spdToPwmpol(spd_3);
  
  if (spd_1 > 0 ) {  a1 = HIGH;  b1 = LOW;  }     // <,> getauscht
  else            {  a1 = LOW;  b1 = HIGH;  }
  
  if (spd_2 > 0 ) {  a2 = HIGH;  b2 = LOW;  }
  else            {  a2 = LOW;  b2 = HIGH;  }
  
  if (spd_3 > 0 ) {  a3 = HIGH;  b3 = LOW;  }
  else            {  a3 = LOW;  b3 = HIGH;  }
  
  //Werte an Motoren schreiben
  schreibeMotoren(p1, a1 , b1 ,p2, a2, b2, p3, a3, b3);
} // end of fahrepololu

void fahre(int richtung, int geschw, int dreh) 
{
  if (maxxon==1) fahremaxxon(richtung, geschw, dreh) ;
  if (maxxon==2) fahrepololu(richtung, geschw, dreh) ;
}

void aus() // Aktives AUS fÃ¼r Maxon (und Pololu?) 
{
  schreibeMotoren(20, HIGH, HIGH, 20, HIGH, HIGH, 20, HIGH, HIGH);
}

void roll() // spricht fÃ¼r sich
{
  fahre(0,0,0);
}

int sign(int x)
{
  if (x>0) return 1;
  if (x<0) return -1;
  return 0;
}

int signum(int x)
{
  if (x>=0) return 1;
  else if (x<0) return -1;
}

// jetzt die ganzen Sensoren auslesen:


int kompass_org()
{
  unsigned char high_byte, low_byte, angle8;
  unsigned int angle16;
  Wire.beginTransmission(CMPS11_ADDRESS);
  Wire.write(ANGLE_8);
  Wire.endTransmission();
  Wire.requestFrom(CMPS11_ADDRESS, 3);
  while(Wire.available() < 3);
  angle8 = Wire.read();               // Read back the 5 bytes
  high_byte = Wire.read();
  low_byte = Wire.read();
  angle16 = high_byte;                 // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;
  return angle16 / 10;
}

int kompass_lesen()
{
  return(-1*((((kompass_org()-head)+180+360)%360)-180));
}

void set_heading()
{
  Serial.print("set_heading ... ");
  head=kompass_org();
  led_hinten += BLAU;
  led_schalten();
  delay(500);
  led_hinten = led_hinten - BLAU + GELB;
  led_schalten();
  delay(500);
  Serial.println("done");
}

void SPI_lesen()
{
  int ballvorne, tor1, ballhinten, tor2;
  int id = 0;
  spi_fehler =  LED_SPI0 + LED_SPI1;
  // IR_Sensor auslesen:
    ir_data[0] = torfarbe;
    torfarbe = 0;
    ir_data[0] = ir_data[0] << 2;
    SPI_MASTER.txrx16(ir_data, ir_returnData, ir_dataLength,CTAR_0,CS1);
    id = (ir_returnData[0] & 24576);
    id = id >> 13;
    if(id == 0)
    {
      ballvorne = ir_returnData[0];
    }
    else if(id == 1)
    {
      tor1 = ir_returnData[0];
    }
    else if(id == 2)
    {
      ballhinten = ir_returnData[0];
    }
    else if(id == 3)
    {
      tor2 = ir_returnData[0];
    }
    ir_data[0] = 1;
    SPI_MASTER.txrx16(ir_data, ir_returnData, ir_dataLength,CTAR_0,CS1);
    id = (ir_returnData[0] & 24576);
    id = id >> 13;
    if(id == 0)
    {
      ballvorne = ir_returnData[0];
    }
    else if(id == 1)
    {
      tor1 = ir_returnData[0];
    }
    else if(id == 2)
    {
      ballhinten = ir_returnData[0];
    }
    else if(id == 3)
    {
      tor2 = ir_returnData[0];
    }
    ir_data[0] = 2;
    SPI_MASTER.txrx16(ir_data, ir_returnData, ir_dataLength,CTAR_0,CS1);
    id = (ir_returnData[0] & 24576);
    id = id >> 13;
    if(id == 0)
    {
      ballvorne = ir_returnData[0];
    }
    else if(id == 1)
    {
      tor1 = ir_returnData[0];
    }
    else if(id == 2)
    {
      ballhinten = ir_returnData[0];
    }
    else if(id == 3)
    {
      tor2 = ir_returnData[0];
    }
    ir_data[0] = 3;
    SPI_MASTER.txrx16(ir_data, ir_returnData, ir_dataLength,CTAR_0,CS1);
    id = (ir_returnData[0] & 24576);
    id = id >> 13;
    if(id == 0)
    {
      ballvorne = ir_returnData[0];
    }
    else if(id == 1)
    {
      tor1 = ir_returnData[0];
    }
    else if(id == 2)
    {
      ballhinten = ir_returnData[0];
    }
    else if(id == 3)
    {
      tor2 = ir_returnData[0];
    }
    int ir_fehler = 1;
    if ((ballhinten/32768>0) || (ballhinten%2<1) || (ballvorne/32768>0) || (ballvorne%2<1) || (tor1/32768>0) || (tor1%2<1) || (tor2/32768>0) || (tor2%2<1)) ir_fehler=1; else ir_fehler=0;
    //Serial.println("Bv: " + String(ballvorne) + " Tv: " + String(tor1) + " Bh: " + String(ballhinten) + " Th: " + String(tor2));
  // Auswertung IR-Paket
    if (ir_fehler==0)
    {
      failcounter = 0;
      if((!odroid1) && ((ballvorne != 1) || (tor1 != 8193)))
        {
          odroid1 = true;
          if(odroidtime1 < 10){odroidtime1++; delay(50); torfarbe = 5;}
        }
        else if(((ballvorne != 1) || (tor1 != 8193)))
        {
          odroid1 = false;
        }
        if((!odroid2) && ((ballhinten != 16385) || (tor2 != 24577)))
        {
          odroid2 = true;
          if(odroidtime2 < 10){odroidtime2++; delay(50); torfarbe = 5;}
        }
        else if(((ballhinten != 16385) || (tor2 != 24577)))
        {
          odroid2 = false;
        }
      camsignv = ballvorne;
      
      xposv = camsignv;
      xposv = xposv & 8160;
      xposv = xposv >> 5;
      xposv = (xposv*1000000)/796875;
      distv = camsignv;
      distv = distv & 30;
      distv = distv >> 1;
      onsv = camsignv;
      onsv = onsv & 8160;
      if(onsv == 0){onsv = 0;}else{onsv = 1;}

      camsignh = ballhinten;
      
      xposh = camsignh;
      xposh = xposh & 8160;
      xposh = xposh >> 5;
      xposh = (xposh*1000000)/796875;
      disth = camsignh;
      disth = disth & 30;
      disth = disth >> 1;
      onsh = camsignh;
      onsh = onsh & 8160;
      if(onsh == 0){onsh = 0;}else{onsh = 1;}
      
      camsignvt = tor1;
      
      xposvt = camsignvt;
      xposvt = xposvt & 8128;
      xposvt = xposvt >> 7;
      xposvt = (xposvt*10)/4;
      distvt = camsignvt;
      distvt = distvt & 252;
      distvt = distvt >> 2;
      onsvt = camsignvt;
      onsvt = onsvt & 8160;
      if(onsvt == 0){onsvt = 0;}else{onsvt = 1;}
      ssv = camsignvt;
      ssv = (ssv & 2);
      ssv = ssv >> 1;
      ss = ssv;

      camsignht = tor2;
      
      xposht = camsignht;
      xposht = xposht & 8128;
      xposht = xposht >> 6;
      xposht = (xposht*10)/4;
      distht = camsignht;
      distht = distht & 60;
      distht = distht >> 2;
      onsht = camsignht;
      onsht = onsht & 8160;
      if(onsht == 0){onsht = 0;}else{onsht = 1;}
      if(onsv == true && onsh == true){if(disth>distv){onsv=false;}else{onsh=false;}}
      if(onsv == false && onsh == false){ballrichtung = -999;}
      if(onsv == true){ballentfernung = distv; if(xposv > 160){ballrichtung = (xposv -160)*9/16;}else{ballrichtung = (xposv*9/16)-90;}}
      else if(onsh == true){ballentfernung = disth; if(xposh > 160){ballrichtung = ((-(320-xposh))*9/16)-90;}else{ballrichtung = (xposh*9/16)+90;}}
      
      if(onsvt == false){vtorrichtung = -999;}
      if(onsht == false){htorrichtung = -999;}
      if(onsvt == true){vtorentfernung = distvt; if(xposvt > 80){vtorrichtung = xposvt - 80;}else{vtorrichtung = (-1)* (80 - xposvt);}}
      if(onsht == true){htorentfernung = distht; if(xposht > 160){htorrichtung = ((-(320-xposht))*9/16)-90;}else{htorrichtung = (xposht*9/16)+90;}}
      
      spi_fehler= spi_fehler - LED_SPI1;    // SPI Fehler fÃ¼r SPI-1 aussachleten

    }
//    else
//    {
//      failcounter++;
//      if(failcounter > 25)
//      {
//        torfarbe = 5;
//        failcounter = 0;
//        delay(200);
//      }
//    }
    


  // Boden_Sensor auslesen:
    SPI_MASTER.txrx16(boden_data, boden_returnData, boden_dataLength,CTAR_0,CS0);
    //uint16_t schwarz_paket    = boden_returnData[0];
    uint16_t weiss_paket  = boden_returnData[0];
    int boden_fehler = 1;  
    if ((weiss_paket/32768>0) || (weiss_paket%2<1)/* || (schwarz_paket/32768>0) || (schwarz_paket%2<1)*/) 
        boden_fehler=1; else boden_fehler=0;
  
  // Auswertung boden-Paket
    if (boden_fehler==0)
    {
       weissabstand = ((weiss_paket%256)/2)-100; 
       weisswinkel  = (weiss_paket/256)%64;
       weisswinkel = (weisswinkel*6)-180;
       sieht_weiss  = ((weiss_paket/256)/64)%2;

       /*schwarzabstand = ((schwarz_paket%256)/2)-100; 
       schwarzwinkel  = (schwarz_paket/256)%64; 
       schwarzwinkel = (schwarzwinkel*6)-180;
       sieht_schwarz  = ((schwarz_paket/256)/64)%2;*/
       spi_fehler= spi_fehler - LED_SPI0; // SPI Fehler fÃ¼r SPI-0 aussachleten
    }
    

  // zukÃ¼nftig weitere Sensoren auslesen:
    //SPI_MASTER.txrx16(data, returnData, dataLength,CTAR_0,CS2);
    //SPI_MASTER.txrx16(data, returnData, dataLength,CTAR_0,CS3);
    
  SPI_MASTER.packetCT=0;
}

void taster_lesen()
{
  Wire.requestFrom(TASTER_ADDRESS, 1);
  if(Wire.available()) 
  {
     tastbyte = Wire.receive();
  }
  else tastbyte=255;
}

void analog_lesen()
{
  Wire.requestFrom(AD_ADDRESS, 4);
  if(Wire.available()) 
  {
     ad_batt          = Wire.receive();
     ad_lichtschranke = Wire.receive();
     ad_2             = Wire.receive();
     ad_3             = Wire.receive();
  }  
}

void cam_lesen(){}

bool lichtschranke()
{
  if(ad_lichtschranke < 30)
  {
    return true;
  }
  else
  {
    return false;
  }
}


void lesen()
{
  kompass = kompass_lesen();
  SPI_lesen();
  taster_lesen();
  analog_lesen();
  
}


void led_schalten()
{
  Wire.beginTransmission(LED_VORNE_ADDRESS); // 0x22 klappt
  Wire.write(255-led_vorne);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission(LED_HINTEN_ADDRESS); // 0x22 klappt
  Wire.write(255-led_hinten);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
}


void setup() 
{
  Serial.begin(115200);
  Serial.print("Setup ... ");
  Serial1.begin(38400);
  // Motor Pins einrichten
  pinMode(PWM1, OUTPUT); 
  pinMode(ENA1, OUTPUT);  
  pinMode(DIR1, OUTPUT); 
  pinMode(PWM2, OUTPUT); 
  pinMode(ENA2, OUTPUT);  
  pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT); 
  pinMode(ENA3, OUTPUT);  
  pinMode(DIR3, OUTPUT);
  pinMode(KICKER, OUTPUT);
  pinMode(DRIBBLER, OUTPUT);
  
  digitalWrite(KICKER, LOW);
  digitalWrite(DRIBBLER, LOW);
  roll();

  // SPI BUS starten
  SPI_MASTER.begin_MASTER(SCK, MOSI, MISO, CS0, CS_ActiveLOW);
  SPI_MASTER.enableCS(CS0, CS_ActiveLOW);
  SPI_MASTER.enableCS(CS1, CS_ActiveLOW);
  SPI_MASTER.enableCS(CS2, CS_ActiveLOW);
  SPI_MASTER.enableCS(CS3, CS_ActiveLOW);
  
  SPI_MASTER.setCTAR(CTAR_0,16,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV16);   // DIV4 heiÃŸt 12 MHz
  for (int i=0; i<ir_dataLength; i++) { ir_data[i]=0; }
  for (int i=0; i<boden_dataLength; i++) { boden_data[i]=0; }


  // I2C BUS starten
  Wire.begin(); // join i2c bus (address optional for master) 
  
  // LEDs alle an
  
  Wire.beginTransmission(LED_VORNE_ADDRESS); 
  Wire.write(0);                
  Wire.endTransmission();
  
  Wire.beginTransmission(LED_HINTEN_ADDRESS); 
  Wire.write(0);                
  Wire.endTransmission();

  delay(500);

  Wire.beginTransmission(LED_VORNE_ADDRESS); 
  Wire.write(255);                
  Wire.endTransmission();
  
  Wire.beginTransmission(LED_HINTEN_ADDRESS); 
  Wire.write(255);                
  Wire.endTransmission();

  delay(500);

  Wire.beginTransmission(LED_VORNE_ADDRESS); 
  Wire.write(0);                
  Wire.endTransmission();
  
  Wire.beginTransmission(LED_HINTEN_ADDRESS); 
  Wire.write(0);                
  Wire.endTransmission();

  delay(500);
  
  // LEDS alle aus
  Wire.beginTransmission(LED_VORNE_ADDRESS); 
  Wire.write(255);                
  Wire.endTransmission();
  
  Wire.beginTransmission(LED_HINTEN_ADDRESS); 
  Wire.write(255);                
  Wire.endTransmission();


  // Analogport fÃ¼r Lesemodus einrichten
  Wire.beginTransmission(AD_ADDRESS); 
  Wire.write(AD_SETUP);
  Wire.endTransmission();
  delay(2);
  Wire.beginTransmission(AD_ADDRESS); 
  Wire.write(AD_CONFIG);
  Wire.endTransmission();
  delay(2);

  modus=0;  // Modus, wenn der Roboter eingeschaltet wird.
  spielart = 0; // solange nicht genutzt, bleibt es auf 0
  Serial.println("done");
}

String blau_lesen()
{
  inputString = "";
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    if (inChar == '$') {
      fertiginput = true;
    }
    
    if(inChar == '\n')
    {
      stringComplete = true;
      break;
    }
    if(fertiginput == false)
    {
      inputString += inChar;
    }
    else
    {
      teststring += inChar;
    }
    /*Serial.print(inputString);
    Serial.print("  -  ");
    Serial.println(teststring);*/
  }
  if(stringComplete)
  {
    if(teststring == normalstring)
    {
      testerg = true;
    }
    else
    {
      testerg = false;
    }
    fertiginput = false;
    teststring = "";
  }
  if(stringComplete && testerg)
  {
    return inputString;
  }
  else
  {
    return "-1";
  }
}

void blau_senden(String senden)
{
  senden += normalstring;
  Serial1.print(String(senden + "\n"));
}

int taster_auswerten()
{
  if(tastbyte != 255)
  {
    if(tastbyte == TASTE_STOP){return 0;}
    else if(tastbyte == TASTE_START){return 1;}
    
     else if (tastbyte == TASTE_L_XOOO) 
        {
          return 2;
        }
    else if (tastbyte == TASTE_L_OXOO) 
        {
          return 3;
        }  
    else if (tastbyte == TASTE_L_OOXO) 
        {
          return 4;
        }
    else if (tastbyte == TASTE_L_OOOX) 
        {
          return 5;
        }
    else if (tastbyte == TASTE_R_XOOO) 
        {
          return 6;   
        }
    else if (tastbyte == TASTE_R_OXOO) 
        {     
          return 7;  
        }  
    if (tastbyte == TASTE_R_OOXO) 
        {      
          return 8;
        }
    else if (tastbyte == TASTE_R_OOOX) 
        {     
          return 9;
        }

  }
  return modus;
  
}

void led_setzen()
{
  led_hinten  = 0;
  led_vorne   = 0;
  led_vorne=led_vorne +   spi_fehler;
                          
  if (modus == 1)             {
                              led_hinten=led_hinten +   (BLINK1*((zeit/100000) % 2));
                              led_hinten=led_hinten +   (BLINK2*((zeit/100000+1) % 2));
                              }
  if((blue == 1)&&(modus != 1))
  {
    led_hinten += BLAU;
  }
  else if((blue == 2)&&(modus != 1))
  {
    led_hinten += GELB;
  }

  if (tastbyte == TASTE_STOP)
  {
    if (ad_batt > 170)          led_hinten=led_hinten +   LED4;     
    if (ad_batt > 165)          led_hinten=led_hinten +   LED3;
    if (ad_batt > 160)          led_hinten=led_hinten +   LED2;     
    if (ad_batt > 155)          led_hinten=led_hinten +   LED1;
  }

    
}

void all_data()
{
  Serial.println();
  Serial.println();
  Serial.println("--------------------------------");
  Serial.println();
  Serial.println();
  Serial.print("white:  ");Serial.print(sieht_weiss);Serial.print("  ,  ");Serial.print(weissabstand);Serial.print("  ,  ");Serial.print(weisswinkel);Serial.println();
  Serial.print("black:  ");Serial.print(sieht_schwarz);Serial.print("  ,  ");Serial.print(schwarzabstand);Serial.print("  ,  ");Serial.print(schwarzwinkel);Serial.println();
  Serial.print("compass:  ");Serial.print(kompass);Serial.println();
  Serial.print("battery:  ");Serial.print(ad_batt);Serial.println();
  Serial.print("xpos:   ");Serial.print("null");Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("--------------------------------");
  Serial.println();
  Serial.println();
  delay(500);
}


