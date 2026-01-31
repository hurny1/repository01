# CYD Upload Guide - Best Practices for Flashing Firmware

This guide covers the best methods for uploading firmware to your CYD (Cheap Yellow Display) ESP32 board.

## Table of Contents
- [Prerequisites](#prerequisites)
- [USB Driver Installation](#usb-driver-installation)
- [Upload Methods](#upload-methods)
- [Step-by-Step Upload Process](#step-by-step-upload-process)
- [Boot Mode and Buttons](#boot-mode-and-buttons)
- [Troubleshooting Upload Issues](#troubleshooting-upload-issues)
- [Advanced Options](#advanced-options)

## Prerequisites

### Required Hardware
- **CYD Board** with USB cable
- **Quality USB Cable** (data-capable, not charge-only)
- **Computer** with USB port
- **Stable Power Supply** (5V @ 1A recommended)

### Required Software
- **PlatformIO** (recommended) OR **Arduino IDE**
- **USB Drivers** (CH340 or CP210x, depending on your CYD variant)
- **Git** (for cloning the repository)

## USB Driver Installation

### Identifying Your USB Chip

CYD boards typically use one of two USB-to-serial chips:
- **CH340** (most common on cheaper variants)
- **CP210x** (Silicon Labs, on some variants)

To identify which chip your board uses:
1. Connect CYD to computer via USB
2. Check Device Manager (Windows) or `lsusb` (Linux/Mac)
3. Look for "CH340" or "CP210x" in the device name

### Windows Driver Installation

#### CH340 Drivers (Most Common)
1. Download CH340 drivers from [official source](http://www.wch.cn/downloads/CH341SER_EXE.html)
2. Extract and run the installer
3. Restart your computer
4. Connect CYD - should appear as COM port in Device Manager

**Direct Download**: Search "CH340 driver Windows" if the official link is unavailable

#### CP210x Drivers
1. Download from [Silicon Labs website](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
2. Run the installer
3. Restart computer
4. Connect CYD - should appear as COM port

### macOS Driver Installation

#### CH340 Drivers
1. Download macOS CH340 driver
2. Install and restart
3. Check with: `ls /dev/cu.*`
4. Look for `/dev/cu.wchusbserial*`

**Note**: macOS Ventura+ may have issues with CH340. Try signed drivers or use alternative methods.

#### CP210x Drivers
Usually works natively on macOS 10.9+, but if needed:
1. Download from Silicon Labs
2. Install package
3. Restart

### Linux Driver Installation

#### CH340 Drivers
Usually built into kernel (Linux 2.6.24+):
```bash
# Check if driver is loaded
lsmod | grep ch341

# If not, load manually
sudo modprobe ch341
```

#### CP210x Drivers
Also typically built-in:
```bash
# Check driver
lsmod | grep cp210x

# Device should appear at /dev/ttyUSB0 (or similar)
ls -l /dev/ttyUSB*
```

**Permissions**: Add user to dialout group:
```bash
sudo usermod -a -G dialout $USER
# Log out and back in for changes to take effect
```

## Upload Methods

### Method 1: PlatformIO CLI (Recommended)

**Best for**: Command-line users, automation, CI/CD

**Advantages**:
- Fast and reliable
- Automatic dependency management
- Best compatibility
- Easy to script

**Installation**:
```bash
# Install PlatformIO Core
pip install platformio

# Or via Homebrew (macOS)
brew install platformio
```

**Upload Command**:
```bash
cd camera-and-cyd
pio run -t upload
```

### Method 2: PlatformIO IDE (VS Code)

**Best for**: Visual development, debugging, GUI preference

**Advantages**:
- Integrated development environment
- Built-in serial monitor
- Easy configuration
- Great for beginners

**Installation**:
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install PlatformIO IDE extension
3. Open project folder
4. Click "Upload" button in status bar

### Method 3: Arduino IDE

**Best for**: Arduino ecosystem users

**Setup Required**:
1. Install ESP32 board support
2. Manually install libraries (TFT_eSPI, ArduinoJson)
3. Configure board settings manually

**Note**: PlatformIO is recommended over Arduino IDE for this project due to easier configuration.

## Step-by-Step Upload Process

### First-Time Setup

1. **Install USB Drivers**
   ```bash
   # Verify device is recognized
   # Windows: Check Device Manager for COM port
   # Linux: ls /dev/ttyUSB*
   # macOS: ls /dev/cu.*
   ```

2. **Clone Repository**
   ```bash
   git clone https://github.com/hurny1/camera-and-cyd.git
   cd camera-and-cyd
   ```

3. **Configure WiFi**
   Edit `src/main.cpp`:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

4. **Connect CYD**
   - Plug USB cable into CYD
   - Connect to computer
   - Wait for driver recognition (first time may take a moment)

### Upload Process

#### Using PlatformIO CLI

```bash
# Navigate to project directory
cd camera-and-cyd

# Build project (optional - upload does this automatically)
pio run

# Upload to CYD
pio run -t upload

# Upload and monitor in one command
pio run -t upload && pio device monitor
```

#### Using PlatformIO IDE (VS Code)

1. Open project folder in VS Code
2. PlatformIO should auto-detect `platformio.ini`
3. Click **PlatformIO: Upload** in the bottom toolbar
4. Wait for upload to complete
5. Click **PlatformIO: Serial Monitor** to view output

### Monitoring Serial Output

After upload, monitor to verify:
```bash
pio device monitor

# Or specify port and baud rate
pio device monitor -p /dev/ttyUSB0 -b 115200
```

Look for:
- ✅ WiFi connection successful
- ✅ IP address displayed
- ✅ Camera initialization successful
- ✅ PSRAM detected (if available)

## Boot Mode and Buttons

### CYD Button Functions

Most CYD boards have two buttons:
- **BOOT (GPIO0)** - Boot mode selector
- **RST/EN** - Reset button

### Normal Boot (Running Mode)

Simply power on or press RST button:
```
1. Press RST
2. Release RST
→ ESP32 boots normally and runs your program
```

### Flash/Upload Mode

**Automatic Mode** (Most Common):
- PlatformIO/Arduino IDE handles this automatically
- No button pressing needed
- Works with most USB-to-serial chips

**Manual Mode** (If automatic fails):
```
1. Hold down BOOT button
2. Press and release RST button
3. Release BOOT button
→ ESP32 enters flash mode
4. Run upload command immediately
```

**Verification**: Serial monitor shows:
```
waiting for download
```

### If Upload Fails

Try manual boot mode sequence:
1. Prepare upload command: `pio run -t upload`
2. Hold BOOT button
3. Press RST button
4. Release both buttons
5. Execute upload command within 5 seconds

## Troubleshooting Upload Issues

### Error: Serial Port Not Found

**Symptoms**:
```
Error: could not open port '/dev/ttyUSB0'
Error: COM port not available
```

**Solutions**:
1. **Check USB cable** - Use data cable, not charge-only
2. **Verify driver installation** - Reinstall CH340/CP210x drivers
3. **Check port permissions** (Linux):
   ```bash
   sudo chmod 666 /dev/ttyUSB0
   # Or add user to dialout group permanently
   sudo usermod -a -G dialout $USER
   ```
4. **Try different USB port** - Some ports may have power issues
5. **Specify port manually**:
   ```bash
   # In platformio.ini, add:
   upload_port = COM3        # Windows
   upload_port = /dev/ttyUSB0  # Linux
   upload_port = /dev/cu.wchusbserial1420  # macOS
   ```

### Error: Upload Timeout

**Symptoms**:
```
A fatal error occurred: Timed out waiting for packet header
```

**Solutions**:
1. **Use manual boot mode** (hold BOOT, press RST)
2. **Lower upload speed** - Edit `platformio.ini`:
   ```ini
   upload_speed = 115200
   ```
3. **Check USB cable quality** - Poor cables cause timeouts
4. **Disable antivirus temporarily** - May block serial access
5. **Try different USB port** - Front panel ports sometimes problematic

### Error: Serial Exception

**Symptoms**:
```
serial.serialutil.SerialException: [Errno 13] Permission denied
```

**Solution (Linux/macOS)**:
```bash
# Add user to dialout/uucp group
sudo usermod -a -G dialout $USER

# Log out and back in, then verify
groups
```

### Error: Not Enough Space

**Symptoms**:
```
Sketch too big
region `irom0_0_seg' overflowed
```

**Solutions**:
1. **Verify partition scheme** in `platformio.ini`:
   ```ini
   board_build.partitions = huge_app.csv
   ```
2. **Reduce debug level**:
   ```ini
   build_flags = 
       -DCORE_DEBUG_LEVEL=0  ; Minimal logging
   ```
3. **Remove unused libraries**

### Error: Brown-out Detector

**Symptoms**:
```
Brownout detector was triggered
rst:0x10 (RTCWDT_RTC_RESET)
```

**Solutions**:
1. **Use better power supply** - 5V @ 1A minimum
2. **Try different USB cable** - Higher quality cable
3. **Use powered USB hub**
4. **Add bulk capacitor** (100µF) near ESP32 power pins
5. **Don't power from computer USB** - Often insufficient current

### Error: Camera Init Failed

**Symptoms** (after upload):
```
Camera init failed with error 0x105
Camera init failed with error 0x20001
```

**Solutions**:
1. Check camera connections (see WIRING.md)
2. Verify power supply (camera needs stable power)
3. Ensure PSRAM is available
4. Try lower resolution in camera_config.h

## Advanced Options

### Upload Speed Optimization

**Default**: 460800 baud (safe for most boards)

**Faster Upload** (if stable):
```ini
; In platformio.ini
upload_speed = 921600
```

**Slower Upload** (if problems):
```ini
upload_speed = 115200
```

### Specify Upload Port

```ini
; In platformio.ini
[env:esp32dev]
upload_port = COM3              ; Windows
; upload_port = /dev/ttyUSB0    ; Linux
; upload_port = /dev/cu.wchusbserial1420  ; macOS
```

Or via command line:
```bash
pio run -t upload --upload-port /dev/ttyUSB0
```

### Erase Flash Before Upload

Useful for troubleshooting:
```bash
# Erase entire flash
pio run -t erase

# Then upload fresh
pio run -t upload
```

### Upload Filesystem (SPIFFS)

If you add data files:
```bash
# Upload filesystem
pio run -t uploadfs
```

### Monitor Baud Rate

If serial output is garbled:
```ini
; In platformio.ini
monitor_speed = 115200  ; Match your Serial.begin() value
```

### OTA Upload (Future Enhancement)

Over-The-Air updates allow wireless upload:
1. Implement OTA handlers in code
2. Upload via web interface or ArduinoOTA
3. No USB connection needed

**Note**: Not currently implemented in this project.

## Upload Speed Comparison

| Method | Speed | Reliability | Ease of Use |
|--------|-------|-------------|-------------|
| PlatformIO CLI | ⚡⚡⚡ Fast | ✅ Excellent | 🔧 Intermediate |
| PlatformIO IDE | ⚡⚡⚡ Fast | ✅ Excellent | ✅ Easy |
| Arduino IDE | ⚡⚡ Moderate | ⚠️ Good | ✅ Easy |
| OTA Updates | ⚡⚡⚡ Fast | ⚠️ Network dependent | 🔧 Advanced |

## Best Practices

### ✅ Do's

1. **Use quality USB cables** - Data cables, not charge-only
2. **Install proper drivers first** - CH340 or CP210x
3. **Use PlatformIO** - Better dependency management
4. **Monitor serial output** - Verify successful boot
5. **Keep backups** - Save working configurations
6. **Use version control** - Track your changes
7. **Test incrementally** - Upload and test often

### ❌ Don'ts

1. **Don't power from computer USB** - Often insufficient current
2. **Don't skip driver installation** - Required for communication
3. **Don't use damaged cables** - Causes upload failures
4. **Don't interrupt uploads** - May corrupt firmware
5. **Don't forget WiFi config** - Won't connect without credentials
6. **Don't use 5GHz WiFi** - ESP32 only supports 2.4GHz

## Quick Reference

### Successful Upload Checklist

- [ ] USB drivers installed (CH340/CP210x)
- [ ] Quality USB cable connected
- [ ] Device recognized (check Device Manager / lsusb)
- [ ] PlatformIO installed
- [ ] WiFi credentials configured in `src/main.cpp`
- [ ] Project compiles without errors (`pio run`)
- [ ] Upload completes successfully (`pio run -t upload`)
- [ ] Serial monitor shows boot messages
- [ ] WiFi connects and IP address displayed
- [ ] Web interface accessible

### Common Commands

```bash
# Check PlatformIO version
pio --version

# List connected devices
pio device list

# Build only (no upload)
pio run

# Upload firmware
pio run -t upload

# Upload and monitor
pio run -t upload && pio device monitor

# Monitor serial output
pio device monitor

# Clean build files
pio run -t clean

# Update PlatformIO
pip install -U platformio
```

## Getting Help

If you continue to experience upload issues:

1. **Check serial output** for specific error messages
2. **Review this guide** for relevant troubleshooting
3. **Verify hardware**:
   - USB cable is data-capable
   - Drivers are correctly installed
   - Power supply is adequate
4. **Try different upload method** (CLI vs IDE)
5. **Check GitHub issues** for similar problems
6. **Open new issue** with:
   - Full error message
   - Operating system
   - PlatformIO version
   - Upload command used
   - Serial monitor output

## Additional Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Espressif Flashing Guide](https://docs.espressif.com/projects/esptool/en/latest/)
- [CH340 Driver Issues](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers)

---

**Next Steps**: After successful upload, see [QUICKSTART.md](QUICKSTART.md) for using the camera system.
