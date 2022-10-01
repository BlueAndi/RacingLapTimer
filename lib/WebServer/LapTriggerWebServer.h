/* MIT License
 *
 * Copyright (c) 2020-2022 Andreas Merkle <web@blue-andi.de>
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

#ifndef LAP_TRIGGER_WEB_SERVER_H_
#define LAP_TRIGGER_WEB_SERVER_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "WIFI.h"
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <WebSocketsServer.h>
#include "Competition.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 *  Abstraction Class for ESP8266 Web Server.
 */
class LapTriggerWebServer
{
public:

    /**
     *  Class Constructor.
     * 
     *  @param[in] goalLine Instance of Competition Handler.
     */
    LapTriggerWebServer(Competition &goalLine);

    /**
     *  Default Destructor.
     */
    ~LapTriggerWebServer();

    /**
     *  Initialization of Module.
     * 
     *  @return If initialization is successful, returns true. Otherwise, false.
     */
    bool begin();

    /**
     *  Executes Loop Cycle.
     * 
     *  @return If WebServer successfully handled, returns true. Otherwise, false..
     */
    bool runCycle();

private:
    /** Hostname for DNS Server. */
    const char *HOSTNAME = "laptimer";

    /** Webserver port. */
    const uint32_t WEBSERVER_PORT = 80;

    /** Websocket port. */
    const uint32_t WEBSOCKET_PORT = 81;

    /** Competition Handler Instance. */
    Competition *m_laptrigger;

    /** Webserver on port for http protocol. */
    ESP8266WebServer m_webServer;

    /** Websocket server on port for ws protocol. */
    WebSocketsServer m_webSocketSrv;

    /**
     *  Handler for websocket event.
     *
     *  @param[in] clientId  Websocket client id.
     *  @param[in] type      Event type.
     *  @param[in] payload   Event payload.
     *  @param[in] length    Event payload length.
     */
    void webSocketEvent(uint8_t clientId, WStype_t type, uint8_t *payload, size_t length);

    /** Handler for POST Request for the storage of the STA Credentials. */
    void handleCredentials();

    /**
     *  Parses incoming Web Socket Event of Type TEXT.
     * 
     *  @param[in] clientId  Websocket client id.
     *  @param[in] type      Event type.
     *  @param[in] payload   Event payload.
     *  @param[in] length    Event payload length.
     */
    void parseWSTextEvent(const uint8_t clientId, const WStype_t type, const uint8_t *payload, const size_t length);

    /**
     * Default constructor is not allowed.
     */
    LapTriggerWebServer();

    /** 
     *  An instance shall not be copied. 
     *  
     *  @param[in] server Server instance to copy.
     */
    LapTriggerWebServer(const LapTriggerWebServer &server);

    /** 
     *  An instance shall not assigned.
     *   
     *  @param[in] server Server instance to assign.
     *  @return Pointer to this instance.
     */
    LapTriggerWebServer &operator=(const LapTriggerWebServer &server);

};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* LAP_TRIGGER_WEB_SERVER_H_ */