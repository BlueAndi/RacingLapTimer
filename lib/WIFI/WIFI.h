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
 * @brief  Implementation of Wireless Capabilities
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

#ifndef WIFI_H_
#define WIFI_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <ESP8266WiFi.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 *  WLAN Class for Implementation of Wireless Capabilites.
 */
class WIFI
{
public:
    /**
     *  Default Constructor.
     */
    WIFI();

    /**
     *  Default Destructor.
     */
    ~WIFI();

    /**
     *  Initialize WIFI Module.
     * 
     *  @return If initialization is successful, returns True. Otherwise, false.
     */
    bool begin();

    /**
     *  Executes WIFI Connection Check.
     * 
     *  @return success.
     */
    bool runCycle();

    /**
     *  Saves new STA Credentials.
     * 
     *  @param[in] ssid SSID of host network to connect to.
     *  @param[in] password Password of host network to connect to.
     *  @return If Credentials succesfully saved, return True. Otherwise, False.
     */
    bool saveCredentials(const String &ssid, const String &password);

    /**
     *  Get the IP Address of the Syste,
     * 
     *  @return Returns the IP Address.
     */
    const IPAddress &getIPAddress(void);

private:
    /**
     *  Connect to Wireless Access Point.
     * 
     *  @return If the device is succesfully connected to the Wireless Network, 
     *  returns True. Otherwise False.
     */
    bool connectStation();

    /**
     *  Imports the Station Credentials from the EEPROM.
     * 
     *  @return If Credentials successfully imported, returns True. Otherwise, False.
     */
    bool importCredentials();

    /**
     *  Deletes stored Credentials.
     */
    void clearEEPROM();

    /** Timeout for WiFi connection. */
    const unsigned long WIFI_TIMEOUT_MS = 5000;

    /** Maximum length for saved Credentials. */
    const uint8_t CREDENTIALS_MAX_LENGTH = 50;

    /** Address of saved SSID in EEPROM. */
    const uint8_t NVM_SSID_ADDRESS = 0;

    /** Address of saved Password in EEPROM. */
    const uint8_t NVM_PASSWORD_ADDRESS = 50;

    /** True if system is connected in STA mode to a Network. */
    bool isStaAvailable = false;

    /** WiFi AP SSID. */
    String m_ApSSID;

    /** WiFi AP Password. */
    String m_ApPassword;

    /** WiFi STA SSID. */
    String m_Sta_SSID;

    /** WiFi STA Password. */
    String m_Sta_Password;

    /** Local IP of the Device */
    IPAddress m_LocalIP;

}; /** WIFI */

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

#endif /*WIFI_H_*/