/* Arduino I2C Multiplexer TCA9548A with two I2C Oled

Created by Yvan / https://Brainy-Bits.com

This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

#include <U8glib.h>   // Library for OlED display https://github.com/olikraus/u8glib/

#include <I2CMux.h>    // Library for TCA9548A by Alvaro Ferran https://github.com/alvaroferran/I2C-multiplexer
#include <Wire.h>

#define I2CMulti_Addr 0x70  // Address of the I2C Multiplexer board

#define change_switch 10    // Tact switch used to select which OLED to use
#define increase_count_switch 11  // Tact switch used to increase the counter

int selected_oled=2;  // Use to keep track of which I2C channel is selected on the Multiplexer

I2CMux I2CMulti(I2CMulti_Addr);  // Init of the multiplexer library

int Oled1c=0;   // Counter variable for OLED #1
int Oled2c=0;   // Counter variable for OLED #2


U8GLIB_SSD1306_128X32 First_Oled(U8G_I2C_OPT_FAST);  // Init of the OLED #1 using I2C
U8GLIB_SSD1306_128X32 Second_Oled(U8G_I2C_OPT_FAST);  // Init of the OLED #2 using I2C

void setup() {

  Wire.begin();
  
// Using Input Pullups for the tact switches
  pinMode(change_switch, INPUT_PULLUP);
  pinMode(increase_count_switch, INPUT_PULLUP);

// Rotate screen 180 degrees on OLED, uncomment if required
  //u8gDHT.setRot180();

// Select font to use
  First_Oled.setFont(u8g_font_unifont);
  Second_Oled.setFont(u8g_font_unifont);
  
  I2CMulti.switchToBus(2);  // Switch to multiplexer i2c channel #2
  First_Oled.setContrast(100);  // Set oled contrast at startup

// Put stuff on OLED at startup    
  First_Oled.firstPage(); 
    do {
        First_Oled.drawStr( 88, 10, "|");  
        First_Oled.drawStr( 0, 32, "Oled #");
        First_Oled.setPrintPos(50, 32);
        First_Oled.print(Oled1c, 1);
        First_Oled.drawStr( 88, 32, "|");
      } 
    while( First_Oled.nextPage() );

  I2CMulti.switchToBus(7);
  Second_Oled.setContrast(0);
  
  Second_Oled.firstPage(); 
    do {
        Second_Oled.drawStr( 88, 10, "|");  
        Second_Oled.drawStr( 0, 32, "Oled #");
        Second_Oled.setPrintPos(50, 32);
        Second_Oled.print(Oled2c, 1);
        Second_Oled.drawStr( 88, 32, "|");
      }
    while( Second_Oled.nextPage() );

  I2CMulti.switchToBus(2); // Select i2c multiplexer channel to #2            
  
}


void loop() {

// if tact switch clicked change i2c channel
  if (digitalRead(change_switch) == LOW) {
    delay(200);
    
//change contrast and channel
  if (selected_oled == 2) {
      First_Oled.setContrast(0);
      I2CMulti.switchToBus(7);
      Second_Oled.setContrast(100);
      selected_oled=7;
    } else {
        Second_Oled.setContrast(0);
        I2CMulti.switchToBus(2);
        First_Oled.setContrast(100);
        selected_oled=2;       
    }
  }

// if tact switch clicked increase counter on selected OLED
  if (digitalRead(increase_count_switch) == LOW) {
    delay(50);
    if (selected_oled == 2) {
      Oled1c++;
      First_Oled.firstPage(); 
      do {
          First_Oled.drawStr( 88, 10, "|");  
          First_Oled.drawStr( 0, 32, "Oled #");
          First_Oled.setPrintPos(50, 32);
          First_Oled.print(Oled1c, 1);
          First_Oled.drawStr( 88, 32, "|");
        } while( First_Oled.nextPage() );
    } else {
      Oled2c++;
      Second_Oled.firstPage(); 
      do {
          Second_Oled.drawStr( 88, 10, "|");  
          Second_Oled.drawStr( 0, 32, "Oled #");
          Second_Oled.setPrintPos(50, 32);
          Second_Oled.print(Oled2c, 1);
          Second_Oled.drawStr( 88, 32, "|");
        } while( Second_Oled.nextPage() );
    }
  }
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
