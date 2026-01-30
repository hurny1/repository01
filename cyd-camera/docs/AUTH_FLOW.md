# Authentication Flow Diagram

## HTTP Basic Authentication Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                   User Access Flow                              │
└─────────────────────────────────────────────────────────────────┘

    User Browser                ESP32 Web Server
         │                              │
         │  1. GET http://192.168.1.100/ │
         ├─────────────────────────────>│
         │                              │
         │                              │ checkAuthentication()
         │                              │ No credentials found
         │                              │
         │  2. 401 Unauthorized         │
         │     WWW-Authenticate: Basic  │
         │<─────────────────────────────┤
         │                              │
         │                              │
    ┌────┴────┐                         │
    │ Login   │                         │
    │ Prompt  │                         │
    │ Appears │                         │
    └────┬────┘                         │
         │                              │
    User enters:                        │
    Username: admin                     │
    Password: esp32cam                  │
         │                              │
         │  3. GET http://192.168.1.100/ │
         │     Authorization: Basic YWRt...│
         ├─────────────────────────────>│
         │                              │
         │                              │ checkAuthentication()
         │                              │ Decode credentials
         │                              │ Compare with stored
         │                              │ Match! ✓
         │                              │
         │  4. 200 OK                   │
         │     HTML Page                │
         │<─────────────────────────────┤
         │                              │
         │  Camera interface displayed  │
         │                              │
```

## Authentication Check Process

```
┌─────────────────────────────────────────────────────────────────┐
│              checkAuthentication() Function                     │
└─────────────────────────────────────────────────────────────────┘

                    Request Arrives
                          │
                          ▼
              ┌───────────────────────┐
              │ Extract Authorization │
              │ header from request   │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Decode Base64         │
              │ credentials           │
              └───────────┬───────────┘
                          │
                          ▼
              ┌───────────────────────┐
              │ Split into           │
              │ username:password    │
              └───────────┬───────────┘
                          │
                          ▼
         ┌────────────────────────────────────┐
         │ Compare with stored credentials:   │
         │ www_username = "admin"            │
         │ www_password = "esp32cam"         │
         └────────────┬───────────────────────┘
                      │
           ┌──────────┴──────────┐
           │                     │
      Match? ✓              No Match ✗
           │                     │
           ▼                     ▼
    ┌─────────────┐      ┌──────────────┐
    │ Return true │      │ Return 401   │
    │ Continue    │      │ Unauthorized │
    │ request     │      │ with WWW-    │
    │             │      │ Authenticate │
    └─────────────┘      └──────────────┘
```

## Protected Endpoint Example

```
┌─────────────────────────────────────────────────────────────────┐
│              GET /stream Endpoint                               │
└─────────────────────────────────────────────────────────────────┘

server.on("/stream", HTTP_GET, []() {
    │
    ├─► Step 1: Check Authentication
    │   if (!checkAuthentication()) return;
    │        │
    │        ├─ Has credentials? ─► Yes ─► Verify ─► Match ─► Continue
    │        │                                          │
    │        └─ No credentials? ─────────────────────► 401 & Exit
    │                                                   │
    │                                              Wrong password
    │                                                   │
    │                                                   └─► 401 & Exit
    │
    ├─► Step 2: Serve Content (only if authenticated)
    │   WiFiClient client = server.client();
    │   
    │   // Stream MJPEG frames
    │   while (client.connected()) {
    │       camera_fb_t * fb = esp_camera_fb_get();
    │       // Send frame...
    │   }
    │
    └─► Response sent (video stream)
});
```

## Security Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                    Security Layers                              │
└─────────────────────────────────────────────────────────────────┘

    Internet/Network           ESP32 Device
         │                          │
         │  1. Network Access       │
         ├─────────────────────────>│
         │                          │
         │  ┌─────────────────┐    │
         │  │ WiFi Password   │    │
         │  │ Protection      │    │
         │  └────────┬────────┘    │
         │           │              │
         │  2. Web Request          │
         ├─────────────────────────>│
         │                          │
         │           │              │
         │           ▼              │
         │  ┌─────────────────┐    │
         │  │ HTTP Basic      │    │
         │  │ Authentication  │    │ ← NEW!
         │  │ (username/pass) │    │
         │  └────────┬────────┘    │
         │           │              │
         │           ▼              │
         │  ┌─────────────────┐    │
         │  │ Credential      │    │
         │  │ Verification    │    │
         │  └────────┬────────┘    │
         │           │              │
         │           ▼              │
         │        ✓ Authorized      │
         │                          │
         │  3. Serve Content        │
         │<─────────────────────────┤
         │                          │
```

