#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <base64.h>  

// Replace with your network credentials
const char* ssid = "my WIFI";
const char* password = "my Password";


WebServer server(80);

// Initialize the SPIFFS file system
void initSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
    } else {
        Serial.println("SPIFFS mounted successfully");
    }
}

// Handle the upload endpoint
void handleUpload() {
    if (server.hasArg("plain") == false) {
        server.send(500, "text/plain", "No data received");
        return;
    }

    String filename = "/image.jpg";  // Overwrite the image each time
    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
        server.send(500, "text/plain", "Failed to open file for writing");
        return;
    }

    file.write((uint8_t*)server.arg("plain").c_str(), server.arg("plain").length());
    file.close();

    // Print the image as base64 to the Serial Monitor
    Serial.println("Image received and saved. Here is the base64 encoding:");
    Serial.println(base64::encode((uint8_t*)server.arg("plain").c_str(), server.arg("plain").length()));

    server.send(200, "text/plain", "Image received");
}

// Serve the captured image at /image.jpg endpoint
void handleImageAccess() {
    File file = SPIFFS.open("/image.jpg", "r");
    if (!file) {
        server.send(404, "text/plain", "Image not found");
        return;
    }
    server.streamFile(file, "image/jpeg");
    file.close();
}

void setup() {
    Serial.begin(115200);

    // Initialize SPIFFS
    initSPIFFS();

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());

    // Set up server endpoints
    server.on("/upload", HTTP_POST, handleUpload);       // Image upload endpoint
    server.on("/image.jpg", HTTP_GET, handleImageAccess); // Image access endpoint

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
