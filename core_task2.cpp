#include "core_task2.h"
#include "shared_data.h"
#include "esp_task_wdt.h" //watchdog

int get_k(int i)
{
  return i*2;
}

void Task2code(void *pvParameters) {
  while (1) {
    esp_task_wdt_reset();
    Serial.println("Task 2");
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      Serial.printf("Counter: %d, LED: %s\n", 
        sharedData.counter, 
        sharedData.ledState ? "ON" : "OFF");
        Serial.println(sharedData.name);
        Serial.println(get_k(sharedData.counter));
      xSemaphoreGive(dataMutex);
    }
    delay(500);
  }
}