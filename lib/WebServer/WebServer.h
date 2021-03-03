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

#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_

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
     *  @param[in,out] wireless Instance of WiFi Handler.
     *  @param[in,out] goalLine Instance of Competition Handler.
     */
    LapTriggerWebServer(WIFI &wireless, Competition &goalLine);

    /**
     *  Default Destructor.
     */
    ~LapTriggerWebServer();

    /**
     *  Initialization of Module.
     * 
     *  @return If initialization is successful, returns True. Otherwise, false.
     */
    bool begin();

    /**
     *  Executes Loop Cycle.
     * 
     *  @return If WebServer successfully handled, returns True. Otherwise, false..
     */
    bool cycle();

private:
}; /** LapTriggerWebServer */

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/

#endif /*WEB_SERVER_H_*/