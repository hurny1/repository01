# Copilot Instructions for camera-and-cyd

## Project Overview

This is an ESP32-based camera system for the CYD (Cheap Yellow Display) featuring:
- Live MJPEG video streaming over WiFi
- Web-based camera controls with HTTP Basic Authentication
- ST7789 TFT display integration (240x320)
- Image capture and storage to SPIFFS
- OV2640 camera module support

**Target Audience:** IoT developers, embedded systems enthusiasts, ESP32 makers

## Technology Stack

### Hardware
- **Microcontroller:** ESP32 (dual-core, WiFi enabled)
- **Camera Module:** OV2640 (2MP) or compatible ESP32 camera
- **Display:** ST7789 TFT (240x320 pixels)
- **Storage:** SPIFFS filesystem for web files and captured images
- **Memory:** PSRAM (4MB) for frame buffers

### Software
- **Platform:** PlatformIO (preferred) with Arduino framework
- **Language:** C++ (Arduino/ESP-IDF style)
- **Build System:** PlatformIO
- **Key Libraries:**
  - ESP32 Camera (esp_camera.h)
  - TFT_eSPI (display driver)
  - WiFi and WebServer (ESP32 core)
  - SPIFFS (file system)
  - ArduinoJson (JSON handling)

### Build Commands
- **Build:** `pio run`
- **Upload:** `pio run -t upload`
- **Monitor:** `pio device monitor`
- **Clean:** `pio run -t clean`

## Project Structure

```
repository01/
├── src/
│   └── main.cpp              # Main application logic
├── include/
│   ├── camera_config.h       # Camera pin configuration
│   └── web_interface.h       # Web UI HTML/CSS/JS (as C++ string)
├── platformio.ini            # PlatformIO configuration and dependencies
├── .gitignore                # Excludes build artifacts, node_modules, etc.
└── docs/                     # Documentation (README, ARCHITECTURE, etc.)
```

## Coding Guidelines

### General C++ Style
- **Naming Conventions:**
  - Variables: `camelCase` (e.g., `cameraSettings`, `lastPreviewUpdate`)
  - Constants: `SCREAMING_SNAKE_CASE` for #defines (e.g., `TOUCH_CS`, `TOUCH_IRQ`)
  - Functions: `camelCase` (e.g., `initCamera()`, `updateCameraSettings()`)
  - Structs/Classes: `PascalCase` (e.g., `CameraSettings`)
  
- **Code Organization:**
  - Keep `main.cpp` focused on application logic
  - Move hardware-specific configurations to header files
  - Use function prototypes for clarity
  - Group related functionality together

- **Comments:**
  - Use `//` for single-line comments
  - Use `/* */` for block comments and file headers
  - Document hardware pin assignments clearly
  - Explain non-obvious logic or ESP32-specific quirks

### ESP32-Specific Guidelines
- **Memory Management:**
  - Always return camera frame buffers with `esp_camera_fb_return()`
  - Check PSRAM availability before using high resolutions
  - Monitor heap usage in loops (use `ESP.getFreeHeap()` for debugging)
  - Be cautious with String objects; prefer `const char*` for constants

- **WiFi and Networking:**
  - Always check WiFi connection status before network operations
  - Use `WiFi.status() == WL_CONNECTED`
  - Disable WiFi power saving if stream quality is poor: `WiFi.setSleep(false)`
  - Handle client disconnections gracefully

- **Camera Operations:**
  - Initialize camera before WiFi to save boot time
  - Use PSRAM for frame buffers when available (`config.fb_location = CAMERA_FB_IN_PSRAM`)
  - Always check `esp_camera_init()` return value
  - Return frame buffers immediately after use to prevent memory leaks

- **Display (TFT_eSPI):**
  - All display configuration should be in build flags (`platformio.ini`)
  - Use `tft.setTextColor()` and `tft.setTextSize()` for consistency
  - Clear areas before updating to prevent artifacts
  - Minimize display updates during streaming for performance

### Web Interface
- **HTML/CSS/JS:**
  - Embedded as raw string literals in `web_interface.h`
  - Keep inline for single-file deployment simplicity
  - Use responsive CSS for mobile compatibility
  - Minimize JavaScript dependencies (vanilla JS preferred)

- **API Design:**
  - Use RESTful conventions where practical
  - Return JSON for structured data (`/settings`)
  - Use HTTP status codes correctly (200, 401, 404, 500)
  - Always validate input parameters

## Security Best Practices

### Authentication
- **Default credentials MUST be changed** before deployment
- Current implementation uses HTTP Basic Authentication
- All endpoints require authentication

