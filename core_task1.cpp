#include "core_task1.h"
#include "shared_data.h"
#include "esp_task_wdt.h" //watchdog

void Task1code(void *pvParameters) {
    pinMode(LED_BUILTIN, OUTPUT);
    while (1) {
        esp_task_wdt_reset();
        //Serial.println("Task 1");
        if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
            sharedData.counter++;
            sharedData.ledState = !sharedData.ledState;
            digitalWrite(LED_BUILTIN, sharedData.ledState);
            xSemaphoreGive(dataMutex);
        }
        delay(100);
    }
}