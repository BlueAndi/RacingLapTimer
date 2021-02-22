/**************************************************************************************************
* File: WLAN.h
* @brief: Implementation of Wireless Capabilities
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
#ifndef WLAN_H_
#define WLAN_H_

/* INCLUDES **************************************************************************************/
#include <ESP8266WiFi.h>
/* CONSTANTS *************************************************************************************/

/* FORWARD DECLARATIONS **************************************************************************/

/* INLINE FUNCTIONS ******************************************************************************/

/* PROTOTYPES ************************************************************************************/

/**************************************************************************************************/

/**
*   WLAN Class for Implementation of Wireless Capabilites
*/
class WLAN
{
public:
    WLAN();                                             /**< Default Constructor */
    ~WLAN();                                            /**< Default Destructor */
    bool begin();                                       /**< Initialize WLAN Module */
    bool cycle();                                       /**< Executes WLAN Connection Check */
    bool saveCredentials(String ssid, String password); /**< Saves new STA Credentials */
    const IPAddress &getIPAddress(void);                /**< Returns the IP Address of the System */

private:
    bool connectStation(); /**< Connect to Wireless Access Point */

    const String AP_SSID = "RacingLapTimer"; /**< WiFi AP SSID */
    const String AP_PASSWORD = "let me in";  /**< WiFi AP Password */
    String STA_SSID = "IoT_AP";                    /**< WiFi STA SSID */
    String STA_PASSWORD = "hochschuleulm";                /**< WiFi STA Password */

    IPAddress localIP;
};

#endif /*WLAN_H_*/