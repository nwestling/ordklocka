//Niclas Westling, 2016

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include "RTClib.h"

#define PIN 6   // LED strand data

//IF CLOCK TIME IS WRONG: RUN FILE->EXAMPLE->DS1307RTC->SET TIME

RTC_DS3231 rtc;

// matrix of 11 - 16 bit ints (shorts) for displaying the leds
uint16_t mask[11];


  //////////////////////////////////SET COLORS/////////////////////////////

  //WORDCLOCK COLORS
  int wordred   = 80;
  int wordblue  = 255;
  int wordgreen = 160;

  //CELEBRATION COLORS
  int celebred   = 200;
  int celebblue  = 0;
  int celebgreen = 0;

//////////////////////////////////////////////////////////////////////////////

  long mytimeyear;
  int mytimemonth;
  int mytimeday;
  int mytimehr;
  int mytimemin;
  int mytimesec;
  int mytimedayofweek;
 
 // int mode = 0;
 // int lastState = LOW;   // the previous reading from the input pin

  
// 16 bit mask
#define phraseKLOCKAN_AR mask[0]  |= 0xFEC0
#define phraseFEM        mask[1]  |= 0x7000
#define phraseTIO        mask[1]  |= 0xE00
#define phraseKVART      mask[2]  |= 0xF800
#define phraseTJUGO      mask[2]  |= 0xF80
#define phraseI_HALV     mask[3]  |= 0x83C0
#define phraseHALV       mask[3]  |= 0x3C0
#define phraseOVER_HALV  mask[3]  |= 0x7BC0
#define phraseOVER       mask[3]  |= 0x7800
#define phraseI          mask[3]  |= 0x8000
#define hourETT          mask[6]  |= 0xE000
#define hourTVA          mask[8]  |= 0x1C00
#define hourTRE          mask[6]  |= 0x3800
#define hourFYRA         mask[6]  |= 0x3C0
#define hourFEM          mask[4]  |= 0x380
#define hourSEX          mask[5]  |= 0x700
#define hourSJU          mask[9]  |= 0xE000
#define hourATTA         mask[8]  |= 0x780
#define hourNIO          mask[5]  |= 0x700
#define hourTIO          mask[9]  |= 0x1C0
#define hourELVA         mask[7]  |= 0x3C0
#define hourTOLV         mask[9]  |= 0x1E00

#define phraseFEMTIO_AR  mask[1]  |= 0x7EC0
#define phraseGRETA      mask[4]  |= 0x8000, mask[5]  |= 0x8000, mask[6]  |= 0x8000, mask[7]  |= 0x8000, mask[8]  |= 0x8000
#define phrasePETER      mask[4]  |= 0x2000, mask[5]  |= 0x2000, mask[6]  |= 0x2000, mask[7]  |= 0x2000, mask[8]  |= 0x2000
#define phraseANNA       mask[4]  |= 0x400, mask[5]  |= 0x400, mask[6]  |= 0x400, mask[7]  |= 0x400
#define phraseMARTA      mask[4]  |= 0x80, mask[5]  |= 0x80, mask[6]  |= 0x80, mask[7]  |= 0x80, mask[8]  |= 0x80
#define phrasePETER_ANNA mask[4]  |= 0x2400, mask[5]  |= 0x2400, mask[6]  |= 0x2400, mask[7]  |= 0x2400, mask[8]  |= 0x2000



// define pins
#define NEOPIN 6

// Parameter 1 = number of pixels in matrix
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel matrix = Adafruit_NeoPixel(64, NEOPIN, NEO_GRB + NEO_KHZ800);

// configure for 10x1 neopixel matrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(10, 10, NEOPIN,
      NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
      NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
      NEO_GRB         + NEO_KHZ800);

void setup() {
   matrix.begin();
   Serial.begin(115200);  //Begin serial communcation 


   // This info pulled from RTClib.h
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC"); 
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!"); 
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}


