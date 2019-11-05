

#include <t3spi.h>
/*
 * 
 * 
 *    Für Ausgabe unten schaune -> schreibe();
 *    nochmal ausprobieren; war nämlich snytaktisch falsch :/
 * 
 * 
 * 
 * 
 * 2017_Feb_03
 *  Bodensensor Ring SPI
 */

#define CS10        0x0A    // SPI Cable Select auf Pin 10
T3SPI SPI_SLAVE;

boolean fast = false; // ACHTUNG GANZ WICHTIG!!!!!!!!!!!!!!!!!!!!!!!

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
volatile int serialthree, serialone, serialonet, serialthreet;
boolean torsicht = false;
int torsi;
int torweite = 0;
int iLastX = 0;
int led = 13;
int torfarbe = 0;

#define fahnenlaenge 1

volatile uint16_t fahne[fahnenlaenge] = {};
boolean first = false;
boolean blue = false;
int flag;
                                      //4 blau 3 gelb
int lowh = 0;//5
int highh = 25;//27
int lows = 150; //150
int highs = 200;//240
int lowv = 160;//200
int highv = 220;//255
int lowhr = 0;//5
int highhr = 0;//27
int lowsr = 90; //150
int highsr = 150;//240
int lowvr = 0;//200
int highvr = 0;//255

int lowht = 100;//5
int highht = 125;//27
int lowst = 150; //150
int highst = 225;//240
int lowvt = 150;//200
int highvt = 230;//255

int lowhtg = 36;//5
int highhtg = 40;//27
int lowstg = 140; //150
int highstg = 160;//240
int lowvtg = 190;//200
int highvtg = 255;//255

int cambrightness = 0;
int camhue = 60;
int camcontrast = 90;

int lowhb = 0;//5
int highhb = 19;//27
int lowsb = 150; //150
int highsb = 200;//240
int lowvb = 200;//200
int highvb = 230;//255
int lowhrb = 0;//5
int highhrb = 0;//27
int lowsrb = 20; //150
int highsrb = 60;//240
int lowvrb = 0;//200
int highvrb = 0;//255

int lowhtb = 95;//5
int highhtb = 120;//27
int lowstb = 120; //150
int highstb = 200;//240
int lowvtb = 130;//200
int highvtb = 200;//255

int lowhtgb = 35;//95
int highhtgb = 50;//120
int lowstgb = 150; //120
int highstgb = 210;//200
int lowvtgb = 150;//90
int highvtgb = 255;//150

int cambrightnessb = 15;
int camhueb = 60;
int camcontrastb = 0;

void spi0_isr(void)
{
   SPI_SLAVE.rx16(fahne, fahnenlaenge);
   torfarbe = fahne[0];
   torfarbe = torfarbe >> 2;
    if(torfarbe == 1)
    {
      //wir haben blau
      blue = true;
      Serial1.print("3\n");
      Serial3.print("4\n");
      setparamsv();
      setparamsh();
    }
    else if(torfarbe == 2)
    {
      //wir haben gelb
      blue = false;
      Serial1.print("4\n");
      Serial3.print("3\n");
      setparamsv();
      setparamsh();
    }
    else if(torfarbe == 3)
    {      
      Serial1.print("39\n");
      Serial3.print("39\n");
      delay(50);
      Serial1.print("5\n");//langsam
      Serial3.print("5\n");
    }
    else if(torfarbe == 4)
    {
      Serial1.print("6\n");//schnell
      Serial3.print("6\n");
      delay(5);
      Serial1.print("35\n");
      Serial3.print("35\n");
      delay(5);
      Serial1.print("37\n");
      Serial3.print("37\n");
      delay(500);
      Serial1.print("36\n");
      Serial3.print("36\n");
    }
    else if(torfarbe == 5)
    {
      setparamsv();
      setparamsh();
    }
    else if(torfarbe == 6)
    {
      Serial1.print("40\n");
      Serial3.print("40\n");
      delay(1);
    }
    else if(torfarbe == 7)
    {
      Serial1.print("41\n");
      Serial3.print("41\n");
      delay(1);
    }
    fahne[0] = (fahne[0] & 3);
   volatile uint16_t spi_paket_cam1   =   serialonet;//serialthree;     // 16389,   hinten
   volatile uint16_t spi_paket_cam2   =   serialthree;//serialthreet;    // 16393  
   volatile uint16_t spi_paket_cam3   =   serialthreet;//serialone;       // 16385,   vorne
   volatile uint16_t spi_paket_cam4   =   serialone;//serialonet;      // 16387
   volatile uint16_t returnData[fahnenlaenge] = {};                           // Es wird ein array erwartet, also erzeugen wir einen mit Länge 4
   volatile uint16_t data[fahnenlaenge] = {};                                 // SPI geht immer in zwei Richtungen, also hier vier Empfangs-Pakete...
    if(fahne[0] == 0)
    {
      returnData[0] = spi_paket_cam1;
    }
    else if(fahne[0] == 1)
    {
      
      returnData[0] = spi_paket_cam2;
    }
    else if(fahne[0] == 2)
    {
      returnData[0] = spi_paket_cam3;
    }
    else if(fahne[0] == 3)
    {
      returnData[0] = spi_paket_cam4;      
    }
    /*returnData[0] = spi_paket_cam1;
    returnData[1] = spi_paket_cam2;
    returnData[2] = spi_paket_cam3;
    returnData[3] = spi_paket_cam4;*/

                                   // Und schreiben die Pakete rein
   
   
   
   SPI_SLAVE.rxtx16(data, returnData, fahnenlaenge);                          // Die gelesenen Pakete wird sofort verworfen
}


