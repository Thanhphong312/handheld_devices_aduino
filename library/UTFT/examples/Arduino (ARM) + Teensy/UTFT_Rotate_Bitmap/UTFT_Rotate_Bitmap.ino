// UTFT_Rotate_Bitmap 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ILI9341_16,38,39,40,41);

extern unsigned short power_icon[0x654];

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setColor(0, 0, 0);
}

void loop()
{
    
  myGLCD.drawBitmap (10, 10, 64, 64, power_icon, i, 32, 32);
    
}

