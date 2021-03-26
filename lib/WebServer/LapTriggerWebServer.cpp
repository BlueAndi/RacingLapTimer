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
 * @brief  Abstraction of WebServer.
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

LapTriggerWebServer::LapTriggerWebServer(Competition &goalLine) :
    m_laptrigger(&goalLine),
    m_webServer(WEBSERVER_PORT),
    m_webSocketSrv(WEBSOCKET_PORT)
{
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
        m_webServer.serveStatic("/", LittleFS, "/web/", "max-age=86400");
        m_webServer.on("/config.html", HTTP_POST, [this]() {
            this->handleCredentials();
        });
        m_webServer.onNotFound(
            [this]() {
                this->m_webServer.send(404, "text/plain", "File not found.");
            });
        m_webServer.begin();

        /* Setup websocket server. */
        m_webSocketSrv.onEvent(
            [this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
                this->webSocketEvent(num, type, payload, length);
            });
        m_webSocketSrv.begin();
    }

    return isSuccess;
}

bool LapTriggerWebServer::runCycle()
{
    bool isSuccess = false;
    String outputMessage = "";
    
    if (m_laptrigger->handleCompetition(outputMessage))
    {
        m_webSocketSrv.broadcastTXT(outputMessage);
    }

    isSuccess = MDNS.update();
    m_webServer.handleClient();
    m_webSocketSrv.loop();

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
                m_webSocketSrv.sendTXT(clientId, "ACK");
            }
            else
            {
                m_webSocketSrv.sendTXT(clientId, "NACK");
            }
        }
        else if (cmd.equals("GROUPS"))
        {
            /* Client requests the number of Groups */
            uint8_t groups;
            if(true == m_laptrigger->getNumberofGroups(groups))
            {
                m_webSocketSrv.sendTXT(clientId, "EVT;GROUPS;"+ String(groups));
            }
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
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
    if (m_webServer.method() != HTTP_POST)
    {
        m_webServer.send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        String ssidInput = "";
        String passwordInput = "";

        for (uint8_t i = 0; i < m_webServer.args(); i++)
        {
            if (m_webServer.argName(i) == "STA_SSID")
            {
                ssidInput = m_webServer.arg(i);
            }
            else if (m_webServer.argName(i) == "STA_PASSWORD")
            {
                passwordInput = m_webServer.arg(i);
            }
        }

        if (ssidInput.isEmpty())
        {
            String message = "POST form was:\n";
            for (uint8_t i = 0; i < m_webServer.args(); i++)
            {
                message += " " + m_webServer.argName(i) + ": " + m_webServer.arg(i) + "\n";
            }
            m_webServer.send(200, "text/plain", message);
            Serial.println(message);
        }
        else
        {
            if (Flash::saveCredentials(ssidInput, passwordInput))
            {
                m_webServer.send(200, "text/plain", "Credentials Accepted.\nRestarting...");
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
