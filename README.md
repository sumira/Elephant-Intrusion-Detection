
# ESP32-CAM Google Vision API Integration

This project demonstrates how to use the ESP32-CAM module to capture images, encode them in Base64 format, and send them to the Google Vision API for image analysis. The system connects to Wi-Fi to communicate with the API and retrieves image analysis results in real-time.

## Features

- Captures images using the ESP32-CAM module.
- Encodes images to Base64 format for API compatibility.
- Sends image data to the Google Vision API for label detection.
- Displays detected labels and their confidence scores via the serial monitor.

## Prerequisites

### Hardware
- ESP32-CAM module (AI-Thinker model).
- FTDI programmer or USB-to-serial adapter for programming.
- Stable power supply for the ESP32-CAM module.

### Software
- Arduino IDE with ESP32 board support installed.
- Required libraries:
  - `esp_camera`
  - `WiFi`
  - `HTTPClient`
  - `ArduinoJson`
  - `base64`

## Setup Instructions

### 1. Install Required Libraries
Ensure the following libraries are installed in the Arduino IDE:
- `esp_camera`
- `WiFi`
- `HTTPClient`
- `ArduinoJson`
- `base64`

### 2. Configure the Code
Update the `config.h` file and the main script:
- Add your Google Vision API key in `config.h`:
  ```cpp
  #define API_KEY "YOUR_GOOGLE_CLOUD_API_KEY"
  ```
- Replace the Wi-Fi credentials in the main script:
  ```cpp
  const char* ssid = "YOUR_WIFI_SSID";
  const char* password = "YOUR_WIFI_PASSWORD";
  ```

### 3. Upload the Code
1. Connect the ESP32-CAM module to your computer using an FTDI programmer or USB-to-serial adapter.
2. Select the appropriate board and port in the Arduino IDE (`AI Thinker ESP32-CAM`).
3. Upload the code to the ESP32-CAM.

### 4. Run the Application
- Open the serial monitor in the Arduino IDE at 115200 baud rate.
- The ESP32-CAM will connect to the specified Wi-Fi network and display its IP address.
- The system will analyze captured images and print detection results in the serial monitor.

## Google Vision API Integration

The project uses the [Google Vision API](https://cloud.google.com/vision) for image analysis. The API processes captured images and returns detected labels along with confidence scores.

### Sample Output
Example response from the API:
```
Label: Elephant, Confidence: 92.34%
Label: Animal, Confidence: 88.56%
Label: Wildlife, Confidence: 85.12%
```

## Troubleshooting

- **Wi-Fi Connection Issues**:
  - Ensure the Wi-Fi credentials are correct.
  - Verify the network signal is strong.

- **Camera Initialization Errors**:
  - Double-check the wiring.
  - Ensure the correct camera pins are defined in `camera_pins.h`.

- **API Key Errors**:
  - Verify that the API key is valid and has permissions for the Vision API.

## Future Improvements

- Add support for other Google Vision API features (e.g., face detection, text recognition).
- Implement advanced image preprocessing for improved detection accuracy.
- Develop a web-based interface to display analysis results.

## License

This project is open-source and licensed under the MIT License.

## Acknowledgments

- [Google Vision API](https://cloud.google.com/vision)
- [ESP32-CAM Documentation](https://randomnerdtutorials.com/esp32-cam-projects-esp32-camera/)
