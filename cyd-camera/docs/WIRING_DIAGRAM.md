# Camera Module Wiring Diagram

## Visual Connection Guide

### Single Board Setup Overview

```
                    ┌─────────────────────────────────────┐
                    │         CYD Board                   │
                    │  (ESP32 + ST7789 Display)           │
                    └─────────────────────────────────────┘
                                    │
                    ┌───────────────┴───────────────┐
                    │                               │
            ┌───────▼──────┐              ┌────────▼────────┐
            │  Camera Pins │              │  Display Pins   │
            │  (connect    │              │  (pre-wired     │
            │   these)     │              │   internally)   │
            └──────────────┘              └─────────────────┘
```

### Camera Module Pin Connections

```
  Camera Module                        CYD Board
  (OV2640/OV7670)                      ESP32 GPIO
  
  ┌─────────────┐                    ┌──────────────┐
  │             │                    │              │
  │    [LENS]   │                    │   [DISPLAY]  │
  │             │                    │              │
  │  Pin Header │                    │   Connector  │
  └──────┬──────┘                    └──────┬───────┘
         │                                  │
    ┌────┴─────┐                     ┌──────┴────────┐
    │          │                     │               │
    │ D0  ─────┼─────────────────────┤ GPIO5         │
    │ D1  ─────┼─────────────────────┤ GPIO18        │
    │ D2  ─────┼─────────────────────┤ GPIO19        │
    │ D3  ─────┼─────────────────────┤ GPIO4   ⚠️    │
    │ D4  ─────┼─────────────────────┤ GPIO35  (IN)  │
    │ D5  ─────┼─────────────────────┤ GPIO34  (IN)  │
    │ D6  ─────┼─────────────────────┤ GPIO39  (IN)  │
    │ D7  ─────┼─────────────────────┤ GPIO32        │
    │          │                     │               │
    │ XCLK ────┼─────────────────────┤ GPIO0         │
    │ PCLK ────┼─────────────────────┤ GPIO22        │
    │ VSYNC───┼─────────────────────┤ GPIO25        │
    │ HREF ────┼─────────────────────┤ GPIO23        │
    │          │                     │               │
    │ SDA  ────┼─────────────────────┤ GPIO26  (I2C) │
    │ SCL  ────┼─────────────────────┤ GPIO27  (I2C) │
    │          │                     │               │
    │ 3.3V ────┼─────────────────────┤ 3.3V          │
    │ GND  ────┼─────────────────────┤ GND           │
    │          │                     │               │
    └──────────┘                     └───────────────┘
```

### Pin Legend

- ⚠️ GPIO4: Modified from standard ESP32-CAM to avoid conflict
- (IN): Input-only pins (GPIO34, 35, 36, 39)
- (I2C): Camera control interface (SCCB)

### Color Coding for Wires (Suggested)

If using colored Dupont wires, this scheme helps organization:

| Pin Group | Color Suggestion | Pins |
|-----------|------------------|------|
| Data 0-3 | 🔴 Red bundle | D0, D1, D2, D3 |
| Data 4-7 | 🟡 Yellow bundle | D4, D5, D6, D7 |
| Clocks | 🔵 Blue bundle | XCLK, PCLK, VSYNC, HREF |
| I2C | 🟢 Green bundle | SDA, SCL |
| Power | ⚫ Black/White | 3.3V, GND |

### Step-by-Step Wiring

1. **Power Off Everything** - Disconnect all power before wiring

2. **Connect Data Lines** (8 wires)
   ```
   Camera → CYD
   D0 → GPIO5
   D1 → GPIO18
   D2 → GPIO19
   D3 → GPIO4   ⚠️ Note: Different from ESP32-CAM standard!
   D4 → GPIO35
   D5 → GPIO34
   D6 → GPIO39
   D7 → GPIO32
   ```

3. **Connect Clock/Sync Lines** (4 wires)
   ```
   Camera → CYD
   XCLK  → GPIO0
   PCLK  → GPIO22
   VSYNC → GPIO25
   HREF  → GPIO23
   ```