void setup() 
{                

  Serial.begin(9600);
  Serial1.begin(115200);
  Serial3.begin(115200);

  inputString.reserve(200);
  // spi setup
  SPI_SLAVE.begin_SLAVE(SCK, MOSI, MISO, CS0); 
  SPI_SLAVE.setCTAR_SLAVE(16, SPI_MODE0);
  NVIC_ENABLE_IRQ(IRQ_SPI0);
Serial.println("Setup...done");
  delay(300);

    setparamsv();
  setparamsh();

  first = true;

} // end of setup()




void loop() 
{
  Serial1.print("1\n");
   stringComplete = false;
  inputString = "";
  while (Serial1.available()) {
    // get the new byte:
    
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      
      inputString.remove(0,1);
      serialone = inputString.toInt();
      
    }
  }
  if(((serialone & 32768) == 32768) || ((serialone & 1) != 1) || (serialone == 0))
      {
        serialone = 1;
      }
      delay(10);
Serial1.print("2\n");
 stringComplete = false;
  inputString = "";
      while (Serial1.available()) {
    // get the new byte:
    
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      
      inputString.remove(0,1);
      serialonet = inputString.toInt();
      
    }
  }
  if(((serialonet & 32768) == 32768) || ((serialonet & 1) != 1) || (serialonet == 0))
      {
        serialonet = 8193;
      }
  
  Serial3.print("1\n");
   stringComplete = false;
  inputString = "";
  while (Serial3.available()) {
    // get the new byte:
    
    char inChar = (char)Serial3.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      inputString.remove(0,1);
      serialthree = inputString.toInt();
    }
  }
  
  if(((serialthree & 32768) == 32768) || ((serialthree & 1) != 1) || (serialthree == 0))
      {
        serialthree = 16385;
      }
delay(10);
      Serial3.print("2\n");
  inputString = "";
  stringComplete = false;
  while (Serial3.available()) {
    // get the new byte:
    
    char inChar = (char)Serial3.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      
      inputString.remove(0,1);
      serialthreet = inputString.toInt();
    }
  }
if(((serialthreet & 32768) == 32768) || ((serialthreet & 1) != 1) || (serialthreet == 0))
      {
        serialthreet = 24577;
      }
  //schreibe();                               //epische Ausgabe - Kommentierung aufheben um es sichtbar zu machen
}

void schreibe()
{
  Serial.print(serialone);
  Serial.print(" (16385) ");  
  Serial.print("  ;  ");
  Serial.print(serialonet);
  Serial.print(" (16387/Tor) ");
  Serial.print("  ;  ");
  Serial.print(serialthree);
  Serial.print(" (16389) ");
  Serial.print("  ;  ");
  Serial.print(serialthreet);
  Serial.print(" (16393/Tor) ");
  Serial.print("  ;  ");
  Serial.println("");
}

