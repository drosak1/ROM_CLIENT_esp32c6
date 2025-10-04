# ESP32c6 Multitasking Example

This project demonstrates multitasking on the ESP32 platform using FreeRTOS tasks, hardware interrupts, and a watchdog timer. It is designed for educational purposes and is structured for clarity and modularity.

## Główne funkcje
- **Wielowątkowość:** Dwa zadania (Task1, Task2) uruchamiane na różnych rdzeniach ESP32
- **Obsługa przerwań:** Przycisk na GPIO4 wywołuje przerwanie
- **Komunikacja szeregowa:** Osobny task do obsługi komunikacji przez port szeregowy
- **Watchdog (WDT):** Monitoruje główną pętlę i taski, automatycznie resetuje system przy zawieszeniu
- **Współdzielone dane:** Przykład bezpiecznego współdzielenia danych między taskami

## Pliki projektu
- `test.ino` – główny plik uruchomieniowy
- `core_task1.*`, `core_task2.*` – implementacje zadań
- `serial_task.*` – obsługa komunikacji szeregowej
- `shared_data.*` – współdzielone dane między taskami

## Szybki start
1. Otwórz projekt w Arduino IDE lub VS Code z wtyczką PlatformIO.
2. Wgraj na płytkę ESP32.
3. Podłącz przycisk do GPIO4 (z rezystorem podciągającym lub użyj wbudowanego INPUT_PULLUP).
4. Monitoruj port szeregowy (baud 115200).

## Budowanie i uruchamianie
- Standardowy proces kompilacji Arduino/PlatformIO.
- Pliki binarne i logi kompilacji znajdują się w katalogu `build/`.

## Wzorce i konwencje
- Każdy task ma własny plik nagłówkowy i źródłowy.
- Współdzielone dane trzymane są w strukturze `sharedData` (patrz `shared_data.h/cpp`).
- Przerwania obsługiwane są przez funkcję `handleInterrupt()`.
- Watchdog konfigurowany w `setup()` i resetowany w `loop()`.

## Rozszerzanie projektu
- Dodawaj nowe taski analogicznie do istniejących.
- Współdzielone dane dodawaj do struktury w `shared_data.h`.
- Komunikację między taskami realizuj przez zmienne globalne zabezpieczone mutexem lub atomowe typy.


## Boards
- esp32 by Espressif Systems tested version 3.3.0

## Visual (MAC OS)
- zainstaluj Arduino Maker Workshop
- zainstaluj Platformio IO
- Arduino Maker Workshop › Arduino CLI: Executable Executable filenam -> arduino-cli
- Arduino Maker Workshop › Arduino CLI: Install Path -> /Applications/Arduino IDE.app/Contents/Resources/Arduino IDE

## Github
- git init & add files & commit
- git remote add origin https://github.com/drosak1/ROM_CLIENT_esp32c6.git
- git pull origin main --allow-unrelated-histories
- https://github.com/settings/tokens -> “Personal access tokens (classic)”
- [x] repo [x] workflow [x] read:org
- Username for 'https://github.com': drosak1
- Password for 'https://github.com': <paste the token>
- git push --force origin main


## Tryby uśpienia ESP32-C6
ESP32-C6 (RISC-V, Wi-Fi 6, BLE 5) ma 3 główne tryby oszczędzania energii:
Dla ESP32-C6 RTC IO są : (GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5, GPIO6, GPIO7)
Tryb, Co działa, Zużycie energii (orientacyjnie)
Modem-sleep	CPU aktywny, Wi-Fi/BLE śpi okresowo	3–20 mA	gdy CPU musi działać, ale Wi-Fi nie cały czas
Light-sleep	CPU i większość peryferiów wstrzymana, RAM zachowany	0.8–2 mA	gdy chcesz szybkie wybudzenie
Deep-sleep	CPU, Wi-Fi, BLE i RAM wyłączone (tylko RTC działa)	10–20 µA	minimalne zużycie – np. zasilanie bateryjne

## CPU Board Options
- Upload Speed                          - 921600
- USB CDC On Boot                       - Enabled
- CPU Frequency                         - 80MHz (WiFi)
- Flash Frequency                       - 80MHz
- Flash Mode                            - QIO
- Flash Size                            - 4MB (32Mb)
- Partition Scheme                      - RainMaker 4MB
- Core Debug Level                      - None
- Erase All Flash Before Sketch Upload  - Disabled
- JTAG Adapter                          - Disabled
- Zigbee Mode                           - Disabled


## Autor
Dawid Rosak
