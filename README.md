# Arduino ESP32 Multitasking Example

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

## Autor
Dawid Rosak
