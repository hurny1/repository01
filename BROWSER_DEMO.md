# Browser Authentication Demo

## What Users Will See

### Step 1: Accessing the Camera
When users navigate to `http://[ESP32_IP]/`, they see:

```
┌─────────────────────────────────────────────────────────────┐
│  Browser Window                                             │
├─────────────────────────────────────────────────────────────┤
│  ← → ↻  http://192.168.1.100/                   🔒 Not secure│
├─────────────────────────────────────────────────────────────┤
│                                                             │
│                  ┌───────────────────────────┐              │
│                  │  🔒 Sign in               │              │
│                  │                           │              │
│                  │  http://192.168.1.100     │              │
│                  │  is requesting your       │              │
│                  │  username and password.   │              │
│                  │                           │              │
│                  │  The site says:           │              │
│                  │  "Login Required"         │              │
│                  │                           │              │
│                  │  Username:                │              │
│                  │  ┌─────────────────────┐ │              │
│                  │  │ admin              │ │              │
│                  │  └─────────────────────┘ │              │
│                  │                           │              │
│                  │  Password:                │              │
│                  │  ┌─────────────────────┐ │              │
│                  │  │ ••••••••           │ │              │
│                  │  └─────────────────────┘ │              │
│                  │                           │              │
│                  │  [Cancel]    [Sign In]    │              │
│                  │                           │              │
│                  └───────────────────────────┘              │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Step 2: After Successful Login
Once authenticated, users see the camera interface:

```
┌─────────────────────────────────────────────────────────────┐
│  Browser Window                                             │
├─────────────────────────────────────────────────────────────┤
│  ← → ↻  http://192.168.1.100/                   🔒 Not secure│
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ╔═══════════════════════════════════════════════════════╗ │
│  ║         🎥 CYD ESP32 Camera                          ║ │
│  ║    Live Video Stream & Camera Controls              ║ │
│  ╚═══════════════════════════════════════════════════════╝ │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │                                                     │   │
│  │                                                     │   │
│  │           [Live Camera Feed Here]                  │   │
│  │                                                     │   │
│  │                                                     │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │  Camera Settings                                    │   │
│  │                                                     │   │
│  │  Brightness:  ◄───────●────────►  0                │   │
│  │  Contrast:    ◄───────●────────►  0                │   │
│  │  Saturation:  ◄───────●────────►  0                │   │
│  │  Quality:     ◄──●─────────────►  10               │   │
│  │                                                     │   │
│  │  ☐ Mirror    ☐ Vertical Flip                       │   │
│  │                                                     │   │
│  │  [Apply Settings] [Capture Image] [Refresh]        │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Step 3: Invalid Credentials
If wrong username/password is entered:

```
┌─────────────────────────────────────────────────────────────┐
│  Browser Window                                             │
├─────────────────────────────────────────────────────────────┤
│  ← → ↻  http://192.168.1.100/                   🔒 Not secure│
├─────────────────────────────────────────────────────────────┤
│                                                             │
│                  ┌───────────────────────────┐              │
│                  │  ⚠️ Sign in              │              │
│                  │                           │              │
│                  │  http://192.168.1.100     │              │
│                  │  is requesting your       │              │
│                  │  username and password.   │              │
│                  │                           │              │
│                  │  ❌ The username or       │              │
│                  │     password is incorrect │              │
│                  │                           │              │
│                  │  Username:                │              │
│                  │  ┌─────────────────────┐ │              │
│                  │  │ admin              │ │              │
│                  │  └─────────────────────┘ │              │
│                  │                           │              │
│                  │  Password:                │              │
│                  │  ┌─────────────────────┐ │              │
│                  │  │                    │ │              │
│                  │  └─────────────────────┘ │              │
│                  │                           │              │
│                  │  [Cancel]    [Sign In]    │              │
│                  │                           │              │
│                  └───────────────────────────┘              │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## Mobile Browser View

### Portrait Mode (iPhone/Android)

```
┌─────────────────────┐
│ ◄  192.168.1.100   │
├─────────────────────┤
│                     │
│  ┌───────────────┐  │
│  │ 🔒 Sign in    │  │
│  │               │  │
│  │ 192.168.1.100 │  │
│  │ requires      │  │
│  │ username and  │  │
│  │ password      │  │
│  │               │  │
│  │ Username:     │  │
│  │ ┌───────────┐ │  │
│  │ │ admin    │ │  │
│  │ └───────────┘ │  │
│  │               │  │
│  │ Password:     │  │
│  │ ┌───────────┐ │  │
│  │ │ ••••••   │ │  │
│  │ └───────────┘ │  │
│  │               │  │
│  │  Cancel Log In│  │
│  │               │  │
│  └───────────────┘  │
│                     │
│                     │
└─────────────────────┘
```

After login:

```
┌─────────────────────┐
│ ◄  192.168.1.100   │
├─────────────────────┤
│ 🎥 CYD Camera      │
├─────────────────────┤
│                     │
│ ┌─────────────────┐ │
│ │                 │ │
│ │  Live Stream    │ │
│ │                 │ │
│ └─────────────────┘ │
│                     │
│ Camera Settings     │
│ ──────────────────  │
│ Brightness [-2,+2]  │
│ ◄──────●─────────►  │
│                     │
│ Contrast [-2,+2]    │
│ ◄──────●─────────►  │
│                     │
│ Saturation [-2,+2]  │
│ ◄──────●─────────►  │
│                     │
│ Quality [0-63]      │
│ ◄──●─────────────►  │
│                     │
│ ☐ Mirror            │
│ ☐ Vertical Flip     │
│                     │
│ [Apply Settings]    │
│ [Capture Image]     │
│                     │
└─────────────────────┘
```

## Desktop Browser Examples

### Chrome
- Shows "Sign in" dialog with Chrome styling
- Stores credentials in browser memory
- Auto-fills on subsequent requests
- Can clear via Chrome settings

### Firefox  
- Shows "Authentication Required" dialog
- Option to "Remember this password"
- Can manage via Firefox password manager
- Logout by clearing site data

### Safari
- Shows authentication sheet
- Integrates with iCloud Keychain
- Option to save to keychain
- Logout via Safari preferences

### Edge
- Shows Windows Hello compatible dialog
- Can use Windows credentials
- Syncs across devices
- Manage in Edge settings

## Command Line Examples

### curl (Linux/Mac)
```bash
# With -u flag (recommended)
curl -u admin:esp32cam http://192.168.1.100/

