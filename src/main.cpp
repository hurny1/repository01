#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_camera.h>
#include <TFT_eSPI.h>
#include <SPIFFS.h>
#include "camera_config.h"
#include "web_interface.h"

// WiFi credentials - update these for your network
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web authentication credentials - update these for security
const char* www_username = "admin";
const char* www_password = "esp32cam";

// Web server on port 80
WebServer server(80);

// TFT display instance
TFT_eSPI tft = TFT_eSPI();

// Camera settings
struct CameraSettings {
    int brightness = 0;      // -2 to 2
    int contrast = 0;        // -2 to 2
    int saturation = 0;      // -2 to 2
    int quality = 10;        // 0-63 (lower is better quality)
    bool mirror = false;
    bool vflip = false;
} cameraSettings;

// Display state
bool showingPreview = true;
unsigned long lastPreviewUpdate = 0;
const unsigned long previewInterval = 100; // Update preview every 100ms

// Touch calibration for CYD
#define TOUCH_CS 33
#define TOUCH_IRQ 36

// Function prototypes
void initCamera();
void initWiFi();
void initDisplay();
void initWebServer();
void updateCameraSettings();
void displayPreview();
void handleTouch();
void captureAndSave();
void drawControls();
bool checkAuthentication();

void setup() {
    Serial.begin(115200);
    Serial.println("CYD ESP32 Camera System Starting...");
    
    // Initialize SPIFFS for web files and storage
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed!");
        tft.println("SPIFFS FAILED!");
        while(1) { delay(1000); } // Halt system
    }
    
    // Initialize display
    initDisplay();
    tft.println("Initializing...");
    
    // Initialize camera
    initCamera();
    tft.println("Camera OK");
    
    // Initialize WiFi
    initWiFi();
    tft.println("WiFi Connected");
    tft.print("IP: ");
    tft.println(WiFi.localIP());
    
    // Initialize web server
    initWebServer();
    tft.println("Web Server Started");
    
    delay(2000);
    drawControls();
    
    Serial.println("System ready!");
    Serial.print("Camera Stream URL: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

void loop() {
    server.handleClient();
    
    // Note: Preview and touch features not yet implemented
    // Uncomment when implementations are ready
    // if (showingPreview && (millis() - lastPreviewUpdate > previewInterval)) {
    //     displayPreview();
    //     lastPreviewUpdate = millis();
    // }
    // handleTouch();
    
    delay(1);
}

void initCamera() {
    camera_config_t config = getCameraConfig();
    
    // Initialize camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        tft.println("Camera FAILED!");
        return;
    }
    
    // Get camera sensor
    sensor_t * s = esp_camera_sensor_get();
    if (s == NULL) {
        Serial.println("Failed to get camera sensor");
        return;
    }
    
    // Apply initial settings
    s->set_brightness(s, cameraSettings.brightness);
    s->set_contrast(s, cameraSettings.contrast);
    s->set_saturation(s, cameraSettings.saturation);
    s->set_quality(s, cameraSettings.quality);
    s->set_hmirror(s, cameraSettings.mirror);
    s->set_vflip(s, cameraSettings.vflip);
    
    // Set frame size for streaming
    s->set_framesize(s, FRAMESIZE_QVGA); // 320x240
    
    Serial.println("Camera initialized successfully");
}

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    tft.println("Connecting WiFi...");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWiFi connection failed!");
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("WiFi FAILED!");
        tft.println("Check credentials");
        while(1) { delay(1000); } // Halt system
    }
}

void initDisplay() {
    tft.init();
    tft.setRotation(1); // Landscape mode
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
}

bool checkAuthentication() {
    if (!server.authenticate(www_username, www_password)) {
        server.requestAuthentication();
        return false;
    }
    return true;
}

