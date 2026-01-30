# CYD ESP32 Camera Wiring Guide

## Single Board Configuration

**⚠️ Important:** This project uses a **SINGLE ESP32** setup where the CYD's built-in ESP32 controls both the display AND a camera module.

### System Architecture
```
┌─────────────────────────────────┐
│         CYD Board               │
│  ┌───────────────────────────┐  │
│  │      ESP32 (main)         │  │
│  │  ┌─────────┐  ┌─────────┐ │  │
│  │  │ Camera  │  │ Display │ │  │
│  │  │ Module  │  │ ST7789  │ │  │
│  │  └─────────┘  └─────────┘ │  │
│  └───────────────────────────┘  │
└─────────────────────────────────┘
```

### Camera Module to CYD ESP32 Connections

**Camera Module → CYD ESP32 GPIO Pins:**

| Camera Pin | Function | CYD GPIO | Notes |
|------------|----------|----------|-------|
| D0 | Data 0 | GPIO5 | |
| D1 | Data 1 | GPIO18 | |
| D2 | Data 2 | GPIO19 | |
| D3 | Data 3 | GPIO4 | ⚠️ Changed to avoid GPIO21 conflict |
| D4 | Data 4 | GPIO35 | Input only pin |
| D5 | Data 5 | GPIO34 | Input only pin |
| D6 | Data 6 | GPIO39 | Input only pin |
| D7 | Data 7 | GPIO32 | |
| XCLK | Clock | GPIO0 | Camera clock |
| PCLK | Pixel Clock | GPIO22 | |
| VSYNC | V-Sync | GPIO25 | |
| HREF | H-Ref | GPIO23 | |
| SDA | I2C Data | GPIO26 | SCCB (camera I2C) |
| SCL | I2C Clock | GPIO27 | SCCB (camera I2C) |
| 3.3V | Power | 3.3V | Camera power |
| GND | Ground | GND | Common ground |

### Display Pins (Built-in to CYD)

**CYD Display → ESP32 GPIO (Pre-wired):**

| Display Pin | Function | CYD GPIO |
|-------------|----------|----------|
| MOSI | SPI Data | GPIO13 |
| SCLK | SPI Clock | GPIO14 |
| CS | Chip Select | GPIO15 |
| DC | Data/Command | GPIO2 |
| BL | Backlight | GPIO21 |

### Touch Screen Pins (Built-in to CYD)

| Touch Pin | Function | CYD GPIO |
|-----------|----------|----------|
| CS | Chip Select | GPIO33 |
| IRQ | Interrupt | GPIO36 |

## Wiring Tips

### Important Notes for Single Board Setup

1. **Camera Module Types**
   - OV2640 module (recommended)
   - OV7670 module (needs different config)
   - Most common camera modules work
   - Must be 3.3V compatible

2. **Power Considerations**
   - Camera draws 100-200mA
   - Display draws 40-60mA  
   - Total: 200-300mA minimum
   - Use 5V power supply with at least 1A capacity
   - CYD's 3.3V regulator powers the camera

3. **Pin Conflict Resolution**
   - Display backlight uses GPIO21
   - Camera D3 changed to GPIO4 (was GPIO21 in ESP32-CAM standard)
   - This allows both to work together
   - No wiring changes needed for display (built-in to CYD)

## Pin Configuration Reference

### Camera Module Pins (Connect to CYD)

| Function | CYD GPIO | Direction | Notes |
|----------|----------|-----------|-------|
| D0 | 5 | Input | Data bit 0 |
| D1 | 18 | Input | Data bit 1 |
| D2 | 19 | Input | Data bit 2 |
| D3 | 4 | Input | Data bit 3 (modified for CYD) |
| D4 | 35 | Input | Data bit 4 (input only) |
| D5 | 34 | Input | Data bit 5 (input only) |
| D6 | 39 | Input | Data bit 6 (input only) |
| D7 | 32 | Input | Data bit 7 |
| XCLK | 0 | Output | Camera clock (20MHz) |
| PCLK | 22 | Input | Pixel clock |
| VSYNC | 25 | Input | Vertical sync |
| HREF | 23 | Input | Horizontal ref |
| SDA | 26 | I/O | I2C data (SCCB) |
| SCL | 27 | Output | I2C clock (SCCB) |
| 3.3V | 3.3V | Power | Camera power |
| GND | GND | Ground | Common ground |

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
| ESP32 Core | 80-240mA | WiFi active |
| Camera Module | 80-150mA | During capture |
| TFT Display | 40-60mA | Depends on brightness |
| Backlight LED | 20-40mA | At full brightness |
| **Total** | **220-490mA** | Peak consumption |

**Recommended Power Supply:** 5V @ 1A minimum

## Troubleshooting

### Camera Issues
- **No camera detected**: Check I2C connections (GPIO26, GPIO27) and power
- **Camera init failed**: Verify all data pins connected correctly
- **Poor image quality**: Check camera lens focus and ensure good lighting
- **Brownout detector**: Insufficient power supply, use 5V @ 1A

### Display Issues
- **Blank screen**: Check SPI connections and backlight
- **Inverted colors**: Adjust color mode in TFT_eSPI settings
- **Touch not responding**: Verify touch controller wiring

### WiFi Issues
- **Won't connect**: Check 2.4GHz WiFi, not 5GHz
- **Weak signal**: Add external antenna to ESP32
- **Interference**: Keep away from large metal objects