void setparamsv()
{
      Serial1.print("7\n");
      Serial1.print(String(lowh) + "\n");
      delay(5);
      Serial1.print("8\n");
      Serial1.print(String(lows) + "\n");
      delay(5);
      Serial1.print("9\n");
      Serial1.print(String(lowv) + "\n");
      delay(5);
      Serial1.print("10\n");
      Serial1.print(String(highh) + "\n");
      delay(5);
      Serial1.print("11\n");
      Serial1.print(String(highs) + "\n");
      delay(5);
      Serial1.print("12\n");
      Serial1.print(String(highv) + "\n");
      delay(5);
      Serial1.print("13\n");
      Serial1.print(String(lowhr) + "\n");
      delay(5);
      Serial1.print("14\n");
      Serial1.print(String(lowsr) + "\n");
      delay(5);
      Serial1.print("15\n");
      Serial1.print(String(lowvr) + "\n");
      delay(5);
      Serial1.print("16\n");
      Serial1.print(String(highhr) + "\n");
      delay(5);
      Serial1.print("17\n");
      Serial1.print(String(highsr) + "\n");
      delay(5);
      Serial1.print("18\n");
      Serial1.print(String(highvr) + "\n");
      delay(5);
      Serial1.print("19\n");
      Serial1.print(String(cambrightness) + "\n");
      delay(5);
      Serial1.print("20\n");
      Serial1.print(String(camhue) + "\n");
      delay(5);
      Serial1.print("21\n");
      Serial1.print(String(lowht) + "\n");
      delay(5);
      Serial1.print("22\n");
      Serial1.print(String(lowst) + "\n");
      delay(5);
      Serial1.print("23\n");
      Serial1.print(String(lowvt) + "\n");
      delay(5);
      Serial1.print("24\n");
      Serial1.print(String(highht) + "\n");
      delay(5);
      Serial1.print("25\n");
      Serial1.print(String(highst) + "\n");
      delay(5);
      Serial1.print("26\n");
      Serial1.print(String(highvt) + "\n");
      delay(5);
      Serial1.print("27\n");
      Serial1.print(String(lowhtg) + "\n");
      delay(5);
      Serial1.print("28\n");
      Serial1.print(String(lowstg) + "\n");
      delay(5);
      Serial1.print("29\n");
      Serial1.print(String(lowvtg) + "\n");
      delay(5);
      Serial1.print("30\n");
      Serial1.print(String(highhtg) + "\n");
      delay(5);
      Serial1.print("31\n");
      Serial1.print(String(highstg) + "\n");
      delay(5);
      Serial1.print("32\n");
      Serial1.print(String(highvtg) + "\n");
      delay(5);
      Serial1.print("34\n");
      delay(5);
      Serial1.print("42\n");
      Serial1.print(String(camcontrast) + "\n");
      delay(5);
}

void setparamsh()
{
      Serial3.print("7\n");
      Serial3.print(String(lowhb) + "\n");
      delay(5);
      Serial3.print("8\n");
      Serial3.print(String(lowsb) + "\n");
      delay(5);
      Serial3.print("9\n");
      Serial3.print(String(lowvb) + "\n");
      delay(5);
      Serial3.print("10\n");
      Serial3.print(String(highhb) + "\n");
      delay(5);
      Serial3.print("11\n");
      Serial3.print(String(highsb) + "\n");
      delay(5);
      Serial3.print("12\n");
      Serial3.print(String(highvb) + "\n");
      delay(5);
      Serial3.print("13\n");
      Serial3.print(String(lowhrb) + "\n");
      delay(5);
      Serial3.print("14\n");
      Serial3.print(String(lowsrb) + "\n");
      delay(5);
      Serial3.print("15\n");
      Serial3.print(String(lowvrb) + "\n");
      delay(5);
      Serial3.print("16\n");
      Serial3.print(String(highhrb) + "\n");
      delay(5);
      Serial3.print("17\n");
      Serial3.print(String(highsrb) + "\n");
      delay(5);
      Serial3.print("18\n");
      Serial3.print(String(highvrb) + "\n");
      delay(5);
      Serial3.print("19\n");
      Serial3.print(String(cambrightnessb) + "\n");
      delay(5);
      Serial3.print("20\n");
      Serial3.print(String(camhueb) + "\n");
      delay(5);
      Serial3.print("21\n");
      Serial3.print(String(lowhtb) + "\n");
      delay(5);
      Serial3.print("22\n");
      Serial3.print(String(lowstb) + "\n");
      delay(5);
      Serial3.print("23\n");
      Serial3.print(String(lowvtb) + "\n");
      delay(5);
      Serial3.print("24\n");
      Serial3.print(String(highhtb) + "\n");
      delay(5);
      Serial3.print("25\n");
      Serial3.print(String(highstb) + "\n");
      delay(5);
      Serial3.print("26\n");
      Serial3.print(String(highvtb) + "\n");
      delay(5);
      Serial3.print("27\n");
      Serial3.print(String(lowhtgb) + "\n");
      delay(5);
      Serial3.print("28\n");
      Serial3.print(String(lowstgb) + "\n");
      delay(5);
      Serial3.print("29\n");
      Serial3.print(String(lowvtgb) + "\n");
      delay(5);
      Serial3.print("30\n");
      Serial3.print(String(highhtgb) + "\n");
      delay(5);
      Serial3.print("31\n");
      Serial3.print(String(highstgb) + "\n");
      delay(5);
      Serial3.print("32\n");
      Serial3.print(String(highvtgb) + "\n");
      delay(5);
      Serial3.print("33\n");
      delay(5);
      Serial1.print("42\n");
      Serial1.print(String(camcontrastb) + "\n");
      delay(5);
}

