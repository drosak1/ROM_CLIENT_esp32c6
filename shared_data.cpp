#include "shared_data.h"

SharedData sharedData = {0, false, "xx"};
SemaphoreHandle_t dataMutex = nullptr;

void initSharedData() {
    dataMutex = xSemaphoreCreateMutex();
}