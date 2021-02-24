/* MIT License
 *
 * Copyright (c) 2020 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  Main entry point
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <WebSocketsServer.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Competition states */
typedef enum
{
    COMPETITION_STATE_UNRELEASED = 0, /**< Competition is not released yet */
    COMPETITION_STATE_RELEASED,       /**< Competition is released, but not started yet. */
    COMPETITION_STATE_STARTED,        /**< Competition is started */
    COMPETITION_STATE_FINISHED        /**< Competition is finished */

} CompetitionState;

/******************************************************************************
 * Prototypes
 *****************************************************************************/

static void webSocketEvent(uint8_t clientId, WStype_t type, uint8_t* payload, size_t length);
static void handleCompetition();
static bool isRobotDetected();

/******************************************************************************
 * Variables
 *****************************************************************************/

/** Serial interface baudrate. */
static const uint32_t   SERIAL_BAUDRATE         = 115200U;

/** Wifi access point SSID */
static const char*      WIFI_AP_SSID            = "RacingLapTimer";

/** Hostname */
static const char*      HOSTNAME                = "laptimer";

/** WiFi manager webserver password, only relevant if the access point mode is running. */
static const char*      WIFI_MGR_WEB_PASSWORD   = "let me in";

/** Webserver port */
static const uint32_t   WEBSERVER_PORT          = 80;

/** Websocket port */
static const uint32_t   WEBSOCKET_PORT          = 81;

/** Digital input pin (arduino pin) for the laser obstacle detection sensor. */
static const uint8_t    SENSOR_DIN_PIN          = 5;

/**
 * After the first detection of the robot with the ext. sensor, this consider
 * the duration in ms after that the sensor will be considered again.
 */
static const uint32_t   SENSOR_BLIND_PERIOD     = 400;

/** Webserver on port for http protocol */
static ESP8266WebServer gWebServer(WEBSERVER_PORT);

/** Websocket server on port for ws protocol */
static WebSocketsServer gWebSocketSrv(WEBSOCKET_PORT);

/** Competition start timestamp in ms */
static uint32_t         gStartTimestamp         = 0;

/** Current competition state */
static CompetitionState gCompetitionState       = COMPETITION_STATE_UNRELEASED;

/******************************************************************************
 * External functions
 *****************************************************************************/

/**
 * Setup the system.
 */
void setup() /* cppcheck-suppress unusedFunction */
{
    WiFiManager wifiMgr; /* No permanent instance is necessary! */
    bool        isError = false;

    /* Setup serial interface */
    Serial.begin(SERIAL_BAUDRATE);

    /* Prepare sensor input pin */
    pinMode(SENSOR_DIN_PIN, INPUT);

    /* Mount filesystem */
    if (false == LittleFS.begin())
    {
        Serial.printf("%lu: Failed to mount filesystem.\n", millis());
        isError = true;
    }
    /* If there are no credentials stored in persistent memory, a access point
     * will be spawned. This call will only return if credentials are stored and
     * a connection was successful established.
     */
    else if (false == wifiMgr.autoConnect(WIFI_AP_SSID, WIFI_MGR_WEB_PASSWORD))
    {
        Serial.printf("%lu: WiFi manager failed to connect.\n", millis());
        isError = true;
    }
    /* Setup mDNS service */
    else if (false == MDNS.begin(HOSTNAME))
    {
        Serial.printf("%lu: Failed to start mDNS service.", millis());
        isError = true;
    }
    else
    {
        /* Setup webserver */
        gWebServer.serveStatic("/", LittleFS, "/web/", "max-age=86400");
        gWebServer.onNotFound(
            []()
            {
                gWebServer.send(404, "text/plain", "File not found.");
            }
        );
        gWebServer.begin();

        /* Setup websocket server */
        gWebSocketSrv.onEvent(webSocketEvent);
        gWebSocketSrv.begin();
    }

    if (true == isError)
    {
        delay(30000);
        ESP.restart();
    }
    else
    {
        Serial.printf("%lu: Ready.\n", millis());
    }

    return;
}

