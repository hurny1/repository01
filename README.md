# Camera and CYD - ESP32 Camera System

**Project Name:** camera-and-cyd

A complete camera system for the CYD (Cheap Yellow Display) with ESP32, featuring live video streaming, web-based controls, and on-device display.

**Single Board Setup:** This project uses the CYD's built-in ESP32 to control BOTH a camera module AND the display.

## Features

- 📹 **Live Video Streaming** - Real-time MJPEG stream accessible via web browser
- 🎛️ **Camera Controls** - Adjust brightness, contrast, saturation, and quality
- 💾 **Image Storage** - Capture and save images to SPIFFS
- 📺 **CYD Display** - View camera info and status on the built-in TFT display
- 🌐 **Web Interface** - Beautiful, responsive web UI for full camera control
- 🔧 **Easy Configuration** - Simple WiFi setup and customizable settings

## Hardware Requirements

### Required Components
1. **CYD (Cheap Yellow Display)**
   - ESP32 with ST7789 TFT display (240x320)
   - Built-in touch screen
   - This is your main board
2. **Camera Module** (one of the following)
   - OV2640 camera module (recommended, 2MP)
   - OV7670 camera module (VGA)
   - Any ESP32-compatible camera module
3. **Power Supply**
   - 5V USB power supply (1A minimum)
   - Quality USB cable
4. **Wiring**
   - Dupont wires or soldering for camera connections
   - See WIRING.md for detailed pin connections

### Pin Connections

#### Camera Module to CYD Connections

Connect your camera module to the CYD's ESP32 GPIO pins:

```
Camera Module       CYD GPIO
-------------       --------
D0         -------> GPIO5
D1         -------> GPIO18
D2         -------> GPIO19
D3         -------> GPIO4
D4         -------> GPIO35
D5         -------> GPIO34
D6         -------> GPIO39
D7         -------> GPIO32
XCLK       -------> GPIO0
PCLK       -------> GPIO22
VSYNC      -------> GPIO25
HREF       -------> GPIO23
SDA        -------> GPIO26
SCL        -------> GPIO27
3.3V       -------> 3.3V
GND        -------> GND
```

**Note:** GPIO4 is used for D3 (instead of GPIO21) to avoid conflict with display backlight.

#### CYD Display Pins (Pre-wired)

The display is already connected inside the CYD:

```
TFT_MOSI: GPIO13
TFT_SCLK: GPIO14
TFT_CS:   GPIO15
TFT_DC:   GPIO2
TFT_BL:   GPIO21 (backlight)
```

## Software Setup

