# Important Configuration Notes

## System Architecture Options

This project supports two hardware configurations:

### Option 1: Two Separate Boards (Recommended)
- **ESP32-CAM**: Runs the camera and web server
- **CYD Display**: Shows camera status via WiFi connection
- Both boards connect to the same WiFi network
- No pin conflicts, easier setup

### Option 2: Single Board (Advanced)
- Single ESP32 with both camera module and display
- Requires careful pin assignment to avoid conflicts
- More compact but harder to configure

## Pin Conflict Warning

⚠️ **GPIO21 Conflict:** The default configuration has a pin conflict:
- Camera uses GPIO21 for D3 (data line)
- Display uses GPIO21 for TFT_BL (backlight)

**If using Option 1 (two boards):** No issue, each board uses its own GPIO21.

**If using Option 2 (single board):** You MUST reassign pins:
- Change display backlight to GPIO22 or GPIO33
- OR change camera D3 to another available GPIO
- Update both `platformio.ini` and `camera_config.h`

## WiFi Credentials

**Security Warning:** The default code has WiFi credentials hardcoded in `src/main.cpp`.

**Best Practice:**
1. Create a `secrets.h` file:
```cpp
#ifndef SECRETS_H
#define SECRETS_H
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
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
