/* MIT License
 *
 * Copyright (c) 2020 - 2025 Andreas Merkle <web@blue-andi.de>
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
#include "Settings.h"

#include <Log.h>

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

LapTriggerWebServer::LapTriggerWebServer(Competition &goalLine) : m_laptrigger(&goalLine),
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
        LOG_ERROR("Failed to start mDNS service.");
        isSuccess = false;
    }
    else
    {
        /* Setup webserver. */
        m_webServer.serveStatic("/", LittleFS, "/web/", "max-age=86400");
        m_webServer.on("/settings.html", HTTP_POST, [this]() {
            this->handleCredentials();
        });
        m_webServer.onNotFound(
            [this]() {
                this->m_webServer.send(404, "text/plain", "File not found.");
            });
        m_webServer.begin();

        /* Setup websocket server. */
        m_webSocketSrv.onEvent(
            [this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) { // cppcheck-suppress constParameterPointer
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

void LapTriggerWebServer::webSocketEvent(uint8_t clientId, WStype_t type, const uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_ERROR:
        LOG_INFO("Ws client (%u) error.", clientId);
        break;

    case WStype_DISCONNECTED:
        LOG_INFO("Ws client (%u) disconnected.", clientId);
        break;

    case WStype_CONNECTED:
        LOG_INFO("Ws client (%u) connected.", clientId);
        break;

    case WStype_TEXT:
        parseWSTextEvent(clientId, type, payload, length);
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
            LOG_WARNING(message);
        }
        else
        {
            Settings::getInstance().setWiFiSSID(ssidInput);
            Settings::getInstance().setWiFiPassphrase(passwordInput);

            m_webServer.send(200, "text/plain", "Credentials Accepted.\nRestarting...");
            delay(3000);
            ESP.restart();
        }
    }
}

void LapTriggerWebServer::parseWSTextEvent(const uint8_t clientId, const WStype_t type, const uint8_t *payload, const size_t length)
{
    String cmd = "";
    String par = "";
    size_t index = 0;

    const char *strPayload = reinterpret_cast<const char *>(payload);

    for (index = 0; index < length; ++index)
    {
        char temp = strPayload[index];
        if (';' == temp)
        {
            index++;
            break;
        }
        else
        {
            cmd += temp;
        }
    }

    for (size_t paramIndex = index; paramIndex < length; ++paramIndex)
    {
        char temp = strPayload[paramIndex];

        if (';' == temp)
        {
            break;
        }
        else
        {
            par += temp;
        }
    }

    LOG_INFO("Ws client (%u): %s", clientId, cmd.c_str());

    if (cmd.equals("RELEASE"))
    {
        if (m_laptrigger->setReleasedState(par.toInt()))
        {
            m_webSocketSrv.sendTXT(clientId, "ACK");
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else if (cmd.equals("GET_GROUPS"))
    {
        /* Client requests the number of Groups */
        uint8_t groups = 0;

        if (true == m_laptrigger->getNumberofGroups(groups))
        {
            String outputMessage = "ACK;GET_GROUPS;" + String(groups);
            m_webSocketSrv.sendTXT(clientId, outputMessage);
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else if (cmd.equals("SET_GROUPS"))
    {
        if (m_laptrigger->setNumberofGroups(par.toInt()))
        {
            m_webSocketSrv.sendTXT(clientId, "ACK;SET_GROUPS");
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else if (cmd.equals("GET_TABLE"))
    {
        uint8_t numberOfGroups = 0;

        if (m_laptrigger->getNumberofGroups(numberOfGroups))
        {
            String outputMessage = "ACK;GET_TABLE;" + String(numberOfGroups);
            m_webSocketSrv.sendTXT(clientId, outputMessage);

            for (uint8_t currentGroup = 0; currentGroup < numberOfGroups; currentGroup++)
            {
                uint32_t groupLaptime = 0;
                String output = "";

                groupLaptime = m_laptrigger->getLaptime(currentGroup);

                output = "EVT;TABLE;";
                output += currentGroup;
                output += ';';
                output += groupLaptime;
                output += ';';

                String selectedName = "";

                if (m_laptrigger->getGroupName(currentGroup, selectedName))
                {
                    output += selectedName;
                }
                else
                {
                    output += "Group ";
                    output += (char)(currentGroup + 65);
                }

                m_webSocketSrv.sendTXT(clientId, output);
            }
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else if (cmd.equals("CLEAR"))
    {
        if (m_laptrigger->clearLaptime(par.toInt()))
        {
            String outputMessage = "ACK;CLEAR;" + par;
            m_webSocketSrv.sendTXT(clientId, outputMessage);
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else if (cmd.equals("SET_NAME"))
    {
        uint8_t selectedGroup = par.toInt();
        uint8_t namePos = par.indexOf(":");
        String selectedName = par.substring(namePos + 1);
        String outputMessage = "NACK";

        if (true == m_laptrigger->setGroupName(selectedGroup, selectedName))
        {
            outputMessage = "ACK;SET_NAME;";
            outputMessage += selectedGroup;
            outputMessage += ';';
            outputMessage += selectedName;
        }

        m_webSocketSrv.sendTXT(clientId, outputMessage);
    }
    else if (cmd.equals("GET_NAME"))
    {
        uint8_t selectedGroup = par.toInt();
        String selectedName = "";
        String outputMessage = "NACK";

        if (true == m_laptrigger->getGroupName(selectedGroup, selectedName))
        {
            outputMessage = "ACK;GET_NAME;";
            outputMessage += selectedGroup;
            outputMessage += ';';
            outputMessage += selectedName;
        }

        m_webSocketSrv.sendTXT(clientId, outputMessage);
    }
    else if (cmd.equals("CLEAR_NAME"))
    {
        if (m_laptrigger->clearName(par.toInt()))
        {
            String outputMessage = "ACK;CLEAR_NAME;" + par;
            m_webSocketSrv.sendTXT(clientId, outputMessage);
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else if(cmd.equals("REJECT_RUN"))
    {
        if (m_laptrigger->rejectRun())
        {
            String outputMessage = "ACK;REJECT_RUN";
            m_webSocketSrv.sendTXT(clientId, outputMessage);
        }
        else
        {
            m_webSocketSrv.sendTXT(clientId, "NACK");
        }
    }
    else
    {
        m_webSocketSrv.sendTXT(clientId, "NACK");
    }
}

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
