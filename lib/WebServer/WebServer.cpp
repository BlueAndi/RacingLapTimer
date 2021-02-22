/**************************************************************************************************
* File: WebServer.cpp
* @brief: Implementation of WebServer.h
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "WebServer.h"

/* CONSTANTS *************************************************************************************/
static const uint32_t WEBSERVER_PORT = 80; /**< Webserver port */
static const uint32_t WEBSOCKET_PORT = 81; /**< Websocket port */

/** Webserver on port for http protocol */
static ESP8266WebServer gWebServer(WEBSERVER_PORT);

/** Websocket server on port for ws protocol */
static WebSocketsServer gWebSocketSrv(WEBSOCKET_PORT);

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/** WebSocket Message Handler */
static void webSocketEvent(uint8_t clientId, WStype_t type, uint8_t *payload, size_t length);

/* VARIABLES *************************************************************************************/

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
*   Default Constructor
*/
Server::Server()
{
}

/**************************************************************************************************/

/**
*   Default Destructor
*/
Server::~Server()
{
}

/**************************************************************************************************/

/**
*   Initialization of Module
*/
bool Server::begin()
{
    bool success = true;

    /* Mount filesystem */
    if (false == LittleFS.begin())
    {
        Serial.printf("%lu: Failed to mount filesystem.\n", millis());
        success = false;
    }
    /* Setup mDNS service */
    else if (false == MDNS.begin(HOSTNAME))
    {
        Serial.printf("%lu: Failed to start mDNS service.", millis());
        success = false;
    }
    else
    {
        /* Setup webserver */
        gWebServer.serveStatic("/", LittleFS, "/web/", "max-age=86400");
        gWebServer.onNotFound(
            []() {
                gWebServer.send(404, "text/plain", "File not found.");
            });
        gWebServer.begin();

        /* Setup websocket server */
        gWebSocketSrv.onEvent(webSocketEvent);
        gWebSocketSrv.begin();
    }

    return success;
}

/**************************************************************************************************/

/**
*   Executes Loop Cycle
*/
bool Server::cycle()
{
    bool success = true;

    MDNS.update();
    gWebServer.handleClient();
    gWebSocketSrv.loop();

    return success;
}

/**************************************************************************************************/

/**
*   Sends WebSocket Message to all Clients
*/
void Server::WS_textAll(String message)
{
    gWebSocketSrv.broadcastTXT(message);
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/

/**
 * Handle websocket event.
 *
 * @param[in] clientId  Websocket client id
 * @param[in] type      Event type
 * @param[in] payload   Event payload
 * @param[in] length    Event payload length
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