void applyMask() {

   for (byte row = 0; row < 10; row++) 
   {
      for (byte col = 0; col < 16; col++) 
      {
         boolean masker = bitRead(mask[row], 15 - col); // bitread is backwards because bitRead reads rightmost digits first. could have defined the word masks differently
         switch (masker) 
         {
            case 0:  // this LED should not be lit
               matrix.drawPixel(col, row, 0);
               break;
            case 1:
              // word_mode color set
               matrix.drawPixel(col, row, matrix.Color(wordred, wordgreen, wordblue));
               break;
         }
      }
      // reset mask for next time
      mask[row] = 0;
   }
   
   matrix.show(); // show it!
}


void applyCelebrationMask() {

   for (byte row = 0; row < 10; row++) 
   {
      for (byte col = 0; col < 16; col++) 
      {
         boolean masker = bitRead(mask[row], 15 - col); // bitread is backwards because bitRead reads rightmost digits first. could have defined the word masks differently
         switch (masker) 
         {
            case 0:
               matrix.drawPixel(col, row, 0);
               break;
            case 1:
               // word_mode color set
               matrix.drawPixel(col, row, matrix.Color(celebred, celebgreen, celebblue));
               break;
         }
      }
      // reset mask for next time
      mask[row] = 0;
   }


   matrix.show(); // show it!
}

void displayCelebration() {
    applyCelebrationMask(); 
}

void displayWords() {

   phraseKLOCKAN_AR;

  //calculate minutes on the hour
    if(mytimemin>57 && mytimemin<3){
    }
    // do nothing, no minutes it's on the hour
    
    if(mytimemin>2 && mytimemin<8){
      phraseFEM;
      phraseOVER;
    }
    
    if(mytimemin>7 && mytimemin<13){
      phraseTIO;
      phraseOVER;
    }
    if(mytimemin>12 && mytimemin<18){
      phraseKVART;
      phraseOVER;
    }
    if(mytimemin>17 && mytimemin<23){
      phraseTJUGO;
      phraseOVER;
    }
    if(mytimemin>22 && mytimemin<28){
      phraseFEM;
      phraseI_HALV;
    }
    if(mytimemin>27 && mytimemin<33){
      phraseHALV;
    }
    if(mytimemin>32 && mytimemin<38){
      phraseFEM;
      phraseOVER_HALV;
    }
    if(mytimemin>37 && mytimemin<43){
      phraseTJUGO;
      phraseI;
    }
    if(mytimemin>42 && mytimemin<48){
      phraseKVART;
      phraseI;
    }    
    if(mytimemin>47 && mytimemin<53){
      phraseTIO;
      phraseI;
    }
    if(mytimemin>52 && mytimemin<58){
      phraseFEM;
      phraseI;
    }


  // Calculate hour & oclocks
  if(mytimehr==1){
    if(mytimemin>22){
      hourTVA;
    }
    else
    {
      hourETT;
    }
  }
  if(mytimehr==2){
   if(mytimemin>22){
      hourTRE;
    }
    else
    {
      hourTVA;
    }
  }
    if(mytimehr==3){
     if(mytimemin>22){
      hourFYRA;
    }
    else
    {
      hourTRE;
    }
  }
  if(mytimehr==4){
    if(mytimemin>22){
      hourFEM;
    }
    else
    {
      hourFYRA;
    }
  }
  if(mytimehr==5){
    if(mytimemin>22){
      hourSEX;
    }
    else
    {
      hourFEM;
    }
  }
  if(mytimehr==6){
    if(mytimemin>22){
      hourSJU;
    }
    else
    {
      hourSEX;
    }
  }
  if(mytimehr==7){
    if(mytimemin>22){
      hourATTA;
    }
    else
    {
      hourSJU;
    }
  }
  if(mytimehr==8){
    if(mytimemin>22){
      hourNIO;
    }
    else
    {
      hourATTA;
    }
  }
  if(mytimehr==9){
    if(mytimemin>22){
      hourTIO;
    }
    else
    {
      hourNIO;
    }
  }
  if(mytimehr==10){
    if(mytimemin>22){
      hourELVA;
    }
    else
    {
      hourTIO;
    }
  }
  if(mytimehr==11){
    if(mytimemin>22){
      hourTOLV;
    }
    else
    {
      hourELVA;
    }
  }
  if(mytimehr==12){
    if(mytimemin>22){
      hourETT;
    }
    else
    {
      hourTOLV;
    }
  }
      if(mytimehr==13){
    if(mytimemin>22){
      hourTVA;
    }
    else
    {
      hourETT;
    }
  }
  if(mytimehr==14){
    if(mytimemin>22){
      hourTRE;
    }
    else
    {
      hourTVA;
    }
  }
    if(mytimehr==15){
    if(mytimemin>22){
      hourFYRA;
    }
    else
    {
      hourTRE;
    }
  }
  if(mytimehr==16){
    if(mytimemin>22){
      hourFEM;
    }
    else
    {
      hourFYRA;
    }
  }
  if(mytimehr==17){
    if(mytimemin>22){
      hourSEX;
    }
    else
    {
      hourFEM;
    }
  }
  if(mytimehr==18){
    if(mytimemin>22){
      hourSJU;
    }
    else
    {
      hourSEX;
    }
  }
  if(mytimehr==19){
    if(mytimemin>22){
      hourATTA;
    }
    else
    {
      hourSJU;
    }
  }
  if(mytimehr==20){
    if(mytimemin>22){
      hourNIO;
    }
    else
    {
      hourATTA;
    }
  }
  if(mytimehr==21){
    if(mytimemin>22){
      hourTIO;
    }
    else
    {
      hourNIO;
    }
  }
  if(mytimehr==22){
    if(mytimemin>22){
      hourELVA;
    }
    else
    {
      hourTIO;
    }
  }
  if(mytimehr==23){
    if(mytimemin>22){
      hourTOLV;
    }
    else
    {
      hourELVA;
    }
  }
  if(mytimehr==0){
    if(mytimemin>22){
      hourETT;
    }
    else
    {
      hourTOLV;
    }
  }
  applyMask(); 
}

