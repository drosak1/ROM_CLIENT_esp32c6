#include "serial_task.h"
#include "Arduino.h"

void SerialReceiverTask(void *pvParameters) {
    String inputBuffer = "";

    for (;;) { 
        Serial.print("*");
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
        inputBuffer.trim();
        Serial.print("Odebrano linijkę: ");
        Serial.println(inputBuffer);
        inputBuffer = "";  // wyczyść bufor
        } else {
        inputBuffer += c;
        }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // małe opóźnienie, nie blokuj CPU
    }
}