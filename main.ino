/*
  Visual 
  PIN_1   GND
  PIN_2   3,3V
  PIN_3   EN (RST)
  PIN_4   ANALOG_4 (IO_4)
  PIN_5   <>
  PIN_6
  PIN_7
  PIN_8
  PIN_9   ANALOG_1 (IO_1)
  PIN_10  (10kOhm to 3,3V)
  PIN_11  P3
  PIN_12  P1
  PIN_13  USB_D_N
  PIN_14  USB_D_P
  PIN_15  (PGM_MODE BUTTON - PROGRAM MODE)
  PIN_16  
  PIN_17
  PIN_18
  PIN_19  
  PIN_20  SCL (IO_22 - I2C CLK)
  PIN_21  SDA (IO_23 - I2C DATA)
  PIN_22  
  PIN_23  GND
  PIN_24  RxD
  PIN_25  TxD
  PIN_26  ANALOG_3 (IO_3)
  PIN_27  ANALOG_2 (IO_2)
  PIN_28  GND

*/
#include <Arduino.h>
#include <esp_sleep.h>
#include <Time.h> //watchdog and timer
#include <Ticker.h>
#include <Wire.h>

#include <WiFi.h>
#include <DNSServer.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <WebServer.h>

#include "core_task1.h"
#include "core_task2.h"
#include "serial_task.h"
#include "shared_data.h"
#include "esp_task_wdt.h" //watchdog
#include "dlb_eeprom.h"
#include "WWW_site.h"
#include "dlb_server.h"

int server_firmware_version = 1;

#define BUTTON_PIN    4  // GPIO4
#define BUTTON_3 GPIO_NUM_3
#define WAKEUP_TIME_SEC 10        // czas wybudzenia w sekundach

volatile bool buttonPressed = false;  // musi być volatile! aby kompilator nie optymalizował ich "na zbyt mądrze"
static unsigned long lastPrint = 0;

bool Serial_ = true;

String server_fingerprit;
WiFiMulti wifiMulti;
DNSServer dnsServer;
WebServer server(80);

TaskHandle_t Task1;
TaskHandle_t Task2;

TaskHandle_t SerialTaskHandle = NULL;

/* -------------------------------------------------------------------------------------- Definicja obiektow */
dlb_eeprom dlb_eeprom_obj(true);
dlb_server dlb_server_obj(&server, server_firmware_version);
/* -------------------------------------------------------------------------------------- Zmienne konfiguracyjne */
// String user_name ="";
// String wifiSSID = "";
// String wifiPassword = "";
// String server_name = "";

void IRAM_ATTR handleInterrupt() {
  buttonPressed = true;
}

void IRAM_ATTR handleInterrupt_GPIO8() {
  Serial.println("handleInterrupt_GPIO8");
}

void IRAM_ATTR handleInterrupt_GPIO10() {
  Serial.println("handleInterrupt_GPIO10");
}

void IRAM_ATTR handleInterrupt_GPIO11() {
  Serial.println("handleInterrupt_GPIO11");
}

// Funkcja do obsługi strony głównej
void handleRoot() {
    server.send(200, "text/html", web_site());
}

// Funkcja do zapisywania danych z formularza
void handleSave() {
    if (server.hasArg("wifissid") && server.hasArg("wifipass") && server.hasArg("server_name")) {

    if (server.hasArg("wifissid")) {  // Sprawdź, czy istnieje parametr o nazwie "variable_name"
      if(server.arg("wifissid")=="*"){
        dlb_eeprom_obj.save("", 0, 22);
      }
      else
        //DLB_EPR.save(server->arg("wifissid"),0,22);  
        dlb_eeprom_obj.save(server.arg("wifissid"), 0, 22);
    }

    if (server.hasArg("wifipass")) {  // Sprawdź, czy istnieje parametr o nazwie "variable_name"
      if(server.arg("wifipass")=="*"){
        dlb_eeprom_obj.save("", 22, 22);
      }
      else
      //DLB_EPR.save(server->arg("wifipass"),22,22);  
      dlb_eeprom_obj.save(server.arg("wifipass"), 22, 22);
    }

    if (server.hasArg("username")) {  // Sprawdź, czy istnieje parametr o nazwie "variable_name"
      if(server.arg("username")=="*"){
        dlb_eeprom_obj.save("", 44, 20);
      }
      else
      //DLB_EPR.save(server->arg("username"),44,20);  
      dlb_eeprom_obj.save(server.arg("username"), 44, 20);
    }

    if (server.hasArg("server_name")) {
      dlb_eeprom_obj.save(server.arg("server_name"), 64, 20);
    }

    server.send(200, "text/plain", "Dane zapisane! Restartowanie...");
    delay(1000);
    esp_restart();  // Restart ESP po zapisaniu danych
  } 
  else {
    server.send(400, "text/plain", "Błąd: Brak danych");
  }
}

