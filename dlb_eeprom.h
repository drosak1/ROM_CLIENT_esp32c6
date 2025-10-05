#ifndef DLB_EEPROM_h
#define DLB_EEPROM_h

#include <EEPROM.h>
#include <Arduino.h>

#define EEPROM_SIZE 512

/*
 * EEPROM.length() -> max 512 byte
 * DLB_EPR = dlb_eeprom();
 * DLB_EPR.save("haslo do wifi",0,100);
 * haslo = DLB_EPR.read(0,100);
 * 
 */

class dlb_eeprom
{
public:
    dlb_eeprom(bool serial_);
    void save(String text, uint16_t addr, uint16_t len);
    String read(uint16_t addr, uint16_t len);
    void save_char(char znak, uint16_t addr);
    char read_char(uint16_t addr);
    bool znak(char myChar);
protected:

private:
  bool Serial_ = false;

};

#endif