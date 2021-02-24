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
* File: WLAN.h
* @brief: Implementation of Wireless Capabilities.
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
*   @brief WLAN Class for Implementation of Wireless Capabilites.
*/
class WIFI
{
public:
    WIFI();                                                           /**< Default Constructor. */
    ~WIFI();                                                          /**< Default Destructor. */
    bool begin();                                                     /**< Initialize WLAN Module. */
    bool cycle();                                                     /**< Executes WLAN Connection Check. */
    bool saveCredentials(const String &ssid, const String &password); /**< Saves new STA Credentials. */
    const IPAddress &getIPAddress(void);                              /**< Returns the IP Address of the System. */

private:
    bool connectStation();    /**< Connect to Wireless Access Point. */
    bool importCredentials(); /**< Imports Credentials from EEPROM. */
    void clearEEPROM();       /**< Deletes stored Credentials. */

    String AP_SSID;      /**< WiFi AP SSID. */
    String AP_PASSWORD;  /**< WiFi AP Password. */
    String STA_SSID;     /**< WiFi STA SSID. */
    String STA_PASSWORD; /**< WiFi STA Password. */

    IPAddress localIP;
}; /** WIFI */

#endif /*WLAN_H_*/