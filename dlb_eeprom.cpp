#include "dlb_eeprom.h"

dlb_eeprom::dlb_eeprom(bool serial_)
{
  this->Serial_ = serial_;
}

void dlb_eeprom::save(String text, uint16_t addr, uint16_t len)
{
  uint16_t i = 0;
  
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("failed to initialize EEPROM");
    delay(5000);
  }
  delay(10);
  //clear
  for (i = addr; i < (addr+len) ; i++) {
    EEPROM.write(i, 0);
  }

  //save
  for (i = 0; i < text.length(); i++)
  {
    EEPROM.write((addr+i), text[i]);
    if(this->Serial_) { Serial.print("EEPROM save addr -> "); Serial.print(addr+i); Serial.print(" value -> "); Serial.println(text[i]); }
  } 
  
  EEPROM.commit(); //zapis eepromm
  EEPROM.end(); // zwolnienie pamieci RAM
}

void dlb_eeprom::save_char(char znak, uint16_t addr)
{
  EEPROM.begin(512);
  delay(10);
  EEPROM.write(addr, znak);
  
  if(this->Serial_) { Serial.print("EEPROM save addr -> "); Serial.print(addr); Serial.print(" value -> "); Serial.println(znak); }
  
  EEPROM.commit(); //zapis eepromm
  EEPROM.end(); // zwolnienie pamieci RAM
}

char dlb_eeprom::read_char(uint16_t addr)
{
  char pom;
  EEPROM.begin(512);
  delay(10);
  pom = char(EEPROM.read(addr));
  if(this->Serial_) { Serial.print("EEPROM read addr -> "); Serial.print(addr); Serial.print(" value -> "); Serial.println(pom); }
  return pom;
}

String dlb_eeprom::read(uint16_t addr, uint16_t len)
{
  String text = "";
  uint16_t i = 0;
  char pom;
  
  EEPROM.begin(512);
  delay(10);
  if(this->Serial_) Serial.println("Startup Reading EEPROM");

  for (i = addr; i < (addr+len); ++i)
    {
      pom = char(EEPROM.read(i));
      if(this->Serial_) { Serial.print("EEPROM read addr -> "); Serial.print(i); Serial.print(" value -> "); Serial.println(pom); }
      if (this->znak(pom)) text += pom;
    }
  text=text.c_str();
  return text;
}


bool dlb_eeprom::znak(char myChar)
{
    if (isAscii(myChar)) { // tests if myChar isa letter or a number
    //Serial.println(" The char is OK");
    return true;
    }
    else {
    //Serial.println(" The char is NOK");
    return false;
    }
}