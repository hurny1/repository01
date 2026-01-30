# Important Configuration Notes

## System Architecture

This project uses a **SINGLE BOARD** configuration:

### Hardware Setup (Current Configuration)
- **CYD Board**: ESP32 with built-in ST7789 display (320x240)
- **Camera Module**: OV2640 or compatible, connected to CYD GPIO pins
- **Single ESP32**: Controls both display and camera
- **Pin Assignment**: Optimized to avoid conflicts

```
┌─────────────────────────────┐
│       CYD Board             │
│  ┌───────────────────────┐  │
│  │     ESP32             │  │
│  │    (controls          │  │
│  │     everything)       │  │
│  │                       │  │
│  │  ┌─────┐   ┌──────┐  │  │
│  │  │Cam  │   │ TFT  │  │  │
│  │  │Mod  │   │Display│ │  │
│  │  └─────┘   └──────┘  │  │
│  └───────────────────────┘  │
└─────────────────────────────┘
```

## Pin Assignment Details

### Pin Conflict Resolution

**✅ Resolved:** The default ESP32-CAM pin layout uses GPIO21 for camera D3, but CYD uses GPIO21 for display backlight.

**Solution Implemented:**
- Camera D3 reassigned to GPIO4
- Display backlight remains on GPIO21
- Both components now work together without conflicts

## WiFi Credentials

**Security Warning:** The default code has WiFi credentials hardcoded in `src/main.cpp`.

**Best Practice:**
1. Create a `secrets.h` file:
```cpp
#ifndef SECRETS_H
#define SECRETS_H
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
// Web authentication
const char* www_username = "your_username";
const char* www_password = "your_strong_password";
#endif
```

2. Add to `.gitignore`:
```
secrets.h
```

3. Include in `main.cpp`:
```cpp
#include "secrets.h"
```

## Web Authentication

**NEW:** HTTP Basic Authentication is now required for all web endpoints.

**Default Credentials:**
- Username: `admin`
- Password: `esp32cam`

⚠️ **CRITICAL:** Change these defaults immediately! Edit `src/main.cpp`:
```cpp
const char* www_username = "admin";        // Change this
const char* www_password = "esp32cam";     // Change this
```

**Password Guidelines:**
- Use at least 12 characters
- Mix uppercase, lowercase, numbers, and symbols
- Don't use common words or patterns
- Don't reuse passwords from other services

**How Authentication Works:**
- HTTP Basic Authentication
- Browser will prompt for credentials
- Credentials stored in browser session
- All API endpoints protected
- 401 Unauthorized returned without valid credentials

## Resolution Settings

**Default:** Camera uses UXGA (1600x1200) with PSRAM, SVGA (800x600) without.

**To change resolution**, edit `camera_config.h`:
```cpp
config.frame_size = FRAMESIZE_QVGA;  // Lower for faster streaming
config.frame_size = FRAMESIZE_VGA;   // Good balance
config.frame_size = FRAMESIZE_UXGA;  // Highest quality (needs PSRAM)
```

## Memory Considerations

**PSRAM:** Required for resolutions above SVGA
- Check with `psramFound()` in serial output
- ESP32-CAM AI-Thinker usually includes PSRAM
- Some cheap clones may not have PSRAM

**SPIFFS:** Used for image storage
- Default partition scheme: `huge_app.csv`
- To maximize storage, adjust partition scheme in `platformio.ini`
- Monitor space: `SPIFFS.totalBytes()` and `SPIFFS.usedBytes()`

## Power Supply Requirements

**Minimum:** 5V @ 500mA
**Recommended:** 5V @ 1A

⚠️ Insufficient power causes:
- Brownout detector resets
- Camera initialization failures
- WiFi disconnections
- Display glitches

**Don't power from:**
- Computer USB (often insufficient current)
- 3.3V regulator (ESP32-CAM needs 5V)

**Do power from:**
- Quality USB power adapter (5V 1A+)
- Benchtop power supply
- Battery pack (with voltage regulator)

## Network Configuration

**WiFi Requirements:**
- 2.4GHz network (ESP32 doesn't support 5GHz)
- WPA2 security supported
- Open networks supported
- WPA3 NOT supported on most ESP32

**Firewall:** May need to allow:
- Port 80 (HTTP)
- mDNS if using hostname

## CORS and API Access

The current implementation includes basic CORS headers for API endpoints.

**If you need to access from external applications:**
- API endpoints return JSON data
- Use standard HTTP requests
- No authentication required (security consideration!)

## Troubleshooting Checklist

Before reporting issues, verify:
- [ ] Power supply provides 5V @ 500mA+
- [ ] WiFi credentials are correct
- [ ] Using 2.4GHz WiFi (not 5GHz)
- [ ] PSRAM detected (for high resolutions)
- [ ] No GPIO pin conflicts
- [ ] All libraries installed via PlatformIO
- [ ] Serial monitor shows no errors
- [ ] Camera module properly seated
- [ ] Display shows initialization messages

## Performance Tuning

### Increase Frame Rate
- Lower resolution (QVGA = fastest)
- Increase JPEG quality number (lower quality = faster)
- Reduce number of concurrent connections
- Use wired Ethernet (if adapter available)

### Improve Image Quality
- Lower JPEG quality number (higher quality)
- Increase resolution
- Ensure good lighting
- Adjust brightness/contrast settings
- Focus camera lens

### Reduce Latency
- Position ESP32 closer to router
- Reduce WiFi interference
- Use 5GHz router on 2.4GHz-only channel
- Disable power saving on ESP32

## Development Tips

**Serial Debugging:**
- Baud rate: 115200
- Enable debug output: `#define CORE_DEBUG_LEVEL 3`
- Use `Serial.printf()` for formatted output

**OTA Updates (Future):**
- Reserve partition space in `platformio.ini`
- Implement OTA handlers
- Upload via `/update` endpoint

**Custom Web Interface:**
- Modify `include/web_interface.h`
- All HTML/CSS/JS in single string
- Use raw string literals for readability
- Minify for production to save flash

## License and Attribution

- ESP32 Arduino Core: LGPL 2.1
- TFT_eSPI: Free BSD License
- This project: See LICENSE file

Always respect component licenses when distributing modified versions.
