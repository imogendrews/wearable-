# Wearable Pulse Tracker

This project uses an ESP32 with a pulse sensor to track visitors' heartbeats in an exhibition. Pulse data is sent to a Supabase backend for visualization.

## Features

- Capture pulse using PulseSensor
- Send data to Supabase via HTTP
- Optional LED feedback for heartbeat
- Data stored per visitor session

## Hardware

- ESP32 Dev Kit
- PulseSensor.com sensor
- Optional NFC module

## Software

- PlatformIO / Arduino Framework
- Supabase backend
- VS Code for development

## Setup

1. Make sure that you have the extension PlatformIO IDE installed and enabled
2. Connect the pulse sensor:
   - 3V3 → 3V
   - GND → GND
   - Signal → GPIO34
3. Update `ssid` and `password` in `main.cpp` for your Wi-Fi.
4. Update `SUPABASE_URL` and `SUPABASE_ANON_KEY` with your Supabase project details.
5. Compile and upload via PlatformIO.

### Quick PlatformIO tutorial

1. Install PlatformIO (VS Code extension or CLI). For macOS CLI (recommended via `pipx`):

```bash
python3 -m pip install --user pipx
python3 -m pipx ensurepath
# restart your terminal
pipx install platformio
```

2. Confirm your ESP32 is connected and find its serial port:

```bash
ls /dev/cu.*
# or
platformio device list
```

3. From the project root, build the firmware:

```bash
platformio run -e esp32dev
```

4. Upload to the board (replace the env name or port if you changed it in `platformio.ini`):

```bash
platformio run -t upload -e esp32dev
```

5. Open the serial monitor to see device output (replace the port if different):

```bash
platformio device monitor -p /dev/cu.usbserial-0001 -b 115200
```

Press Ctrl+C to exit the monitor.

GUI (VS Code) alternative: Use the PlatformIO toolbar (Build / Upload / Monitor) or Command Palette commands: `PlatformIO: Build`, `PlatformIO: Upload`, `PlatformIO: Serial Monitor`.

NB: You will probably need to add the following code to the Pulse Sensor Playground file. You can find it in .pio -> libdeps/esp32dev -> PulseSensor Playground -> src -> PulseSensorPlayground.cpp.

On line 48 add:

`bool PulseSensorPlayground::begin() (and delete other code)`

and on line 493 replace the exisiting code with this:

```// Use the Arduino-ESP32 timer API: timerBegin(timer_number, prescaler, countUp)
   // With APB clock 80MHz, prescaler 80 yields a 1MHz tick (1us period).
   sampleTimer = timerBegin(0, 80, true); // timer 0, 1uS tick period
   timerAttachInterrupt(sampleTimer, &onInterrupt, true);
   // Set alarm for 2000 ticks = 2000us (2ms -> 500Hz) and enable it
   timerAlarmWrite(sampleTimer, 2000, true);
   timerAlarmEnable(sampleTimer);
```

## Notes

- Do not commit `.pio` or `.vscode` folders.
- Use `.gitignore` and make sure this is in it:

```Supabase:
.branches
.temp

dotenvx:
.env.keys
.env.local
.env.*.local

.pio/
.vscode/
*.bin
*.elf
*.pioenvs/

 Mac / System files:
.DS_Store

src/frontend/node_modules/

```
