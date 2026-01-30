# CYD ESP32 Camera Wiring Guide

## Connection Diagram

### Important Note on Pin Configuration

**⚠️ GPIO Pin Allocation:**
This project assumes you have TWO separate ESP32 boards:
1. **ESP32-CAM** - Handles camera operations
2. **CYD (ESP32 with display)** - Displays status and controls

Both connect to the same WiFi network. The camera streams to the CYD via WiFi.

If you want to use a **single ESP32** with both camera and display, you MUST modify the pin assignments to avoid conflicts, particularly GPIO21 which is used by both the camera (D3) and display (backlight).

### ESP32-CAM to CYD ESP32 Wiring

```
ESP32-CAM           CYD ESP32
---------           ---------
5V         -------> 5V
GND        -------> GND
U0TXD      -------> U0RXD (for programming)
U0RXD      -------> U0TXD (for programming)
```

**Note**: For camera operation, the ESP32-CAM runs independently. The CYD acts as a display and control interface via WiFi.

### Alternative Setup: Integrated System

If using a single ESP32 with both camera and display:

```
Component          ESP32 Pin
---------          ---------
Camera Data Bus    As defined in camera_config.h
Display MOSI       GPIO13
Display SCLK       GPIO14
Display CS         GPIO15
Display DC         GPIO2
Display BL         GPIO21
Touch CS           GPIO33
Touch IRQ          GPIO36
```

## Wiring Tips

1. **Power Supply**
   - Use a stable 5V power source
   - Camera can draw significant current (300-400mA)
   - Avoid powering from USB if current is insufficient

2. **Signal Integrity**
   - Keep camera data wires short
   - Use twisted pairs for I2C lines (SDA/SCL)
   - Add 100nF capacitor near camera power pins

3. **Programming**
   - Pull GPIO0 to GND for flash mode
   - Use FTDI or CH340 programmer
   - Connect EN to 3.3V through 10K resistor

## Pin Configuration Reference

### ESP32-CAM (AI-Thinker)

| Function | GPIO | Notes |
|----------|------|-------|
| D0 | 5 | Data bit 0 |
| D1 | 18 | Data bit 1 |
| D2 | 19 | Data bit 2 |
| D3 | 21 | Data bit 3 |
| D4 | 36 | Data bit 4 |
| D5 | 39 | Data bit 5 |
| D6 | 34 | Data bit 6 |
| D7 | 35 | Data bit 7 |
| XCLK | 0 | Camera clock |
| PCLK | 22 | Pixel clock |
| VSYNC | 25 | Vertical sync |
| HREF | 23 | Horizontal ref |
| SDA | 26 | I2C data |
| SCL | 27 | I2C clock |
| PWDN | 32 | Power down |
| LED | 4 | Flash LED |

### CYD Display (ST7789)

| Function | GPIO | Notes |
|----------|------|-------|
| MOSI | 13 | SPI data |
| SCLK | 14 | SPI clock |
| CS | 15 | Chip select |
| DC | 2 | Data/Command |
| RST | -1 | Not used |
| BL | 21 | Backlight |

## Power Budget

| Component | Current Draw | Notes |
|-----------|--------------|-------|
| ESP32 Core | 80-160mA | Active WiFi |
| Camera | 100-200mA | During capture |
| Display | 20-40mA | Depends on brightness |
| LED Flash | 0-150mA | When enabled |
| **Total** | **200-550mA** | Peak consumption |

## Troubleshooting

### Camera Issues
- **No camera detected**: Check power and I2C connections (GPIO26, GPIO27)
- **Poor image quality**: Verify camera lens focus and lighting
- **Brownout detector**: Insufficient power supply

### Display Issues
- **Blank screen**: Check SPI connections and backlight
- **Inverted colors**: Adjust color mode in TFT_eSPI settings
- **Touch not responding**: Verify touch controller wiring

### WiFi Issues
- **Won't connect**: Check 2.4GHz WiFi, not 5GHz
- **Weak signal**: Add external antenna to ESP32
- **Interference**: Keep away from large metal objects
