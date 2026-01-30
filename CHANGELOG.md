# Changelog - camera-and-cyd

## [1.1.0] - 2026-01-30

### Added
- **Project Name:** Officially named "camera-and-cyd"
- **HTTP Basic Authentication** for web access
  - Username and password protection on all endpoints
  - Default credentials: admin/esp32cam (should be changed)
  - Browser login prompt for secure access
- Authentication documentation in all guides

### Security
- All web endpoints now require authentication
- Protected video stream from unauthorized access
- Protected camera controls and settings
- Protected image capture and storage access

## [1.0.0] - 2026-01-30

### Added
- Initial release of CYD ESP32 Camera System
- ESP32-CAM support with OV2640 camera
- Live MJPEG video streaming over HTTP
- Web interface for camera control
  - Brightness adjustment (-2 to +2)
  - Contrast adjustment (-2 to +2)
  - Saturation adjustment (-2 to +2)
  - JPEG quality control (0-63)
  - Image mirror and vertical flip options
- CYD TFT display integration (ST7789)
  - Camera status display
  - IP address display
  - Current settings display
  - Image capture notifications
- Image capture and storage to SPIFFS
- RESTful API endpoints for camera control
- Comprehensive documentation
  - Setup guide
  - Wiring diagrams
  - Troubleshooting section
- PlatformIO project configuration
- Example configuration file

### Technical Details
- Framework: Arduino
- Platform: ESP32
- Libraries: TFT_eSPI, ESP32 Camera, WebServer
- Supported resolutions: QVGA to UXGA (with PSRAM)
- Storage: SPIFFS for image storage
- Network: WiFi 2.4GHz, HTTP server on port 80

### Known Limitations
- Touch screen controls not yet implemented
- Preview on CYD display simplified (no live video decode)
- Maximum resolution depends on PSRAM availability
- No OTA update support yet