void initWebServer() {
    // Serve main page (requires authentication)
    server.on("/", HTTP_GET, []() {
        if (!checkAuthentication()) return;
        server.send_P(200, "text/html", getWebInterface());
    });
    
    // Video stream endpoint (requires authentication)
    server.on("/stream", HTTP_GET, []() {
        if (!checkAuthentication()) return;
        
        WiFiClient client = server.client();
        
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
        client.println();
        
        while (client.connected()) {
            camera_fb_t * fb = esp_camera_fb_get();
            if (!fb) {
                Serial.println("Camera capture failed");
                break;
            }
            
            client.print("--frame\r\n");
            client.print("Content-Type: image/jpeg\r\n");
            client.print("Content-Length: ");
            client.print(fb->len);
            client.print("\r\n\r\n");
            client.write(fb->buf, fb->len);
            client.print("\r\n");
            
            esp_camera_fb_return(fb);
            
            if (!client.connected()) break;
        }
    });
    
    // Camera settings endpoint (requires authentication)
    server.on("/settings", HTTP_GET, []() {
        if (!checkAuthentication()) return;
        
        String json = "{";
        json += "\"brightness\":" + String(cameraSettings.brightness) + ",";
        json += "\"contrast\":" + String(cameraSettings.contrast) + ",";
        json += "\"saturation\":" + String(cameraSettings.saturation) + ",";
        json += "\"quality\":" + String(cameraSettings.quality) + ",";
        json += "\"mirror\":" + String(cameraSettings.mirror) + ",";
        json += "\"vflip\":" + String(cameraSettings.vflip);
        json += "}";
        server.send(200, "application/json", json);
    });
    
    // Update settings endpoint (requires authentication)
    server.on("/settings", HTTP_POST, []() {
        if (!checkAuthentication()) return;
        
        if (server.hasArg("brightness")) {
            int val = server.arg("brightness").toInt();
            cameraSettings.brightness = constrain(val, -2, 2);
        }
        if (server.hasArg("contrast")) {
            int val = server.arg("contrast").toInt();
            cameraSettings.contrast = constrain(val, -2, 2);
        }
        if (server.hasArg("saturation")) {
            int val = server.arg("saturation").toInt();
            cameraSettings.saturation = constrain(val, -2, 2);
        }
        if (server.hasArg("quality")) {
            int val = server.arg("quality").toInt();
            cameraSettings.quality = constrain(val, 0, 63);
        }
        if (server.hasArg("mirror")) {
            cameraSettings.mirror = server.arg("mirror") == "true";
        }
        if (server.hasArg("vflip")) {
            cameraSettings.vflip = server.arg("vflip") == "true";
        }
        
        updateCameraSettings();
        server.send(200, "text/plain", "Settings updated");
    });
    
    // Capture and save image (requires authentication)
    server.on("/capture", HTTP_GET, []() {
        if (!checkAuthentication()) return;
        
        captureAndSave();
        server.send(200, "text/plain", "Image captured");
    });
    
    // List saved images (requires authentication)
    server.on("/images", HTTP_GET, []() {
        if (!checkAuthentication()) return;
        
        File root = SPIFFS.open("/");
        String json = "[";
        bool first = true;
        int count = 0;
        const int MAX_FILES = 50; // Limit to prevent memory issues
        
        File file = root.openNextFile();
        while (file && count < MAX_FILES) {
            String filename = String(file.name());
            if (filename.endsWith(".jpg")) {
                if (!first) json += ",";
                json += "\"" + filename + "\"";
                first = false;
                count++;
            }
            file.close(); // Explicitly close file
            file = root.openNextFile();
        }
        json += "]";
        
        server.send(200, "application/json", json);
    });
    
    server.begin();
    Serial.println("Web server started");
    Serial.println("Authentication enabled - use credentials to access");
}

void updateCameraSettings() {
    sensor_t * s = esp_camera_sensor_get();
    if (s == NULL) return;
    
    s->set_brightness(s, cameraSettings.brightness);
    s->set_contrast(s, cameraSettings.contrast);
    s->set_saturation(s, cameraSettings.saturation);
    s->set_quality(s, cameraSettings.quality);
    s->set_hmirror(s, cameraSettings.mirror);
    s->set_vflip(s, cameraSettings.vflip);
    
    Serial.println("Camera settings updated");
}

void displayPreview() {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        return;
    }
    
    // Draw a scaled-down version on the display
    // This is a simplified preview - actual implementation would need
    // JPEG decoding and scaling
    
    esp_camera_fb_return(fb);
}

void handleTouch() {
    // Touch handling for CYD controls
    // This would need proper touch library implementation
    // Placeholder for touch-based control of brightness, contrast, etc.
}

void captureAndSave() {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }
    
    // Check available space (keep at least 100KB free)
    size_t totalBytes = SPIFFS.totalBytes();
    size_t usedBytes = SPIFFS.usedBytes();
    if ((totalBytes - usedBytes) < (fb->len + 100000)) {
        Serial.println("SPIFFS space low, cannot save image");
        esp_camera_fb_return(fb);
        return;
    }
    
    // Generate filename with timestamp
    String filename = "/img_" + String(millis()) + ".jpg";
    
    // Save to SPIFFS
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        esp_camera_fb_return(fb);
        return;
    }
    
    size_t written = file.write(fb->buf, fb->len);
    file.close();
    
    if (written != fb->len) {
        Serial.println("File write incomplete");
        SPIFFS.remove(filename); // Clean up partial file
        esp_camera_fb_return(fb);
        return;
    }
    
    Serial.print("Image saved: ");
    Serial.println(filename);
    
    esp_camera_fb_return(fb);
    
    // Show notification on display (non-blocking)
    tft.fillRect(0, 0, 320, 30, TFT_GREEN);
    tft.setTextColor(TFT_BLACK, TFT_GREEN);
    tft.setCursor(10, 5);
    tft.println("Image Saved!");
    // Note: drawControls() will be called on next interaction
}

void drawControls() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    
    // Title
    tft.setCursor(10, 10);
    tft.setTextSize(2);
    tft.println("CYD Camera");
    
    // IP Address
    tft.setTextSize(1);
    tft.setCursor(10, 40);
    tft.print("IP: ");
    tft.println(WiFi.localIP());
    
    // Camera settings
    tft.setCursor(10, 60);
    tft.print("Brightness: ");
    tft.println(cameraSettings.brightness);
    
    tft.setCursor(10, 80);
    tft.print("Contrast: ");
    tft.println(cameraSettings.contrast);
    
    tft.setCursor(10, 100);
    tft.print("Quality: ");
    tft.println(cameraSettings.quality);
    
    // Instructions
    tft.setCursor(10, 140);
    tft.println("Use web interface");
    tft.setCursor(10, 160);
    tft.println("to control camera");
}
