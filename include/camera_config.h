#ifndef CAMERA_CONFIG_H
#define CAMERA_CONFIG_H

#include <esp_camera.h>

// Camera Module Pin Configuration for CYD (Single ESP32)
// These pins are chosen to avoid conflicts with CYD display pins:
// Display uses: GPIO13 (MOSI), GPIO14 (SCLK), GPIO15 (CS), GPIO2 (DC), GPIO21 (BL)
// Touch uses: GPIO33 (CS), GPIO36 (IRQ)
camera_config_t getCameraConfig() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    
    // Data pins (D0-D7) - using available GPIOs
    config.pin_d0 = 5;
    config.pin_d1 = 18;
    config.pin_d2 = 19;
    config.pin_d3 = 4;    // Changed from 21 to avoid display backlight conflict
    config.pin_d4 = 35;   // Input only
    config.pin_d5 = 34;   // Input only
    config.pin_d6 = 39;   // Input only
    config.pin_d7 = 32;
    
    // Clock and sync pins
    config.pin_xclk = 0;
    config.pin_pclk = 22;
    config.pin_vsync = 25;
    config.pin_href = 23;
    
    // I2C pins for camera control (SCCB)
    config.pin_sccb_sda = 26;
    config.pin_sccb_scl = 27;
    
    // Power and reset
    config.pin_pwdn = -1;     // Not used, camera always powered
    config.pin_reset = -1;    // Not used
    
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    
    // Frame size and quality settings
    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA; // 1600x1200
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA; // 800x600
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }
    
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.grab_mode = CAMERA_GRAB_LATEST;
    
    return config;
}

#endif // CAMERA_CONFIG_H