# With header
curl -H "Authorization: Basic YWRtaW46ZXNwMzJjYW0=" \
     http://192.168.1.100/

# Without credentials (fails)
curl http://192.168.1.100/
# Output: 401 Unauthorized
```

### wget
```bash
# With credentials
wget --user=admin --password=esp32cam \
     http://192.168.1.100/

# Save stream
wget --user=admin --password=esp32cam \
     http://192.168.1.100/stream -O stream.mjpeg
```

### PowerShell (Windows)
```powershell
# Create credential
$credentials = "admin:esp32cam"
$bytes = [System.Text.Encoding]::UTF8.GetBytes($credentials)
$base64 = [Convert]::ToBase64String($bytes)

# Make request
$headers = @{
    Authorization = "Basic $base64"
}
Invoke-WebRequest -Uri "http://192.168.1.100/" -Headers $headers
```

## API Client Examples

### JavaScript (Fetch API)
```javascript
// In web page
fetch('http://192.168.1.100/settings', {
    method: 'GET',
    headers: {
        'Authorization': 'Basic ' + btoa('admin:esp32cam')
    }
})
.then(response => response.json())
.then(data => console.log(data));
```

### Python (Requests)
```python
import requests
from requests.auth import HTTPBasicAuth

# Get settings
response = requests.get(
    'http://192.168.1.100/settings',
    auth=HTTPBasicAuth('admin', 'esp32cam')
)
print(response.json())
```

### Node.js
```javascript
const axios = require('axios');

axios.get('http://192.168.1.100/settings', {
    auth: {
        username: 'admin',
        password: 'esp32cam'
    }
})
.then(response => console.log(response.data));
```

## Security Indicators

### Browser Security Warnings

**HTTP (Current Implementation):**
```
🔓 Not secure | http://192.168.1.100/
```
- Credentials sent in Base64 (not encrypted)
- Anyone on network can intercept
- Suitable for private networks only

**HTTPS (Future Enhancement):**
```
🔒 Secure | https://192.168.1.100/
```
- Credentials encrypted
- Safe from network sniffing
- Requires SSL certificate

## Error Messages

### 401 Unauthorized
```
HTTP/1.1 401 Unauthorized
WWW-Authenticate: Basic realm="Login Required"
Content-Length: 0
```

### Browser Display
```
┌─────────────────────────────┐
│  401 Unauthorized           │
│                             │
│  This page requires         │
│  authentication             │
│                             │
│  [Retry]                    │
└─────────────────────────────┘
```

## User Experience Flow

```
1. User enters URL
   ↓
2. Browser requests page
   ↓
3. ESP32 checks for credentials
   ↓
4. No credentials → 401 response
   ↓
5. Browser shows login dialog
   ↓
6. User enters username/password
   ↓
7. Browser resends request with credentials
   ↓
8. ESP32 validates credentials
   ↓
9a. Valid → Serve content ✅
9b. Invalid → 401 again ❌
   ↓
10. Browser caches credentials for session
    ↓
11. Subsequent requests auto-authenticated
```

## Tips for Users

### First Time Setup
1. Change default credentials in code
2. Upload to ESP32
3. Note the IP address from serial monitor
4. Open browser to IP address
5. Enter new credentials
6. Bookmark for easy access

### Daily Use
1. Open bookmark
2. Browser auto-authenticates
3. View camera immediately
4. No repeated login needed

### Logout
- Close all browser windows
- Clear browser cache/cookies
- Or use browser's "Forget this site" option

### Troubleshooting
- Can't login? Check credentials in code
- Always prompted? Check browser cache settings
- 401 errors? Verify password is correct
