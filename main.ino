/*
  Visual 
*/
#include <Arduino.h>
#include <esp_sleep.h>
#include <Time.h> //watchdog and timer
#include <Ticker.h>
#include <Wire.h>

#include "core_task1.h"
#include "core_task2.h"
#include "serial_task.h"
#include "shared_data.h"
#include "esp_task_wdt.h" //watchdog
#include "dlb_eeprom.h"
#define BUTTON_PIN    4  // GPIO4
#define BUTTON_3 GPIO_NUM_3
#define WAKEUP_TIME_SEC 10        // czas wybudzenia w sekundach

volatile bool buttonPressed = false;  // musi być volatile! aby kompilator nie optymalizował ich "na zbyt mądrze"
static unsigned long lastPrint = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

TaskHandle_t SerialTaskHandle = NULL;

/* -------------------------------------------------------------------------------------- Definicja obiektow */
dlb_eeprom dlb_eeprom_obj(true);

/* -------------------------------------------------------------------------------------- Zmienne konfiguracyjne */
// String user_name ="";
// String wifiSSID = "";
// String wifiPassword = "";
// String server_name = "";

void IRAM_ATTR handleInterrupt() {
  buttonPressed = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // Czekaj na połączenie
  delay(500);

  Serial.println("System uruchomiony. Czekam ...");

  sharedData.wifiSSID = dlb_eeprom_obj.read(0, 22);
  sharedData.wifiPassword = dlb_eeprom_obj.read(22, 22);
  sharedData.user_name = dlb_eeprom_obj.read(44, 20);
  sharedData.server_name = dlb_eeprom_obj.read(64, 20);

  /* -------------------------------------------------------------------------------------- Sprawdź, co obudziło ESP */
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("💤 Wybudzenie przez TIMER");
      break;
    case ESP_SLEEP_WAKEUP_EXT1:
      Serial.println("💤 Wybudzenie przez PRZYCISK (EXT1)");
      break;
    default:
      Serial.println("🔄 Uruchomienie po resecie / power-on");
      break;
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT); // używamy pullup na zewnatrz !!!

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, CHANGE);

  initSharedData();
  // Tworzenie tasków na różnych rdzeniach
  xTaskCreate(Task1code, "Task1", 10000, NULL, 1, &Task1); //core1
  xTaskCreate(Task2code, "Task2", 10000, NULL, 1, &Task2); //core2

  xTaskCreate(
    SerialReceiverTask,    // funkcja
    "Serial Task",         // nazwa
    1024,                  // rozmiar stosu
    NULL,                  // argumenty
    1,                     // priorytet
    &SerialTaskHandle      // uchwyt
  );

  sharedData.name = "Dawid";

  // Inicjalizacja WDT z timeoutem 3 sekundy
  esp_task_wdt_config_t twdt_config = {
    .timeout_ms = 3000,                               // Timeout w milisekundach
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // monitoruj oba rdzenie (jeśli dostępne)
    .trigger_panic = true                             // automatyczny reset przy timeout
  };

  esp_task_wdt_init(&twdt_config);

  // Rejestruj wszystkie taski wraz z loop()
  esp_task_wdt_add(NULL);  // loop()
  esp_task_wdt_add(Task1); // Task1
  esp_task_wdt_add(Task2); // Task2

  // Ustawienie timera budzenia
  //esp_sleep_enable_timer_wakeup(60 * 1000000); // 60 sekund

  // Maska pinu 9
  uint64_t pinMask = 1ULL << BUTTON_3;

  // Wybudzenie: przycisk (stan niski) lub timer (10 sekund)
  esp_sleep_enable_ext1_wakeup(pinMask, ESP_EXT1_WAKEUP_ALL_LOW);
  //esp_sleep_enable_timer_wakeup(WAKEUP_TIME_SEC * 1000000);

  // Start deep sleep
  esp_task_wdt_deinit();  // wyłącza watchdog (nie jest potrzebny przy deep sleep)
  esp_light_sleep_start();
  esp_task_wdt_reset();
}

void loop() {
  esp_task_wdt_reset();

  if (millis() - lastPrint > 2000) {
    Serial.println("loop() działa niezależnie...");
    lastPrint = millis();
  }

  if(buttonPressed){
    buttonPressed = false;
    Serial.println("buttonPressed");
  }

  delay(1000);
}