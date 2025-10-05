#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <Arduino.h>
#include <freertos/semphr.h>

// Dane współdzielone między taskami
struct SharedData {
    int counter;
    bool ledState;
    String name;
    //z EEPROM
    String user_name;
    String wifiSSID;
    String wifiPassword;
    String server_name;
};

// Inicjalizacja zmiennych i mutexa
extern SharedData sharedData;
extern SemaphoreHandle_t dataMutex;

void initSharedData();

#endif