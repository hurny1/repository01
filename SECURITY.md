# Security Guide

## Web Authentication

As of version 1.1.0, the CYD ESP32 Camera System includes HTTP Basic Authentication to protect access to the camera stream and controls.

### Default Credentials

⚠️ **WARNING:** These default credentials MUST be changed before deployment!

- **Username:** `admin`
- **Password:** `esp32cam`

### Changing Credentials

Edit `src/main.cpp` and modify these lines:

```cpp
// Web authentication credentials - update these for security
const char* www_username = "admin";        // Change to your username
const char* www_password = "esp32cam";     // Change to a strong password
```

### Password Best Practices

1. **Length:** Use at least 12 characters
2. **Complexity:** Mix uppercase, lowercase, numbers, and symbols
3. **Uniqueness:** Don't reuse passwords from other services
4. **Randomness:** Avoid dictionary words and common patterns

Example strong password: `Cy9d#Cam2026!Secure`

### How Authentication Works

The system uses **HTTP Basic Authentication**:

1. When you access any protected endpoint (all of them), your browser prompts for credentials
2. Credentials are sent with each request (Base64 encoded)
3. Server validates credentials before serving content
4. Invalid credentials receive a 401 Unauthorized response

### Protected Endpoints

All endpoints require authentication:

- `GET /` - Main web interface
- `GET /stream` - Video stream
- `GET /settings` - Get camera settings
- `POST /settings` - Update camera settings  
- `GET /capture` - Capture image
- `GET /images` - List saved images

### Security Limitations

⚠️ **Important Security Notes:**

1. **HTTP Only:** Traffic is NOT encrypted
   - Credentials sent in Base64 (easily decoded)
   - Anyone monitoring network can see credentials
   - Video stream is visible to network sniffers

2. **No HTTPS:** SSL/TLS not implemented
   - Would require certificate management
   - ESP32 has limited resources for SSL

3. **Stored in Code:** Credentials hardcoded in source
   - Consider using SPIFFS or external storage
   - Be careful not to commit credentials to git

### Deployment Recommendations

#### For Home Networks (Trusted)
- ✅ Change default credentials
- ✅ Use strong password
- ✅ Keep on private WiFi network
- ⚠️ Don't expose to internet directly

#### For Public Access (Higher Risk)
- ✅ All of the above
- ✅ Use VPN for remote access
- ✅ Consider implementing HTTPS
- ✅ Use firewall rules
- ✅ Monitor access logs
- ⚠️ Never use on untrusted networks

### Future Security Enhancements

Planned improvements:

- [ ] HTTPS/TLS support
- [ ] User management (multiple users)
- [ ] Session tokens instead of Basic Auth
- [ ] Rate limiting for brute force protection
- [ ] Access logging
- [ ] Two-factor authentication
- [ ] Certificate-based authentication

### Storing Credentials Securely

Instead of hardcoding in `main.cpp`, consider:

**Option 1: Separate Header File (Recommended)**

1. Create `include/secrets.h`:
```cpp
#ifndef SECRETS_H
#define SECRETS_H

const char* ssid = "YourWiFi";
const char* password = "YourWiFiPass";
const char* www_username = "yourusername";
const char* www_password = "your$trong!Password123";

#endif
```

2. Add to `.gitignore`:
```
include/secrets.h
```

3. Include in `main.cpp`:
```cpp
#include "secrets.h"
```

**Option 2: SPIFFS Storage**

Store credentials in SPIFFS filesystem (requires code changes):
- Credentials loaded at boot
- Can be updated without recompiling
- Still accessible if someone has device access

**Option 3: Environment Variables (PlatformIO)**

Use build flags in `platformio.ini`:
```ini
build_flags = 
    -DWWW_USERNAME=\"myuser\"
    -DWWW_PASSWORD=\"mypassword\"
```

### Testing Authentication

**From Command Line:**

```bash
# Test with curl (should succeed)
curl -u admin:esp32cam http://192.168.1.100/

# Test without credentials (should fail with 401)
curl http://192.168.1.100/

# Test wrong credentials (should fail with 401)
curl -u admin:wrongpass http://192.168.1.100/
```

**From Browser:**
1. Navigate to `http://[ESP32_IP]/`
2. Browser shows authentication dialog
3. Enter username and password
4. Browser caches credentials for session
5. Log out by closing all browser windows

### Troubleshooting

**Browser keeps asking for password:**
- Check username/password are correct
- Check ESP32 serial output for errors
- Clear browser cache/cookies
- Try different browser

**Can't remember password:**
- Re-upload firmware with new credentials
- Monitor serial output to verify new password

**403 Forbidden:**
- Authentication succeeded but access denied
- Check if endpoint exists
- Check ESP32 logs

**401 Unauthorized:**
- Wrong username or password
- Check credentials in code match what you're entering
- Check for typos in password

### Security Audit Checklist

Before deployment:

- [ ] Changed default username
- [ ] Changed default password to strong password
- [ ] Removed credentials from git repository
- [ ] Using separate secrets file
- [ ] Added secrets file to .gitignore
- [ ] Tested authentication works
- [ ] Verified unauthorized access blocked
- [ ] Documented credentials securely
- [ ] Configured network security (firewall, etc.)
- [ ] Decided on access policy (local only vs VPN)

## Reporting Security Issues

If you discover a security vulnerability, please:

1. **Do NOT** create a public GitHub issue
2. Email the project maintainer privately
3. Include details about the vulnerability
4. Allow time for a fix before public disclosure

Thank you for helping keep this project secure!
