/* MIT License
 *
 * Copyright (c) 2020-2021 Andreas Merkle <web@blue-andi.de>
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
 * @brief  Implementation of WebServer.h.
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "LapTriggerWebServer.h"
#include "FlashMem.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

LapTriggerWebServer::LapTriggerWebServer(Competition &goalLine) : m_laptrigger(&goalLine)
{
    m_WebServer = new ESP8266WebServer(WEBSERVER_PORT);
    m_WebSocketSrv = new WebSocketsServer(WEBSOCKET_PORT);
}

LapTriggerWebServer::~LapTriggerWebServer()
{
    /* Unmount Filesystem */
    LittleFS.end();
}

bool LapTriggerWebServer::begin()
{
    bool isSuccess = true;

    /* Setup mDNS service. */
    if (false == MDNS.begin(HOSTNAME))
    {
        Serial.printf("%lu: Failed to start mDNS service.", millis());
        isSuccess = false;
    }
    else
    {
        /* Setup webserver. */
        m_WebServer->serveStatic("/", LittleFS, "/web/", "max-age=86400");
        m_WebServer->on("/config.html", HTTP_POST, [this]() {
            this->handleCredentials();
        });
        m_WebServer->onNotFound(
            [this]() {
                this->m_WebServer->send(404, "text/plain", "File not found.");
            });
        m_WebServer->begin();

        /* Setup websocket server. */
        m_WebSocketSrv->onEvent(
            [this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
                this->webSocketEvent(num, type, payload, length);
            });
        m_WebSocketSrv->begin();
    }

    return isSuccess;
}

bool LapTriggerWebServer::runCycle()
{
    bool isSuccess = false;

    String outputMessage = "";
    if (m_laptrigger->handleCompetition(outputMessage))
    {
        m_WebSocketSrv->broadcastTXT(outputMessage);
    }

    isSuccess = MDNS.update();
    m_WebServer->handleClient();
    m_WebSocketSrv->loop();

    return isSuccess;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

void LapTriggerWebServer::webSocketEvent(uint8_t clientId, WStype_t type, uint8_t *payload, size_t length)
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
                m_WebSocketSrv->sendTXT(clientId, "ACK");
            }
            else
            {
                m_WebSocketSrv->sendTXT(clientId, "NACK");
            }
        }
        else
        {
            m_WebSocketSrv->sendTXT(clientId, "NACK");
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

void LapTriggerWebServer::handleCredentials()
{
    if (m_WebServer->method() != HTTP_POST)
    {
        m_WebServer->send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        String ssidInput = "";
        String passwordInput = "";

        for (uint8_t i = 0; i < m_WebServer->args(); i++)
        {
            if (m_WebServer->argName(i) == "STA_SSID")
            {
                ssidInput = m_WebServer->arg(i);
            }
            else if (m_WebServer->argName(i) == "STA_PASSWORD")
            {
                passwordInput = m_WebServer->arg(i);
            }
        }

        if (ssidInput.isEmpty())
        {
            String message = "POST form was:\n";
            for (uint8_t i = 0; i < m_WebServer->args(); i++)
            {
                message += " " + m_WebServer->argName(i) + ": " + m_WebServer->arg(i) + "\n";
            }
            m_WebServer->send(200, "text/plain", message);
            Serial.println(message);
        }
        else
        {
            if (Flash::saveCredentials(ssidInput, passwordInput))
            {
                m_WebServer->send(200, "text/plain", "Credentials Accepted.\nRestarting...");
                delay(3000);
                ESP.restart();
            }
        }
    }
}

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
