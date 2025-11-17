#include <Arduino.h>
#include <U8g2lib.h>
#include "INA228.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 22, 21, U8X8_PIN_NONE);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
INA228 INA(0x40);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin();

  Serial.println("Init System");

  u8g2.begin();
  u8g2.setContrast(10);
  u8g2.clearBuffer();					// clear the internal memory

  u8g2.setDrawColor(1);

  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,10,"Init System");	// write something to the internal memory

  if (!INA.begin() )
  {
    Serial.println("Could not connect To INA228. Fix and Reboot");
    u8g2.drawStr(0,30,"INA228 not found");
  }
  else {
    u8g2.drawStr(0,30,"INA228 found");

    INA.setMaxCurrentShunt(1, 0.333);
    INA.setAccumulation(1);  //  clear registers?
  }

  u8g2.sendBuffer();	
  delay(5000); 
}

void loop() {
  char anLine[20];
  float bus, shunt, current, power, temp, energy, charge;
  int lineHeight = 12;

  // put your main code here, to run repeatedly:
  Serial.println("Hello World!");
  delay(2000);

  //Serial.println("\nVBUS\tVSHUNT\tCURRENT\tPOWER\tTEMP");
  Serial.println("\nVBUS\tVSHUNT\tCURRENT\tPOWER\tTEMP\tJOULE\tCOULOMB");
  for (int i = 0; i < 20; i++)
  {
    bus = INA.getBusVoltage(); 
    shunt = INA.getShuntMilliVolt(); 
    current = INA.getMilliAmpere(); 
    power = INA.getMilliWatt(); 
    temp = INA.getTemperature(); 
    energy = INA.getEnergy(); 
    charge = INA.getCharge();


    Serial.print(bus, 3);
    Serial.print("\t");
    Serial.print(shunt, 3);
    Serial.print("\t");
    Serial.print(current, 3);
    Serial.print("\t");
    Serial.print(power, 3);
    Serial.print("\t");
    Serial.print(temp, 3);
    Serial.print("\t");
    Serial.print(energy, 8);  //  how many decimals valid
    Serial.print("\t");
    Serial.print(charge, 8);
    Serial.println();

    sprintf(anLine, "i %d", i);

    u8g2.clearBuffer();	
    sprintf(anLine, "bus %.3fV", bus);
    u8g2.drawStr(0,13, anLine);

    sprintf(anLine, "shunt %.3fmV", shunt);
    u8g2.drawStr(0,24, anLine);

    sprintf(anLine, "current %.3fmA", current);
    u8g2.drawStr(0,35, anLine);

    sprintf(anLine, "power %.3fmW", power);
    u8g2.drawStr(0,46, anLine);

    sprintf(anLine, "temp %.3f°C", temp);
    u8g2.drawStr(0,57, anLine);

    u8g2.sendBuffer();

    delay(1000);
  }
}
