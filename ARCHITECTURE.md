# System Architecture

## Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    CYD ESP32 Camera System                  │
└─────────────────────────────────────────────────────────────┘

┌──────────────────┐         ┌──────────────────┐
│   ESP32-CAM      │         │   CYD Display    │
│   (Camera)       │◄───────►│   (ESP32+TFT)    │
│                  │  WiFi   │                  │
│  - OV2640 Camera │         │  - ST7789 320x240│
│  - PSRAM 4MB     │         │  - Touch Screen  │
│  - JPEG Encoding │         │  - Status Display│
└──────────────────┘         └──────────────────┘
         │                            │
         │                            │
         └────────────┬───────────────┘
                      │
                 WiFi Network
                      │
         ┌────────────┴───────────────┐
         │                            │
┌────────▼────────┐         ┌─────────▼────────┐
│  Web Browser    │         │  Mobile Device   │
│  (Desktop/      │         │  (Phone/Tablet)  │
│   Laptop)       │         │                  │
│                 │         │                  │
│  - Live Stream  │         │  - Live Stream   │
│  - Controls     │         │  - Controls      │
│  - Settings     │         │  - Settings      │
└─────────────────┘         └──────────────────┘
```

## Component Flow

```
┌─────────────┐
│   Camera    │
│  Hardware   │
└──────┬──────┘
       │
       │ Image Capture
       ▼
┌─────────────┐
│   Camera    │
│   Sensor    │
│  (OV2640)   │
└──────┬──────┘
       │
       │ Raw Image Data
       ▼
┌─────────────┐
│    JPEG     │
│  Encoding   │
└──────┬──────┘
       │
       │ Compressed JPEG
       ▼
┌─────────────┐      ┌──────────────┐
│   PSRAM     │      │   SPIFFS     │
│   Buffer    │      │   Storage    │
└──────┬──────┘      └──────▲───────┘
       │                    │
       │ Stream             │ Save
       │                    │
       ▼                    │
┌─────────────┐      ┌──────┴───────┐
│   HTTP      │      │   Capture    │
│   Server    │      │   Function   │
└──────┬──────┘      └──────────────┘
       │
       │ MJPEG Stream
       ▼
┌─────────────┐
│   Web       │
│   Client    │
└─────────────┘
```

## Data Flow

### 1. Camera Initialization
```
main.cpp:setup()
    │
    ├─► initCamera()
    │   └─► getCameraConfig() (camera_config.h)
    │       └─► esp_camera_init()
    │
    ├─► initWiFi()
    │   └─► WiFi.begin(ssid, password)
    │
    ├─► initDisplay()
    │   └─► tft.init() + drawControls()
    │
    └─► initWebServer()
        └─► Setup HTTP routes
```

### 2. Video Streaming
```
Client Request: GET /stream
    │
    ▼
server.on("/stream")
    │
    └─► Loop while connected:
        │
        ├─► esp_camera_fb_get()
        │   └─► Capture frame from camera
        │
        ├─► Send MJPEG boundary
        │
        ├─► Send JPEG data
        │
        ├─► esp_camera_fb_return()
        │   └─► Release frame buffer
        │
        └─► Repeat
```

### 3. Settings Control
```
Web UI: Adjust slider
    │
    ▼
JavaScript: onChange event
    │
    ▼
POST /settings
    │
    ▼
Update cameraSettings struct
    │
    ▼
updateCameraSettings()
    │
    ├─► sensor->set_brightness()
    ├─► sensor->set_contrast()
    ├─► sensor->set_saturation()
    └─► sensor->set_quality()
```

### 4. Image Capture
```
Web UI: Click "Capture"
    │
    ▼
GET /capture
    │
    ▼
captureAndSave()
    │
    ├─► esp_camera_fb_get()
    │   └─► Get frame
    │
    ├─► SPIFFS.open(filename, WRITE)
    │   └─► Create file
    │
    ├─► file.write(jpeg_data)
    │   └─► Write to storage
    │
    ├─► Display notification on CYD
    │
    └─► Return success
```

## Web Interface Components

```
HTML Page (web_interface.h)
    │
    ├─► <img id="stream" src="/stream">
    │   └─► Displays live MJPEG stream
    │
    ├─► Camera Control Sliders
    │   ├─► Brightness (-2 to +2)
    │   ├─► Contrast (-2 to +2)
    │   ├─► Saturation (-2 to +2)
    │   └─► Quality (0 to 63)
    │
    ├─► Checkbox Options
    │   ├─► Mirror (horizontal flip)
    │   └─► Vertical Flip
    │
    └─► Action Buttons
        ├─► Apply Settings
        ├─► Capture Image
        └─► Refresh Settings
```

## API Endpoints

| Method | Endpoint | Description | Response |
|--------|----------|-------------|----------|
| GET | / | Main web interface | HTML page |
| GET | /stream | MJPEG video stream | multipart/x-mixed-replace |
| GET | /settings | Get camera settings | JSON object |
| POST | /settings | Update settings | Text confirmation |
| GET | /capture | Capture and save image | Text confirmation |
| GET | /images | List saved images | JSON array |

## Memory Map

```
┌────────────────────────────────────┐
│          ESP32 Memory              │
├────────────────────────────────────┤
│  Flash (4MB)                       │
│  ├─ Program Code (~1.5MB)          │
│  ├─ SPIFFS (Variable)              │
│  │  └─ Saved Images (.jpg)         │
│  └─ OTA Partition (Future)         │
├────────────────────────────────────┤
│  PSRAM (4MB) - if available        │
│  ├─ Camera Frame Buffers (x2)      │
│  │  └─ UXGA: 1600x1200 JPEG        │
│  └─ Temporary Storage              │
├────────────────────────────────────┤
│  SRAM (520KB)                      │
│  ├─ Stack & Heap (~200KB free)     │
│  ├─ WiFi Buffers (~50KB)           │
│  ├─ TCP Buffers (~40KB)            │
│  └─ Application Variables          │
└────────────────────────────────────┘
```

## Network Protocol

```
HTTP Request/Response Flow:

Client                          ESP32
  │                              │
  ├─── GET / ──────────────────► │
  │                              ├─ Load web_interface.h
  │ ◄──── HTML Page ─────────────┤
  │                              │
  ├─── GET /stream ─────────────► │
  │                              ├─ Start camera loop
  │ ◄──── MJPEG Frame 1 ─────────┤
  │ ◄──── MJPEG Frame 2 ─────────┤
  │ ◄──── MJPEG Frame 3 ─────────┤
  │          (continuous)         │
  │                              │
  ├─── POST /settings ──────────► │
  │      brightness=1            ├─ Update camera sensor
  │ ◄──── OK ────────────────────┤
  │                              │
```

## Power States

```
┌─────────────┐
│   Startup   │
│  ~400mA     │
└──────┬──────┘
       │
       ▼
┌─────────────┐     WiFi Connected
│   WiFi      │────────────────────►┌─────────────┐
│  Connect    │                     │   Idle      │
│  ~300mA     │◄────────────────────│  ~150mA     │
└─────────────┘     Disconnect      └──────┬──────┘
                                           │
                    Streaming              │
       ┌───────────────────────────────────┘
       │
       ▼
┌─────────────┐
│  Streaming  │
│  ~350mA     │
└──────┬──────┘
       │
       │ Capture
       ▼
┌─────────────┐
│  Capture    │
│  ~400mA     │
└─────────────┘
```
