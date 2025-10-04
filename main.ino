/*
  Visual 
*/

#include "core_task1.h"
#include "core_task2.h"
#include "serial_task.h"
#include "shared_data.h"
#include "esp_task_wdt.h" //watchdog
#include <Arduino.h>

#define BUTTON_PIN 4  // GPIO4
volatile bool buttonPressed = false;  // musi być volatile! aby kompilator nie optymalizował ich "na zbyt mądrze"
static unsigned long lastPrint = 0;


TaskHandle_t Task1;
TaskHandle_t Task2;

TaskHandle_t SerialTaskHandle = NULL;

void IRAM_ATTR handleInterrupt() {
  buttonPressed = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // Czekaj na połączenie

  Serial.println("System uruchomiony. Czekam ...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
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
}

void loop() {
  esp_task_wdt_reset();

  if (millis() - lastPrint > 2000) {
    Serial.println("loop() działa niezależnie...");
    lastPrint = millis();
  }

  delay(1000);
}