#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "base64.h"
#include "config.h"

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// WiFi credentials
const char* ssid = "Dialog 4G 564";
const char* password = "5a2Cc5a5";


// Google Cloud Vision API
const char* GOOGLE_CLOUD_API_KEY = API_KEY;
const char* VISION_API_ENDPOINT = "https://vision.googleapis.com/v1/images:annotate?key=";

void startCameraServer();
void setupLedFlash(int pin);

String captureAndEncodeImage() {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return "";
    }
    
    String encoded = base64::encode(fb->buf, fb->len);
    esp_camera_fb_return(fb);
    return encoded;
}

void analyzeImage() {
    String imageBase64 = captureAndEncodeImage();
    if (imageBase64.length() == 0) return;

    HTTPClient http;
    String endpoint = String(VISION_API_ENDPOINT) + GOOGLE_CLOUD_API_KEY;
    http.begin(endpoint);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<16384> doc;
    JsonObject requests = doc.createNestedArray("requests").createNestedObject();
    JsonObject image = requests.createNestedObject("image");
    image["content"] = imageBase64;
    JsonArray features = requests.createNestedArray("features");
    JsonObject feature = features.createNestedObject();
    feature["maxResults"] = 10;
    feature["type"] = "LABEL_DETECTION";

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(response);
        
        // Parse response
        StaticJsonDocument<16384> respDoc;
        DeserializationError error = deserializeJson(respDoc, response);
        
        if (!error) {
            JsonArray responses = respDoc["responses"];
            for (JsonObject resp : responses) {
                JsonArray labels = resp["labelAnnotations"];
                for (JsonObject label : labels) {
                    const char* description = label["description"];
                    float score = label["score"];
                    Serial.printf("Label: %s, Confidence: %.2f%%\n", description, score * 100);
                }
            }
        }
    }

    http.end();
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    if (config.pixel_format == PIXFORMAT_JPEG) {
        if (psramFound()) {
            config.jpeg_quality = 10;
            config.fb_count = 2;
            config.grab_mode = CAMERA_GRAB_LATEST;
        } else {
            config.frame_size = FRAMESIZE_SVGA;
            config.fb_location = CAMERA_FB_IN_DRAM;
        }
    }

    // Initialize camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t* s = esp_camera_sensor_get();
    if (s->id.PID == OV3660_PID) {
        s->set_vflip(s, 1);
        s->set_brightness(s, 1);
        s->set_saturation(s, -2);
    }
    if (config.pixel_format == PIXFORMAT_JPEG) {
        s->set_framesize(s, FRAMESIZE_QVGA);
    }

#if defined(LED_GPIO_NUM)
    setupLedFlash(LED_GPIO_NUM);
#endif

    // Connect to WiFi
    WiFi.begin(ssid, password);
    WiFi.setSleep(false);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    startCameraServer();
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        analyzeImage();
    }
    delay(3000);
}