### Prerequisites
- [PlatformIO](https://platformio.org/) installed
- USB drivers for ESP32
- Arduino framework

### Installation Steps

1. **Clone the repository**
   ```bash
   git clone https://github.com/hurny1/camera-and-cyd.git
   cd camera-and-cyd
   ```

2. **Configure WiFi and Authentication**
   Edit `src/main.cpp` and update your WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   
   // Web authentication credentials - CHANGE THESE!
   const char* www_username = "admin";
   const char* www_password = "esp32cam";
   ```
   
   ⚠️ **Security Note:** Always change the default username and password!

3. **Build and Upload**
   ```bash
   pio run -t upload
   ```

4. **Monitor Serial Output**
   ```bash
   pio device monitor
   ```
   
   Look for the IP address in the serial output.

5. **Access Web Interface**
   Open a web browser and navigate to:
   ```
   http://[ESP32_IP_ADDRESS]/
   ```
   
   You will be prompted for authentication:
   - **Username:** `admin` (default)
   - **Password:** `esp32cam` (default)
   
   ⚠️ **Change the default credentials in `src/main.cpp` for security!**

## Usage

### Authentication

The system uses HTTP Basic Authentication to protect access to the camera stream and controls.

**Default Credentials:**
- Username: `admin`
- Password: `esp32cam`

⚠️ **Security Recommendations:**
1. **Change default credentials immediately** in `src/main.cpp`
2. Use a strong password (at least 12 characters)
3. Keep credentials secure and don't share them
4. Only use on trusted networks
5. Consider using a VPN if accessing remotely

**To change credentials:**
Edit these lines in `src/main.cpp`:
```cpp
const char* www_username = "admin";        // Change this
const char* www_password = "esp32cam";     // Change this
```

### Web Interface Features

1. **Live Video Stream**
   - Automatically starts when you open the web page
   - Real-time MJPEG stream from the camera

2. **Camera Controls**
   - **Brightness**: Adjust from -2 to 2
   - **Contrast**: Adjust from -2 to 2
   - **Saturation**: Adjust from -2 to 2
   - **Quality**: Set JPEG quality (0=best, 63=worst)
   - **Mirror**: Flip image horizontally
   - **Vertical Flip**: Flip image vertically

3. **Actions**
   - **Apply Settings**: Save and apply camera settings
   - **Capture Image**: Take a photo and save to SPIFFS
   - **Refresh Settings**: Reload current settings from device

### CYD Display

The CYD display shows:
- Camera system status
- Current IP address
- Current brightness, contrast, and quality settings
- Connection status
- Image capture notifications

### API Endpoints

All endpoints require HTTP Basic Authentication:

- `GET /` - Main web interface (requires authentication)
- `GET /stream` - MJPEG video stream (requires authentication)
- `GET /settings` - Get current camera settings (JSON, requires authentication)
- `POST /settings` - Update camera settings (requires authentication)
- `GET /capture` - Capture and save image (requires authentication)
- `GET /images` - List saved images (requires authentication)

## Troubleshooting

### Camera Not Working
1. Check that PSRAM is available (message in serial output)
2. Verify camera pin connections
3. Try lowering frame size in `camera_config.h`
4. Check power supply (camera needs stable 5V)

### WiFi Not Connecting
1. Verify SSID and password are correct
2. Check WiFi signal strength
3. Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)

### Display Not Showing
1. Verify TFT_eSPI library is installed
2. Check display pin configuration in `platformio.ini`
3. Ensure backlight pin is configured correctly

### Poor Video Quality
1. Increase JPEG quality (lower number = better quality)
2. Ensure good lighting conditions
3. Check if PSRAM is available for higher resolution

## File Structure

```
camera-and-cyd/
├── platformio.ini          # PlatformIO configuration
├── src/
│   └── main.cpp           # Main application code
├── include/
│   ├── camera_config.h    # ESP32-CAM pin configuration
│   └── web_interface.h    # Web UI HTML/CSS/JS
├── data/                  # SPIFFS data (for saved images)
├── WIRING.md              # Detailed wiring guide
└── README.md              # This file
```

## Customization

### Change Camera Resolution
Edit `camera_config.h` to modify frame size:
```cpp
config.frame_size = FRAMESIZE_QVGA;  // Options: QVGA, VGA, SVGA, XGA, UXGA
```

### Modify Display Layout
Edit the `drawControls()` function in `main.cpp` to customize the CYD display.

### Update Web Interface
Modify `include/web_interface.h` to change the web UI design.

## Technical Details

### Memory Usage
- PSRAM: Used for camera frame buffers (if available)
- SPIFFS: Used for web files and image storage
- Heap: ~100KB available for operation

### Performance
- Frame Rate: 10-15 FPS (depends on settings and network)
- Resolution: Up to 1600x1200 (UXGA) with PSRAM
- Latency: ~200-500ms (typical)

### Power Consumption
- Active streaming: ~300-400mA @ 5V
- Idle: ~150-200mA @ 5V

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

This project is licensed under the terms specified in the LICENSE file.

## Acknowledgments

- ESP32 Arduino Core
- TFT_eSPI library by Bodmer
- ESP32 Camera library

## Support

For issues and questions:
1. Check the troubleshooting section
2. Review serial monitor output
3. Open an issue on GitHub

## Future Enhancements

- [ ] Motion detection
- [ ] Time-lapse recording
- [ ] SD card storage support
- [ ] Face detection
- [ ] OTA (Over-The-Air) updates
- [ ] Touch screen controls on CYD
- [ ] Multiple camera support
- [ ] Cloud storage integration
