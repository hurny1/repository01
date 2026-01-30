#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

const char* getWebInterface() {
    return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CYD ESP32 Camera</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: white;
            border-radius: 15px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            overflow: hidden;
        }
        
        header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        
        h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.2);
        }
        
        .subtitle {
            font-size: 1.1em;
            opacity: 0.9;
        }
        
        .content {
            padding: 30px;
        }
        
        .video-container {
            position: relative;
            width: 100%;
            max-width: 800px;
            margin: 0 auto 30px;
            background: #000;
            border-radius: 10px;
            overflow: hidden;
            box-shadow: 0 10px 30px rgba(0,0,0,0.3);
        }
        
        #stream {
            width: 100%;
            height: auto;
            display: block;
        }
        
        .controls {
            background: #f8f9fa;
            padding: 25px;
            border-radius: 10px;
            margin-bottom: 20px;
        }
        
        .control-group {
            margin-bottom: 25px;
        }
        
        .control-group:last-child {
            margin-bottom: 0;
        }
        
        label {
            display: block;
            font-weight: 600;
            margin-bottom: 10px;
            color: #495057;
            font-size: 1.1em;
        }
        
        input[type="range"] {
            width: 100%;
            height: 8px;
            border-radius: 5px;
            background: #ddd;
            outline: none;
            -webkit-appearance: none;
        }
        
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            box-shadow: 0 2px 5px rgba(0,0,0,0.2);
        }
        
        input[type="range"]::-moz-range-thumb {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            box-shadow: 0 2px 5px rgba(0,0,0,0.2);
        }
        
        .value-display {
            display: inline-block;
            min-width: 40px;
            padding: 5px 10px;
            background: #667eea;
            color: white;
            border-radius: 5px;
            text-align: center;
            font-weight: bold;
            margin-left: 10px;
        }
        
        .button-group {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 15px;
            margin-top: 20px;
        }
        
        button {
            padding: 15px 25px;
            font-size: 1.1em;
            font-weight: 600;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 10px rgba(0,0,0,0.1);
        }
        
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 15px rgba(0,0,0,0.2);
        }
        
        button:active {
            transform: translateY(0);
        }
        
        .btn-primary {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        
        .btn-success {
            background: linear-gradient(135deg, #56ab2f 0%, #a8e063 100%);
            color: white;
        }
        
        .btn-info {
            background: linear-gradient(135deg, #2193b0 0%, #6dd5ed 100%);
            color: white;
        }
        
        .checkbox-group {
            display: flex;
            gap: 20px;
            flex-wrap: wrap;
        }
        
        .checkbox-item {
            display: flex;
            align-items: center;
            gap: 10px;
        }
        
        input[type="checkbox"] {
            width: 20px;
            height: 20px;
            cursor: pointer;
        }
        
        .status {
            padding: 15px;
            border-radius: 8px;
            margin-top: 20px;
            font-weight: 500;
            display: none;
        }
        
        .status.success {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
            display: block;
        }
        
        .status.error {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
            display: block;
        }
        
        @media (max-width: 768px) {
            h1 {
                font-size: 1.8em;
            }
            
            .content {
                padding: 20px;
            }
            
            .controls {
                padding: 15px;
            }
            
            .button-group {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>🎥 CYD ESP32 Camera</h1>
            <p class="subtitle">Live Video Stream & Camera Controls</p>
        </header>
        
        <div class="content">
            <div class="video-container">
                <img id="stream" src="/stream" alt="Camera Stream">
            </div>
            
            <div class="controls">
                <h2 style="margin-bottom: 20px; color: #667eea;">Camera Settings</h2>
                
                <div class="control-group">
                    <label>
                        Brightness 
                        <span class="value-display" id="brightness-value">0</span>
                    </label>
                    <input type="range" id="brightness" min="-2" max="2" value="0" step="1">
                </div>
                
                <div class="control-group">
                    <label>
                        Contrast 
                        <span class="value-display" id="contrast-value">0</span>
                    </label>
                    <input type="range" id="contrast" min="-2" max="2" value="0" step="1">
                </div>
                
                <div class="control-group">
                    <label>
                        Saturation 
                        <span class="value-display" id="saturation-value">0</span>
                    </label>
                    <input type="range" id="saturation" min="-2" max="2" value="0" step="1">
                </div>
                
                <div class="control-group">
                    <label>
                        Quality (0=best, 63=worst)
                        <span class="value-display" id="quality-value">10</span>
                    </label>
                    <input type="range" id="quality" min="0" max="63" value="10" step="1">
                </div>
                
                <div class="control-group">
                    <label>Image Options</label>
                    <div class="checkbox-group">
                        <div class="checkbox-item">
                            <input type="checkbox" id="mirror">
                            <label for="mirror" style="margin: 0;">Mirror</label>
                        </div>
                        <div class="checkbox-item">
                            <input type="checkbox" id="vflip">
                            <label for="vflip" style="margin: 0;">Vertical Flip</label>
                        </div>
                    </div>
                </div>
                
                <div class="button-group">
                    <button class="btn-primary" onclick="applySettings()">Apply Settings</button>
                    <button class="btn-success" onclick="captureImage()">Capture Image</button>
                    <button class="btn-info" onclick="loadSettings()">Refresh Settings</button>
                </div>
                
                <div id="status" class="status"></div>
            </div>
        </div>
    </div>
    
    <script>
        // Update value displays
        document.getElementById('brightness').addEventListener('input', function() {
            document.getElementById('brightness-value').textContent = this.value;
        });
        
        document.getElementById('contrast').addEventListener('input', function() {
            document.getElementById('contrast-value').textContent = this.value;
        });
        
        document.getElementById('saturation').addEventListener('input', function() {
            document.getElementById('saturation-value').textContent = this.value;
        });
        
        document.getElementById('quality').addEventListener('input', function() {
            document.getElementById('quality-value').textContent = this.value;
        });
        
        function showStatus(message, isError = false) {
            const status = document.getElementById('status');
            status.textContent = message;
            status.className = 'status ' + (isError ? 'error' : 'success');
            
            setTimeout(() => {
                status.style.display = 'none';
            }, 3000);
        }
        
        function applySettings() {
            const formData = new URLSearchParams();
            formData.append('brightness', document.getElementById('brightness').value);
            formData.append('contrast', document.getElementById('contrast').value);
            formData.append('saturation', document.getElementById('saturation').value);
            formData.append('quality', document.getElementById('quality').value);
            formData.append('mirror', document.getElementById('mirror').checked);
            formData.append('vflip', document.getElementById('vflip').checked);
            
            fetch('/settings', {
                method: 'POST',
                body: formData
            })
            .then(response => response.text())
            .then(data => {
                showStatus('Settings applied successfully!');
            })
            .catch(error => {
                showStatus('Error applying settings: ' + error, true);
            });
        }
        
        function loadSettings() {
            fetch('/settings')
            .then(response => response.json())
            .then(data => {
                document.getElementById('brightness').value = data.brightness;
                document.getElementById('brightness-value').textContent = data.brightness;
                document.getElementById('contrast').value = data.contrast;
                document.getElementById('contrast-value').textContent = data.contrast;
                document.getElementById('saturation').value = data.saturation;
                document.getElementById('saturation-value').textContent = data.saturation;
                document.getElementById('quality').value = data.quality;
                document.getElementById('quality-value').textContent = data.quality;
                document.getElementById('mirror').checked = data.mirror;
                document.getElementById('vflip').checked = data.vflip;
                showStatus('Settings loaded successfully!');
            })
            .catch(error => {
                showStatus('Error loading settings: ' + error, true);
            });
        }
        
        function captureImage() {
            fetch('/capture')
            .then(response => response.text())
            .then(data => {
                showStatus('Image captured and saved!');
            })
            .catch(error => {
                showStatus('Error capturing image: ' + error, true);
            });
        }
        
        // Load initial settings on page load
        window.addEventListener('load', loadSettings);
    </script>
</body>
</html>
)rawliteral";
}

#endif // WEB_INTERFACE_H