/**
 * Main loop, which is called periodically.
 */
void loop() /* cppcheck-suppress unusedFunction */
{
    MDNS.update();
    gWebServer.handleClient();
    gWebSocketSrv.loop();
    handleCompetition();

    return;
}

/******************************************************************************
 * Local functions
 *****************************************************************************/

/**
 * Handle websocket event.
 *
 * @param[in] clientId  Websocket client id
 * @param[in] type      Event type
 * @param[in] payload   Event payload
 * @param[in] length    Event payload length
 */
static void webSocketEvent(uint8_t clientId, WStype_t type, uint8_t* payload, size_t length)
{
    String cmd;
    size_t index = 0;

    switch(type)
    {
    case WStype_ERROR:
        Serial.printf("%lu: Ws client (%u) error.\n", millis(), clientId);
        break;

    case WStype_DISCONNECTED:
        Serial.printf("%lu: Ws client (%u) disconnected.\n", millis(), clientId);
        break;

    case WStype_CONNECTED:
        Serial.printf("%lu: Ws client (%u) connected.\n", millis(), clientId);
        break;

    case WStype_TEXT:
        for(index = 0; index < length; ++index)
        {
            cmd += reinterpret_cast<char*>(payload)[index];
        }

        Serial.printf("%lu: Ws client (%u): %s\n", millis(), clientId, cmd.c_str());

        if (cmd.equals("RELEASE"))
        {
            if ((COMPETITION_STATE_UNRELEASED == gCompetitionState) ||
                (COMPETITION_STATE_FINISHED == gCompetitionState))
            {
                gCompetitionState = COMPETITION_STATE_RELEASED;
                gWebSocketSrv.sendTXT(clientId, "ACK");
            }
            else
            {
                gWebSocketSrv.sendTXT(clientId, "NACK");
            }
        }
        else
        {
            gWebSocketSrv.sendTXT(clientId, "NACK");
        }
        break;

    case WStype_BIN:
        /* Not supported. */
        break;

    case WStype_FRAGMENT_TEXT_START:
        /* Not supported. */
        break;

    case WStype_FRAGMENT_BIN_START:
        /* Not supported. */
        break;

    case WStype_FRAGMENT:
        /* Not supported. */
        break;

    case WStype_FRAGMENT_FIN:
        /* Not supported. */
        break;

    case WStype_PING:
        /* Not supported. */
        break;

    case WStype_PONG:
        /* Not supported. */
        break;

    default:
        break;
    }
}

/**
 * Handle the competition state machine, depending on the user input from
 * web frontend and sensor input.
 */
static void handleCompetition()
{
    uint32_t duration = 0;

    switch(gCompetitionState)
    {
    case COMPETITION_STATE_UNRELEASED:
        /* Don't care about external sensor.
         * User must release the first competition.
         */
        break;

    case COMPETITION_STATE_RELEASED:
        /* React on external sensor */
        if (true == isRobotDetected())
        {
            gStartTimestamp = millis();
            gWebSocketSrv.broadcastTXT("EVT;STARTED");
            gCompetitionState = COMPETITION_STATE_STARTED;
        }
        break;

    case COMPETITION_STATE_STARTED:
        duration = millis() - gStartTimestamp;

        /* React on external sensor */
        if (SENSOR_BLIND_PERIOD <= duration)
        {
            if (true == isRobotDetected())
            {
                gWebSocketSrv.broadcastTXT(String("EVT;FINISHED;") + duration);
                gCompetitionState = COMPETITION_STATE_FINISHED;
            }
        }
        break;

    case COMPETITION_STATE_FINISHED:
        /* Don't care about external sensor.
         * User must release next competition.
         */
        break;

    default:
        break;
    }
}

/**
 * Is a roboter detected or not?
 *
 * @return If a roboter is detected, it will return true otherwise false.
 */
static bool isRobotDetected()
{
    bool  isDetected  = false;
    int   state       = digitalRead(SENSOR_DIN_PIN);

    if (HIGH == state)
    {
        isDetected = true;
    }

    return isDetected;
}
