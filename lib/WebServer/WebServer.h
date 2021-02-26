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
* File: WebServer.h
* @brief: Abstraction of WebServer.
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_

/* INCLUDES **************************************************************************************/
#include <WLAN.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <WebSocketsServer.h>
#include "Competition.h"

/* CONSTANTS *************************************************************************************/

/* FORWARD DECLARATIONS **************************************************************************/

/* INLINE FUNCTIONS ******************************************************************************/

/* PROTOTYPES ************************************************************************************/

/**************************************************************************************************/

/**
 *  Abstraction Class for ESP8266 Web Server.
 */
class LapTriggerWebServer
{
public:
    LapTriggerWebServer(WIFI &wireless, Competition &goalLine); /**< Default Constructor. */
    ~LapTriggerWebServer();                                     /**< Default Destructor. */
    bool begin();                                               /**< Initialization of Module. */
    bool cycle();                                               /**< Executes Loop Cycle. */

private:
}; /** LapTriggerWebServer */

#endif /*WEB_SERVER_H_*/