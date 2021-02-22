/**************************************************************************************************
* File: WebServer.h
* @brief: Abstraction of WebServer
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
*   Abstraction Class for ESP8266 Web Server
*/
class LapTriggerWebServer
{
public:
    LapTriggerWebServer(Competition &goalLine);           /**< Default Constructor */
    ~LapTriggerWebServer();                               /**< Default Destructor */
    bool begin();                                         /**< Initialization of Module */
    bool cycle();                                         /**< Executes Loop Cycle */
    
private:
};

#endif /*WEB_SERVER_H_*/