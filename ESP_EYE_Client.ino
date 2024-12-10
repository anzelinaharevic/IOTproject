#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "my WIFI";
const char* password = "my password";

// Replace with the IP address of the receiver ESP32
const char* serverUrl = "http://192.168.178.66/upload";

// Initialize the camera configuration
void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 34;
    config.pin_d1 = 13;
    config.pin_d2 = 14;
    config.pin_d3 = 35;
    config.pin_d4 = 39;
    config.pin_d5 = 38;
    config.pin_d6 = 37;
    config.pin_d7 = 36;
    config.pin_xclk = 4;
    config.pin_pclk = 25;
    config.pin_vsync = 5;
    config.pin_href = 27;
    config.pin_sscb_sda = 18;
    config.pin_sscb_scl = 23;
    config.pin_reset = -1;
    config.pin_pwdn = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;

    // Init with config
    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera init failed");
        return;
    }
}

// Capture and send an image
void captureAndSendImage() {
    // Capture image
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    // Send the image over HTTP
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "image/jpeg");

        int httpResponseCode = http.POST(fb->buf, fb->len);
        if (httpResponseCode > 0) {
            Serial.printf("Image sent successfully! Response code: %d\n", httpResponseCode);
        } else {
            Serial.printf("Error in sending image: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    } else {
        Serial.println("WiFi not connected");
    }

    // Return frame buffer back to the driver
    esp_camera_fb_return(fb);
}

void setup() {
    Serial.begin(115200);

    // Initialize the camera
    initCamera();

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    // Set up a button on GPIO 0 to trigger capture
    pinMode(0, INPUT_PULLUP);
}

void loop() {
    // Capture and send image on button press
    if (digitalRead(0) == LOW) {
        captureAndSendImage();
        delay(2000); // Debounce delay
    }
}