// captive portal page detect that WiFi AP has a captive portal page
void handleNotFound() {
  server.sendHeader("Location", "/portal");
  server.send(302, "text/plain", "redirect to captive portal");
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
void setup() {
  Serial.begin(115200);
  while (!Serial); // Czekaj na połączenie
  delay(500);

  Serial.println("System uruchomiony. Czekam ...");

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);

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
  pinMode(GPIO_NUM_3, INPUT); //WakeUP

  pinMode(BUTTON_3, INPUT); // używamy pullup na zewnatrz !!!

  pinMode(GPIO_NUM_2, OUTPUT);
  pinMode(GPIO_NUM_8, OUTPUT);  //??? nie dziala
  pinMode(GPIO_NUM_9, OUTPUT);
  pinMode(GPIO_NUM_10, OUTPUT);
  pinMode(GPIO_NUM_11, OUTPUT);
  pinMode(GPIO_NUM_16, OUTPUT);
  pinMode(GPIO_NUM_17, OUTPUT);
  pinMode(GPIO_NUM_18, OUTPUT);
  pinMode(GPIO_NUM_19, OUTPUT);
  pinMode(GPIO_NUM_20, OUTPUT);
  pinMode(GPIO_NUM_21, OUTPUT);
  pinMode(GPIO_NUM_22, OUTPUT);

  // RISING – zbocze narastające
  // FALLING – zbocze opadające
  // CHANGE – dowolna zmiana
  // HIGH lub LOW – poziom logiczny (rzadziej używany)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
  //attachInterrupt(digitalPinToInterrupt(GPIO_NUM_8), handleInterrupt_GPIO8, FALLING);
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

  // Maska pinu 8
  uint64_t pinMask = 1ULL << GPIO_NUM_3;

  // Wybudzenie: przycisk (stan niski) lub timer (10 sekund)
  esp_sleep_enable_ext1_wakeup(pinMask, ESP_EXT1_WAKEUP_ALL_LOW);
  //esp_sleep_enable_timer_wakeup(WAKEUP_TIME_SEC * 1000000);

  // Start deep sleep
  // esp_task_wdt_deinit();  // wyłącza watchdog (nie jest potrzebny przy deep sleep)
  // esp_light_sleep_start();
  // esp_task_wdt_reset();

  WiFi.mode(WIFI_AP);
  WiFi.softAP("ROM_APN");
  if (dnsServer.start()) {
      Serial.println("Started DNS server in captive portal-mode");
  } else {
      Serial.println("Err: Can't start DNS server!");
  }
  server.onNotFound([]() {
    server.send(200, "text/html", web_site());
  });
  // Obsługa żądania GET
  server.on("/setting", HTTP_GET, []() {
    String message = "<html><h1>done!</h1></html>";
    handleSave();
    server.send(200, "text/html", message);  // Wyślij odpowiedź
    //if(Serial_) Serial.println("restart");
    esp_restart();
  });
  server.begin();
  
  WiFi.begin(sharedData.wifiSSID, sharedData.wifiPassword);
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(100);
  //  Serial.print("...");
  //}
} //setup
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
void loop() {
  esp_task_wdt_reset();
  server.handleClient();

  Serial.print("GPIO_NUM_3 - ");
  Serial.println(GPIO_NUM_3);

  int analog_0 = analogReadMilliVolts(0);
  int analog_1 = analogReadMilliVolts(1);

  Serial.print("Analog_0 - ");
  Serial.print(analog_0);
  Serial.print("  Analog_1 - ");
  Serial.println(analog_1);

  if (millis() - lastPrint > 5000) {
    Serial.println("loop() działa niezależnie...");
    lastPrint = millis();
  }

  if(buttonPressed){
    buttonPressed = false;
    Serial.println("buttonPressed");
  }


  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("no WiFi connection ...");
    delay(1000);
    WiFi.begin(sharedData.wifiSSID, sharedData.wifiPassword);
    int nn = 0;
    while ((WiFi.status() != WL_CONNECTED) and (nn<5)) {
      delay(100);
      Serial.print("...");
      esp_task_wdt_reset();
      nn++;
    }
  }
  else{

  dlb_server_obj.send_http_event("http://"+String(sharedData.server_name)+"/PST_LOG/X_copy.php?v1=ANDON_BECZKI&v2=1&v3=104333-01&v6=49&MACADDR=40:4c:ca:4b:60:f8:ff:fe&IP=175.255.255.000");
  dlb_server_obj.send_https_event("https://172.16.72.191");


  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("RSSI: ");
  Serial.println(String(WiFi.RSSI())+" dB");

  // Ustawienie timera budzenia
  //esp_sleep_enable_timer_wakeup(100 * 1000000);
  // Start deep sleep
  //esp_task_wdt_deinit();  // wyłącza watchdog (nie jest potrzebny przy deep sleep)
  neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red UPDATE
  delay(1000);
  //esp_deep_sleep_start();
  //esp_task_wdt_reset();
  }
  
  delay(1000);

}
