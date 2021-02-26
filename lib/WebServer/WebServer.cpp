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
/**************************************************************************************************
*   @file WebServer.cpp
*   Implementation of WebServer.h.
*   @author Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "WebServer.h"

/* CONSTANTS *************************************************************************************/

/** Webserver port. */
static const uint32_t WEBSERVER_PORT = 80;

/** Websocket port. */
static const uint32_t WEBSOCKET_PORT = 81;

/** Webserver on port for http protocol. */
static ESP8266WebServer gWebServer(WEBSERVER_PORT);

/** Websocket server on port for ws protocol. */
static WebSocketsServer gWebSocketSrv(WEBSOCKET_PORT);

/** Hostname. */
static const char *HOSTNAME = "laptimer";

/**
 *  Handle websocket event.
 *
 *  @param[in] clientId  Websocket client id.
 *  @param[in] type      Event type.
 *  @param[in] payload   Event payload.
 *  @param[in] length    Event payload length.
 */
static void webSocketEvent(uint8_t clientId, WStype_t type, uint8_t *payload, size_t length);

/** Competition Handler Instance. */
static Competition *m_laptrigger;

/** WLAN Handler Instance. */
static WIFI *m_wireless;

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/** Handler for POST Request for the storage of the STA Credentials. */
static void handleCredentials();

/* VARIABLES *************************************************************************************/

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
 *  Default Constructor.
 * 
 *  @param wireless Instance of WiFi Handler.
 *  @param goalLine Instance of Competition Handler.
 */
LapTriggerWebServer::LapTriggerWebServer(WIFI &wireless, Competition &goalLine)
{
    m_wireless = &wireless;
    m_laptrigger = &goalLine;
}

/**************************************************************************************************/

/**
 *  Default Destructor.
 */
LapTriggerWebServer::~LapTriggerWebServer()
{
}

/**************************************************************************************************/

/**
 *  Initialization of Module.
 * 
 *  @return success.
 */
bool LapTriggerWebServer::begin()
{
    bool success = true;

    /** Mount filesystem. */
    if (false == LittleFS.begin())
    {
        Serial.printf("%lu: Failed to mount filesystem.\n", millis());
        success = false;
    }
    /** Setup mDNS service. */
    else if (false == MDNS.begin(HOSTNAME))
    {
        Serial.printf("%lu: Failed to start mDNS service.", millis());
        success = false;
    }
    else
    {
        /** Setup webserver. */
        gWebServer.serveStatic("/", LittleFS, "/web/", "max-age=86400");
        gWebServer.on("/config.html", HTTP_POST, handleCredentials);
        gWebServer.onNotFound(
            []() {
                gWebServer.send(404, "text/plain", "File not found.");
            });
        gWebServer.begin();

        /** Setup websocket server. */
        gWebSocketSrv.onEvent(webSocketEvent);
        gWebSocketSrv.begin();
    }

    return success;
}

/**************************************************************************************************/

/**
 *  Executes Loop Cycle.
 * 
 *  @return success.
 */
bool LapTriggerWebServer::cycle()
{
    bool success = true;

    String outputMessage = "";
    if (m_laptrigger->handleCompetition(outputMessage))
    {
        gWebSocketSrv.broadcastTXT(outputMessage);
    }

    MDNS.update();
    gWebServer.handleClient();
    gWebSocketSrv.loop();

    return success;
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/

/**
 *  Handle websocket event.
 *
 *  @param[in] clientId  Websocket client id.
 *  @param[in] type      Event type.
 *  @param[in] payload   Event payload.
 *  @param[in] length    Event payload length.
 */
static void webSocketEvent(uint8_t clientId, WStype_t type, uint8_t *payload, size_t length)
{
    String cmd;
    size_t index = 0;

    switch (type)
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

        for (index = 0; index < length; ++index)
        {
            cmd += reinterpret_cast<char *>(payload)[index];
        }

        Serial.printf("%lu: Ws client (%u): %s\n", millis(), clientId, cmd.c_str());
        if (cmd.equals("RELEASE"))
        {
            if (m_laptrigger->setReleasedState())
            {
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

/**************************************************************************************************/

/**
 *  Handler for POST Request for the storage of the STA Credentials.
 */
static void handleCredentials()
{
    if (gWebServer.method() != HTTP_POST)
    {
        gWebServer.send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        String ssidInput = "";
        String passwordInput = "";

        for (uint8_t i = 0; i < gWebServer.args(); i++)
        {
            if (gWebServer.argName(i) == "STA_SSID")
            {
                ssidInput = gWebServer.arg(i);
            }
            else if (gWebServer.argName(i) == "STA_PASSWORD")
            {
                passwordInput = gWebServer.arg(i);
            }
        }

        if (ssidInput.isEmpty())
        {
            String message = "POST form was:\n";
            for (uint8_t i = 0; i < gWebServer.args(); i++)
            {
                message += " " + gWebServer.argName(i) + ": " + gWebServer.arg(i) + "\n";
            }
            gWebServer.send(200, "text/plain", message);
            Serial.println(message);
        }
        else
        {
            if (m_wireless->saveCredentials(ssidInput, passwordInput))
            {
                gWebServer.send(200, "text/plain", "Credentials Accepted.\nRestarting...");
                delay(3000);
                ESP.restart();
            }
        }
    }
}