boolean celebrationDate() {
   if ((mytimemonth == 9) and (mytimeday == 9)) {
     //Peters birthday
     phrasePETER;
     return true;
   }
   else if ((mytimemonth == 12) and (mytimeday == 8)) {
     //Annas birthday
     phraseANNA;
     return true;
     }
     else if ((mytimemonth == 11) and (mytimeday == 2)) {
     // Gretas birthday
     phraseGRETA;
     return true;
      }
      else if ((mytimemonth == 5) and (mytimeday == 2)) {
     // Martas birthday
     phraseMARTA;
     return true;
      }
      else if ((mytimemonth == 9) and (mytimeday == 17)) {
     // Wedding day
     phrasePETER_ANNA;
     return true;
      }
      else if ((mytimemonth == 2) and (mytimeday == 14)) {
     // Valentines day
     phrasePETER_ANNA;
     return true;
      }
      else {
      return false;
      }
}

void loop() {
     
    DateTime now = rtc.now();

    mytimeyear=now.year();
    mytimemonth=now.month();
    mytimeday=now.day();
    mytimehr=now.hour();
    mytimemin=now.minute();
    mytimesec=now.second();

//calculate if daylight saving should be applied
//first check what day of week it is
    mytimedayofweek=(mytimeday + int(2.6 * (((mytimemonth + 12 - 3) % 12) + 1) - 0.2) - 40 + (mytimemonth < 3 ? mytimeyear-1 : mytimeyear) + int((mytimemonth < 3 ? mytimeyear-1 : mytimeyear)/4) + 5) % 7;
//now apply business rules for swedish daylight saving rules. last sunday in March and last sunday in October 2am is when the change happens.
  if ((mytimemonth  >  3 && mytimemonth < 10 ) || //April to September definitely DST
      (mytimemonth ==  3 && mytimeday >= 8 && mytimedayofweek == 0 && mytimehr >= 2) ||  // DST starts last Sunday of March;  2am
      (mytimemonth == 10 && mytimeday <  8 && mytimedayofweek >  0) ||
      (mytimemonth == 10 && mytimeday <  8 && mytimedayofweek == 0 && mytimehr < 2)) {   // DST ends last Sunday of November; 2am
    mytimehr=mytimehr+1;
    if (mytimehr==24) {mytimehr=0; } 
  }


// if minute is odd value and (there is a celebration to display) then displayCelebration else displayWords 
    if ((mytimemin % 2 != 0 ) && (celebrationDate())) {
        displayCelebration();
      }
      else
      {
      displayWords();
      }

}









