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

/* CONSTANTS *************************************************************************************/

/* FORWARD DECLARATIONS **************************************************************************/

/* INLINE FUNCTIONS ******************************************************************************/

/* PROTOTYPES ************************************************************************************/

/**************************************************************************************************/

/**
*   Abstraction Class for ESP8266 Web Server
*/
class Server
{
public:
    Server();                        /**< Default Constructor */
    ~Server();                       /**< Default Destructor */
    bool begin();                    /**< Initialization of Module */
    bool cycle();                    /**< Executes Loop Cycle */
    void WS_textAll(String message); /**< Sends WebSocket Message to all clients */
private:
    const char *HOSTNAME = "laptimer"; /**< Hostname */
};

#endif /*WEB_SERVER_H_*/