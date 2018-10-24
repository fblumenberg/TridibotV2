#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "FS.h"
#include <ArduinoOTA.h>
#include <WebSocketsServer.h>
//#include "SPIFFS.h"

#include "Config.h"
#include "WebSocket.h"
#include "Ota.h"
#include "Movement.h"

static void connectToWiFi();

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.setDebugOutput(true);
    Serial.println("8 channel Servo test!");

    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }

    if (!loadConfig())
    {
        Serial.println("Failed to load config");
    }
    else
    {
        Serial.println("Config loaded");
    }

    connectToWiFi();
    setupOTA();
    setupWebSocket();
    setupMovement();
    delay(10);
}

void loop()
{
    loopOTA();
    loopWebSocket();

    char variable = 'X';
    if (Serial.available())
    {
        variable = Serial.read();
    }

    handleCommand(variable);

    delay(100);
}

void connectToWiFi()
{
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(config.wifiSSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(config.wifiSSID.c_str(), config.wifiPassword.c_str());
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