4. **Connect I2C Lines** (2 wires)
   ```
   Camera → CYD
   SDA → GPIO26
   SCL → GPIO27
   ```

5. **Connect Power** (2 wires)
   ```
   Camera → CYD
   3.3V → 3.3V
   GND  → GND
   ```

6. **Double Check**
   - Total: 16 wires
   - No loose connections
   - Power and GND not reversed
   - All data pins correctly connected

### Common Camera Module Pinouts

#### OV2640 Module
```
Top View (component side)
┌─────────────────┐
│  [  LENS  ]     │
│                 │
│ 3V3  GND  SCL   │
│ SDA  VSYNC HREF │
│ PCLK XCLK D7    │
│ D6   D5   D4    │
│ D3   D2   D1    │
│ D0              │
└─────────────────┘
```

#### OV7670 Module (Similar)
```
Pin order may vary by manufacturer
Check your module's datasheet
Most follow similar layout
```

### Troubleshooting Wiring

#### Camera Not Detected
- ✅ Check I2C connections (SDA/SCL)
- ✅ Verify 3.3V power is connected
- ✅ Ensure GND is common between camera and CYD
- ✅ Check for loose Dupont connectors

#### Corrupted Image
- ✅ Verify all 8 data lines (D0-D7)
- ✅ Check clock lines (XCLK, PCLK)
- ✅ Ensure short wire lengths (< 10cm preferred)
- ✅ Avoid running near power lines

#### No Image
- ✅ Check VSYNC and HREF connections
- ✅ Verify camera lens is focused
- ✅ Ensure adequate lighting
- ✅ Check if camera module is powered

### Recommended Tools

- **Dupont Wires**: Female-to-female, 10-15cm length
- **Multimeter**: For checking continuity and voltages
- **Label Tape**: Mark wires for easy troubleshooting
- **Cable Ties**: Keep wires organized

### Safety Notes

⚠️ **Before connecting:**
1. Unplug all power
2. Check pin assignments twice
3. Verify voltage (3.3V, NOT 5V for camera)
4. Don't force connectors

⚠️ **During operation:**
1. Don't disconnect while powered
2. Handle camera module carefully (ESD sensitive)
3. Keep away from water
4. Monitor for overheating

### Testing Your Wiring

After connecting everything:

1. **Power On**
   - Connect 5V USB to CYD
   - Check if display lights up
   - Serial monitor should show "Initializing..."

2. **Check Serial Output**
   ```
   Expected messages:
   - "Camera initialized successfully"
   - "WiFi connected"
   - "IP Address: 192.168.x.x"
   ```

3. **If Camera Init Fails**
   - Check I2C connections first
   - Verify power (3.3V with multimeter)
   - Recheck all data pin connections
   - Try different camera module if available

4. **If Display Works But No Camera**
   - Camera module may be faulty
   - Try swapping SDA/SCL
   - Check camera module power consumption

### Pin Assignment Table

Quick reference for wiring:

| Camera | Function | CYD GPIO | Notes |
|--------|----------|----------|-------|
| D0 | Data 0 | 5 | |
| D1 | Data 1 | 18 | |
| D2 | Data 2 | 19 | |
| D3 | Data 3 | 4 | ⚠️ Modified |
| D4 | Data 4 | 35 | Input only |
| D5 | Data 5 | 34 | Input only |
| D6 | Data 6 | 39 | Input only |
| D7 | Data 7 | 32 | |
| XCLK | Clock out | 0 | 20MHz |
| PCLK | Pixel clock | 22 | Input |
| VSYNC | V-Sync | 25 | Input |
| HREF | H-Ref | 23 | Input |
| SDA | I2C Data | 26 | Bidirectional |
| SCL | I2C Clock | 27 | Output |
| 3.3V | Power | 3.3V | ~150mA |
| GND | Ground | GND | Common |

Print this diagram and keep it handy while wiring! 📝