### Credential Management
- **NEVER commit real WiFi credentials or passwords to the repository**
- Use placeholder values in committed code:
  ```cpp
  const char* ssid = "YOUR_WIFI_SSID";
  const char* password = "YOUR_WIFI_PASSWORD";
  const char* www_username = "admin";
  const char* www_password = "esp32cam";
  ```
- Consider creating a `secrets.h` file and adding it to `.gitignore`
- Document credential changes in SECURITY.md

### Security Limitations
- **No HTTPS:** All traffic is unencrypted
- **Basic Auth:** Credentials sent in Base64 (easily decoded)
- **Local Network Only:** Not designed for internet exposure
- See SECURITY.md for detailed security considerations

## Common Tasks

### Adding a New Camera Setting
1. Add property to `CameraSettings` struct in `main.cpp`
2. Update `updateCameraSettings()` to apply the new setting
3. Add control to web interface in `web_interface.h`
4. Update `/settings` endpoint handlers (GET and POST)
5. Update display output in `drawControls()` if needed

### Changing Camera Pins
1. Edit `include/camera_config.h`
2. Update pin definitions in `camera_pins` struct
3. Document changes in WIRING.md
4. Ensure no conflicts with display pins (see platformio.ini)

### Modifying Display Layout
1. Edit `drawControls()` function in `main.cpp`
2. Use TFT_eSPI drawing functions
3. Test with actual hardware (display simulator not available)
4. Consider text size and color for readability

### Adding New Web Endpoints
1. Define handler function (follow naming pattern: `handle*`)
2. Add route in `initWebServer()` with `server.on()`
3. Include authentication check with `checkAuthentication()`
4. Document in ARCHITECTURE.md API endpoints section
5. Update web interface if UI controls needed

## Testing

### Manual Testing
- **Primary Method:** This is an embedded hardware project; automated tests are minimal
- **Serial Monitor:** Essential for debugging (115200 baud)
- **Web Interface:** Test all controls and endpoints manually
- **Physical Hardware:** Final validation requires actual ESP32/camera/display

### Testing Checklist
- [ ] Camera initializes without errors
- [ ] WiFi connects and displays IP address
- [ ] Web interface accessible and loads correctly
- [ ] Authentication rejects invalid credentials
- [ ] Video stream displays in browser
- [ ] All camera controls work as expected
- [ ] Image capture saves to SPIFFS
- [ ] Display shows correct status information
- [ ] Memory usage remains stable (check heap)

### Debugging Tips
- Enable verbose serial output for troubleshooting
- Check PSRAM availability: Look for "PSRAM found" in serial output
- Monitor heap usage if experiencing crashes: `Serial.printf("Free heap: %d\n", ESP.getFreeHeap())`
- Use `--no-pager` with git commands for cleaner output
- Check camera connections if "Camera init failed" appears

## Hardware Considerations

### Pin Conflicts
- **Critical:** Avoid GPIO pins already used by TFT display:
  - MOSI: GPIO13, SCLK: GPIO14, CS: GPIO15, DC: GPIO2, BL: GPIO21
- Camera uses: GPIO0, 4, 5, 18, 19, 22, 23, 25, 26, 27, 32, 34, 35, 39
- Touch: GPIO33 (CS), GPIO36 (IRQ)

### Power Requirements
- Minimum 1A @ 5V USB power supply
- Current draw varies: 150mA (idle) to 400mA (streaming/capture)
- Use quality USB cable to prevent brownouts
- Consider external power if WiFi is unstable

### Frame Size vs Performance
- QVGA (320x240): Fast, low memory, good for testing
- VGA (640x480): Balanced quality and performance
- SVGA (800x600): Requires PSRAM, moderate bandwidth
- UXGA (1600x1200): Maximum quality, requires PSRAM, high bandwidth

## References

- [Project README](../README.md) - Setup and usage instructions
- [ARCHITECTURE.md](../ARCHITECTURE.md) - System architecture and data flow
- [SECURITY.md](../SECURITY.md) - Security considerations and best practices
- [WIRING.md](../WIRING.md) - Hardware connections and pinout
- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Camera Library](https://github.com/espressif/esp32-camera)
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)

## Important Reminders

- **This is embedded hardware code** - test on actual hardware, not simulation
- **Memory is limited** - be mindful of heap usage, avoid memory leaks
- **WiFi credentials** - never commit real credentials to git
- **Authentication** - always require authentication for all endpoints
- **Frame buffers** - always return camera frame buffers after use
- **Serial output** - use liberally for debugging hardware issues
- **Documentation** - update relevant .md files when making architectural changes