## Authentication State Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    User States                                  │
└─────────────────────────────────────────────────────────────────┘

        ┌──────────────┐
        │ Unauthenticated│
        │    State      │
        └───────┬────────┘
                │
                │ Access endpoint
                ▼
        ┌──────────────┐
        │ Login Prompt │
        │   Displayed  │
        └───────┬────────┘
                │
        ┌───────┴────────┐
        │                │
    Cancel          Enter Credentials
        │                │
        ▼                ▼
    ┌────────┐    ┌─────────────┐
    │ Denied │    │ Credentials │
    │        │    │ Checked     │
    └────────┘    └──────┬──────┘
                         │
                ┌────────┴────────┐
                │                 │
            Invalid           Valid
                │                 │
                ▼                 ▼
        ┌──────────────┐  ┌──────────────┐
        │ 401 Error    │  │ Authenticated│
        │ Try Again    │  │    State     │
        └──────────────┘  └──────┬───────┘
                                 │
                                 │ Session active
                                 ▼
                         ┌──────────────┐
                         │ Access       │
                         │ Granted      │
                         │              │
                         │ - View stream│
                         │ - Controls   │
                         │ - Settings   │
                         └──────────────┘
```

## Browser Credential Caching

```
┌─────────────────────────────────────────────────────────────────┐
│              Session Management                                 │
└─────────────────────────────────────────────────────────────────┘

First Access:
    Browser: No credentials stored
    Server:  401 → Login prompt appears
    User:    Enters credentials
    Browser: Stores in memory for session
    Server:  Validates & grants access

Subsequent Requests (same session):
    Browser: Automatically sends stored credentials
    Server:  Validates each time
    User:    No prompt (credentials cached)

Session End:
    - Close all browser windows/tabs
    - Clear browser cache
    - Browser forgets credentials
    - Next access requires re-login
```

## API Request with Authentication

```
┌─────────────────────────────────────────────────────────────────┐
│          Programmatic API Access                                │
└─────────────────────────────────────────────────────────────────┘

Using curl:

  # Method 1: -u flag
  curl -u admin:esp32cam http://192.168.1.100/settings

  # Method 2: Authorization header
  curl -H "Authorization: Basic YWRtaW46ZXNwMzJjYW0=" \
       http://192.168.1.100/settings

Using JavaScript:

  fetch('http://192.168.1.100/settings', {
    headers: {
      'Authorization': 'Basic ' + btoa('admin:esp32cam')
    }
  })

Using Python:

  import requests
  from requests.auth import HTTPBasicAuth
  
  response = requests.get(
      'http://192.168.1.100/settings',
      auth=HTTPBasicAuth('admin', 'esp32cam')
  )
```

## Failed Authentication Flow

```
┌─────────────────────────────────────────────────────────────────┐
│               Unauthorized Access Attempt                       │
└─────────────────────────────────────────────────────────────────┘

    User                          Server
     │                              │
     │  GET / (no credentials)     │
     ├────────────────────────────>│
     │                              │
     │                              │ checkAuthentication()
     │                              │ No header found
     │                              │
     │  401 Unauthorized            │
     │  WWW-Authenticate: Basic     │
     │  realm="ESP32 Camera"        │
     │<─────────────────────────────┤
     │                              │
     │  Connection closed           │
     │                              │
     
    OR with wrong credentials:
    
     │  GET / (wrong password)     │
     ├────────────────────────────>│
     │                              │
     │                              │ checkAuthentication()
     │                              │ Decode & compare
     │                              │ Mismatch!
     │                              │
     │  401 Unauthorized            │
     │  WWW-Authenticate: Basic     │
     │<─────────────────────────────┤
     │                              │
     │  Try again prompt            │
```

## Base64 Encoding Example

```
Credentials: admin:esp32cam

Step 1: Concatenate with colon
    "admin:esp32cam"

Step 2: Encode to Base64
    "YWRtaW46ZXNwMzJjYW0="

Step 3: Add to Authorization header
    Authorization: Basic YWRtaW46ZXNwMzJjYW0=

Note: Base64 is encoding, NOT encryption!
Anyone can decode it, so use HTTPS in production.
```
