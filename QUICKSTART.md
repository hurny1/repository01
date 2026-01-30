# Quick Start Guide

## 🚀 Get Started in 5 Minutes

### Step 0: Hardware Setup
1. **Get your CYD board** - The ESP32 with built-in display
2. **Get a camera module** - OV2640 recommended (2MP camera)
3. **Wire the camera to CYD** - Follow pin connections in WIRING.md:
   - Connect camera data pins (D0-D7) to CYD GPIO pins
   - Connect camera I2C (SDA/SCL) to GPIO26/27
   - Connect camera power (3.3V) and ground (GND)
4. **Power the CYD** - Use 5V USB supply (1A minimum)

### Step 1: Configure WiFi
Edit `src/main.cpp` lines 11-12:
```cpp
const char* ssid = "YourWiFiName";      // Change this
const char* password = "YourPassword";   // Change this
```

### Step 2: Upload Code
```bash
# Install PlatformIO if you haven't already
pip install platformio

# Navigate to project directory
cd repository01

# Build and upload
pio run -t upload

# Monitor serial output to get IP address
pio device monitor
```

### Step 3: Access Web Interface
1. Look for the IP address in serial monitor output
2. Open browser and go to: `http://[IP_ADDRESS]/`
3. You should see the live camera stream!

## 📺 What You'll See

### On the CYD Display:
```
┌─────────────────────────┐
│    CYD Camera           │
│                         │
│ IP: 192.168.1.100       │
│                         │
│ Brightness: 0           │
│ Contrast: 0             │
│ Quality: 10             │
│                         │
│ Use web interface       │
│ to control camera       │
└─────────────────────────┘
```

### In Your Web Browser:
```
┌────────────────────────────────────────┐
│  🎥 CYD ESP32 Camera                   │
│  Live Video Stream & Camera Controls   │
├────────────────────────────────────────┤
│  ┌──────────────────────────────────┐ │
│  │                                  │ │
│  │      [Live Video Stream]         │ │
│  │                                  │ │
│  └──────────────────────────────────┘ │
│                                        │
│  Camera Settings                       │
│  ┌──────────────────────────────────┐ │
│  │ Brightness:  [====|====]      0  │ │
│  │ Contrast:    [====|====]      0  │ │
│  │ Saturation:  [====|====]      0  │ │
│  │ Quality:     [==|========]    10 │ │
│  │                                  │ │
│  │ ☐ Mirror   ☐ Vertical Flip      │ │
│  │                                  │ │
│  │ [Apply Settings] [Capture Image] │ │
│  └──────────────────────────────────┘ │
└────────────────────────────────────────┘
```

## 🎮 Controls

### Brightness (-2 to +2)
- **-2**: Very dark
- **-1**: Slightly dark
- **0**: Normal (default)
- **+1**: Slightly bright
- **+2**: Very bright

### Contrast (-2 to +2)
- **-2**: Very low contrast
- **-1**: Low contrast
- **0**: Normal (default)
- **+1**: High contrast
- **+2**: Very high contrast

### Quality (0 to 63)
- **0-10**: Best quality, larger files
- **11-30**: Good quality, medium files
- **31-63**: Lower quality, smaller files

### Mirror & Flip
- **Mirror**: Flip image horizontally
- **Vertical Flip**: Flip image vertically

## 🔧 Common Adjustments

### Too Dark?
1. Increase Brightness to +1 or +2
2. Ensure good lighting in the room
3. Check camera lens for obstructions

### Poor Quality?
1. Lower Quality value (try 5-10)
2. Ensure stable WiFi connection
3. Check power supply is adequate (5V, 500mA+)

### Stream Not Loading?
1. Check IP address is correct
2. Verify WiFi connection
3. Restart ESP32 if needed
4. Check serial monitor for errors

## 📸 Capturing Images

1. Click "Capture Image" button
2. Image is saved to SPIFFS storage
3. Notification appears on CYD display
4. Access saved images via `/images` endpoint

## 🛠️ Troubleshooting

### Camera Won't Initialize
```
Error: Camera init failed with error 0x...
Solution: 
1. Check camera cable connections
2. Verify power supply (5V, 500mA minimum)
3. Try different USB cable/power source
```

### WiFi Won't Connect
```
Error: WiFi connection failed!
Solution:
1. Double-check SSID and password
2. Ensure 2.4GHz network (not 5GHz)
3. Move closer to router
4. Check router allows new connections
```

### Display is Blank
```
Error: Display not showing anything
Solution:
1. Check TFT_eSPI library is installed
2. Verify display connections
3. Adjust TFT_BL (backlight) pin
4. Try different display rotation
```

### Stream is Slow
```
Issue: Low frame rate or laggy stream
Solution:
1. Reduce camera resolution in camera_config.h
2. Improve WiFi signal strength
3. Close other browser tabs
4. Increase Quality value (lower quality = faster)
```

## 📱 Accessing from Mobile

The web interface is fully responsive and works great on mobile devices:

1. Connect your phone to the same WiFi
2. Open browser and enter ESP32 IP address
3. Bookmark for quick access
4. Use full-screen mode for best experience

## 🔒 Security Notes

⚠️ **Important**: This system has no built-in authentication!

- Anyone on your network can access the camera
- Consider adding password protection for production use
- Use on trusted networks only
- Don't expose directly to the internet

## 🎯 Next Steps

### Enhance Your System
1. Add motion detection
2. Implement time-lapse recording
3. Add SD card for more storage
4. Enable OTA updates
5. Add authentication
6. Integrate with home automation

### Learning Resources
- ESP32 Camera Documentation
- TFT_eSPI Library Examples
- PlatformIO Documentation
- Arduino ESP32 Core

## 💡 Tips & Tricks

### Better Performance
- Enable PSRAM for higher resolutions
- Use wired Ethernet adapter (if available)
- Position camera away from WiFi router
- Use quality power supply (not computer USB)

### Better Image Quality
- Adjust camera focus (twist lens slightly)
- Ensure adequate lighting (avoid backlight)
- Clean camera lens
- Experiment with settings

### Save Power
- Lower display brightness
- Reduce frame rate
- Use lower resolution
- Disable WiFi when not streaming

## 📞 Getting Help

If you encounter issues:
1. Check the [README.md](README.md) for detailed setup
2. Review [WIRING.md](WIRING.md) for connection diagrams
3. Read [ARCHITECTURE.md](ARCHITECTURE.md) for system design
4. Check serial monitor for error messages
5. Open an issue on GitHub

## ✅ Verification Checklist

- [ ] PlatformIO installed
- [ ] WiFi credentials configured
- [ ] Code uploaded successfully
- [ ] Serial monitor shows IP address
- [ ] CYD display shows camera info
- [ ] Web interface loads in browser
- [ ] Video stream is visible
- [ ] Controls work (brightness, contrast)
- [ ] Image capture works
- [ ] Settings persist across changes

Congratulations! Your CYD ESP32 Camera System is ready! 🎉
