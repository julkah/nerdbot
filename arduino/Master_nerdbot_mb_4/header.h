#include <i2c_t3.h>

/*
 * Achtung, dieser header ist das Ergebnis jahrelanger Entwicklung
 * Innerhalb der header.h sollte eigentlich nichts geÃ¤ndert werden mÃ¼ssen.
 * Falls es doch jemandem erscheint, als mÃ¼sse hier etwas geÃ¤ndert werden, darf dies nur 
 * nach RÃ¼cksprache mit dem Coach erfolgen, da dann alle anderen Teams Ã¼ber die Ã„nderungen informiert werden mÃ¼ssen.
 * 
 * Das Verhalten des Roboters wird ausschlieÃŸlich in der handeln.h programmiert!!!
 */

#define TASTE_STOP  255-128
#define TASTE_START 255-64
#define I2C_AUTO_RETRY true

#define TASTE_L_XOOO 255-32-16  // linker Shift, ganz links
#define TASTE_L_OXOO 255-32-8   // linker Shift, 2. links
#define TASTE_L_OOXO 255-32-4   // linker Shift, 2. rechts
#define TASTE_L_OOOX 255-32-2   // linker Shift, ganz rechts

#define TASTE_R_XOOO 255-1-16   // rechter Shift, ganz links
#define TASTE_R_OXOO 255-1-8    // rechter Shift, 2. links
#define TASTE_R_OOXO 255-1-4    // rechter Shift, 2. rechts
#define TASTE_R_OOOX 255-1-2    // rechter Shift, ganz rechts

#define LED1    1         // LED hinten
#define LED2    2 
#define LED3    4
#define LED4    8
#define GELB    16
#define BLAU    32
#define BLINK1  64
#define BLINK2  128

#define L_GRUEN 1         // LED vorne
#define L_GELB  32  // prÃ¼fen!
#define R_GRUEN 16
#define R_GELB  64  // prÃ¼fen!
#define ROT     128

#define LED_SPI0    4
#define LED_SPI1    2
#define LED_SPI2    8

#include <t3spi.h>

//Initialize T3SPI class as SPI_MASTER
T3SPI SPI_MASTER;

//The number of integers per data packet
#define farbenlaenge 1
#define ir_dataLength 1
#define boden_dataLength 1
#define CS10        0x0A

#define CMPS11_ADDRESS 0x60       // cmps11 
#define ANGLE_8  1

#define AD_ADDRESS 0x64           // ad-Port fÃ¼r Analogwerte wie Batterie, Lichtsensor, ...
#define AD_CONFIG   0b00010111
#define AD_SETUP    0b11011010

#define TASTER_ADDRESS      0x20          //  I2C Adresse Taster-Chip
#define LED_HINTEN_ADDRESS  0x21          //  I2C Adresse LED1-Chip
#define LED_VORNE_ADDRESS   0x22          //  I2C Adresse LED2-Chip

#define PWM2    4
#define ENA2    2
#define DIR2    3

#define PWM1    5
#define ENA1    16
#define DIR1    17

#define PWM3    6
#define ENA3    22
#define DIR3    23

#define KICKER    15
#define DRIBBLER  14

byte led_hinten;  // Datenpaket fÃ¼r 8 LEDs
byte led_vorne;   // Datenpaket fÃ¼r 8 LEDs

byte spi_fehler;
elapsedMicros zeit;
elapsedMillis nodel;
elapsedMillis kickzeit;
int kompass;
int head;



static int spee = 50;
static int kk = 10;

int zsm;
byte tastbyte;
boolean hatball=false;

int blue = 0;         // 3 Modi: Anfang (nichts ausgewÃ¤hlt, 0), blau haben (1), gelb haben (2)
volatile uint16_t farbe[farbenlaenge] = {};
bool aufnahme = false;
bool band = false;

//Initialize the arrays for SPI data
volatile uint16_t ir_returnData[ir_dataLength] = {};
volatile uint16_t boden_returnData[boden_dataLength] = {};
volatile uint16_t ir_data[ir_dataLength] = {}; 
volatile uint16_t boden_data[boden_dataLength] = {};

volatile uint16_t cout;

int modus;    // sagt, in welchem Modus der bot sich befindet
int spielart; // Wie soll der Roboter spielen?  z.B. Offensiv, defensiv, test1, test2, ...

int camsignv, xposv, distv, onsv, camsignh, xposh, disth, onsh, ssv; 
int camsignvt, xposvt, distvt, onsvt, camsignht, xposht, distht, onsht; 
int torfarbe = 0; 
int ballrichtung = -1;
int ballentfernung = 0;
int vtorentfernung = 0;
int htorentfernung = 0;
int vtorrichtung = -1;
int htorrichtung = -1;
int weisswinkel;
int schwarzwinkel;
int weissabstand;
int schwarzabstand;
int sieht_weiss;
int sieht_schwarz;
int failcounter = 0;

int ad_batt;
int ad_lichtschranke;
int ad_2;
int ad_3;

boolean odroid1 = false;
bool odroid2 = false;

String inputString = "";         // a string to hold incoming data
String teststring = "";
String blau = "";
static String normalstring = "$r";
boolean stringComplete = false;  // whether the string is complete
boolean testerg = false;
boolean fertiginput = false;
bool ss = false;
bool fast = true;
bool near = true;
bool licht = false;

int komp_alt;
int komp_av;

int abst_alt;

elapsedMillis bluetime;
elapsedMillis battime;
elapsedMillis aufnahmetime;
int odroidtime1 = 0;
int odroidtime2 = 0;

int maxxon=1; // Flag, ob Maxxon (1) oder pololu (2)

