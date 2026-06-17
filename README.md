# 🌦️ ESP32 Weather Station Dashboard

### BMP180 Sensor + SSD1306 OLED Display + I2C

An embedded C++ weather monitoring system built using **ESP32**, **PlatformIO**, and **VS Code**.

The project reads **temperature** and **atmospheric pressure** data from a **BMP180/BMP085 barometric sensor** through the **I2C communication protocol** and displays live sensor values with a real-time temperature trend graph on a **128x64 SSD1306 OLED display**.

---

## ✨ Features

- Real-time temperature monitoring
- Atmospheric pressure measurement
- SSD1306 OLED dashboard display
- Live temperature trend graph
- I2C communication with multiple devices
- Rolling history buffer for graph plotting
- Function-based embedded firmware structure
- Wokwi simulation support

---

## 🛠️ Hardware Components

| Component | Purpose |
|---|---|
| ESP32 DevKit V1 | Main microcontroller |
| BMP180/BMP085 | Temperature and pressure sensor |
| SSD1306 OLED (128x64) | Display module |

---

## 🔌 Circuit Connection

Both the BMP180/BMP085 sensor and OLED display share the same I2C bus.

| ESP32 Pin | Connected To | Function |
|---|---|---|
| 3V3 | VCC | Power supply |
| GND | GND | Common ground |
| GPIO 22 | SCL | I2C Clock |
| GPIO 21 | SDA | I2C Data |

### I2C Addresses

| Device | Address |
|---|---|
| BMP180/BMP085 | `0x77` |
| SSD1306 OLED | `0x3C` |

---

## 📁 Project Structure

```
ESP32-Weather-Station/
│
├── src/
│   └── main.cpp              # Main firmware code
│
├── diagram.json              # Wokwi circuit configuration
├── wokwi.toml                # Wokwi simulator settings
├── platformio.ini            # PlatformIO configuration
│
└── README.md
```

---

## ⚙️ Software Architecture

The firmware uses a lightweight function-based design.

### Program Flow

```
                 loop()

                   |
                   |
          Check elapsed time
             (every 2 sec)

                   |

                   v

        +----------------------+
        | readSensorAndTrack() |
        +----------------------+

                   |

                   |

        Read BMP180/BMP085 data

                   |

                   |

        Store temperature history

        Update graph buffer index

                   |

                   v

          +----------------+
          |  drawDisplay() |
          +----------------+

                   |

                   |

        Display values and graph

        on SSD1306 OLED
```

---

## 🧠 Firmware Functions

### initializeSystem()

Initializes:

- Serial communication
- I2C bus
- OLED display
- BMP180/BMP085 sensor
- Temperature history buffer

---

### readSensorAndTrack()

Responsible for:

- Reading temperature data
- Reading pressure data
- Updating stored history values
- Maintaining graph buffer

---

### drawDisplay()

Handles OLED rendering:

- Clears display buffer
- Prints sensor readings
- Converts data into graph coordinates
- Draws temperature trend graph
- Updates OLED screen

---

# 💻 Running The Project

## Requirements

Install:

- VS Code
- PlatformIO IDE Extension
- Wokwi Extension

---

## Clone Repository

```bash
git clone <repository-url>

cd ESP32-Weather-Station
```

---

## Build Project

Using PlatformIO:

```
PlatformIO → Build
```

or terminal:

```bash
pio run
```

---

## Run Simulation

Open VS Code Command Palette:

```
Ctrl + Shift + P
```

Select:

```
Wokwi: Start Simulator
```

The ESP32 circuit will start running.

---

## 📚 Libraries Used

- Adafruit BMP085 Library
- U8g2 OLED Graphics Library
- Arduino ESP32 Framework

---

## 🚀 Future Improvements

- Add humidity sensor
- Add WiFi data logging
- MQTT cloud dashboard
- Store historical sensor readings
- Battery-powered version
- Web dashboard integration

---

## 🧰 Development Stack

| Tool | Usage |
|---|---|
| C++ | Firmware development |
| ESP32 | Microcontroller |
| PlatformIO | Build system |
| VS Code | Development environment |
| Wokwi | Circuit simulation |