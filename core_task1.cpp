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
            digitalWrite(GPIO_NUM_2,HIGH);
            digitalWrite(GPIO_NUM_8,HIGH);
            digitalWrite(GPIO_NUM_9,HIGH);
            digitalWrite(GPIO_NUM_10,HIGH);
            digitalWrite(GPIO_NUM_11,HIGH);
            digitalWrite(GPIO_NUM_16,HIGH);
            digitalWrite(GPIO_NUM_17,HIGH);
            digitalWrite(GPIO_NUM_18,HIGH);
            digitalWrite(GPIO_NUM_19,HIGH);
            digitalWrite(GPIO_NUM_20,HIGH);
            digitalWrite(GPIO_NUM_21,HIGH);
            delay(100);
            digitalWrite(GPIO_NUM_2,LOW);
            digitalWrite(GPIO_NUM_8,LOW);
            digitalWrite(GPIO_NUM_9,LOW);
            digitalWrite(GPIO_NUM_10,LOW);
            digitalWrite(GPIO_NUM_11,LOW);
            digitalWrite(GPIO_NUM_16,LOW);
            digitalWrite(GPIO_NUM_17,LOW);
            digitalWrite(GPIO_NUM_18,LOW);
            digitalWrite(GPIO_NUM_19,LOW);
            digitalWrite(GPIO_NUM_20,LOW);
            digitalWrite(GPIO_NUM_21,LOW);
            delay(100);

            xSemaphoreGive(dataMutex);
        }
        delay(100);
    }
}